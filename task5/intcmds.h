#ifndef INTERNALCMDS_H_TASK5
#define INTERNALCMDS_H_TASK5

#include "tree.h"
#include "exec.h"

/*print curent directory*/
//int intrn_pwd(void *);

/*change curent directory*/
int intrn_cd(char *argv[]);

/*print help*/
int intrn_help(void);

/*print lex_list*/
int intrn_print_lex(struct darr li);

/*print tree*/
int intrn_print_tree(tree tr);

/*print bckgrnd list*/
int intrn_jobs(bckgrnd bc);


#endif /* INTERNALCMDS__H_TASK5 */
