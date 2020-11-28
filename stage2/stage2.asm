[bits 32]
section .text
global _start
_start:
	jmp start
	
	
	extern main
	extern dv_num
	extern dv_uid
	
	
start:

	mov esp, _stack
	mov ebp, 0;
	
	mov dword[dv_uid],ebx
	mov byte[dv_num],dl
	call main

	hlt

section .bss
	resb 0x2000 ;8KiB
_stack
