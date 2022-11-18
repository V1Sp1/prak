#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/* pr1 < f1.dat | pr2 > f2.dat && pr3 */

int main(int argc, char *argv[])
{
    pid_t pr1, pr2, pr3;
    int status;
    int fd[2];
    if (argc != 4){
        exit(7);
    }
    pipe(fd);
    pr1 = fork();
    if(pr1 == -1){
        perror("fork:");
        exit(1);
    }
    if(pr1 == 0){
        int fdi = open("f1.dat", O_RDONLY);
        if(fdi == -1){
            perror("f1.dat:");
            exit(8);
        }
        dup2(fdi, 0);
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        close(fdi);
        execlp(argv[1], argv[1], (char*)0);
        perror(argv[1]);
        exit(2);
    }
    pr2 = fork();
    if(pr2 == -1){
        perror("fork:");
        exit(3);
    }
    if(pr2 == 0){
        int fdo = open("f2.dat", O_WRONLY | O_CREAT, 0666);
        if(fdo == -1){
            perror("f2.dat:");
            exit(9);
        }
        dup2(fd[0], 0);
        dup2(fdo, 1);
        close(fdo);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[2], argv[2], (char*)0);
        perror(argv[2]);
        exit(4);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pr2, &status, 0);
    wait(NULL);
    if(WIFEXITED(status) && (WEXITSTATUS(status) == 0)){
        pr3 = fork();
        if(pr3 == -1){
            perror("fork:");
            exit(5);
        }
        if(pr3 == 0){
            execlp(argv[3], argv[3], (char*)0);
            perror(argv[3]);
            exit(6);
        }
        wait(NULL);
    }
    exit(0);
}
