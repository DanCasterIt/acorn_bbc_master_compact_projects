#include <stdio.h>
#include <stdint.h>

int main(void)	{
	uint8_t byte = 0;
	
	//__asm__ ("LDA #54");
	//__asm__ ("STA %o", byte);
	//
	//__asm__ ("ldy #%o", byte);
	//__asm__ ("lda (sp),y");
	//https://comp.sys.oric.narkive.com/jFNJTc5K/help-with-cc65
	//
	//__asm__ ("LDA #54");
	//__asm__ ("STA (sp),#%o", byte);
	
	__asm__ ("LDA #54");
	__asm__ ("LDY #%o", byte);
	__asm__ ("STA (sp),y");
	
	printf("byte = %u\n", byte);
	return 0;
}