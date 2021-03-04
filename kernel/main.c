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

extern unsigned char keyboard_charset[1];
char cpu_name[128];



void shell() {
	int cmd = 0;
	int of = 0;
	char ch = 0;
	char argv[256];
	memset(argv,0,256);
	
	cli();
	keyboard_charset[0] = 0;
	sti();
	
	printf("\nkernel prompt de comando:\n\n~ $ ");
	
	while(1) {
	
		cli();
		ch = keyboard_charset[0];
		keyboard_charset[0] = 0;
		sti();


		if(ch != 0){
			cli();
			printf("%c",ch);
			sti();
			if(ch == '\n'){
				
				if(!strcmp(argv,"shutdown\0") )cmd  = 1;
				
				
				switch(cmd) {
			
					case 1: 
						printf("O seu computador ira desligar\n");
						poweroff(5000);
					break;
					
					default:
					
						printf("comando invaido: %s\n",argv);
					break;
			
				}
				
				memset(argv,0,256);
				of = 0;
				
				printf("~ $ ");
			
			}else { 
			
				if(ch == '\b' && of > 0) argv[--of] = 0;
				else if(ch == '\b') argv[of] =  ch;
				else argv[of++] =  ch;
				
			}
			
			
		}
		
		__asm__ __volatile__("hlt");
	
	}	

}
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
	
	memset(cpu_name,0,128);
	cpuid_processor_brand(cpu_name);
	
	
	
	printf("Sirius OS (Kernel mode: AMD64 or x86_64)\nCPU: %s\n",cpu_name);
	
	
	// TODO drivers de interface
	keyboard_install();
	mouse_install();
	
	//int i = 0;
	sti();
	/*while(1) {
	
		sleep(1000);
		printf("%d ",i++);
	}*/
	
	
	shell();
	
	
}
