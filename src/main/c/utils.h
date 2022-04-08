#ifndef GHOSTLAB_UTILS_H
#define GHOSTLAB_UTILS_H

#include "types.h"

struct gl_message_t;

// Checks `errno` to call `perror` if needed and returns `EXIT_FAILURE`.
int gl_get_error();

uint8_t *gl_string_from_cstring(const char *src);

int gl_write_message(int fd, struct gl_message_t *dst);

int gl_read_message(int fd, struct gl_message_t *dst);

int gl_printf_string(uint8_t **str);

int gl_printf_message(struct gl_message_t *msg);

#endif /* GHOSTLAB_UTILS_H */
