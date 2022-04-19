#ifndef GHOSTLAB_ARRAY_H
#define GHOSTLAB_ARRAY_H

#include <common/types.h>

void *gl_memmove(void *dst, const void *src, uint64_t size);

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

// Initialies an array with a specific size.
#define gl_array_create(t, n) internal_gl_array_create(0, n, sizeof(t))

// Initialies an array from a C array pointer.
#define gl_array_create_from_carray(ptr, n) internal_gl_array_create(ptr, n, sizeof(*(ptr)))

// Returns the header of the array.
#define gl_array_get_header(a) ((gl_array_header_t *)((uint8_t *)(a) - sizeof(gl_array_header_t)))

// Returns the size of the array.
#define gl_array_get_size(a) ((a) ? gl_array_get_header(a)->size : 0)

// Sets the capacity of the array (must be higher than current capacity).
#define gl_array_set_capacity(a, n) ((*(void **)&(a)) = _gl_array_set_capacity((void *)(a), n, sizeof(*(a))))

// Returns the capacity of the array.
#define gl_array_get_capacity(a) ((a) ? gl_array_get_header(a)->capacity : 0)

// Returns the first element of the array.
#define gl_array_get_first(a) ( (a)[0])

// Returns the last element of the array.
#define gl_array_get_last(a) ((a)[gl_array_get_size(a) - 1])

// Returns `true` if the array is empty.
#define gl_array_is_empty(a) (!gl_array_get_size(a))

// Ensures that the array is at least of capacity `n`.
// If it isn't, the array grows geometrically to become able to host at least `n` items.
#define gl_array_ensure(a, n) ((n) > gl_array_get_capacity(a) ? (*(void **)&(a)) = internal_gl_array_grow((void *)(a), n, sizeof(*(a))) : 0)

// Pushes the item at the end of the array (it will grow if needed).
#define gl_array_push(a, item) (gl_array_ensure(a, gl_array_get_size(a) + 1), (a)[gl_array_get_header(a)->size++] = (item), (a) + gl_array_get_header(a)->size - 1)

// Inserts the item at the given index of the array (it will grow if needed).
#define gl_array_insert(a, item, i) (gl_array_push(a, item), gl_memmove((a) + (i) + 1, (a) + (i), (gl_array_get_size(a) - (i) - 1) * sizeof(*(a))), (a)[i] = (item), (a) + (i))

// Pops the last item of the array.
#define gl_array_pop(array) ((array)[--gl_array_get_header(array)->size])

// Removes the item at the given index of the array.
#define gl_array_remove(a, i) (gl_memmove((a) + (i), (a) + (i) + 1, (gl_array_get_size(a) - (i) - 1) * sizeof(*(a))), (--gl_array_get_header(a)->size), (void *)0)

// Frees the data of an array (must be called to deallocated memory).
#define gl_array_free(a) (internal_gl_array_free((void *)(a)), (*(void **)&(a)) = 0)

// Internal function.
// Sets the capacity of the array for a given type.
void *internal_gl_array_set_capacity(void *array, uint64_t capacity, uint64_t item_size);

// Internal function.
// Geometrically grows the array to hold at least a given number of items of a given type.
void *internal_gl_array_grow(void *array, uint64_t to_at_least, uint64_t item_size);

// Internal function.
// Initializes an array from a C array pointer.
void *internal_gl_array_create(const void *ptr, uint64_t size, uint64_t item_size);

// Internal function.
// Frees data of the array.
void internal_gl_array_free(void *array);

#endif /* GHOSTLAB_ARRAY_H */
