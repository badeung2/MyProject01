/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
				Microchip Technology Inc.

  @File Name:
				system.h

  @Summary:
				This is the sysetm.h file generated using PIC24 / dsPIC33 /
  PIC32MM MCUs

  @Description:
				This header file provides implementations for driver APIs for
  all modules selected in the GUI. Generation Information : Product Revision  :
  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.5 Device            : dsPIC33EV256GM106
				The generated drivers are tested against the following:
								Compiler          :  XC16 v2.10
								MPLAB             :  MPLAB X v6.05
*/

/*
				(c) 2020 Microchip Technology Inc. and its subsidiaries. You may
   use this software and any derivatives exclusively with Microchip products.

				THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
   WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
   IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
				PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS,
   COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

				IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL,
   PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
   KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
   HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
				FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON
   ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
   FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

				MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR
   ACCEPTANCE OF THESE TERMS.
*/

#include "system.h"
#include "can1.h"
#include "clock.h"
#include "dma.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
#include "system_types.h"
#include "tmr1.h"
#include "traps.h"
#include "uart1.h"

// PIC24 / dsPIC33 / PIC32MM MCUs Configuration Bit Settings
// FSEC
#pragma config BWRP = OFF	  // Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED // Boot Segment Code-Protect Level bits->No
							  // Protection (other than BWRP)
#pragma config BSS2 = OFF	  // Boot Segment Control bit->No Secondary Boot Segment
#pragma config GWRP = OFF	  // General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED // General Segment Code-Protect Level bits->No
							  // Protection (other than GWRP)
#pragma config CWRP = OFF	  // Configuration Segment Write-Protect
							  // bit->Configuration Segment may be written
#pragma config CSS = DISABLED // Configuration Segment Code-Protect Level
							  // bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF  // Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 0x1FFF // Boot Segment Flash Page Address Limit
							  // bits->8191

// FSIGN

// FOSCSEL
#pragma config FNOSC = FRC // Oscillator Source Selection->FRC
#pragma config IESO = ON   // Two-speed Oscillator Start-up enable bit->Start up device with FRC,
// then switch to user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE  // Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFNC = OFF // OSC2 Pin Function bit->OSC2 is clock output
#pragma config IOL1WAY = ON	  // Peripheral pin select configuration->Allow only one reconfiguration
#pragma config FCKSM = CSECME // Clock Switching Mode bits->Both Clock switching
							  // and Fail-safe Clock Monitor are enabled

// FWDT
#pragma config WDTPOST = PS32768 // Watchdog Timer Postscaler bits->1:32768
#pragma config WDTPRE = PR128	 // Watchdog Timer Prescaler bit->1:128
#pragma config PLLKEN = ON		 // PLL Lock Wait Enable bit->Clock switch to PLL will
// wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF // Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF // Watchdog Timer Enable bit->WDT disabled. SWDTEN bit control

// FPOR
#pragma config FPwrT = PWR128 // Power-on Reset Timer Value Select bits->128ms
#pragma config BOREN = ON	  // Brown-out Reset (BOR) Detection Enable bit->BOR is enabled
#pragma config ALTI2C1 = OFF  // Alternate I2C1 pins->I2C1 mapped to SDA1/SCL1 pins

// FICD
#pragma config ICS = PGD1	// ICD Communication Channel Select bits->Communicate
							// on PGEC1 and PGED1
#pragma config ANSIW = OFF	// Device Config Register Window Lock->Device configuration register are
							// not locked to ICSP write
#pragma config JTAGEN = OFF // JTAG Enable bit->JTAG is disabled

// FDMTIVTL
#pragma config DMTIVTL = 0xFFFF // Deadman Timer Interval Low Word->65535

// FDMTIVTH
#pragma config DMTIVTH = 0xFFFF // Deadman Timer Interval High Word->65535

// FDMTCNTL
#pragma config DMTCNTL = 0xFFFF // Deadman Timer Instruction Count Low Word->65535

// FDMTCNTH
#pragma config FDMTCNTH = 0xFFFF // Deadman Timer Instruction Count High Word->65535

// FDMT
#pragma config DMTEN = OFF // Deadman Timer Enable bit->Deadman Timer is
						   // disabled

// FDEVOPT
#pragma config PWMLOCK = ON	 // PWM Lock Enable bit->Certain PWM registers may
							 // only be written after a key sequence
#pragma config ALTI2C2 = OFF // Alternate I2C2 pins->I2C2 mapped to SDA2/SCL2 pins

// FALTREG
#pragma config CTXT1 = OFF // Specifies Interrupt Priority Level (IPL) Associated with Alternate
						   // Working Register Set 1 bits->Not Assigned
#pragma config CTXT2 = OFF // Specifies Interrupt Priority Level (IPL) Associated with Alternate
						   // Working Register Set 2 bits->Not Assigned

void SYSTEM_Initialize(void)
{
	PIN_MANAGER_Initialize();
	CLOCK_Initialize();
	INTERRUPT_Initialize();
	UART1_Initialize();
	DMA_Initialize();
	CAN1_Initialize();
	TMR1_Initialize();
	INTERRUPT_GlobalEnable();
	SYSTEM_CORCONModeOperatingSet(CORCON_MODE_PORVALUES);
}

/**
 End of File
*/