#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define N 20  // The position up to which Fibonacci numbers will be calculated

// Struct to hold the data for each thread
typedef struct {
    int start;
    int end;
    long long *fib_array;
} ThreadData;

void *calculate_fibonacci(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start = data->start;
    int end = data->end;
    long long *fib_array = data->fib_array;

    for (int i = start; i <= end; i++) {
        if (i == 0) {
            fib_array[i] = 0;
        } else if (i == 1) {
            fib_array[i] = 1;
        } else {
            fib_array[i] = fib_array[i - 1] + fib_array[i - 2];
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    long long *fib_array = malloc(N * sizeof(long long));

    if (fib_array == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    int segment_size = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? N - 1 : (i + 1) * segment_size - 1;
        thread_data[i].fib_array = fib_array;
        pthread_create(&threads[i], NULL, calculate_fibonacci, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Fibonacci sequence up to position %d:\n", N);
    for (int i = 0; i < N; i++) {
        printf("%lld ", fib_array[i]);
    }
    printf("\n");

    free(fib_array);
    return 0;
}