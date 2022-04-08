#ifndef GHOSTLAB_ARRAY_H
#define GHOSTLAB_ARRAY_H

#include "types.h"

// An array is a pointer of elements (like a C array) that is prefixed by its size and capacity in memory.
//
// Example:
// ```
// char **a = 0;
// char *e1 = "Hello, ";
// gl_array_push(a, e1);
// char *e2 = "World!";
// gl_array_push(a, e2);
// ```
//
// Here's how it would look inside the memory:
// |16|2|"hello, "|"world!"|
//  ↑  ↑ ↑         ↑
//  │  │ │         └──────────────────────────────────────────────────────┐
//  │  │ └──────────────────────────────────────────────────────────┐     │
//  │  └──────────────────────────────┐                             │     │
//  gl_array_get_header(a)->capacity  │                             │     │
//                                    gl_array_get_header(a)->size  │     │
//                                                                  a[0]  a[1]
//
// This array concept was inspired by stb's stretchy buffer (https://github.com/nothings/stb/blob/master/deprecated/stretchy_buffer.txt).

// Defines the header of an array (informations about its data).
typedef struct gl_array_header_t {
    uint64_t capacity;
    uint64_t size;
} gl_array_header_t;

// Initialies an array from a C array pointer.
#define gl_array_create_from_carray(ptr, n) internal_gl_array_create_from_carray(ptr, n, sizeof(*(ptr)))

// Returns the header of the array.
#define gl_array_get_header(a) ((gl_array_header_t *)((uint8_t *)(a) - sizeof(gl_array_header_t)))

// Returns the size of the array.
#define gl_array_get_size(a) ((a) ? gl_array_get_header(a)->size : 0)

// Sets the capacity of the array (must be higher than current capacity).
#define gl_array_set_capacity(a, n) ((*(void **)&(a)) = _gl_array_set_capacity((void *)(a), n, sizeof(*(a))))

// Returns the capacity of the array.
#define gl_array_get_capacity(a) ((a) ? gl_array_get_header(a)->capacity : 0)

// Ensures that the array is at least of capacity `n`.
// If it isn't, the array grows geometrically to become able to host at least `n` items.
#define gl_array_ensure(a, n) ((n) > gl_array_get_capacity(a) ? (*(void **)&(a)) = internal_gl_array_grow((void *)(a), n, sizeof(*(a))) : 0)

// Pushes the item at the end of the array (it will grow if needed).
#define gl_array_push(a, item) (gl_array_ensure(a, gl_array_get_size(a) + 1), (a)[gl_array_get_header(a)->size++] = (item), (a) + gl_array_get_header(a)->size - 1)

// Inserts the item at the given index of the array (it will grow if needed).
#define gl_array_insert(a, item, i) (gl_array_push(a, item), memmove((a) + (i) + 1, (a) + (i), (gl_array_get_size(a) - (i) - 1) * sizeof(*(a))), (a)[i] = (item), (a) + (i))

// Pops the last item of the array.
#define gl_array_pop(array) ((array)[--gl_array_get_header(array)->size])

// Removes the item at the given index of the array.
#define gl_array_remove(a, i) (((a) ? memmove((a) + (i), (a) + (i) + 1, (gl_array_get_size(a) - (i) - 1 * sizeof(*(a))) : 0), (a) ? --gl_array_get_header(a)->size : 0)

// Frees the data of an array (must be called to deallocated memory).
#define gl_array_free(a) ((*(void **)&(a)) = internal_gl_array_free((void *)(a)))

// Internal function.
// Sets the capacity of the array for a given type.
static inline void *internal_gl_array_set_capacity(void *array, uint64_t capacity, uint64_t item_size) {
    uint8_t *header = array ? (uint8_t *)gl_array_get_header(array) : 0;
    
    const uint64_t header_size = sizeof(gl_array_header_t);
    const uint64_t size = gl_array_get_size(array);
    const uint64_t memory = capacity ? item_size * capacity + header_size : 0;
    
    header = (uint8_t *)realloc(header, memory);
    
    if (!header) {
        return 0;
    }
    
    ((gl_array_header_t *)header)->size = size < capacity ? size : capacity;
    ((gl_array_header_t *)header)->capacity = capacity;
    
    return header + header_size;
}

// Internal function.
// Geometrically grows the array to hold at least a given number of items of a given type.
static inline void *internal_gl_array_grow(void *array, uint64_t to_at_least, uint64_t item_size) {
    const uint64_t capacity = gl_array_get_capacity(array);
    
    if (capacity >= to_at_least) {
        return array;
    }
    
    const uint64_t min_new_capacity = capacity ? capacity * 2 : 16;
    const uint64_t new_capacity = min_new_capacity > to_at_least ? min_new_capacity : to_at_least;
    
    return internal_gl_array_set_capacity(array, new_capacity, item_size);
}

// Internal function.
// Initializes an array from a C array pointer.
static inline void *internal_gl_array_create_from_carray(const void *ptr, uint64_t size, uint64_t item_size) {
    void *array = internal_gl_array_set_capacity(0, size, item_size);
    gl_array_get_header(array)->size = size;
    memcpy(array, ptr, size * item_size);
    
    return array;
}

// Internal function.
// Frees data of the array.
static inline void *internal_gl_array_free(void *array) {
    uint8_t *header = array ? (uint8_t *)gl_array_get_header(array) : 0;
    
    if (header == 0) {
        return 0;
    }
    
    free(header);
    
    return 0;
}

#endif /* GHOSTLAB_ARRAY_H */
