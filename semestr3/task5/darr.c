#include "darr.h"
#include <stdlib.h>

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
    switch(dar->type){
        case darr_of_char:
            dar->allocsize = dar->cursize + 1;
            dar->arr = realloc(dar->arr, sizeof(char) * dar->allocsize);
            ((char*)dar->arr)[dar->cursize] = '\0';
            break;
        case darr_of_pchar:
            if(dar->arr == NULL){
                return 0;
            }
            dar->allocsize = dar->cursize + 1;
            dar->arr = realloc(dar->arr, sizeof(char*) * dar->allocsize);
            ((char**)dar->arr)[dar->cursize] = NULL;
            break;
        default:
            return -1;
    }
    return 1;
}
