#include <i965.h>
#include <pci.h>
#include <stdio.h>
#include <gui.h>
#include <string.h>
#include <mm.h>


i965_t gtt[1];


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

int i965_pci_init(int bus, int dev, int fun)
{	

	unsigned long bar0,bar1,bar2,bar3,bar4;
	
	unsigned data = pci_read_config_dword(bus, dev, fun, 0x00);

	gtt->did = data >>16 &0xffff;
	gtt->vid = data &0xffff;
	
	bar0	= pci_read_config_dword(bus, dev, fun, 0x10) &0xFFFFFFFF;
	bar1  	= pci_read_config_dword(bus, dev, fun, 0x14) &0xFFFFFFFF;
	bar2 	= pci_read_config_dword(bus, dev, fun, 0x18) &0xFFFFFFFF;
	bar3  	= pci_read_config_dword(bus, dev, fun, 0x1C) &0xFFFFFFFF;
	bar4 	= pci_read_config_dword(bus, dev, fun, 0x20) &0xFFFFFFFF;
	
	
#if defined(__x86_64__) || defined(__aarch64__)

	gtt->mmio_base =  (bar1 << 32 | bar0) &0xFFFFFFFFFFFFFFF0;
	gtt->memory = (bar3 << 32 | bar2) &0xFFFFFFFFFFFFFFF0;
	gtt->iobar = bar4 &0x00000000FFFFFFF0;		
#elif defined(__i386__) || defined(__arm__)
	
	gtt->mmio_base = bar0 &0xFFFFFFF0;
	gtt->memory = bar2 &0xFFFFFFF0;
	gtt->iobar = bar4 &0xFFFFFFF0;
#else
#error YOUR ARCH HERE
#endif

	
	return 0;
}

framebuffer_t fb[2];
mode_t mode[1];
int setup_i965(){

	unsigned int data = pci_scan_bcc(3);
	
	if(data == -1) {
	
		printf("panic: PCI Display Controller not found!\n");
		return -1;
	}
	
	printf("Display Controller\n");

	i965_pci_init(data  >>  8 &0xff,data  >> 3  &31,data &7);
	
	if((gtt->vid &0xffff) != 0x8086) { 
		
		printf("Graphic Native Intel, not found, device id %x, vendor id %x\n",gtt->did,gtt->vid);
		return 1;
	}
	
	
	
	//Mapear o mmio_base e memória de vídeo
	mm_mp( gtt->mmio_base, &gtt->mmio_base, 0, 0);
	
	//for(;;);
	
	
	/*char edid[128];
	
	gmbus_read(gtt,0x50, edid, 128);
	
	printf("edid = %s",edid);*/
	
	//disable
	disable_dac(gtt);
	disable_plane(gtt);
	disable_pipe(gtt);
	disable_dpll(gtt);
	disable_legacy_vga_emul(gtt);

	//enable
	enable_dpll(gtt);
	
	// Setup the display timings of your desired mode
	timings(gtt,fb,mode);
	
	gtt->memory = gui->virtual_buffer;
	
	for(int y=0;y < mode->height;y++) {
		for(int x=0;x < mode->width;x++) {
			*(unsigned int*)((unsigned int*)gtt->memory+x+(mode->width*y)) = 0x0;
		}
	}
	
	
	enable_pipe(gtt);
	
	// Set a framebuffer address and stride and enable the primary plane and all secondary planes that you wish to use.
	for(int i=0;i<2;i++){
	
		*(unsigned int*)(gtt->mmio_base + 0x70184 + i*0x1000) |= 0; // plane offser
		*(unsigned int*)(gtt->mmio_base + 0x70188 + i*0x1000) |= fb[i].stride; // plane stride
		*(unsigned int*)(gtt->mmio_base + 0x7019c + i*0x1000) |= fb[i].address; // plane address
	
	}
	
	
	
	//Nelson, quanto tempo eu devo esperar aqui?
	for(int i=0; i <100000;i++)outanyb(0x80);


	enable_plane(gtt);
	
	enable_dac(gtt);
	

	
	
	gui->horizontal_resolution = mode->width;
	gui->vertical_resolution = mode->height;
	gui->pixels_per_scan_line	= gui->horizontal_resolution;
	
	
	gui->x 	= 0;
	gui->y 	= 0;
	gui->width 	= gui->horizontal_resolution;
	gui->height 	= gui->vertical_resolution;
	
	
	gui->cursor_x = gui->cursor_y = 0;
	
	//for(int x=0; x < 1000; x++)*(unsigned int*)((unsigned int*)gtt->memory+x+(mode->width*0)) = -1;
	
	//printf("Graphic Native Intel,  mode->width %d, mode->height %d\n", mode->width,mode->height);
	
	
	
	/*for(int i=0; i < 64*64; i++) {
	
		*(unsigned int*)(0x500000 + gtt->memory + i*4) = 0;
	
	}
	
	
	DrawMouse(-1, (unsigned int *)(0x500000 + gtt->memory),cursor18x18);
	
	
	
	*(unsigned int*)(gtt->mmio_base + 0x70080) = 0x7;
	
	*(unsigned int*)(gtt->mmio_base + 0x70088) = 0x01000200;
	
	*(unsigned int*)(gtt->mmio_base + 0x70084) = 0x500000;

*/
	return 0;
}


