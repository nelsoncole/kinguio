ASFLAGS64  :=-f elf64 -O0
CFLAGS64 :=-Wall -O0 -m64 -std=c11 -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -nostdlib -nostdinc -I ./include
LDFLAGS64  :=-m elf_x86_64 -Map kernel.map -T link.ld
AS=nasm
CC=gcc
LD=ld

DRIVER = pci.o  i965.o cursor.o acpi.o hpet.o lpc.o pit.o ps2.o mouse.o keyboard.o
LIB = string.o vsprintf.o stdio.o stdlib.o
objs= setup.o main.o gui.o io.o font8x16.o gdt.o idt.o vetor.o exception.o paging.o mm.o cpuid.o msr.o lvt.o irq.o apic.o sleep.o $(DRIVER) $(LIB)
.PHONY: kernel.bin
	
kernel.bin: $(objs)
	$(LD) $(LDFLAGS64) -o $@ $^
	mv *.bin ../bin
	rm *.o

%.o: %.asm
	$(AS) $(ASFLAGS64) $< -o $@
	
%.o: %.c
	$(CC) $(CFLAGS64) -c -o $@ $<

%.o: lib/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<

%.o: gui/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<

%.o: driver/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<
		
%.o: driver/intel/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<
	
%.o: driver/acpi/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<

%.o: driver/pit/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<
	
%.o: driver/ps2/%.c
	$(CC) $(CFLAGS64) -c -o $@ $<
	
