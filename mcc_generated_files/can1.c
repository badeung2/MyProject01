#include "can1.h"
C1CTRL1bits.WIN = 1;
C1RXM0SIDbits.SID = 0x0;
C1RXM1SIDbits.SID = 0x0;
C1RXM2SIDbits.SID = 0x0;
C1RXM0SIDbits.EID = 0x0;
C1RXM1SIDbits.EID = 0x0;
C1RXM2SIDbits.EID = 0x0;
C1RXM0EID = 0x00;
C1RXM1EID = 0x00;
C1RXM2EID = 0x00;
C1RXM0SIDbits.MIDE = 0x0;
C1RXM1SIDbits.MIDE = 0x0;
C1RXM2SIDbits.MIDE = 0x0;
C1CTRL1bits.WIN = 0;
C1TR01CONbits.TXEN0 = 0x1;
C1TR01CONbits.TXEN1 = 0x0;
C1TR23CONbits.TXEN2 = 0x0;
C1TR23CONbits.TXEN3 = 0x0;
C1TR45CONbits.TXEN4 = 0x0;
C1TR45CONbits.TXEN5 = 0x0;
C1TR67CONbits.TXEN6 = 0x0;
C1TR67CONbits.TXEN7 = 0x0;
C1TR01CONbits.TX0PRI = 0x0;
C1TR01CONbits.TX1PRI = 0x0;
C1TR23CONbits.TX2PRI = 0x0;
C1TR23CONbits.TX3PRI = 0x0;
C1TR45CONbits.TX4PRI = 0x0;
C1TR45CONbits.TX5PRI = 0x0;
C1TR67CONbits.TX6PRI = 0x0;
C1TR67CONbits.TX7PRI = 0x0;
C1RXFUL1 = 0x0000;
C1RXFUL2 = 0x0000;
C1RXOVF1 = 0x0000;
C1RXOVF2 = 0x0000;
C1INTFbits.RBIF = 0;
C1CTRL1bits.REQOP = CAN_NORMAL_OPERATION_MODE;
while(C1CTRL1bits.OPMODE != CAN_NORMAL_OPERATION_MODE);

CAN1_SetBusErrorHandler(&CAN1_DefaultBusErrorHandler);
CAN1_SetTxErrorPassiveHandler(&CAN1_DefaultTxErrorPassiveHandler);
CAN1_SetRxErrorPassiveHandler(&CAN1_DefaultRxErrorPassiveHandler);
CAN1_SetBusWakeUpActivityInterruptHandler(&CAN1_DefaultBusWakeUpActivityHandler);
CAN1_SetRxBufferInterruptHandler(&CAN1_DefaultReceiveBufferHandler);
CAN1_SetRxBufferOverFlowInterruptHandler(&CAN1_DefaultRxBufferOverFlowHandler);

IEC2bits.C1IE = 1;
C1INTEbits.RBIE = 1;
C1INTEbits.ERRIE = 1;
C1INTEbits.RBOVIE = 1;
}

void CAN1_TransmitEnable(void)
{
	DMA_PeripheralAddressSet(CAN1_TX_DMA_CHANNEL, (uint16_t)&C1TXD);
	DMA_StartAddressASet(CAN1_TX_DMA_CHANNEL, (uint16_t)(&can1msgBuf));
	DMA_ChannelEnable(CAN1_TX_DMA_CHANNEL);
}

void CAN1_ReceiveEnable(void)
{
	DMA_PeripheralAddressSet(CAN1_RX_DMA_CHANNEL, (uint16_t)&C1RXD);
	DMA_StartAddressASet(CAN1_RX_DMA_CHANNEL, (uint16_t)(&can1msgBuf));
	DMA_ChannelEnable(CAN1_RX_DMA_CHANNEL);
}

CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES requestMode)
{
	CAN_OP_MODE_STATUS status = CAN_OP_MODE_REQUEST_SUCCESS;
	if((CAN_CONFIGURATION_MODE == CAN1_OperationModeGet()) || (requestMode == CAN_DISABLE_MODE) || (requestMode == CAN_CONFIGURATION_MODE))
	{
		C1CTRL1bits.REQOP = requestMode;
		while(C1CTRL1bits.OPMODE != requestMode);
	}
	else
	{
		status = CAN_OP_MODE_REQUEST_FAIL;
	}
	return status;
}

CAN_OP_MODES CAN1_OperationModeGet(void) { return C1CTRL1bits.OPMODE; }

CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(CAN_TX_PRIOIRTY priority, CAN_MSG_OBJ *sendCanMsg)
{
	CAN_TX_MSG_REQUEST_STATUS txMsgStatus = CAN_TX_MSG_REQUEST_SUCCESS;
	CAN1_TX_CONTROLS *pTxControls = (CAN1_TX_CONTROLS *)&C1TR01CON;
	uint_fast8_t i;
	bool messageSent = false;

	if(sendCanMsg->field.dlc > CAN_DLC_8) { txMsgStatus |= CAN_TX_MSG_REQUEST_DLC_ERROR; }

	if(CAN1_TX_BUFFER_COUNT > 0)
	{
		for(i = 0; i < CAN1_TX_BUFFER_COUNT; i++)
		{
			if(pTxControls->transmit_enabled == 1)
			{
				if(pTxControls->send_request == 0)
				{
					CAN1_MessageToBuffer(&can1msgBuf[i][0], sendCanMsg);
					pTxControls->priority = priority;
					pTxControls->send_request = 1;
					messageSent = true;
					break;
				}
			}
			pTxControls++;
		}
	}
	if(messageSent == false) { txMsgStatus |= CAN_TX_MSG_REQUEST_BUFFER_FULL; }
	return txMsgStatus;
}

bool CAN1_Receive(CAN_MSG_OBJ *recCanMsg)
{
	uint_fast8_t currentBuffer;
	uint_fast8_t shiftAmount;
	bool messageReceived = false;
	uint16_t receptionFlags;

	if(C1INTFbits.RBOVIF == 1)
	{
		C1INTFbits.RBOVIF = 0;
		if(CAN1_RxBufferOverFlowInterruptHandler) { CAN1_RxBufferOverFlowInterruptHandler(); }
		return messageReceived;
	}

	if(recCanMsg->data == NULL) { return messageReceived; }

	currentBuffer = C1FIFObits.FNRB;
	if(currentBuffer < 16)
	{
		receptionFlags = C1RXFUL1;
		shiftAmount = currentBuffer;
	}
	else
	{
		receptionFlags = C1RXFUL2;
		shiftAmount = currentBuffer - 16;
	}

	if(((receptionFlags >> shiftAmount) & 0x1) == 0x1)
	{
		CAN1_DMACopy(currentBuffer, recCanMsg);
		if(currentBuffer < 16) { C1RXFUL1 &= ~(1 << shiftAmount); }
		else
		{
			C1RXFUL2 &= ~(1 << shiftAmount);
		}
		messageReceived = true;
	}

	return (messageReceived);
}

bool CAN1_IsBusOff() { return C1INTFbits.TXBO; }

bool CAN1_IsRXErrorPassive() { return C1INTFbits.RXBP; }

bool CAN1_IsRxErrorWarning(void) { return (C1INTFbits.RXWAR); }

bool CAN1_IsRxErrorActive(void)
{
	bool errorState = false;
	if((0 < C1ECbits.RERRCNT) && (C1ECbits.RERRCNT < 128)) { errorState = true; }

	return errorState;
}

bool CAN1_IsTXErrorPassive() { return (C1INTFbits.TXBP); }

bool CAN1_IsTxErrorWarning(void) { return (C1INTFbits.TXWAR); }

bool CAN1_IsTxErrorActive(void)
{
	bool errorState = false;
	if((0 < C1ECbits.TERRCNT) && (C1ECbits.TERRCNT < 128)) { errorState = true; }

	return errorState;
}

uint8_t CAN1_ReceivedMessageCountGet()
{
	uint_fast8_t messageCount;
	uint_fast8_t currentBuffer;
	uint16_t receptionFlags;

	messageCount = 0;

#if (CAN1_FIFO_STARTING_BUFFER < 16)
	/* Check any message in buffer 0 to buffer 15*/
	receptionFlags = C1RXFUL1;
	if(receptionFlags != 0)
	{
		/* check whether a message is received */
		for(currentBuffer = 0; currentBuffer < 16; currentBuffer++)
		{
			if(((receptionFlags >> currentBuffer) & 0x1) == 0x1) { messageCount++; }
		}
	}
#endif

	/* Check any message in buffer 16 to buffer 32*/
	receptionFlags = C1RXFUL2;
	if(receptionFlags != 0)
	{
		/* check whether a message is received */
		for(currentBuffer = 0; currentBuffer < 16; currentBuffer++)
		{
			if(((receptionFlags >> currentBuffer) & 0x1) == 0x1) { messageCount++; }
		}
	}

	return (messageCount);
}

void CAN1_Sleep(void)
{
	C1INTFbits.WAKIF = 0;
	C1INTEbits.WAKIE = 1;

	/* put the module in disable mode */
	C1CTRL1bits.REQOP = CAN_DISABLE_MODE;
	while(C1CTRL1bits.OPMODE != CAN_DISABLE_MODE);

	// Wake up from sleep should set the CAN1 module straight into Normal mode
}

void __attribute__((weak)) CAN1_DefaultBusErrorHandler(void) { CAN1_CallbackBusOff(); }

