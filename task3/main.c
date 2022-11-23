#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"

enum lex_states {
    lex_start,
    lex_word,
    lex_newline,
    lex_finish,
    /*TODO: lex_error*/
};

struct lex {
    int c;
    struct listw list;
    enum lex_states state;
    /*TODO: int isinquotes if it is 1 space add to word*/
};

void init_lex(struct lex *le)
{
    le->state = lex_start;
    null_list(&le->list);
}

int issep(int c){
    return c == '|' ||
           c == '&' ||
           c == ';' ||
           c == '<' ||
           c == '>' ||
           c == '(' ||
           c == ')';
}

int isdsep(int c)
{
    return (c == '|') ||
           (c == '&') ||
           (c == '>');
}

int symset(int c){
    return c != '\n' &&
           c != '\t' &&
           c != ' ' &&
           !issep(c) &&
           c != EOF;
}

void insertion_sort(struct listw *li)
{
    int i, j;
    char *buf;
    for(i = 1; i < li->sizelist - 1; i++){
        buf = li->list[i];
        for(j = i; (j > 0) && (strcmp(li->list[j-1], buf) > 0); j--){
            li->list[j] = li->list[j - 1];
        }
        li->list[j] = buf;
    }
}

void lex_step(struct lex *le){
    switch(le->state){
        case lex_start:
            if((le->c == ' ') || (le->c == '\t')){
                le->c = getsym();
            } else if(le->c == EOF){
                termlist(&le->list);
                printlist(&le->list);
                insertion_sort(&le->list);
                printlist(&le->list);
                clearlist(&le->list);
                le->state = lex_finish;
            } else if(le->c == '\n'){
                termlist(&le->list);
                printlist(&le->list);
                insertion_sort(&le->list);
                printlist(&le->list);
                printf("task3:$");
                fflush(stdout);
                le->state = lex_newline;
                le->c = getsym();
            } else {
                nullbuf();
                addsym(le->c);
                if(issep(le->c)){
                    int tmpc = le->c;
                    le->c = getsym();
                    if(isdsep(le->c) && (le->c == tmpc)){
                        addsym(le->c);
                        le->c = getsym();
                    }
                    le->state = lex_start;
                    addword(&le->list);
                } else {
                    le->c = getsym();
                    le->state = lex_word;
                }
            }
            break;
        case lex_word:
            if(symset(le->c)){
                addsym(le->c);
                le->c = getsym();
            } else {
                le->state = lex_start;
                addword(&le->list);
            }
        break;
        case lex_newline:
            clearlist(&le->list);
            le->state = lex_start;
            break;
        case lex_finish:
            exit(0);
            break;
        }
}

int main(void){
    struct lex le;
    init_lex(&le);
    printf("task3:$");
    fflush(stdout);
    le.c = getsym();
    while(1){
        lex_step(&le);
    }
    return 0;
}
