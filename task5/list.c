#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


/*get symbol*/
int getsym(void)
{
    static char str[N];         /* symbol buffer */
    static int curstr = 0;      /* index symbol in str */
    static int sizestr = 0;     /* size of str */
    ++curstr;
    if(curstr >= sizestr){
        curstr = 0;
        sizestr = read(0, str, N);
    }
    if(sizestr > 0){
        return str[curstr];
    }
    return EOF;
}

/*null darray*/
void null_darr(struct darr *dar)
{
    dar->allocsize = 0;
    dar->cursize = 0;
    dar->arr = NULL;
}

/*clear dar*/
int clear_darr(struct darr *dar)
{
    if(dar->type == none){
        return -1;
    }
    if(dar->arr == NULL){
        return 0;
    }
    if(dar->type == darr_of_pchar){
        int i;
        for(i = 0; ((char**)dar->arr)[i] != NULL; ++i){
            free(((char**)dar->arr)[i]);
        }
    }
    free(dar->arr);
    null_darr(dar);
    return 1;
}

/*add elem with type dar->type to dar*/
int add_to_darr(struct darr *dar, void *elem)
{
    switch(dar->type){
        case darr_of_char:
            if(dar->cursize >= dar->allocsize){
                dar->allocsize += SIZE_ADD;
                dar->arr = realloc(dar->arr, sizeof(char) * dar->allocsize);
            }
            ((char*)dar->arr)[dar->cursize] = *(char*)elem;
            break;
        case darr_of_pchar:
            if(dar->cursize >= dar->allocsize){
                dar->allocsize += SIZE_ADD;
                dar->arr = realloc(dar->arr, sizeof(char*) * dar->allocsize);
            }
            ((char**)dar->arr)[dar->cursize] = (char*)elem;
            break;
        default:
            return -1;
    }
    dar->cursize++;
    return 1;
}

/*terminate darray*/
int term_darr(struct darr *dar)
{
    dar->allocsize = dar->cursize + 1;
    switch(dar->type){
        case darr_of_char:
            dar->arr = realloc(dar->arr, sizeof(char) * dar->allocsize);
            ((char*)dar->arr)[dar->cursize] = '\0';
            break;
        case darr_of_pchar:
            dar->arr = realloc(dar->arr, sizeof(char*) * dar->allocsize);
            ((char**)dar->arr)[dar->cursize] = NULL;
            break;
        default:
            return -1;
    }
    return 1;
}



/*null word buffer*/
void nullbuf(struct wbuf *wb)
{
    wb->sizebuf = 0;
    wb->curbuf = 0;
    wb->buf = NULL;
}

/*add symbol to word buffer*/
void addsym(struct wbuf *wb, int c)
{
    if(wb->curbuf >= wb->sizebuf){
        wb->sizebuf += SIZE_ADD;
        wb->buf = realloc(wb->buf, sizeof(char) * wb->sizebuf);
    }
    wb->buf[wb->curbuf] = c;
    wb->curbuf++;
}

/*terminate word buffer by 0*/
void termbuf(struct wbuf *wb)
{
    wb->sizebuf = wb->curbuf + 1;
    wb->buf = realloc(wb->buf, sizeof(char) * wb->sizebuf);
    wb->buf[wb->curbuf] = '\0';
}

void null_list(struct listw *li)
{
    li->sizelist = 0;
    li->curlist = 0;
    li->list = NULL;
}

void clearlist(struct listw *li)
{
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

/*terminate list of words by NULL*/
void termlist(struct listw *li)
{
    if(li->list == NULL){
        return;
    }
    li->sizelist = li->curlist + 1;
    li->list = realloc(li->list, li->sizelist * sizeof(char*));
    li->list[li->curlist] = NULL;
}

/*add word to list of words*/
void addword(struct listw *li, char *word)
{
    if(li->curlist >= li->sizelist){
        li->sizelist += SIZE_ADD;
        li->list = realloc(li->list, li->sizelist * sizeof(char*));
    }
    li->list[li->curlist] = word;
    li->curlist++;
}
