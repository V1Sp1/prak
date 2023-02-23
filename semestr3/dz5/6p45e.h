#ifndef PRAC6P45E_H
#define PRAC6P45E_H
#include <stdio.h>
#include "6p42.h"

typedef struct tnode {
    int data;
    struct tnode *left;
    struct tnode *right;
} tnode;

typedef tnode *tree;

int tree_count_n_level(tree tr, int n);

void tree_print(tree tr);

void tree_add(tree *ptr, int data);

void tree_free(tree *ptr);

#endif /* PRAC6P45E_H */
