#include "6p28and6p40.h"

void swap_fst_lst(list *pl)
{
    list last, l = *pl;
    if((l == NULL) || (l -> next == NULL)){
        return;
    }
    while(l -> next -> next != NULL){
        l = l -> next;
    }
    last = l -> next;
    last -> next = (*pl) -> next;
    (*pl) -> next = NULL;
    l -> next = *pl;
    *pl = last;
}

void rm_first_eq_h(list l, double *data)
{
    list buf;
    for(; l -> next != NULL; l = l -> next){
        if(l -> next -> data == data){
            buf = l -> next;
            l -> next = buf -> next;
            free(buf);
            break;
        }
    }
}

void rm_first_eq_no_h(list *pl, double *data)
{
    list buf;
    for(; (*pl) != NULL; pl = &((*pl) -> next)){
        if((*pl) -> data == data){
            buf = *pl;
            *pl = buf -> next;
            free(buf);
            break;
        }
    }
}

void rm_all_eq_h(list l, double *data)
{
    list buf;
    for(; l -> next != NULL; l = l -> next){
        if(l -> next -> data == data){
            buf = l;
            l -> next = buf -> next;
            free(buf);
        }
    }
}

void rm_all_eq_no_h(list *pl, double *data)
{
    list buf;
    while((*pl) != NULL){
        if((*pl) -> data == data){
            buf = *pl;
            *pl = (*pl) -> next;
            free(buf);
        } else {
            pl = &((*pl) -> next);
        }
    }
}

void cp_all_eq(list l, double *data)
{
    list buf;
    for(; l != NULL; l = l -> next){
        if(l -> data == data){
            buf = (list)malloc(sizeof(inode));
            buf -> data = data;
            buf -> next = l;
            *l = *buf;
            l = l -> next;
        }
    }
}

/*6p40*/
list cp_xor(list l1, list l2)
{
    list l3, buf;
    if(l1 == NULL){
        buf = l1;
        l1 = l2;
        l2 = buf;
    }
    if(l1 == NULL){
        return l1;
    }
    l3 = (list)malloc(sizeof(inode));
    buf = l3;
    for(; l1 -> next != NULL; l1 = l1 -> next){
        buf -> data = l1 -> data;
        buf = buf -> next;
        buf = (list)malloc(sizeof(inode));
    }
    buf -> data = l1 -> data;
    buf -> next = NULL;
    for(; l2 != NULL; l2 = l2 -> next){
        buf = l3;
        for(; buf != NULL; buf = buf -> next){
            if(buf -> data == l2 -> data){
                break;
            }
        }
        if(buf == NULL){
            buf = (list)malloc(sizeof(inode));
            buf -> data = l2 -> data;
        }
    }
    return l3;
}
