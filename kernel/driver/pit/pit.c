#include <pit.h>
#include <io.h>

void pit_set_frequencia(unsigned int freq)
{

	unsigned int divisor = 1193180/freq;

	cli();
	
    	// Control uint16_t register
	// bits 7-6 = 0 - Set counter 0 (counter divisor),bit 5-4=11 LSB/MSB 16-bit
	// bit 3-1=x11 Square wave generator, bit 0 =0 Binary counter 16-bit
	outportb(0x43,0x36);
	outportb(0x40,(unsigned char)(divisor & 0xFF));		
	outportb(0x40,(unsigned char)(divisor >> 8) & 0xFF);

}


void pit_enable(int control){

	cli();
	
	if(control)
	{
		// Set mode 2 (rate generator)
		outportb(0x43, 0x36);
	
	} else {
	
		// Set mode 1 (hardware re-triggerable one-shot)
		outportb(0x43, 0x32 );
		
	}

}
