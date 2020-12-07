[bits 64]
section .text
global _start
_start:
	jmp start
	
start:
	mov rsp, _stack
	mov rbp, 0;

	mov rax,0xff000000ff0000
	mov rdi,0x3c00000 + 800*4 + 2400
	
	
	mov rcx,200
	mov rbx,0
.loop:
	mov [rdi + rbx],rax
	add rbx,8
	
	loop .loop

	hlt
	
section .bss
	resb 0x2000 ;8KiB
_stack:
