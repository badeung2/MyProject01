#include <stdint.h>
#include <string.h>
// uCAN_MSG 구조체 정의는 사용 환경(ecan1.h 등)에 따라 적절히 참조해야 합니다.

/**
 * 전송용 CRC16 (CCITT-FALSE) 산출 함수
 * resources/crc16_table.c에 정의된 함수를 호출하거나 통합하여 사용합니다.
 */
extern uint16_t Calculate_CRC16(const uint8_t *data, uint16_t len);

/**
 * CAN 메시지를 24바이트 프로토콜 패킷으로 변환하여 UART로 전송합니다.
 * @param canMsg 수신된 CAN 메시지 구조체 포인터
 * @param timestamp 데이터 발생 시점 (ms)
 */
void Send_CAN_to_UART(const uCAN_MSG *canMsg, uint32_t timestamp)
{
	uint8_t packet[24];

	// 1. Header (2 bytes)
	packet[0] = 0xAA;
	packet[1] = 0x55;

	// 2. Timestamp (4 bytes)
	memcpy(&packet[2], &timestamp, 4);

	// 3. CAN ID (4 bytes)
	memcpy(&packet[6], &canMsg->id, 4);

	// 4. DLC (1 byte)
	packet[10] = canMsg->dlc;

	// 5. Reserved/Padding (1 byte) - 24바이트 정렬용
	packet[11] = 0x00;

	// 6. Data (8 bytes) - 고정 크기 처리
	memset(&packet[12], 0, 8);
	if(canMsg->dlc > 0 && canMsg->dlc <= 8) { memcpy(&packet[12], canMsg->data, canMsg->dlc); }

	// 7. CRC16 (2 bytes) - Header 제외 영역(2~19 index) 계산
	uint16_t crc = Calculate_CRC16(&packet[2], 18);
	packet[20] = (uint8_t)(crc >> 8);
	packet[21] = (uint8_t)(crc & 0xFF);

	// 8. Footer (2 bytes) - Header와 대칭 (0x55AA)
	packet[22] = 0x55;
	packet[23] = 0xAA;

	// 9. UART 물리 계층 전송 (프로젝트에 구현된 함수 호출)
	// 예: UART1_WriteBuffer(packet, 24);
}
