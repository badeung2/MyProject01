#include "CAN_Queue.h"
#include <string.h>

/* dsPIC33 EV Interrupt safe critical section macros */
#define ENTER_CRITICAL()                                                                                                                             \
	{                                                                                                                                                \
		uint16_t _temp_SR;                                                                                                                           \
		__asm__ volatile("mov SR, %0" : "=r"(_temp_SR));                                                                                             \
		__asm__ volatile("disi #0x3FFF");                                                                                                            \
	}
#define EXIT_CRITICAL()                                                                                                                              \
	{                                                                                                                                                \
		__asm__ volatile("mov %0, SR" : : "r"(_temp_SR));                                                                                            \
	}

/* Alternative common dsPIC macro if compiler supports it */
#ifndef INTERRUPT_PROTECT
#define INTERRUPT_PROTECT(x)                                                                                                                         \
	{                                                                                                                                                \
		uint16_t cpu_sr = SRbits.IPL;                                                                                                                \
		if(SRbits.IPL < 7) SRbits.IPL = 7;                                                                                                           \
		x;                                                                                                                                           \
		SRbits.IPL = cpu_sr;                                                                                                                         \
	}
#endif

/* Simple version for dsPIC33 using SET_CPU_IPL */
#include <xc.h>
#define QUEUE_LOCK()                                                                                                                                 \
	uint16_t _cached_ipl;                                                                                                                            \
	SET_AND_SAVE_CPU_IPL(_cached_ipl, 7)
#define QUEUE_UNLOCK() RESTORE_CPU_IPL(_cached_ipl)

static CAN_QUEUE_t rxQueue;
static CAN_QUEUE_t txQueue;

void CAN_Queue_Initialize(void)
{
	memset(&rxQueue, 0, sizeof(rxQueue));
	memset(&txQueue, 0, sizeof(txQueue));
}

bool CAN_Queue_PushRx(const HAL_CAN_Packet *pkt)
{
	if(rxQueue.count >= CAN_QUEUE_SIZE) { return false; }

	QUEUE_LOCK();
	rxQueue.messages[rxQueue.head] = *pkt;
	rxQueue.head = (rxQueue.head + 1) % CAN_QUEUE_SIZE;
	rxQueue.count++;
	QUEUE_UNLOCK();
	return true;
}

bool CAN_Queue_PopRx(HAL_CAN_Packet *pkt)
{
	if(rxQueue.count == 0) { return false; }

	QUEUE_LOCK();
	*pkt = rxQueue.messages[rxQueue.tail];
	rxQueue.tail = (rxQueue.tail + 1) % CAN_QUEUE_SIZE;
	rxQueue.count--;
	QUEUE_UNLOCK();
	return true;
}

bool CAN_Queue_PushTx(const HAL_CAN_Packet *pkt)
{
	if(txQueue.count >= CAN_QUEUE_SIZE) { return false; }

	QUEUE_LOCK();
	txQueue.messages[txQueue.head] = *pkt;
	txQueue.head = (txQueue.head + 1) % CAN_QUEUE_SIZE;
	txQueue.count++;
	QUEUE_UNLOCK();
	return true;
}

bool CAN_Queue_PopTx(HAL_CAN_Packet *pkt)
{
	if(txQueue.count == 0) { return false; }

	QUEUE_LOCK();
	*pkt = txQueue.messages[txQueue.tail];
	txQueue.tail = (txQueue.tail + 1) % CAN_QUEUE_SIZE;
	txQueue.count--;
	QUEUE_UNLOCK();
	return true;
}

uint8_t CAN_Queue_GetTxCount(void)
{
	uint8_t count;
	QUEUE_LOCK();
	count = txQueue.count;
	QUEUE_UNLOCK();
	return count;
}
