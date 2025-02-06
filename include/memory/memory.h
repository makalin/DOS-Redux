#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

// Memory management structures
typedef struct {
    uint32_t base;
    uint32_t size;
    uint32_t type;
} memory_region_t;

// Memory management functions
void init_memory_manager(void);
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);

// Memory information
uint32_t get_total_memory(void);
uint32_t get_free_memory(void);

#endif // MEMORY_H 