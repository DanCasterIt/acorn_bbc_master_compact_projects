#include <stdio.h>
#include <stdint.h>

#define FRED_RD 0x92	//0xFC00 to 0xFCFF
#define JIM_RD 0x94		//0xFD00 to 0xFDFF
#define SHELA_RD 0x96	//0xFE00 to 0xFEFF

#define FRED_RW 0x93	//0xFC00 to 0xFCFF
#define JIM_RW 0x95		//0xFD00 to 0xFDFF
#define SHELA_RW 0x97	//0xFE00 to 0xFEFF

uint8_t page = 0;
uint8_t page_offset = 0;
uint8_t in_byte = 0;
uint8_t out_byte = 0;

void IO_read(void)	{
	__asm__ ("LDA %v", page);
	__asm__ ("LDX %v", page_offset);
	__asm__ ("LDY %v", in_byte);
	__asm__ ("JSR $FFF4");
	__asm__ ("STY %v",out_byte);
}

int main(void)	{
	int i;
	label:
	printf("Please, select page to dump:\n");
	printf("FRED = 0\t(0xFC00 to 0xFCFF)\n");
	printf("JIM = 1\t\t(0xFD00 to 0xFDFF)\n");
	printf("SHELA = 2\t(0xFE00 to 0xFEFF)\n");
	printf("Selection: ");
	scanf("%u", &i);
	printf("\n");
	switch(i)	{
		case 0:
			page = FRED_RD;
			break;
		case 1:
			page = JIM_RD;
			break;
		case 2:
			page = SHELA_RD;
			break;
		default:
			printf("Selection not allowed.\n");
			goto label;
			break;
	}
	for(i = 0; i < 0xFF; i++)	{
		page_offset = i;
		IO_read();
		printf("0x%02X ", out_byte);
	}
	printf("\n");
	return 0;
}
