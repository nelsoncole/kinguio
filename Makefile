ASFLAGS  :=-f elf32 -O0
CFLAGS32 :=-Wall -O0 -m32 -ffreestanding -nostdlib -nostdinc -I ./stage2/include
LDFLAGS  :=-m elf_i386 -Map Kernel.map -T stage2/link.ld
AS=nasm
CC=gcc
LD=ld

objs= stage2.o main.o data.o stdlib.o string.o gui.o font8x16.o stdio.o pci.o ata.o ahci.o storage.o

.PHONY: stage0.bin stage1.bin stage2.bin move clean test fs

stage0.bin: stage0/stage0.asm
	nasm -f bin -o $@ $<
	
stage1.bin: stage1/stage1.asm
	nasm -i stage1 -f bin -o $@ $<
	
stage2.bin: $(objs)
	$(LD) $(LDFLAGS) -o $@ $^
#stage2
%.o: stage2/%.asm
	$(AS) $(ASFLAGS) $< -o $@
	
%.o: stage2/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<

%.o: stage2/lib/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
	
%.o: stage2/gui/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
	
%.o: stage2/drive/%.c
	$(CC) $(CFLAGS32) -c -o $@ $<
	
	
move:
	mv *.bin bin
clean:
	rm *.o
	rm bin/*.bin
	rm fs
	rm disk.vhd

test:
	dd if=/dev/zero of=disk.vhd bs=512 count=8192 conv=noerror,sync
	./fs -f disk.vhd
	./fs -g bin/stage1.bin disk.vhd
	./fs -g bin/stage2.bin disk.vhd
	qemu-system-x86_64 -m 64 -drive file=disk.vhd,format=raw,bus=0
	
fs:
	gcc -Wall -C fs.c -o fs
	
#git add ./
#git commit -m "commit++"	
#git push -u origin master
	
