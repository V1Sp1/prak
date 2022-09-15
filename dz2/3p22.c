/*Determine whether it is true that
the cube of the sum of the digits of a natural number nis n times n*/
#include <stdio.h>

int main(void)
{
    int n, k, t;
    k = scanf("%d", &n);
    if(k != 1){
        printf("Error: wrong input\n");
    }
    for(k = 0, t = n; n != 0; n /= 10){
        k += n%10;
    }
    if(k * k * k == t * t){
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    return 0;
}
