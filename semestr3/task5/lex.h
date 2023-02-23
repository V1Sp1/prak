#ifndef LEX_H_TASK5
#define LEX_H_TASK5
#include "darr.h"

/*
get line end by '\n' and parse it
retrurns 1 if '\n', 0 if EOF, -1 if error
*/
int getlinesem(struct darr *li, struct darr *wb);

/*print_list*/
int print_darr(struct darr *dar, int sep, int end);

/*replace $HOME, $SHELL, $USER, $EUID*/
int addval(struct darr *li);

#endif /* LEX_H_TASK5 */
