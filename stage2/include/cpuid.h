#ifndef __CPUID_H
#define __CPUID_H

enum {
	
	// Basic CPUID Information
	CPUID_GET_VENDOR_STRING,
	CPUID_GET_FEATURES,
	CPUID_GET_TLB,
	CPUID_GET_SERIAL,
	// Extended Function CPUID Information
	CPUID_GET_EXTENDED = 0x80000000,
	CPUID_GET_EXTENDED_FEATURES,
	CPUID_GET_BRAND_STRING,
	CPUID_GET_BRAND_STRING_MORE,
	CPUID_GET_BRAND_STRING_END
	
	
	
}CPUID_REQUEST;


int cpuid_vendor(char *string);
int cpuid_processor_brand(char *string);


#endif



// Volume 2: Instruction Set Reference, A-Z p471, Volume 3 p2701, Volume 4: Model-Specific Registers p4413
