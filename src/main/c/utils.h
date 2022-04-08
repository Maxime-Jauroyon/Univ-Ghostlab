#ifndef GHOSTLAB_UTILS_H
#define GHOSTLAB_UTILS_H

#include "types.h"

struct gl_message_t;

// Checks `errno` to call `perror` if needed and returns `EXIT_FAILURE`.
int gl_get_error();

int gl_write_uint8(gl_buffer_t *buf, const uint8_t *n);

int gl_write_uint16(gl_buffer_t *buf, const uint16_t *n);

int gl_read_message(int fd, struct gl_message_t *dst);

#endif /* GHOSTLAB_UTILS_H */
