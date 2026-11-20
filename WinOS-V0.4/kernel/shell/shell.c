#include "shell.h"
#include "kernel.h"
#include "vga.h"
#include "keyboard.h"
#include "string.h"
#include "io.h"
#include "memory/heap.h"
#include "fs/fs.h"         

/* =====================================================================
 * Commands
 *
 * To add a new command:
 *   1. write a function   static void cmd_xxx(const char *args)
 *   2. add one line in the "commands" table below
 * "args" is the text written after the command name ("" if nothing).
 * ===================================================================== */

static void cmd_help(const char *args);
static void cmd_clear(const char *args);
static void cmd_echo(const char *args);
static void cmd_version(const char *args);
static void cmd_reboot(const char *args);
static void cmd_mem(const char *args);
static void cmd_memtest(const char *args);
static void cmd_write(const char *args);
static void cmd_cat(const char *args);
static void cmd_rm(const char *args);
static void cmd_touch(const char *args);
static void cmd_ls(const char *args);

struct command
{
    const char *name;
    const char *description;
    void (*function)(const char *args);
};

static const struct command commands[] =
{
    { "help",    "show this list",              cmd_help    },
    { "clear",   "clear the screen",            cmd_clear   },
    { "echo",    "print a text (echo hello)",   cmd_echo    },
    { "version", "show the WinOS version",      cmd_version },
    { "reboot",  "restart the computer",        cmd_reboot  },
    { "mem",     "show the memory usage",       cmd_mem     },
    { "memtest", "test kmalloc and kfree",      cmd_memtest },
    { "write",   "write a text in a file",      cmd_write   },
    { "cat",     "show a file",                 cmd_cat     },
    { "rm",      "delete a file",               cmd_rm      },
    { "touch",   "create an empty file",        cmd_touch   },
    { "ls",      "list the files",              cmd_ls      },

};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

static void cmd_help(const char *args)
{
    (void)args;

    print("Available commands:\n");

    for (unsigned int i = 0; i < COMMAND_COUNT; i++)
    {
        set_color(VGA_COLOR_INFO);
        print("  ");
        print(commands[i].name);
        set_color(VGA_COLOR_DEFAULT);

        // align the descriptions
        for (int space = strlen(commands[i].name); space < 10; space++)
        {
            put_char(' ');
        }

        print(commands[i].description);
        new_line();
    }
}

static void cmd_clear(const char *args)
{
    (void)args;
    clear_screen();
}

static void cmd_echo(const char *args)
{
    print(args);
    new_line();
}

static void cmd_version(const char *args)
{
    (void)args;
    print(WINOS_NAME " version " WINOS_VERSION "\n");
}

static void cmd_reboot(const char *args)
{
    (void)args;
    print("Rebooting...\n");

    // ask the PS/2 controller to pulse the CPU reset line
    while (inb(0x64) & 0x02)
    {
    }
    outb(0x64, 0xFE);

    // if it did not work, just stop here
    for (;;)
    {
        asm volatile ("cli; hlt");
    }
}

static void cmd_mem(const char *args)
{
    (void)args;

    unsigned int used, free, blocks;      // trois boîtes vides
    heap_stats(&used, &free, &blocks);    // & = "l'adresse de" chaque boîte

    print("Heap: ");
    print_dec(used);
    print(" bytes used, ");
    print_dec(free);
    print(" bytes free, ");
    print_dec(blocks);
    print(" block(s)\n");
}

static void cmd_memtest(const char *args)
{
    (void)args;

    print("1. start         ");
    cmd_mem(0);

    char *a = kmalloc(100);
    print("2. kmalloc(100)  ");
    cmd_mem(0);

    char *b = kmalloc(50);
    print("3. kmalloc(50)   ");
    cmd_mem(0);

    kfree(a);
    print("4. kfree(a)      ");
    cmd_mem(0);

    kfree(b);
    print("5. kfree(b)      ");
    cmd_mem(0);

    print("6. kfree(b) again -> ");
    print_dec(kfree(b));
    new_line();

    print("7. kfree(0x50)    -> ");
    print_dec(kfree((void *)0x50));
    new_line();
}

