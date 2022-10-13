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

void *queue_get(queue *pqu)
{
    if(*pqu == NULL){
        return NULL;
    }
    queue fst = *pqu;
    void *data = fst -> data;
    *pqu = (*pqu) -> next;
    free(fst);
    return data;
}
