#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *buf;       /* word buffer */
static int curbuf;      /* index symbol in buf */
static int sizebuf;     /* size of buf */

int getsym(void){
    static char str[N];     /* symbol buffer */
    static int curstr = 0;      /* index symbol in str */
    static int sizestr = 0;     /* size of str */

#if USE_FSCANF_IN_GETSYM
#define N_TO_STR2(X) #X
#define N_TO_STR(X) N_TO_STR2(X)
    if(curstr == sizebuf){
        curstr = 0;
        sizestr = 0;
        if(fscanf(stdin, "%" N_TO_STR(N) "[^\n]%n", str, &sizestr) == EOF){
            return EOF;
        }
        if(sizestr != 10){
            str[sizestr] = '\n';
            fscanf(stdin, "%*c");
            ++sizestr;
        }
    }
    ++curstr;
    return str[curstr - 1];
#undef N_TO_STR
#undef N_TO_STR2
#else
    ++curstr;
    if(curstr >= sizestr){
        curstr = 0;
        sizestr = read(0, str, N);
    }
    if(sizestr > 0){
        return str[curstr];
    }
    return EOF;
#endif
}

void nullbuf(void)
{
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

void clearbuf(void)
{
    if(buf != NULL){
        free(buf);
    }
    nullbuf();
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
