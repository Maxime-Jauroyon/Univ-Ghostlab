#include "memory.h"
#include <string.h>

void *gl_memmove(void *dst, const void *src, uint64_t size) {
    return memmove(dst, src, size);
}

void *gl_memcpy(void *dst, const void *src, size_t size) {
    return memcpy(dst, src, size);
}