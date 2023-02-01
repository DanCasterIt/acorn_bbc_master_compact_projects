#include <stdio.h>
#include <stdint.h>

uint8_t letter = 'D';

int main(void)	{
	__asm__ ("LDA %v", letter);
	//__asm__ ("LDA #$41");    //base 16 (hex) -> A
	//__asm__ ("LDA #65");     //base 10 -> A
	__asm__ ("JSR $FFEE");
	__asm__ ("JSR $FFE7");
	return 0;
}
