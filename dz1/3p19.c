/*Count the number of positive integers n ( 102 <= n <= 987 ), in which all three digits are different.*/
#include <stdio.h>

int main(void)
{
    int n;
    int count = 0;
    for (n = 102; n <= 987; n++) {
        if (((n / 100) != (n % 10)) && (((n / 10) % 10) != (n % 10)) && ((n / 100) != ((n / 10) % 10))) {
            count++;
        }
    }
    /* 9*8*8=648 */
    printf("there are %d positive integers greater than or equal to 102 and less than or equal to 987\n", count);
    return 0;
}
