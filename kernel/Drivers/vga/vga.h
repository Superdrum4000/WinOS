#ifndef VGA_H
#define VGA_H

void put_char_at(int x, int y, char c);
void put_char(char c);
void print(const char *text);
void print_at(int x, int y, const char *text);
void print_hex(unsigned char value);
void new_line(void);
void backspace(void);
void draw_cursor(void);
void erase_cursor(void);

#endif