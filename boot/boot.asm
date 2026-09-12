bits 16
org 0x7C00

mov si, message

boucle:
    mov al, [si]
    cmp al, 0
    je fin

    mov ah, 0x0E
    int 0x10

    inc si
    jmp boucle

fin:
    jmp $

message db "Hello WinOS!", 0

times 510-($-$$) db 0
dw 0xAA55