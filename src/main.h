/******************************************************************************
*   FILE: main.h
*
*   PURPOSE: Header file for main.c
*
*   DEVICE: PIC18F66K22
*
*   COMPILER: Microchip XC8 v1.32
*
*   IDE: MPLAB X v3.45
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/
#ifndef __MAIN_H_
#define __MAIN_H_

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "isr.h"
#include "config.h"     //Project specific header file
#include "timer.h"
#include "spi.h"
#include "disp.h"
#include "adc.h"


/********************************************************
*FUNCTION: void tick100msDelay( uint16_t ticks )
*PURPOSE: Much more accurate timer that works off interrupts
            User must define how many 1/10s of a tick1000mond he/she
            wishes to pass
*PRECONDITION: Timers must be set up and running in order for this to work
*POSTCONDITION: tick100ms * 1/10s of a tick1000mond have passed.
*RETURN: Nothing
********************************************************/
void tick100msDelay( uint16_t ticks );

/********************************************************
*FUNCTION: void tick20msDelay( uint16_t ticks )
*PURPOSE: Much more accurate timer that works off interrupts
            User passes in how many 20ms chunks he/she wishes to pass 
*PRECONDITION: Timer0 set up and running and set to interrupt
*POSTCONDITION: Blocking delay inserted
*RETURN: Nothing
********************************************************/
void tick20msDelay( uint16_t ticks );


/********************************************************
*FUNCTION: void SetUp( void );
*PURPOSE: Set up the PIC I/O and etc...
*PRECONDITION: PIC not configured
*POSTCONDITION: PIC I/O Configured
*RETURN: Nothing
********************************************************/
void SetUp( void );

float GetBatteryVoltage ( void );  //TODO need to comment

#endif