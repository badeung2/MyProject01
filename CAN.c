#include "CAN.h"
#include "CAN_Logger.h"
#include "CAN_Queue.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/tmr1.h"
#include <string.h>

/* Forward Declarations */
static bool dsPIC33_CAN1_Send(const HAL_CAN_Packet *pkt);
static bool dsPIC33_CAN1_Receive(HAL_CAN_Packet *pkt);
static bool dsPIC33_CAN1_IsBusOff(void);

/* Hardware Device Mapping */
static const CAN_device_t dsPIC33_CAN1_Device = {.send = dsPIC33_CAN1_Send, .receive = dsPIC33_CAN1_Receive, .isBusOff = dsPIC33_CAN1_IsBusOff};

/* Global Network Configuration */
CAN_ChannelConfig_t CAN_Network[MAX_CAN_CHANNELS] = {{.device = &dsPIC33_CAN1_Device, .isActive = true}, {.device = NULL, .isActive = false}};

/* Example Task Configuration (Can be customized by user) */
CAN_ScheduledTask g_CanTasks[] = {{.pkt = {.id = 0x100, .id_type = 0, .frame_type = 0, .dlc = 8, .data = {1, 2, 3, 4, 5, 6, 7, 8}},
								   .interval_ms = 100,
								   .last_run_ms = 0,
								   .channel_idx = CAN_CH1,
								   .callback = NULL}};

const uint8_t g_NumCanTasks = sizeof(g_CanTasks) / sizeof(CAN_ScheduledTask);

void HAL_CAN_Init(void)
{
	CAN_Queue_Initialize();
	CAN_Logger_Initialize();
	/* Note: can1_initialize is typically called by SYSTEM_Initialize */
}

void Run_CAN_Process(void)
{
	HAL_CAN_Packet rxPkt;
	CAN_ChannelConfig_t *ch = &CAN_Network[CAN_CH1];

	if(!ch->isActive || ch->device == NULL) { return; }
	if(ch->device->isBusOff()) { return; }

	while(ch->device->receive(&rxPkt))
	{
		CAN_Queue_PushRx(&rxPkt);
		CAN_Logger_Log(&rxPkt);
	}
}

void Run_CAN_Scheduler(void)
{
	uint32_t now = TMR1_SoftwareCounterGet();

	for(int i = 0; i < g_NumCanTasks; i++)
	{
		CAN_ScheduledTask *task = &g_CanTasks[i];

		if(now - task->last_run_ms >= task->interval_ms)
		{
			CAN_ChannelConfig_t *ch = &CAN_Network[task->channel_idx];

			if(ch->isActive && ch->device != NULL && ch->device->send(&task->pkt))
			{
				if(task->callback) { task->callback(); }
			}
			task->last_run_ms = now;
		}
	}
}

/* HAL Implementation for dsPIC33 (MCC Generated CAN1) */
static bool dsPIC33_CAN1_Send(const HAL_CAN_Packet *pkt)
{
	CAN_MSG_OBJ msg;
	msg.msgId = pkt->id;
	msg.field.idType = pkt->id_type;
	msg.field.frameType = pkt->frame_type;
	msg.field.dlc = pkt->dlc;
	msg.data = (uint8_t *)pkt->data;

	return (CAN1_Transmit(CAN_PRIORITY_MEDIUM, &msg) == CAN_TX_MSG_REQUEST_SUCCESS);
}

static bool dsPIC33_CAN1_Receive(HAL_CAN_Packet *pkt)
{
	CAN_MSG_OBJ msg;
	uint8_t data[8];
	msg.data = data;

	if(CAN1_Receive(&msg))
	{
		pkt->id = msg.msgId;
		pkt->id_type = msg.field.idType;
		pkt->frame_type = msg.field.frameType;
		pkt->dlc = msg.field.dlc;
		memcpy(pkt->data, msg.data, 8);
		return true;
	}
	return false;
}

static bool dsPIC33_CAN1_IsBusOff(void) { return CAN1_IsBusOff(); }
