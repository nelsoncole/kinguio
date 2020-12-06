bits 32
section .text
global isr00,isr01,isr02,isr03,isr04,isr05,isr06,isr07,isr08,isr09
global isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19
global isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29
global isr30,isr31


extern fault_exception
isr_jmp:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
    	push ss
	mov eax,[esp+52]	; pega vector number
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	call fault_exception
	pop eax
	
    	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad	
	add esp,8
	sti
	iretd
isr00:
	push dword 0
	push dword 0
	jmp isr_jmp
isr01:
	push dword 0
	push dword 1
	jmp isr_jmp
isr02:
	push dword 0
	push dword 2
	jmp isr_jmp
isr03:
	push dword 0
	push dword 3
	jmp isr_jmp
isr04:
	push dword 0
	push dword 4
	jmp isr_jmp
isr05:
	push dword 0
	push dword 5
	jmp isr_jmp
isr06:
	push dword 0
	push dword 6
	jmp isr_jmp
isr07:
	push dword 0
	push dword 7
	jmp isr_jmp
isr08:
	;error code
	push dword 8
	jmp isr_jmp
isr09:
	push dword 0
	push dword 9
	jmp isr_jmp
isr10:
	;error code
	push dword 10
	jmp isr_jmp
isr11:
	;error code
	push dword 11
	jmp isr_jmp
isr12:
	;error code
	push dword 12
	jmp isr_jmp
isr13:
	;error code
	push dword 13
	jmp isr_jmp
isr14:
	;error code
	push dword 14
	jmp isr_jmp
isr15:
	push dword 0
	push dword 15
	jmp isr_jmp
isr16:
	push dword 0
	push dword 16
	jmp isr_jmp
isr17:
	;error code
	push dword 17
	jmp isr_jmp
isr18:
	push dword 0
	push dword 18
	jmp isr_jmp
isr19:
	push dword 0
	push dword 19
	jmp isr_jmp
isr20:
	push dword 0
	push dword 20
	jmp isr_jmp
isr21:
	push dword 0
	push dword 21
	jmp isr_jmp
isr22:
	push dword 0
	push dword 22
	jmp isr_jmp
isr23:
	push dword 0
	push dword 23
	jmp isr_jmp
isr24:
	push dword 0
	push dword 24
	jmp isr_jmp
isr25:
	push dword 0
	push dword 25
	jmp isr_jmp
isr26:
	push dword 0
	push dword 26
	jmp isr_jmp
isr27:
	push dword 0
	push dword 27
	jmp isr_jmp
isr28:
	push dword 0
	push dword 28
	jmp isr_jmp
isr29:
	push dword 0
	push dword 29
	jmp isr_jmp
isr30:
	push dword 0
	push dword 30
	jmp isr_jmp
isr31:
	push dword 0
	push dword 31
	jmp isr_jmp
	
global irq0,irq1,irq2,irq3,irq4,irq5,irq6,irq7,irq8,irq9
global irq10,irq11,irq12,irq13,irq14,irq15

extern irq_function
irq_jmp:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
    	push ss
	mov eax,[esp+52]	; pega vector number
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	pop eax
	push eax
	call irq_function
	pop eax	
    	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad	
	add esp,8

	sti
	iretd


irq0:
	push dword 0
	push dword 32
	jmp irq_jmp
irq1:
	push dword 0
	push dword 33
	jmp irq_jmp
irq2:
	push dword 0
	push dword 34
	jmp irq_jmp
irq3:
	push dword 0
	push dword 35
	jmp irq_jmp
irq4:
	push dword 0
	push dword 36
	jmp irq_jmp
irq5:
	push dword 0
	push dword 37
	jmp irq_jmp
irq6:
	push dword 0
	push dword 38
	jmp irq_jmp
irq7:
	push dword 0
	push dword 39
	jmp irq_jmp
irq8:
	push dword 0
	push dword 40
	jmp irq_jmp
irq9:
	push dword 0
	push dword 41
	jmp irq_jmp

irq10:
	push dword 0
	push dword 42
	jmp irq_jmp
irq11:
	push dword 0
	push dword 43
	jmp irq_jmp
irq12:
	push dword 0
	push dword 44
	jmp irq_jmp
irq13:
	push dword 0
	push dword 45
	jmp irq_jmp
irq14:
	push dword 0
	push dword 46
	jmp irq_jmp
irq15:
	push dword 0
	push dword 47
	jmp irq_jmp
