#include "memalloc.h"
#include <stdio.h>

int main() {
    // Demonstrating basic allocation
    int* numbers = (int*)malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) {
        numbers[i] = i + 1;
    }

    // Demonstrating reallocation
    numbers = (int*)realloc(numbers, 10 * sizeof(int));
    for (int i = 5; i < 10; i++) {
        numbers[i] = i + 1;
    }

    // Printing array
    for (int i = 0; i < 10; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    // Cleaning up
    free(numbers);
    print_memory_leaks();
    return 0;
}