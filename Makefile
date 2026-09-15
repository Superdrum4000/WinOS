CC = i386-elf-gcc
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy

CFLAGS = -ffreestanding -fno-stack-protector -Wall -Wextra
LDFLAGS = -T linker/linker.ld

all: clean winos.img

winos.img: boot/boot.bin kernel/kernel.bin
	cat boot/boot.bin kernel/kernel.bin > winos.img

boot/boot.bin: boot/boot.asm
	nasm -f bin boot/boot.asm -o boot/boot.bin

kernel/kernel.bin: kernel/kernel.elf
	$(OBJCOPY) -O binary kernel/kernel.elf kernel/kernel.bin

kernel/kernel.elf: kernel/kernel.o
	$(LD) $(LDFLAGS) -o kernel/kernel.elf kernel/kernel.o

kernel/kernel.o: kernel/core/kernel.c kernel/io/io.h
	$(CC) $(CFLAGS) -I kernel/io -c kernel/core/kernel.c -o kernel/kernel.o

clean:
	rm -f boot/boot.bin
	rm -f kernel/kernel.o
	rm -f kernel/kernel.elf
	rm -f kernel/kernel.bin
	rm -f winos.img