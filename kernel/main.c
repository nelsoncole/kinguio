#include <io.h>
#include <gui.h>
#include <paging.h>
#include <mm.h>
#include <driver.h>
#include <gdt.h>
#include <idt.h>

#include <apic.h>

#include <cpuid.h>

#include <acpi.h>
#include <hpet.h>
#include <pit.h>

#include <ps2.h>

#include <stdio.h>
#include <string.h>
#include <sleep.h>



extern void shell();

void main(unsigned long entry_pointer_info)
{

	
	initialize_gui(entry_pointer_info);
	
	printf("RUN: kernel --> x86_64\n");
	
	printf("Setup LGDT ...\\\\\n");
	printf("Setup LIDT ...\\\\\n");
	gdt_install();
	idt_install();
	
	printf("Setup Paging ...\\\\\n");
	page_install(entry_pointer_info);
	printf("Setup MM ...\\\\\n");
	initialize_mm_mp();
	
	setup_acpi();
	setup_apic();
	setup_ioapic();
	
	setup_hpet(100);
	apic_timer_umasked();
	ioapic_umasked(1);
	ioapic_umasked(2);
	ioapic_umasked(12);
	
	 
	
	
	setup_i965();
	
	char cpu_name[128];
	memset(cpu_name,0,128);
	cpuid_processor_brand(cpu_name);
	
	
	
	printf("Sirius OS (Kernel mode: AMD64 or x86_64)\nCPU: %s\n",cpu_name);
	
	
	// TODO drivers de interface
	keyboard_install();
	mouse_install();
	
	sti();
	
	/*int i = 0;
	while(1) {
	
		sleep(1000);
		cli();
		printf("%d ",i++);
		sti();
	}*/
	
	
	shell();
	
	
}

