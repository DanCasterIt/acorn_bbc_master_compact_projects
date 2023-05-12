#include <stdio.h>
#include <stdint.h>

#define FRED_RD 0x92	//0xFC00 to 0xFCFF
#define JIM_RD 0x94		//0xFD00 to 0xFDFF
#define SHELA_RD 0x96	//0xFE00 to 0xFEFF

#define FRED_WR 0x93	//0xFC00 to 0xFCFF
#define JIM_WR 0x95		//0xFD00 to 0xFDFF
#define SHELA_WR 0x97	//0xFE00 to 0xFEFF

typedef struct manipulate	{
	uint8_t bit0 : 1;
	uint8_t bit1 : 1;
	uint8_t bit2 : 1;
	uint8_t bit3 : 1;
	uint8_t bit4 : 1;
	uint8_t bit5 : 1;
	uint8_t bit6 : 1;
	uint8_t bit7 : 1;
}manipulate_t;

manipulate_t manipulate_v;

// IO() function arguments
uint8_t page = 0;
uint8_t page_offset = 0;
uint8_t in_byte = 0;
uint8_t out_byte = 0;

void IO(void)	{
	__asm__ ("LDA %v", page);
	__asm__ ("LDX %v", page_offset);
	__asm__ ("LDY %v", in_byte);
	__asm__ ("JSR $FFF4");
	__asm__ ("STY %v",out_byte);
}

void to_bin(void * number, unsigned char num_size, char *string)	{
	int i, e, f;
	unsigned char *num;
	for(f = 0, e = 8 * num_size -1; f < num_size;  f++)	{
		num = ((unsigned char *)number + f);
		for(i = 0; i < 8; i++, e--)	string[e] = ((*num >> i) & 1) ? '1' : '0';
	}
	string[8 * num_size] = '\0';
}

void print(uint8_t out_byte)	{
	char str[8+1];
	to_bin((void*) &out_byte, 1, str);
	printf("ACCCON@0xFE34=0x%02X:\n", out_byte);
	printf("bit 7 IRR: %c\n", str[0]);
	printf("bit 6 TST: %c\n", str[1]);
	printf("bit 5 IFJ: %c\n", str[2]);
	printf("bit 4 ITU: %c\n", str[3]);
	printf("bit 3 Y  : %c\n", str[4]);
	printf("bit 2 X  : %c\n", str[5]);
	printf("bit 1 E  : %c\n", str[6]);
	printf("bit 0 D  : %c\n", str[7]);
}

int main(void)	{
	int i;
	while(1)	{
		label:
		printf("Please make a selection:\n");
		printf("0) exit the program\n");
		printf("1) print page FRED  (0xFC00 to 0xFCFF)\n");
		printf("2) print page JIM   (0xFD00 to 0xFDFF)\n");
		printf("3) print page SHELA (0xFE00 to 0xFEFF)\n");
		printf("4) print ACCCON register\n");
		printf("5) toggle ACCCON register bits\n");
		printf("Selection: ");
		fflush(stdin);
		scanf("%u", &i);
		printf("\n");
		switch(i)	{
			case 0:
				goto exit;
			case 1:
				page = FRED_RD;
				goto loop;
			case 2:
				page = JIM_RD;
				goto loop;
			case 3:
				page = SHELA_RD;
				goto loop;
			case 4:
				page = SHELA_RD;
				page_offset = 0x34;
				IO();
				print(out_byte);
				goto label;
			case 5:
				bitsel: printf("Select which bit to toggle (0 to 7): ");
				fflush(stdin);
				scanf("%u", &i);
				printf("\n");
				if(!(i >= 0 && i < 8))	{
					printf("invalid number.\n");
					goto bitsel;
				}
				page = SHELA_RD;
				page_offset = 0x34;
				IO();
				printf("read: 0x%02X\n", out_byte);
				manipulate_v = (manipulate_t)out_byte;
				if(i == 0) manipulate_v.bit0 = ~manipulate_v.bit0;
				else if(i == 1) manipulate_v.bit1 = ~manipulate_v.bit1;
				else if(i == 2) manipulate_v.bit2 = ~manipulate_v.bit2;
				else if(i == 3) manipulate_v.bit3 = ~manipulate_v.bit3;
				else if(i == 4) manipulate_v.bit4 = ~manipulate_v.bit4;
				else if(i == 5) manipulate_v.bit5 = ~manipulate_v.bit5;
				else if(i == 6) manipulate_v.bit6 = ~manipulate_v.bit6;
				else if(i == 7) manipulate_v.bit7 = ~manipulate_v.bit7;
				page = SHELA_WR;
				in_byte = (uint8_t)manipulate_v;
				printf("write: 0x%02X\n", in_byte);
				IO();
				page = SHELA_RD;
				IO();
				print(out_byte);
				goto label;
			default:
				printf("Selection not allowed.\n");
				goto label;
		}
		loop: for(i = 0; i < 0xFF + 1; i++)	{
			page_offset = i;
			IO();
			printf("%02X ", out_byte);
			if(i%13 == 12) printf("\n");
		}
		printf("\n");
	}
	exit: return 0;
}
