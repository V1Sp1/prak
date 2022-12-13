#ifndef CHAT_LIST_H
#define CHAT_LIST_H

struct node{
    int elem;
    struct node *next;
};
/*ordened list with a hader*/
typedef struct node *lst;

void initlst(lst *pl);

void rmlst(lst *pl);

/*returnes 1 if added, 0 if exists*/
int add2lst(lst pl, int elem);

void rmelem(lst pl, int elem);

#endif /* CHAT_LIST_H */
