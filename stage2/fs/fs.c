#include <fs.h>
#include <storage.h>
#include <stdlib.h>
#include <string.h>

int filename_cmp(const char *s1, const char *s2)
{

	char s3[256];
	
	unsigned char *us1 = (unsigned char *)s1;
	unsigned char *us2 = (unsigned char *)s2;
	unsigned char *us3 = (unsigned char *)s3;
	int i;
	
	for(i=0; i < 96; i++) { 
	
		if(!*us2) {
			us3[i] = ' ';
		} else {
			us3[i] = *us2++;
		}
	}
	
	for(i=0;i < 96; i++) {
	
		if(*us1++ != *us3++) {
			return 1;
		}
	}
	
	return 0;
}

int read_super_block(super_block_t *f,part_t *p)
{
	// Primeiro sector de particao
	return( read_sector(p->dv_num,1, p->start,f) );
}

int read_directory_entry(directory_entry_t *d,super_block_t *f,part_t *p,int type, const char *filename)
{
	// calcule o data_sector
	unsigned int data_sector = f->rsv + f->hidden;
	
	// calcule o first_sector
	unsigned int first_sector = (f->root_blk * f->sector_por_blk) + data_sector;
	
	// ler o primeiro bloco do directorio raiz 8KiB em geral.
	char *root = (char*)malloc(1024*8);
	memset(root,0,1024*8);
	int r = read_sector(p->dv_num, f->sector_por_blk, first_sector,root);
	
	int i;
	for(i=0; i < 64; i++) {
	
		r = filename_cmp((const char *)root + (i*128), filename);
	
		if(!r) break;
	}
	
	memcpy(d, root+(i*128), 128);
	
	//free(root);

	return 0;	
}


int read_block(void *buffer,unsigned int start,int count, int dv_num)
{
	return( read_sector(dv_num, count, start,buffer) );
}



void testfs() {

	printf("Testando o fs\n");
	const char filename[] = "README.md\0";
	
	
	super_block_t *f = (super_block_t *)malloc(1024);
	memset(f,0,1024);
	
	part_t *p = (part_t *)malloc(sizeof(part_t));
	memset(p,0,sizeof(part_t));
	
	
	p->start = 1;
	p->dv_num = 0;
	
	int r = read_super_block(f,p);
	printf("read super bloco r = %d, sig = %s\n",r,f->sig);
	
	directory_entry_t d[128];
	read_directory_entry(d,f,p,0,filename);
	
	char *buf = (char *)malloc(8192);
	memset(buf,0,8192);
	
	unsigned int start =  (d->first_blk*f->sector_por_blk) + f->rsv + f->hidden ;
	
	r = read_block(buf, start, f->sector_por_blk, p->dv_num);
	printf("r = %d\nFile name = %s\nFile size = %d\n%s\n",r,filename,d->file_size,buf);

}

