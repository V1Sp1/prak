#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum mode {oct, sym};

/* if str is param add flags and returns 1
if isn't parameter else returns 0 */
int addparam(char *str, enum mode *md)
{
    if(str[0] == '-'){
        switch(str[1]){
            case 'b':
                *md = oct;
                break;
            case 'c':
                *md = sym;
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
    enum mode md = sym;
    int width = 10;
    int c, count = 1;
    if((argc != 2) && (argc != 3)){
        printf("rshell od: wrong number of parameters\n");
        return 3;
    }
    for(c = 1; c < argc - 1; ++c){
        if(!addparam(argv[c], &md)){
            printf("rshell grep: expected flags(-c or -b)\n");
            return 2;
        }
    }
    f = fopen(argv[argc - 1], "r");
    if(!f){
        perror(argv[argc - 1]);
        return 1;
    }
    while((c = fgetc(f)) != EOF){
        if(count % width == 1){
            if(count != 1){
                putchar('\n');
            }
            printf("%06d ", count);
        }
        switch(md){
            case oct:
                printf("%03o ", c);
                break;
            case sym:
                if(c == '\n'){
                    printf("%2s ", "\\n");
                } else if (c == '\t'){
                    printf("%2s ", "\\t");
                } else {
                    printf("%2c ", c);
                }
                break;
            default:
                break;
        }
        ++count;
    }
    putchar('\n');
    fclose(f);
    return 0;
}
