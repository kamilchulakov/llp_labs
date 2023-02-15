#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

void debug(const char* fmt, ... ) {
#ifdef DEBUG

    va_list args;
    va_start (args, fmt);
    vfprintf(stderr, fmt, args);
    va_end (args);

#else
    (void) fmt;
#endif
}