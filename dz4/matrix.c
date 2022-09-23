#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

void matinput(double ***mat, int m, int n)
{
    int i, j;
    *mat = (double**)malloc(m * sizeof(double*));
    for(i = 0; i < m; ++i){
        (*mat)[i] = (double*)malloc(n * sizeof(double));
        for(j = 0; j < n; ++j){
            scanf("%lf", &(*mat)[i][j]);
        }
    }
}

double **matrand(int m, int n)
{
    int i, j;
    double **dst = (double**)malloc(n * sizeof(double*));
    for(i = 0; i < m; ++i){
        dst[i] = (double*)malloc(m * sizeof(double));
        for(j = 0; j < n; ++j){
            dst[i][j] = 1 + (20.0*rand()/(RAND_MAX+1.0));
        }
    }
    return dst;

}

void matrm(double **mat, int m, int n)
{
    int i;
    for(i = 0; i < m; ++i){
        free(mat[i]);
    }
    free(mat);
}

void matprint(double **mat, int m, int n)
{
    int i, j;
    if(!mat){
        return;
    }
    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
            printf("%.3lf ", mat[i][j]);
        }
        putchar('\n');
    }
}

double **mattrans(double **src, int m, int n)
{
    int i, j;
    double **dst = (double**)malloc(n * sizeof(double*));
    for(i = 0; i < n; ++i){
        dst[i] = (double*)malloc(m * sizeof(double));
        for(j = 0; j < m; ++j){
            dst[i][j] = src[j][i];
        }
    }
    return dst;
}

double **matcpy(double **src, int m, int n)
{
    int i;
    double **dst = (double**)malloc(m * sizeof(double*));
    for(i = 0; i < m; ++i){
        dst[i] = (double*)malloc(n * sizeof(double));
        memcpy(dst[i], src[i], n * sizeof(double));
    }
    return dst;
}

/*
A - m x m, B - m x b, C - m x c, ..., NULL to A, A^{-1}B, A^{-1}C, ..., NULL
void **matmulinv(double **src, int m, ...)
{
    va_list vl;
    int i, j;
    double **mat;
    va_start(vl, m);
    for(

    va_end(vl);
}
*/

int main(void)
{
    int m, n, d;
    double **mat = NULL, **mat2 = NULL, **mat3 = NULL;
    printf("Input m and n - row and col\n");
    d = scanf("%d %d", &m, &n);
    if (d != 2){
        printf("Error: wrong input\n");
        return 1;
    }
    srand(time(NULL));
    mat3 = matrand(m, n);
    printf("m x n matrix of random numbers\n");
    matprint(mat3, m, n);
    printf("Input m x n matrix A\n");
    matinput(&mat, m, n);
    mat2 = mattrans(mat, m, n);
    printf("A^T matrix:\n");
    matprint(mat2, n, m);
    matrm(mat3, m, n);
    mat3 = matcpy(mat, m, n);
    printf("copy of A:\n");
    matprint(mat3, m, n);
    matrm(mat, m, n);
    matrm(mat2, n, m);
    matrm(mat3, m, n);
    return 0;
}
