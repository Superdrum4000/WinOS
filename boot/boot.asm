bits 16
org 0x7C00

%ifndef KERNEL_SECTORS
    %define KERNEL_SECTORS 3
%endif

mov [boot_drive], dl

mov si, message

print_loop:
    mov al, [si]
    cmp al, 0
    je load_kernel

    mov ah, 0x0E
    int 0x10

    inc si
    jmp print_loop

load_kernel:
    mov ax, 0x0000
    mov es, ax

    mov bx, 0x8000

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [boot_drive]

    int 0x13
    jc disk_error

    cli

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp 0x08:protected_mode

bits 32

protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    mov eax, 0x8000
    jmp eax

bits 16

disk_error:
    mov si, error_message

error_loop:
    mov al, [si]
    cmp al, 0
    je error_halt

    mov ah, 0x0E
    int 0x10

    inc si
    jmp error_loop

error_halt:
    cli
    hlt
    jmp error_halt

message       db "Loading kernel...", 0
error_message db "Disk error!", 0
boot_drive    db 0

gdt:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF

gdt_descriptor:
    dw 0x17
    dd gdt

times 510-($-$$) db 0
dw 0xAA55