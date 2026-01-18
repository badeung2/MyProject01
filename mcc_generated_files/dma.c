#include "dma.h"

void __attribute__((weak)) DMA_Channel2_CallBack(void) {}
void DMA_Channel2_Tasks(void)
{
	if(IFS1bits.DMA2IF)
	{
		DMA_Channel2_CallBack();
		IFS1bits.DMA2IF = 0;
	}
}

void __attribute__((weak)) DMA_Channel3_CallBack(void) {}
void DMA_Channel3_Tasks(void)
{
	if(IFS2bits.DMA3IF)
	{
		DMA_Channel3_CallBack();
		IFS2bits.DMA3IF = 0;
	}
}

/**
  End of File
*/
