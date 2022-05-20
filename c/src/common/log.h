#ifndef GHOSTLAB_LOG_H
#define GHOSTLAB_LOG_H

#include <common/types.h>

// Defines a type of log.
typedef enum gl_log_type_t {
    GL_LOG_TYPE_DEFAULT,
    GL_LOG_TYPE_INFO,
    GL_LOG_TYPE_WARNING,
    GL_LOG_TYPE_ERROR,
    GL_LOG_TYPE_USER
} gl_log_type_t;

// Defines a log line.
typedef struct gl_log_t {
    gl_log_type_t type;
    char data[512];
} gl_log_t;

// Pushes an output (from a va_list) to the list of logs.
void gl_log_vpush(const char *format, gl_log_type_t type, va_list args);

// Pushes an output to the list of logs.
void gl_log_push(const char *format, ...);

// Pushes an array of output to the list of logs.
void gl_log_push_array(const char *array[], uint32_t length);

// Pushes an info output to the list of logs.
void gl_log_push_info(const char *format, ...);

// Pushes a warning output to the list of logs.
void gl_log_push_warning(const char *format, ...);

// Pushes an error output to the list of logs.
void gl_log_push_error(const char *format, ...);

// Pushes a user input to the list of logs.
void gl_log_push_user(const char *format, ...);

// Frees all logs.
void gl_log_free();

// Returns if we should scroll (a new text has been added?).
bool gl_log_should_scroll();

// Returns all logs.
gl_log_t **gl_logs();

#endif /* GHOSTLAB_LOG_H */
