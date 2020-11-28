#ifndef __GUI_H
#define __GUI_H

extern struct _gui *gui;
extern short font8x16[128*16];

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
	
	//glyph
	unsigned int cursor_x;
	unsigned int cursor_y;
	
}__attribute__ ((packed)) gui_t;


void initialize_gui();
void put_pixel(long x, long y, unsigned int color);
void put_pixel_buff(long x, long y, unsigned int color,void *buffer);
void refresh_rate();
void clears_creen();
void draw_char_transparent( int x, int y, int ch, unsigned int fg_color, 
							void *buffer,
							void *font_buffer);
int glyph(int ch, unsigned int color);


#endif
