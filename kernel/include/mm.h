#ifndef __MM_H
#define __MM_H

void initialize_memory();
int mm_mp( unsigned long phy_addr, unsigned long *virt_addr,unsigned size, int flag);

#endif
