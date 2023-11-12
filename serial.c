#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 1000

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

void multiply_matrices() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

    // Multiply matrices
    multiply_matrices();

    // Record the end time
    if (clock_gettime(CLOCK_MONOTONIC, &end_time) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Calculate the wall-clock time used
    cpu_time_used = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Execution Time: %f seconds\n", cpu_time_used);

    return 0;
}
