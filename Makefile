CC = i386-elf-gcc
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy

CFLAGS = -ffreestanding -fno-stack-protector -Wall -Wextra
LDFLAGS = -T linker/linker.ld

all: clean build/winos.img

build/winos.img: boot/boot.bin build/kernel/kernel.bin
	mkdir -p build
	cat boot/boot.bin build/kernel/kernel.bin > build/winos.img
	
boot/boot.bin: boot/boot.asm build/kernel/kernel.bin
	KERNEL_SIZE=$$(wc -c < build/kernel/kernel.bin); \
	KERNEL_SECTORS=$$((($$KERNEL_SIZE + 511) / 512)); \
	nasm -f bin -dKERNEL_SECTORS=$$KERNEL_SECTORS boot/boot.asm -o boot/boot.bin

build/kernel/kernel.bin: build/kernel/kernel.elf
	$(OBJCOPY) -O binary build/kernel/kernel.elf build/kernel/kernel.bin

build/kernel/kernel.elf: build/kernel/kernel.o build/kernel/Drivers/vga/vga.o build/kernel/Drivers/keyboard/keyboard.o
	mkdir -p build/kernel
	$(LD) $(LDFLAGS) -o build/kernel/kernel.elf build/kernel/kernel.o build/kernel/Drivers/vga/vga.o build/kernel/Drivers/keyboard/keyboard.o

build/kernel/kernel.o: kernel/core/kernel.c kernel/io/io.h
	mkdir -p build/kernel
	$(CC) $(CFLAGS) -I kernel/io -I kernel/Drivers/vga -I kernel/Drivers/keyboard -c kernel/core/kernel.c -o build/kernel/kernel.o

build/kernel/Drivers/vga/vga.o: kernel/Drivers/vga/vga.c kernel/Drivers/vga/vga.h kernel/io/io.h
	mkdir -p build/kernel/Drivers/vga
	$(CC) $(CFLAGS) -I kernel/io -I kernel/Drivers/vga -c kernel/Drivers/vga/vga.c -o build/kernel/Drivers/vga/vga.o

build/kernel/Drivers/keyboard/keyboard.o: kernel/Drivers/keyboard/keyboard.c kernel/Drivers/keyboard/keyboard.h kernel/io/io.h
	mkdir -p build/kernel/Drivers/keyboard
	$(CC) $(CFLAGS) -I kernel/io -I kernel/Drivers/keyboard -c kernel/Drivers/keyboard/keyboard.c -o build/kernel/Drivers/keyboard/keyboard.o

clean:
	rm -rf build
	rm -f boot/boot.bin
	rm -f winos.img