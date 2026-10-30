#include "kernel.h"
#include "vga.h"
#include "shell.h"
#include "memory/heap.h"

void kernel_main(void)
{
    vga_init();
    heap_init();
    print("Heap Ready\n");

    print("Kernel successfully started !\n");
    print("Initializing drivers...\n");
    print("VGA successfully started\n");
    print("Keyboard ready (Belgian AZERTY)\n");
    print("All drivers successfully started !\n");
    new_line();

    shell_run();    // never returns
}
