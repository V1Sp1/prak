#ifndef PRAC6P28_H
#define PRAC6P28_H
#include <stdlib.h>

typedef struct inode {
    double *data;
    struct inode *next;
} inode;

typedef inode *list;


void swap_fst_lst(list *pl);

void rm_first_eq_h(list l, double *data);

void rm_first_eq_no_h(list *pl, double *data);

void rm_all_eq_h(list l, double *data);

void rm_all_eq_no_h(list *pl, double *data);

void cp_all_eq(list l, double *data);

#endif /* PRAC6P28_H */
