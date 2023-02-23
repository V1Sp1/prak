#ifndef ERROR_H_TASK5
#define ERROR_H_TASK5

enum status_t {
    OK,
    LEX_LOST_QUOTE,
    SYN_NOT_EXPECTED_END,
    SYN_NOT_EXPECTED_WORD,
    SYN_NOT_EXPECTED_REDIR_OR_SEP,
    SYN_NOT_EXPECTED_CONV,
    SYN_NOT_EXPECTED_LBRACKET,
    SYN_NOT_EXPECTED_RBRACKET,
    SYN_MET_CLOSE_BRACKET_BEFORE_OPEN,
    SYN_EXPECTED_CLOSE_BRACKET,
    SYN_MANY_REDIRECTIONS,
    INV_FORK,
    INV_ALLOC,
};


enum status_t getstatus(void);

void setstatus(enum status_t st);

/*TODO: print position*/
void printstatus(void);

#endif /* ERROR_H_TASK5 */
