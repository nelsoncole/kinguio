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
#include <data.h>

extern unsigned int *cof_parameter();
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
	
	ata_initialize();
	printf("===============================================\n");
	
	printf("Read kernel.bin\n");
	
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
	
	// Verificar suport x86_64
	unsigned int a,d;
	cpuid(0x80000001, &a,&d);
	
	// Test LM-bit
	if(!(d >> 29 &1)) {
			
			puts("x86_64 hardware not supported\n");
			for(;;);
	
	}
	puts("How do I enable x86_64:\n");
	puts("Disable paging\n");
	puts("Set the PAE enable bit in CR4\n");
	puts("Load CR3 with the physical address of the PML4\n");
	puts("Enable long mode by setting the EFER.LME flag in MSR 0xC0000080\n");
	puts("Enable paging\n");
	
	page_install();

	printf("===============================================\n");
	
	unsigned long pointer = (unsigned long) cof_parameter();
	gdt_execute_long_mode(0x401000,pointer);
}