void timings(i965_t *driver, framebuffer_t fb[2], mode_t *mode)
{
	char *edid = (char *) (0x500 + 54);
	
	int x = (edid[2] &0xff) | (edid[4] >> 4 &0xf) << 8;
	int y = (edid[5] &0xff) | (edid[7] >> 4 &0xf) << 8;
	//int sx = (edid[12] &0xff) | (edid[14] >> 4 &0xf) << 8;
	//int sy = (edid[13] &0xff) | (edid[14] &0xf) << 8;
	
	
	printf("x = %d, y = %d\n",x,y);


	for(int i=0;i < 2; i++) {
	
	
		// Setup the display timings of your desired mode
	DISPLAY_PIPELINE *pipe = (DISPLAY_PIPELINE*)(driver->mmio_base + 0x60000 + (0x1000*i) );
	
		fb[i].width = x;
		fb[i].height = y;
		fb[i].stride = fb[i].width * 4;
		fb[i].address = 0; //4MiB
	
		mode->width = x;
		mode->height = y;
		mode->bpp = 32;
		
		if(fb[i].stride%64) {
	
			mode->width = (fb[i].stride/64);
			mode->width = (mode->width*64)/4;	
	
		}
	
	
		pipe->htotal.active = fb[i].width  - 1;
		pipe->vtotal.active = fb[i].height - 1;
		pipe->pi_peasrc.h_image_size = mode->width - 1;
		pipe->pi_peasrc.v_image_size = mode->height -1;
		
	}
	
}


void disable_dac(i965_t *driver) 
{
	// ADPA (Analog Display Port A)
	*(unsigned int*)(driver->mmio_base + ADPA) &=~0x80000000;  // disable
	*(unsigned int*)(driver->mmio_base + ADPA) |= 0xc00;	// Monitor off
	
}

void enable_dac(i965_t *driver)
{
	// ADPA (Analog Display Port A)
	*(unsigned int*)(driver->mmio_base + ADPA) |=0x80000000;  // enable
	*(unsigned int*)(driver->mmio_base + ADPA) &=~0xc00;	// Monitor on
}

// Register DSPACNTR
void disable_plane(i965_t *driver)
{	// Display Plane A (Primary A) disable
	*(unsigned int*)(driver->mmio_base + DSPACNTR) &=~0x80000000;

	// Display Plane B (Primary B) disable
	*(unsigned int*)(driver->mmio_base + DSPACNTR + 0x1000) &=~0x80000000;

	// Display Plane C (Primary C) disable
	*(unsigned int*)(driver->mmio_base + DSPACNTR + 0x2000) &=~0x80000000;
}
void enable_plane(i965_t *driver)
{	// Display Plane A (Primary A) enable
	*(unsigned int*)(driver->mmio_base + DSPACNTR) |=0xc0000000; 

	//Display Plane B (Primary B) enable
	*(unsigned int*)(driver->mmio_base + DSPACNTR + 0x1000) |=0xc0000000; 

	//Display Plane C (Primary C) enable
	//*(unsigned int*)(driver->mmio_base + DSPACNTR + 0x2000) |=0xc0000000;
}

