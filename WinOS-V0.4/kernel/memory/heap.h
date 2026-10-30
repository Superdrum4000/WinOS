#ifndef HEAP_H
#define HEAP_H

#define HEAP_START  0x100000    // 1 Mo
#define HEAP_END    0x400000    // 4 Mo

void heap_init(void);
void *kmalloc(unsigned int size);
int kfree(void *ptr);
void heap_stats(unsigned int *used, unsigned int *free, unsigned int *blocks);

#endif // HEAP_H