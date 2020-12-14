#include <ehci.h>
#include <stdio.h>
#include <pci.h>

EHCI_MEM_T *ehci_driver;
ehci_t ehci;

static int ehci_pci_configuration_space(int bus,int dev,int fun)
{

    	unsigned int data;
    	// VENDOR and DEVICE ID offset 0
    	data  = read_pci_config_addr(bus,dev,fun,0);
    	ehci.vid = data &0xffff;
    	ehci.did = data >> 16 &0xffff;

    	// BAR
    	data  = read_pci_config_addr(bus,dev,fun,0x10);
    	ehci.mmio_base = data &~0x3ff;

    	return 0;
}


int ehci_init(void)
{
	printf("EHCI initialize\n");
    	unsigned int data = pci_scan_bcc_scc_prog(0xC,0x3,0x20);
    	if(data  == -1)	{
    		puts("PCI PANIC: EHCI (USB2.0) Controller not found!\n");
    		return (-1); // Abortar
    	}

    	ehci_pci_configuration_space(data  >>  24 &0xff,data  >> 16  &0xff,data &0xffff);
    	printf("vid 0x%x, did 0x%x, mmio 0x%x\n",ehci.vid,ehci.did,ehci.mmio_base);
    	
    	
    	ehci_driver= (EHCI_MEM_T*)ehci.mmio_base;

    	printf("Testing EHCI (USB2.0)\n");
    	
    	printf("caplength 0x%x\n",ehci_driver->caplength);
    	printf("hcivrsion 0x%x\n",ehci_driver->hcivrsion);
    	printf("Host Controller Structural Parameters 0x%x\n",ehci_driver->hcsparams);
    	printf("Host Controller Capability Parameters 0x%x\n",ehci_driver->hccparams);

    	printf("USB2.0_CMD USB 2.0 Command  %x\n",ehci_driver->cmd);
    	printf("USB2.0_STS USB 2.0 Status %x\n",ehci_driver->sts);
    	printf("USB 2.0 Frame Index %x\n",ehci_driver->frindex);
    	printf("Control Data Structure Segment  %x\n",ehci_driver->ctrldssegment);
    	printf("Period Frame List Base Address %x\n",ehci_driver->perodiclistbase);
    	printf("Current Asynchronous List Address %x\n",ehci_driver->asynclistaddr);
    	printf("Configure Flag %x\n",ehci_driver->configflag);
    	
    	for(int i=0; i < 6;i++)printf("porta %d = %x\n",i,ehci_driver->port[i]);

	return 0;
}
