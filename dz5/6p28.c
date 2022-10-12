#include "6p28.h"

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
