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

typedef node *carlist;

node *node_init(char *brand, int num, unsigned long seller_ip, unsigned short seller_port);

void carlist_addcar(carlist cr, node *elem);

node *carlist_sell_car(node *current, char *brand);

/*free carlist*/
void carlist_free(carlist cl);

/*save curlist to file*/
/*carlist_save(carlist cl, int fd);*/

#endif /* CARLIST_H */
