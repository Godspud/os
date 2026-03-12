#include "string.h"

int strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

void memset(void *ptr, int value, int n)
{
    unsigned char *p = (unsigned char *)ptr;
    while (n--)
    {
        *p++ = (unsigned char)value;
    }
}

void memcpy(void *dest, const void *src, int n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (n--)
    {
        *d++ = *s++;
    }
}
