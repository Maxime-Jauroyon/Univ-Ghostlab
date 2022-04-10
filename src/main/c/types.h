#ifndef GHOSTLAB_TYPES_H
#define GHOSTLAB_TYPES_H

// Includes integer types (`uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `int8_t`, `int16_t`, `int32_t`, `int64_t`).
#include <stdint.h>
// Include boolean type.
#include <stdbool.h>
// Include variadic types.
#include <stdarg.h>
// Include `size_t`.
#include <stddef.h>

// The name of the currently built executable (used for logging purposes).
#ifdef GHOSTLAB_CLIENT
#define GHOSTLAB_EXECUTABLE_NAME "client"
#else
#define GHOSTLAB_EXECUTABLE_NAME "server"
#endif
#define GHOSTLAB_VERSION "1.0.0"
#define GHOSTLAB_DEFAULT_SERVER_IP "127.0.0.1"
#define GHOSTLAB_DEFAULT_SERVER_PORT "4785"
#define GHOSTLAB_DEFAULT_MULTICAST_IP "231.1.2.4"
#define GHOSTLAB_DEFAULT_MULTICAST_PORT "7759"
#define GHOSTLAB_DEFAULT_UDP_PORT "5541"
#define GHOSTLAB_TCP_TERMINATOR '*'
#define GHOSTLAB_UDP_TERMINATOR '+'
#define GHOSTLAB_SEPARATOR ' '

// Prints an error message.
#define gl_error(message) fprintf(stderr, GHOSTLAB_EXECUTABLE_NAME ": " message)

// Asserts a condition that returns `-1` in case of error.
#define gl_assert(condition) if (!(condition)) { return -1; } (void)0

typedef enum gl_conversion_type_t {
    GL_CONVERSION_TYPE_AUTOMATIC,
    GL_CONVERSION_TYPE_BIG_ENDIAN,
    GL_CONVERSION_TYPE_LITTLE_ENDIAN
} gl_conversion_type_t;

#endif /* GHOSTLAB_TYPES_H */
