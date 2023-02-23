#ifndef MATRIX_H_Rob
#define MATRIX_H_Rob

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

/*declare matrix m x n*/
double **matdef(int m, int n);

/*input elements of matrix m x n*/
void matinput(double **mat, int m, int n);

/*fills the matrix with random numbers*/
void matrand(double **mat, int m, int n);

/*remove matrix*/
void matrm(double **mat, int m, int n);

/*print matrix*/
void matprint(double **mat, int m, int n);

/*returns the transposed matrix A^T in n x m for A in m x n*/
double **mattrans(double **src, int m, int n);

/*copy matrix*/
double **matcpy(double **src, int m, int n);

/*returns the inverse matrix A^{-1} if exsts or NULL else for A in m x m*/
double **matinvert(double **src, int m);

#endif /* MATRIX_H */
