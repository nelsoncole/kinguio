AS      = as
LD      = ld
CC      = gcc
AR      = ar
MAKE    = make
NASM    = nasm

.PHONY: all stage0 stage1 stage2 kernel mout_vhd clean test test2 fs vbox_install sdb_install push
all: stage0 stage1 stage2 kernel fs mout_vhd #test2 clean
stage0:
	$(MAKE) -C stage0/
stage1:
	$(MAKE) -C stage1/
stage2:
	$(MAKE) -C stage2/
kernel:
	$(MAKE) -C kernel/
	
mout_vhd:
	dd if=/dev/zero of=disk.vhd bs=512 count=8192 conv=noerror,sync
	./fs -f disk.vhd
	./fs -g bin/stage1.bin disk.vhd
	./fs -g bin/stage2.bin disk.vhd
	./fs -g bin/kernel.bin disk.vhd
	./fs -g README.md disk.vhd

clean:
	rm bin/*.bin
	rm fs
	rm disk.vhd

test:
	qemu-system-x86_64 -m 128 -drive file=disk.vhd,format=raw,bus=0
test2:
	qemu-system-x86_64 -m 128 -trace enable=usb* -device usb-ehci,id=ehci -drive if=none,id=usbstick,file=disk.vhd -device usb-storage,bus=ehci.0,drive=usbstick
	
fs:
	gcc -Wall -C fs.c -o fs
	
	
vbox_install:
	./fs -f "/home/nelson/VirtualBox VMs/Kinguio/"*.vhd
	./fs -g bin/stage1.bin "/home/nelson/VirtualBox VMs/Kinguio/"*.vhd
	./fs -g bin/stage2.bin "/home/nelson/VirtualBox VMs/Kinguio/"*.vhd
	./fs -g bin/kernel.bin "/home/nelson/VirtualBox VMs/Kinguio/"*.vhd
	./fs -g README.md "/home/nelson/VirtualBox VMs/Kinguio/"*.vhd
	
sdb_install:
	sudo ./fs -f /dev/sdb
	sudo ./fs -g bin/stage1.bin /dev/sdb
	sudo ./fs -g bin/stage2.bin /dev/sdb
	sudo ./fs -g bin/kernel.bin /dev/sdb
	sudo ./fs -g README.md /dev/sdb
	
push:
	git add ./
	git commit -m "commit++"	
	git push -u origin main
	