void CAN1_SetBusErrorHandler(void *handler) { CAN1_BusErrorHandler = handler; }

void __attribute__((weak)) CAN1_DefaultTxErrorPassiveHandler(void) { CAN1_CallbackTxErrorPassive(); }

void CAN1_SetTxErrorPassiveHandler(void *handler) { CAN1_TxErrorPassiveHandler = handler; }

void __attribute__((weak)) CAN1_DefaultRxErrorPassiveHandler(void) { CAN1_CallbackRxErrorPassive(); }

void CAN1_SetRxErrorPassiveHandler(void *handler) { CAN1_RxErrorPassiveHandler = handler; }

void __attribute__((weak)) CAN1_DefaultBusWakeUpActivityHandler(void) {}

void CAN1_SetBusWakeUpActivityInterruptHandler(void *handler) { CAN1_BusWakeUpActivityInterruptHandler = handler; }

void __attribute__((weak)) CAN1_DefaultReceiveBufferHandler(void) { CAN1_CallbackMessageReceived(); }

void CAN1_SetRxBufferInterruptHandler(void *handler) { CAN1_RxBufferInterruptHandler = handler; }

void __attribute__((weak)) CAN1_DefaultRxBufferOverFlowHandler(void) { CAN1_CallbackRxBufferOverflow(); }

void CAN1_SetRxBufferOverFlowInterruptHandler(void *handler) { CAN1_RxBufferOverFlowInterruptHandler = handler; }

void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void)
{
	if(C1INTFbits.ERRIF)
	{
		if(C1INTFbits.TXBO == 1)
		{
			if(CAN1_BusErrorHandler) { CAN1_BusErrorHandler(); }
		}

		if(C1INTFbits.TXBP == 1)
		{
			if(CAN1_TxErrorPassiveHandler) { CAN1_TxErrorPassiveHandler(); }
		}

		if(C1INTFbits.RXBP == 1)
		{
			if(CAN1_RxErrorPassiveHandler) { CAN1_RxErrorPassiveHandler(); }
		}

		/* Call error notification function */
		C1INTFbits.ERRIF = 0;
	}

	if(C1INTFbits.RBIF)
	{
		if(CAN1_RxBufferInterruptHandler) { CAN1_RxBufferInterruptHandler(); }

		C1INTFbits.RBIF = 0;
	}

	if(C1INTFbits.WAKIF)
	{
		if(CAN1_BusWakeUpActivityInterruptHandler) { CAN1_BusWakeUpActivityInterruptHandler(); }

		C1INTFbits.WAKIF = 0;
	}

	IFS2bits.C1IF = 0;
}

/*******************************************************************************

  !!! Deprecated Definitions and APIs !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/******************************************************************************
 *
 *    Function:		CAN1_transmit
 *    Description:       Transmits the message from user buffer to CAN1 buffer
 *                       as per the buffer number allocated.
 *                       Allocation of the buffer number is done by user
 *
 *    Arguments:		priority : priority of the message to be
 * transmitted sendCanMsg: pointer to the message object
 *
 *    Return Value:      true - Transmit successful
 *                       false - Transmit failure
 ******************************************************************************/
bool CAN1_transmit(CAN_TX_PRIOIRTY priority, uCAN_MSG *sendCanMsg)
{
	uint8_t msgObjData[8] = {0};
	CAN_MSG_OBJ txCanMsg;
	txCanMsg.data = msgObjData;

	txCanMsg.msgId = sendCanMsg->frame.id;
	txCanMsg.field.idType = sendCanMsg->frame.idType;
	txCanMsg.field.frameType = sendCanMsg->frame.msgtype;
	txCanMsg.field.dlc = sendCanMsg->frame.dlc;
	txCanMsg.data[0] = sendCanMsg->frame.data0;
	txCanMsg.data[1] = sendCanMsg->frame.data1;
	txCanMsg.data[2] = sendCanMsg->frame.data2;
	txCanMsg.data[3] = sendCanMsg->frame.data3;
	txCanMsg.data[4] = sendCanMsg->frame.data4;
	txCanMsg.data[5] = sendCanMsg->frame.data5;
	txCanMsg.data[6] = sendCanMsg->frame.data6;
	txCanMsg.data[7] = sendCanMsg->frame.data7;

	return (CAN1_Transmit(priority, &txCanMsg));
}

/******************************************************************************
 *
 *    Function:		CAN1_receive
 *    Description:       Receives the message from CAN1 buffer to user buffer
 *
 *    Arguments:		recCanMsg: pointer to the message object
 *
 *    Return Value:      true - Receive successful
 *                       false - Receive failure
 ******************************************************************************/
