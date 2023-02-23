#include <stdio.h>

void bubble_sort(int *arr, int len)
{
    int i, j, c;
    for(i = 0; i < len; i++){
        for(j = len - 1; j > i; j--){
            if(arr[j-1] > arr[j]){
                c = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = c;
            }
        }
    }
}

void insertion_sort(int *arr, int len)
{
    int i, j, c;
    for(i = 1; i < len; i++){
        c = arr[i];
        for(j = i; (j > 0) && (arr[j-1] > c); j--){
            arr[j] = arr[j - 1];
        }
        arr[j] = c;
    }
}

void selection_sort(int *arr, int len)
{
    int i, j, mini;
    for(i = 0; i < len - 1; i++){
        mini = i;
        for(j = i + 1; j < len; j++){
            if(arr[mini] > arr[j]){
                mini = j;
            }
        }
        if(mini != i){
            j = arr[i];
            arr[i] = arr[mini];
            arr[mini] = j;
        }
    }

}

#define SIZE sizeof(m)/sizeof(*m)

int main(void)
{
    int i;
    int m[] = {3, 1, 2, 7, -2, 9, 3};
    /*insertion_sort(m, SIZE);*/
    selection_sort(m, SIZE);
    for(i = 0; i < SIZE; i++){
        printf("m[%d] = %d\n", i, m[i]);
    }
    return 0;
}
