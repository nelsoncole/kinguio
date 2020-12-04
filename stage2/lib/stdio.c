#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gui.h>
#include <fs.h>


int putchar(int c)
{
	return (glyph(c));

}

void puts(const char* str)
{
	
	int i;
	if(!str)return;
	 
	for(i=0;i <strlen(str);i++) putchar(str[i]);
}


short r_printf_buf[1024];

int printf(const char *fmt, ...)
{
	char *buf = (char*)r_printf_buf;
	memset(buf,0,1024*sizeof(short)); 

	int ret;
  	va_list ap;
  	va_start (ap, fmt);
  	ret = vsprintf(buf, fmt, ap);
  	va_end (ap);
  	
  	puts(buf);
  	
  	return ret;

}

static char *_vsputs_r(char *dest, char *src)
{
	unsigned char *usrc = (unsigned char *) src;
	unsigned char *udest = (unsigned char *) dest;
	
	while(*usrc) *udest++ = *usrc++;

	return (char * ) udest;
	
}

int vsprintf(char * str,const char * fmt, va_list ap)
{

	char *str_tmp = str;

	char _c_r[] = "\0\0";

	int index = 0;
	unsigned char u;	
	int d;
	char c, *s;
	char buffer[256];

	while (fmt[index])
	{
		switch (fmt[index])
		{
		case '%':
			++index;
			switch (fmt[index])
			{
			
			case 'c':
				*_c_r = c = (char) va_arg (ap, int);
				str_tmp  = _vsputs_r(str_tmp,_c_r);
				break;

			case 's':
				s = va_arg (ap, char*);
				str_tmp  = _vsputs_r(str_tmp,s);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, int);
				itoa (d,buffer);
				str_tmp  = _vsputs_r(str_tmp,buffer);
				break;

			case 'u':
				u = va_arg (ap, unsigned int);
				itoa  (u,buffer);
				str_tmp  = _vsputs_r(str_tmp,buffer);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, int);
				i2hex(d, buffer,8);
				str_tmp  = _vsputs_r(str_tmp,buffer);
				break;
			
			default:
				str_tmp  = _vsputs_r(str_tmp,"%%");
				break;
				
				
			}
			break;

		default:
			*_c_r = fmt[index]; //
			str_tmp  = _vsputs_r(str_tmp,_c_r);
			break;
		}
		++index;
	}

    	return ((int)str_tmp - (int)str);

}


//FILE


/*
 *  Modo 	Significado
 *
 * r		Abre um arquivo-texto para leitura
 * w		Cria um arquivo-texto para escrita
 * a		Anexa a um arquivo-texto
 * rb		Abre um arquivo binário para leitura
 * wb		Cria um arquivo binário para escrita
 * ab		Anexa a um arquivo binário
 * r+		Abre um arquivo-texto para leitura/escrita
 * w+		Cria um arquivo-texto para leitura/escrita
 * a+		Anexa ou cria um arquivo-texto para leitura/escrita
 * r+b		Abre um arquivo binário para leitura/escrita
 * w+b		Cria um arquivo binário para escrita/escrita
 * a+b		Anexa ou cria um arquivo binário para escrita/escrita
 *
 *	
 */


FILE *fopen (const char *path,const char *mode) 
{
	if(mode[0] == '\0') return NULL;


	FILE *fp = open_file(path,mode);

	return fp;
	
}

void rewind (FILE *fp)
{
	if(!fp) return;
	
	fp->off = fp->off2 = 0;
	fp->level = 0;
	
	// define leitura
	fp->flags &=~0x10;

}


int fseek (FILE *fp, long num_bytes, int origin )
{	
	switch(origin) {

		case SEEK_SET:

			fp->off = num_bytes;
			fp->off2 = num_bytes;
		break;
		case SEEK_CUR:

			fp->off += num_bytes;
			fp->off2 += num_bytes;
		break;
		case SEEK_END:
			
			fp->off = fp->fsize - num_bytes;
			fp->off2 = fp->off;
		break;

	}

	fp->level = fp->off % fp->bsize;

	return 0;
}

long int ftell(FILE *fp)
{
	if(!fp) return (0);

	return (fp->off);

}

int fgetc (FILE *fp)
{	
	int r;
	
	if(fp->off >= fp->fsize) {
	
		return EOF;
	}
	
	
	if(!(fp->flags&0x10)) {
		// ler
		fp->flags |= 0x10;
		
		int addr = fp->off/fp->bsize;
		file_read_block(fp, addr);
	}


	fp->curp = (unsigned char*)(fp->buffer + fp->level++);
	r = *(unsigned char*)(fp->curp) &0xff;

	// update
	if(fp->level >= fp->bsize) {
		fp->flags &=~0x10;
		fp->level = 0;
	}
	
	fp->off++;

	return (r);
}

size_t fread (void *buffer,size_t size, size_t count, FILE *fp)
{
	if(!fp) return (0);

	size_t i;
	int c;
	size_t rc = 0;

	unsigned char *buf = (unsigned char*)buffer;

	for(i=0;i < size*count;i++)
	{
		c = fgetc (fp);
			//feof(fp);
		if(c == EOF) return (rc/size);

		*buf++ = c;
		rc++;
	}

	return (rc/size);
}


