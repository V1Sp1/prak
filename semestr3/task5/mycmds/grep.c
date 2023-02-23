#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum {INVERTMATCH = 1, NUMERATE = 2, START_SIZE = 32};

/* if str is param add flags and returns 1
if isn't parameter else returns 0 */
int addparam(char *str, int *flags)
{
    if(str[0] == '-'){
        switch(str[1]){
            case 'n':
                *flags |= NUMERATE;
                break;
            case 'v':
                *flags |= INVERTMATCH;
                break;
            default:
                return 0;
        }
        if(str[2] != '\0'){
            return 0;
        }
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *f;
    int c, count = 1, flags = 0;
    int cursize = 0;
    int allocsize = START_SIZE;
    char *tmp, *str;
    if((argc < 3) || (5 < argc)){
        printf("rshell grep: wrong number of parameters\n");
        return 3;
    }
    for(c = 1; c < argc - 2; ++c){
        if(!addparam(argv[c], &flags)){
            printf("rshell grep: expected flags(-v or -n)\n");
            return 2;
        }
    }
    f = fopen(argv[argc - 1], "r");
    if(!f){
        perror(argv[argc - 1]);
        return 1;
    }
    str = malloc(allocsize * sizeof(char));
    while((c = fgetc(f)) != EOF){
        if(cursize >= allocsize){
            allocsize *= 2;
            str = realloc(str, allocsize * sizeof(char));
        }
        switch(c){
            case '\n':
                str[cursize] = '\0';
                cursize = 0;
                tmp = strstr(str, argv[argc - 2]);
                if((tmp && !(flags & INVERTMATCH)) || (!tmp && (flags & INVERTMATCH))){
                    if(flags & NUMERATE){
                        printf("%d: ", count);
                    }
                    printf("%s\n", str);
                }
                ++count;
                break;
            default:
                str[cursize] = c;
                ++cursize;
                break;
        }
    }
    if(cursize > 1){
        ++count;
        str[cursize] = '\0';
        if((tmp && !(flags & INVERTMATCH)) || (!tmp && (flags & INVERTMATCH))){
            if(flags & NUMERATE){
                printf("%d: ", count);
            }
            printf("%s", str);
        }
    }
    fclose(f);
    free(str);
    return 0;
}
