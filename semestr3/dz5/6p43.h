#ifndef PRAC6P43_H
#define PRAC6P43_H
#include <stdlib.h>

typedef struct inode {
    void *data;
    struct inode *next;
} inode;

typedef inode *stack;

void stack_push(stack *pst, void *data);

void *stack_pop(stack *pst);

void stack_free(stack *pst);

#endif /* PRAC6P43_H */
