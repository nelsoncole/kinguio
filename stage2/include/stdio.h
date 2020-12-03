#ifndef __STDIO_H
#define __STDIO_H
#include <stdarg.h>


#define NULL ((void *)0)

#define EOF (-1)
#define _IOFBF		0
#define _IOLBF		1
#define _IONBF		2

#define BUFSIZ		8192

#define FOPEN_MAX	16
#define FILENAME_MAX	256

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2



int vsprintf(char * str,const char * fmt, va_list ap);

int putchar(int c);
void puts(const char* str);
int printf(const char *fmt, ...);


// Virtual File System
typedef struct _FILE {



}__attribute__ ((packed)) FILE;


#endif
