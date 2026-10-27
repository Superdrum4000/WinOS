#ifndef VGA_H
#define VGA_H

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_LINES   24      // only 24 lines are used (more readable, personal choice)

// colors: high 4 bits = background, low 4 bits = text
#define VGA_COLOR_DEFAULT  0x1F     // white on blue
#define VGA_COLOR_PROMPT   0x1E     // yellow on blue
#define VGA_COLOR_ERROR    0x1C     // light red on blue
#define VGA_COLOR_INFO     0x1B     // light cyan on blue

void vga_init(void);
void clear_screen(void);
void set_color(unsigned char color);

void put_char_at(int x, int y, char c);     // does NOT move the cursor
void put_char(char c);                      // handles '\n'
void print(const char *text);
void print_at(int x, int y, const char *text);
void print_hex(unsigned char value);
void print_dec(unsigned int value);

void new_line(void);
void backspace(void);
void draw_cursor(void);
void erase_cursor(void);

#endif
