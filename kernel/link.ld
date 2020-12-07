/**
 *
 *	File Name: link.ld
 **/
OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start)
SECTIONS
{
	. = 0x401000;
	
	_start = .;
	
	.text : AT(ADDR(.text) - 0x400000)
	{
		 code = .;
    		*(.text)
    		*(.rodata)
    		. = ALIGN(4096);
	}
	
	.data : AT(ADDR(.data) - 0x400000)
	{
		data = .;
    		*(.data)
    		. = ALIGN(4096);
	}
	
	.bss : AT(ADDR(.bss) - 0x400000)
	{
		bss = .;
    		*(.bss)
		*(COMMON)
		. = ALIGN(4096);
	}
	
	_end = .;
}