bool CAN1_receive(uCAN_MSG *recCanMsg)
{
	bool messageReceived = false;
	uint8_t msgObjData[8] = {0};
	CAN_MSG_OBJ rxCanMsg;
	rxCanMsg.data = msgObjData;

	if(true == CAN1_Receive(&rxCanMsg))
	{
		recCanMsg->frame.id = rxCanMsg.msgId;
		recCanMsg->frame.idType = rxCanMsg.field.idType;

		if(rxCanMsg.field.frameType == CAN_FRAME_RTR) { recCanMsg->frame.msgtype = CAN_MSG_RTR; }
		else
		{
			recCanMsg->frame.msgtype = CAN_MSG_DATA;
		}

		recCanMsg->frame.data0 = rxCanMsg.data[0];
		recCanMsg->frame.data1 = rxCanMsg.data[1];
		recCanMsg->frame.data2 = rxCanMsg.data[2];
		recCanMsg->frame.data3 = rxCanMsg.data[3];
		recCanMsg->frame.data4 = rxCanMsg.data[4];
		recCanMsg->frame.data5 = rxCanMsg.data[5];
		recCanMsg->frame.data6 = rxCanMsg.data[6];
		recCanMsg->frame.data7 = rxCanMsg.data[7];
		recCanMsg->frame.dlc = rxCanMsg.field.dlc;
		messageReceived = true;
	}

	return (messageReceived);
}

/******************************************************************************
*
*    Function:		CAN1_isBusOff
*    Description:       Checks whether the transmitter in Bus off state
*

*    Return Value:      true - Transmitter in Bus Off state
*                       false - Transmitter not in Bus Off state
******************************************************************************/
bool CAN1_isBusOff() { return C1INTFbits.TXBO; }

/******************************************************************************
 *
 *    Function:		CAN1_isRXErrorPassive
 *    Description:       Checks whether the receive in error passive state
 *
 *    Return Value:      true - Receiver in Error Passive state
 *                       false - Receiver not in Error Passive state
 ******************************************************************************/
bool CAN1_isRXErrorPassive() { return C1INTFbits.RXBP; }

/******************************************************************************
 *
 *    Function:		CAN1_isTXErrorPassive
 *    Description:       Checks whether the transmitter in error passive state
 *
 *    Return Value:      true - Transmitter in Error Passive state
 *                       false - Transmitter not in Error Passive state
 ******************************************************************************/
bool CAN1_isTXErrorPassive() { return (C1INTFbits.TXBP); }

/******************************************************************************
 *
 *    Function:		CAN1_messagesInBuffer
 *    Description:       returns the number of messages that are received
 *
 *    Return Value:      Number of message received
 ******************************************************************************/
uint8_t CAN1_messagesInBuffer()
{
	uint_fast8_t messageCount;
	uint_fast8_t currentBuffer;
	uint16_t receptionFlags;

	messageCount = 0;

#if (CAN1_FIFO_STARTING_BUFFER < 16)
	/* Check any message in buffer 0 to buffer 15*/
	receptionFlags = C1RXFUL1;
	if(receptionFlags != 0)
	{
		/* check whether a message is received */
		for(currentBuffer = 0; currentBuffer < 16; currentBuffer++)
		{
			if(((receptionFlags >> currentBuffer) & 0x1) == 0x1) { messageCount++; }
		}
	}
#endif

	/* Check any message in buffer 16 to buffer 32*/
	receptionFlags = C1RXFUL2;
	if(receptionFlags != 0)
	{
		/* check whether a message is received */
		for(currentBuffer = 0; currentBuffer < 16; currentBuffer++)
		{
			if(((receptionFlags >> currentBuffer) & 0x1) == 0x1) { messageCount++; }
		}
	}

	return (messageCount);
}

/******************************************************************************
 *
 *    Function:		CAN1_sleep
 *    Description:       Puts CAN1 module in disable mode.
 *
 ******************************************************************************/
void CAN1_sleep(void)
{
	C1INTFbits.WAKIF = 0;
	C1INTEbits.WAKIE = 1;

	/* put the module in disable mode */
	C1CTRL1bits.REQOP = CAN_DISABLE_MODE;
	while(C1CTRL1bits.OPMODE != CAN_DISABLE_MODE);

	// Wake up from sleep should set the CAN1 module straight into Normal mode
}

/* Null weak implementations of callback functions. */
void __attribute__((weak)) CAN1_CallbackBusOff(void) {}

void __attribute__((weak)) CAN1_CallbackTxErrorPassive(void) {}

void __attribute__((weak)) CAN1_CallbackRxErrorPassive(void) {}

void __attribute__((weak)) CAN1_CallbackMessageReceived(void) {}

void __attribute__((weak)) CAN1_CallbackRxBufferOverflow() {}

/**
 End of File
*/