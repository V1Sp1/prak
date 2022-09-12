/* For a given positive real number a, find
the smallest positive integer n such that
1+1/2+1/3+... +1/n > a */

#include <stdio.h>

int main(void)
{
    double a;
    int n;
    n = scanf("%lf", &a);
    if (n != 1) {
        printf("Error: wrong input\n");
        return 1;
    }
    for(n=1; 1 <= n * a; n++) {
        a -= 1 / ((double)n);
    }
    printf("%d\n", n);
    return 0;
}
