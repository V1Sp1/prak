#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "darr.h"
#include "lex.h"
#include "tree.h"
#include "exec.h"

void inv()
{
    printf("%s", "\x1b[36m");
    char s[100];
    gethostname(s, 100);
    printf("%s@%s", getenv("USER"), s); 
    printf("%s", "\x1B[37m");
    getcwd(s, 100);
    printf(":%s$ ", s);
    fflush(stdout);
}

void add_to_path(void)
{
    static char rshell_path[] = "/home/s02210389/kurs2/rshell/final/mycmds";
    char *buf = getenv("PATH");
    int size = sizeof(rshell_path)/sizeof(*rshell_path);
    char *tmp;
    if(buf != NULL){
        tmp = malloc(size + 1 + strlen(buf));
        sprintf(tmp, "%s:%s", rshell_path, buf);
    } else {
        tmp = malloc(size);
        sprintf(tmp, "%s", rshell_path);
    }
    setenv("PATH", tmp, 1);
    free(tmp);
}

int main(int argc, char *argv[]){
    int l = 1;
    int t;
    struct darr wb = {.type = darr_of_char};
    struct darr li = {.type = darr_of_pchar};
    struct darr bckgrndend = {.type = darr_of_pchar};
    tree tr;
    bckgrnd bck;
    add_to_path();
    null_bckgrnd(&bck);
    null_darr(&li);
    null_darr(&bckgrndend);
    while(l){
        if(isatty(0)){
            inv();
        }
        l = getlinesem(&li, &wb);
        addval(&li);
        if(l == -1){
            putchar('\n');
            printstatus();
            l = 0;
        }
        if((l == 0) && isatty(0)){
            putchar('\n');
        }
        if(l == 1){
            creat_tree(&tr);
            t = build_tree(&tr, li.arr);
            if(t == 1){
                t = exec_tree(tr, li, &bck, &bckgrndend);
                if(t == -1){
                    l = 0;
                } else if(t == 0){
                    /*kill current group???*/
                }
            } else {
                putchar('\n');
                printstatus();
            }
            clear_tree(&tr);
        }
        clear_bck_zombie(&bck, &bckgrndend);
        term_darr(&bckgrndend);
        print_darr(&bckgrndend, '\n', 0);
        clear_darr(&bckgrndend);
        clear_darr(&li);
    }
    clear_bckgrndlist(&bck);
    return 0;
}
