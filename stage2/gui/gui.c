#include <gui.h>
#include <stdlib.h>
#include <string.h>
struct _gui *gui;

extern void puts(const char *s);
void initialize_gui()
{
	gui = (struct _gui*) malloc(sizeof(gui_t));
	memset(gui,0,sizeof(gui_t));

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

void refresh_rate() 
{
	return; //NOTA 
	
	memcpy(gui->frame_buffer, gui->bank_buffer,\
	gui->width * (gui->height) * (gui->bpp/8));
	
}


void clears_creen() 
{
	memset(gui->frame_buffer/*G->BankBuffer*/,0,gui->width * (gui->height) * (gui->bpp/8));
}


void draw_char_transparent( int x, int y, int ch, unsigned int fg_color, 
							void *buffer,
							void *font_buffer)
{
	unsigned short font = 0;
	const unsigned short *font_data = (unsigned short*) font_buffer;
   	int cx, cy;
	unsigned short mask;
    
   	for(cy=0;cy < 16;cy++){
		mask = 1;
        	font = font_data[(ch *16) + cy];
		
                for(cx = 8 -1 ;cx >= 0;cx--){ 
                       if(font&mask){
				//put_pixel_buff(x + cx,y + cy,fg_color,buffer);
				put_pixel(x + cx,y + cy,fg_color);
			}
			mask += mask;
                  
          }
        
    }

}


int glyph(int ch, unsigned int color)
{

	if(ch == '\n') {
	
		gui->cursor_x = 0;
		gui->cursor_y++;
		return ch;
	
	}
	
	if(gui->cursor_y*16 >= gui->height) {
	
		gui->cursor_y = 0;
	}
	
	if(gui->cursor_x*8 >= gui->width) {
	
		gui->cursor_x = 0;
		gui->cursor_y++;
	}

	draw_char_transparent(gui->cursor_x*8, gui->cursor_y*16, ch, color, gui->bank_buffer,font8x16);
	
	gui->cursor_x++;
	
	return ch;
}