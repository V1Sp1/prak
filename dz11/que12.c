#include <sys/ipc.h>
#include <sys/msg.h> /*message queue*/
#include <sys/types.h>
#include <stdio.h>

#include "set.h"

int main(void)
{
    key_t key;
    int msgid;
    struct qmsg message;
    key = ftok(QUE_FTOK_K, 's');
    msgid = msgget(key, QUE_PERMS | IPC_CREAT);
    for(;;){
        msgrcv(msgid, (struct msgbuf*)(&message), 256, PROCESS_N, 0);
        if((message.data[0] == 'q') || (message.data[0] == 'Q')){
            break;
        }
        printf("Process %d: %s\n", PROCESS_N, message.data);
    }
    return 0;
}
