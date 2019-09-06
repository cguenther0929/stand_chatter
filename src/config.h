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

#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "struct.h"

/* PREPROCESSOR CALCULATION TO DETERMINE TIMER4'S INCREMENT RATE */
#define MCU_OSC_FRQ         8000000.0                  // Oscillator used for MCU
#define OSC_DIV4            (MCU_OSC_FRQ/4.0)           // Oscillator used for MCU

/* REGISTER VALUES FOR 10MS TIME BASE */         
#define TMR0_INTUP_SETTING  1                                                           // 1 = Caused interrupts, 0 = do not cause interrupts
#define TMR0_PRESCALER      8                                                           // Options are 1, 2, 4, 8, 16, 32, 128, or 256
#define TMR0_INC_FREQ       (OSC_DIV4/TMR0_PRESCALER)                                   // Effective rate at which the timer increments
#define HEART_BEAT_MS       20.0                                                        // Interrupt at this periodicity (mili-seconds)
#define TMR0_TICKS          ((HEART_BEAT_MS/1000.0)*TMR0_INC_FREQ)                      // How many timer ticks between interrupts
#define TMR0HIGH            (uint8_t)((65535-TMR0_TICKS)/256)                           // Value to be loaded into the 8-bit register
#define TMR0LOW             (uint8_t)(TMR0_TICKS-(256*(uint8_t)(TMR0_TICKS/256)))        // Module implementation to obtain register low value

/* DEFINE CODE VERSION NUMBER */
#define MAJVER                  0x00
#define MINVER                  0x01
#define BUGVER                  0x00

/* DEFINITIONS FOR MESSAGES */
#define NUM_MESSAGES            12

/* DEFINITIONS RELATED TO DISPLAY */
#define MAX_DISP_DWELL          5                           // Max amount of time (in seconds) display is allowed to be ON
#define SPLASH_SCREEN_DWELL     5                           // Amount of time (in one hundred mili-second) a temp message is allowed to display

/* PROCESSOR DEFINES FOR RFM69 LoRa RADIO MODULE */
#define RFM_IRQ_PIN             LATEbits.LATE7
#define RFM_SPI_CS              LATDbits.LATD7                      
#define RFM_RST                 LATDbits.LATD3                      

/* PIN DEFINES FOR LCD SCREEN */
#define disp_enable             LATBbits.LATB4          // Active low signal for turning on display
#define disp_reset              LATCbits.LATC1          // Active low signal to reset the display
#define disp_reg_sel            LATCbits.LATC0          // Register select signal. 0 = instruction, 1 = data
#define disp_spi_cs             LATCbits.LATC2          // Display Chip Select Signal 

/* DEFINES FOR LED PINS */
#define health_led              LATBbits.LATB5          // For driving health LED  

/* DEFINES FOR PUSH BUTTONS */
#define PB_LT_PIN               PORTBbits.RB0           // Inputs from buttons
#define PB_RT_PIN               PORTBbits.RB1
#define BUTTON_PUSHED           1                       // State of input when button pushed
#define BUTTON_RELEASED         !(BUTTON_PUSHED)
#define BUTTON_DEBOUNCE_TIME    0.140                   // Should be in increments of timer resolution (i.e. 20ms)
#define BUTTON_DEBOUNCE_TICKS   (uint8_t)(BUTTON_DEBOUNCE_TIME/0.02)

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

/* IO MNEMONICS */
#define LED_ON              0
#define LED_OFF             1
#define DISPLAY_OFF         1
#define DISPLAY_ON          0

#endif