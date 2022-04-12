#ifndef GHOSTLAB_MEMORY_H
#define GHOSTLAB_MEMORY_H

#include "types.h"

void *gl_malloc(uint64_t size);
void *gl_calloc(uint64_t count, uint64_t size);
void gl_free(void *ptr);
void *gl_memmove(void *dst, const void *src, uint64_t size);
void *gl_memcpy(void *dst, const void *src, size_t size);
void *gl_realloc(void *ptr, size_t size);
void *gl_strdup(void *ptr);
void gl_memory_check_for_leaks();

#endif /* GHOSTLAB_MEMORY_H */
