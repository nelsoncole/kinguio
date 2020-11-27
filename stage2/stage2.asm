[bits 32]
[org 0x10000]
	jmp start
	
	
start:
	mov esi,string
	call print

	hlt
	
	
print:
	xor edi,edi
	pushad
	mov edi,[_vmm]
.next:
	lodsb
	cmp al,0
	jz .end
	mov ah,0x2
	cld
	stosw
	jmp .next
.end:
	popad
	ret

_vmm dd 0xb8000 + 160 * 2

string db "RUN: STAGE 2",0
