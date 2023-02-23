#include <stdlib.h>
#include "list.h"
#include <stdio.h>

int add2lst(lst pl, int elem)
{
    lst tmp = (lst)malloc(sizeof(struct node));
    tmp->elem = elem;
    for(; pl->next != NULL; pl = pl->next){
        if(pl->next->elem == elem){
            free(tmp);
            return 0;
        }
        if(pl->next->elem > elem){
            break;
        }
    }
    tmp->next = pl->next;
    pl->next = tmp;
    return 1;
}

void rmelem(lst pl, int elem)
{
    lst buf;
    for(; pl->next != NULL; pl = pl->next){
        if(pl->next->elem == elem){
            buf = pl->next;
            pl->next = buf->next;
            free(buf);
            break;
        }
    }
}

void initlst(lst *pl)
{
    *pl = (lst)malloc(sizeof(struct node));
    (*pl)->next = NULL;
}

void rmlst(lst *pl)
{
    lst tmp;
    while(*pl != NULL){
        tmp = *pl;
        *pl = tmp->next;
        free(tmp);
    }
}
