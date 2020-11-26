
strerr_read db "READ ERROR",0xa,0xd,0
read:
	; DI --> Segmento
	; BX --> Offset
	; EAX --> Starting sector LBA47
	
	pusha
	mov si,dap
	mov [si + 0], byte 0x10	; Tamanho da DAP
	mov [si + 1], byte 0		; reservado
	mov [si + 2], word 1		; Sector count
	mov [si + 4], BX		; Offset
	mov [si + 6], DI		; Segment
	mov [si + 8], EAX		; Start sector LBA47
	mov [si +12], dword 0		; Starting sector LBA47
	mov dl, byte [dv_num]
	mov ah, 0x42
	int 0x13
	jc  .err
	popa
	ret

.err:	
	popa
	mov si,strerr_read
	call print
	ret
