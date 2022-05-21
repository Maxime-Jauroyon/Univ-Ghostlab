#include <common/log.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#if GHOSTLAB_TUI
#include <common/ui.h>
#endif
#include <common/array.h>
#include <common/memory.h>

static bool g_scroll = true;
static bool g_is_newline = true;
static gl_log_t **g_logs = 0;
static pthread_mutex_t g_print_mutex = PTHREAD_MUTEX_INITIALIZER;

void gl_log_vpush(const char *format, gl_log_type_t type, va_list args) {
    pthread_mutex_lock(&g_print_mutex);
    
    if (!g_is_newline) {
        char buf[512] = { 0 };
        vsprintf(buf, format, args);
        strcat(gl_array_get_last(g_logs)->data, buf);
    } else {
        gl_log_t *log = gl_malloc(sizeof(*log));
        log->type = type;
        char buf[512] = { 0 };
        vsprintf(buf, format, args);
        if (log->type == GL_LOG_TYPE_DEFAULT) {
            sprintf(log->data, GHOSTLAB_EXECUTABLE_NAME ": %s", buf);
        } else if (log->type == GL_LOG_TYPE_INFO) {
            sprintf(log->data, GHOSTLAB_EXECUTABLE_NAME ": info: %s", buf);
        } else if (log->type == GL_LOG_TYPE_WARNING) {
            sprintf(log->data, GHOSTLAB_EXECUTABLE_NAME ": warning: %s", buf);
        } else if (log->type == GL_LOG_TYPE_ERROR) {
            sprintf(log->data, GHOSTLAB_EXECUTABLE_NAME ": error: %s", buf);
        } else if (log->type == GL_LOG_TYPE_USER) {
            sprintf(log->data, "$ %s", buf);
        }
        gl_array_push(g_logs, log); // NOLINT
    }
    
    if (strcmp(format + strlen(format) - 1, "\n") == 0) {
#if GHOSTLAB_GUI
        printf("%s", gl_array_get_last(g_logs)->data);
#else
        if (!gl_ui_started()) {
            printf("%s", gl_array_get_last(g_logs)->data);
        }
#endif
        g_is_newline = true;
    } else {
        g_is_newline = false;
    }
    
    g_scroll = true;
    
    pthread_mutex_unlock(&g_print_mutex);
}

void gl_log_push(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, GL_LOG_TYPE_DEFAULT, args);
    va_end(args);
}

void gl_log_push_array(const char *array[], uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        gl_log_push(array[i], GL_LOG_TYPE_DEFAULT);
    }
}

void gl_log_push_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, GL_LOG_TYPE_INFO, args);
    va_end(args);
}

void gl_log_push_warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, GL_LOG_TYPE_WARNING, args);
    va_end(args);
}

void gl_log_push_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, GL_LOG_TYPE_ERROR, args);
    va_end(args);
}

void gl_log_push_user(const char *format, ...) {
    va_list args;
    va_start(args, format);
    gl_log_vpush(format, GL_LOG_TYPE_USER, args);
    va_end(args);
}

void gl_log_free() {
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
