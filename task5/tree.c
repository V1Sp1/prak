#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "tree.h"
#include "error.h"

#define SIZE 5

enum lex_t {isNULL, word, rbracket, lbracket, rein, reout, reappend, seq, backgrnd, and, or, conv};

enum lex_t lexanaliz(char *lex)
{
    int curs = 0;
    enum lex_t res;
    if(lex == NULL){
        return isNULL;
    }
    switch(lex[curs]){
        case '(':
            res = lbracket;
            break;
        case ')':
            res = rbracket;
            break;
        case '<':
            res = rein;
            break;
        case '>':
            res = reout;
            break;
        case ';':
            res = seq;
            break;
        case '&':
            res = backgrnd;
            break;
        case '|':
            res = conv;
            break;
        default:
            return word;
    }
    ++curs;
    switch(lex[curs]){
        case '\0':
            return res;
        case '>':
            res = reappend;
            break;
        case '&':
            res = and;
            break;
        case '|':
            res = or;
            break;
        default:
            return word;
    }
    ++curs;
    if(lex[curs] == '\0'){
        return res;
    } else {
        return word;
    }
}

int lexcheck(enum lex_t cur_t, enum lex_t prev_t)
{
    switch(cur_t){
        case isNULL:
            if((prev_t != isNULL) && (prev_t != word) && (prev_t != rbracket) && (prev_t != seq) && (prev_t != backgrnd)){
                setstatus(SYN_NOT_EXPECTED_END);
                return 0;
            }
            break;
        case word:
            if(prev_t == rbracket){
                setstatus(SYN_NOT_EXPECTED_WORD);
                return 0;
            }
            break;
        case backgrnd:
        case seq:
        case and:
        case or:
        case rein:
        case reout:
        case reappend:
            if((prev_t != word) && (prev_t != rbracket)){
                setstatus(SYN_NOT_EXPECTED_REDIR_OR_SEP);
                return 0;
            }
            break;
        case conv:
            if((prev_t != word) && (prev_t != rbracket) && (prev_t != rein) && (prev_t != reout) && (prev_t != reappend)){
                setstatus(SYN_NOT_EXPECTED_CONV);
                return 0;
            }
            break;
        case lbracket:
            if((prev_t == word) || (prev_t == rein) || (prev_t == reout) || (prev_t == reappend)){
                setstatus(SYN_NOT_EXPECTED_LBRACKET);
                return 0;
            }
            break;
       case rbracket:
            if((prev_t != word) && (prev_t != lbracket) && (prev_t != rbracket) && (prev_t != seq) && (prev_t != backgrnd)){
                setstatus(SYN_NOT_EXPECTED_RBRACKET);
                return 0;
            }
            break;
        default:
            return 0;
    }
    return 1;
}


