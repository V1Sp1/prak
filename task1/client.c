#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>

#include "sessaddr.h"
#include "session.h"
#include "msgs.h"


volatile static sig_atomic_t working = 1;

static void sigint_hndlr(int sig)
{
    int save_errno = errno;
    signal(SIGINT, sigint_hndlr);
    working = 0;
    errno = save_errno;
}

struct client_str {
    struct session *sess_arr[2];     /*[0] - stdin, [1] - server*/
};

static int client_init(struct client_str *cli, const char *serv_ip, int serv_port)
{
    int sock, s;
    struct sockaddr_in addr;
    struct sess_addr s_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket");
        return 0;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv_port);
    s = inet_aton(serv_ip, &(addr.sin_addr));
    if(!s) {
        fputs("Invalid ip adress\n", stdout);
        return 2;
    }
    if(-1 == connect(sock, (struct sockaddr*) &addr, sizeof(addr))) {
        perror("connect");
        return 0;
    }
    make_sess_addr(&s_addr, 0, NULL);
    cli->sess_arr[0] = make_new_session(&s_addr);
    make_sess_addr(&s_addr, sock, &addr);
    cli->sess_arr[1] = make_new_session(&s_addr);
    return 1;
}

static void client_off(struct client_str *cli)
{
    free(cli->sess_arr[0]);
    cli->sess_arr[0] = NULL;
    close(cli->sess_arr[1]->from.fd);
    free(cli->sess_arr[1]);
    cli->sess_arr[1] = NULL;
}

static void client_handl_step(struct client_str *cli, char *line, int sd)
{
    int len;
    if(sd == 1){
        fprintf(stdout, "%s\n", line);
        fflush(stdout);
    } else {
        switch(cli->sess_arr[0]->state) {
            case sess_help:
                fputs(help_msg, stderr);
                cli->sess_arr[0]->state = sess_start;
                break;
            case sess_sell:
            case sess_buy:
            case sess_list:
                len = strlen(line);
                line = realloc(line, len + 2);
                line[len] = '\n';
                line[len+1] = '\0';
                session_send_str(cli->sess_arr[1], line);
                cli->sess_arr[0]->state = sess_start;
                break;
            case sess_wrong:
                fputs(incorrect_input_msg, stderr);
                cli->sess_arr[0]->state = sess_start;
                break;
            case sess_finish:
            case sess_error:
            default:
                break;
        }
    }
    free(line);
}

static int client_go(struct client_str *cli)
{
    fd_set readfds;
    char *line;
    struct session *sess;
    int i, sr;
    while(working) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(cli->sess_arr[1]->from.fd, &readfds);
        sr = select(cli->sess_arr[1]->from.fd + 1, &readfds, NULL, NULL, NULL);
        if(sr == -1) {
            if(errno == EINTR) {
                continue;
            }
            perror("select");
            return 5;
        }
        for(i = 0; i < 2; ++i) {
            sess = cli->sess_arr[i];
            if(FD_ISSET(sess->from.fd, &readfds)) {
                sr = session_do_read(sess);
                if(!sr) {
                    working = 0;
                    break;
                }
                line = session_form_line(sess, 0);
                if(line != NULL) {
                    client_handl_step(cli, line, i);
                }
                sr = session_check_correctness(sess, i == 0 ? long_line_msg : NULL);
                if(!sr) {
                    working = 0;
                    break;
                }
            }
        }
    }
    client_off(cli);
    return 0;
}

int main(int argc, const char * const *argv)
{
    struct client_str client;
    long port;
    char *endptr;
    signal(SIGINT, sigint_hndlr);
    if(argc != 3) {
        fputs("Usage: client <ip> <port>\n", stderr);
        return 1;
    }
    port = strtol(argv[2], &endptr, 10);
    if(!*argv[2] || *endptr) {
        fputs("Invalid port number\n", stderr);
        return 3;
    }
    if(!client_init(&client, argv[1], port))
        return 4;

    return client_go(&client);
}
