#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <printf.h>

struct header_t {
    size_t size;
    unsigned is_free;
    struct header_t *next; // linked list to keep track our malloc
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

header_t *head, *tail;
pthread_mutex_t global_malloc_lock;

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
    size_t total_size;
    void *block;
    header_t *header;
    if (!size) {
        return NULL;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);
    if (header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void *) (header + 1);
    }

    total_size = sizeof(header_t) + size;
    block = sbrk(total_size);
    if (block == (void *) -1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;
    if (!head) {
        head = header;
    }
    if (tail) {
        tail->s.next = header;
    }
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *) (header + 1);
}

void free(void *block) {
    header_t *header, *tmp;
    void *program_break;

    if (!block) {
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = (header_t *) block - 1;

    program_break = sbrk(0);
    if ((char *) block + header->s.size == program_break) {
        if (head == tail) {
            head = NULL;
            tail = NULL;
        } else {
            tmp = head;
            while (tmp) {
                if (tmp->s.next == tail) {
                    tmp->s.next = NULL;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - sizeof(header_t) - header->s.size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }
    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

void *calloc(size_t num, size_t n_size) {
    size_t size;
    void *block;

    if (!num || !n_size) {
        return NULL;
    }

    size = num * n_size;
    if (n_size != size / num) {
        return NULL;
    }
    block = malloc(size);
    if (!block) {
        return NULL;
    }
    memset(block, 0, size);
    return block;
}

void *realloc(void *block, size_t size) {
    header_t *header;
    void *ret;
    if (!block || !size) {
        return malloc(size);
    }
    header = (header_t *) block - 1;
    if (header->s.size >= size) {
        return block;
    }
    ret = malloc(size);
    if (ret) {
        memcpy(ret, block, header->s.size);
        free(block);
    }
    return ret;
}

// Debugging tool for memory leak
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
