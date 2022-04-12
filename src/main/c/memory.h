#ifndef GHOSTLAB_MEMORY_H
#define GHOSTLAB_MEMORY_H

#include "types.h"

void *gl_memmove(void *dst, const void *src, uint64_t size);
void *gl_memcpy(void *dst, const void *src, size_t size);

#endif /* GHOSTLAB_MEMORY_H */
