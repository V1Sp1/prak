#include "intcmds.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "lex.h"
#include "tree.h"
#include "exec.h"

/*change curent directory*/
int intrn_cd(char *argv[])
{
    char *s;
    if(argv[1] == NULL){
        s = getenv("HOME");
        if(s == NULL){
            fprintf(stderr, "-rshell: I don't know where is home\n");
            return 1;
        }
    } else if(argv[2] != NULL){
        fprintf(stderr, "-rshell: cd: too much arguments\n");
        return 2;
    } else {
        s = argv[1];
    }
    if(chdir(s) == -1) {
        return 3;
    }
    return 0;
}

int intrn_help(void)
{
    static char msg[] = "This is help to rshell\n"
                        "will sone:\ncurent group\t"
                        "\tnot close ( at line"
                        "\terrors msgs";
    printf("%s\n", msg);
    return 0;
}

/*print lex_list*/
int intrn_print_lex(struct darr li)
{
    if(li.type != darr_of_pchar){
        return 1;
    }
    print_darr(&li, ' ', '\n');
    fflush(stdout);
    return 0;
}

/*print tree*/
int intrn_print_tree(tree tr)
{
    print_tree(tr, 0);
    return 0;
}

/*print bckgrnd list*/
int intrn_jobs(bckgrnd bc)
{
    print_bckgrnd(bc);
    return 0;
}
