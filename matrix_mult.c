 /*
  * matrix_mult.c
  * Cmputes matrix multiplication through serial and parallel algorithms, showing the time differences between the methods
  * Author: Omer Yurekli - yureklio@bc.edu
*/

#include "matrix_mult.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

void init_matrix(double * matrix, int dim) {
    double k = 1.0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrix[i * dim + j] = k++;
        }
    }
}


void multiply_serial(const double *a, const double *b, double *c, int dim, int num_workers) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            c[i * dim + j] = 0.0;
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }
}


double *mmap_checked(size_t size) {
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        exit(EXIT_FAILURE);
    }
    return (double *)addr;
}

int munmap_checked(void *addr, size_t size) {
    if (munmap(addr, size) == -1) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
    
pid_t fork_checked() {
    pid_t pid = fork();
    if (pid == -1) {
        exit(EXIT_FAILURE);
    }
    return pid;
}

int verify(const double *m1, const double *m2, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (m1[i * dim + j] != m2[i * dim + j]) {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}

void multiply_chunk(const double *a, const double *b, double *c, int dim, int row_start, int chunk) {
    for (int i = row_start; i < row_start + chunk; i++) {
        for (int j = 0; j < dim; j++) {
            c[i * dim + j] = 0.0;
            for (int k = 0; k < dim; k++) {
                c[i * dim + j] += a[i * dim + k] * b[k * dim + j];
            }
        }
    }
}

void multiply_parallel_processes(const double * const a, const double * const b, double * const c, const int dim, const int num_workers) {
    size_t matrix_size = dim * dim * sizeof(double);
    double *shared_c = mmap_checked(matrix_size);
    int chunk_size = dim / num_workers;
    pid_t pid;
    int i;
    for (i = 0; i < num_workers - 1; i++) {
        pid = fork_checked();
        if (pid == 0) {
            multiply_chunk(a, b, shared_c, dim, i * chunk_size, chunk_size);
            exit(EXIT_SUCCESS);
        }
    }
    multiply_chunk(a, b, shared_c, dim, i * chunk_size, dim - (i * chunk_size));
    while (wait(NULL) > 0);
    memcpy(c, shared_c, matrix_size);
    munmap_checked(shared_c, matrix_size);
}
    
void * task(void *arg) {
    Args *args = (Args *)arg;
    multiply_chunk(args->a, args->b, args->c, args->dim, args->row_start, args->chunk);
    return NULL;
}


void multiply_parallel_threads(const double *a, const double *b, double *c, int dim, int num_workers) {
    pthread_t threads[num_workers - 1];
    Args args[num_workers];
    int chunk = dim / num_workers;
    for (int i = 0; i < num_workers; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].c = c;
        args[i].dim = dim;
        args[i].row_start = i * chunk;
        args[i].chunk = (i == num_workers - 1) ? (dim - i * chunk) : chunk;
        if (i < num_workers - 1) {
            pthread_create(&threads[i], NULL, task, &args[i]);
        }
    }
    task(&args[num_workers - 1]);
    for (int i = 0; i < num_workers - 1; i++) {
        pthread_join(threads[i], NULL);
    }
}


struct timeval time_diff(struct timeval *start, struct timeval *end) {
    struct timeval diff;
    diff.tv_sec = end->tv_sec - start->tv_sec;
    diff.tv_usec = end->tv_usec - start->tv_usec;
    if (diff.tv_usec < 0) {
        diff.tv_sec--;
        diff.tv_usec += 1000000;
    }
    return diff;
}

void print_elapsed_time(struct timeval *start, struct timeval *end, const char *name) {
    struct timeval diff = time_diff(start, end);
    printf("%s algorithm took %ld seconds and %ld microseconds.\n", name, (long)diff.tv_sec, (long)diff.tv_usec);
}

void print_verification(const double * const m1, const double * const m2, const int dim, const char * const name) {
    int result = verify(m1, m2, dim);
    printf("Verification for %s: %s\n", name, result == SUCCESS ? "success" : "failure");
}

void run_and_time(
    multiply_function multiply_matrices,
    const double * const a,
    const double * const b,
    double * const c,
    const double * const gold,
    const int dim,
    const char * const name,
    const int num_workers,
    const bool verify) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    multiply_matrices(a, b, c, dim, num_workers);
    gettimeofday(&end, NULL);
    print_elapsed_time(&start, &end, name);
    if (verify) {
        print_verification(c, gold, dim, name);
    }
}


