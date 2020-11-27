[bits 32]
section .text
extern main
global _start
_start:
	jmp start
	mov esp, _stack
	mov ebp, 0;
	
	
start:
	
	call main

	hlt
	
section .bss
	resb 0x2000 ;8KiB
_stack:
