#include <i965.h>
#include <pci.h>
#include <stdio.h>
#include <gui.h>
#include <string.h>


i965_t gtt[1];

int i965_pci_init(int bus, int dev, int fun)
{	
	unsigned data = pci_read_config_dword(bus, dev, fun, 0x00);

	gtt->did = data >>16 &0xffff;
	gtt->vid = data &0xffff;
	
	gtt->mmio_base	= pci_read_config_dword(bus, dev, fun, 0x10) &0xFFFFFFF0;
	gtt->memory 	= pci_read_config_dword(bus, dev, fun, 0x18) &0xFFFFFFF0;
	gtt->iobar 	= pci_read_config_dword(bus, dev, fun, 0x20) &0xFFFFFFF0;
	
	return 0;
}

framebuffer_t fb[2];
mode_t mode[1];
int i965()
{

	unsigned int data = pci_scan_bcc(3);
	
	if(data == -1) {
	
		printf("panic: PCI Display Controller not found!\n");
		return -1;
	}
	
	printf("Display Controller\n");

	i965_pci_init(data  >>  8 &0xff,data  >> 3  &31,data &7);
	
	
	printf("device id %x, vendor id %x\n",gtt->did,gtt->vid);
	printf("mmio_base 0x%x, memory 0x%x, iobar 0x%x\n",gtt->mmio_base,gtt->memory,gtt->iobar );
	
	
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
	enable_pipe(gtt);
	
	// Set a framebuffer address and stride and enable the primary plane and all secondary planes that you wish to use.
	for(int i=0;i<2;i++){
		*(unsigned int*)(gtt->mmio_base + 0x70184 + i*0x1000) = 0; // plane offser
		*(unsigned int*)(gtt->mmio_base + 0x70188 + i*0x1000) = fb[i].stride; // plane stride
		*(unsigned int*)(gtt->mmio_base + 0x7019c + i*0x1000) = fb[i].address; // plane address
	
	}


	enable_plane(gtt);
	enable_dac(gtt);
	
	
	for(int y=0;y < mode->height;y++)
	for(int x=0;x < mode->width;x++) *(unsigned int*)((unsigned int*)gtt->memory+x+(mode->width*y)) = 0x000088;
	
	
	gui->horizontal_resolution = mode->width;
	gui->vertical_resolution = mode->height;
	gui->pixels_per_scan_line	= gui->horizontal_resolution;
	
	gui->x 	= 0;
	gui->y 	= 0;
	gui->width 	= gui->horizontal_resolution;
	gui->height 	= gui->vertical_resolution;
	
	//clears_creen();
	
	gui->cursor_x = gui->cursor_y = 0;
	
	
	printf("Graphic Native Intel\n");
	
	
	memset((char*)0x50000,-1,64*64*4);
	
	
	*(unsigned int*)(gtt->mmio_base + 0x70084) = 0x50000;
	
	*(unsigned int*)(gtt->mmio_base + 0x70080) = 0x18000027;
	
	*(unsigned int*)(gtt->mmio_base + 0x70088) = 0x1000100;

	
	printf("Cursor A Control Register %x\n",*(unsigned int*)(gtt->mmio_base + 0x70080));
	printf("Cursor A Base Address Register %x\n",*(unsigned int*)(gtt->mmio_base + 0x70084));
	printf("Cursor A Position Register %x\n",*(unsigned int*)(gtt->mmio_base + 0x70088));
	

	
	return 0;
}


void timings(i965_t *driver, framebuffer_t fb[2], mode_t *mode)
{
	char *edid = (char *) (0x500 + 54);
	
	int x = (edid[2] &0xff) | (edid[4] >> 4 &0xf) << 8;
	int y = (edid[5] &0xff) | (edid[7] >> 4 &0xf) << 8;
	//int sx = (edid[12] &0xff) | (edid[14] >> 4 &0xf) << 8;
	//int sy = (edid[13] &0xff) | (edid[14] &0xf) << 8;
	
	// Setup the display timings of your desired mode
	DISPLAY_PIPELINE *pipe = (DISPLAY_PIPELINE*)(driver->mmio_base + 0x60000);

	for(int i=0;i < 2; i++) {
	
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
		
		pipe = (DISPLAY_PIPELINE*)(driver->mmio_base + 0x60000 + 0x1000);
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
