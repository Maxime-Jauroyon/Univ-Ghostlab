#ifndef GHOSTLAB_STRING_H
#define GHOSTLAB_STRING_H

#include "types.h"

uint8_t *gl_string_create_from_cstring(const char *src);

int gl_write_string(uint8_t **buf, const uint8_t **n);

int gl_write_cstring(uint8_t **buf, const char **n);

int gl_string_printf(uint8_t **str);

#endif /* GHOSTLAB_STRING_H */
