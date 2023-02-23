#include "error.h"
#include <stdio.h>

enum status_t status;

enum status_t getstatus(void)
{
    return status;
}

void setstatus(enum status_t st)
{
    status = st;
}

/*TODO: print error position*/
void printstatus(void)
{
    switch(status){
        case OK:
            fprintf(stderr, "rshell: OK\n");
            break;
        case LEX_LOST_QUOTE:
            fprintf(stderr, "rshell: closing quote is missing\n");
            break;
        case SYN_NOT_EXPECTED_END:
            fprintf(stderr, "rshell: not expected end of line\n");
            break;
        case SYN_NOT_EXPECTED_WORD:
            fprintf(stderr, "rshell: not expected word\n");
            break;
        case SYN_NOT_EXPECTED_REDIR_OR_SEP:
            fprintf(stderr, "rshell: not expected redirection or sep\n");
            break;
        case SYN_NOT_EXPECTED_CONV:
            fprintf(stderr, "rshell: not expected convier\n");
            break;
        case SYN_NOT_EXPECTED_LBRACKET:
            fprintf(stderr, "rshell: not expected lbracket\n");
            break;
        case SYN_NOT_EXPECTED_RBRACKET:
            fprintf(stderr, "rshell: not expected rbracket\n");
            break;
        case SYN_MET_CLOSE_BRACKET_BEFORE_OPEN:
            fprintf(stderr, "rshell: a closing bracket is met, "
                    "but an opening bracket is not\n");
            break;
        case SYN_EXPECTED_CLOSE_BRACKET:
            fprintf(stderr, "rshell: expected close rbracket\n");
            break;
        case SYN_MANY_REDIRECTIONS:
            fprintf(stderr, "rshell: too many redirections\n");
            break;
        case INV_FORK:
            perror("fork");
            break;
        case INV_ALLOC:
            perror("alloc");
            break;
        default:
            break;
    }
    fflush(stderr);
}
