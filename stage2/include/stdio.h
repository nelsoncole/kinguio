#ifndef __STDIO_H
#define __STDIO_H
#include <stdarg.h>

int vsprintf(char * str,const char * fmt, va_list ap);

int putchar(int c);
void puts(const char* str);
int printf(const char *fmt, ...);



#endif
