/* Calculate with precision eps > 0 the value of e - the limit of the sequence {x_{i}} at i \to \infty
x_{i} = (1+1/i)^i, i = 1, 2, ...  Consider that the required accuracy is achieved if |x_{i}-x_{i+1}| < eps. */
#include <stdio.h>
#include <math.h>

int main(void)
{
    int i;
    double eps, ax, bx;
    printf("enter precision: ");
    i = scanf("%lf", &eps);
    if((i != 1) || (eps <= 0)) {
        printf("Error: wrong input\n");
        return 1;
    }
    ax = 2;     /*(1+1)^1*/
    bx = 2.25; /*(1+1/2)^2*/
    for(i = 3; (bx - ax) >= eps; i++) { /*???*/
        ax = bx;
        bx = pow(1+1/((double)i), i);
    }
    printf("e is approximately %lf\n", bx);
    return 0;
}
