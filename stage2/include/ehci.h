#ifndef __EHCI_H
#define __EHCI_H

// 0x8086 0x24DD (USB EHCI—D29:F7) ICH5 


typedef volatile struct EHCI_PORT
{
	unsigned int ccsts:1;       // Current Connect Status
    	unsigned int cstsc:1;       // Connect Status Change
   	unsigned int ped:1;         // Port Enabled/Disabled
    	unsigned int pdc:1;         // Port Enable/Disable Change 
    	unsigned int overa:1;       // Overcurrent Active
   	unsigned int overc:1;       // Overcurrent Change 
 	unsigned int fpresume:1;    // Force Port Resume
	unsigned int Suspend:1;     // Suspend 
    	unsigned int preset:1;      // Port Reset
    	unsigned int rsv0:1;
    	unsigned int lsts:2;        // Line Status
    	unsigned int pp:1;          // Port Power
    	unsigned int po:1;          // Port Owner 
   	unsigned int rsv1:2;
    	unsigned int ptc:4;         // Port Test Control 
    	unsigned int wce:1;         // Wake on Connect Enable (WKCNNT_E)
    	unsigned int wde:1;         // Wake on Disconnect Enable (WKDSCNNT_E)
    	unsigned int woe:1;         // Wake on Overcurrent Enable (WKOC_E)
    	unsigned int rsv2:9;

}__attribute__((packed)) EHCI_PORT_T;

typedef volatile struct EHCI_MEM
{
    	unsigned short caplength;     // Capabilities Registers Length
    	unsigned short hcivrsion;     // Host Controller Interface Version Number
   	unsigned int hcsparams;     // Host Controller Structural Parameters, value default(00103206h(D29:F7), 00102205(D26:F7))
    	unsigned int hccparams;     // Host Controller Capability Parameters
    	unsigned char  rsv0[0x20-0x0C];
    	// +offset 20h
  	unsigned int cmd;           // USB2.0_CMD USB 2.0 Command 
    	unsigned int sts;           // USB2.0_STS USB 2.0 Status
    	unsigned int intr;          // USB 2.0_INTR USB 2.0 Interrupt Enable 
    	unsigned int frindex;       // USB 2.0 Frame Index
    	unsigned int ctrldssegment; // Control Data Structure Segment 
    	unsigned int perodiclistbase; // Period Frame List Base Address 
    	unsigned int asynclistaddr; // Current Asynchronous List Address 
    	unsigned char  rsv1[0x60-0x3C]; // Reserved
    	unsigned int configflag;    // Configure Flag
    	EHCI_PORT_T port[6];    // Port x Status and Control TODO Port6 74h–77h (D29 Only), Port7 78h–7Bh (D29 Only)


    	// Debug Port Register Address Map
    	unsigned char  rsv2[0xA0-0x7C];
    	unsigned int ctrlsts;       // Control/Status 
    	unsigned int pid;           // USB PIDs
    	unsigned int datal;       // Data Buffer (Bytes 7:0)
    	unsigned int datah;
    	unsigned int config;        // config   
    	unsigned char  rsv3[0x400-0xB4]; 
}__attribute__((packed)) EHCI_MEM_T;



typedef struct _ehci {
	unsigned int vid;
	unsigned int did;
	unsigned int mmio_base;
}__attribute__((packed)) ehci_t;



int ehci_init(void);




#endif
