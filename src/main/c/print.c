#include "print.h"
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>

static bool g_use_newline_indicator = true;
static pthread_mutex_t g_print_mutex = PTHREAD_MUTEX_INITIALIZER;

void gl_printf_no_indicator(const char *format, ...) {
    pthread_mutex_lock(&g_print_mutex);
    
    va_list args;
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
//    va_start(args, format);
//    vsyslog(LOG_NOTICE, format, args);
//    va_end(args);
    
    if (strstr(format, "\n")) {
        g_use_newline_indicator = true;
    }
    
    pthread_mutex_unlock(&g_print_mutex);
}

void gl_printf(const char *format, ...) {
    pthread_mutex_lock(&g_print_mutex);
    
    va_list args;
    
    if (g_use_newline_indicator) {
        printf(GHOSTLAB_EXECUTABLE_NAME ": ");
        g_use_newline_indicator = false;
    }
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
//    va_start(args, format);
//    vsyslog(LOG_NOTICE, format, args);
//    va_end(args);
    
    if (strstr(format, "\n")) {
        g_use_newline_indicator = true;
    }
    
    pthread_mutex_unlock(&g_print_mutex);
}

void gl_printf_warning(const char *format, ...) {
    pthread_mutex_lock(&g_print_mutex);
    
    va_list args;
    
    if (g_use_newline_indicator) {
        printf(GHOSTLAB_EXECUTABLE_NAME ": warning: ");
        g_use_newline_indicator = false;
    }
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
//    va_start(args, format);
//    vsyslog(LOG_WARNING, format, args);
//    va_end(args);
    
    if (strstr(format, "\n")) {
        g_use_newline_indicator = true;
    }
    
    pthread_mutex_unlock(&g_print_mutex);
}

void gl_printf_error(const char *format, ...) {
    pthread_mutex_lock(&g_print_mutex);
    
    va_list args;
    
    if (g_use_newline_indicator) {
        printf(GHOSTLAB_EXECUTABLE_NAME ": error: ");
        g_use_newline_indicator = false;
    }
    
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
//    va_start(args, format);
//    vsyslog(LOG_ERR, format, args);
//    va_end(args);
    
    if (strstr(format, "\n")) {
        g_use_newline_indicator = true;
    }
    
    pthread_mutex_unlock(&g_print_mutex);
}

void gl_gets(const char *format, char *dst) {
    if (format) {
        gl_printf(format);
    }
    
    fgets(dst, 512, stdin);
    g_use_newline_indicator = true;
    dst[strlen(dst) - 1] = 0;
}
