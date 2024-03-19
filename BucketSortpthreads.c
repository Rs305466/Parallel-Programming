#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    float *array;
    int size;
    int capacity;
} Bucket;

typedef struct {
    Bucket *bucket;
} ThreadArg;

void initBucket(Bucket *bucket) {
    bucket->capacity = 4; // Initial capacity
    bucket->size = 0;
    bucket->array = (float *)malloc(bucket->capacity * sizeof(float));
}

void addToBucket(Bucket *bucket, float element) {
    if (bucket->size == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->array = (float *)realloc(bucket->array, bucket->capacity * sizeof(float));
    }
    bucket->array[bucket->size++] = element;
}

int compare(const void *a, const void *b) {
    return (*(float *)a - *(float *)b);
}

void *sortBucket(void *arg) {
    ThreadArg *threadArg = (ThreadArg *)arg;
    Bucket *bucket = threadArg->bucket;
    if (bucket->size > 1) { // Only sort if there are at least 2 elements
        qsort(bucket->array, bucket->size, sizeof(float), compare);
    }
    pthread_exit(NULL);
}

void bucketSort(float arr[], int n) {
    Bucket buckets[n];
    pthread_t threads[n];
    ThreadArg threadArgs[n];

    for (int i = 0; i < n; i++) {
        initBucket(&buckets[i]);
    }

    for (int i = 0; i < n; i++) {
        int bucketIdx = n * arr[i];
        addToBucket(&buckets[bucketIdx], arr[i]);
    }

    for (int i = 0; i < n; i++) {
        threadArgs[i].bucket = &buckets[i];
        pthread_create(&threads[i], NULL, sortBucket, (void *)&threadArgs[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    int index = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < buckets[i].size; j++) {
            arr[index++] = buckets[i].array[j];
        }
        free(buckets[i].array);
    }
}

int main() {
    int n = 100; // Increased array size to 100
    float arr[n];

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        arr[i] = (float)rand() / (float)(RAND_MAX);
    }

    printf("Original array: \n");
    for (int i = 0; i < n; i++) {
        printf("%f ", arr[i]);
    }
    printf("\n");

    clock_t start = clock();

    bucketSort(arr, n);

    clock_t end = clock();

    printf("Sorted array: \n");
    for (int i = 0; i < n; i++) {
        printf("%f ", arr[i]);
    }
    printf("\n");

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent sorting: %f seconds\n", time_spent);

    return 0;
}
