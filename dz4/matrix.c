#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

double **matdef(int m, int n)
{
    int i;
    double **mat = (double**)malloc(m * sizeof(double*));
    for(i = 0; i < m; ++i){
        mat[i] = (double*)malloc(n * sizeof(double));
    }
    return mat;
}

void matinput(double **mat, int m, int n)
{
    int i, j;
    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
            scanf("%lf", &mat[i][j]);
        }
    }
}

void matrand(double **mat, int m, int n)
{
    int i, j;
    for(i = 0; i < m; ++i){
        for(j = 0; j < n; ++j){
            mat[i][j] = 1 + (20.0*rand()/(RAND_MAX+1.0));
        }
    }
}

void matrm(double **mat, int m, int n)
{
    int i;
    if(!mat){
        return;
    }
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
    double **dst = matdef(n, m);
    for(i = 0; i < n; ++i){
        for(j = 0; j < m; ++j){
            dst[i][j] = src[j][i];
        }
    }
    return dst;
}

double **matcpy(double **src, int m, int n)
{
    int i;
    double **dst = matdef(m, n);
    for(i = 0; i < m; ++i){
        memcpy(dst[i], src[i], n * sizeof(double));
    }
    return dst;
}

double **matinvert(double **src, int m)
{
    double val, *tmp;
    double **matid = matdef(m, m);
    double **cpsrc = matcpy(src, m, m);
    int h, i, j, k;
    for(i = 0; i < m; ++i){
        for(j = 0; j < m; ++j){
            matid[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    for(i = 0, j = 0; (i < m) && (j < m); ++i, ++j){
        for(h = i, k = i; k < m; ++k){
            if(fabs(cpsrc[h][j]) < fabs(cpsrc[k][j])){
                h = k;
            }
        }
        if(cpsrc[h][j] == 0){
            if(i != 0){
                --i;
            }
            continue;
        }
        tmp = cpsrc[h];
        cpsrc[h] = cpsrc[i];
        cpsrc[i] = tmp;
        tmp = matid[h];
        matid[h] = matid[i];
        matid[i] = tmp;

        for(k = 0; k < m; ++k){
            if(k != i){
                val = cpsrc[k][j] / cpsrc[i][j];
                cpsrc[k][j] = 0;
                for(h = j + 1; h < m; ++h){
                    cpsrc[k][h] -= cpsrc[i][h] * val;
                }
                for(h = 0; h < m; ++h){
                    matid[k][h] -= matid[i][h] * val;
                }
            }
        }
        val = cpsrc[i][j];
        for(h = j; h < m; ++h){
            cpsrc[i][h] /= val;
        }
        for(h = 0; h < m; ++h){
            matid[i][h] /= val;
        }
    }
    return matid;
}

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

    /*matrand(mat3, m, n);
    printf("m x n matrix of random numbers\n");
    matprint(mat3, m, n);*/
    
    printf("Input m x n matrix A\n");
    matinput(mat, m, n);
    mat2 = mattrans(mat, m, n);
    /*printf("A^T matrix:\n");
    matprint(mat2, n, m);*/

    mat4 = matinvert(mat, m);
    printf("invert A\n");
    matprint(mat4, m, m);

    matrm(mat3, m, n);
    mat3 = matcpy(mat, m, n);
    /*printf("copy of A:\n");
    matprint(mat3, m, n);*/

    matrm(mat, m, n);
    matrm(mat2, n, m);
    matrm(mat3, m, n);
    matrm(mat4, m, m);
    return 0;
}
