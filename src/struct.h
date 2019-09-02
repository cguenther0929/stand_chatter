/******************************************************************************
*   FILE: struct.h
*
*   PURPOSE: Structures used throughout the code base are defined here.  
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
#ifndef __STRUCT_H_
#define __STRUCT_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"             //Project specific header file
#include "main.h"             //Project specific header file
#include "rfm69.h"

extern struct RFM {
    /* RADIO TRANSCEIVER RELATED */
    uint8_t         rfmmode;                        // Keep track of the radio's mode
    char            rxdata[16];                     // Receive buffer
} RFM;

extern struct GlobalInformation         //  Structure to pass information that is shared among modules
{
    uint16_t        tick20ms;                       // Increases every 20 mili-seconds
    uint16_t        tick100ms;                      // Used to keep track of passing deci-tick1000monds
    uint16_t        tick500ms;                      // Used to keep track of passing deci-tick1000monds
    uint16_t        tick1000ms;                     // Use this to tick tick1000monds -- might not be used for audio interface board
    
    bool            flag20ms;                       // Set every 10ms -- not supported in this application
    bool            flag100ms;                      // Set every 100ms
    bool            flag500ms;                      // Set every 500ms
    bool            flag1000ms;                     // Set every 1000ms


    /* STATE MACHINE RELATED */
    uint8_t         current_state;                  // Keep track of the radio's mode
    uint8_t         disp_seconds_ctr;               // For tracking how long the display is on
    uint8_t         msg_to_send;                    // Keep track of which message the user wishes to send
    uint8_t         splash_screen_tmr;              // Timer 
    bool            disp_tmr_active;                // Flag that determines whether or not display timer is running
    bool            splash_screen_tmr_active;       // Flag that determines whether or not splash screen timer is running 
    

    /* BUTTON RELATED */
    uint8_t         btn_lt_press_ctr;
    uint8_t         btn_rt_press_ctr;
    uint8_t         btn_both_press_ctr;
    bool            btn_lt_pressed;
    bool            btn_rt_pressed;
    bool            btn_both_pressed;


} GlobalInformation;


#endif