#include <stdio.h>
#include <stdint.h>

typedef struct SOUND	{
	uint16_t channel;
	uint16_t amplitude;
	uint16_t pitch;
	uint16_t duration;
}SOUND_t;

SOUND_t SOUND_v;
uint8_t LSB = 0;
uint8_t MSB = 0;

int main(void)	{
    LSB = ((uint16_t)&SOUND_v);
    MSB = ((uint16_t)&SOUND_v) >> 8;
	SOUND_v.channel = 0x0001;	// 1
	SOUND_v.amplitude = 0xFFF1;	// -15
	SOUND_v.pitch = 0x00C8;		// 200
	SOUND_v.duration = 0x0014;	// 20
	printf("SOUND_v.channel = 0x%04X\n", SOUND_v.channel);
	printf("SOUND_v.amplitude = 0x%04X\n", SOUND_v.amplitude);
	printf("SOUND_v.pitch = 0x%04X\n", SOUND_v.pitch);
	printf("SOUND_v.duration = 0x%04X\n", SOUND_v.duration);
	printf("((uint16_t)&SOUND_v) = 0x%04X\n", ((uint16_t)&SOUND_v));
	printf("LSB = 0x%02X\n", LSB);
	printf("MSB = 0x%02X\n", MSB);
	__asm__ ("LDX %v", LSB);
	__asm__ ("LDY %v", MSB);
	__asm__ ("LDA #$07");
	__asm__ ("JSR $FFF1");
	return 0;
}