int build_tree(tree *tr, char **li)
{
    static struct darr argcur = {.type = darr_of_pchar};
    static char **plex = NULL;
    tree cur_cmd = *tr;
    tree prev_cmd = cur_cmd;
    tree cur_conv = cur_cmd;
    enum lex_t prev_t = isNULL;
    enum lex_t cur_t;
    int correct = 1;
    int tmp;
    if(plex == NULL){
        plex = li;
        if(plex == NULL){
            return -1;
        }
    }
    null_darr(&argcur);
    while(correct){
        cur_t = lexanaliz(*plex);
        correct = lexcheck(cur_t, prev_t);
        if(!correct){
            continue;
        }
        switch(cur_t){
            case isNULL:
                plex = NULL;
                term_darr(&argcur);
                cur_cmd->argv = argcur.arr;
                null_darr(&argcur);
                if((prev_t == seq) || (prev_t == backgrnd)){
                    free(cur_cmd); /*REWRITE THIS PART*/
                    prev_cmd->next = NULL;
                }
                return 1;
            case word:
                if(prev_t == rein){
                    cur_cmd->infile = *plex;
                    break;
                } else if((prev_t == reout) || (prev_t == reappend)){
                    cur_cmd->outfile = *plex;
                    break;
                }
                for(; lexanaliz(*plex) == word; ++plex){
                    add_to_darr(&argcur, *plex);
                }
                prev_t = cur_t;
                continue;
            case rein: 
                if(cur_cmd->infile != NULL){
                    setstatus(SYN_MANY_REDIRECTIONS);
                    correct = 0;
                    continue;
                }
                break;
            case reappend:
                cur_cmd->append = 1;
            case reout:
                if(cur_cmd->outfile != NULL){
                    setstatus(SYN_MANY_REDIRECTIONS);
                    correct = 0;
                    continue;
                }
                break;
            case lbracket:
                ++plex;
                creat_tree(&cur_cmd->psubcmd);
                tmp = build_tree(&cur_cmd->psubcmd, li);
                if(tmp){
                    setstatus(SYN_EXPECTED_CLOSE_BRACKET);
                    correct = 0;
                } else if (getstatus() == SYN_MET_CLOSE_BRACKET_BEFORE_OPEN){
                    cur_t = rbracket;
                    break;
                } else {
                    correct = 0;
                }
                continue;
            case rbracket:
                term_darr(&argcur);
                cur_cmd->argv = argcur.arr;
                null_darr(&argcur);
                setstatus(SYN_MET_CLOSE_BRACKET_BEFORE_OPEN);
                return 0;
            case backgrnd:
                for(; cur_conv->next != NULL; cur_conv = cur_conv->next){
                    cur_conv->backgrnd = 1;
                }
                cur_conv->backgrnd = 1;
            case conv:
            case seq:
            case and:
            case or:
                term_darr(&argcur);
                cur_cmd->argv = argcur.arr;
                null_darr(&argcur);
                prev_cmd = cur_cmd;
                creat_tree(&cur_cmd);
                prev_cmd->next = cur_cmd;
                if(cur_t == and){
                    prev_cmd->type = AND;
                } else if(cur_t == or){
                    prev_cmd->type = OR;
                } else if(cur_t == conv){
                    prev_cmd->type = PIPE;
                } else {
                    cur_conv = cur_cmd;
                }
                break;
            default:
                correct = 0;
                continue;
        }
        ++plex;
        prev_t = cur_t;
    }
    plex = NULL;
    if(cur_cmd->argv == NULL){
        term_darr(&argcur);
        cur_cmd->argv = argcur.arr;
        null_darr(&argcur);
    }
    return 0;
}

void null_tree(tree tr)
{
    tr->argv = NULL;
    tr->infile = NULL;
    tr->outfile = NULL;
    tr->backgrnd = 0;
    tr->append = 0;
    tr->psubcmd = NULL;
    tr->next = NULL;
    tr->type = NXT;
}

void creat_tree(tree *tr)
{
    *tr = (tree)malloc(sizeof(struct cmd_inf));
    null_tree(*tr);
}

void clear_tree(tree *tr)
{
    if(*tr == NULL){
        return;
    }
    clear_tree(&((*tr)->psubcmd));
    clear_tree(&((*tr)->next));
    if((*tr)->argv != NULL){
        free((*tr)->argv);
    }
    free(*tr);
    *tr = NULL;
}

void make_shift(int n){
    while(n--){
        putc(' ', stderr);
    }
}

void print_argv(char **p, int shift){
    char **q=p;
    if(p != NULL){
        while(*p != NULL){
             make_shift(shift);
             fprintf(stderr, "argv[%d]=%s\n", (int)(p-q), *p);
             p++;
        }
    }
}

void print_tree(tree tr, int shift){
    char **p;
    if(tr == NULL)
        return;
    p = tr->argv;
    if(p != NULL)
        print_argv(p, shift);
    else{
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if(tr->infile == NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", tr->infile);
    make_shift(shift);
    if(tr->outfile == NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", tr->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", tr->append);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", tr->backgrnd);
    make_shift(shift);
    fprintf(stderr, "type=%s\n", tr->type == NXT ? "NXT" : tr->type == OR ? "OR" : tr->type == AND ? "AND" : "PIPE" );
    make_shift(shift);
    if(tr->psubcmd == NULL)
        fprintf(stderr, "psubcmd=NULL \n");
    else{
        fprintf(stderr, "psubcmd---> \n");
        print_tree(tr->psubcmd, shift+5);
    }
    make_shift(shift);
    if(tr->next == NULL)
        fprintf(stderr, "next=NULL \n");
    else{
        fprintf(stderr, "next---> \n");
        print_tree(tr->next, shift + 5);
    }
}
