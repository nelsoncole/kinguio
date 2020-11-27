#include <stdlib.h>
#include <gui.h>

void main()
{

	initialize_heap(DEF_HEAP_START);
	
	initialize_gui();
	
	for(int i=0;i < gui-> width-200;i++) put_pixel(i, 8, 0xff0000);

	//puts("RUN: STAGE 2 ==> 32-BIT");

}
