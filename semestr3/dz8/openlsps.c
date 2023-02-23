#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    if(argc != 3){
        printf("Error: wrong number of parametrs\n");
        exit(1);
    }
    pid = fork();
    if(pid == -1){
        perror("fork:");
        exit(2);
    }
    if(pid == 0){
        int fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if(fd == -1){
            perror(argv[1]);
            exit(1);
        }
        printf("Executing ls...\n");
        dup2(fd, 1);
        close(fd);
        execlp("ls", "ls", "-l", (char*)0);
        perror("ls:");
        exit(3);
    }
    wait(NULL);
    printf("ls finish\n");
    pid = fork();
    if(pid == -1){
        perror("fork: ");
        exit(2);
    }
    if(pid == 0){
        int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if(fd == -1){
            perror(argv[2]);
            exit(1);
        }
        printf("Executing ps...\n");
        dup2(fd, 1);
        close(fd);
        execlp("ps", "ps", (char*)0);
        perror("ps:");
        exit(3);
    }
    wait(NULL);
    printf("ls finish\n");
    return 0;
}
