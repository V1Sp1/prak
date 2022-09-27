#include "matrix.h"

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
            matrm(matid, m, m);
            matrm(cpsrc, m, m);
            return NULL;
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
    matrm(cpsrc, m, m);
    return matid;
}

