#include "keyboard.h"
#include "io.h"

/* ---------------------------------------------------------------------
 * Special scancodes (set 1)
 * ------------------------------------------------------------------ */
#define SC_EXTENDED      0xE0   // next scancode is an "extended" key
#define SC_LEFT_SHIFT    0x2A
#define SC_RIGHT_SHIFT   0x36
#define SC_ALT           0x38   // with 0xE0 prefix = AltGr
#define SC_CAPS_LOCK     0x3A
#define SC_ENTER         0x1C
#define SC_KP_SLASH      0x35   // with 0xE0 prefix = '/' of the numpad
#define SC_RELEASED      0x80   // bit set when a key is released (break code)

/* Code page 437 values for the non-ASCII chars of the Belgian keyboard */
#define CP_E_ACUTE   0x82   // é
#define CP_E_GRAVE   0x8A   // è
#define CP_C_CEDIL   0x87   // ç
#define CP_A_GRAVE   0x85   // à
#define CP_U_GRAVE   0x97   // ù
#define CP_SECTION   0x15   // §
#define CP_SQUARE    0xFD   // ²
#define CP_MICRO     0xE6   // µ
#define CP_DEGREE    0xF8   // °
#define CP_POUND     0x9C   // £

/* ---------------------------------------------------------------------
 * Belgian AZERTY layout
 * index = scancode, value = char (0 = nothing to print)
 * ------------------------------------------------------------------ */

// no modifier
static const unsigned char map_normal[128] =
{
    [0x29] = CP_SQUARE,
    [0x02] = '&', [0x03] = CP_E_ACUTE, [0x04] = '"', [0x05] = '\'',
    [0x06] = '(', [0x07] = CP_SECTION, [0x08] = CP_E_GRAVE, [0x09] = '!',
    [0x0A] = CP_C_CEDIL, [0x0B] = CP_A_GRAVE, [0x0C] = ')', [0x0D] = '-',
    [0x0E] = '\b',

    [0x0F] = ' ',   // Tab (simple space for now)
    [0x10] = 'a', [0x11] = 'z', [0x12] = 'e', [0x13] = 'r', [0x14] = 't',
    [0x15] = 'y', [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p',
    [0x1A] = '^', [0x1B] = '$',
    [0x1C] = '\n',

    [0x1E] = 'q', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g',
    [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l', [0x27] = 'm',
    [0x28] = CP_U_GRAVE, [0x2B] = CP_MICRO,

    [0x56] = '<',
    [0x2C] = 'w', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b',
    [0x31] = 'n', [0x32] = ',', [0x33] = ';', [0x34] = ':', [0x35] = '=',

    [0x39] = ' ',

    // numpad (Num Lock considered always ON)
    [0x37] = '*', [0x4A] = '-', [0x4E] = '+',
    [0x47] = '7', [0x48] = '8', [0x49] = '9',
    [0x4B] = '4', [0x4C] = '5', [0x4D] = '6',
    [0x4F] = '1', [0x50] = '2', [0x51] = '3',
    [0x52] = '0', [0x53] = '.',
};

// with Shift
static const unsigned char map_shift[128] =
{
    [0x29] = '3',   // ³ does not exist in code page 437
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5',
    [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0',
    [0x0C] = CP_DEGREE, [0x0D] = '_',
    [0x0E] = '\b',

    [0x0F] = ' ',
    [0x10] = 'A', [0x11] = 'Z', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T',
    [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P',
    [0x1A] = '"', [0x1B] = '*',
    [0x1C] = '\n',

    [0x1E] = 'Q', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G',
    [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L', [0x27] = 'M',
    [0x28] = '%', [0x2B] = CP_POUND,

    [0x56] = '>',
    [0x2C] = 'W', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
    [0x31] = 'N', [0x32] = '?', [0x33] = '.', [0x34] = '/', [0x35] = '+',

    [0x39] = ' ',

    [0x37] = '*', [0x4A] = '-', [0x4E] = '+',
    [0x47] = '7', [0x48] = '8', [0x49] = '9',
    [0x4B] = '4', [0x4C] = '5', [0x4D] = '6',
    [0x4F] = '1', [0x50] = '2', [0x51] = '3',
    [0x52] = '0', [0x53] = '.',
};

// with AltGr
static const unsigned char map_altgr[128] =
{
    [0x02] = '|', [0x03] = '@', [0x04] = '#', [0x07] = '^',
    [0x0A] = '{', [0x0B] = '}',
    [0x1A] = '[', [0x1B] = ']',
    [0x2B] = '`', [0x56] = '\\', [0x35] = '~',
};

/* ---------------------------------------------------------------------
 * Keyboard state
 * ------------------------------------------------------------------ */
static int shift_pressed = 0;
static int altgr_pressed = 0;
static int caps_lock = 0;
static int extended = 0;        // 1 if the previous scancode was 0xE0

int keyboard_has_data(void)
{
    return inb(0x64) & 1;
}

unsigned char keyboard_read(void)
{
    while (!keyboard_has_data())
    {
    }

    return inb(0x60);
}

static int is_letter(unsigned char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char keyboard_translate(unsigned char scancode)
{
    // 0xE0 = the real scancode comes next
    if (scancode == SC_EXTENDED)
    {
        extended = 1;
        return 0;
    }

    int was_extended = extended;
    extended = 0;

    int released = scancode & SC_RELEASED;
    unsigned char key = scancode & 0x7F;

    // --- modifier keys: we only remember their state ---
    if (key == SC_LEFT_SHIFT || key == SC_RIGHT_SHIFT)
    {
        // E0 2A / E0 AA are "fake shifts" sent with some keys: ignore them
        if (!was_extended)
        {
            shift_pressed = !released;
        }
        return 0;
    }

    if (key == SC_ALT && was_extended)
    {
        altgr_pressed = !released;
        return 0;
    }

    // we don't care about the other released keys
    if (released)
    {
        return 0;
    }

    if (key == SC_CAPS_LOCK)
    {
        caps_lock = !caps_lock;
        return 0;
    }

    // --- extended keys (arrows, numpad Enter and '/', ...) ---
    if (was_extended)
    {
        if (key == SC_ENTER)
        {
            return '\n';
        }

        if (key == SC_KP_SLASH)
        {
            return '/';
        }

        return 0;   // arrows, Home, End... not handled yet
    }

    // --- normal keys ---
    if (altgr_pressed)
    {
        return (char)map_altgr[key];
    }

    unsigned char c = shift_pressed ? map_shift[key] : map_normal[key];

    // Caps Lock inverts upper/lower case, only for letters
    if (caps_lock && is_letter(c))
    {
        c = shift_pressed ? map_normal[key] : map_shift[key];
    }

    return (char)c;
}

// wait until a key that produces a char is pressed
char keyboard_getchar(void)
{
    for (;;)
    {
        char c = keyboard_translate(keyboard_read());

        if (c != 0)
        {
            return c;
        }
    }
}
