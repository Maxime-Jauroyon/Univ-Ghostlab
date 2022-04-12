#include "print.h"
#include <stdio.h>
#include <syslog.h>
#include <string.h>

static bool g_use_newline_indicator = true;

void gl_printf_no_indicator(const char *format, ...) {
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
}

void gl_printf(const char *format, ...) {
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
}

void gl_printf_warning(const char *format, ...) {
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
}

void gl_printf_error(const char *format, ...) {
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
}

void gl_gets(const char *format, char *dst) {
    gl_printf(format);
    fgets(dst, 512, stdin);
    g_use_newline_indicator = true;
    dst[strlen(dst) - 1] = 0;
}
