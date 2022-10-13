#include "6p45e.h"

tree tree_next_level(queue *qu, tree trlst)
{
    tree tmp, trnew = NULL;
    if(*qu == NULL){
        return trnew;
    }
    do {
        tmp = (tree)queue_get(qu);
        if(tmp == NULL){
            break;
        }
        if(tmp -> left != NULL){
            trnew = tmp -> left;
            queue_put(qu, trnew);
        }
        if(tmp -> right != NULL){
            trnew = tmp -> right;
            queue_put(qu, trnew); 
        }
    } while((*qu != NULL) && ((tree)((*qu) -> data) == trlst));
    return trnew;
}

int queue_lenght(queue qu)
{
    int i = 0;
    for(; qu != NULL; qu = qu -> next){
        ++i;
    }
    return i;
}

int tree_count_n_level(tree tr, int n)
{
    int i;
    tree trlst = tr;
    queue qu = NULL;
    queue_put(&qu, tr);
    for(i = 0; (trlst != NULL) && (i < n); ++i){
        trlst = tree_next_level(&qu, trlst);
    }
    if(i == n){
        return 0;
    }
    i = queue_lenght(qu);
    queue_free(&qu);
    return i;
}

void queue_print(queue qu)
{
    for(; qu != NULL; qu = qu -> next){
        printf("%d ", ((tree)(qu -> data)) -> data);
    }
    putchar('\n');
}

void tree_print(tree tr)
{
    tree trlst = tr;
    queue qu = NULL;
    if(tr == NULL){
        return;
    }
    queue_put(&qu, tr);
    queue_print(qu);
    do {
        trlst = tree_next_level(&qu, trlst);
        if(qu != NULL){
            queue_print(qu);
        }
    } while(trlst != NULL);
    queue_free(&qu);
}

void tree_add(tree *ptr, int data)
{
    if(*ptr == NULL){
        *ptr = (tree)malloc(sizeof(tnode));
        (*ptr) -> data = data;
        (*ptr) -> left = NULL;
        (*ptr) -> right = NULL;
        return;
    }
    if((*ptr) -> data == data){
        return;
    }
    if((*ptr) -> data > data){
        tree_add(&((*ptr) -> left), data);
    } else {
        tree_add(&((*ptr) -> right), data);
    }
}

void tree_free(tree *ptr)
{
    if(*ptr == NULL){
        return;
    }
    tree_free(&((*ptr) -> left));
    tree_free(&((*ptr) -> right));
    free(*ptr);
    *ptr = NULL;
}

int main(void)
{
    tree tr = NULL;
    tree_add(&tr, 1);
    tree_add(&tr, 3);
    tree_add(&tr, 5);
    tree_add(&tr, -3);
    tree_add(&tr, -4);
    tree_add(&tr, 7);
    tree_add(&tr, 9);
    tree_add(&tr, -2);
    tree_add(&tr, 2);
    tree_add(&tr, 6);
    tree_print(tr);
    printf("On 3 level: %d\n", tree_count_n_level(tr, 3));
    tree_free(&tr);
    return 0;
}
