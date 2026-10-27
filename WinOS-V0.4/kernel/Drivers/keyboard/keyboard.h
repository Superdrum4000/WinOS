#ifndef KEYBOARD_H
#define KEYBOARD_H

/* PS/2 keyboard driver (polling), Belgian AZERTY layout.
 *
 * Accented letters (é è à ç ù ...) are returned with their
 * code page 437 value, the character set of the VGA text mode,
 * so they are displayed correctly on screen. */

int keyboard_has_data(void);
unsigned char keyboard_read(void);
char keyboard_translate(unsigned char scancode);   // 0 = no char for this scancode
char keyboard_getchar(void);                       // waits for a real char

#endif
