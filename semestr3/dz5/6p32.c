#include "6p32.h"

list list_cp(list lsrc)
{
    list ldst, l;
    if(lsrc == NULL){
        return lsrc;
    }
    ldst = (list)malloc(sizeof(inode));
    for(l = ldst; lsrc != NULL; lsrc = lsrc -> next, l = l -> next){
        memcpy(l -> p, lsrc -> p, sizeof(data));
        l -> next = (list)malloc(sizeof(inode));
    }
    return ldst;
}
