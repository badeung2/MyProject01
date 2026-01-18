#ifndef CAN_LOGGER_H
#define CAN_LOGGER_H

#include "CAN.h"

/**
 * 전송용 24바이트 패킷 로깅 초기화
 */
void CAN_Logger_Initialize(void);

/**
 * 수신된 CAN 메시지를 UART로 전송 (24바이트 프로토콜)
 * @param pkt CAN 메시지 데이터
 */
void CAN_Logger_Log(const HAL_CAN_Packet *pkt);

#endif
