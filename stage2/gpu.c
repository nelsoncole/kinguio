#include <stdio.h>
#include <stdint.h>
#include <pci.h>
#include <gui.h>

/*  BLT Engine Status (40000h 4FFFFh) (Software Debug) */
#define BR00                  0x40000
#define BR01                  0x40004
#define BR02                  0x40008
#define BR03                  0x4000C
#define BR04                  0x40010
#define BR05                  0x40014
#define BR06                  0x40018
#define BR07                  0x4001C
#define BR08                  0x40020
#define BR09                  0x40024
#define BR10                  0x40028
#define BR11                  0x4002C
#define BR12                  0x40030
#define BR13                  0x40034
#define BR14                  0x40038
#define BR15                  0x4003C
#define BR16                  0x40040
#define BR17                  0x40044
#define BR18                  0x40048
#define BR19                  0x4004C

#define BR20                  0x40050
#define BR21                  0x40054
#define BR22                  0x40058
#define BR23                  0x4005C
#define BR24                  0x40060
#define BR25                  0x40064

#define SSLADD                0x40074
#define DSLH                  0x40078
#define DSLRADD               0x4007C



// Display Clock Control Registers (06000h–06FFFh)
#define DPLLA_CNTL	0x06014
#define DPLLB_CNTL	0x06018
#define ADPA 		0x61100

// Display Pipeline A
#define PIPEA_DSL	0x70000
#define PIPEA_SLC	0x70004
#define PIPEACONF 	0x70008


//Display A (Primary) Plane Control
#define DSPACNTR	0x70180
#define DSPALINOFF	0x70184
#define DSPASTRIDE	0x70188
#define DSPASURF	0x7019C
#define DSPATILEOFF	0x701A4



//VGA Plane Control registers
// Bit 31 = plane control disable
// Bit 29 = plane pipe select
#define VGA_PLANE_CTRL 	0x41000

//VGA control registers
#define VGACNTRL 	0x71400
// SR01⎯Clocking Mode
#define VGA_CLOCKING_MODE_CTRL 0x3C5



// Port control registers 
// documentado em PCH Display Registers
#define CRT_PORT_CTRL 	0xE1100
#define HDMIC_PORT_CTRL 0xE1150
#define HDMID_PORT_CTRL 0xE1160
#define LVDS_PORT_CTRL 	0xE1180

// nao documentado
#define PIPE_DATA_M1 0x60030



unsigned int GTTMMADR = 0;
unsigned int GTTMMADR2 = 0;


#define MCHBAR 	0x10000


typedef struct _TOTAL {
	//Pipe Horizontal Total Register
	unsigned int 	active :12;
	unsigned int 	rsv1 :4;
	unsigned int	total :13;
	unsigned int	rsv2 :3;
}__attribute__ ((packed)) TOTAL;

typedef struct _BLANK {
	// Pipe Blank Register
	unsigned int 	start :13;
	unsigned int 	rsv1 :3;
	unsigned int	end :13;
	unsigned int	rsv2 :3;
}__attribute__ ((packed)) BLANK;
typedef struct _SYNC {
	// Pipe Sync Register
	unsigned int 	start :13;
	unsigned int 	rsv1 :3;
	unsigned int	end :13;
	unsigned int	rsv2 :3;
}__attribute__ ((packed)) SYNC;
typedef struct _PIPEASRC {
	// Pipe Source Image Size
	unsigned int 	v_image_size :12;
	unsigned int 	rsv1 :4;
	unsigned int	h_image_size :12;
	unsigned int	rsv2 :4;	
}__attribute__ ((packed)) PIPEASRC;
typedef struct _BCLRPAT {
	// Pipe Border Color Pattern Register
	unsigned int 	border_blue :8;
	unsigned int 	border_green :8;
	unsigned int	border_red :8;
	unsigned int	rsv :8;	
}__attribute__ ((packed)) BCLRPAT;
typedef struct _VSYNCSHIFT {
	// Vertical Sync Shift Register
	unsigned int 	shift :13;
	unsigned int 	rsv :19;
}__attribute__ ((packed)) VSYNCSHIFT;

