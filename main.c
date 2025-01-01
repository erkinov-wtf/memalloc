#include <stdio.h>
#include <string.h>
#include "memalloc.h"

// Test structure to verify struct allocation
struct TestStruct {
    int id;
    char name[50];
    double value;
};

// Structure to track test results
struct TestResult {
    const char* test_name;
    int passed;
    char message[100];
};

// Test struct allocation and manipulation
struct TestResult test_struct_allocation() {
    struct TestResult result = {"Struct Allocation", 0, ""};

    // Allocate structure
    struct TestStruct* test = malloc(sizeof(struct TestStruct));
    if (!test) {
        strcpy(result.message, "Failed to allocate struct");
        return result;
    }

    // Initialize structure
    test->id = 1;
    strcpy(test->name, "Test");
    test->value = 3.14;

    // Verify values
    if (test->id != 1 || strcmp(test->name, "Test") != 0 || test->value != 3.14) {
        strcpy(result.message, "Struct values don't match expected");
        free(test);
        return result;
    }

    free(test);
    result.passed = 1;
    strcpy(result.message, "Struct allocation test passed");
    return result;
}

// Test array allocation with calloc
struct TestResult test_array_allocation() {
    struct TestResult result = {"Array Allocation", 0, ""};

    // Allocate array of 5 TestStructs
    struct TestStruct* array = calloc(5, sizeof(struct TestStruct));
    if (!array) {
        strcpy(result.message, "Failed to allocate array");
        return result;
    }

    // Verify initialization to zero
    for (int i = 0; i < 5; i++) {
        if (array[i].id != 0 || array[i].name[0] != '\0' || array[i].value != 0.0) {
            strcpy(result.message, "Array not properly zeroed");
            free(array);
            return result;
        }
    }

    free(array);
    result.passed = 1;
    strcpy(result.message, "Array allocation test passed");
    return result;
}

// Test realloc functionality
struct TestResult test_reallocation() {
    struct TestResult result = {"Reallocation", 0, ""};

    // Initial allocation
    int* numbers = malloc(3 * sizeof(int));
    if (!numbers) {
        strcpy(result.message, "Initial allocation failed");
        return result;
    }

    // Set initial values
    numbers[0] = 1;
    numbers[1] = 2;
    numbers[2] = 3;

    // Reallocate to larger size
    int* new_numbers = realloc(numbers, 5 * sizeof(int));
    if (!new_numbers) {
        strcpy(result.message, "Reallocation failed");
        free(numbers);
        return result;
    }

    // Verify original values preserved
    if (new_numbers[0] != 1 || new_numbers[1] != 2 || new_numbers[2] != 3) {
        strcpy(result.message, "Original values not preserved after realloc");
        free(new_numbers);
        return result;
    }

    free(new_numbers);
    result.passed = 1;
    strcpy(result.message, "Reallocation test passed");
    return result;
}

// Run all tests and report results
void run_all_tests() {
    struct TestResult results[] = {
            test_struct_allocation(),
            test_array_allocation(),
            test_reallocation()
    };

    int total_tests = sizeof(results) / sizeof(results[0]);
    int passed_tests = 0;

    printf("\n=== Memory Allocator Test Results ===\n");
    for (int i = 0; i < total_tests; i++) {
        printf("\nTest: %s\n", results[i].test_name);
        printf("Status: %s\n", results[i].passed ? "PASSED" : "FAILED");
        printf("Message: %s\n", results[i].message);

        if (results[i].passed) passed_tests++;
    }

    printf("\nSummary: %d/%d tests passed\n", passed_tests, total_tests);
}

int main() {
    run_all_tests();

    int* p1 = malloc(sizeof(int));
    *p1 = 42;
    free(p1);  // No leak

    // Test 2: Deliberate leak
    int* p2 = malloc(sizeof(int) * 5);

    // Test 3: Another leak
    char* str = malloc(20);
    strcpy(str, "Will be leaked");

    print_memory_leaks();
    return 0;
    return 0;
}