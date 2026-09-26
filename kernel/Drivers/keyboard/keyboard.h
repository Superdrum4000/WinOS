#ifndef KEYBOARD_H
#define KEYBOARD_H

int keyboard_has_data(void);
unsigned char keyboard_read(void);
char keyboard_translate(unsigned char scancode);

#endif