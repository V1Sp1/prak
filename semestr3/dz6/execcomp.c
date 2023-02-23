#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int namesize = strlen(argv[0]);
    char *nameptr = (char*)malloc(sizeof(char) * (namesize + 3));
    strcpy(nameptr, argv[0]);
    nameptr[namesize] = '.';
    nameptr[namesize + 1] = 'c';
    nameptr[namesize + 2] = '\0';
    pid = fork();
    if(pid == -1){
        perror("fork:");
        exit(1);
    }
    if(pid == 0){
        execlp("gcc", "gcc", "-Wall", nameptr, "-o", argv[0] ,(char*)0);
        perror("gcc:");
        exit(1);
    }
    wait(NULL);
    free(nameptr);
    return 0;
}