// Register PIPEACONF
void disable_pipe(i965_t *driver)
{	// Pipe A Configuration Register
	*(unsigned int*)(driver->mmio_base + PIPEACONF) &=~0xc0000000;  // disable
	// Pipe B Configuration Register
	*(unsigned int*)(driver->mmio_base + PIPEACONF + 0x1000) &=~0xc0000000; // disable
}

void enable_pipe(i965_t *driver)
{	// Pipe A Configuration Register
	*(unsigned int*)(driver->mmio_base + PIPEACONF) |= 0xc0000000; // enable
	// Pipe B Configuration Register
	*(unsigned int*)(driver->mmio_base +  PIPEACONF + 0x1000) |= 0xc0000000; // enable
}

//
void disable_dpll(i965_t *driver)
{

}

void enable_dpll(i965_t *driver)
{

	*(unsigned int*)(driver->mmio_base + DPLLA_CNTL) = 0x94020C00;

}
// Registro VGACNTRL
void disable_legacy_vga_emul(i965_t *driver)
{	unsigned char data = inpb(VGA_CLOCKING_MODE_CTRL)  | 0x20; // set bit 5 screen off
	outpb(VGA_CLOCKING_MODE_CTRL,data);
	// VGA plane control disable
	*(unsigned int*)(driver->mmio_base + VGA_PLANE_CTRL) |=0x80000000;
	// VGA Centering Enable
	*(unsigned int*)(driver->mmio_base + VGACNTRL) &=~0x3000000;
	// VGA Display Disable
	*(unsigned int*)(driver->mmio_base + VGACNTRL) |=0x80000000;
}


// GMBUS functions
void gmbus_reset(i965_t *driver)
{  	
   	/**(unsigned int*)(driver->mmio_base + GMBUS1) = 0;
   	
   	gmbus_enable_writeprot(driver);
   	gmbus_disable_writeprot(driver);
   	*/
}

void gmbus_stoptransaction(i965_t *driver)
{
	*(unsigned int*)(driver->mmio_base + GMBUS1) = (1 << 30) | (1 << 27);
}

void gmbus_wait(i965_t *driver)
{
	while( ! ( *(unsigned int*)(driver->mmio_base + GMBUS2) & (1 << 11) ) );
}

void gmbus_read(i965_t *driver,unsigned int offset, void *buffer, int size)
{
	int i;
 	unsigned int gmbus0_val = 0;
   	unsigned int gmbus1_val = 0;
   	
   	unsigned char *buf = (unsigned char *)buffer;
   	
   	// SW Ready
   	// WAIT
   	// INDEX
   	// READ
   	gmbus1_val = (1 << 30) | (1 << 25) | (1 << 26) | (size & 511) << 16 | ((offset & 127) << 1) | 1;
   	
   	gmbus_reset(driver);
   	
   	*(unsigned int*)(driver->mmio_base + GMBUS0) = gmbus0_val;
   	*(unsigned int*)(driver->mmio_base + GMBUS2) = gmbus1_val;

	 for (i = 0; i < size; i += 4) {
       
       	gmbus_wait(driver);
       	unsigned int bytes = *(unsigned int*)(driver->mmio_base + GMBUS3);
       	
       	buf[i] = bytes & 0xFF;
       	if (size > i + 1) buf[i + 1] = (bytes >> 8) & 0xFF;
       	if (size > i + 2) buf[i + 2] = (bytes >> 16) & 0xFF;
       	if (size > i + 3) buf[i + 3] = (bytes >> 24) & 0xFF;
   	}

   	gmbus_stoptransaction(driver);

}


void gmbus_write(unsigned int offset, const void *buffer, int size)
{


}
