#include <stdio.h>
#include <stdint.h>

int main(void)	{
	int val = 3;
	uint8_t a = ((uint16_t)&val);
	__asm__ ("LDX %o", a);
	return 0;
}
