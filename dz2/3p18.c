/* Count the number of positive integers n ( 111 <= n <= 999 )
have two identical digits in their entries.*/
#include <stdio.h>

int main(void)
{
    int n;
    int count = 0;
    for(n = 111;  n <= 999; n++){
        if(((n / 100) == (n % 10)) || (((n / 10) % 10) == (n % 10)) || ((n / 100) == ((n / 10) % 10))){
            count++;
        }
    }
    printf("there are %d positive integers greater than or equal to 111 and less than or equal to 999\n", count);
    return 0;
}
