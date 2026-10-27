#include "string.h"

// number of chars before the final '\0'
size_t strlen(const char *s)
{
    size_t length = 0;

    while (s[length] != '\0')
    {
        length++;
    }

    return length;
}

// 0 if both strings are equal, otherwise the difference of the first different char
int strcmp(const char *a, const char *b)
{
    while (*a != '\0' && *a == *b)
    {
        a++;
        b++;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

// same as strcmp but only compares the first n chars
int strncmp(const char *a, const char *b, size_t n)
{
    while (n > 0 && *a != '\0' && *a == *b)
    {
        a++;
        b++;
        n--;
    }

    if (n == 0)
    {
        return 0;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

// fill memory with one byte value
void *memset(void *dest, int value, size_t count)
{
    unsigned char *d = (unsigned char *)dest;

    while (count > 0)
    {
        *d = (unsigned char)value;
        d++;
        count--;
    }

    return dest;
}

// copy memory from src to dest
void *memcpy(void *dest, const void *src, size_t count)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    while (count > 0)
    {
        *d = *s;
        d++;
        s++;
        count--;
    }

    return dest;
}
