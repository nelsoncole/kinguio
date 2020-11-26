#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _super_block {

	char sig[8];				// 00 - 7
	char name[16];				// 08 - 23
	unsigned int reserved;		// 24 - 27
	unsigned int byte_per_sector;		// 28 - 31
	unsigned int sector_por_blk;		// 32 - 35
	unsigned int num_of_blk;		// 36 - 39
	unsigned int size_of_blk;		// 40 - 43
	unsigned int root_blk;			// 44 - 47
	unsigned int total_num_of_sector;	// 48 - 51
	

}__attribute__ ((packed)) super_block;

typedef struct _directory_entry {

	char filename[96];
	unsigned char rsv;
	unsigned char attr;
	unsigned short time;
	unsigned short time2;
	unsigned short date;
	unsigned short date2;
	unsigned int blk;
	unsigned int filesize;
	char reserved[14];
}__attribute__ ((packed)) directory_entry;


unsigned int disk_size = 0;

FILE *fd1, *fd2;

unsigned char data[512];

unsigned int size_of_blk(super_block *fs)
{
	
	unsigned int n = (((fs->total_num_of_sector - \
	fs->reserved)/fs->sector_por_blk)*4)/(fs->byte_per_sector * fs->sector_por_blk);
	
	if( ( (((fs->total_num_of_sector - \
	fs->reserved)/fs->sector_por_blk)*4)%(fs->byte_per_sector * fs->sector_por_blk) ) ) n += 1;
	return (n);
}

unsigned int num_of_blk(super_block *fs)
{
	return( (fs->total_num_of_sector - fs->reserved)/fs->sector_por_blk );
}


void format_blk(FILE *fd,super_block *fs)
{

	unsigned int data = 0;
	
	int i;
	
	fseek (fd, fs->reserved*fs->byte_per_sector, SEEK_SET );

	for(i=0; i < fs->num_of_blk;i++){
	 
		if(i < fs->size_of_blk)data =0x8fffffff;
		else data = 0;
		
		fwrite(&data,1,4,fd);
	}

	fflush(fd);
	
	rewind(fd);
}

void root_dir(FILE *fd,super_block *fs)
{
	unsigned int data = -1;
	int i;
	
	fs->root_blk = fs->size_of_blk;
	
	int off = (fs->reserved*fs->byte_per_sector) + (fs->root_blk*4);
	
	fseek (fd, off, SEEK_SET );
	
	fwrite(&data,1,4,fd);
	
	fflush(fd);
	
	
	//clear directory
	off = (fs->reserved*fs->byte_per_sector) + (fs->sector_por_blk*fs->byte_per_sector*fs->root_blk);
	
	fseek (fd, off, SEEK_SET );
	
	data = 0;
	for(i=0; i < (fs->sector_por_blk*fs->byte_per_sector/4);i++){
		
		fwrite(&data,1,4,fd);
	}
	
	fflush(fd);
	rewind(fd);
}

int create_file(const char *fname,FILE *fd,super_block *fs,directory_entry *directory){

	FILE *f;
	
	directory_entry *dir = directory;
	
	memset(dir,0,128);
	
	if((f=fopen(fname,"r+w"))==NULL) {
		printf("Erro ao copiar  o arquivo: \"%s\"",fname);
		return 1; 
	}
	
	
	
	strcpy(dir->filename,fname);
	
	fseek (f, 0, SEEK_END );
	dir->filesize = ftell(f);
      	rewind(f);
      	
      	
      	unsigned int data_sector =  fs->reserved;
      	unsigned int root_sector = data_sector + (fs->root_blk * fs->sector_por_blk);
	
	int index;
	unsigned int off;
	unsigned int blk = -1;
	
	// localizar blk disponivel
	index = 0;
	off = fs->reserved * fs->byte_per_sector;
	fseek (fd, off, SEEK_SET );
	
	for(index = 0;index < fs->num_of_blk;index++) {
	
		fread(&blk,1,4,fd);
		
		if(!blk){
		
			// gravar o index
			
			dir->blk = index;
			blk = -1;
			fseek (fd, off, SEEK_SET );
			fwrite(&blk,1,4,fd);
			fflush(fd);
		
			break;
		}
		off = off + 4;
		
	}
	
	// verificar erro
	if(index >= fs->num_of_blk) return 1;
	
	
	
	// gravar directory
	index = 0;
	off = (root_sector * fs->byte_per_sector) + (128*index);
	fseek (fd, off, SEEK_SET );
	fwrite(dir,1,128,fd);
	fflush(fd);
	rewind(fd);
	
	fclose(f);
	return 0;
}

