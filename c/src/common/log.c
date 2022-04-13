#include <common/log.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "array.h"
#include "memory.h"

static bool g_scroll = true;
static bool g_is_newline = true;
static gl_log_t **g_logs = 0;
static pthread_mutex_t g_print_mutex = PTHREAD_MUTEX_INITIALIZER;

void gl_log_push(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, args);
    va_end(args);
}

void gl_log_vpush(const char *format, va_list args) {
    pthread_mutex_lock(&g_print_mutex);
    
    if (!g_is_newline) {
        char buf[512] = { 0 };
        vsprintf(buf, format, args);
        sprintf(gl_array_get_last(g_logs)->data, "%s%s", gl_array_get_last(g_logs)->data, buf);
    } else {
        gl_log_t *log = gl_malloc(sizeof(*log));
        log->type = GL_LOG_TYPE_DEFAULT;
        vsprintf(log->data, format, args);
        gl_array_push(g_logs, log); // NOLINT
    }
    
    if (strcmp(format + strlen(format) - 1, "\n") == 0) {
        g_is_newline = true;
    } else {
        g_is_newline = false;
    }
    
    g_scroll = true;
    
    pthread_mutex_unlock(&g_print_mutex);
}

void gl_log_push_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, args);
    va_end(args);
    gl_array_get_last(g_logs)->type = GL_LOG_TYPE_INFO;
}

void gl_log_push_warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, args);
    va_end(args);
    gl_array_get_last(g_logs)->type = GL_LOG_TYPE_WARNING;
}

void gl_log_push_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, args);
    va_end(args);
    gl_array_get_last(g_logs)->type = GL_LOG_TYPE_ERROR;
}

void gl_log_push_user(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, args);
    va_end(args);
    gl_array_get_last(g_logs)->type = GL_LOG_TYPE_USER;
}

void gl_logs_free() {
    if (g_logs) {
        for (uint32_t i = 0; i < gl_array_get_size(g_logs); i++) {
            gl_free(g_logs[i]);
        }
    }
    
    gl_array_free(g_logs);
    
    g_is_newline = true;
}

bool gl_log_should_scroll() {
    if (g_scroll) {
        g_scroll = false;
        return true;
    }
    
    return false;
}

gl_log_t **gl_logs() {
    return g_logs;
}