typedef struct _CRCCTRLRED {
	// Pipe CRC Color Channel Control Register (Red)
	unsigned int	value :23;
	unsigned int	rsv :5;
	unsigned int	select :3;
	unsigned int	crc :1;

}__attribute__ ((packed)) CRCCTRLRED;

typedef struct _CRCCTRLX {
	// Pipe CRC Color Channel Control Register  (Green, Blue, Residual)
	unsigned int	value :23;
	unsigned int	rsv :9;
}__attribute__ ((packed)) CRCCTRLX;

typedef struct _CRCRESX {
	//  Pipe CRC Color Channel Result Register (Red, Green, Blue, Residual1)
	unsigned int	value :23;
	unsigned int	rsv :9;
}__attribute__ ((packed)) CRCRESX;

// Display Engine Pipeline Registers (60000h–6FFFFh)
typedef volatile struct _DISPLAY_PIPELINE {

	TOTAL 	htotal;
	BLANK 	hblank;
	SYNC 	hsynic;
	TOTAL	vtotal;		
	BLANK	vblank;
	SYNC 	vsynic;
	unsigned int 	rsv1;
	PIPEASRC	pi_peasrc;
	BCLRPAT		bclrpat;
	unsigned int	rsv2;
	VSYNCSHIFT 	vsyncshift;
	unsigned int	rsv3;
	CRCCTRLRED	crc_red_control;
	CRCCTRLX	crc_green_control;
	CRCCTRLX	crc_blue_control;
	CRCCTRLX	crc_residual_control;

	CRCRESX	crc_red_result;
	CRCRESX	crc_green_result;
	CRCRESX	crc_blue_result;
	CRCRESX	crc_residual_result;
	//end
}__attribute__ ((packed)) DISPLAY_PIPELINE;


struct _framebuffer {

	unsigned int width;
	unsigned int height;
	unsigned int stride;
	unsigned int address;
};



typedef struct _pci {
	uint8_t fun;
	uint8_t dev;
	uint8_t bus;
	uint8_t rsvd;
}pci_t;

static int gpu_pci(int bus,int dev,int fun)
{

	unsigned int did,vid;

	printf("PCI Display Controller\n");
	uint32_t data = 0;
	uint32_t z0 = (fun &0xff) | ((dev &0xff) << 8) | ((bus &0xff) << 16);
	pci_t *pci = (pci_t*)((uint32_t)&z0);
	
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x00);

	did = data >>16 &0xffff;
	vid = data &0xffff;
	printf("Device ID: %X Vendor ID: %X\n",did,vid);
	
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x10);

	GTTMMADR = data&0xFFFFFFF0; 

	printf("BAR0 (MMIO region address): %X\n",GTTMMADR);
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x14);
	printf("BAR1: %X\n",data);
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x18);
	GTTMMADR2 = data&0xFFFFFFF0;
	printf("BAR2 (Frame buffer address): %X\n",GTTMMADR2);
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x1C);
	printf("BAR3: %X\n",data);
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x20);
	printf("BAR4 (I/O Base Address): %X\n",data);
	data = pci_read_config_dword(pci->bus,pci->dev,pci->fun,0x24);
	printf("BAR5: %X\n",data);


	if(did == 0x116)printf("HD Graphics 3000\n");



	return 0;
}

// Programming the DPLL
// DPLL = (reference frequency * (5 * (M1 - 2) + (M2 - 2)) / N) / (P1 * P2) 
// formula que gera o relogio de pixel
// Variable 	DPLL 		VCO 		N 	M 	M1 	M2 	P 	P1 	P2
// Min 		25,000 kHz 	1,750,000 kHz 	1 	104 	17 	5 	10 	1 	10
// Max 		270,000 kHz 	3,500,000 kHz 	4 	138 	23 	11 	30 	3 	10


unsigned long N, M1, M2, P1, P2;


