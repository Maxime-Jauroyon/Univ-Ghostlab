#ifndef GHOSTLAB_TYPES_H
#define GHOSTLAB_TYPES_H

// Includes integer types (`uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `int8_t`, `int16_t`, `int32_t`, `int64_t`).
#include <stdint.h>
// Include variadic types.
#include <stdarg.h>
// Include `size_t`.
#include <stddef.h>
// Include boolean type.
#include <stdbool.h>

// The name of the currently built executable (used for logging purposes).
#if GHOSTLAB_CLIENT
#ifndef GHOSTLAB_EXECUTABLE_NAME
#define GHOSTLAB_EXECUTABLE_NAME "client"
#endif
#elif GHOSTLAB_SERVER
#ifndef GHOSTLAB_EXECUTABLE_NAME
#define GHOSTLAB_EXECUTABLE_NAME "server"
#endif
#else
#ifndef GHOSTLAB_EXECUTABLE_NAME
#define GHOSTLAB_EXECUTABLE_NAME "app"
#endif
#endif
#define GHOSTLAB_VERSION "1.0.0"
#define GHOSTLAB_DEFAULT_SERVER_IP "127.0.0.1"
#define GHOSTLAB_DEFAULT_SERVER_PORT "4785"
#define GHOSTLAB_DEFAULT_MULTICAST_IP "231.1.2.4"
#define GHOSTLAB_DEFAULT_MULTICAST_PORT "7759"
#define GHOSTLAB_DEFAULT_UDP_IP "127.0.0.1"
#define GHOSTLAB_DEFAULT_UDP_PORT "5541"
#define GHOSTLAB_TCP_TERMINATOR '*'
#define GHOSTLAB_UDP_TERMINATOR '+'
#define GHOSTLAB_SEPARATOR ' '

// Asserts a condition that returns `-1` in case of error.
#define gl_assert(condition) if (!(condition)) { return -1; } (void)0

// Defines the list of data conversions that can be made.
typedef enum gl_conversion_type_t {
    GL_CONVERSION_TYPE_AUTOMATIC,
    GL_CONVERSION_TYPE_BIG_ENDIAN,
    GL_CONVERSION_TYPE_LITTLE_ENDIAN
} gl_conversion_type_t;

// Defines a position (vector 2D).
typedef struct gl_pos_t {
    uint32_t x;
    uint32_t y;
} gl_pos_t;

#endif /* GHOSTLAB_TYPES_H */
