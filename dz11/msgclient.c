#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "chat_setings.h"



void recvmsg(int msgid, int id)
{
    struct msg msgfrom;
    for(;;){
        msgrcv(msgid, (struct msgbuf*)(&msgfrom), sizeof(msgfrom) - sizeof(msgfrom.mtype), id, 0);
        switch(msgfrom.opt){
            case to_all:
                printf("%d:%s", msgfrom.pid, msgfrom.data);
                break;
            case join:
                printf("%d joined to chat\n", msgfrom.pid);
                break;
            case leave:
                printf("%d leave the chat\n", msgfrom.pid);
                break;
            default:
                break;
        }
        /*TODO*/
        /*printf("%d(you):", id);
        fflush(stdout);*/
    }
}

int main(void)
{
    int msgid;
    int pid = getpid(), spid;
    key_t key;
    char str[MAX_MSG];
    struct msg msg;
    key = ftok(FTOK_K, 's');
    msgid = msgget(key, 0);
    if(msgid == -1){
        perror("no quems");
        return 1;
    }
    msg.mtype = SERV;
    msg.pid = pid;
    msgsnd(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), 0);
    msgrcv(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), pid, 0);
    if(msg.opt != join){
        printf("I can't join\n");
        return 2;
    }
    spid = fork();
    if(spid == -1){
        perror("fork:");
        return 3;
    }
    if(spid == 0){
        recvmsg(msgid, pid);
    }
    msg.mtype = SERV;
    printf("Wellcome to the chat %d\n", msgid);
    for(;;){
        fgets(str, MAX_MSG, stdin);
        if(strcmp(str, "quit\n") == 0){
            msg.opt = leave;
            msgsnd(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), 0);
            break;
        } else {
            msg.opt = to_all;
            strcpy(msg.data, str);
            msgsnd(msgid, (struct msgbuf*)(&msg), sizeof(msg) - sizeof(msg.mtype), 0);
        }
    }
    kill(spid, SIGKILL);
    return 0;
}
