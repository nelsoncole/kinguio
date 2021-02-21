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
	mov dword[file_sector_count],ecx
	call main

	hlt

global gdt_flush
gdt_flush:
    	push ebp
	mov ebp, esp
    	mov eax, [esp+8]
    	lgdt [eax]
    	jmp 0x08:.flush
.flush:
    	mov ax, 0x10
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
	mov ss, ax

    	mov esp, ebp
	pop ebp
    	ret

global idt_flush
idt_flush:
    	push ebp
	mov ebp,esp
    	mov eax, [esp+8]
    	lidt [eax]
    	mov esp,ebp
	pop ebp
    	ret

global long_mode
long_mode:    
	push ebp
	mov ebp, esp
    	mov eax, [esp + 0x08]
    	mov edx, [esp + 0x0c]
    	mov ebx, [esp + 0x10]
    	lgdt [eax]
    	jmp 0x08:.flush
.flush:
    	mov ax, 0x10
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
	mov ss, ax

    	mov esp, ebp
	pop ebp
	
	cli
	push ebx
	jmp edx
	
	ret

global file_sector_count
file_sector_count dd 0

section .bss
	resb 0x8000 ;32KiB
_stack:
