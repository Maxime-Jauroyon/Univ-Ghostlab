#ifndef GHOSTLAB_STRING_H
#define GHOSTLAB_STRING_H

#include <common/types.h>

uint8_t *gl_string_create_from_cstring(const char *src);

uint8_t *gl_string_create_from_number(const char *src, uint8_t size);

uint8_t *gl_string_create_from_uint32(uint32_t value, uint8_t size);

uint8_t *gl_string_create_from_ip(const char *src);

uint8_t *gl_string_copy(const uint8_t *src);

char *gl_cstring_create_from_string(const uint8_t *src);

char *gl_cstring_create_from_ip(const uint8_t *src);

int32_t gl_write_string(uint8_t **buf, const uint8_t **n);

int32_t gl_write_cstring(uint8_t **buf, const char **n);

void gl_string_printf(uint8_t **str);

int64_t gl_string_strtol(const uint8_t *str);



#endif /* GHOSTLAB_STRING_H */
