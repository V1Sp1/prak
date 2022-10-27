#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int lenbstr(const char *str)
{
    int k = 0;
    for(; *str != '\0'; ++str){
        if(*str == '/'){
            ++k;
        }
    }
    return k;
}

void depth(const char *pathdir, char *maxp)
{
    DIR *dir;
    struct dirent *dd;
    char path[PATH_MAX];
    int slen;
    struct stat stb;
    if(lenbstr(pathdir) > lenbstr(maxp)){
        strcpy(maxp, pathdir);
    }
    printf("Currecnt path: %s\n", pathdir);
    dir = opendir(pathdir);
    if(!dir){
        return;
    }
    while((dd = readdir(dir)) != NULL){
        slen = snprintf(path, sizeof(path), "%s/%s", pathdir, dd->d_name);
        if((slen + 1 > sizeof(path)) || (strcmp(dd -> d_name, ".") == 0) || (strcmp(dd->d_name, "..") == 0)){
            continue;
        }
        if(stat(path, &stb) < 0){
            continue;
        }
        if(S_ISDIR(stb.st_mode) && !S_ISLNK(stb.st_mode)){
            depth(path, maxp);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    const char *name = ".";
    char maxp[PATH_MAX] = ".";
    if(argc > 1){
        name = argv[1];
    }
    depth(name, maxp);
    printf("maxpath: %s\n", maxp);
    return 0;
}

/*
S_ISREG(st_mode) Регулярный файл
S_ISDIR(st_mode) Каталог
S_ISCHR(st_mode) Специальный файл символьного устройства
S_ISBLK(st_mode) Специальный файл блочного устройства
S_ISFIFO(st_mode) Канал (именованный или неименованный)
S_ISLNK(st_mode) Символическая ссылка
S_ISSOCK(st_mode) Сокет
*/
