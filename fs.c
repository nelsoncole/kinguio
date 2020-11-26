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
	unsigned char busy;
	unsigned char attr;
	unsigned short time;
	unsigned short time2;
	unsigned short date;
	unsigned short date2;
	unsigned int blk;
	unsigned int filesize;
	unsigned int entry;
	char reserved[10];
}__attribute__ ((packed)) directory_entry;


unsigned int disk_size = 0;

FILE *fd1, *fd2;

unsigned char data[512];


void read_super_block(FILE *fd,super_block *fs)
{

	// reserved - 1;
	int off = (2-1)*512;
	
	fseek (fd, off, SEEK_SET );
	fread(fs,1,sizeof(super_block),fd);	
	rewind(fd);
}

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

void clean_blk_enter(unsigned int blk,FILE *fd,super_block *fs)
{
	unsigned int data = 0;
	unsigned int data_sector =  fs->reserved;
      	unsigned int first_sector = data_sector + (blk * fs->sector_por_blk);
      	unsigned int off = first_sector * fs->byte_per_sector;
      	
      	int i, n = (fs->byte_per_sector * fs->sector_por_blk)/4;
      	
	fseek (fd, off, SEEK_SET );
	for(i=0 ;i < n;i++) fwrite(&data,1,4,fd);

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
	
	
	clean_blk_enter(fs->root_blk,fd,fs);
}

unsigned int search_blk_null(FILE *fd,super_block *fs)
{
	unsigned int blk;
	unsigned int index = 0;
	unsigned int off = fs->reserved * fs->byte_per_sector;
	
	fseek (fd, off, SEEK_SET );
	
	for(index = 0;index < fs->num_of_blk;index++) {
	
		fread(&blk,1,4,fd);
		
		if(!blk){
		
			// gravar o index
			blk = -1;
			fseek (fd, off, SEEK_SET );
			fwrite(&blk,1,4,fd);
			fflush(fd);
		
			rewind(fd);
			return (index);
		}
		off = off + 4;
		
	}
	
	rewind(fd);
	return (-1);

}

int write_blk(unsigned int new_blk,unsigned int cur_blk,FILE *fd,super_block *fs)
{
	int r;
	unsigned int blk = new_blk;
	unsigned int off = (fs->reserved * fs->byte_per_sector) + (4*cur_blk);
	
	
	fseek (fd, off, SEEK_SET );
	r = fwrite(&blk,1,4,fd);
	if(r != 4) return -1;
	
	fflush(fd);
	rewind(fd);

	return 0;
}

int create_file(const char *fname,FILE *fd,super_block *fs,directory_entry *directory) {
	
	directory_entry *dir = directory;
	directory_entry dir2[1];
	
	memset(dir,0,128);
	
	strcpy(dir->filename,fname);
	dir->filesize = 0;
      	
      	
      	unsigned int data_sector =  fs->reserved;
      	unsigned int root_sector = data_sector + (fs->root_blk * fs->sector_por_blk);
	
	int index;
	unsigned int off;
	
	// localizar blk disponivel
	dir->blk = search_blk_null(fd,fs);
	// verificar erro
	if(dir->blk == -1) {
		printf("Error ao criar o arquivo \"%s\"",fname);
	 	return 1;
	}


	//numero de entrada, aqui tem que pesquisar
	for(index = 0;index < 64 ;index++) {
		off = (root_sector * fs->byte_per_sector) + (128*index);
		fseek (fd, off, SEEK_SET );
		fread(dir2,1,128,fd);
		//dir->busy == 0, entrada livre
		if(!dir2->busy) break;
	}
	
	
	
	if(index >= 64 ) {
		printf("Error ao criar o arquivo \"%s\", atingio o limite de arquivos na entrada de directorio",fname);
		return -1;
	}
	
	
	// gravar directory
	dir->entry = index;
	dir->busy = -1;
	
	off = (root_sector * fs->byte_per_sector) + (128*dir->entry);
	fseek (fd, off, SEEK_SET );
	fwrite(dir,1,128,fd);
	fflush(fd);
	rewind(fd);
	
	return 0;
}

