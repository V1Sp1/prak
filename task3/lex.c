#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *buf;       /* word buffer */
static int curbuf;      /* index symbol in buf */
static int sizebuf;     /* size of buf */
static char str[N];     /* symbol buffer */
static int curstr;      /* index symbol in str */
static int sizestr;     /* size of str */

int getsym(void){
    ++curstr;
    if(curstr >= sizestr){
        curstr = 0;
        sizestr = read(0, str, N);
    }
    if(sizestr > 0){
        return str[curstr];
    }
    return EOF;
    /*TODO: error cheching*/
}

void nullbuf(void){
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

void addsym(int c){
    if (curbuf >= sizebuf){
        sizebuf += SIZE_ADD;
        buf = realloc(buf, sizeof(char) * sizebuf);
    }
    buf[curbuf] = c;
    ++curbuf;
}

void null_list(struct listw *li){
    li->sizelist = 0;
    li->curlist = 0;
    li->list = NULL;
}

void clearlist(struct listw *li){
    int i;
    if(li->list ==  NULL){
        return;
    }
    for(i = 0; li->list[i] != NULL; ++i){
        free(li->list[i]);
    }
    free(li->list);
    null_list(li);
}

void termlist(struct listw *li){
    if(li->list == NULL){
        return;
    }
    li->sizelist = li->curlist + 1;
    li->list = realloc(li->list, li->sizelist * sizeof(char*));
    li->list[li->curlist] = NULL;
}

void addword(struct listw *li){
    sizebuf = curbuf + 1;
    buf = realloc(buf, sizebuf);
    buf[curbuf] = '\0';
    if(li->curlist >= li->sizelist){
        li->sizelist += SIZE_ADD;
        li->list = realloc(li->list, li->sizelist * sizeof(char*));
    }
    li->list[li->curlist] = buf;
    li->curlist++;
}

void printlist(struct listw *li){
    int i;
    if(li->list == NULL){
        return;
    }
    for(i = 0; i < li->sizelist - 1; ++i){
        printf("%s ", li->list[i]);
    }
    putchar('\n');
}
