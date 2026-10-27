# =====================================================================
# WinOS Makefile
#
#   make         -> compile tout et crée build/winos.img
#   make run     -> compile puis lance WinOS dans QEMU
#   make clean   -> supprime le dossier build/
#
# Chaque fichier .c trouvé dans kernel/ est compilé automatiquement :
# plus besoin de modifier le Makefile quand on ajoute un fichier.
# =====================================================================

CC      = i386-elf-gcc
LD      = i386-elf-ld
OBJCOPY = i386-elf-objcopy
ASM     = nasm

BUILD   = build

# tous les dossiers de kernel/ sont des dossiers d'include (-I)
INCLUDES = $(addprefix -I ,$(shell find kernel -type d))

CFLAGS  = -ffreestanding -fno-stack-protector -fno-asynchronous-unwind-tables \
          -fno-builtin -nostdlib -O1 -Wall -Wextra $(INCLUDES)
LDFLAGS = -T linker/linker.ld

C_SOURCES = $(shell find kernel -name '*.c')
OBJECTS   = $(BUILD)/kernel/entry.o $(patsubst %.c,$(BUILD)/%.o,$(C_SOURCES))
HEADERS   = $(shell find kernel -name '*.h')

.PHONY: all run clean

all: $(BUILD)/winos.img

run: $(BUILD)/winos.img
	qemu-system-i386 -drive format=raw,file=$(BUILD)/winos.img

# image disque = bootloader (secteur 0) + kernel (secteurs 1, 2, ...)
$(BUILD)/winos.img: $(BUILD)/boot.bin $(BUILD)/kernel/kernel.pad.bin
	cat $^ > $@

# le bootloader a besoin de connaître la taille du kernel en secteurs
$(BUILD)/boot.bin: boot/boot.asm $(BUILD)/kernel/kernel.pad.bin
	KERNEL_SECTORS=$$(( $$(wc -c < $(BUILD)/kernel/kernel.pad.bin) / 512 )); \
	$(ASM) -f bin -dKERNEL_SECTORS=$$KERNEL_SECTORS boot/boot.asm -o $@

# kernel complété avec des zéros jusqu'à un multiple de 512 octets
$(BUILD)/kernel/kernel.pad.bin: $(BUILD)/kernel/kernel.bin
	dd if=$< of=$@ bs=512 conv=sync status=none

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/kernel.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/kernel/kernel.elf: $(OBJECTS) linker/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILD)/kernel/entry.o: kernel/entry.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

$(BUILD)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
