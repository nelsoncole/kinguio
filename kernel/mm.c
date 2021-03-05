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
	pdpte[PDPTE_ENTRY].p = 1;
	pdpte[PDPTE_ENTRY].rw = 1;
	pdpte[PDPTE_ENTRY].pcd= 1;
	pdpte[PDPTE_ENTRY].phy_addr_pd = (addr >> 12) &0xfffffffff;
	
	
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

unsigned long alloc_pages_initialize()
{

	/*AllocTablePages = (UINT8*) ((_end) + 0x1000*256);
	
	// For 32 MiB = (8192*4KiB)
	setmem((UINT8*)AllocTablePages,8192*sizeof(UINT8),0);

	return ((UINTN)AllocTablePages);*/
	
	return 0;
}

/**
 * Aloca bloco de 4KiB
 *
 **/
unsigned long alloc_pages(int type, unsigned len, unsigned long *addr)
{

	return (0);

}

void free_pages(void *addr)
{
	

}
