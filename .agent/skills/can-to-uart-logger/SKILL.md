---
name: can-to-uart-logger
description: CAN 버스에서 수신된 데이터를 프로토콜화하여 UART를 통해 PC로 안정적으로 전송하는 가이드를 제공합니다. (24바이트 정렬 및 CRC16 적용)
---

# CAN to UART Logger 구현 가이드 (24-byte Aligned + CRC16)

이 스킬은 dsPIC33EV에서 수신한 CAN 데이터를 24바이트 고정 크기의 바이너리 패킷으로 변환하여 UART로 고속 전송하는 방법을 안내합니다.

## ## When to use this skill
- CAN 버스 데이터를 PC 모니터링 툴(Python, C# 등)로 실시간 전송해야 할 때
- 대량의 CAN 메시지를 전송 시 데이터 누락이나 깨짐을 방지하고 싶을 때
- DMA와 고속 UART를 연동하여 시스템 부하를 최소화하고 싶을 때

## ## How to use it

### 1. 프로토콜 사양 (총 24 Bytes 고정)
데이터 정렬(Alignment)과 동기화를 극대화한 프레임 구조입니다.

| Offset | Field | Size | Description |
| :--- | :--- | :--- | :--- |
| 0 | Header | 2 | 0xAA, 0x55 |
| 2 | Timestamp | 4 | ms 단위 시스템 시간 (uint32_t) |
| 6 | CAN ID | 4 | Standard/Extended ID (uint32_t) |
| 10 | DLC | 1 | 데이터 길이 (0~8) |
| 11 | Reserved | 1 | 0x00 (24바이트 정렬용 패딩) |
| 12 | Data | 8 | 실제 CAN 데이터 (8 bytes) |
| 20 | CRC16 | 2 | Header를 제외한 전체 패킷의 CRC16 (CCITT-FALSE) |
| 22 | Footer | 2 | 0x55, 0xAA |

### 2. 코드 구현

#### CRC16 (CCITT-FALSE) 산출 (Lookup Table 방식)
대용량 데이터 처리를 위해 고안된 고성능 룩업 테이블 방식입니다. 자세한 소스 코드는 리소스 파일을 참조하십시오.

- **리소스 파일**: [crc16_table.c](file:///c:/Users/realg/MPLABXProjects/20260103_CAN01.X/.agent/skills/can-to-uart-logger/resources/crc16_table.c)

```c
// resources/crc16_table.c에 정의된 함수를 사용합니다.
uint16_t Calculate_CRC16(const uint8_t *data, uint16_t len);
```
```

#### 패킷 조립 및 송신 패턴
CAN 메시지를 24바이트 프로토콜 규격에 맞춰 조립하고 송신하는 구체적인 구현 예시입니다.

- **리소스 파일**: [can_uart_logger.c](file:///c:/Users/realg/MPLABXProjects/20260103_CAN01.X/.agent/skills/can-to-uart-logger/resources/can_uart_logger.c)

```c
// resources/can_uart_logger.c에 정의된 함수를 참조합니다.
void Send_CAN_to_UART(const uCAN_MSG *canMsg, uint32_t timestamp);
```

### 3. PC측 수신 (Python 예시)
```python
import struct

# 24바이트 언팩 규격: < (Little Endian), H (Header), I (Time), I (ID), B (DLC), x (Padding), 8s (Data), H (CRC), H (Footer)
# 참고: Header/Footer는 고정값이므로 체크용으로 사용
fmt = "<HIIBx8sHH"
header, ts, can_id, dlc, data, crc, footer = struct.unpack(fmt, raw_24byte_data)
```

## ## Related Files
- `mcc_generated_files/uart1.c` (또는 전용 UART 드라이버)
- `can-monitoring` 스킬 (수신 데이터 소스)
