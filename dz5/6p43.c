#include "6p43.h"

void stack_push(stack *pst, void *data)
{
    stack fst = (stack)malloc(sizeof(inode));
    fst -> next = *pst;
    fst -> data = data;
    (*pst) = fst;
}

/*if *pst eq NULL than mustn't work...*/
void *stack_pop(stack *pst)
{
    stack fst = *pst;
    void *data = fst -> data;
    *pst = (*pst) -> next;
    free(fst);
    return data;
}
