#ifndef GHOSTLAB_STRING_H
#define GHOSTLAB_STRING_H

#include <common/types.h>

// Custom dynamic string using dynamic arrays, made to support ASCII characters only.

// Creates a string from a C-string.
uint8_t *gl_string_create_from_cstring(const char *src);

// Creates a string from a C-string number format.
uint8_t *gl_string_create_from_number(const char *src, uint8_t size);

// Creates a string from an uint32_t value.
uint8_t *gl_string_create_from_uint32(uint32_t value, uint8_t size);

// Creates a string from an C-string IP format.
uint8_t *gl_string_create_from_ip(const char *src);

// Copies a string.
uint8_t *gl_string_copy(const uint8_t *src);

// Creates a C-string from a string.
char *gl_cstring_create_from_string(const uint8_t *src);

// Creates a C-string from a string IP format.
char *gl_cstring_create_from_ip(const uint8_t *src);

// Writes a given string in a buffer.
int32_t gl_write_string(uint8_t **buf, const uint8_t **n);

// Writes a given C-string in a buffer.
int32_t gl_write_cstring(uint8_t **buf, const char **n);

// Logs a string.
void gl_string_printf(uint8_t **str);

// Converts a string to an int64_t.
int64_t gl_string_strtol(const uint8_t *str);



#endif /* GHOSTLAB_STRING_H */
