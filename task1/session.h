#ifndef SESSION_H_CAR
#define SESSION_H_CAR

#include <netinet/in.h>

#ifndef INBUFSIZE
#define INBUFSIZE 1024
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

struct session {
    int fd;
    unsigned long from_ip;
    unsigned short from_port;
    char buf[INBUFSIZE];
    unsigned buf_used;
    enum sess_states state;
};

void session_check_cmd(struct session *sess, char *line);

void session_send_str(struct session *sess, const char *str);

struct session *make_new_session(int fd, struct sockaddr_in *from);

char *session_form_line(struct session *sess);

int session_do_read(struct session *sess);

int session_check_correctness(struct session *sess, const char *reportmsg);

#endif /* SESSION_H_CAR */
