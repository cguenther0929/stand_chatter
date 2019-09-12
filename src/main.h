/******************************************************************************
*   FILE: main.h
*
*   PURPOSE: Header file for main.c
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

/*
 * Function:  void tick100msDelay( uint16_t ticks );
 * --------------------
 * Blocking delay function.  Blocks in increments 
 * of 100ms chunks.  Number of "chunks" depends on 
 * "ticks" value passed to function.  This timing 
 * routine is fairly accurate as it uses a timer
 * w/ interrupts to count.  Timebase must first
 * be configured via SetUp function.    
 *
 * returns: Nothing 
 */
void tick100msDelay( uint16_t ticks );

/*
 * Function:  void tick20msDelay( uint16_t ticks );
 * --------------------
 * Blocking delay function.  Blocks in increments 
 * of 20ms chunks.  Number of "chunks" depends on 
 * "ticks" value passed to function.  This timing 
 * routine is fairly accurate as it uses a timer
 * w/ interrupts to count.  Timebase must first
 * be configured via SetUp function.    
 *
 * returns: Nothing 
 */
void tick20msDelay( uint16_t ticks );

/*
 * Function:  void SetUp( void );
 * --------------------
 * SetUp routine covering all aspects of the application. 
 * Processor GPIOs are configured.  Peripherials are 
 * configured.  ADC is configured.  System timebase 
 * and general timing parameters are configured.  
 *
 * returns: Nothing 
 */
void SetUp( void );

/*
 * Function:  float GetBatteryVoltage ( void );
 * --------------------
 * Retrieve batter voltage.  ADC must first be fully
 * configred/initialized.   
 *
 * returns: Battery Voltage as float
 */
float GetBatteryVoltage ( void );   

/*
 * Function:  void EvaluateState(  char pre_loaded_message[][16]);
 * --------------------
 * Evaluate current state and whethere or not the FSM 
 * shall advanced.  Pass in the n x 16 multi-dimensional 
 * array that contains pre-loaded messages 
 *
 * returns: Nothing
 */
void EvaluateState(  char pre_loaded_message[][16]);

/*
 * Function:  void EvaluateButtonInputs ( void )
 * --------------------
 * Evaluate whethere or not a button has been pushed
 *
 * returns: Nothing
 */
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
void DisplayDwellTmr( uint8_t action);  

/*
 * Function:  void PrintSplashScreen( void )
 * --------------------
 * Print information like model name/FW version/
 * battery voltage before the device enters into 
 * the FSM. computes an approximation of pi using:
 *
 * returns: Nothing
 */
void PrintSplashScreen( void );     

#endif