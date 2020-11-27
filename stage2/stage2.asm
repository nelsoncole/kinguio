[bits 16]
[org 0]

	jmp 0x1000:start
	
print:
	pusha
.next:
	cld 			; flag de direcção
	lodsb			; a cada loop carrega si p --> al, actualizando si
	cmp 	al, 0		; compara al com o 0
	je 	.end		; se al for 0 pula para o final do programa	
	mov	ah, 0x0e	; função TTY da BIOS imprime caracter na tela
	int 	0x10		; interrupção de vídeo
	jmp 	.next
.end:
	popa
	ret

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
	
	mov si, string
	call print
	
	xor ax,ax
	int 0x16
	
	int 0x19

string db 0xd,0xa,"RUN: STAGE 2",0xd,0xa,0
