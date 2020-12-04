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
/*
 *
 * Flags
 * 0x2  =stdin
 * 0x3  =stdout
 * 0x4  =stderr 
 * 0x10 = Leitura
 * 0x20 = Arquivo
 * 0x40 = Pronto
 * 0x80 = File null
 * 0x88 = Spin lock
 * 	
 */
typedef struct _FILE {
	short 		level;		// Nível do buffer cheio/vazio
	unsigned 	flags;		// Sinalizadores de status
	char 		fd;		// Descritor de arquivo
	unsigned char 	hold;		// Caractere ungetc se não existir um buffer
	unsigned 	bsize;		// Tamanho do buffer
	unsigned char 	*buffer;	// Buffer de transferência
	unsigned char 	*curp;		// Ponteiro activo atual
	unsigned 	istemp;	// Indicador de arquivo temporário
	short 		token;		// Usado para verificação de validade
	//
	unsigned char 	mode[4];
	//
	char 		fname[256];	// File name
	unsigned int 	fsize;		// File Size
	unsigned int 	byte_per_sector;
	unsigned int 	count;
	unsigned int	fs_type;	// Tipo de sistema de arquivo
	unsigned int 	num_of_blocks; 
	unsigned int 	*blocks;	// 1024*1024 (absolute address sector)
	
	// 
	unsigned int 	dv_num;


}__attribute__ ((packed)) FILE;


#endif
