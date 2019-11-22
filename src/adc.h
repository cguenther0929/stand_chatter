/******************************************************************************
*   FILE: adc.h
*
*   PURPOSE:  ADC header file.  This file will define all routines and
*           values that will be used by the ADC module.
*
*   TODO:
*
*   NOTE:
*
******************************************************************************/

#ifndef __H_ADC_H
#define __H_ADC_H

#include <xc.h>         // Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>  
#include <string.h>     // Needed mainly for memcpy function
#include "config.h"     // Project specific header file
#include "math.h"

#define REF4D096                1
#define REF2D048                2
#define EXTREF                  3
#define INTREF                  4
#define AVDD                    5
#define LEFT_JUSTIFIED          1
#define RIGHT_JUSTIFIED         2


/*
 * Function:  void EnableA2D( void );
 * --------------------
 * Turn on and configure the A2D peripheral  
 *
 * returns: Nothing 
 */
void EnableA2D( void );

/*
 * Function:  uint16_t ReadA2D( uint8_t CHAN, bool FILTER );
 * --------------------
 * Read from ADC at the desired channel.
 * Take 16 samples and average if FILTER bool is set.
 * ADC module shall first be configured via InitA2D.  
 * 
 * returns: 10 bit ADC reading from channel (little endin)
 */
uint16_t ReadA2D( uint8_t CHAN, bool FILTER );

/*
 * Function:  void InitA2D(uint8_t format, uint8_t acqtime)
 * --------------------
 * Initialize the analog to digital converter
 * 
 * returns: Nothing
 */
void InitA2D(uint8_t format, uint8_t acqtime);

/*
 * Function:  void EnableAnalogCh(uint8_t Chan);
 * --------------------
 * To enable a particular pin to read an analog voltage
 * This will work best if ANCON0 and ANCON1 are first
 * cleared such that all other pins will function as 
 * digital I/O.
 * After calling this function and passing in parameter
 * "Chan", it is then possible to read an analog voltage 
 * at pin "Chan".  
 * 
 * returns: Nothing
 */
void EnableAnalogCh(uint8_t Chan);

/*
 * Function:  void AnalogRefSel(uint8_t PosRef, uint8_t NegRef);
 * --------------------
 * Configures the positive and negative reference for the 
 * A2D.  See function to understand what the options are
 * 
 * returns: Nothing
 */
void AnalogRefSel(uint8_t PosRef, uint8_t NegRef);

#endif
/* END OF FILE */