// Register ADPA
void disable_all_output_conectors(unsigned int mmio)
{
	// ADPA (Analog Display Port A)
	*(unsigned int*)(mmio + ADPA) &=~0x80000000;  // disable

	*(unsigned int*)(mmio + ADPA) |= 0xc00;	// Monitor off


}
void enable_all_output_conectors(unsigned int mmio)
{

	// ADPA (Analog Display Port A)
	*(unsigned int*)(mmio + ADPA) |=0x80000000;  // enable

	*(unsigned int*)(mmio + ADPA) &=~0xc00;	// Monitor on

}


// Register DSPACNTR
void disable_all_plane(unsigned int mmio)
{
	// Display Plane A (Primary A) disable
	*(unsigned int*)(mmio + DSPACNTR) &=~0x80000000;

	// Display Plane B (Primary B) disable
	*(unsigned int*)(mmio + DSPACNTR + 0x1000) &=~0x80000000;

	// Display Plane C (Primary C) disable
	*(unsigned int*)(mmio + DSPACNTR + 0x2000) &=~0x80000000;

}
void enable_all_plane(unsigned int mmio)
{
	// Display Plane A (Primary A) enable
	*(unsigned int*)(mmio + DSPACNTR) |=0x80000000; 

	//Display Plane B (Primary B) enable
	*(unsigned int*)(mmio + DSPACNTR + 0x1000) |=0x80000000; 

	//Display Plane C (Primary C) enable
	*(unsigned int*)(mmio + DSPACNTR + 0x2000) |=0x80000000;

}

// Register PIPEACONF
void disable_display_pipe(unsigned int mmio)
{
	// Pipe A Configuration Register
	*(unsigned int*)(mmio + PIPEACONF) &=~0xc0000000;  // disable
	// Pipe B Configuration Register
	*(unsigned int*)(mmio + PIPEACONF + 0x1000) &=~0xc0000000; // disable


}


void enable_display_pipe(unsigned int mmio)
{
	
	// Pipe A Configuration Register
	*(unsigned int*)(mmio + PIPEACONF) |= 0xc0000000; // enable
	
	// Pipe B Configuration Register
	*(unsigned int*)(mmio +  PIPEACONF + 0x1000) |= 0xc0000000; // enable
}


// Registro VGACNTRL
void disable_legacy_vga_emul(unsigned int mmio)
{

	unsigned char data = inpb(VGA_CLOCKING_MODE_CTRL)  | 0x20; // set bit 5 screen off
	outpb(VGA_CLOCKING_MODE_CTRL,data);

	// VGA plane control disable
	*(unsigned int*)(mmio + VGA_PLANE_CTRL) |=0x80000000;

	// VGA Centering Enable
	*(unsigned int*)(mmio + VGACNTRL) &=~0x3000000;

	// VGA Display Disable
	*(unsigned int*)(mmio + VGACNTRL) |=0x80000000;
}


