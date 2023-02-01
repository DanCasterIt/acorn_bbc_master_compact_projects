#include <stdio.h>

int main(void) {
    char i = 0x1b;
    int e, f;
    // Green text
    printf("\x1b[32mHello, World\n");
    // Reset colors to defaults
    printf("\x1b[0m");
	// Red text and blue background
    puts("\x1b[31m\x1b[44mHello, World");
    // Reset colors to defaults
    printf("\x1b[0m");
    
    // Green text
    printf("%c[32mHello, World\n", i);
    // Reset colors to defaults
    printf("%c[0m", i);
    
    for(e = 30; e < 38; e++)	{
    	printf("%c[%dmHello, World\n", i, e);
	}
    for(e = 30, f = 47; e < 38; e++, f--)	{
    	printf("%c[%dm%c[%dmHello, World\n", i, e, i, f);
	}
    puts("\x1b[31m\x1b[44m");
    printf("text A\n");
    printf("text B\n");
    printf("text C\n");
    // Reset colors to defaults
    printf("\x1b[0m");
    printf("text A\n");
    printf("text B\n");
    printf("text C\n");
    return 0;
}
