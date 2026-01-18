#include <stdbool.h>
#include <stdint.h>

// uCAN_MSG 구조체 및 하드웨어 관련 헤더는 실제 환경에 맞게 포함해야 함

/**
 * CAN 메시지 수신 큐(원형 버퍼) 구조체
 */
typedef struct
{
	uCAN_MSG messages[64]; // 큐 크기
	uint8_t head;
	uint8_t tail;
	uint8_t count;
} CAN_QUEUE;

static CAN_QUEUE rxQueue, txQueue;

/**
 * CAN 인터럽트 서비스 루틴(ISR) 예시
 * RBIF(수신), TBIF(송신 완료) 인터럽트를 처리합니다.
 */
void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void)
{
	if(C1INTFbits.RBIF)
	{ // 수신 인터럽트
		uCAN_MSG msg;
		if(ECAN1_Receive(&msg))
		{
			// Rx 큐에 적재
			uint8_t nextHead = (rxQueue.head + 1) % 64;
			if(nextHead != rxQueue.tail)
			{ // Buffer Full 체크
				rxQueue.messages[rxQueue.head] = msg;
				rxQueue.head = nextHead;
				rxQueue.count++;
			}
		}
		C1INTFbits.RBIF = 0;
	}

	if(C1INTFbits.TBIF)
	{ // 송신 완료 인터럽트
		if(txQueue.count > 0)
		{
			// Tx 큐에서 다음 데이터 송신
			if(ECAN1_transmit(&txQueue.messages[txQueue.tail]))
			{
				txQueue.tail = (txQueue.tail + 1) % 64;
				txQueue.count--;
			}
		}
		C1INTFbits.TBIF = 0;
	}
}

/**
 * 비동기 송신을 위해 메시지를 TX 큐에 추가합니다.
 */
bool CAN_SendToQueue(const uCAN_MSG *msg)
{
	uint8_t nextHead = (txQueue.head + 1) % 64;
	if(nextHead == txQueue.tail) return false; // Buffer Full

	txQueue.messages[txQueue.head] = *msg;
	txQueue.head = nextHead;
	txQueue.count++;

	// 비어있던 상태라면 첫 전송 트리거 로직 추가 가능
	return true;
}

/**
 * 수신된 데이터를 RX 큐에서 하나 꺼내옵니다.
 */
bool CAN_GetFromRxQueue(uCAN_MSG *msg)
{
	if(rxQueue.count == 0) return false;

	*msg = rxQueue.messages[rxQueue.tail];
	rxQueue.tail = (rxQueue.tail + 1) % 64;
	rxQueue.count--;
	return true;
}
