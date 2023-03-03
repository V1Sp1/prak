#ifndef SESSADDR_H_CAR
#define SESSADDR_H_CAR

#include <netinet/in.h>

struct sess_addr {
    int fd;
    unsigned long ip;
    unsigned long port;
};

void make_sess_addr(struct sess_addr *s_addr, int fd, struct sockaddr_in *from);

#endif /* SESADDR_H_CAR */
