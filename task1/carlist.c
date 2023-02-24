#include "carlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* search str from left to right
 * returned nleft and nright
 * if nleft==NULL str is before nright
 * if nright==NULL str is after nleft
 * if (nleft!=NULL)&&(nright!=NULL) str is between nleft and nright
 * else error ???*/
static void carlist_search(node *left, node *right, char *str, node **nleft, node **nright)
{
    int pos;
    char cur, tmp;
    if(str == NULL) {
        *nleft = NULL;
        *nright = NULL;
        return;
    }
    for(pos = 0; str[pos]; ++pos) {
        cur = str[pos];
        for(; left != NULL; left = left->next) {
            tmp = left->brand[pos];
            if(tmp > cur){
                *nleft = NULL;
                *nright = left;
                return;
            } else if(tmp == cur) {
                break;
            }
        }
        for(; right != NULL; right = right->prev) {
            tmp = right->brand[pos];
            if(tmp < cur){
                *nleft = right;
                *nright = NULL;
                return;
            } else if(tmp == cur) {
                break;
            }
        }
    }
    if((left == NULL) || (right == NULL)) {
        *nleft = NULL;
        *nright = NULL;
        return;
    }
    if(!left->brand[pos]) {
        *nleft = left;
        *nright = right;
    } else {
        *nleft = NULL;
        *nright = left;
    }
}

node *node_init(const char *brand, int num, unsigned long seller_ip, unsigned short seller_port, int seller_fd)
{
    node *tmp = malloc(sizeof(node));
    if(brand != NULL) {
        int len = strlen(brand) + 1;
        tmp->brand = malloc(sizeof(char) * len);
        strcpy(tmp->brand, brand);
    } else {
        tmp->brand = NULL;
    }
    tmp->num = num;
    tmp->seller_ip = seller_ip;
    tmp->seller_port = seller_port;
    tmp->seller_fd = seller_fd;
    tmp->prev = NULL;
    tmp->next = NULL;
    return tmp;
}

void node_free(node *current)
{
    if(current->brand != NULL) {
        free(current->brand);
    }
    free(current);
}

/* add car to the left of current
 * if current is NULL add to the left end*/
void carlist_add_left(node *carlist, node *current, node *elem)
{
    if(current == NULL) {
        current = carlist->prev;
        if(carlist->prev == NULL) {
            carlist->prev = elem;
            elem->prev = NULL;
            elem->next = NULL;
            return;
        }
    }
    elem->prev = current->prev;
    elem->next = current;
    if(current->prev == NULL) {
        carlist->prev = elem;
    } else {
        current->prev->next = elem;
    }
    current->prev = elem;
}

/* add car to the right of current
 * if current is NULL add to the right end*/
static void carlist_add_right(node *carlist, node *current, node *elem)
{
    if(current == NULL) {
        current = carlist->next;
        if(carlist->next == NULL) {
            carlist->next = elem;
            elem->prev = NULL;
            elem->next = NULL;
            return;
        }
    }
    elem->prev = current;
    elem->next = current->next;
    if(current->next == NULL) {
        carlist->next = elem;
    } else {
        current->next->prev = elem;
    }
    current->next = elem;
}

void carlist_addcar(node *carlist, node *elem)
{
    node *nleft, *nright;
    if((carlist->prev == NULL) && (carlist->next == NULL)) {
        carlist_add_left(carlist, NULL, elem);
        carlist_add_right(carlist, NULL, elem);
        return;
    }
    carlist_search(carlist->prev, carlist->next, elem->brand, &nleft, &nright);
    if(nleft == NULL) {
        carlist_add_left(carlist, nright, elem);
        return;
    }
    if(nright == NULL) {
        carlist_add_right(carlist, nleft, elem);
        return;
    }
    /*search seller*/
    for(; nleft != nright->next; nleft = nleft->next) {
        if(nleft->seller_fd > elem->seller_fd){
            carlist_add_left(carlist, nleft, elem);
            return;
        } else if(nleft->seller_fd == elem->seller_fd) {
            nleft->num += elem->num;
            node_free(elem);
            return;
        }
    }
    carlist_add_left(carlist, nleft, elem);
}


/*don't free current*/
void rm_from_carlist(node *carlist, node *current)
{
    if(current->prev == NULL) {
        carlist->prev = current->next;
    } else {
        current->prev->next = current->next;
    }
    if(current->next == NULL) {
        carlist->next = current->prev;
    } else {
        current->next->prev = current->prev;
    }
}

node *carlist_sell_car(node *carlist, char *brand)
{
    node *nleft, *nright;
    if((carlist->prev == NULL) && (carlist->next == NULL)) {
        return NULL;
    }
    carlist_search(carlist->prev, carlist->next, brand, &nleft, &nright);
    if((nleft == NULL) || (nright == NULL)) {
        return NULL;
    } else {
        --nleft->num;
        if(nleft->num <= 0) {
            rm_from_carlist(carlist, nleft);
        }
        return nleft;
    }
}

void carlist_print(node carlist)
{
    node *tmp;
    printf("==========================================\n");
    for(tmp = carlist.prev; tmp != NULL; tmp = tmp->next) {
        printf("brand:%-6s|num:%2d|fd:%2d\n", tmp->brand, tmp->num, tmp->seller_fd);
    }
}

/*free carlist, don't free cr*/
void carlist_free(node *carlist)
{
    node *tmp;
    for(tmp = carlist->prev; tmp != NULL; tmp = carlist->prev) {
        carlist->prev = tmp->next;
        node_free(tmp);
    }
}

