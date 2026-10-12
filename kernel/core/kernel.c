#include "vga.h"
#include "io.h"
#include "keyboard.h"

#define INPUT_BUFFER_SIZE 64

static char input_buffer[INPUT_BUFFER_SIZE];
static int input_length = 0;

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
    print("WinOS> ");
    
    for (;;)
    {  
        unsigned char scancode = keyboard_read();
        char c = keyboard_translate(scancode);

        if (c == '\n')
        {
            input_buffer[input_length] = '\0';

            new_line();
            print("Buffer recieved: ");
            print(input_buffer);
            new_line();

            input_length = 0;
            input_buffer[0] = '\0';

            print("WinOS> ");
        }
        else if (c == '\b')
        {
            if (input_length > 0)
            {
                input_length--;
                input_buffer[input_length] = '\0';
                backspace();
            }
        }
        else if (c != 0 && input_length < INPUT_BUFFER_SIZE - 1)
        {
            input_buffer[input_length] = c;
            input_length++;

           input_buffer[input_length] = '\0';

            put_char(c);
        }
    }
}