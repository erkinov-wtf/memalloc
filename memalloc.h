#ifndef MEMORY_ALLOC_MEMALLOC_H
#define MEMORY_ALLOC_MEMALLOC_H

#include <stddef.h>

void* malloc(size_t size);
void free(void* block);
void* calloc(size_t num, size_t nsize);
void* realloc(void* block, size_t size);

// debug funcs
void* debug_malloc(size_t size, const char* file, int line);
void debug_free(void* ptr, const char* file, int line);
void print_memory_leaks(void);

#define malloc(size) debug_malloc(size, __FILE__, __LINE__)
#define free(ptr) debug_free(ptr, __FILE__, __LINE__)

#endif