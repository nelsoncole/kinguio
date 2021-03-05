#include <hpet.h>


void sleep(unsigned long ms)
{
	if(!ms) return;

	hpet_sleep(ms, 0);

}
