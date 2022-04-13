#ifndef GHOSTLAB_LOG_H
#define GHOSTLAB_LOG_H

#include <common/types.h>

typedef enum gl_log_type_t {
    GL_LOG_TYPE_DEFAULT,
    GL_LOG_TYPE_INFO,
    GL_LOG_TYPE_WARNING,
    GL_LOG_TYPE_ERROR,
    GL_LOG_TYPE_USER
} gl_log_type_t;

typedef struct gl_log_t {
    gl_log_type_t type;
    char data[512];
} gl_log_t;

void gl_log_push(const char *format, ...);

void gl_log_vpush(const char *format, va_list args);

void gl_log_push_info(const char *format, ...);

void gl_log_push_warning(const char *format, ...);

void gl_log_push_error(const char *format, ...);

void gl_log_push_user(const char *format, ...);

void gl_logs_free();

bool gl_log_should_scroll();

gl_log_t **gl_logs();

#endif /* GHOSTLAB_LOG_H */
