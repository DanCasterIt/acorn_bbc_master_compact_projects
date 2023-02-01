#include <stdio.h>
#include <stdint.h>

int main()    {
    uint16_t *ptr;
    for(ptr = (uint16_t *)0x7C00; ptr < (uint16_t *)0x7FE7; ((uint8_t *)ptr)++)   {
        *ptr = 'A' + (((uint16_t)ptr - 24) % 26);
    }
    return 0;
}
