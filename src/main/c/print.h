#ifndef GHOSTLAB_PRINT_H
#define GHOSTLAB_PRINT_H

#include "types.h"

void gl_printf_no_indicator(const char *format, ...);

void gl_printf(const char *format, ...);

void gl_printf_warning(const char *format, ...);

void gl_printf_error(const char *format, ...);

#endif /* GHOSTLAB_PRINT_H */
