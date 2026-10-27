#include "kernel.h"
#include "vga.h"
#include "shell.h"

void kernel_main(void)
{
    vga_init();

    print("Kernel successfully started !\n");
    print("Initializing drivers...\n");
    print("VGA successfully started\n");
    print("Keyboard ready (Belgian AZERTY)\n");
    print("All drivers successfully started !\n");
    new_line();

    shell_run();    // never returns
}
