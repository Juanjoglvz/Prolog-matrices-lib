#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(...) dbgprintf(__FILE__, __LINE__, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while(0)
#endif

void dbgprintf(const char* source_file, int source_line, const char* format_string, ...);



#endif