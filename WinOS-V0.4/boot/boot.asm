; =====================================================================
; WinOS bootloader (secteur de boot, 512 octets)
;
;  1. Le BIOS nous charge à 0x7C00 en mode réel 16 bits.
;  2. On charge le kernel depuis le disque à l'adresse 0x8000,
;     secteur par secteur, avec la lecture LBA étendue (int 0x13, AH=0x42).
;     -> plus de limite de ~62 secteurs comme avec l'ancienne lecture CHS.
;  3. On active la ligne A20 (utile plus tard pour la mémoire > 1 Mo).
;  4. On charge la GDT et on passe en mode protégé 32 bits.
;  5. On saute vers le kernel (entry.asm -> kernel_main).
; =====================================================================

bits 16
org 0x7C00

%ifndef KERNEL_SECTORS
    %define KERNEL_SECTORS 16
%endif

KERNEL_SEGMENT equ 0x0800        ; 0x0800:0x0000 = adresse physique 0x8000

start:
    ; On ne sait pas ce que le BIOS a mis dans les registres de segment :
    ; on les remet tous à zéro pour que "org 0x7C00" soit correct.
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00               ; petite pile juste sous le bootloader
    sti

    mov [boot_drive], dl         ; le BIOS donne le numéro du disque dans DL

    mov si, message
    call print_string

; ---------------------------------------------------------------------
; Chargement du kernel : 1 secteur à la fois, KERNEL_SECTORS fois
; ---------------------------------------------------------------------
load_kernel:
    mov cx, KERNEL_SECTORS

.next_sector:
    push cx

    mov si, dap                  ; DS:SI -> Disk Address Packet
    mov ah, 0x42                 ; lecture étendue (LBA)
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    pop cx

    add word [dap_segment], 0x20 ; +512 octets (0x20 paragraphes de 16 octets)
    inc dword [dap_lba]          ; secteur suivant

    loop .next_sector

; ---------------------------------------------------------------------
; Activation de A20 (méthode "fast A20" via le port 0x92)
; ---------------------------------------------------------------------
    in al, 0x92
    or al, 0x02
    and al, 0xFE                 ; ne surtout pas mettre le bit 0 (= reset)
    out 0x92, al

; ---------------------------------------------------------------------
; Passage en mode protégé
; ---------------------------------------------------------------------
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    jmp 0x08:protected_mode      ; far jump = recharge CS avec le segment code

; ---------------------------------------------------------------------
; Fonctions 16 bits
; ---------------------------------------------------------------------

; Affiche la chaîne terminée par 0 pointée par SI (BIOS teletype)
print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

disk_error:
    mov si, error_message
    call print_string
.halt:
    cli
    hlt
    jmp .halt

; ---------------------------------------------------------------------
; Code 32 bits
; ---------------------------------------------------------------------
bits 32

protected_mode:
    mov ax, 0x10                 ; segment de données de la GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000             ; pile du kernel

    jmp 0x08:0x8000              ; -> _start (kernel/entry.asm)

; ---------------------------------------------------------------------
; Données
; ---------------------------------------------------------------------
message       db "Loading kernel...", 13, 10, 0
error_message db "Disk error!", 0
boot_drive    db 0

; Disk Address Packet pour int 0x13 AH=0x42
align 4
dap:
    db 0x10                      ; taille du paquet (16 octets)
    db 0                         ; réservé
    dw 1                         ; nombre de secteurs à lire
    dw 0x0000                    ; offset de destination
dap_segment:
    dw KERNEL_SEGMENT            ; segment de destination
dap_lba:
    dq 1                         ; secteur LBA de départ (0 = ce bootloader)

; GDT : descripteur nul, code (0x08), données (0x10) — 0 à 4 Go, plat
gdt:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt

times 510-($-$$) db 0
dw 0xAA55
