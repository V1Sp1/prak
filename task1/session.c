#include "session.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

/*order is imortant for indexing*/
static const char *cmds[] = {"HELP", "BUY ", "SELL ", "LIST"};

void session_check_cmd(struct session *sess, char *line)
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

void session_send_str(struct session *sess, const char *str)
{
    if((sess == NULL) || (str == NULL)) {
        return;
    }
    write(sess->from.fd, str, strlen(str));
}

struct session *make_new_session(struct sess_addr *from)
{
    struct session *sess = malloc(sizeof(*sess));
    sess->from = *from;
    sess->buf_used = 0;
    sess->state = sess_start;
    return sess;
}

char *session_form_line(struct session *sess, int modify_str)
{
    char tmp, nl = '\0';
    int i, pos = -1;
    char *line;
    for(i = 0; i < sess->buf_used; ++i) {
        tmp = sess->buf[i];
        if(pos == -1) {
            if((tmp == '\n') || (tmp == '\r')) {
                pos = i;
                nl = tmp;
            }
        } else {
            if((tmp == '\n') || (tmp == '\r')) {
                if(tmp == nl) {
                    sess->state = sess_error;
                    return NULL;
                } else {
                    pos = i;
                    break;
                }
            }
            break;
        }
    }
    if(pos == -1) {
        return NULL;
    }
    session_check_cmd(sess, sess->buf);
    if(modify_str && (sess->state != sess_wrong)) {
        i = strlen(cmds[sess->state]);
    } else {
        i = 0;
    }
    line = malloc(pos-i+1);
    memcpy(line, sess->buf+i, pos-i);
    line[pos-i] = 0;
    if((pos > i) && ((line[pos-i-1] == '\r') || (line[pos-i-1] == '\n'))) {
        line[pos-i-1] = 0;
    }
    sess->buf_used -= (pos+1);
    memmove(sess->buf, sess->buf+pos+1, sess->buf_used);
    return line;
}

int session_do_read(struct session *sess)
{
    int rc, bufp = sess->buf_used;
    rc = read(sess->from.fd, sess->buf + bufp, INBUFSIZE - bufp);
    if(rc <= 0) {
        sess->state = sess_error;
        return 0;   /* this means "don't continue" for the caller */
    }
    sess->buf_used += rc;
    return 1;
}

int session_check_correctness(struct session *sess, const char *reportmsg)
{
    if(sess->buf_used >= INBUFSIZE) {
        /* we can't read further, no room in the buffer, no whole line yet */
        if(reportmsg != NULL) {
            session_send_str(sess, reportmsg);
        }
        sess->state = sess_error;
        return 0;
    }
    if(sess->state == sess_finish) {
        return 0;
    }
    return 1;
}
