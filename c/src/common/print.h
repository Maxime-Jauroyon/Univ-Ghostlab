#ifndef GHOSTLAB_PRINT_H
#define GHOSTLAB_PRINT_H

#include <common/types.h>

void gl_printf_no_indicator(const char *format, ...);

void gl_printf(const char *format, ...);

void gl_printf_warning(const char *format, ...);

void gl_printf_error(const char *format, ...);

void gl_gets(const char *format, char *dst);

#endif /* GHOSTLAB_PRINT_H */
