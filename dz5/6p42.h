#ifndef PRAC6P42_H
#define PRAC6P42_H
#include <stdlib.h>

typedef struct inode {
    void *data;
    struct inode *next;
} inode;

typedef inode *queue;

void queue_put(queue *pqu, void *data);

/*if *pqu eq NULL than mustn't work*/
void *queue_get(queue *pqu);

#endif /* PRAC6P42_H */
