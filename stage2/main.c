#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gui.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>
#include <storage.h>
#include <fs.h>
#include <cpuid.h>
#include <paging.h>


void main()
{

	initialize_heap(DEF_HEAP_START);
	initialize_gui();
	
	printf("===============================================\n");
	printf("Run: stage 2, 32-bit, start = 0x11000\n");
	printf("===============================================\n");
	
	char cpu[64];
	//cpuid_vendor(s);
	cpuid_processor_brand(cpu);
	printf("%s\n",cpu);
	printf("===============================================\n");
	
	printf("Video mode: %dx%dx%d\n",gui->width,gui->height,gui->bpp);
	printf("===============================================\n");
	
	printf("GDT and IDT install\n");
	gdt_install();
	idt_install();

	printf("===============================================\n");
	
	pci_get_info(0,2);
	printf("===============================================\n");

	ata_initialize();
	printf("===============================================\n");
	
	FILE *f = fopen("kernel.bin","r");
	if(!f) {
	
		printf("error read kernel.bin %x\n",f);
		
		for(;;);
	}
	
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	rewind(f);
	
	fread ((void*)0x400000, 1, size, f);
	
	fclose(f);
	
	
	
	printf("===============================================\n");
	puts("How do I enable x86_64:\n");
	puts("Disable paging\n");
	puts("Set the PAE enable bit in CR4\n");
	puts("Load CR3 with the physical address of the PML4\n");
	puts("Enable long mode by setting the EFER.LME flag in MSR 0xC0000080\n");
	puts("Enable paging\n");
	
	page_install();

	printf("===============================================\n");
	
	gdt_execute_long_mode(0x401000,0);
}
