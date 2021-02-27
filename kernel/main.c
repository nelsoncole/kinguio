#include <io.h>
#include <gui.h>
#include <paging.h>
#include <mm.h>
#include <driver.h>
#include <gdt.h>
#include <idt.h>

#include <apic.h>

#include <cpuid.h>


#include <stdio.h>
#include <string.h>


char cpu_name[128];
void main(unsigned long entry_pointer_info)
{
	initialize_gui(entry_pointer_info);
	
	printf("Run: kernel --> x86_64\n");
	
	gdt_install();
	idt_install();
	
	page_install(entry_pointer_info);
	initialize_mm_mp();
	
	setup_apic();
	setup_ioapic();
	
	apic_timer_umasked();
	//ioapic_umasked(2);
	sti(); 
	
	
	setup_i965();
	
	memset(cpu_name,0,128);
	cpuid_processor_brand(cpu_name);
	
	restauro_de_tela();
	
	printf("Sirius OS (Kernel mode: AMD64 or x86_64)\nCPU: %s\n",cpu_name);
	
	
	
	for(;;);
}
