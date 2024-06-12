# Parallel Matrix Multiplication

This project demonstrates the performance benefits of parallel computing through matrix multiplication. It compares the execution times of three different algorithms: a serial algorithm, a parallel algorithm using separate processes, and a parallel algorithm using threads within the same process.

## Overview

Matrix multiplication is performed on DIM-by-DIM square matrices, represented as flat arrays of type `double`. The project includes functions for initializing matrices, performing matrix multiplication in serial, and using parallel processes and threads.

## File Structure

- **matrix_mult.c**: The main source code file implementing the matrix multiplication algorithms.
- **matrix_mult.h**: The header file containing symbolic constant definitions, type definitions, and function declarations.
- **makefile**: The makefile for building and cleaning the project.

## Constants and Types

### Symbolic Constants

Defined in `matrix_mult.h`:
- `DIM`: The dimension of the matrices (both height and width). Initially set to a smaller value (e.g., 4 or 8) for development and debugging. Eventually, it will be set to 1024.
- `NUM_WORKERS`: The number of parallel processes or threads used in the parallel algorithms. Set to 4.
- `SUCCESS`: A return value indicating success, set to 0.
- `FAILURE`: A return value indicating failure, set to -1.

### Type Definitions

- **`multiply_function`**: A type alias for a pointer to a matrix multiplication function.
- **`Args`**: A structure type for passing arguments to `pthread_create`.

## Functions

### Initialization and Utility Functions

- **`void init_matrix(double *matrix, int dim)`**: Initializes a matrix with successive integers starting from 1.
- **`int verify(const double *m1, const double *m2, int dim)`**: Verifies that two matrices are equal.
- **`void print_elapsed_time(struct timeval *start, struct timeval *end, const char *const name)`**: Prints the elapsed time for an operation.

### Matrix Multiplication Functions

- **`void multiply_serial(const double *a, const double *b, double *c, int dim, int num_workers)`**: Performs serial matrix multiplication.
- **`void multiply_parallel_processes(const double *a, const double *b, double *c, int dim, int num_workers)`**: Performs matrix multiplication using parallel processes.
- **`void multiply_parallel_threads(const double *a, const double *b, double *c, int dim, int num_workers)`**: Performs matrix multiplication using parallel threads.

### Running and Timing Functions

- **`void run_and_time(multiply_function multiply_matrices, const double *const a, const double *const b, double *const c, const double *const gold, const int dim, const char *const name, const int num_workers, const bool verify)`**: Runs a matrix multiplication function and times its execution.

## Usage

### Compiling the Project

Use the provided `makefile` to compile the project:

```sh
make

After compilation, run the executable:
./matrix_mult

Cleaning Up

To clean up the build files, use:
make clean


Example Output
The program will output the elapsed time for each of the matrix multiplication methods and verify the correctness of the results.
