#ifndef DARR_H_TASK5
#define DARR_H_TASK5

/*will be added to durr.arr if it overflowed*/
#ifndef SIZE_ADD
#define SIZE_ADD 30
#endif

enum darr_t {darr_of_pchar, darr_of_char, none};
struct darr {
    void *arr;     
    int cursize;       /* current size of darr_t*/
    int allocsize;     /* size of alloc */
    enum darr_t type;  /*type of elements*/
};

/*null darray*/
void null_darr(struct darr *dar);

/*clear dar*/
int clear_darr(struct darr *dar);

/*add elem with type dar->type to dar*/
int add_to_darr(struct darr *dar, void *elem);

/*terminate dar*/
int term_darr(struct darr *dar);

#endif /* LIST_H_TASK5 */
