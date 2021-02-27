#ifndef __MM_H
#define __MM_H

#define DEVICE_PAGE_DIR 	0x140000
#define DEVICE_PAGE_TABLE 	0x141000
#define PDPTE_ENTRY 		3
#define DEVICE_MEMORY_MAP 	0xC0000000

//C0000000h - FFFFFFFFh = 1024MiB
// align 2 MiB

void initialize_mm_mp();
int mm_mp( unsigned long phy_addr, unsigned long *virt_addr,unsigned size, int flag);

#endif
