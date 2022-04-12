#include "string.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "array.h"
#include "utils.h"

uint8_t *gl_string_create_from_cstring(const char *src) {
    uint8_t *dst = 0;
    
    for (uint64_t i = 0; i < strlen(src); i++) {
        gl_array_push(dst, (uint8_t)(src)[i]);
    }
    
    return dst;
}

uint8_t *gl_string_create_from_number(const char *src, uint8_t size) {
    // `src` is not null and not too big.
    if (!src || strlen(src) > size) {
        return 0;
    }
    
    // Every char of `src` is a number.
    for (uint8_t i = 0; i < (uint8_t)strlen(src); i++) {
        if (!isdigit(src[i])) {
            return 0;
        }
    }
    
    uint8_t *dst = 0;
    
    for (uint64_t i = 0; i < size - strlen(src); i++) {
        gl_array_push(dst, '0');
    }
    
    for (uint64_t i = 0; i < strlen(src); i++) {
        gl_array_push(dst, (uint8_t)(src)[i]);
    }
    
    return dst;
}

uint8_t *gl_string_create_from_ip(const char *src) {
    // `src` is not null and not too big.
    if (!src || strlen(src) > 15) {
        return 0;
    }
    
    // The format of `src` is a valid IP address.
    int distance_from_last_point = 0;
    int total_num_points = 0;
    for (uint8_t i = 0; i < (uint8_t)strlen(src); i++) {
        if (isdigit(src[i]) && distance_from_last_point < 3) {
            distance_from_last_point++;
            continue;
        } else if (src[i] == '.' && distance_from_last_point > 0 && total_num_points < 3) {
            distance_from_last_point = 0;
            total_num_points++;
            continue;
        } else {
            return 0;
        }
    }
    
    uint8_t *dst = 0;
    
    for (uint64_t i = 0; i < strlen(src); i++) {
        gl_array_push(dst, (uint8_t)(src)[i]);
    }
    
    for (uint64_t i = 0; i < 15 - strlen(src); i++) {
        gl_array_push(dst, '#');
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
