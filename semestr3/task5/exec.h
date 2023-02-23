#ifndef EXEC_H_TASK5
#define EXEC_H_TASK5

#include "tree.h"
#include "darr.h"

struct backlist {
    int pid;
    /*char *info;*/
    struct backlist *next;
};

typedef struct backlist *bckgrnd;

void null_bckgrnd(bckgrnd *bc);

void add_to_bckgrnd(bckgrnd *bc, int pid);

void clear_bckgrndlist(bckgrnd *bc);

void print_bckgrnd(bckgrnd bc);

int rmelem_bckgrnd(bckgrnd *bc, int pid);

int clear_bck_zombie(bckgrnd *bc, struct darr *bckgrndend);

/*PERMS to create files*/
enum {PERMS = 0666};

/*
1 if OK exec
0 if error exec
-1 if it time to exit
*/
int exec_tree(tree tr, struct darr li, bckgrnd *bc, struct darr *bckgrndend);

#endif /* EXEC_H_TASK5 */
