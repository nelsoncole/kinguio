#include <io.h>
#include <ps2.h>

#include <sleep.h>


// Esta função é usada para a calibragem do IBF e OBF
// Se a entra é 0 = IBF, se entrada é 1 = OBF

void kbdc_wait(int type)
{

	int spin = 10;
      	if(type == 0) {

                  while(!inportb(0x64)&1 && spin--) {
                  
                  	sti();
                  	sleep(100); // 100 ms
                  	cli();
                  }

        } else if (type == 1) {

                  while(inportb(0x64)&2 && spin--)
                  {
                  
                  	sti();
                  	sleep(100); // 100 ms
                  	cli();
                  }

      	}
      	

}

void kbdc_set_cmd(int val)
{
	kbdc_wait(1);
  	outportb(0x64, val);

}
