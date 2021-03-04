#include <hpet.h>


void sleep(unsigned long ms)
{

	hpet_sleep(ms, 0);

}