static void cmd_touch(const char *args)
{
    int result = fs_create(args);
    if (result == 1)      print("Invalid name\n");
    else if (result == 2) print("This file already exists\n");
    else if (result == 3) print("Out of memory\n");
}

static void cmd_ls(const char *args)
{
    (void)args;
    struct file *f = fs_first();
    if (f == 0) { print("No file\n"); return; }

    while (f != 0)
    {
        print(f->name);
        print("  (");
        print_dec(f->size);
        print(" bytes)\n");
        f = f->next;
    }
}

static void cmd_write(const char *args)
{
    // séparer le nom du texte : "notes hello world" -> "notes" + "hello world"
    char name[FS_NAME_MAX];
    unsigned int i = 0;

    while (args[i] != ' ' && args[i] != '\0')
    {
        if (i >= FS_NAME_MAX - 1) { print("Invalid name\n"); return; }
        name[i] = args[i];
        i++;
    }
    name[i] = '\0';

    const char *text = args + i;        // sauter les espaces avant le texte
    while (*text == ' ') text++;

    int result = fs_write(name, text);
    if (result == 1)      print("File not found (use touch first)\n");
    else if (result == 3) print("Out of memory\n");
}

static void cmd_cat(const char *args)
{
    struct file *f = fs_find(args);
    if (f == 0) { print("File not found\n"); return; }

    // le contenu n'a pas de '\0' à la fin : on affiche "size" caractères
    for (unsigned int i = 0; i < f->size; i++)
        put_char(f->data[i]);
    new_line();
}

static void cmd_rm(const char *args)
{
    if (fs_delete(args) == 1) print("File not found\n");
}

/* =====================================================================
 * Shell
 * ===================================================================== */

static void print_prompt(void)
{
    set_color(VGA_COLOR_PROMPT);
    print("WinOS> ");
    set_color(VGA_COLOR_DEFAULT);
}

// read one line typed by the user (until Enter) in "buffer"
static void read_line(char *buffer, int size)
{
    int length = 0;
    buffer[0] = '\0';

    for (;;)
    {
        char c = keyboard_getchar();

        if (c == '\n')
        {
            new_line();
            return;
        }

        if (c == '\b')
        {
            // we can't erase the prompt: only what was typed
            if (length > 0)
            {
                length--;
                buffer[length] = '\0';
                backspace();
            }
        }
        else if (length < size - 1)
        {
            buffer[length] = c;
            length++;
            buffer[length] = '\0';

            put_char(c);
        }
    }
}

// cut the line in two parts: the command name and its arguments
// ex: "  echo  hello world" -> name = "echo", args = "hello world"
static void execute(char *line)
{
    // skip the spaces at the beginning
    while (*line == ' ')
    {
        line++;
    }

    // empty line: nothing to do
    if (*line == '\0')
    {
        return;
    }

    // find the end of the command name
    char *args = line;
    while (*args != ' ' && *args != '\0')
    {
        args++;
    }

    // cut the string after the name, and skip the spaces before the args
    if (*args == ' ')
    {
        *args = '\0';
        args++;

        while (*args == ' ')
        {
            args++;
        }
    }

    for (unsigned int i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(line, commands[i].name) == 0)
        {
            commands[i].function(args);
            return;
        }
    }

    set_color(VGA_COLOR_ERROR);
    print("Unknown command: ");
    print(line);
    set_color(VGA_COLOR_DEFAULT);
    print(" (type 'help')\n");
}

void shell_run(void)
{
    static char buffer[SHELL_BUFFER_SIZE];

    print("Hello WinOS ! Type 'help' to see the commands.\n");

    for (;;)
    {
        print_prompt();
        read_line(buffer, SHELL_BUFFER_SIZE);
        execute(buffer);
    }
}
