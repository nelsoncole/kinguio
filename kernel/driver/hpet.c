#include <acpi.h>
#include <stdio.h>
#include <string.h>

#include <mm.h>

void setup_hpet()
{
	char sig[5] = {0,0,0,0,0};
	
	printf("HPET_Setup ... \\\\ \n");
	 
	hpet = (HPET*) acpi_probe(rsdt, xsdt, "HPET");
	
	if(hpet){
	
		memcpy(sig,&hpet->signature,4);
		
		printf("HPET was found, ");
		printf("Signature = '%s'. Length = %d. Revision 0x%x\n",sig,hpet->length,hpet->rersion);
		

		
	
	}else {
	
		printf("HPET not found\n");
	}

}
