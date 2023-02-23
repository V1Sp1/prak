#ifndef LIST_H_TASK5
#define LIST_H_TASK5

/*will be added to buffer or list if it overflowed*/
#ifndef SIZE_ADD
#define SIZE_ADD 16
#endif

/* read N bytes*/
#ifndef N
#define N 10
#endif


enum darr_t {darr_of_pchar, darr_of_char, none};
struct darr {
    void *arr;     
    int cursize;       /* current size of darr_t*/
    int allocsize;     /* size of alloc */
    enum darr_t type;  /*type of elements*/
};


struct listw {
    char **list;     /* list of words(as array) */
    int curlist;     /* index current word in list */
    int sizelist;    /* size of list */
};

struct wbuf {
    char *buf;       /* word buffer */
    int curbuf;      /* index symbol in buf */
    int sizebuf;     /* size of buf */
};

/*get symbol*/
int getsym(void);


/*null darray*/
void null_darr(struct darr *dar);

/*clear dar*/
int clear_darr(struct darr *dar);

/*add elem with type dar->type to dar*/
int add_to_darr(struct darr *dar, void *elem);

/*terminate dar*/
int term_darr(struct darr *dar);



/*null word buffer*/
void nullbuf(struct wbuf *wb);

void termbuf(struct wbuf *wb);

/*add symbol to word buffer*/
void addsym(struct wbuf *wb, int c);

/*null list of words*/
void null_list(struct listw *li);

/*clear list of words*/
void clearlist(struct listw *li);

/*terminate list of words by NULL*/
void termlist(struct listw *li);

/*add word from word buffer to list of words*/
void addword(struct listw *li, char *word);

#endif /* LIST_H_TASK5 */
