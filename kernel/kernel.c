#include "io.h"

void kernel_main(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    for (int i = 0; i < 2000; i++)
    {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x10;
    }

    for (;;)
    {
    }
}