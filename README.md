# Custom Memory Allocator

This program implements a custom memory allocator in C, featuring basic memory allocation, deallocation, and
reallocation functions (`malloc`, `free`, `calloc`, and `realloc`). It also includes memory leak detecting tool.
Just found free hours, so did this fucking thing to learn memory stuff

---

## Features

- **Custom Memory Management**: Implements dynamic memory management functions like ``malloc`` in std C library.
- **Thread Safety**: Utilizes `pthread_mutex_t` to ensure thread-safe memory operations.
- **Memory Leak Detection**: Tracks memory allocations and reports leaks with file and line number information.
- **Flexible Testing Shit**: Includes unit tests covering basic allocations, edge cases, and memory leak scenarios.

---

### Build and Test
I've added *[Justfile](https://just.systems)* to automate building and testing
```bash
just build
just test
```
You can use docker version
```bash
just docker-build
just docker-test
```

---

## Usage

### Memory Allocation

```c
void *ptr = malloc(128); // Allocates 128 bytes
free(ptr);               // Frees allocated memory
```

### Debugging Memory Leaks

```c
void *ptr = debug_malloc(128, __FILE__, __LINE__);
debug_free(ptr, __FILE__, __LINE__);
print_memory_leaks();
```

---
