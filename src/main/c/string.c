#include "string.h"
#include <string.h>
#include <printf.h>
#include "array.h"
#include "utils.h"

uint8_t *gl_string_create_from_cstring(const char *src) {
    uint8_t *dst = 0;
    
    for (uint64_t i = 0; i < strlen(src); i++) {
        gl_array_push(dst, (uint8_t)(src)[i]);
    }
    
    return dst;
}

int gl_write_string(uint8_t **buf, const uint8_t **n) {
    for (uint64_t i = 0; i < gl_array_get_size(*n); i++) {
        gl_uint8_write(buf, &(*n)[i]);
    }
    
    return gl_array_get_size(*n);
}

int gl_write_cstring(uint8_t **buf, const char **n) {
    for (uint64_t i = 0; i < strlen(*n); i++) {
        gl_uint8_write(buf, (const uint8_t *)&(*n)[i]);
    }
    
    return (int)strlen(*n);
}

int gl_string_printf(uint8_t **str) {
    for (uint64_t i = 0; i < gl_array_get_size(*str); i++) {
        printf("%c", (*str)[i]);
    }
    
    return 0;
}
