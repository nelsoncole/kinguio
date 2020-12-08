#include <gui.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct _gui gui[1];
gui_t *g;

extern void puts(const char *s);
void initialize_gui(unsigned int *pointer)
{

	// GUI	
	g = (gui_t*) (0x100000 + 0x40);
	memcpy(gui,g,sizeof(gui_t));
	
	gui->bank_buffer	= 0; // 8MB
	gui->task_buffer	= 0;
	gui->window_screen	= 0;
	gui->window_background	= 0;
			
	// Extra.area		
	gui->x 	= 0;
	gui->y 	= 0;
	gui->width 	= gui->horizontal_resolution;
	gui->height 	= gui->vertical_resolution;


	gui->font.x = 8;
	gui->font.y = 16;
	gui->font.fg_color = 0xffffff;
	gui->font.bg_color = 0;
	gui->font.buf = (unsigned long)font8x16;

	clears_creen();

}


void put_pixel(long x, long y, unsigned int color)
{
	*(unsigned int*)((unsigned int*)gui->virtual_buffer + (gui->pixels_per_scan_line * y) + x) = color;
	
}

void put_pixel_buff(long x, long y, unsigned int color,void *buffer)
{
	
	unsigned int *buf = (unsigned int*)buffer;

	if((x >= gui->horizontal_resolution) || (gui->vertical_resolution) ) return;	

	*(unsigned int*)((unsigned int*)buf + (gui->pixels_per_scan_line * y) + x) = color;
	
}

void refresh_rate() 
{
	unsigned char *dest = ( unsigned char*)gui->virtual_buffer;
	unsigned char *src  = ( unsigned char*)gui->bank_buffer;
	
	memcpy(dest, src,gui->width * (gui->height) * (gui->bpp/8));
	
}


void clears_creen() 
{
	memset((unsigned*)gui->virtual_buffer/*G->BankBuffer*/,0,gui->width * (gui->height) * (gui->bpp/8));
	
}


void draw_char_transparent( int x, int y, int ch, unsigned int fg_color, 
							void *buffer,
							struct _font *font)
{
	unsigned short f = 0;
	const unsigned short *font_data = (unsigned short*) font->buf;
   	int cx, cy;
	unsigned short mask;
    
   	for(cy=0;cy < font->y ;cy++){
		mask = 1;
        	f = font_data[(ch *font->y) + cy];
		
                for(cx = font->x -1 ;cx >= 0;cx--){ 
                       if(f&mask){
				//put_pixel_buff(x + cx,y + cy,fg_color,buffer);
				put_pixel(x + cx,y + cy,fg_color);
			}
			mask += mask;
                  
          }
        
    }

}


int glyph(int ch)
{

	if(ch == '\n') {
	
		gui->cursor_x = 0;
		gui->cursor_y++;
		return ch;
	
	}
	
	if(gui->cursor_y*gui->font.y >= gui->height) {
	
		gui->cursor_y = 0;
	}
	
	if(gui->cursor_x*gui->font.x >= gui->width) {
	
		gui->cursor_x = 0;
		gui->cursor_y++;
	}

	draw_char_transparent(gui->cursor_x*gui->font.x, gui->cursor_y*gui->font.y, ch, gui->font.fg_color,
	(void *)gui->bank_buffer,&gui->font);
	
	gui->cursor_x++;
	
	return ch;
}