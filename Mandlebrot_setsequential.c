#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 800
#define MAX_ITER 1000

typedef struct {
    double real;
    double imag;
} Complex;

int mandelbrot(Complex c) {
    Complex z = {0, 0};
    int n;

    for (n = 0; n < MAX_ITER; n++) {
        if (z.real * z.real + z.imag * z.imag > 4.0)
            return n;
        double real_temp = z.real * z.real - z.imag * z.imag + c.real;
        double imag_temp = 2 * z.real * z.imag + c.imag;
        z.real = real_temp;
        z.imag = imag_temp;
    }

    return MAX_ITER;
}

void saveBitmap(const char *filename, unsigned char *data, int width, int height) {
    FILE *f;
    int filesize = 54 + 3 * width * height;

    unsigned char header[54] = {
        'B', 'M',
        filesize & 0xFF, (filesize >> 8) & 0xFF, (filesize >> 16) & 0xFF, (filesize >> 24) & 0xFF,
        0, 0, 0, 0,
        54, 0, 0, 0,
        40, 0, 0, 0,
        width & 0xFF, (width >> 8) & 0xFF, (width >> 16) & 0xFF, (width >> 24) & 0xFF,
        height & 0xFF, (height >> 8) & 0xFF, (height >> 16) & 0xFF, (height >> 24) & 0xFF,
        1, 0, 24, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    f = fopen(filename, "wb");
    if (f == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fwrite(header, 1, 54, f);
    fwrite(data, 1, 3 * width * height, f);

    fclose(f);
}

int main() {
    unsigned char *imageData = (unsigned char *)malloc(3 * WIDTH * HEIGHT);
    if (imageData == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    double minX = -2.0;
    double maxX = 1.0;
    double minY = -1.5;
    double maxY = 1.5;

    clock_t start_time = clock(); // Start timer

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Complex c;
            c.real = minX + (maxX - minX) * x / (WIDTH - 1);
            c.imag = minY + (maxY - minY) * y / (HEIGHT - 1);

            int value = mandelbrot(c);
            unsigned char color[3];
            color[0] = value % 8 * 32;
            color[1] = value % 16 * 16;
            color[2] = value % 32 * 8;

            int index = 3 * (x + y * WIDTH);
            imageData[index] = color[0];
            imageData[index + 1] = color[1];
            imageData[index + 2] = color[2];
        }
    }

    clock_t end_time = clock(); // Stop timer
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Print the execution time to the terminal
    printf("Execution Time: %lf seconds\n", execution_time);

    // Write execution time to an external file
    FILE *time_file = fopen("execution_time_sequential.txt", "w");
    if (time_file == NULL) {
        perror("Error opening time file");
        return 1;
    }
    fprintf(time_file, "Execution Time: %lf seconds\n", execution_time);
    fclose(time_file);

    saveBitmap("mandelbrotsequential.bmp", imageData, WIDTH, HEIGHT);

    free(imageData);

    return 0;
}

gcc -o mandelbrot_sequential mandelbrot_sequential.c
./mandelbrot_sequential
