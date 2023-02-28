#ifndef CARLIST_H_CAR
#define CARLIST_H_CAR

struct node {
    char *brand;
    int num;
    unsigned long seller_ip;
    unsigned short seller_port;
    int seller_fd;
    struct node *prev, *next;
};

struct node *node_init(const char *brand, int num, unsigned long seller_ip, unsigned short seller_port, int seller_fd);

void node_free(struct node *current);

/*elem may be freed*/
void carlist_addcar(struct node *carlist, struct node *elem);

struct node *carlist_sell_car(struct node *current, const char *brand);

/*free carlist*/
void carlist_free(struct node *carlist);

void carlist_print(struct node carlist);

/*save curlist to file*/
/*carlist_save(carlist cl, int fd);*/

#endif /* CARLIST_H_CAR */
