#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MATRIX_SIZE 1000
#define NUM_THREADS 2

int matrix1[MATRIX_SIZE][MATRIX_SIZE];
int matrix2[MATRIX_SIZE][MATRIX_SIZE];
int result[MATRIX_SIZE][MATRIX_SIZE];

void generate_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void multiply_rows(int row_start, int row_end, int local_result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            local_result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                local_result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main() {
    // Generate random matrices
    generate_matrix(matrix1);
    generate_matrix(matrix2);

    // Record the start time using clock_gettime
    struct timespec start_time, end_time;
    double cpu_time_used;

    if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Set the number of threads
    omp_set_num_threads(NUM_THREADS);

    // Set the number of rows each thread will handle
    int rows_per_thread = MATRIX_SIZE / NUM_THREADS;

    // Parallelize the matrix multiplication
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int row_start = thread_id * rows_per_thread;
        int row_end = (thread_id + 1) * rows_per_thread;

        // Allocate a temporary matrix for each thread
        int local_result[MATRIX_SIZE][MATRIX_SIZE];

        // Multiply rows for each thread
        multiply_rows(row_start, row_end, local_result);

        // Combine the results using a critical section
        #pragma omp critical
        {
            for (int i = row_start; i < row_end; i++) {
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    result[i][j] = local_result[i][j];
                }
            }
        }
    }

    // Record the end time
    if (clock_gettime(CLOCK_MONOTONIC, &end_time) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Calculate the wall-clock time used
    cpu_time_used = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Execution Time: %f seconds\n", cpu_time_used);

    // Print the result matrix
    

    return 0;
}
