#ifndef MYTREE_H
#define MYTREE_H

typedef struct tnode {
    int elem;
    struct tnode *left;
    struct tnode *right;
} tnode;

typedef tnode *tree;

int tree_search(tree tr, int elem);

void tree_add(tree *ptr, int data);

void tree_free(tree *ptr);

#endif /* MYTREE_H */
