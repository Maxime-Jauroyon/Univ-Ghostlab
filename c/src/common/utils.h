#ifndef GHOSTLAB_UTILS_H
#define GHOSTLAB_UTILS_H

#include <common/types.h>

// Checks if a character is a TCP terminator "*".
#define gl_is_tcp_terminator(c) ((c) == GHOSTLAB_TCP_TERMINATOR)

// Checks if a character is a UDP terminator "+".
#define gl_is_udp_terminator(c) ((c) == GHOSTLAB_UDP_TERMINATOR)

// Checks if a character is a separator " ".
#define gl_is_separator(c) ((c) == GHOSTLAB_SEPARATOR)

// Checks if a character is a terminator.
#define gl_is_terminator(c) (gl_is_tcp_terminator(c) || gl_is_udp_terminator(c))

// Checks if a character is a separator or a terminator.
#define gl_is_separator_or_terminator(c) (gl_is_separator(c) || gl_is_terminator(c))

// Converts a string to uint16_t.
uint16_t gl_string_to_uint16(const uint8_t *n, gl_conversion_type_t conversion_type);

// Converts a string to uint32_t.
uint32_t gl_string_to_uint32(const uint8_t *n, gl_conversion_type_t conversion_type);

// Converts a string to uint64_t.
uint64_t gl_string_to_uint64(const uint8_t *n, gl_conversion_type_t conversion_type);

// Writes an uint8_t in a buffer.
int32_t gl_uint8_write(uint8_t **buf, const uint8_t *n);

// Writes an uint16_t in a buffer.
int32_t gl_uint16_write(uint8_t **buf, const uint16_t *n, gl_conversion_type_t conversion_type);

// Writes an uint32_t in a buffer.
int32_t gl_uint32_write(uint8_t **buf, const uint32_t *n, gl_conversion_type_t conversion_type);

// Writes an uint64_t in a buffer.
int32_t gl_uint64_write(uint8_t **buf, const uint64_t *n, gl_conversion_type_t conversion_type);

// Receives an uint8_t from a socket.
int32_t gl_uint8_recv(int32_t fd, uint8_t *n);

// Receives a list of uint8_t from a socket until a separator is met.
int32_t gl_uint8_array_recv_until_separator(int32_t fd, uint8_t **dst, uint8_t *last_c, uint16_t max_size, bool precise_size, bool allow_spaces, const uint8_t *buf, uint32_t *i);

// Returns a random number between [min, max] ranges.
uint32_t gl_rand(uint32_t min, uint32_t max);

// Checks if a given C-string is a valid IP format.
bool gl_is_ip_valid(const char *src);

// Checks if a given C-string is a valid number format.
bool gl_is_number_valid(const char *src, uint32_t size);

// Take the number of ticks at the start of the program.
void gl_start_ticking();

// Gets the current number of tick.
uint32_t gl_get_ticks();

// Sleeps a certain time in ms.
void gl_sleep(uint32_t ms);

#endif /* GHOSTLAB_UTILS_H */
