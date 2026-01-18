#ifndef CAN_QUEUE_H
#define CAN_QUEUE_H

#include "CAN.h"
#include <stdbool.h>
#include <stdint.h>

#define CAN_QUEUE_SIZE 64

typedef struct
{
	HAL_CAN_Packet messages[CAN_QUEUE_SIZE];
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t count;
} CAN_QUEUE_t;

void CAN_Queue_Initialize(void);
bool CAN_Queue_PushRx(const HAL_CAN_Packet *pkt);
bool CAN_Queue_PopRx(HAL_CAN_Packet *pkt);
bool CAN_Queue_PushTx(const HAL_CAN_Packet *pkt);
bool CAN_Queue_PopTx(HAL_CAN_Packet *pkt);
uint8_t CAN_Queue_GetTxCount(void);

#endif
