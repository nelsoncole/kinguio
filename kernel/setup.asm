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
	
L1:	hlt
	jmp L1

global gdt_flush
gdt_flush:
    	mov rax, rdi
    	lgdt [rax]

    	mov rax, 0x10
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
	mov ss, ax
    	ret

global idt_flush
idt_flush:
   	mov rax, rdi
    	lidt [rax]

    	ret
    	
global call_function
call_function:
   	mov rax, rdi
   	mov rdi, rsi
    	call rax
    	ret
	
section .bss
	resb 0x8000 ;32KiB
_stack:
