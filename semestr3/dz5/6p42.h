#ifndef PRAC6P42_H
#define PRAC6P42_H
#include <stdlib.h>

typedef struct inode {
    void *data;
    struct inode *next;
} inode;

typedef inode *queue;

void queue_put(queue *pqu, void *data);

void *queue_get(queue *pqu);

void queue_free(queue *pqu);

#endif /* PRAC6P42_H */
