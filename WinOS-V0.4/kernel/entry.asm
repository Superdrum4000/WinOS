; =====================================================================
; Point d'entrée du kernel
;
; Le bootloader saute toujours à 0x8000. Avant, il fallait que
; kernel_main soit par chance la première fonction du binaire.
; Maintenant, le linker place la section ".text.entry" en premier :
; _start est donc TOUJOURS à 0x8000, quel que soit l'ordre des fichiers.
; =====================================================================

bits 32

section .text.entry

global _start
extern kernel_main
extern __bss_start
extern __bss_end

_start:
    ; La section .bss (variables globales non initialisées ou à 0)
    ; n'est pas stockée dans kernel.bin : on doit la remplir de zéros.
    mov edi, __bss_start
    mov ecx, __bss_end
    sub ecx, edi
    xor eax, eax
    cld
    rep stosb

    call kernel_main

    ; Si kernel_main revient un jour, on arrête proprement le CPU.
.hang:
    cli
    hlt
    jmp .hang
