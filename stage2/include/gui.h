#ifndef __GUI_H
#define __GUI_H

extern struct _gui *gui;

typedef struct _gui
{
	// Default
	unsigned int	pitch;
	unsigned int 	bpp;
	unsigned int 	planes;
    	unsigned int	horizontal_resolution;  	//	width;
	unsigned int	vertical_resolution;  		//	hight;
 	unsigned int	pixels_per_scan_line;	
    	unsigned int	max_pixel_clock;
	unsigned int 	frame_buffer_size;
	unsigned int	*frame_buffer;
	unsigned int	*bank_buffer;
	unsigned int	*task_buffer;
	unsigned int	*window_screen;
	unsigned int	*window_background; 
	
	// Extra.area
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	
}__attribute__ ((packed)) gui_t;


void initialize_gui();
void put_pixel(long x, long y, unsigned int color);
void put_pixel_buff(long x, long y, unsigned int color,void *buffer);


#endif