int gpu() 
{

	unsigned int data = pci_scan_bcc(3);
	if(data == -1) {
		printf("panic: PCI Display Controller not found!\n");
		return -1;
	}

	gpu_pci(data  >>  8 &0xff,data  >> 3  &31,data &7);


	// FIXME Native Intel graphic mode


	/* More specifically disabling the display hardware consists of the following steps: 
	 *
	 * 1 - 	Disable all output connectors. 
	 *
	 * 2 - 	Disable all planes. This includes the primary plane and cursor planes. 
	 *
	 * 3 - 	Disable the display pipe.
	 *
	 * 4 - 	Disable the DPLL.
	 *
	 * 5 -	Disable the legacy VGA emulation. 
	 */

	disable_all_output_conectors(GTTMMADR);
	disable_all_plane(GTTMMADR);
	disable_display_pipe(GTTMMADR);
	// DPLL
	disable_legacy_vga_emul(GTTMMADR);



	/* Enabling the display reverses this sequence:
	 *
	 * 1 - 	Program the DPLL to generate a suitable pixel 
	 *	clock and enable it. Wait for the clock to stabilize.
	 *
	 * 2 - 	Setup the display timings of your desired mode and enable the display pipe
	 *
	 * 3 - 	Set a framebuffer addressand stride and 
	 *  	enable the primary plane and all secondary planes that you wish to use
	 *
	 * 4 - 	Enable the output connectors
	 */
	
	// DPLL
	*(unsigned int*)(GTTMMADR + DPLLA_CNTL) = 0x94020C00;
	
	// Setup the display timings of your desired mode
	DISPLAY_PIPELINE *pipeline = (DISPLAY_PIPELINE*)(GTTMMADR + 0x60000);

	


	struct _framebuffer fb;
	
	fb.width = 1366;
	fb.height = 768;
	fb.stride = fb.width * 4;
	fb.address = 0;
	
	int width = fb.width;
	if(fb.stride%64) {
	
	
		width = (fb.stride/64);
		width = (width*64)/4;
		
	
	}
	
	
	
	
	pipeline->htotal.active = fb.width  - 1;
	pipeline->vtotal.active = fb.height - 1;
	pipeline->pi_peasrc.h_image_size = width-1;//pipeline->htotal.active;
	pipeline->pi_peasrc.v_image_size = pipeline->vtotal.active;
	

	enable_display_pipe(GTTMMADR);
	
	
	*(unsigned int*)(GTTMMADR + 0x70184) = 0; // plane offser
	*(unsigned int*)(GTTMMADR + 0x70188) = fb.stride; // plane stride
	*(unsigned int*)(GTTMMADR + 0x7019c) = fb.address; // plane address
	
	
	//TODO:testando o modo grafico native.
	
	gui->horizontal_resolution = width;//fb.width;
	gui->vertical_resolution = fb.height;
	gui->pixels_per_scan_line	= gui->horizontal_resolution;
	
	gui->x 	= 0;
	gui->y 	= 0;
	gui->width 	= gui->horizontal_resolution;
	gui->height 	= gui->vertical_resolution;
	
	clears_creen();
	
	for(int y=0;y < 100;y++)
	for(int x=0;x < 100;x++) *(unsigned int*)((unsigned int*)gui->virtual_buffer+x+(gui->width*y)) = 0xff0000ff;
	
	for(int x=0;x < gui->width;x++) *(unsigned int*)((unsigned int*)gui->virtual_buffer+x+(gui->width*140)) = 0xffff0000;
	
	for(int x=0;x < gui->width;x++) *(unsigned int*)((unsigned int*)gui->virtual_buffer+x+(gui->width*180)) = 0xff00ff00;
	
	enable_all_plane(GTTMMADR);
	enable_all_output_conectors(GTTMMADR);
	
	
	printf("%d %d %d\n",*(unsigned int*)(GTTMMADR + 0x70188),*(unsigned int*)(GTTMMADR + 0x7019c),fb.stride);

	printf("pipeline->htotal.total %d\n",pipeline->htotal.total);
	printf("pipeline->vtotal.total %d\n",pipeline->vtotal.total);

	printf("pipeline->hblank.start %d\n",pipeline->hblank.start);
	printf("pipeline->hblank.end %d\n",pipeline->hblank.end);
	printf("pipeline->vblank.start %d\n",pipeline->vblank.start);
	printf("pipeline->vblank.end %d\n",pipeline->vblank.end);

	printf("pipeline->hsynic.start %d\n",pipeline->hsynic.start);
	printf("pipeline->hsynic.end %d\n",pipeline->hsynic.end);
	printf("pipeline->vsynic.start %d\n",pipeline->vsynic.start);
	printf("pipeline->vsynic.end %d\n",pipeline->vsynic.end);
	

	printf("pipeline->pi_peasrc.h_image_size %d\n",pipeline->pi_peasrc.h_image_size);
	printf("pipeline->pi_peasrc.v_image_size %d\n",pipeline->pi_peasrc.v_image_size);



	return (0);
}
