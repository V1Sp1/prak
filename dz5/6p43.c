#include "6p43.h"

void stack_push(stack *pst, void *data)
{
    if(data == NULL){
        return;
    }
    stack fst = (stack)malloc(sizeof(inode));
    fst -> next = *pst;
    fst -> data = data;
    (*pst) = fst;
}

void *stack_pop(stack *pst)
{
    if(*pst == NULL){
        return NULL;
    }
    stack fst = *pst;
    void *data = fst -> data;
    *pst = (*pst) -> next;
    free(fst);
    return data;
}

void stack_free(stack *pst)
{
    stack st;
    for(st = *pst; st != NULL; st = *pst){
        *pst = st -> next
        free(st);
    }
}
