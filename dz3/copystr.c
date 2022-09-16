#include <stdio.h>

void strcpy(char *src, char *dst)
{
    for( ; *src; dst++, src++){
        *dst = *src;
    }
    *dst = 0;
}

int main(void)
{
    char a[] = "Hello";
    char b[10];
    strcpy(a, b);
    printf("%s\n", b);
    return 0;
}
