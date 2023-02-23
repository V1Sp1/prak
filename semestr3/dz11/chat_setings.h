#ifndef CHAT_H
#define CHAT_H

#define FTOK_K "/home/s02210389/kurs2/sem021222/msg/chat"
#define PERMS 0666
#define MAX_MSG 256
#define SERV 1

/*
 * TODO: rewriting input
 * TODO: send msg by part of input str
 * TODO: locale
 * TODO: save data
 * */

enum option {to_all, to_one, join, leave, end_session};

struct msg {
    long mtype;
    int pid;
    enum option opt;
    char data[MAX_MSG];
};

#endif /* CHAT_H */
