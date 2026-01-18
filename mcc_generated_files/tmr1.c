#include "tmr1.h"
#include <stdio.h>

void (*TMR1_InterruptHandler)(void) = NULL;
void TMR1_CallBack(void);

typedef struct _TMR_OBJ_STRUCT
{
	volatile bool timerElapsed;
	volatile uint32_t count;
} TMR_OBJ;

static TMR_OBJ tmr1_obj;

if(TMR1_InterruptHandler) { TMR1_InterruptHandler(); }
tmr1_obj.count++;
tmr1_obj.timerElapsed = true;
IFS0bits.T1IF = false;
}

void TMR1_Period16BitSet(uint16_t value)
{
	PR1 = value;
	tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Period16BitGet(void) { return (PR1); }

void TMR1_Counter16BitSet(uint16_t value)
{
	TMR1 = value;
	tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Counter16BitGet(void) { return (TMR1); }
void __attribute__((weak)) TMR1_CallBack(void) {}

void TMR1_SetInterruptHandler(void (*InterruptHandler)(void))
{
	IEC0bits.T1IE = false;
	TMR1_InterruptHandler = InterruptHandler;
	IEC0bits.T1IE = true;
}

void TMR1_Start(void)
{
	tmr1_obj.timerElapsed = false;
	IEC0bits.T1IE = true;
	T1CONbits.TON = 1;
}

void TMR1_Stop(void)
{
	T1CONbits.TON = false;
	IEC0bits.T1IE = false;
}

bool TMR1_GetElapsedThenClear(void)
{
	bool status;
	status = tmr1_obj.timerElapsed;
	if(status == true) { tmr1_obj.timerElapsed = false; }
	return status;
}

uint32_t TMR1_SoftwareCounterGet(void) { return tmr1_obj.count; }
void TMR1_SoftwareCounterClear(void) { tmr1_obj.count = 0; }

/**
 End of File
*/
