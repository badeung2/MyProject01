---
name: can-monitoring
description: dsPIC33EV256GM106에서 인터럽트와 큐(원형 버퍼)를 사용하여 효율적인 CAN 통신(송수신)을 구현하는 가이드를 제공합니다.
---

# dsPIC33EV256GM106 CAN 인터럽트 & 큐 구현 가이드

이 스킬은 dsPIC33EV MCU에서 ECAN 모듈의 성능을 극대화하기 위해 인터럽트와 소프트웨어 큐(원형 버퍼)를 사용하는 송수신 방식을 안내합니다.

## ## When to use this skill
- 실시간성이 중요하여 폴링 방식의 데이터 유실이 우려될 때
- 백그라운드에서 데이터를 송수신하고 메인 루프에서 처리를 분리하고 싶을 때
- 송신 버퍼가 가득 찼을 때 대기하지 않고 비동기적으로 데이터를 보내고 싶을 때

## ## How to use it

### 1. MCC(Microchip Code Configurator) 설정
1.  **ECAN1 Module**:
    -   `Enable ECAN1 Interrupt`를 체크합니다.
    -   `TX`, `RX` 인터럽트를 각각 활성화합니다.
2.  **DMA Module**:
    -   ECAN1 전용 DMA 채널(Tx, Rx)을 설정하여 하드웨어 버퍼와 RAM 사이의 전송을 자동화합니다.

### 2. 코드 구현 패턴
인터럽트와 큐(원형 버퍼)를 활용한 효율적인 구현 상세는 리소스 파일을 참조하십시오.

- **리소스 파일**: [can_monitoring.c](file:///c:/Users/realg/MPLABXProjects/20260103_CAN01.X/.agent/skills/can-monitoring/resources/can_monitoring.c)

#### 주요 구현 요소
1.  **원형 버퍼(Queue)**: 메시지를 비동기적으로 관리하기 위한 구조체 정의.
2.  **ISR(인터럽트 핸들러)**: 하드웨어 이벤트를 감지하여 큐에 데이터를 적재/제출.
3.  **애플리케이션 API**: `CAN_SendToQueue()`, `CAN_GetFromRxQueue()` 등의 추상화된 함수 제공.

### 3. 주의사항
- **임계 영역(Critical Section)**: 큐의 `count`, `head`, `tail` 변수를 수정할 때는 인터럽트를 일시적으로 비활성화하거나 원자적 연산을 사용하여 데이터 오염을 방지해야 합니다.
- **Buffer Overflow**: 큐가 가득 찼을 때의 처리 정책(가장 오래된 데이터 덮어쓰기 등)을 프로젝트 요구사항에 맞게 설정하십시오.

## ## Related Files
- `mcc_generated_files/ecan1.c` (ISR 및 하드웨어 연동)
- `can_queue.h / can_queue.c` (큐 관리 로직 분리 권장)
