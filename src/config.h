/******************************************************************************
*   FILE: config.h
*
*   PURPOSE: Configuration file specific to the processor being used and the 
*           underlying hardware. 
*
*   DEVICE: PPIC18F66K22
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
#ifndef __CONFIG_H_
#define __CONFIG_H_

//#include "type.h"       //Needed for defined functions
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

/* PREPROCESSOR CALCULATION TO DETERMINE TIMER4'S INCREMENT RATE */
#define MCU_OSC_FRQ         16000000.0                  // Oscillator used for MCU
#define OSC_DIV4            (MCU_OSC_FRQ/4.0)           // Oscillator used for MCU

/* REGISTER VALUES FOR 10MS TIME BASE */         
#define TMR0_INTUP_SETTING  1                                                           // 1 = Caused interrupts, 0 = do not cause interrupts
#define TMR0_PRESCALER      2.0                                                         // Options are 1, 2, 4, 8, 16, 32, 128, or 256
#define TMR0_INC_FREQ       (OSC_DIV4/TMR0_PRESCALER)                                   // Effective rate at which the timer increments
#define HEART_BEAT_MS       100.0                                                       // Interrupt at this periodicity (mili-seconds)
#define TMR0_TICKS          ((HEART_BEAT_MS/1000.0)*TMR0_INC_FREQ)                      // How many timer ticks between interrupts
#define TMR0HIGH            (uint8_t)((65535-TMR0_TICKS)/256)                           // Value to be loaded into the 8-bit register
#define TMR0LOW             (uint8_t)(TMR0_TICKS-(256*(uint8_t)(TMR0_TICKS/256))        // Module implementation to obtain register low value

/* REGISTER VALUES FOR 100MS TIME BASE */    //TODO can probably remove this
// #define TMR0HIGH            255         //16MHz IN OSC w/ prescaler of 2 and a period of 100ms
// #define TMR0LOW             55        

/* DEFINE CODE VERSION NUMBER */
#define MAJVER              0x00
#define MINVER              0x00
#define BUGVER              0x01

/* PROCESSOR DEFINES FOR RFM69 LoRa RADIO MODULE */
// TODO:  how about assigning the other DIO signals?
// TODO: how are we going to assign the SPI peripheral? Will we be clever and list it here?
#define RF69_IRQ_PIN        LATEbits.LATE7
#define RF69_SPI_CS         LATDbits.LATD7                      
#define RF69_RST            LATDbits.LATD3                      

/* PIN DEFINES FOR LCD SCREEN */

/* DEFINES FOR LED PINS */
#define HEALTH_LED          LATBbits.LATB5          // For driving health LED  

/* DEFINES FOR PUSH BUTTONS */
#define PB1                 PORTBbits.RB0           // Inputs from buttons
#define PB2                 PORTBbits.RB1

/* DEFINES FOR ANALOG SENSE */
#define BAT_VOLTAGE_CH      0           // Analog channel for reading battery voltage

/* DEFINE VARIOUS PIN FUNCITONS */
#define output              0           //Define the output pin direction setting
#define input               1

/* INTUTIVE TIMER NAMES */
#define TIMER1              1
#define TIMER2              2
#define TIMER3              3
#define TIMER4              4
#define TIMER5              5
#define TIMER6              6
#define TIMER7              7
#define TIMER8              8
#define TIMER9              9   
#define TIMER10             10

/* LED CONTROL */
#define ledon               0
#define ledoff              1

#endif