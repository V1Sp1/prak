#ifndef CARLIST_H_CAR
#define CARLIST_H_CAR

#include "sessaddr.h"

struct node {
    char *brand;
    int num;
    struct sess_addr seller;
    struct node *prev, *next;
};

struct node *node_init(const char *brand, int num, struct sess_addr *seller);

void node_free(struct node *current);

/*elem may be freed*/
void carlist_addcar(struct node *carlist, struct node *elem);

struct sess_addr *carlist_sell_car(struct node *current, const char *brand);

/*free carlist*/
void carlist_free(struct node *carlist);

void carlist_print(struct node carlist);

/*save curlist to file*/
/*carlist_save(carlist cl, int fd);*/

#endif /* CARLIST_H_CAR */
