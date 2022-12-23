#include <stdio.h>
#include "list.h"

void printlst(lst l)
{
    l = l->next;
    for(; l != NULL; l = l->next){
        printf("%d ", l->elem);
    }
    putchar('\n');
}

int main(void)
{
    lst l;
    initlst(&l);
    add2lst(l, 3);
    printlst(l);
    add2lst(l, 7);
    printlst(l);
    add2lst(l, 3);
    printlst(l);
    add2lst(l, 90);
    printlst(l);
    add2lst(l, -21);
    printlst(l);
    add2lst(l, 78);
    printlst(l);
    add2lst(l, 8);
    printlst(l);
    rmelem(l, 5);
    printlst(l);
    rmelem(l, 90);
    printlst(l);
    rmelem(l, 90);
    printlst(l);
    rmelem(l, 7);
    printlst(l);
    rmlst(&l);
    return 0;
}
