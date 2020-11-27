LDSBASE equ 0x600
LDSPARAMETERSBASE equ 0x4000
LDSVESABASE equ 0x4020

string db "RUN: STAGE 1",0xd,0xa,0
string2 db "[PRESS ENTER TO CONTINUE ...]",0
strerror db "BOOTLOADER ERROR",0

dv_num db 0
uid dd 0
dap times 16 db 0
vbe_mode_info_block: times 512 db 0

; partition

super_block_reserved dd 0
super_block_byte_per_sector dd 0
super_block_sector_por_blk dd 0
super_block_root_blk dd 0

; outro
data_sector dd 0
first_sector dd 0
first_blk dd 0
address_offset dw 0

file_address_di dw 0
file_address_bx dw 0
filesystem_address dw 0
filename db "stage2.bin                                                                                      ",0 ;96 bytes