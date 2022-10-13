#include "6p42.h"

void queue_put(queue *pqu, double data)
{
    queue lst = (queue)malloc(sizeof(inode));
    lst -> next = NULL;
    lst -> data = data;
    if(*pqu == NULL){
        *pqu = lst;
        return;
    }
    while((*pqu) -> next != NULL){
        pqu = &((*pqu) -> next);
    }
    (*pqu) -> next = lst;
}

/*if *pqu eq NULL than mustn't work...*/
double queue_get(queue *pqu)
{
    queue fst = *pqu;
    double data;
    *pqu = (*pqu) -> next;
    data = fst -> data;
    free(fst);
    return data;
}

void queue_throw(queue qu, void (*fp)(double))
{
    for(; qu != NULL; qu = qu -> next){
        fp(qu -> data);
    }
}
