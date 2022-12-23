#include "tree.h"
#include <stdlib.h>


int tree_search(tree ptr, int elem)
{
    if(ptr == NULL){
        return 0;
    }
    if(ptr->elem == elem){
        return 1;
    } else if(ptr->elem > elem){
        return tree_search(ptr->left, elem);
    } else {
        return tree_search(ptr->right, elem);
    }
}

void tree_add(tree *ptr, int elem)
{
    if(*ptr == NULL){
        *ptr = (tree)malloc(sizeof(tnode));
        (*ptr)->elem = elem;
        (*ptr)->left = NULL;
        (*ptr)->right = NULL;
        return;
    }
    if((*ptr)->elem == elem){
        return;
    }
    if((*ptr)->elem > elem){
        tree_add(&((*ptr)->left), elem);
    } else {
        tree_add(&((*ptr)->right), elem);
    }
}

void tree_free(tree *ptr)
{
    if(*ptr == NULL){
        return;
    }
    tree_free(&((*ptr)->left));
    tree_free(&((*ptr)->right));
    free(*ptr);
    *ptr = NULL;
}
