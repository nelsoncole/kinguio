#include <mm.h>
#include <paging.h>

#include <stdio.h>
#include <string.h>


//Estas função é para mapear dispositvos, mm_mp(...)
unsigned long g_mm_mp_index;
pae_page_directory_t *device_pde;

void initialize_mm_mp()
{

	device_pde = (pae_page_directory_t *) (DEVICE_PAGE_DIR);
	memset(device_pde,0,512*sizeof(pae_page_directory_t));
	
	unsigned long addr = (unsigned long) device_pde;
	
	// 0xC0000000  3GiB
	pdpte[DEVICE_PDPTE_ENTRY].p = 1;
	pdpte[DEVICE_PDPTE_ENTRY].rw = 1;
	pdpte[DEVICE_PDPTE_ENTRY].pcd= 1;
	pdpte[DEVICE_PDPTE_ENTRY].phy_addr_pd = (addr >> 12) &0xfffffffff;
	
	
	g_mm_mp_index = 0;
	
}

int mm_mp( unsigned long phy_addr, unsigned long *virt_addr,unsigned size, int flag) 
{

	pae_page_directory_t *pde = device_pde;
	
	pae_page_table_t *pte = (pae_page_table_t *) (DEVICE_PAGE_TABLE + (0x1000*g_mm_mp_index));
	pae_page_table_t *__pte = pte;
	
	unsigned long addr = phy_addr;
	int index = g_mm_mp_index;
	
	if(size > 0x40000000 )
	{
		printf("Memory Map error, size > 1GiB\n");
		*(unsigned long*)(virt_addr) = 0;
		return 1;
	
	}else if(size > (0x40000000-(index*0x200000)))
	{
		printf("Memory Map error, size > %d bytes, (0x%d bytes)\n",(0x40000000-(index*0x200000)),size);
		*(unsigned long*)(virt_addr) = 0;
		return 2;
	} 
	
	int count = (size/PAGE_SIZE);
	
	if(size%PAGE_SIZE)
	{
		count++;
	}
	
	int count2 = (count/512);
	
	if(count%512)
	{
		count2++;
	}
	
	
	memset(pte,0,512*sizeof(pae_page_table_t)*count2);
	
	//2 MiB
	for(int i=0;i < count; i++) {
		
		__pte->p = 1;
		__pte->rw = 1;
		__pte->pcd= 1;
		
		if(flag&1){
			__pte->us = 1;
		}
		__pte->frames = (addr >>12) &0xffffffffffff;
		__pte++;
		
		addr += 0x1000;
	}

	addr = (unsigned long)pte;
	
	for(int i=0;i < count2; i++) {
		pde[index + i].p = 1;
		pde[index + i].rw = 1;
		if(flag&1){
			pde[index + i].us = 1;
		}
		pde[index + i].pcd= 1;
		pde[index + i].phy_addr_pt = (addr >>12) &0xffffffffffff;
		
		addr += 0x1000;
		
		g_mm_mp_index++;
	}
	

	//flush_tlb(); // Actualiza TLB
	//__asm__ __volatile__("invlpg (%0)" ::"r" (pml4e) : "memory");
	
	*(unsigned long*)(virt_addr) = (DEVICE_MEMORY_MAP + (index * 0x200000));
	
	return 0;
}

// TODO
// 0x2000000 -- 0x3000000 -- 16 MiB RAM_BITMAP
// 0x3000000 -- 0x3001000 -- 4 KiB ALLOC_PAGE_DIR
// 0x3001000 -- 0x3201000 -- 2 MiB ALLOC_PAGE_TABLE

unsigned char *RAM_BITMAP;
pae_page_directory_t *alloc_pde;
pae_page_table_t *alloc_pte;
unsigned long ram_setup(unsigned long entry_pointer_info)
{

	RAM_BITMAP = (unsigned char*) (0x2000000);
	
	// Reserved Kernel 64MiB
	int len = ((64*1024)/4)/8;
	
	memset(RAM_BITMAP, -1, len);
	
	// Free 64 MiB to 128 MiB
	int offset = len;
	len = (((128 - 64)*1024)/4)/8;
	
	memset(RAM_BITMAP + offset, 0, len);

	return ((unsigned long) RAM_BITMAP);
}

unsigned long alloc_frame()
{
	unsigned char *bmp = RAM_BITMAP + 2048;
	
	for(unsigned int i = 0; i <  2048; i++ ){
	
		for(int t = 0; t < 8; t++) {
		
			if( !(*bmp >> t &0x1) ) {
			
				*bmp |= 1 << t;
				return 0x4000000 + ( (i*8+t)*0x1000); //64MiB +++
			}
		}
		
		bmp++;
	}
	
	return 0;
}

unsigned long alloc_pages_setup(unsigned long entry_pointer_info)
{
	alloc_pde = (pae_page_directory_t *) (0x3000000);
	pae_page_directory_t *pde = alloc_pde;
	memset( pde,0, 512*sizeof(pae_page_directory_t));
	
	
	alloc_pte = (pae_page_table_t *) (0x3001000);
	pae_page_table_t *pte = alloc_pte;
	memset(pte, 0, 512*sizeof(pae_page_table_t) *512);
	
	unsigned long addr = 0;
	// 1 GiB
	for(int i=0;i < 512*512; i++) {
		
		pte->p = 0;
		pte->rw = 1;
		pte->frames = (addr >>12) &0xffffffffffff;
		pte++;
		
	}
	
	addr = (unsigned long) alloc_pte;
	for(int i=0;i < 512; i++) {
		
		pde->p = 1;
		pde->rw = 1;
		//if(i > 31) pde->us = 1;
		pde->phy_addr_pt = (addr >>12) &0xffffffffffff;
		
		addr +=0x1000;
		pde++;
	}
	
	// 0x40000000  1GiB
	addr = (unsigned long) alloc_pde;
	pdpte[1].p = 1;
	pdpte[1].rw = 1;
	pdpte[1].pcd= 1;
	pdpte[1].phy_addr_pd = (addr >> 12) &0xffffffffffff;
	
	
	return 0;
}

/**
 * Aloca bloco de 4KiB
 *
 **/
unsigned long alloc_pages(int type, unsigned len, unsigned long *addr)
{
	//pae_page_directory_t *pde = alloc_pde;
	pae_page_table_t *pte = alloc_pte;
	
	int index = 0;
	
	for(int i = 0; i < 512*512; i++)
	{
		if(pte->p == 0) {
			index = i;
			
			for(int y=0; y < len; y++) {
				unsigned long frame = alloc_frame();
				pte->p = 1;
				pte->frames = (frame >>12) &0xffffffffffff;
				
				pte++;
			}
			
			break;
		}
	
		pte++;
	}
	
	
	
	*(unsigned long*)(addr) = (ALLOC_PAGE_MEMORY + (index * 0x1000));

	return (len);

}

void free_pages(void *addr)
{
	

}

