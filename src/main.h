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
#include <stdio.h>
#include "config.h"     //Project specific header file
#include "struct.h"
#include "isr.h"
#include "timer.h"
#include "spi.h"
#include "disp.h"
#include "adc.h"


/* STATE FOR FSM */
#define STATE_IDLE_DISP         0           // Idle state with dispaly on -- RX Continuous
#define STATE_IDLE_NO_DISP      1           // Idle state with display off -- RX Continuous
#define STATE_SELECT_RECIPENTS  2           // Scrolling through a list of possible recipents 
#define STATE_SELECT_MSG        3           // Scrolling through list of possible messages
#define STATE_CONFIRM_MSG       4           // Confirm message to send
#define STATE_TRANSMIT_MSG      5           // Transmitting message  

/* DEFINES FOR TIMER ACTION*/
#define DISP_TMR_RST        0
#define DISP_TMR_CNT        1
#define DISP_TMR_ENABLE     2
#define DISP_TMR_DISABLE    3

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

/********************************************************
*FUNCTION: float GetBatteryVoltage ( void )
*PURPOSE: Retreive batter voltage
*PRECONDITION: ADC must be configured.  
*POSTCONDITION: Battery voltage measured
*RETURN: Float value of battery voltage
********************************************************/
float GetBatteryVoltage ( void );   

/********************************************************
*FUNCTION: void EvaluateState(  char pre_loaded_message[][16])
*PURPOSE: Evaluate FSM
*PRECONDITION: pre loaded messages array must be configured.  
            struct containing receiver buffer shall be configured.
*POSTCONDITION: FSM analyzed
*RETURN: Nothing
********************************************************/
void EvaluateState(  char pre_loaded_message[][16]);

/********************************************************
*FUNCTION: void EvaluateButtonInputs ( void )
*PURPOSE: Evaluate whethere or not a button has been pushed
*PRECONDITION: GPIO for buttons must me configured.  Timebase
            shall be configured and running.  
*POSTCONDITION: Button pushes analyzed
*RETURN: Nothing
********************************************************/
void EvaluateButtonInputs ( void ); 

/*
 * Function:  void DisplayDwellTmr( uint8_t action)
 * --------------------
 * Handle display dwell timer.  This timer is responsible
 * for powering off the display backlight after a period of 
 * no activity (no buttons or received messages)
 *
 * returns: Nothing
 */
void DisplayDwellTmr( uint8_t action);  // TODO need to comment

/*
 * Function:  void PrintSplashScreen( void )
 * --------------------
 * Print information like model name/FW version/
 * battery voltage before the device enters into 
 * the FSM. computes an approximation of pi using:
 *
 * returns: Nothing
 */
void PrintSplashScreen( void );     // TODO need to comment

#endif