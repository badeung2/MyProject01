#ifndef CAN_H
#define CAN_H

#include <stdbool.h>
#include <stdint.h>

/**
 * CAN Message Object (HAL Level)
 */
typedef struct
{
	uint32_t id;
	uint8_t id_type;	/* 0: Standard, 1: Extended */
	uint8_t frame_type; /* 0: Data, 1: RTR */
	uint8_t dlc;
	uint8_t data[8];
} HAL_CAN_Packet;

/**
 * Scheduled CAN Task Structure
 */
typedef struct
{
	HAL_CAN_Packet pkt;
	uint32_t interval_ms;
	uint32_t last_run_ms;
	uint8_t channel_idx;
	void (*callback)(void);
} CAN_ScheduledTask;

/**
 * Hardware Abstraction Interface
 */
typedef struct
{
	bool (*send)(const HAL_CAN_Packet *pkt);
	bool (*receive)(HAL_CAN_Packet *pkt);
	bool (*isBusOff)(void);
} CAN_device_t;

typedef struct
{
	const CAN_device_t *device;
	bool isActive;
} CAN_ChannelConfig_t;

/**
 * Channel Definitions
 */
typedef enum
{
	CAN_CH1 = 0,
	CAN_CH2,
	MAX_CAN_CHANNELS
} CAN_Channel_Index_t;

/**
 * Global External Variables
 */
extern CAN_ChannelConfig_t CAN_Network[MAX_CAN_CHANNELS];
extern CAN_ScheduledTask g_CanTasks[];
extern const uint8_t g_NumCanTasks;

/**
 * Public APIs
 */
void HAL_CAN_Init(void);
void Run_CAN_Process(void);
void Run_CAN_Scheduler(void);

#endif