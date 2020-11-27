.PHONY: stage0.bin stage1.bin stage2.bin move clean test fs

stage0.bin: stage0/stage0.asm
	nasm -f bin -o $@ $<
	
stage1.bin: stage1/stage1.asm
	nasm -i stage1 -f bin -o $@ $<
	
stage2.bin: stage2/stage2.asm
	nasm -i stage2 -f bin -o $@ $<
	
	
move:
	mv *.bin bin
clean:
	rm bin/*.bin
	rm fs
	rm disk.vhd

test:
	dd if=/dev/zero of=disk.vhd bs=512 count=8192 conv=noerror,sync
	./fs -f disk.vhd
	./fs -g bin/stage1.bin disk.vhd
	./fs -g bin/stage2.bin disk.vhd
	qemu-system-x86_64 -m 64 -drive file=disk.vhd,format=raw
	
fs:
	gcc -Wall -C fs.c -o fs
	
#git add ./
#git commit -m "commit++"	
#git push -u origin master
	
