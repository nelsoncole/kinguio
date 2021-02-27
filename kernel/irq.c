#include <idt.h>
#include <irq.h>
#include <apic.h>

#include <stdio.h>

extern void irq0();	extern void irq10();	extern void irq20();
extern void irq1();	extern void irq11();	extern void irq21();
extern void irq2();	extern void irq12();	extern void irq22();
extern void irq3();	extern void irq13();	extern void irq23();
extern void irq4();	extern void irq14();	
extern void irq5();	extern void irq15();
extern void irq6();	extern void irq16();
extern void irq7();	extern void irq17();
extern void irq8();	extern void irq18();
extern void irq9();	extern void irq19();


void irq_function(int n){

	apic_eoi_register();
	switch(n - 0x40){
		
		case 0: printf("Null IRQ n: %d\n",n - 0x40); 

			break;
		case 1: printf("Null IRQ n: %d\n",n - 0x40);
			break;
		case 2: printf("Null IRQ n: %d\n",n - 0x40);
			
			break;
		case 3: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 4: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 5: printf("Null IRQ n: %d\n",n - 0x40);
 
			break;
		case 6: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 7: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 8: printf("Null IRQ n: %d\n",n - 0x40);
			break;
		case 9: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 10: printf("Null IRQ n: %d\n",n - 0x40); 
			
			break;
		case 11: printf("Null IRQ n: %d\n",n - 0x40);
 	
			break;
		case 12: printf("Null IRQ n: %d\n",n - 0x40);
			break;
		case 13: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 14: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 15: printf("Null IRQ n: %d\n",n - 0x40);
 
			break;
		case 16: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 17: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 18: printf("Null IRQ n: %d\n",n - 0x40);
 
			break;
		case 19: printf("Null IRQ n: %d\n",n - 0x40);

			break;
		case 20: printf("Null IRQ n: %d\n",n - 0x40);
			
			break;
		case 21: printf("Null IRQ n: %d\n",n - 0x40);
 
			break;
		case 22: printf("Null IRQ n: %d\n",n - 0x40);
 
			break;
		case 23: printf("Null IRQ n: %d\n",n - 0x40);
			break;
		
		default:
			printf("Default IRQ n: %d\n",n - 0x40);
		break;
	}
	
}


void irq_install()
{
	idt_gate(0x40,(unsigned long)irq0, 8,0);
	idt_gate(0x41,(unsigned long)irq1, 8,0);
	idt_gate(0x42,(unsigned long)irq2, 8,0);
	idt_gate(0x43,(unsigned long)irq3, 8,0);
	idt_gate(0x44,(unsigned long)irq4, 8,0);
	idt_gate(0x45,(unsigned long)irq5, 8,0);
	idt_gate(0x46,(unsigned long)irq6, 8,0);
	idt_gate(0x47,(unsigned long)irq7, 8,0);
	idt_gate(0x48,(unsigned long)irq8, 8,0);
	idt_gate(0x49,(unsigned long)irq9, 8,0);
	idt_gate(0x4A,(unsigned long)irq10, 8,0);
	idt_gate(0x4B,(unsigned long)irq11, 8,0);
	idt_gate(0x4C,(unsigned long)irq12, 8,0);
	idt_gate(0x4D,(unsigned long)irq13, 8,0);
	idt_gate(0x4E,(unsigned long)irq14, 8,0);
	idt_gate(0x4F,(unsigned long)irq15, 8,0);
	idt_gate(0x50,(unsigned long)irq16, 8,0);
	idt_gate(0x51,(unsigned long)irq17, 8,0);
	idt_gate(0x52,(unsigned long)irq18, 8,0);
	idt_gate(0x53,(unsigned long)irq19, 8,0);
	idt_gate(0x54,(unsigned long)irq20, 8,0);
	idt_gate(0x55,(unsigned long)irq21, 8,0);
	idt_gate(0x56,(unsigned long)irq22, 8,0);
	idt_gate(0x57,(unsigned long)irq23, 8,0);
}


