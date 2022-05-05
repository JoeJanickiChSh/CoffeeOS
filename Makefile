all:
	nasm -f elf32 kernel_start.asm -o ks.o
	gcc -m32 -c kernel.c -o kernel.o
	ld -melf_i386 -T linker.ld -o iso/boot/kernel.bin ks.o kernel.o 
	grub-mkrescue -o os.iso iso
clean:
	rm *.o
	rm *.iso
	rm iso/boot/kernel.bin
