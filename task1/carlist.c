#include "carlist.h"
#include <stdlib.h>

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
    for(pos = 0; !str[pos]; ++pos) {
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

/* search seller from left to right
 * returned NULL if don't found
 * else returned first met*/
static node *
carlist_search_seller(node *left, node *right, int fd)
{
    if((left == NULL) || (right == NULL)) {
        return NULL;
    }
    for(; left != right; left = left->next) {
        if(left->seller_fd == fd) {
            return left;
        }
    }
    return NULL;
}

node *node_init(char *brand, int num, unsigned long seller_ip, unsigned short seller_port, int seller_fd)
{
    node *tmp = malloc(sizeof(node));
    tmp->brand = brand;
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
    free(current->brand);
    free(current);
}

void carlist_add_left(carlist cr, node *current, node *elem)
{
    if(current == NULL) {
        current = cr->prev;
        if(cr->prev == NULL) {
            cr->prev = elem;
            elem->prev = NULL;
            elem->next = NULL;
            return;
        }
    }
    elem->prev = current->prev;
    elem->next = current;
    if(current->prev == NULL) {
        cr->prev = elem;
    } else {
        current->prev->next = elem;
    }
    current->prev = elem;
}

void carlist_add_right(carlist cr, node *current, node *elem)
{
    if(current == NULL) {
        current = cr->next;
        if(cr->next == NULL) {
            cr->next = elem;
            elem->prev = NULL;
            elem->next = NULL;
            return;
        }
    }
    elem->prev = current;
    elem->next = current->next;
    if(current->next == NULL) {
        cr->next = elem;
    } else {
        current->next->prev = elem;
    }
    current->next = elem;
}

/*don't free current*/
void rm_from_carlist(carlist cr, node *current)
{
    if(current->prev == NULL) {
        cr->prev = current->next;
    } else {
        current->prev->next = current->next;
    }
    if(current->next == NULL) {
        cr->next = current->prev;
    } else {
        current->next->prev = current->prev;
    }
}

void carlist_addcar(carlist cr, node *elem)
{
    node *nleft, *nright, *tmp;
    if((cr->prev == NULL) && (cr->next == NULL)) {
        carlist_add_left(cr, NULL, elem);
        carlist_add_right(cr, NULL, elem);
    }
    carlist_search(cr->prev, cr->next, elem->brand, &nleft, &nright);
    if(nleft == NULL) {
        carlist_add_left(cr, nright, elem);
        return;
    }
    if(nright == NULL) {
        carlist_add_right(cr, nleft, elem);
        return;
    }
    tmp = carlist_search_seller(nleft, nright, elem->seller_fd);
    if(tmp == NULL){
        carlist_add_left(cr, nleft, elem);
    } else {
        tmp->num += elem->num;
    }
}


node *carlist_sell_car(carlist cr, char *brand)
{
    node *nleft, *nright;
    if((cr->prev == NULL) && (cr->next == NULL)) {
        return NULL;
    }
    carlist_search(cr->prev, cr->next, brand, &nleft, &nright);
    if((nleft == NULL) || (nright == NULL)) {
        return NULL;
    } else {
        --nleft->num;
        if(nleft->num <= 0) {
            rm_from_carlist(cr, nleft);
        }
        return nleft;
    }
}


/*free carlist, don't free cr*/
void carlist_free(carlist cr)
{
    node *tmp;
    for(; cr->prev != cr->next ; cr->prev = tmp) {
        tmp = cr->prev->next;
        rm_from_carlist(cr, cr->prev);
        node_free(cr->prev);
    }
}

