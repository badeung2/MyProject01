/**
  PIN MANAGER Generated Driver File

  @Company:
		Microchip Technology Inc.

  @File Name:
		pin_manager.c

  @Summary:
		This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM
  MCUs device.  This manager configures the pins direction, initial state,
  analog setting. The peripheral pin select, PPS, configuration is also handled
  by this manager.

  @Description:
		This source file provides implementations for PIN MANAGER.
		Generation Information :
				Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.5
				Device            :  dsPIC33EV256GM106
		The generated drivers are tested against the following:
				Compiler          :  XC16 v2.10
				MPLAB 	          :  MPLAB X v6.05
*/

/*
		(c) 2020 Microchip Technology Inc. and its subsidiaries. You may use
   this software and any derivatives exclusively with Microchip products.

		THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
		EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
   IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
		PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS,
   COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

		IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL,
   PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
   KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
   HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
		FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
   IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
   ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

		MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
   THESE TERMS.
*/

/**
		Section: Includes
*/

#include "pin_manager.h"
#include <xc.h>

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize(void)
{
	LATB = 0x0000;
	LATC = 0x0000;
	LATD = 0x0000;
	LATE = 0x0000;
	LATF = 0x0000;
	TRISB = 0xFFFF;
	TRISC = 0xFFFF;
	TRISD = 0xFFFF;
	TRISE = 0xFFFF;
	TRISF = 0xFFFF;
	ANSELB = 0x0000;
	ANSELC = 0x0000;
	ANSELE = 0x0000;

	__builtin_write_OSCCONL(OSCCON & 0xbf);
	RPOR1bits.RP37R = 0x0001;
	RPINR18bits.U1RXR = 0x0024;
	RPOR11bits.RP112R = 0x000E;
	RPINR26bits.C1RXR = 0x0021;
	__builtin_write_OSCCONL(OSCCON | 0x40);
}
