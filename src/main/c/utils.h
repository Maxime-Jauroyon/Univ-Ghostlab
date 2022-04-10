#ifndef GHOSTLAB_UTILS_H
#define GHOSTLAB_UTILS_H

#include "types.h"

#define gl_is_tcp_terminator(c) ((c) == GHOSTLAB_TCP_TERMINATOR)
#define gl_is_udp_terminator(c) ((c) == GHOSTLAB_UDP_TERMINATOR)
#define gl_is_separator(c) ((c) == GHOSTLAB_SEPARATOR)
#define gl_is_terminator(c) (gl_is_tcp_terminator(c) || gl_is_udp_terminator(c))
#define gl_is_separator_or_terminator(c) (gl_is_separator(c) || gl_is_terminator(c))

// Checks `errno` to call `perror` if needed and returns `0`.
int gl_error_get(int err);

uint16_t gl_uint8_to_uint16(const uint8_t *n, gl_conversion_type_t conversion_type);

uint32_t gl_uint8_to_uint32(const uint8_t *n, gl_conversion_type_t conversion_type);

uint64_t gl_uint8_to_uint64(const uint8_t *n, gl_conversion_type_t conversion_type);

int gl_uint8_write(uint8_t **buf, const uint8_t *n);

int gl_uint16_write(uint8_t **buf, const uint16_t *n, gl_conversion_type_t conversion_type);

int gl_uint32_write(uint8_t **buf, const uint32_t *n, gl_conversion_type_t conversion_type);

int gl_uint64_write(uint8_t **buf, const uint64_t *n, gl_conversion_type_t conversion_type);

int gl_uint8_read(int fd, uint8_t *n);

int gl_uint8_read_until_separator(int fd, uint8_t **dst, uint8_t *last_c, uint16_t max_size, bool precise_size, bool allow_spaces);

#endif /* GHOSTLAB_UTILS_H */
