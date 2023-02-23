#include <stdio.h>

int fib(int n)
{
    if(n <= -1){
        return 0;
    } else if ((n == 0) || (n == 1)){
        return 1;
    } else {
        return fib(n-1)+fib(n-2);
    }
}

int main(void)
{
    int i, n;
    i = scanf("%d", &n);
    if((i != 1) && (n >= 0)){
        printf("Inccorect input");
    }
    for(i = 0; i <= n; i++){
        printf("%d fibbonachi number is %d\n", i, fib(i));
    }
    return 0;
}
