#include "6p42.h"

void queue_put(queue *pqu, void *data)
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
void *queue_get(queue *pqu)
{
    queue fst = *pqu;
    void *data;
    *pqu = (*pqu) -> next;
    data = fst -> data;
    free(fst);
    return data;
}
