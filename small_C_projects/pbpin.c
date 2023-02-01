#include <stdio.h>
#include <stdint.h>

#define SHELA_RD 0x96	//0xFE00 to 0xFEFF
#define SHELA_WR 0x97	//0xFE00 to 0xFEFF
#define UDDRB    0x62   //0xFE62
#define UIORB    0x60   //0xFE60

uint8_t page = 0;
uint8_t page_offset = 0;
uint8_t in_byte = 0;
uint8_t out_byte = 0;

typedef struct manipulate	{
	int bit0 : 1;
	int bit1 : 1;
	int bit2 : 1;
	int bit3 : 1;
	int bit4 : 1;
	int bit5 : 1;
	int bit6 : 1;
	int bit7 : 1;
}manipulate_t;

manipulate_t manipulate_v;

void IO(void)	{
	__asm__ ("LDA %v", page);
	__asm__ ("LDX %v", page_offset);
	__asm__ ("LDY %v", in_byte);
	__asm__ ("JSR $FFF4");
	__asm__ ("STY %v",out_byte);
}

void to_bin (void * number, unsigned char num_size, char *string)	{
	int i, e, f;
	unsigned char *num;
	for(f = 0, e = 8 * num_size -1; f < num_size;  f++)	{
		num = &((unsigned char*)number)[f];
		for(i = 0; i < 8; i++, e--)	{
			if(((*num >> i) & 1) == 1)	string[e] = '1';
			else	string[e] = '0';
		}
	}
	string[8 * num_size] = '\0';
}

void print(uint8_t offset)	{
	char str[8+1];
	page = SHELA_RD;
	page_offset = offset;
	IO();
	to_bin((void*) &out_byte, 1, str);
	printf("bit 7: %c\n", str[0]);
	printf("bit 6: %c\n", str[1]);
	printf("bit 5: %c\n", str[2]);
	printf("bit 4: %c\n", str[3]);
	printf("bit 3: %c\n", str[4]);
	printf("bit 2: %c\n", str[5]);
	printf("bit 1: %c\n", str[6]);
	printf("bit 0: %c\n", str[7]);
}

void toggle(uint8_t offset)	{
	int i;
	print(offset);
	bitsel: printf("Select which bit to toggle (0 to 7): ");
	fflush(stdin);
	scanf("%u", &i);
	printf("\n");
	if(!(i >= 0 && i < 8))	{
		printf("invalid number.\n");
		goto bitsel;
	}
	page = SHELA_RD;
	page_offset = offset;
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
	print(offset);
}

int main(void)	{
	int i;
	while(1)	{
		label:
		printf("Please make a selection:\n");
		printf("0) exit the program\n");
		printf("1) print user VIA DDRB register\n");
		printf("2) toggle user VIA DDRB register bits\n");
		printf("3) print user VIA I/ORB register\n");
		printf("4) toggle user VIA I/ORB register bits\n");
		printf("Selection: ");
		fflush(stdin);
		scanf("%u", &i);
		printf("\n");
		switch(i)	{
			case 0:
				goto exit;
			case 1:
				print(UDDRB);
				goto label;
			case 2:
				toggle(UDDRB);
				goto label;
			case 3:
				print(UIORB);
				goto label;
			case 4:
				toggle(UIORB);
				goto label;
			default:
				printf("Selection not allowed.\n");
				goto label;
		}
	}
	exit: return 0;
}
