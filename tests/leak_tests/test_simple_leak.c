// test_simple_leak.c
#include "../../src/memalloc.h"
#include <assert.h>
#include <stdio.h>

void test_leak_single() {
    void* ptr = malloc(16);
    (void)ptr;
    // Deliberately not freeing to simulate leak
}

void test_leak_multiple() {
    void* ptr1 = malloc(32);
    void* ptr2 = malloc(64);
    (void)ptr1;
    (void)ptr2;
    // Deliberately not freeing to simulate leak
}

void test_no_leak() {
    void* ptr = malloc(128);
    assert(ptr != NULL);
    free(ptr);
}

void run_leak_tests() {
    test_leak_single();
    test_leak_multiple();
    test_no_leak();
    print_memory_leaks();
    printf("Leak tests completed!\n");
}