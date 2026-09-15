#ifndef IO_H
#define IO_H

static __attribute__((always_inline)) inline void outb(
    unsigned short port,
    unsigned char value
)
{
    asm volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
        : "memory"
    );
}

#endif