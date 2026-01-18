#include "uart1.h"
#include "xc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t *volatile rxTail;
static uint8_t *rxHead;
static uint8_t *txTail;
static uint8_t *volatile txHead;
static bool volatile rxOverflowed;

#define UART1_CONFIG_TX_BYTEQ_LENGTH (64 + 1)
#define UART1_CONFIG_RX_BYTEQ_LENGTH (64 + 1)

static uint8_t txQueue[UART1_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t rxQueue[UART1_CONFIG_RX_BYTEQ_LENGTH];

void (*UART1_TxDefaultInterruptHandler)(void);
void (*UART1_RxDefaultInterruptHandler)(void);
uint16_t size;
uint8_t *snapshot_txHead = (uint8_t *)txHead;
if(txTail < snapshot_txHead) { size = (snapshot_txHead - txTail - 1); }
else
{
	size = (UART1_CONFIG_TX_BYTEQ_LENGTH - (txTail - snapshot_txHead) - 1);
}
if(size > 0xFF) { return 0xFF; }
return size;
}

unsigned int __attribute__((deprecated)) UART1_ReadBuffer(uint8_t *buffer, unsigned int numbytes)
{
	unsigned int rx_count = UART1_RxDataAvailable();
	unsigned int i;
	if(numbytes < rx_count) { rx_count = numbytes; }
	for(i = 0; i < rx_count; i++) { *buffer++ = UART1_Read(); }
	return rx_count;
}

unsigned int __attribute__((deprecated)) UART1_WriteBuffer(uint8_t *buffer, unsigned int numbytes)
{
	unsigned int tx_count = UART1_TxDataAvailable();
	unsigned int i;
	if(numbytes < tx_count) { tx_count = numbytes; }
	for(i = 0; i < tx_count; i++) { UART1_Write(*buffer++); }
	return tx_count;
}

UART1_TRANSFER_STATUS __attribute__((deprecated)) UART1_TransferStatusGet(void)
{
	UART1_TRANSFER_STATUS status = 0;
	uint8_t rx_count = UART1_RxDataAvailable();
	uint8_t tx_count = UART1_TxDataAvailable();
	switch(rx_count)
	{
		case 0: status |= UART1_TRANSFER_STATUS_RX_EMPTY; break;
		case UART1_CONFIG_RX_BYTEQ_LENGTH: status |= UART1_TRANSFER_STATUS_RX_FULL; break;
		default: status |= UART1_TRANSFER_STATUS_RX_DATA_PRESENT; break;
	}
	switch(tx_count)
	{
		case 0: status |= UART1_TRANSFER_STATUS_TX_FULL; break;
		case UART1_CONFIG_RX_BYTEQ_LENGTH: status |= UART1_TRANSFER_STATUS_TX_EMPTY; break;
		default: break;
	}
	return status;
}

uint8_t __attribute__((deprecated)) UART1_Peek(uint16_t offset)
{
	uint8_t *peek = rxHead + offset;
	while(peek > (rxQueue + UART1_CONFIG_RX_BYTEQ_LENGTH)) { peek -= UART1_CONFIG_RX_BYTEQ_LENGTH; }
	return *peek;
}

bool __attribute__((deprecated)) UART1_ReceiveBufferIsEmpty(void) { return (UART1_RxDataAvailable() == 0); }
bool __attribute__((deprecated)) UART1_TransmitBufferIsFull(void) { return (UART1_TxDataAvailable() == 0); }
uint16_t __attribute__((deprecated)) UART1_StatusGet(void) { return U1STA; }

unsigned int __attribute__((deprecated)) UART1_TransmitBufferSizeGet(void)
{
	if(UART1_TxDataAvailable() != 0)
	{
		if(txHead > txTail) { return ((txHead - txTail) - 1); }
		else
		{
			return ((UART1_CONFIG_TX_BYTEQ_LENGTH - (txTail - txHead)) - 1);
		}
	}
	return 0;
}

unsigned int __attribute__((deprecated)) UART1_ReceiveBufferSizeGet(void)
{
	if(UART1_RxDataAvailable() != 0)
	{
		if(rxHead > rxTail) { return ((rxHead - rxTail) - 1); }
		else
		{
			return ((UART1_CONFIG_RX_BYTEQ_LENGTH - (rxTail - rxHead)) - 1);
		}
	}
	return 0;
}

void __attribute__((deprecated)) UART1_Enable(void)
{
	U1MODEbits.UARTEN = 1;
	U1STAbits.UTXEN = 1;
}

void __attribute__((deprecated)) UART1_Disable(void)
{
	U1MODEbits.UARTEN = 0;
	U1STAbits.UTXEN = 0;
}
