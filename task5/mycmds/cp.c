#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

enum {BUF_SIZE = 1024};

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    int fd1, fd2;
    ssize_t n;
    struct stat s;
    if(argc != 3){
        printf("rshell cp: wrong number of parameters\n");
        return 3;
    }
    fd1 = open(argv[1], O_RDONLY);
    if(fd1 == -1){
        perror(argv[1]);
        return 1;
    }
    stat(argv[1], &s);
    fd2 = creat(argv[2], s.st_mode);
    if(fd2 == -1){
        perror(argv[2]);
        close(fd1);
        return 2;
    }
    while((n = read(fd1, buf, BUF_SIZE)) > 0){
        write(fd2, buf, n);
    }
    close(fd1);
    close(fd2);
    return 0;
}
