[bits 16]
[org 0x8000]

	jmp 0:start
	
	%include "data.inc"
	%include "print.asm"
	%include "read.asm"
	%include "getpm.asm"
	%include "a20.asm"
	%include "vesa.asm"
	%include "gdt.asm"


start:
	cli
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ax,0x9000 
	mov ss,ax  ; stack = 0x90000
	xor ax,ax
	mov sp,ax
	sti

; Salvar informações de disco
	mov byte[dv_num],dl
	mov dword[uid],ebx
	
	mov si, string
	call print
	
	
	mov si,string2
	call print
	
	xor ax,ax
	int 0x16
	
	
; BIOSes Get Device Parameters
	call GetDeviceParameters

; Habilita Gate A20
	call gatea20

; Testa a Gate A20 



; Carregar o setup


; Definir o modo VESA
	call vesa_vbe_mode
	
	
; Mascara interrupções e desabilita NMI
	
	cli
	in al,0x70
	or al,0x80
	out 0x70,al

; Instala a GDT
    	mov eax,gdtr
	db 0x66		; Execute GDT 32-Bits
	lgdt [eax]

; Habilita o PE 
	mov eax,cr0
	or eax,1
	mov cr0,eax

; Pula para modo protegido

	xor edx,edx
	mov dl,byte[dv_num]
	mov ebx,dword[uid]
	
	;jmp dword 0x8:ModoProtegido	; aqui vamos pular do stage1 para o setup
	
	hlt
	
	
;times (1024*32 - ($-$$)) nop
string db "RUN: STAGE 1",0xd,0xa,0
string2 db "[PRESS ENTER]",0









