#ifndef PRAC6P286P40_H
#define PRAC6P286P40_H
#include <stdlib.h>

typedef struct inode {
    double *data;
    struct inode *next;
} inode;

typedef inode *list;

/*6p28*/
void swap_fst_lst(list *pl);

void rm_first_eq_h(list l, double *data);

void rm_first_eq_no_h(list *pl, double *data);

void rm_all_eq_h(list l, double *data);

void rm_all_eq_no_h(list *pl, double *data);

void cp_all_eq(list l, double *data);

/*6p40*/
list cp_xor(list l1, list l2);

#endif /* PRAC6P286P40_H */
