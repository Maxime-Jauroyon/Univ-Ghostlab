#ifndef GHOSTLAB_MEMORY_H
#define GHOSTLAB_MEMORY_H

#include <common/types.h>

// Custom implementation of malloc to increment the number of allocations made.
void *gl_malloc(uint64_t size);

// Custom implementation of calloc to increment the number of allocations made.
void *gl_calloc(uint64_t count, uint64_t size);

// Custom implementation of free to decrement the number of allocations made.
void gl_free(void *ptr);

// Custom implementation of memmove.
void *gl_memmove(void *dst, const void *src, uint64_t size);

// Custom implementation of memcpy to increment the number of allocations made.
void *gl_memcpy(void *dst, const void *src, size_t size);

// Custom implementation of realloc.
void *gl_realloc(void *ptr, size_t size);

// Custom implementation of strdup to increment the number of allocations made.
void *gl_strdup(void *ptr);

// Checks if any memory leaks have happened (the number of allocations is not equal to 0).
void gl_memory_check_for_leaks();

#endif /* GHOSTLAB_MEMORY_H */
