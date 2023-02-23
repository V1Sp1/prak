#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "list.h"

#include <stdio.h>

#include "chat_setings.h"

static lst l;
static int msgid;

void sigint_hndlr(int sig)
{
    exit(0);
}

void exit_hndlr(void)
{
    msgctl(msgid, IPC_RMID, NULL);
    rmlst(&l);
}

/*TODO: write to smb*/

int main(void)
{
    int t;
    key_t key;
    struct msg msg;
    lst tmp;
    initlst(&l);
    key = ftok(FTOK_K, 's');
    msgid = msgget(key, PERMS | IPC_CREAT);
    atexit(exit_hndlr);
    signal(SIGINT, sigint_hndlr);
    for(;;){
        msgrcv(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), SERV, 0);
        t = add2lst(l, msg.pid);
        if(t == 1){
            msg.mtype = msg.pid;
            msg.opt = join;
            msgsnd(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), 0);
        }
        if(msg.opt == leave){
            rmelem(l, msg.pid);
        }
        tmp = l->next;
        for(; tmp != NULL; tmp = tmp->next){
            msg.mtype = tmp->elem;
            if(msg.pid != tmp->elem){
                msgsnd(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), 0);
            }
        }
    }
}
