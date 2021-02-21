#include <mm.h>
#include <paging.h>


#include <stdio.h>
#include <string.h>


#define PDPTE_ENTRY 3
#define DEVICE_MEMORY_MAP 0xC0000000
//C0000000h - FFFFFFFFh = 1024MiB
// align 4MiB
unsigned long g_mm_mp;


void initialize_memory()
{
	g_mm_mp = 0;
}

int mm_mp( unsigned long phy_addr, unsigned long *virt_addr,unsigned size, int flag) 
{

	pae_page_directory_t *pde = (pae_page_directory_t *) (0x140000);
	pae_page_table_t *pte = (pae_page_table_t *) (0x141000);
	pae_page_table_t *__pte = pte;
	
	memset(pde,0,512*sizeof(pae_page_directory_t));
	
	memset(pte,0,512*sizeof(pae_page_table_t)*1);
	
	unsigned long addr = phy_addr;
	
	//2 MiB
	for(int i=0;i < 512; i++) {
		
		__pte->p = 1;
		__pte->rw = 1;
		__pte->pcd= 1;
		__pte->frames = (addr >>12) &0xffffffffffff;
		__pte++;
		
		addr += 0x1000;
	}

	addr = (unsigned long)pte;
	pde->p = 1;
	pde->rw = 1;
	pde->pcd= 1;
	pde->phy_addr_pt = (addr >>12) &0xffffffffffff;
	
	
	// 0xC0000000  3GiB
	pdpte[PDPTE_ENTRY].p = 1;
	pdpte[PDPTE_ENTRY].rw = 1;
	pdpte[PDPTE_ENTRY].pcd= 1;
	addr = (unsigned long) pde;
	pdpte[PDPTE_ENTRY].phy_addr_pd = (addr >> 12) &0xfffffffff;
	

	//flush_tlb(); // Actualiza TLB
	//__asm__ __volatile__("invlpg (%0)" ::"r" (pml4e) : "memory");
	
	
	
	*(unsigned long*)(virt_addr) = DEVICE_MEMORY_MAP;
	
	return 0;
}
