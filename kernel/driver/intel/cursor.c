#include <i965.h>

unsigned int cursor18x18[18] = {

	0b100000000000000000,
	0b110000000000000000,
	0b111000000000000000,
	0b111100000000000000,
	0b110110000000000000,
	0b110011000000000000,
	0b110001100000000000,
	0b110000110000000000,
	0b110000011000000000,
	0b110000001100000000,
	0b110000000110000000,
	0b110000000011000000,
	0b110000000001100000,
	0b110000000000110000,
	0b111110000011111000,
	0b100011000011000000,
	0b000000110011000000,
	0b000000011110000000,


};


void DrawMouse(unsigned int color, unsigned int *buffer,void *MouseBuffer)
{
	unsigned int font = 0;
	const unsigned int *font_data = (const unsigned int*) MouseBuffer;
	unsigned int mask;
	unsigned int *u;
	
	for(int t=0;t < 18;t++) {
	
		u = (unsigned int *) buffer + 64*t;
		
		mask = 1;
        	font = font_data[t];
		
		for(int i= 18 -1 ;i >= 0;i--)
		{
			
                     if(font&mask) {
                       	u[i] = color;
                     }
                       
			mask += mask;
	
		}
			
		
	} 	

}


void setup_cursor(i965_t *dev)
{
	for(int i=0; i < 64*64; i++) {
	
		*(unsigned int*)(dev->cursor_memory + i*4) = 0;
	
	}
	
	
	DrawMouse(-1, (unsigned int *)(dev->cursor_memory),cursor18x18);
	
	
	
	*(unsigned int*)(dev->mmio_base + 0x70080) = 0x7;
	
	*(unsigned int*)(dev->mmio_base + 0x70088) = 0x01000200;
	
	*(unsigned int*)(dev->mmio_base + 0x70084) = 0x800000; // dev->cursor_memory + address


}


void update_graphic_cursor(int x, int y ){

	*(unsigned int*)(gtt->mmio_base + 0x70088) = (x &0x7fff) | (y &0x7fff) << 16;
}
