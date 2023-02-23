#ifndef LEX_H_RSHELL
#define LEX_H_RSHELL

/*will be added to buffer or list if it overflowed*/
#ifndef SIZE_ADD
#define SIZE_ADD 16
#endif

/* read N bytes*/
#ifndef N
#define N 10
#endif

struct listw {
    char **list;     /* list of words(as array) */
    int curlist;     /* index current word in list */
    int sizelist;    /* size of list */
};

/*get symbol*/
int getsym(void);

/*null word buffer*/
void nullbuf(void);

/*clear word buffer*/
void clearbuf(void);

/*add symbol to word buffer*/
void addsym(int c);

/*null list of words*/
void null_list(struct listw *li);

/*clear list of words*/
void clearlist(struct listw *li);

/*terminate list of words by NULL*/
void termlist(struct listw *li);

/*add word from word buffer to list of words*/
void addword(struct listw *li);

#endif /* LEX_H_RSHELL */
