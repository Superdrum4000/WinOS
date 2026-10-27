#include "vga.h"
#include "io.h"

static volatile unsigned char *const vga = (volatile unsigned char *)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = VGA_COLOR_DEFAULT;

// write one char + color directly in VGA memory
static void write_cell(int x, int y, char c, unsigned char color)
{
    int offset = (y * VGA_WIDTH + x) * 2;

    vga[offset] = c;
    vga[offset + 1] = color;
}

// scroll when the last used line is reached
static void scroll(void)
{
    for (int y = 1; y < VGA_LINES; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            int source = (y * VGA_WIDTH + x) * 2;
            int destination = ((y - 1) * VGA_WIDTH + x) * 2;

            vga[destination] = vga[source];
            vga[destination + 1] = vga[source + 1];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++)
    {
        write_cell(x, VGA_LINES - 1, ' ', VGA_COLOR_DEFAULT);
    }

    cursor_y = VGA_LINES - 1;
}

// disable the hardware cursor and clear the screen
void vga_init(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    current_color = VGA_COLOR_DEFAULT;
    clear_screen();
}

// fill the whole screen (all 25 lines) with blue and put the cursor top left
void clear_screen(void)
{
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            write_cell(x, y, ' ', VGA_COLOR_DEFAULT);
        }
    }

    cursor_x = 0;
    cursor_y = 0;

    draw_cursor();
}

// change the color of the next printed chars
void set_color(unsigned char color)
{
    current_color = color;
}

// print one char at defined coord. (the cursor does not move)
void put_char_at(int x, int y, char c)
{
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
    {
        return;
    }

    write_cell(x, y, c, current_color);
}

// print one char after the last one placed
void put_char(char c)
{
    if (c == '\n')
    {
        new_line();
        return;
    }

    erase_cursor();

    write_cell(cursor_x, cursor_y, c, current_color);

    cursor_x++;

    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_LINES)
    {
        scroll();
    }

    draw_cursor();
}

// print a whole sentence
void print(const char *text)
{
    while (*text != '\0')
    {
        put_char(*text);
        text++;
    }
}

// print a sentence at defined coord. (the cursor does not move)
void print_at(int x, int y, const char *text)
{
    while (*text != '\0')
    {
        put_char_at(x, y, *text);

        x++;
        if (x >= VGA_WIDTH)
        {
            x = 0;
            y++;
        }

        text++;
    }
}

// print a byte in hexadecimal (ex: 0x1C -> "1C"), useful to debug scancodes
void print_hex(unsigned char value)
{
    const char *hex = "0123456789ABCDEF";

    put_char(hex[(value >> 4) & 0x0F]);
    put_char(hex[value & 0x0F]);
}

// print a number in decimal (ex: 42 -> "42")
void print_dec(unsigned int value)
{
    char digits[10];
    int count = 0;

    if (value == 0)
    {
        put_char('0');
        return;
    }

    // digits come out in reverse order (last digit first)
    while (value > 0)
    {
        digits[count] = '0' + (value % 10);
        value /= 10;
        count++;
    }

    while (count > 0)
    {
        count--;
        put_char(digits[count]);
    }
}

// go to the next line
void new_line(void)
{
    erase_cursor();

    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= VGA_LINES)
    {
        scroll();
    }

    draw_cursor();
}

// do a backspace (goes back to the previous line if needed)
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
        cursor_x = VGA_WIDTH - 1;
    }

    write_cell(cursor_x, cursor_y, ' ', VGA_COLOR_DEFAULT);

    draw_cursor();
}

// draw the cursor
void draw_cursor(void)
{
    write_cell(cursor_x, cursor_y, '_', VGA_COLOR_DEFAULT);
}

// erase the cursor (obviously)
void erase_cursor(void)
{
    write_cell(cursor_x, cursor_y, ' ', VGA_COLOR_DEFAULT);
}
