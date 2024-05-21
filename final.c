#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to hold data for each thread
typedef struct {
    int n;
    long long result;
} ThreadData;

void *calculate_fibonacci(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int n = data->n;

    if (n == 0) {
        data->result = 0;
    } else if (n == 1) {
        data->result = 1;
    } else {
        long long a = 0, b = 1;
        for (int i = 2; i <= n; i++) {
            long long temp = a + b;
            a = b;
            b = temp;
        }
        data->result = b;
    }
    pthread_exit(NULL);
}

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

void *calculate_prime(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int n = data->n;
    int count = 0, num = 2;

    while (count < n) {
        if (is_prime(num)) {
            count++;
        }
        if (count < n) num++;
    }
    data->result = num;
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    ThreadData fib_data, prime_data;

    int n;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    fib_data.n = n;
    prime_data.n = n;

    // Create threads to calculate Fibonacci and prime numbers
    pthread_create(&thread1, NULL, calculate_fibonacci, &fib_data);
    pthread_create(&thread2, NULL, calculate_prime, &prime_data);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Print results
    printf("The %dth Fibonacci number is: %lld\n", n, fib_data.result);
    printf("The %dth prime number is: %lld\n", n, prime_data.result);

    return 0;
}
