#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MATRIX_SIZE 1000
#define NUM_THREADS 2

int matrix1[MATRIX_SIZE][MATRIX_SIZE];
int matrix2[MATRIX_SIZE][MATRIX_SIZE];
int result[MATRIX_SIZE][MATRIX_SIZE];

typedef struct {
    int row_start;
    int row_end;
} ThreadData;

void generate_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void* multiply_rows(void* thread_data) {
    ThreadData* data = (ThreadData*)thread_data;

    for (int i = data->row_start; i < data->row_end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // Generate random matrices
    generate_matrix(matrix1);
    generate_matrix(matrix2);

    // Set the number of rows each thread will handle
    int rows_per_thread = MATRIX_SIZE / NUM_THREADS;

    // Record the start time using clock_gettime
    struct timespec start_time, end_time;
    double cpu_time_used;

    if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].row_start = i * rows_per_thread;
        thread_data[i].row_end = (i + 1) * rows_per_thread;

        if (pthread_create(&threads[i], NULL, multiply_rows, (void*)&thread_data[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
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

    return 0;
}
