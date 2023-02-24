#ifndef CARLIST_H
#define CARLIST_H

typedef struct node {
    char *brand;
    int num;
    unsigned long seller_ip;
    unsigned short seller_port;
    int seller_fd;
    struct node *prev, *next;
} node;

node *node_init(const char *brand, int num, unsigned long seller_ip, unsigned short seller_port, int seller_fd);

void node_free(node *current);

/*elem may be freed*/
void carlist_addcar(node *carlist, node *elem);

node *carlist_sell_car(node *current, char *brand);

/*free carlist*/
void carlist_free(node *carlist);

void carlist_print(node carlist);

/*save curlist to file*/
/*carlist_save(carlist cl, int fd);*/

#endif /* CARLIST_H */
