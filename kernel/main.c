#include <gui.h>
#include <stdio.h>
#include <paging.h>
#include <mm.h>
#include <driver.h>
#include <gdt.h>
#include <idt.h>


void main(unsigned long entry_pointer_info)
{
	initialize_gui(entry_pointer_info);
	
	printf("run: kernel --> x86_64\n");
	
	gdt_install();
	idt_install();
	
	
	//__asm__ __volatile__("int $1");
	
	page_install(entry_pointer_info);
	
	initialize_memory();
	
	setup_i965();
	
	//clears_creen();
	
	printf("Done\n");
	
}
