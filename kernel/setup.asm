[bits 64]
section .text
global _start
_start:
	jmp start
	extern main
start:
	mov rsp, _stack
	mov rbp, 0;
	
	xor rdi,rdi
	mov edi,ebx
	call main
	
	hlt
	
section .bss
	resb 0x4000 ;32KiB
_stack:
