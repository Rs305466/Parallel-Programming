#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    float *array;
    int size;
    int capacity;
} Bucket;

void initBucket(Bucket *bucket) {
    // Introduce an artificial delay during initialization
    for (int i = 0; i < 1000000; i++) {
        // Busy-wait loop
    }

    bucket->capacity = 4;
    bucket->size = 0;
    bucket->array = (float *)malloc(bucket->capacity * sizeof(float));
}

void addToBucket(Bucket *bucket, float element) {
    // Introduce a random sleep delay
    int sleep_ms = rand() % 100;
    usleep(sleep_ms * 1000); // Sleep in microseconds

    if (bucket->size == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->array = (float *)realloc(bucket->array, bucket->capacity * sizeof(float));
    }
    bucket->array[bucket->size++] = element;
}

int compare(const void *a, const void *b) {
    // Introduce a random delay during comparison
    int sleep_ms = rand() % 10;
    usleep(sleep_ms * 1000); // Sleep in microseconds

    return (*(float *)a - *(float *)b);
}

void bucketSort(float arr[], int n) {
    Bucket buckets[n];

    for (int i = 0; i < n; i++) {
        initBucket(&buckets[i]);
    }

    for (int i = 0; i < n; i++) {
        int bucketIdx = n * arr[i];
        addToBucket(&buckets[bucketIdx], arr[i]);
    }

    int index = 0;
    for (int i = 0; i < n; i++) {
        qsort(buckets[i].array, buckets[i].size, sizeof(float), compare);
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
