#include "6p42.h"

void queue_put(queue *pqu, void *data)
{
    queue lst;
    if(data == NULL){
        return;
    }
    lst = (queue)malloc(sizeof(inode));
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
    queue fst = *pqu;
    void *data;
    if(*pqu == NULL){
        return NULL;
    }
    data = fst -> data;
    *pqu = (*pqu) -> next;
    free(fst);
    return data;
}

void queue_free(queue *pqu)
{
    queue qu;
    for(qu = *pqu; qu != NULL; qu = *pqu){
        *pqu = qu -> next;
        free(qu);
    }
}
