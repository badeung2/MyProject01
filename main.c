#include "CAN.h"
#include "mcc_generated_files/system.h"

int main(void)
{
	SYSTEM_Initialize();
	HAL_CAN_Init();
	while(1)
	{
		Run_CAN_Process();
		Run_CAN_Scheduler();
	}
}
