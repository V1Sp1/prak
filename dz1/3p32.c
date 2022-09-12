/* 3.32. Print the first n prime numbers ( p is a prime number if p >= 2
and is only divisible by 1 and by itself). */
#include <stdio.h>

int isprime(int n)
{
    int d;
    if (n % 2 == 0){
        return (n == 2) ? 1 : 0;
    }
    for(d = 3; d * d <= n; d += 2){
        if (n % d == 0) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int n, i, c;
    i = scanf("%d", &n);
    if(i != 1) {
        printf("Error: wrong input\n");
        return 1;
    }
    for(c = 0, i = 2; c < n; i++) {
        if(isprime(i)){
            c++;
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}
