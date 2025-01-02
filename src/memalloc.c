#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

#define PAGE_SIZE 4096
#define MIN_ALLOC_SIZE 32

struct header_t {
    size_t size;
    unsigned is_free;
    struct header_t *next;
};

typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;
    ALIGN stub;
};
typedef union header header_t;

header_t *head = NULL, *tail = NULL;
pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

static size_t align_size(size_t size) {
    return (size + sizeof(header_t) + 15) & ~15;
}

static size_t page_align(size_t size) {
    return (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

header_t *get_free_block(size_t size) {
    header_t *curr = head;
    while (curr) {
        if (curr->s.is_free && curr->s.size >= size) {
            return curr;
        }
        curr = curr->s.next;
    }
    return NULL;
}

void *malloc(size_t size) {
    if (!size) return NULL;

    pthread_mutex_lock(&global_malloc_lock);

    // Try to find a free block
    header_t *header = get_free_block(size);
    if (header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    // Calculate required size including header
    size_t aligned_size = align_size(size);
    size_t page_aligned_size = page_align(aligned_size);

    // Map new memory
    void *block = mmap(NULL, page_aligned_size,
                       PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS,
                       -1, 0);

    if (block == MAP_FAILED) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    // Initialize header
    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;

    // Update linked list
    if (!head) {
        head = header;
    }
    if (tail) {
        tail->s.next = header;
    }
    tail = header;

    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
}

void free(void *block) {
    if (!block) return;

    pthread_mutex_lock(&global_malloc_lock);

    header_t *header = (header_t*)block - 1;
    size_t total_size = page_align(align_size(header->s.size));

    // Remove from linked list
    if (header == head) {
        head = header->s.next;
    } else {
        header_t *curr = head;
        while (curr && curr->s.next != header) {
            curr = curr->s.next;
        }
        if (curr) {
            curr->s.next = header->s.next;
        }
        if (header == tail) {
            tail = curr;
        }
    }

    // Unmap the memory
    munmap(header, total_size);

    pthread_mutex_unlock(&global_malloc_lock);
}

void *calloc(size_t num, size_t nsize) {
    size_t size = num * nsize;
    if (!num || !nsize || nsize != size / num) return NULL;

    void *block = malloc(size);
    if (!block) return NULL;

    memset(block, 0, size);
    return block;
}

void *realloc(void *block, size_t size) {
    if (!block) return malloc(size);
    if (!size) {
        free(block);
        return NULL;
    }

    header_t *header = (header_t*)block - 1;
    if (header->s.size >= size) return block;

    void *new_block = malloc(size);
    if (!new_block) return NULL;

    memcpy(new_block, block, header->s.size);
    free(block);
    return new_block;
}

// Your debug functions remain the same
struct allocation_info {
    const char* file;
    int line;
    size_t size;
    void* address;
    struct allocation_info* next;
};

struct allocation_info* allocations = NULL;
pthread_mutex_t debug_lock = PTHREAD_MUTEX_INITIALIZER;

void* debug_malloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);

    pthread_mutex_lock(&debug_lock);
    struct allocation_info* info = malloc(sizeof(struct allocation_info));
    info->file = file;
    info->line = line;
    info->size = size;
    info->address = ptr;
    info->next = allocations;
    allocations = info;
    pthread_mutex_unlock(&debug_lock);

    return ptr;
}

void debug_free(void* ptr, const char* file, int line) {
    pthread_mutex_lock(&debug_lock);
    struct allocation_info** curr = &allocations;
    while (*curr) {
        if ((*curr)->address == ptr) {
            struct allocation_info* to_free = *curr;
            *curr = (*curr)->next;
            free(to_free);
            break;
        }
        curr = &(*curr)->next;
    }
    pthread_mutex_unlock(&debug_lock);
    free(ptr);
}

void print_memory_leaks(void) {
    pthread_mutex_lock(&debug_lock);
    struct allocation_info* curr = allocations;
    size_t total_leaks = 0;
    size_t total_bytes = 0;

    printf("\n=== Memory Leak Report ===\n");
    while (curr) {
        printf("Leak: %zu bytes allocated at %s:%d\n",
               curr->size, curr->file, curr->line);
        total_leaks++;
        total_bytes += curr->size;
        curr = curr->next;
    }
    printf("\nTotal: %zu leaks, %zu bytes\n", total_leaks, total_bytes);
    pthread_mutex_unlock(&debug_lock);
}