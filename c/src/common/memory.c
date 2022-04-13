#include <common/memory.h>
#include <string.h>
#include <stdlib.h>
#include <common/print.h>

static uint64_t g_allocated_ptrs = 0;

void *gl_malloc(uint64_t size) {
    void *ptr = malloc(size);
    
    if (ptr) {
        g_allocated_ptrs++;
    } else {
        gl_printf_error("couldn't allocate memory!");
    }
    
    return ptr;
}

void *gl_calloc(uint64_t count, uint64_t size) {
    void *ptr = calloc(count, size);
    
    if (ptr) {
        g_allocated_ptrs++;
    } else {
        gl_printf_error("couldn't zero allocate memory!");
    }
    
    
    return ptr;
}

void gl_free(void *ptr) {
    if (ptr) {
        g_allocated_ptrs--;
        free(ptr);
    }
    
}

void *gl_memmove(void *dst, const void *src, uint64_t size) {
    void *ptr = memmove(dst, src, size);
    
    if (!ptr) {
        gl_printf_error("couldn't move memory!");
    }
    
    return ptr;
}

void *gl_memcpy(void *dst, const void *src, size_t size) {
    void *ptr = memcpy(dst, src, size);
    
    if (ptr) {
        g_allocated_ptrs++;
    } else {
        gl_printf_error("couldn't copy memory!");
    }
    
    return ptr;
}

void *gl_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    
    if (new_ptr) {
        if (!ptr) {
            g_allocated_ptrs++;
        }
    } else {
        gl_printf_error("couldn't copy memory!");
    }
    
    return new_ptr;
}

void *gl_strdup(void *ptr) {
    void *new_ptr = strdup(ptr);
    
    if (new_ptr) {
        g_allocated_ptrs++;
    } else {
        gl_printf_error("couldn't duplicate string!");
    }
    
    return new_ptr;
}

void gl_memory_check_for_leaks() {
    if (g_allocated_ptrs > 0) {
#ifdef __APPLE__
        gl_printf_error("%llu pointer(s) still allocated!", g_allocated_ptrs);
#else
        gl_printf_error("%lu pointer(s) still allocated!", g_allocated_ptrs);
#endif
    }
}
