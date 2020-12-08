ASFLAGS64  :=-f elf64 -O0
CFLAGS64 :=-Wall -O0 -m64 -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -nostdlib -nostdinc -I ./include
LDFLAGS64  :=-m elf_x86_64 -Map kernel.map -T link.ld
AS=nasm
CC=gcc
LD=ld

objs= setup.o main.o string.o gui.o font8x16.o vsprintf.o stdio.o stdlib.o

.PHONY: kernel.bin move clean
	
kernel.bin: $(objs)
	$(LD) $(LDFLAGS64) -o $@ $^

%.o: %.asm
	$(AS) $(ASFLAGS64) $< -o $@
	
%.o: %.c
	$(CC) $(CFLAGS64) -c -o $@ $<

%.o: lib/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<

%.o: gui/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<
	
move:
	mv *.bin ../bin
clean:
	rm *.o