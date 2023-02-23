#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define COUNTCHLD 7

volatile static sig_atomic_t chldpid[COUNTCHLD];
volatile static sig_atomic_t chldlive;
volatile static sig_atomic_t rnd;
static sig_atomic_t n;
static unsigned randseed;
static int fd[2];

void usr1hndlr(int s)
{
    rnd = rand_r(&randseed) % 2;
    signal(SIGUSR1, usr1hndlr);
}

void usr2hndlr(int s)
{
    signal(SIGUSR1, usr1hndlr);
    read(fd[0], &n, sizeof(int));
    chldpid[n] = 0;
    --chldlive;
}

int main(void)
{
    int i;
    signal(SIGUSR1, usr1hndlr);
    signal(SIGUSR2, usr2hndlr);
    pipe(fd);
    for(i = 0; i < COUNTCHLD; ++i){
        chldpid[i] = fork();
        if(chldpid[i] == -1){
            perror("fork:");
            exit(1);
        }
        if(chldpid[i] == 0){
            close(fd[0]);
            randseed = i + time(NULL);
            for(;;){
                pause();
                if(rnd){
                    printf("%d:Missed\n", i);
                } else {
                    printf("%d:killed\n", i);
                    kill(getppid(), SIGUSR2);
                    write(fd[1], &i, sizeof(int));
                    close(fd[1]);
                    exit(0);
                }
            }
        }
        ++chldlive;
    }
    close(fd[1]);
    while(chldlive > 0){
        for(i = 0; i < COUNTCHLD; ++i){
            if(chldpid[i] != 0){
                kill(chldpid[i], SIGUSR1);
            }
        }
    }
    for(i = 0; i < COUNTCHLD; ++i){
        wait(NULL);
    }
    return 0;
}