void copy(FILE *fd,super_block *fs,directory_entry *directory) {

	FILE *f;
	directory_entry *dir = directory;
	
	if((f=fopen(directory->filename,"r+w"))==NULL) {
		printf("Erro ao copiar  o arquivo: \"%s\"",directory->filename);
		return; 
	}
	
	
	unsigned int data_sector =  fs->reserved;
      	unsigned int first_sector = data_sector + (directory->blk * fs->sector_por_blk);
      	unsigned int root_sector = data_sector + (fs->root_blk * fs->sector_por_blk);
      	unsigned int off;
      	unsigned int size_blk = fs->sector_por_blk * fs->byte_per_sector;
      	
      	
      	// gravar directory, tamanho em bytes
      	fseek (f, 0, SEEK_END );
	dir->filesize = ftell(f);
      	rewind(f);
      	
	off = (root_sector * fs->byte_per_sector) + (128*dir->entry);
	fseek (fd, off, SEEK_SET );
	fwrite(dir,1,128,fd);
	fflush(fd);
	
	
	off = first_sector * fs->byte_per_sector;
	
	int i,n = 1;
	unsigned int new_blk;
	unsigned int cur_blk = dir->blk;
	
	fseek (fd, off, SEEK_SET );
	
	for(i=0;i<(directory->filesize);i++){
	
		if(i == (size_blk*n)) { // new bloco
		
			// obter novo bloco
			new_blk = search_blk_null(fd,fs);
			// valor do ultimo blk e ecrever nele o valor do novo bloco
			write_blk(new_blk,cur_blk,fd,fs);
		
			cur_blk = new_blk;
			n++; 
			
			// new offset
			first_sector = data_sector + (new_blk* fs->sector_por_blk);
			off = first_sector * fs->byte_per_sector;
			fseek (fd, off, SEEK_SET );
			
		} else fputc(getc(f),fd);
		
	}
	
	fflush(fd);
	rewind(fd);
	
	fclose(f);
}
 
int main(int argc, char **argv) {

	int r,d,a,flg;
	

	super_block fs;
	directory_entry directory[1];
	
	if(argc < 3) {
	
		printf("Argumento invalido\n"); 
		return 0;
	}
	
	if( !strcmp("-f",argv[1]) ) {
		d = 2;
		if(argc > 3 && argc < 6) {
		
			if( !strcmp("-g",argv[3]) ) {
			
				a = 4;
				flg = 2;
			
			} else {
				printf("Argumento invalido\n");
				return 0;
			}
		
		} else if(argc == 3) { 
			flg = 1;
		} else {
			printf("Argumento invalido\n");
			return 0;
		}
		
	}else if(!strcmp("-g",argv[1])) {
	
		a = 2;
		
		if(argc == 4) {
			flg = 3;
			d = 3;
		} else if(argc > 3 && argc < 6) {
		
			if( !strcmp("-f",argv[3]) ) {
			
				d = 4;
				
				flg = 2;
			
			} else {
				printf("Argumento invalido\n");
				return 0;
			}
		
		} else {
			printf("Argumento invalido\n");
			return 0;
		}
	}else {
	
		printf("Argumento invalido\n");
		return 0;
	}

      	memset(data,0,512);
      	
      	strcpy(fs.sig,"KINGUIO_");
      	
      	if((fd1=fopen(argv[d],"r+w"))==NULL) {
		printf("Erro ao abrir o disco \"%s\"",argv[d]);
		exit(1); 
	}
      	
      	
	if(flg == 3) {
		read_super_block(fd1,&fs);
	 	goto __gravar;
	 	
	} else if(flg > 3 || flg == 0) return 0;
	
	
	fs.reserved = 2;
	fs.byte_per_sector = 512;
	fs.sector_por_blk = 16;
	
	fseek (fd1, 0, SEEK_END );
	disk_size = ftell(fd1);
      	rewind(fd1);
      	
      	fs.total_num_of_sector = disk_size /fs.byte_per_sector;
      	fs.num_of_blk = num_of_blk(&fs);
      	fs.size_of_blk = size_of_blk(&fs);
	
// -f
//__format;
      	
      	// formatar a tabela de blocos
      	format_blk(fd1,&fs);
      	fs.root_blk = 0;
      	//Criar o directorio raiz
      	root_dir(fd1,&fs);
	
	// Gravar o bootrecord
	if((fd2=fopen("bin/stage0.bin","r+b"))==NULL){
		printf("Erro ao abrir o arquivo \"bootrecord\"");
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
	rewind(fd1);
	
	if(flg == 1) goto __end;

// -g
__gravar:
	
	// gravar arquivo
	create_file(argv[a],fd1,&fs,directory);
	copy(fd1,&fs,directory);
	printf("Copiado: %s, %d bytes, blk %d, dir entry %d\n",directory->filename, directory->filesize,directory->blk,directory->entry);
	
__end:
	fclose(fd1);
	printf("done\n");
	exit(0);
	
	return 0;
}
