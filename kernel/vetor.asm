%include "include/inc.asm"
[bits 64]
[section .text]
global isr00,isr01,isr02,isr03,isr04,isr05,isr06,isr07,isr08,isr09
global isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19
global isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29
global isr30,isr31

extern fault_exception
isr_jmp:

	push gs
	push fs
	
	
	
	
	
	
	mov ax, 0x10
	mov gs,ax
	mov fs,ax
	
	;PUSH_FPU
	;PUSH_XMM
	
	mov rdi, [rsp+16]
	call fault_exception
	
	;POP_XMM
	;POP_FPU

	
	pop fs
	pop gs
	
	
	add rsp, 16
	iretq

isr00:
	push qword 0
	push qword 0
	jmp isr_jmp
isr01:
	push qword 0
	push qword 1
	jmp isr_jmp
isr02:
	push qword 0
	push qword 2
	jmp isr_jmp
isr03:
	push qword 0
	push qword 3
	jmp isr_jmp
isr04:
	push qword 0
	push qword 4
	jmp isr_jmp
isr05:
	push qword 0
	push qword 5
	jmp isr_jmp
isr06:
	push qword 0
	push qword 6
	jmp isr_jmp
isr07:
	push qword 0
	push qword 7
	jmp isr_jmp
isr08:
	;error code
	push qword 8
	jmp isr_jmp
isr09:
	push qword 0
	push qword 9
	jmp isr_jmp
isr10:
	;error code
	push qword 10
	jmp isr_jmp
isr11:
	;error code
	push qword 11
	jmp isr_jmp
isr12:
	;error code
	push qword 12
	jmp isr_jmp
isr13:
	;error code
	push qword 13
	jmp isr_jmp
isr14:
	;error code
	push qword 14
	jmp isr_jmp
isr15:
	push qword 0
	push qword 15
	jmp isr_jmp
isr16:
	push qword 0
	push qword 16
	jmp isr_jmp
isr17:
	;error code
	push qword 17
	jmp isr_jmp
isr18:
	push qword 0
	push qword 18
	jmp isr_jmp
isr19:
	push qword 0
	push qword 19
	jmp isr_jmp
isr20:
	push qword 0
	push qword 20
	jmp isr_jmp
isr21:
	push qword 0
	push qword 21
	jmp isr_jmp
isr22:
	push qword 0
	push qword 22
	jmp isr_jmp
isr23:
	push qword 0
	push qword 23
	jmp isr_jmp
isr24:
	push qword 0
	push qword 24
	jmp isr_jmp
isr25:
	push qword 0
	push qword 25
	jmp isr_jmp
isr26:
	push qword 0
	push qword 26
	jmp isr_jmp
isr27:
	push qword 0
	push qword 27
	jmp isr_jmp
isr28:
	push qword 0
	push qword 28
	jmp isr_jmp
isr29:
	push qword 0
	push qword 29
	jmp isr_jmp
isr30:
	push qword 0
	push qword 30
	jmp isr_jmp
isr31:
	push qword 0
	push qword 31
	jmp isr_jmp
