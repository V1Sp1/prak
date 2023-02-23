#include <stdio.h>
#include "matrix.h"

int main(void)
{
    int m, n, d;
    double **mat = NULL, **mat2 = NULL, **mat3 = NULL, **mat4 = NULL;
    printf("Input m and n - row and col\n");
    d = scanf("%d %d", &m, &n);
    if (d != 2){
        printf("Error: wrong input\n");
        return 1;
    }
    srand(time(NULL));
    mat = matdef(m, n);
    mat2 = matdef(n, m);
    mat3 = matdef(m, n);
    mat4 = matdef(m, m);

    matrand(mat3, m, n);
    printf("m x n matrix of random numbers\n");
    matprint(mat3, m, n);
    
    printf("Input m x n matrix A\n");
    matinput(mat, m, n);
    mat2 = mattrans(mat, m, n);
    printf("A^T matrix:\n");
    matprint(mat2, n, m);

    mat4 = matinvert(mat, m);
    if(mat4 != NULL){
        printf("invert A\n");
        matprint(mat4, m, m);
    } else {
        printf("det(A) = 0\n");
    }

    matrm(mat3, m, n);
    mat3 = matcpy(mat, m, n);
    printf("copy of A:\n");
    matprint(mat3, m, n);

    matrm(mat, m, n);
    matrm(mat2, n, m);
    matrm(mat3, m, n);
    matrm(mat4, m, m);
    return 0;
}
