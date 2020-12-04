#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gui.h>
#include <pci.h>
#include <storage.h>
#include <fs.h>

void main()
{

	initialize_heap(DEF_HEAP_START);
	initialize_gui();
	
	printf("===============================================\n");
	printf("Run: stage 2, 32-bit, start = 0x11000\n");
	printf("===============================================\n");
	
	pci_get_info(0,2);
	printf("===============================================\n");

	ata_initialize();
	printf("===============================================\n");
	
	
	FILE *fp = fopen("README.md","r");
	
	int c;
	for(c = fgetc (fp);c !=EOF;c = fgetc (fp)) {

		putchar(c);
	}
}
