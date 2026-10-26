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

static __attribute__((always_inline)) inline unsigned char inb(
    unsigned short port
)
{
    unsigned char value;

    asm volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
        : "memory"
    );

    return value;
}

#endif