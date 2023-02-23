#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef INBUFSIZE
#define INBUFSIZE 1024
#endif

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 32
#endif

#ifndef INIT_SESS_ARR_SIZE
#define INIT_SESS_ARR_SIZE 32
#endif

enum sess_states {
    sess_start,
    sess_help = sess_start,
    sess_buy, 
    sess_sell, 
    sess_list,
    sess_wrong,
    sess_error,
    sess_finish,
};

/*order is imortant for indexing*/
static const char *cmds[] = {"HELP", "BUY", "SELL", "LIST"};

struct session {
    int fd;
    unsigned long from_ip;
    unsigned short from_port;
    char buf[INBUFSIZE];
    char buf_used;
    enum sess_states state;
};

static void session_send_str(struct session *sess, const char *str)
{
    write(sess->fd, str, strlen(str));
}

static struct session *make_new_session(int fd, struct sockaddr_in *from)
{
    struct session *sess = malloc(sizeof(*sess));
    sess->fd = fd;
    sess->from_ip = ntohl(from->sin_addr.s_addr);
    sess->from_port = ntohs(from->sin_port);
    sess->buf_used = 0;
    sess->state = sess_start;
    return sess;
}

static void session_handl_step(struct session *sess, char *line)
{
    switch(sess->state) {
        case sess_help:
            session_send_str(sess, "you in help");
            break;
        case sess_buy:
            session_send_str(sess, "you in buy");
            sess->state = sess_start;
            /*TODO: check cars and write logs*/
            break;
        case sess_sell:
            session_send_str(sess, "you in sell");
            sess->state = sess_start;
            /*TODO: add to list of cars and sendall info about this and write logs*/
            break;
        case sess_list:
            session_send_str(sess, "you in list");
            sess->state = sess_start;
            /*TODO: send list of all cars*/
            break;
        case sess_wrong:
            session_send_str(sess, "incorrect cmd");
            sess->state = sess_start;
            /*TODO: send "incorrect input"*/
        case sess_finish:
        case sess_error:
            free(line);
            break;
        default:
            break;
    }
}

static void session_check_cmd(struct session *sess, char *line)
{
    enum sess_states i;
    sess->state = sess_wrong;
    for(i = sess_help; i <= sess_list; ++i) {
        if(0 == strncmp(line, cmds[i], strlen(cmds[i]))) {
            sess->state = i;
            break;
        }
    }
}

static void session_check_lf(struct session *sess)
{
    int pos = -1;
    int i;
    char *line;
    for(i = 0; i < sess->buf_used; ++i) {
        if(sess->buf[i] == '\n') {
            pos = i;/*TODO: may be \n\r, \r\n, \n, etc*/
            break;
        }
    }
    if(pos == -1) {
        return;
    }
    line = malloc(pos+1);
    memcpy(line, sess->buf, pos);
    line[pos] = 0;
    sess->buf_used -= (pos+1);
    memmove(sess->buf, sess->buf+pos+1, sess->buf_used);
    if(line[pos-1] == '\r') {
        line[pos-1] = 0; /*TODO: may be \n\r, \r\n, \n, etc*/

    }
    session_check_cmd(sess, line);
    session_handl_step(sess, line);
}

static int session_do_read(struct session *sess)
{
    int rc, bufp = sess->buf_used;
    rc = read(sess->fd, sess->buf + bufp, INBUFSIZE - bufp);
    if(rc <= 0) {
        sess->state = sess_error;
        return 0;   /* this means "don't continue" for the caller */
    }
    sess->buf_used += rc;
    session_check_lf(sess);
    if(sess->buf_used >= INBUFSIZE) {
        /* we can't read further, no room in the buffer, no whole line yet */
        session_send_str(sess, "Line too long! Good bye...\n");
        sess->state = sess_error;
        return 0;
    }
    if(sess->state == sess_finish) {
        return 0;
    }
    return 1;
}

/*TODO: add commit function*/

/* ================ server ================ */

struct server_str {
    int ls;
    FILE *res;
    struct session **sess_array;
    int sess_array_size;
};

static int server_init(struct server_str *serv, int port, const char *fname)
{
    int sock, i, opt;
    struct sockaddr_in addr;
    FILE *f;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("socket");
        return 0;
    }
    opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if(-1 == bind(sock, (struct sockaddr*) &addr, sizeof(addr))) {
        perror("bind");
        return 0;
    }

    listen(sock, LISTEN_QLEN);

    serv->ls = sock;

    f = fopen(fname, "wb");
    if(!f) {
        perror(fname);
        close(sock);
        return 0;
    }
    serv->res = f;

    serv->sess_array = malloc(sizeof(*serv->sess_array) * INIT_SESS_ARR_SIZE);
    serv->sess_array_size = INIT_SESS_ARR_SIZE;
    for(i = 0; i < INIT_SESS_ARR_SIZE; i++){
        serv->sess_array[i] = NULL;
    }

    return 1;
}

static void server_accept_client(struct server_str *serv)
{
    int sd, i;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    sd = accept(serv->ls, (struct sockaddr*) &addr, &len);
    if(sd == -1) {
        perror("accept");
        return;
    }

    if(sd >= serv->sess_array_size) {  /* need to resize */
        int newlen = serv->sess_array_size;
        while(sd >= newlen) {
            newlen += INIT_SESS_ARR_SIZE;
        }
        serv->sess_array = realloc(serv->sess_array, newlen * sizeof(struct session*));
        for(i = serv->sess_array_size; i < newlen; ++i) {
            serv->sess_array[i] = NULL;
        }
        serv->sess_array_size = newlen;
    }

    serv->sess_array[sd] = make_new_session(sd, &addr);
}

static void server_remove_session(struct server_str *serv, int sd)
{
    close(sd);
    serv->sess_array[sd]->fd = -1;
    free(serv->sess_array[sd]);
    serv->sess_array[sd] = NULL;
}

static void server_close_session(struct server_str *serv, int sd)
{
    server_remove_session(serv, sd);
}

static int server_go(struct server_str *serv)
{
    fd_set readfds;
    int i, sr, ssr, maxfd;
    for(;;) {
        FD_ZERO(&readfds);
        FD_SET(serv->ls, &readfds);
        maxfd = serv->ls;
        for(i = 0; i < serv->sess_array_size; ++i) {
            if(serv->sess_array[i]) {
                FD_SET(i, &readfds);
                if(i > maxfd) {
                    maxfd = i;
                }
            }
        }
        sr = select(maxfd+1, &readfds, NULL, NULL, NULL);
        if(sr == -1) {
            perror("select");
            return 4;
        }
        if(FD_ISSET(serv->ls, &readfds)) {
            server_accept_client(serv);
        }
        for(i = 0; i < serv->sess_array_size; i++) {
            if(serv->sess_array[i] && FD_ISSET(i, &readfds)) {
                ssr = session_do_read(serv->sess_array[i]);
                if(!ssr) {
                    server_close_session(serv, i);
                }
            }
        }
    }
    return 0;
}

int main(int argc, const char * const *argv)
{
    struct server_str server;
    long port;
    char *endptr;

    if(argc != 3) {
        fprintf(stderr, "Usage: serv <port> <log_file_name>\n");
        return 1;
    }

    port = strtol(argv[1], &endptr, 10);
    if(!*argv[1] || *endptr) {
        fprintf(stderr, "Invalid port number\n");
        return 2;
    }

    if(!server_init(&server, port, argv[2]))
        return 3;

    return server_go(&server);
}
