/* WinOS kernel linker script
 * Le kernel est chargé à 0x8000 par le bootloader.
 */

ENTRY(_start)

SECTIONS
{
    . = 0x8000;

    .text :
    {
        *(.text.entry)      /* _start (entry.asm) toujours en premier */
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)         /* chaînes de caractères constantes, tables... */
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note*)
    }
}
