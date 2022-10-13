#ifndef PRAC6P42_H
#define PRAC6P42_H
#include <stdlib.h>

typedef struct inode {
    double data;
    struct inode *next;
} inode;

typedef inode *queue;

void queue_put(queue *pqu, double data);

/*if *pqu eq NULL than mustn't work*/
double queue_get(queue *pqu);

void queue_print(queue qu);

void queue_throw(queue qu, void (*fp)(double));

#endif /* PRAC6P42_H */
