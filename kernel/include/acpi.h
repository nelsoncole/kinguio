#ifndef __ACPI_H
#define __ACPI_H


// BIOS Data Area (BDA)
#define BDA 0x400
#define EBDA (BDA + 0xE)



typedef struct _RSDP {

	// ACPI Version 1.0
	char signature[8];
   	unsigned char checksum;
   	char oem_id[6];
   	unsigned char revision;
   	unsigned int rsdt_addr;
   	
   	// ACPI Version 2.0
   	unsigned int length;
 	unsigned long long xsdt_addr;
 	unsigned char extended_check_sum;
 	unsigned char reserved[3];
   	
}__attribute__ ((packed)) RSDP;

typedef struct _RSDT {

	// ACPI Version 1.0
	char signature[4];
	unsigned int length;
	unsigned char revision;
   	unsigned char checksum;
   	char oem_id[6];
   	char oem_table_id[8];
    	unsigned int oem_revision;
    	unsigned int creator_id;
    	unsigned int creator_revision;
    	
    	unsigned int *entry; // 4*n
   	
   	
}__attribute__ ((packed)) RSDT;


typedef struct _XSDT {

	// ACPI Version 1.0
	char signature[4];
	unsigned int length;
	unsigned char revision;
   	unsigned char checksum;
   	char oem_id[6];
   	char oem_table_id[8];
    	unsigned int oem_revision;
    	unsigned int creator_id;
    	unsigned int creator_revision;
    	
    	unsigned long long *entry; // 8*n
   	
   	
}__attribute__ ((packed)) XSDT;

typedef struct _FADT {
	// Header
    	char signature[4];
    	unsigned int length;
    	unsigned char major_version;
    	unsigned char checksum;
    	char oem_id[6];
    	char oem_table_id[8];
    	unsigned int oem_revision;
    	unsigned int creator_id;
    	unsigned int creator_revision;
    	unsigned int FIRMWARE_CTRL;
    	unsigned int  DSDT;
   	unsigned char unneded2[48 - 44];
   	unsigned int  SMI_CMD;
   	unsigned char ACPI_ENABLE;
  	unsigned char ACPI_DISABLE;
   	unsigned char unneded3[64 - 54];
   	unsigned int  PM1a_CNT_BLK;
   	unsigned int  PM1b_CNT_BLK;
  	unsigned char unneded4[89 - 72];
   	unsigned char PM1_CNT_LEN;
       
}__attribute__ ((packed)) FADT;


typedef struct _DSDT {
	// Header
    	char signature[4];
    	unsigned int length;
    	unsigned char rersion;
    	unsigned char checksum;
    	char oem_id[6];
    	char oem_table_id[8];
    	unsigned int oem_revision;
    	unsigned int creator_id;
    	unsigned int creator_revision;
    	unsigned int definition_block;
       
}__attribute__ ((packed)) DSDT;


typedef struct _HPET {
	// Header
    	char signature[4];
    	unsigned int length;
    	unsigned char rersion;
    	unsigned char checksum;
    	char oem_id[6];
    	char oem_table_id[8];
    	unsigned int oem_revision;
    	unsigned int creator_id;
    	unsigned int creator_revision;
    	// Event Timer Block ID
    	unsigned char hardware_rev_id;
    	unsigned char comparator_count:5;
    	unsigned char counter_size:1;
   	unsigned char reserved:1;
    	unsigned char legacy_replacement:1;
    	unsigned short pci_vendor_id;
    	
    	// BASE ADDRESS
    	unsigned char address_space_id;    // 0 - system memory, 1 - system I/O
    	unsigned char register_bit_width;
    	unsigned char register_bit_offset;
    	unsigned char reserved2;
    	unsigned long long address;
    
    	unsigned char hpet_number;
    	unsigned short minimum_tick;
    	unsigned char page_protection;
    	   
}__attribute__ ((packed)) HPET;

extern RSDP *rsdp;
extern RSDT *rsdt;
extern XSDT *xsdt;
extern FADT *fadt;
extern DSDT *dsdt;
extern HPET *hpet;

unsigned long acpi_probe(RSDT *_rsdt, XSDT *_xsdt, char *signature);
void setup_acpi();
void poweroff(unsigned int timeout);

#endif
