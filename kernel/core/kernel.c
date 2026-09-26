#include "vga.h"
#include "io.h"
#include "keyboard.h"

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

    print("Kernel successfully started !\n");
    print("Initiallizing drivers...\n");
    print("VGA successfully started\n");
    print("All drivers successfully started !\n");
    print("Hello WinOS !\n");

    
    for (;;)
    {
        unsigned char scancode = keyboard_read();
        char c = keyboard_translate(scancode);

        if (c == '\n')
        {
            new_line();
        }
        else if (c == '\b')
        {
            backspace();
        }
        else if (c != 0)
        {
            put_char(c);
        }
    }
}