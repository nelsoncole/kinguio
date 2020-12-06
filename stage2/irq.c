#include <idt.h>
#include <stdio.h>


extern void irq0();	extern void irq10();
extern void irq1();	extern void irq11();
extern void irq2();	extern void irq12();
extern void irq3();	extern void irq13();
extern void irq4();	extern void irq14();	
extern void irq5();	extern void irq15();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();

void irq_install(void)
{

	interrupter(0x20,(unsigned int)irq0, 8,0);
	interrupter(0x21,(unsigned int)irq1, 8,0);
	interrupter(0x22,(unsigned int)irq2, 8,0);
	interrupter(0x23,(unsigned int)irq3, 8,0);
	interrupter(0x24,(unsigned int)irq4, 8,0);
	interrupter(0x25,(unsigned int)irq5, 8,0);
	interrupter(0x26,(unsigned int)irq6, 8,0);
	interrupter(0x27,(unsigned int)irq7, 8,0);
	interrupter(0x28,(unsigned int)irq8, 8,0);
	interrupter(0x29,(unsigned int)irq9, 8,0);
	interrupter(0x2A,(unsigned int)irq10, 8,0);
	interrupter(0x2B,(unsigned int)irq11, 8,0);
	interrupter(0x2C,(unsigned int)irq12, 8,0);
	interrupter(0x2D,(unsigned int)irq13, 8,0);
	interrupter(0x2E,(unsigned int)irq14, 8,0);
	interrupter(0x2F,(unsigned int)irq15, 8,0);

}

void irq_function(int  n)
{
	printf("IRQ%d\n",n-32);

}
