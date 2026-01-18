#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include <stdint.h>
typedef enum
{
	CAN_TX_MSG_REQUEST_SUCCESS = 0,		// Transmit message object successfully placed into Transmit Buffer
	CAN_TX_MSG_REQUEST_DLC_ERROR = 1,	// Transmit message object DLC size is more
										// than Transmit Buffer configured DLC size
	CAN_TX_MSG_REQUEST_BUFFER_FULL = 3, // Transmit Buffer is Full
} CAN_TX_MSG_REQUEST_STATUS;

/**
  CAN Operation Modes Enumeration

  @Summary
		Defines the CAN operation modes enumeration.

  @Description
		This enumeration defines the CAN different operating modes.
*/
typedef enum
{
	CAN_NORMAL_OPERATION_MODE = 0, // Deprecated enum value, Use CAN_NORMAL_2_0_MODE instead.
	CAN_NORMAL_2_0_MODE = 0,
	CAN_DISABLE_MODE = 1,
	CAN_LOOPBACK_MODE = 2, // Deprecated enum value, Use CAN_INTERNAL_LOOPBACK_MODE instead.
	CAN_INTERNAL_LOOPBACK_MODE = 2,
	CAN_LISTEN_ONLY_MODE = 3,
	CAN_CONFIGURATION_MODE = 4,
	CAN_LISTEN_ALL_MESSAGES_MODE = 7
} CAN_OP_MODES;

/**
  CAN Transmit Message Priority Selection Enumeration

  @Summary
		Defines the CAN Transmit Message Priority Selection Enumeration.

  @Description
		This enumeration defines the CAN transmit priority selection.
*/
typedef enum
{
	CAN_PRIORITY_HIGH = 0b11,
	CAN_PRIORITY_MEDIUM = 0b10,
	CAN_PRIORITY_LOW = 0b01,
	CAN_PRIORITY_NONE = 0b00
} CAN_TX_PRIOIRTY;

/**
  CAN Message Object Identifier Selection Enumeration

  @Summary
		Defines the CAN message object identifier selection enumeration.

  @Description
		This enumeration defines the CAN message object identifier selection
  option.
*/
typedef enum
{
	CAN_FRAME_STD = 0,
	CAN_FRAME_EXT = 1,
} CAN_MSG_OBJ_ID_TYPE;

/**
  CAN Message Object Frame Type Selection Enumeration

  @Summary
		Defines the CAN message object frame type selection enumeration.

  @Description
		This enumeration defines the CAN message object frame type selection
  option.
*/
typedef enum
{
	CAN_FRAME_DATA = 0,
	CAN_FRAME_RTR = 1,
} CAN_MSG_OBJ_FRAME_TYPE;

/**
  CAN Operation Modes Request Status Enumeration

  @Summary
		Defines the CAN operation modes request status enumeration.

  @Description
		This enumeration defines the CAN operation mode request status.
*/
typedef enum
{
	CAN_OP_MODE_REQUEST_SUCCESS, // Requested Operation mode set successfully
	CAN_OP_MODE_REQUEST_FAIL,	 // Requested Operation mode set failure. Set
								 // configuration mode before setting CAN normal or
								 // debug operation mode.
} CAN_OP_MODE_STATUS;

/**
  CAN Message Object Data Length code Enumeration

  @Summary
		Defines the CAN message object data length code enumeration.

  @Description
		The Data Length Code (DLC) specifies the number of data bytes a message
  frame contains.
*/
typedef enum
{
	CAN_DLC_0,
	CAN_DLC_1,
	CAN_DLC_2,
	CAN_DLC_3,
	CAN_DLC_4,
	CAN_DLC_5,
	CAN_DLC_6,
	CAN_DLC_7,
	CAN_DLC_8
} CAN_DLC;

/*******************************************************************************

  !!! Deprecated Union\Structure and Macros !!!
  !!! These Union\Structure and Macros will not be supported in future releases
!!!

*******************************************************************************/

/* CAN message type identifiers */
#define CAN_MSG_DATA 0x01 // Deprecated macro, Use CAN_FRAME_DATA instead.
#define CAN_MSG_RTR 0x02  // Deprecated macro, Use CAN_FRAME_RTR instead.
#define CAN_BUF_FULL 0x05
#define CAN_BUF_EMPTY 0x06

// Deprecated uCAN_MSG message object implementation
typedef union
{
	struct
	{
		uint32_t id;
		uint8_t idType;
		uint8_t msgtype;
		uint8_t dlc;
		uint8_t data0;
		uint8_t data1;
		uint8_t data2;
		uint8_t data3;
		uint8_t data4;
		uint8_t data5;
		uint8_t data6;
		uint8_t data7;
	} frame;
	uint8_t array[16];
} uCAN_MSG;

#endif //_CAN_TYPES_H
/**
 End of File
*/
