#include <gui.h>
#include <stdlib.h>
struct _gui *gui;

extern void puts(const char *s);
void initialize_gui()
{
	gui = (struct _gui*) malloc(sizeof(gui_t));

		// BIOS VESA Video and Audio
	unsigned char *vbe = (unsigned char*)0x40200;
	unsigned char *vbe_info_mode = (unsigned char*)0x40200 + 0x200;

	// GUI
	gui->pitch 		= (*(unsigned short*)(vbe_info_mode + 16));
	gui->bpp 			= (*(unsigned char*)(vbe_info_mode  + 25));
	gui->planes		= (*(unsigned char*)(vbe_info_mode  + 24));
	gui->horizontal_resolution	= (*(unsigned short*)(vbe_info_mode + 18));  	//	width;
	gui->vertical_resolution	= (*(unsigned short*)(vbe_info_mode + 20)); 	//	hight;
	gui->pixels_per_scan_line	= gui->horizontal_resolution;//GraphicsOutputProtocol->Mode->Info->PixelsPerScanLine;	
	gui->max_pixel_clock	= 0;
	gui->frame_buffer	= (unsigned int*) (*(unsigned int*)(vbe_info_mode + 40));
	gui->frame_buffer_size	= (*(unsigned short*)(vbe + 18));
	gui->bank_buffer	= (unsigned int*) malloc(0x800000); // 8MB
	gui->task_buffer	= 0;
	gui->window_screen	= 0;
	gui->window_background	= 0;
			
	// Extra.area		
	gui->x 	= 0;
	gui->y 	= 0;
	gui->width 	= gui->horizontal_resolution;
	gui->height 	= gui->vertical_resolution;



}


void put_pixel(long x, long y, unsigned int color)
{
	
	*(unsigned int*)((unsigned int*)gui->frame_buffer + (gui->pixels_per_scan_line * y) + x) = color;
	
}

void put_pixel_buff(long x, long y, unsigned int color,void *buffer)
{
	
	unsigned int *buf = (unsigned int*)buffer;

	if((x >= gui->horizontal_resolution) || (gui->vertical_resolution) ) return;	

	*(unsigned int*)((unsigned int*)buf + (gui->pixels_per_scan_line * y) + x) = color;
	
}

