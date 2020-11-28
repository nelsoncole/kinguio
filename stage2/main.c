#include <stdio.h>
#include <stdlib.h>
#include <gui.h>
#include <pci.h>

extern unsigned char dv_num;
extern unsigned int dv_uid;
void main()
{

	initialize_heap(DEF_HEAP_START);
	
	initialize_gui();
	
	printf("RUN: STAGE 2 ==> 32-BIT, %x %x\n",dv_num,dv_uid);
	
	pci_get_info(0,2);

}