void copy(FILE *fd,super_block *fs,directory_entry *directory) {

	FILE *f;
	
	if((f=fopen(directory->filename,"r+w"))==NULL) {
		printf("Erro ao copiar  o arquivo: \"%s\"",directory->filename);
		return; 
	}
	
	unsigned int data_sector =  fs->reserved;
      	unsigned int first_sector = data_sector + (directory->blk * fs->sector_por_blk);
      	unsigned int off = first_sector * fs->byte_per_sector;
      	
      	unsigned int data;
	int i;
	fseek (fd, off, SEEK_SET );
	
	for(i=0;i<(directory->filesize);i++){
	
		//fread(&data,1,4,f);
		//fwrite(&data,1,4,fd);
		
		fputc(getc(f),fd);
		
	}
	
	fflush(fd);
	rewind(fd);
	
	fclose(f);
}
 
int main(int argc, char **argv) {

	int r,i;

	super_block fs;

	fs.reserved = 2;
	fs.byte_per_sector = 512;
	fs.sector_por_blk = 16;
	
	if(argc < 2) return 1;
	
      	memset(data,0,512);
      	
      	strcpy(fs.sig,"KINGUIO_");
      	
      	if((fd1=fopen(argv[1],"r+w"))==NULL) {
		printf("Erro ao abrir o arquivo \"argv[1]\"");
		exit(1); 
	}
	
	
	fseek (fd1, 0, SEEK_END );
	disk_size = ftell(fd1);
      	rewind(fd1);
      	
      	fs.total_num_of_sector = disk_size /fs.byte_per_sector;
      	fs.num_of_blk = num_of_blk(&fs);
      	fs.size_of_blk = size_of_blk(&fs);
      	
      	// formatar a tabela de blocos
      	format_blk(fd1,&fs);
      	fs.root_blk = 0;
      	//Criar o directorio raiz
      	root_dir(fd1,&fs);
	
	// Gravar o bootrecord
	if((fd2=fopen(argv[2],"r+b"))==NULL){
		printf("Erro ao abrir o arquivo \"argv[2]\"");
		exit(1); 
	}
	
	r = fread(data,1,fs.byte_per_sector,fd2);
	
	if(r != fs.byte_per_sector) {
		printf("Erro ao copiar o sector de boot");
		exit(1);
	}
	
		
	r = fwrite(data,1,fs.byte_per_sector,fd1);
	if(r != fs.byte_per_sector) {
		printf("Erro ao ecrever o sector de boot");
		exit(1);
	}
	
	//Gravar o super bloco
	r = fwrite(&fs,1,sizeof(super_block),fd1);
	if(r != sizeof(super_block)) {
		printf("Erro ao gravar o super bloco");
		exit(1);
	}
	
	fclose(fd2);
	fflush(fd1);
	
	
	// copiar o stage1
	directory_entry directory[1];
	create_file(argv[3],fd1,&fs,directory);
	printf("Copiado: %s, %d bytes, blk %d\n",directory->filename, directory->filesize,directory->blk);
	
	copy(fd1,&fs,directory);
	
	
	
	fclose(fd1);
	printf("done\n");
	exit(0);
	
	return 0;
}
