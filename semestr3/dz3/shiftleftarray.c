#include <stdio.h>

int gcd(int a, int b)
{
    int r;
    while(b != 0){
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/*shift arror left*/
void shiftlarr(int arr[], int n, int m)
{
    int i, j, k, t;
    m %= n;
    if(m == 0){
        return;
    } else if(m < 0){
        m += n;
    }
    for(i = 0; i < gcd(n, m); i++){
        j = (i + m) % n;
        t = arr[j];
        for( ; j != i; j = k){
            k = (j + m) % n;
            arr[j] = arr[k];
        }
        arr[i] = t;
    }
}

#define SIZE sizeof(a)/sizeof(*a)

int main(void)
{
    int i, k;
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7};
    k = scanf("%d", &i);
    if(k != 1){
        printf("Error: wrong input");
    }
    shiftlarr(a, SIZE, i);
    for(i = 0; i < SIZE; i++){
        printf("%d ", a[i]);
    }
    putchar('\n');
    return 0;
}
