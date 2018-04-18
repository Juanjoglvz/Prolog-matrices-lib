#include <debug.h>
#include <stdarg.h>

void dbgprintf(const char* source_file, int source_line, const char* format_string, ...)
{
    va_list args;
    va_start(args, format_string);

    fprintf(stderr, "%s:%d: ", source_file, source_line);
    vfprintf(stderr, format_string, args);

    va_end(args);

}