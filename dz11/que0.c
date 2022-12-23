#include <sys/ipc.h>
#include <sys/msg.h> /*message queue*/
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "set.h"

int main(void)
{
    key_t key;
    int msgid;
    char str[256];
    struct qmsg message;
    key = ftok(QUE_FTOK_K, 's');
    msgid = msgget(key, QUE_PERMS | IPC_CREAT);
    for(;;){
        scanf("%256s", str);
        strcpy(message.data, str);
        switch(str[0]){
            case 'a':
            case 'A':
                message.mtype = 1;
                msgsnd(msgid, (struct msgbuf*)(&message), strlen(str) + 1, 0);
                break;
            case 'b':
            case 'B':
                message.mtype = 2;
                msgsnd(msgid, (struct msgbuf*)(&message), strlen(str) + 1, 0);
                break;
            case 'q':
            case 'Q':
                message.mtype = 1;
                msgsnd(msgid, (struct msgbuf*)(&message), strlen(str) + 1, 0);
                message.mtype = 2;
                msgsnd(msgid, (struct msgbuf*)(&message), strlen(str) + 1, 0);
                sleep(10);
                msgctl(msgid, IPC_RMID, NULL);
                return 0;
            default:
                break;
        }
    }
    return 0;
}
