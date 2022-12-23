#include "lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "error.h"

/* read N bytes*/
#ifndef NREAD
#define NREAD 10
#endif

/*get symbol*/
int getsym(void)
{
    static char str[NREAD];     /* symbol buffer */
    static int curstr = 0;      /* index symbol in str */
    static int sizestr = 0;     /* size of str */
    ++curstr;
    if(curstr >= sizestr){
        curstr = 0;
        sizestr = read(0, str, NREAD);
    }
    if(sizestr > 0){
        return str[curstr];
    }
    return EOF;
}

enum symt {sep, dsep, quotes, blank, comm, newline, end, symset};

enum symt symanaliz(int c)
{
    switch(c){
        case ';':
        case '<':
        case '(':
        case ')':
            return sep;
        case '|':
        case '&':
        case '>':
            return dsep;
        case '\'':
        case '"':
            return quotes;
        case ' ':
        case '\t':
            return blank;
        case '#':
            return comm;
        case '\n':
            return newline;
        case EOF:
            return end;
        default:
            return symset;
    }
}

int getlinesem(struct darr *li, struct darr *wb)
{
    if((li->type != darr_of_pchar) || (wb->type != darr_of_char)){
        return -1;
    }
    int c = getsym();
    for(;;){
        if(symanaliz(c) == quotes){
            int fstquote = c;
            c = getsym();
            null_darr(wb);
            while(c != fstquote){
                switch(c){
                    case EOF:
                        term_darr(wb);
                        add_to_darr(li, wb->arr);
                        term_darr(li);
                        setstatus(LEX_LOST_QUOTE);
                        return -1;
                    case '\n':
                        putchar('>');
                        fflush(stdout);
                        add_to_darr(wb, &c);
                        break;
                    case '\\':
                        if(fstquote == '"'){
                            c = getsym();
                            if((c != '\"') && (c != '\\')){
                                int d = '\\';
                                add_to_darr(wb, &d);
                                continue;                               
                            }
                        }
                    default:
                        add_to_darr(wb, &c);
                        break;
                }
                c = getsym();
            }
            term_darr(wb);
            add_to_darr(li, wb->arr);
            c = getsym();
        }
        switch(c){
            case ' ':
            case '\t':
                c = getsym();
                continue;
            case EOF:
                term_darr(li);
                return 0; /*EOF*/
            case '#':
            case '\n':
                term_darr(li);
                return 1; /*newline*/
            case '\'':
            case '"':
                continue;
            default:
                null_darr(wb);
                add_to_darr(wb, &c);
                enum symt t = symanaliz(c);
                if((t == sep) || (t == dsep)){
                    int tmpc = c;
                    c = getsym();
                    if((symanaliz(c) == dsep) && (c == tmpc)){
                        add_to_darr(wb, &c);
                        c = getsym();
                    }
                    term_darr(wb);
                    add_to_darr(li, wb->arr);
                    continue;
                }
                c = getsym();
                break;
        }
        while(symanaliz(c) == symset){
            add_to_darr(wb, &c);
            c = getsym();
        }
        term_darr(wb);
        add_to_darr(li, wb->arr);
        null_darr(wb);
    }
}

int print_darr(struct darr *dar, int sep, int end)
{
    int i;
    if(dar->arr == NULL){
        return 0;;
    }
    switch(dar->type){
        case darr_of_char:
            printf("%s%c", (char*)dar->arr, sep);
            break;
        case darr_of_pchar:
            for(i = 0; i < dar->cursize; ++i){
                printf("%s%c", ((char**)dar->arr)[i], sep);
            }
            break;
        default:
            return -1;
    }
    if(end){
        putchar(end);
    }
    fflush(stdout);
    return 1;
}

int addval(struct darr *li)
{
    int i, tmp;
    char *buf;
    if(li->type != darr_of_pchar){
        return -1;
    }
    if(li->arr == NULL){
        return 0;
    }
    for(i = 0; i < li->cursize; ++i){
        if(((char**)li->arr)[i][0] == '$'){
            if(strcmp(((char**)li->arr)[i], "$HOME") == 0){
                buf = getenv("HOME");
                ((char**)li->arr)[i] = realloc(((char**)li->arr)[i], sizeof(char) * (strlen(buf) + 1));
                strcpy(((char**)li->arr)[i], buf);
                continue;
            }
            if(strcmp(((char**)li->arr)[i], "$PATH") == 0){
                buf = getenv("PATH");
                ((char**)li->arr)[i] = realloc(((char**)li->arr)[i], sizeof(char) * (strlen(buf) + 1));
                strcpy(((char**)li->arr)[i], buf);
                continue;
            }
            if(strcmp(((char**)li->arr)[i], "$SHELL") == 0){
                continue;
            }
            if(strcmp(((char**)li->arr)[i], "$USER") == 0){
                tmp = sysconf(_SC_LOGIN_NAME_MAX);
                ((char**)li->arr)[i] = realloc(((char**)li->arr)[i], sizeof(char) * (tmp));
                strcpy(((char**)li->arr)[i], getlogin());
                continue;
            }
            if(strcmp(((char**)li->arr)[i], "$EUID") == 0){
                tmp = floor(log10(geteuid())) + 1 + 1;
                ((char**)li->arr)[i] = realloc(((char**)li->arr)[i], sizeof(char) * (tmp));
                sprintf(((char**)li->arr)[i], "%d", geteuid());
                continue;
            }
        }
    }
    return 1;
}
