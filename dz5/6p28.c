#include <stdio.h>
#include <stdlib.h>

typedef struct inode {
    double data;        /*TODO: change data to *data */
    struct inode *next;
} inode;

typedef inode *list;

void addfirst(list *a, double data)
{
    list b = (list)malloc(sizeof(inode));
    b -> data = data;
    b -> next = *a;
    *a = b;
}

void rmfirst(list *a)
{
    list b = *a;
    if(b == NULL){
        return;
    }
    *a = b -> next;
    free(b);
}

void rmlist(list *a)
{
    if(*a == NULL){
        return;
    }
    rmfirst(a);
}

void printlist(list a)
{
    if(a == NULL){
        return;
    }
    for(; a != NULL; a = a -> next){
        printf("%lf ", a -> data);
    }
    putchar('\n');
}

void firstendswap(list *a)
{
    list c, b = *a;
    if((b == NULL) || (b -> next == NULL)){
        return;
    }
    while(b -> next -> next != NULL){
        b = b -> next;
    }
    c = b -> next;
    c -> next = (*a) -> next;
    (*a) -> next = NULL;
    b -> next = *a;
    *a = c;
}
/*removes from the list the first occurrence of the element e
with the given value list without a header*/
void rmfirsteqnosen(list *a, double e)
{
    if(*a == NULL){
        return;
    }
    if((*a) -> next == NULL){
        free(a);
        a = NULL;
        return;
    }
    while((*a) -> next != NULL){
        if((*a) -> next -> data == e){
            rmfirst(&((*a) -> next));
            break;
        }
    }
}

int main(void)
{
    list a = NULL;
    addfirst(&a, 5.6);
    addfirst(&a, 7.0);
    addfirst(&a, 8.0);
    addfirst(&a, 9.0);
    addfirst(&a, -1.0);
    addfirst(&a, 0.0);
    addfirst(&a, 7.0);
    printlist(a);
    firstendswap(&a);
    printlist(a);
    rmfirsteqnosen(&a, 0.0);
    printlist(a);
    rmlist(&a);
    return 0;
}
