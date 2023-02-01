#include <stdio.h>
#include <stdint.h>

#define SHELA_RD 0x96	//0xFE00 to 0xFEFF
#define SHELA_WR 0x97	//0xFE00 to 0xFEFF

#define UDDRB    0x62   //0xFE62
#define UIORB    0x60   //0xFE60

#define ACCCON   0x34   //0xFE34

uint8_t page = 0;
uint8_t page_offset = 0;
uint8_t io_byte = 0;

void IO(void)	{
	__asm__ ("LDA %v", page);
	__asm__ ("LDX %v", page_offset);
	__asm__ ("LDY %v", io_byte);
	__asm__ ("JSR $FFF4");
	__asm__ ("STY %v", io_byte);
}

uint8_t Tosbyte(uint8_t A, uint8_t X, uint8_t Y)	{
	page = A;
	page_offset = X;
	io_byte = Y;
	IO();
	return io_byte;
}

uint8_t osbyte(uint8_t A, uint8_t X, uint8_t Y)	{
	__asm__ ("LDY #%o", A);
	__asm__ ("LDA (sp), Y");
	__asm__ ("PHA");
	
	__asm__ ("LDY #%o", X);
	__asm__ ("LDA (sp), Y");
	__asm__ ("PHA");
	
	__asm__ ("LDY #%o", Y);
	__asm__ ("LDA (sp), Y");
	__asm__ ("PHA");
	
	__asm__ ("PLA");
	__asm__ ("TAY");
	
	__asm__ ("PLA");
	__asm__ ("TAX");
	
	__asm__ ("PLA");
	
	__asm__ ("JSR $FFF4");
	
	__asm__ ("TYA");
	__asm__ ("LDY #%o", Y);
	__asm__ ("STA (sp), Y");
	
	return Y;
}

int main(void)	{
	uint8_t A = SHELA_RD, X = ACCCON, Y = 9;
	printf("A = %02X, X = %02X, Y = %02X\n", A, X, Y);
	Y = osbyte(A, X, Y);
	printf("A = %02X, X = %02X, Y = %02X\n", A, X, Y);
	A = SHELA_WR; Y = Y | 0x20;
	Y = osbyte(A, X, Y);
	printf("A = %02X, X = %02X, Y = %02X\n", A, X, Y);
	
	printf("--------\n");
	
	A = SHELA_RD, X = ACCCON, Y = 9;
	printf("A = %02X, X = %02X, Y = %02X\n", A, X, Y);
	Y = Tosbyte(A, X, Y);
	printf("A = %02X, X = %02X, Y = %02X\n", A, X, Y);
	A = SHELA_WR; Y = Y | 0x20;
	Y = Tosbyte(A, X, Y);
	printf("A = %02X, X = %02X, Y = %02X\n", A, X, Y);
	return 0;
}