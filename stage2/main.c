unsigned short *vmm = (unsigned short *)0xb8000+ 80*2;
void puts(const char *s)
{
	while(*s) *vmm++ =*s++ | 0xe << 8;

}

void main()
{

	puts("RUN: STAGE 2 ==> 32-BIT");

}
