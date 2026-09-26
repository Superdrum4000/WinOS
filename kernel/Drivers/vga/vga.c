#include "vga.h"

static int cursor_x = 0;
static int cursor_y = 0;


// scroll when 25 lignes is reached
static void scroll(void)
{
    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    for (int y = 1; y < 24; y++)
    {
        for (int x = 0; x < 80; x++)
        {
            int source = (y * 80 + x) * 2;
            int destination = ((y - 1) * 80 + x) * 2;

            vga[destination] = vga[source];
            vga[destination + 1] = vga[source + 1];
        }
    }

    for (int x = 0; x < 80; x++)
    {
        int offset = ((23 * 80) + x) * 2;

        vga[offset] = ' ';
        vga[offset + 1] = 0x10;
    }

    cursor_y = 23;
}

//print one char at defined coord.
void put_char_at(int x, int y, char c)
{
    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    int position = y * 80 + x;
    int offset = position * 2;

    vga[offset] = c;
    vga[offset + 1] = 0x1F;

    cursor_x = x + 1;
    cursor_y = y;

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= 24)
    {
        scroll();
    }
}

//print one char after the last one placed
void put_char(char c)
{
    erase_cursor();

    put_char_at(cursor_x, cursor_y, c);

    draw_cursor();
}

// print a hole sentence with spaces
void print(const char *text)
{
    while (*text != '\0')
    {
        if (*text == '\n')
        {
            new_line();
        }
        else
        {
            put_char(*text);
        }

        text++;
    }
}

// print only a c char frm keyboard (from what I understood)
void print_hex(unsigned char value)
{
    const char *hex = "0123456789ABCDEF";

    put_char(hex[(value >> 4) & 0x0F]);
    put_char(hex[value & 0x0F]);
}

// go to the next line
void new_line(void)
{
    erase_cursor();

    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= 24)
    {
        scroll();
    }

    draw_cursor();
}

// do a backspace
void backspace(void)
{
    erase_cursor();

    if (cursor_x > 0)
    {
        cursor_x--;
    }
    else if (cursor_y > 0)
    {
        cursor_y--;
        cursor_x = 79;
    }
    else
    {
        draw_cursor();
        return;
    }

    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    int position = cursor_y * 80 + cursor_x;
    int offset = position * 2;

    vga[offset] = ' ';
    vga[offset + 1] = 0x1F;

    draw_cursor();
}

// draw the cursor
void draw_cursor(void)
{
    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    int position = cursor_y * 80 + cursor_x;
    int offset = position * 2;

    vga[offset] = '_';
    vga[offset + 1] = 0x1F;
}

// erase the cursor (obviously)
void erase_cursor(void)
{
    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    int position = cursor_y * 80 + cursor_x;
    int offset = position * 2;

    vga[offset] = ' ';
    vga[offset + 1] = 0x10;
}