#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "sessaddr.h"
#include "session.h"
#include "carlist.h"

#include "msgs.h"

static const char no_car_msg[] =
    "Sorry, now there is no such car in our dealership.\n"
    /*"Write LIST to see all cars\n"*/;
static const char car_brand_msg[] =
    "The car with brand ";
static const char was_sold_msg[] = 
    " was sold";
static const char was_put_up_msg[] = 
    " was put up for sale";
static const char congr_buy_msg[] = 
    "Congratulations on your successful purchase!!!\n";
static const char congr_sell_msg[] = 
    "Congratulations on your successful sale!!!\n";

#ifndef LISTEN_QLEN
#define LISTEN_QLEN 32
#endif

#ifndef INIT_SESS_ARR_SIZE
#define INIT_SESS_ARR_SIZE 32
#endif


volatile static sig_atomic_t working = 1;

static void sigint_hndlr(int sig)
{
    int save_errno = errno;
    signal(SIGINT, sigint_hndlr);
    working = 0;
    errno = save_errno;
}




struct server_str {
    int ls;                     /*listen socket*/
    FILE *res;                  /*TODO: log file*/
    struct node *carlist;       /*list of cars*/
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

    serv->carlist = node_init(NULL, 0, NULL);

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
    struct sess_addr s_addr;
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
    make_sess_addr(&s_addr, sd, &addr);
    serv->sess_array[sd] = make_new_session(&s_addr);
}

static void server_close_session(struct server_str *serv, int sd)
{
    close(sd);
    serv->sess_array[sd]->from.fd = -1;
    free(serv->sess_array[sd]);
    serv->sess_array[sd] = NULL;
}

static void server_off(struct server_str *serv)
{
    int i;
    close(serv->ls);
    fclose(serv->res);
    carlist_free(serv->carlist);
    node_free(serv->carlist);
    for(i = 0; i < serv->sess_array_size; i++) {
        if(serv->sess_array[i] != NULL) {
            session_send_str(serv->sess_array[i], "The server is stoped\n");
            server_close_session(serv, i);
        }
    }
    free(serv->sess_array);
}

static void server_send_str_all(struct server_str *serv, const char *str)
{
    int i;
    for(i = 0; i < serv->sess_array_size; ++i) {
        if(serv->sess_array[i] != NULL) {
            session_send_str(serv->sess_array[i], str);
        }
    }
}


/*TODO: add commit function*/


static void session_handl_step(struct server_str *serv, int sd, char *line)
{
    char *buf;
    struct sess_addr *s_addr;
    struct session *sess = serv->sess_array[sd];
    struct node *carlist = serv->carlist, *tmp;
    switch(sess->state) {
        case sess_help:
            session_send_str(sess, help_msg);
            sess->state = sess_start;
            break;
        case sess_buy:
            s_addr = carlist_sell_car(carlist, line);
            if(s_addr == NULL) {
                session_send_str(sess, no_car_msg);
            } else {
                buf = malloc(sizeof(car_brand_msg)-1+strlen(line)+sizeof(was_sold_msg)-1+1 + 1);
                sprintf(buf, "%s%s%s\n", car_brand_msg, line, was_sold_msg);
                server_send_str_all(serv, buf);
                session_send_str(serv->sess_array[sd], congr_buy_msg);
                session_send_str(serv->sess_array[s_addr->fd], congr_sell_msg);
                free(s_addr);
                free(buf);
                /*write line to log file*/
            }
            sess->state = sess_start;
            break;
        case sess_sell:
            /*check correctness line*/
            tmp = node_init(line, 1, &sess->from);
            buf = malloc(sizeof(car_brand_msg)-1+strlen(line)+sizeof(was_put_up_msg)-1+1 + 1);
            sprintf(buf, "%s%s%s\n", car_brand_msg, line, was_put_up_msg);
            server_send_str_all(serv, buf);
            carlist_addcar(carlist, tmp); /*warning: we no longer own tmp*/
            free(buf);
            /*write line to log file*/
            sess->state = sess_start;
            break;
        case sess_list:
            carlist_print(*carlist);
            sess->state = sess_start;
            break;
        case sess_wrong:
            session_send_str(sess, incorrect_input_msg);
            sess->state = sess_start;
        case sess_finish:
        case sess_error:
            break;
        default:
            break;
    }
    free(line);
}



static int server_go(struct server_str *serv)
{
    fd_set readfds;
    int i, sr, ssr, maxfd;
    char *line;
    struct session *sess;
    while(working) {
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
            if(errno == EINTR) {
                continue;
            }
            perror("select");
            return 4;
        }
        if(FD_ISSET(serv->ls, &readfds)) {
            server_accept_client(serv);
        }
        for(i = 0; i < serv->sess_array_size; ++i) {
            if(serv->sess_array[i] && FD_ISSET(i, &readfds)) {
                sess = serv->sess_array[i];
                ssr = session_do_read(sess);
                if(!ssr) {
                    server_close_session(serv, i);
                    continue;
                }
                line = session_form_line(sess, 1);
                if(line != NULL) {
                    session_handl_step(serv, i, line);
                }
                ssr = session_check_correctness(sess, long_line_msg);
                if(!ssr) {
                    server_close_session(serv, i);
                }
            }
        }
    }
    server_off(serv);
    return 0;
}



int main(int argc, const char * const *argv)
{
    struct server_str server;
    long port;
    char *endptr;
    signal(SIGINT, sigint_hndlr);
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
