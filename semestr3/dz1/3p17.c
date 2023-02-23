/*For the given numbers a, b and c determine how many roots the equation ax^2+bx+c = 0,
and print them out. If the equation has complex roots, print them as v ± iw*/
#include <stdio.h>
#include <math.h>

int main(void)
{
    double a, b, c, d;
    int n;
    printf("enter a, b, c of ax^2+bx+c\n");
    n = scanf("%lf%lf%lf", &a, &b, &c);
    if (n != 3) {
        printf("Error: input\n");
        return 1;
    }
    if (a == 0){
        if (b == 0) {
            if (c == 0) {
                printf("equation has infty solutions\n");
            } else {
                printf("equations has no solution\n");
            }
        } else {
            printf("equation has 1 solution: %.5lf\n", -c / b);
        }
    } else {
        d = b * b - 4 * a * c;
        if (d == 0) {
            printf("equation has 1 solution: %.5lf\n", -b / (2 * a));
        } else if (d > 0) {
            d = sqrt(d);
            printf("equation has 2 solutions: %.5lf, %.5lf\n", (-b + d)/(2 * a), (-b - d)/(2 * a));
        } else {
            b = -b / (2 * a);
            d = sqrt(-d) / (2 * a);
            printf("equation has 2 solutions: %.5lf%+.5lfi,  %.5lf%+.5lfi\n", b, d, b, -d);
        }
    }
    return 0;
}
