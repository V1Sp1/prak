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
static void carlist_search(struct node *left, struct node *right, const char *str,
        struct node **nleft, struct node **nright)
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

struct node *node_init(const char *brand, int num, struct sess_addr *seller)
{
    struct node *tmp = malloc(sizeof(struct node));
    if(brand != NULL) {
        int len = strlen(brand) + 1;
        tmp->brand = malloc(sizeof(char) * len);
        strcpy(tmp->brand, brand);
    } else {
        tmp->brand = NULL;
    }
    tmp->num = num;
    if(seller != NULL) {
        tmp->seller = *seller;
    }
    tmp->prev = NULL;
    tmp->next = NULL;
    return tmp;
}

void node_free(struct node *current)
{
    if(current->brand != NULL) {
        free(current->brand);
    }
    free(current);
}

/* add car to the left of current
 * if current is NULL add to the left end*/
static void carlist_add_left(struct node *carlist, struct node *current, struct node *elem)
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
static void carlist_add_right(struct node *carlist, struct node *current, struct node *elem)
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

void carlist_addcar(struct node *carlist, struct node *elem)
{
    struct node *nleft, *nright;
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
        if(nleft->seller.fd > elem->seller.fd){
            carlist_add_left(carlist, nleft, elem);
            return;
        } else if(nleft->seller.fd == elem->seller.fd) {
            nleft->num += elem->num;
            node_free(elem);
            return;
        }
    }
    carlist_add_left(carlist, nleft, elem);
}


/*don't free current*/
static void rm_from_carlist(struct node *carlist, struct node *current)
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

struct sess_addr *carlist_sell_car(struct node *carlist, const char *brand)
{
    struct node *nleft, *nright;
    if((carlist->prev == NULL) && (carlist->next == NULL)) {
        return NULL;
    }
    carlist_search(carlist->prev, carlist->next, brand, &nleft, &nright);
    if((nleft == NULL) || (nright == NULL)) {
        return NULL;
    } else {
        struct sess_addr *sel = malloc(sizeof(struct sess_addr));
        *sel = nleft->seller;
        --nleft->num;
        if(nleft->num <= 0) {
            rm_from_carlist(carlist, nleft);
            node_free(nleft);
        }
        return sel;
    }
}

/*replace by carlist_save*/
void carlist_print(struct node carlist)
{
    struct node *tmp;
    printf("==========================================\n");
    for(tmp = carlist.prev; tmp != NULL; tmp = tmp->next) {
        printf("brand:%-6s|num:%2d|fd:%2d\n", tmp->brand, tmp->num, tmp->seller.fd);
    }
}

/*free carlist, don't free cr*/
void carlist_free(struct node *carlist)
{
    struct node *tmp;
    for(tmp = carlist->prev; tmp != NULL; tmp = carlist->prev) {
        carlist->prev = tmp->next;
        node_free(tmp);
    }
}

