#include "CAN_Logger.h"
#include "mcc_generated_files/uart1.h"
#include <string.h>

#define LOGGER_SYNC_BYTE1 0xAA
#define LOGGER_SYNC_BYTE2 0x55

void CAN_Logger_Initialize(void) { /* UART initialization is usually handled in SYSTEM_Initialize */ }

static uint16_t Calculate_CRC16(const uint8_t *data, uint16_t len)
{
	uint16_t crc = 0xFFFF;
	for(uint16_t i = 0; i < len; i++)
	{
		crc ^= data[i];
		for(int j = 0; j < 8; j++)
		{
			if(crc & 0x0001) { crc = (crc >> 1) ^ 0xA001; }
			else {
				crc >>= 1;
			}
		}
	}
	return crc;
}

void CAN_Logger_Log(const HAL_CAN_Packet *pkt)
{
	uint8_t buffer[24];
	uint8_t payload[20];
	uint16_t crc;

	memset(payload, 0, sizeof(payload));

	/* ID (Big Endian) */
	payload[0] = (uint8_t)(pkt->id >> 24);
	payload[1] = (uint8_t)(pkt->id >> 16);
	payload[2] = (uint8_t)(pkt->id >> 8);
	payload[3] = (uint8_t)(pkt->id);

	payload[4] = pkt->id_type;
	payload[5] = pkt->frame_type;
	payload[6] = pkt->dlc;

	/* Data */
	memcpy(&payload[7], pkt->data, 8);

	/* CRC Calculation */
	crc = Calculate_CRC16(payload, 20);

	/* Final Packet Assembly */
	buffer[0] = LOGGER_SYNC_BYTE1;
	buffer[1] = LOGGER_SYNC_BYTE2;
	memcpy(&buffer[2], payload, 20);
	buffer[22] = (uint8_t)(crc & 0xFF);
	buffer[23] = (uint8_t)(crc >> 8);

	/* Send via UART */
	for(int i = 0; i < 24; i++) { UART1_Write(buffer[i]); }
}
