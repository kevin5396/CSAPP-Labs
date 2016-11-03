/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    if (N == 32 && M == 32) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 8 && k < N; k++) {
                    for (l = j; l < j + 8 && l < M; l++) {
                        if (k == l)
                            continue;
                        B[l][k] = A[k][l];
                    }
                    if (i == j)
                        B[k][k] = A[k][k];
                }
            }
        }
    } else if (M == 64) {
        int r0,r1,r2,r3, r4, r5, r6, r7;
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                // use B as buffer ! transpose upper left
                for (k = 0; k < 4; k++) {
                    r0 = A[i+k][j];
                    r1 = A[i+k][j+1];
                    r2 = A[i+k][j+2];
                    r3 = A[i+k][j+3];
                    r4 = A[i+k][j+4];
                    r5 = A[i+k][j+5];
                    r6 = A[i+k][j+6];
                    r7 = A[i+k][j+7];

                    B[j][i+k] = r0;
                    B[j+1][i+k] = r1;
                    B[j+2][i+k] = r2;
                    B[j+3][i+k] = r3;

                    // buffer, store transpose B lower left
                    B[j][i+k+4] = r4;
                    B[j+1][i+k+4] = r5;
                    B[j+2][i+k+4] = r6;
                    B[j+3][i+k+4] = r7;
                }
                // other part
                for (k = 0; k < 4; k++) {
                    // fetch transpose lower left from upper right
                    r0 = B[j+k][i+4];
                    r1 = B[j+k][i+5];
                    r2 = B[j+k][i+6];
                    r3 = B[j+k][i+7];

                    B[j+k][i+4] = A[i+4][j+k];
                    B[j+k][i+5] = A[i+5][j+k];
                    B[j+k][i+6] = A[i+6][j+k];
                    B[j+k][i+7] = A[i+7][j+k];

                    r4 = A[i+4][j+k+4];
                    r5 = A[i+5][j+k+4];
                    r6 = A[i+6][j+k+4];
                    r7 = A[i+7][j+k+4];

                    // line j+k+4 in cache!
                    B[j+k+4][i] = r0;
                    B[j+k+4][i+1] = r1;
                    B[j+k+4][i+2] = r2;
                    B[j+k+4][i+3] = r3;
                    B[j+k+4][i+4] = r4;
                    B[j+k+4][i+5] = r5;
                    B[j+k+4][i+6] = r6;
                    B[j+k+4][i+7] = r7;
                }
            }
        }
    } else  {
        for (i = 0; i < N; i += 18) {
            for (j = 0; j < M; j += 18) {
                for (k = i; k < i + 18 && k < N; k++) {
                    for (l = j; l < j + 18 && l < M; l++) {
                        if (k == l)
                            continue;
                        B[l][k] = A[k][l];
                    }
                    if (i == j)
                        B[k][k] = A[k][k];
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{


}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

