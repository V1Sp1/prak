#include "sessaddr.h"

void make_sess_addr(struct sess_addr *s_addr, int fd, struct sockaddr_in *from)
{
    s_addr->fd = fd;
    s_addr->ip = ntohl(from->sin_addr.s_addr);
    s_addr->port = ntohs(from->sin_port); 
}
