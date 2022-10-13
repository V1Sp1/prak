#ifndef PRAC6P32_H
#define PRAC6P32_H
#include <stdlib.h>
#include <string.h>

typedef struct data {
    double f;
    char *s[2];
} data;

typedef struct inode {
    struct data *p;
    struct inode *next;
} inode;

typedef inode *list;

list list_cp(list lsrc);

#endif /* PRAC6P32_H */
