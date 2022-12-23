#ifndef TREE_H_TASK5
#define TREE_H_TASK5

#include "darr.h"

enum type_of_next {NXT, PIPE, AND, OR};

struct cmd_inf {
    char **argv;
    char *infile;
    char *outfile;
    int backgrnd;
    int append;
    struct cmd_inf *psubcmd;
    struct cmd_inf *next;
    enum type_of_next type;
};

typedef struct cmd_inf *tree;

void null_tree(tree tr);
void creat_tree(tree *tr);
int build_tree(tree *tr, char **li);
void clear_tree(tree *tr);
void print_tree(tree tr, int shift);

#endif /* TREE_H_TASK5 */
