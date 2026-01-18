#include "clock.h"
#include <stdint.h>

void CLOCK_Initialize(void)
{
	CLKDIV = 0x3000;
	OSCTUN = 0x0000;
	REFOCON = 0x0000;
	PLLFBD = 0x0030;
	PMD1 = 0x0000;
	PMD2 = 0x0000;
	PMD3 = 0x0000;
	PMD4 = 0x0000;
	PMD6 = 0x0000;
	PMD7 = 0x0000;
	PMD8 = 0x0000;
	__builtin_write_OSCCONH(0x01);
	__builtin_write_OSCCONL(OSCCON | 0x01);
	while(OSCCONbits.COSC != 0x1);
	while(OSCCONbits.LOCK != 1);
}
