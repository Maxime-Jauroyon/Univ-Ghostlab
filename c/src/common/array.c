#include <common/array.h>
#include <string.h>
#include <common/memory.h>

void *internal_gl_array_set_capacity(void *array, uint64_t capacity, uint64_t item_size) {
    uint8_t *header = array ? (uint8_t *)gl_array_get_header(array) : 0;
    
    const uint64_t header_size = sizeof(gl_array_header_t);
    const uint64_t size = gl_array_get_size(array);
    const uint64_t memory = capacity ? item_size * capacity + header_size : 0;
    
    header = (uint8_t *)gl_realloc(header, memory);
    
    if (!header) {
        return 0;
    }
    
    ((gl_array_header_t *)header)->size = size < capacity ? size : capacity;
    ((gl_array_header_t *)header)->capacity = capacity;
    
    return header + header_size;
}

void *internal_gl_array_grow(void *array, uint64_t to_at_least, uint64_t item_size) {
    const uint64_t capacity = gl_array_get_capacity(array);
    
    if (capacity >= to_at_least) {
        return array;
    }
    
    const uint64_t min_new_capacity = capacity ? capacity * 2 : 16;
    const uint64_t new_capacity = min_new_capacity > to_at_least ? min_new_capacity : to_at_least;
    
    return internal_gl_array_set_capacity(array, new_capacity, item_size);
}

void *internal_gl_array_create(const void *ptr, uint64_t size, uint64_t item_size) {
    void *array = internal_gl_array_set_capacity(0, size, item_size);
    gl_array_get_header(array)->size = size;
    
    if (ptr) {
        gl_memcpy(array, ptr, size * item_size);
    } else {
        memset(array, 0, size * item_size);
    }
    
    return array;
}

void internal_gl_array_free(void *array) {
    uint8_t *header = array ? (uint8_t *)gl_array_get_header(array) : 0;
    
    if (header == 0) {
        return;
    }
    
    gl_free(header);
}
