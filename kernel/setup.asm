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

global gdt_flush

extern gdtr, gdt
gdt_flush:
    	mov rax, rdi
    	lgdt [rax]
    	
    	
    	;mov rax, gdtr
	;mov rcx, gdt
	;mov QWORD [rax+2], rcx
	;lgdt [rax]
    	
;    	jmp 0x08:.flush
;.flush:
;    	mov ax, 0x10
;    	mov ds, ax
;    	mov es, ax
;    	mov fs, ax
;    	mov gs, ax
;	mov ss, ax

    	ret

global idt_flush
extern idtr
idt_flush:
   	mov rax, rdi
    	lidt [rax]

    	ret
	
section .bss
	resb 0x8000 ;32KiB
_stack:
