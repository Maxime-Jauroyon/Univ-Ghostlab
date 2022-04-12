#include "print.h"
#include <stdio.h>
#include <syslog.h>

int gl_printf(const char *format, ...) {
    va_list args;
    
    int r = printf(GHOSTLAB_EXECUTABLE_NAME ": ");
    
    if (r < 0) {
        return r;
    }
    
    va_start(args, format);
    r = printf(format, args);
    syslog(LOG_NOTICE, format, args);
    va_end(args);
    
    return r;
}