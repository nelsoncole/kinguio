%include "include/inc.asm"
[BITS 64]
[section .text]
global isr00,isr01,isr02,isr03,isr04,isr05,isr06,isr07,isr08,isr09
global isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19
global isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29
global isr30,isr31

extern fault_exception
isr_jmp:

	push gs
	push fs
	
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rdi
	push rsi
	push rbp
	push rsp
	push rbx
	push rdx
	push rcx
	push rax
      	
      	;PUSH_FPU
	;PUSH_XMM
	


	
	mov ax, 0x10
	mov gs,ax
	mov fs,ax
	
	mov rdi, [rsp + 0x90]
	
	call fault_exception
	
	;POP_XMM
	;POP_FPU

	pop rax
	pop rcx
	pop rdx
	pop rbx
	pop rsp
	pop rbp
	pop rsi
	pop rdi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	
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
	
global lvt0,lvt1,lvt2,lvt3,lvt4
extern lvt_function
lvt_jmp:
	push gs
	push fs

	
	mov ax, 0x10
	mov gs,ax
	mov fs,ax
	
	;PUSH_FPU
	;PUSH_XMM
	
	mov rdi, [rsp+16]
	call lvt_function
	
	;POP_XMM
	;POP_FPU

	
	pop fs
	pop gs
	
	
	add rsp, 16
	iretq
	
lvt0:
	push qword 0
	push qword 32
	jmp lvt_jmp

lvt1:
	push qword 0
	push qword 33
	jmp lvt_jmp
lvt2:
	push qword 0
	push qword 34
	jmp lvt_jmp
lvt3:
	push qword 0
	push qword 35
	jmp lvt_jmp
lvt4:
	push qword 0
	push qword 36
	jmp lvt_jmp



global irq0,irq1,irq2,irq3,irq4,irq5,irq6,irq7,irq8,irq9
global irq10,irq11,irq12,irq13,irq14,irq15,irq16,irq17,irq18,irq19
global irq20,irq21,irq22,irq23

extern irq_function
irq_jmp:

	push gs
	push fs
      	
      	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rdi
	push rsi
	push rbp
	push rsp
	push rbx
	push rdx
	push rcx
	push rax
      	
      	;PUSH_FPU
	;PUSH_XMM

	
	mov ax, 0x10
	mov gs,ax
	mov fs,ax
	
	mov rdi, [rsp + 0x90]
	call irq_function
	
	
	
	;POP_XMM
	;POP_FPU

	pop rax
	pop rcx
	pop rdx
	pop rbx
	pop rsp
	pop rbp
	pop rsi
	pop rdi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	
	pop fs
	pop gs
	
	
	add rsp, 16
	iretq
	
irq0:
	push qword 0
	push qword 64 
	jmp irq_jmp
	
irq1:
	push qword 0
	push qword 65 
	jmp irq_jmp

irq2:
	push qword 0
	push qword 66 
	jmp irq_jmp

irq3:
	push qword 0
	push qword 67 
	jmp irq_jmp

irq4:
	push qword 0
	push qword 68 
	jmp irq_jmp
	
irq5:
	push qword 0
	push qword 69 
	jmp irq_jmp
	
irq6:
	push qword 0
	push qword 70 
	jmp irq_jmp

irq7:
	push qword 0
	push qword 71 
	jmp irq_jmp

irq8:
	push qword 0
	push qword 72 
	jmp irq_jmp

irq9:
	push qword 0
	push qword 73 
	jmp irq_jmp

irq10:
	push qword 0
	push qword 74 
	jmp irq_jmp
	
irq11:
	push qword 0
	push qword 75 
	jmp irq_jmp

irq12:
	push qword 0
	push qword 76 
	jmp irq_jmp

irq13:
	push qword 0
	push qword 77
	jmp irq_jmp

irq14:
	push qword 0
	push qword 78
	jmp irq_jmp
	
irq15:
	push qword 0
	push qword 79 
	jmp irq_jmp
	
irq16:
	push qword 0
	push qword 80 
	jmp irq_jmp

irq17:
	push qword 0
	push qword 81 
	jmp irq_jmp

irq18:
	push qword 0
	push qword 82 
	jmp irq_jmp

irq19:
	push qword 0
	push qword 83 
	jmp irq_jmp

irq20:
	push qword 0
	push qword 84 
	jmp irq_jmp
	
irq21:
	push qword 0
	push qword 85 
	jmp irq_jmp

irq22:
	push qword 0
	push qword 86 
	jmp irq_jmp

irq23:
	push qword 0
	push qword 87 
	jmp irq_jmp
