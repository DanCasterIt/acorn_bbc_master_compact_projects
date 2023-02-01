#include <stdio.h>
#include <stdint.h>

uint8_t byte = 0;

int main(void)	{
	__asm__ ("LDA #54");
	__asm__ ("STA %v",byte);
	printf("byte = %u\n", byte);
	return 0;
}