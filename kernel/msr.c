#include <msr.h>


int getmsr(unsigned int msr, unsigned int *low, unsigned int *hight)
{
	__asm__ __volatile__("rdmsr":"=a"(*low),"=d"(*hight):"c"(msr));
	return 0;
}

int setmsr(unsigned int msr, unsigned int low, unsigned int hight)
{
	__asm__ __volatile__("wrmsr": :"a"(low),"d"(hight),"c"(msr));
	return 0;
}
