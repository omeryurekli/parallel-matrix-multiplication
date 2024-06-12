 /*
  * matrix_mult.h
  * Header file for matrix_mult.c.
  * Author: Omer Yurekli - yureklio@bc.edu
 */

#define MATRIX_MULT_H
#include <stdbool.h>
#define DIM 1024
#define NUM_WORKERS 4
#define SUCCESS 0
#define FAILURE -1

typedef void (*multiply_function)(const double *a, const double *b, double *c, int dim, int num_workers);

typedef struct {
    const double *a;
    const double *b;
    double *c;
    int dim;
    int row_start;
    int chunk;
} Args;

void run_and_time(
        multiply_function multiply_matrices,
        const double * const a,
        const double * const b,
        double * const c,
        const double * const gold,
        const int dim,
        const char * const name,
        const int num_workers,
        const bool verify);

void init_matrix(double *matrix, int dim);
void multiply_serial(const double *a, const double *b, double *c, int dim, int num_workers);
int verify(const double *m1, const double *m2, int dim);
void multiply_parallel_processes(const double *a, const double *b, double *c, int dim, int num_workers);
void multiply_parallel_threads(const double *a, const double *b, double *c, int dim, int num_workers);

