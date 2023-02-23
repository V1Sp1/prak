#include <stdio.h>
#include <stdlib.h>

void inputmatrix(unsigned u, unsigned v, int a[][v])
{
    int i, j, l;
    for(i = 0; i < u; i++){
        for(j = 0; j < v; j++){
            l = scanf("%d", &a[i][j]);
            if (l != 1){
                printf("Error: wrong input");
            }
        }
    }
}

void multimatrix(unsigned m, unsigned n, unsigned k, int a[][n], int b[][k], int c[][k])
{
    unsigned i, j, l;
    for(i = 0; i < m; i++){
        for(j = 0; j < k; j++){
            c[i][j] = 0;
            for(l = 0; l < n; l++){
                c[i][j] += a[i][l] * b[l][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    unsigned i, j;
    if(argc != 4){
        printf("Error: not enough parameters");
        return 1;
    }
    unsigned m = atoi(argv[1]);
    unsigned n = atoi(argv[2]);
    unsigned k = atoi(argv[3]);
    int a[m][n];
    int b[n][k];
    int c[m][k]; /*you mustn't use VLA"*/
    printf("input matrix a\n");
    inputmatrix(m, n, a);
    printf("input matix b\n");
    inputmatrix(n, k, b);
    
    multimatrix(m, n, k, a, b, c);
    printf("matrix c:\n");
    for(i = 0; i < m; i++){
        for(j = 0; j < k; j++){
            printf("%d ", c[i][j]);
        }
        putchar('\n');
    }
    return 0;
}
