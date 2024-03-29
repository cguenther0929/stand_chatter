/******************************************************************************
*   FILE: struct.h
*
*   PURPOSE: Structures used throughout the code base are defined here.  
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/

#ifndef __STRUCT_H
#define __STRUCT_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"             //Project specific header file
#include "main.h"             //Project specific header file
#include "rfm69.h"

extern struct RFM {
    uint8_t         rfmmode;                        // Keep track of the radio's mode
    bool            valid_msg_received;                   // Flag set when first valid message received
    uint8_t         rcvd_msg_len;                   // Read size of message received
    char            rxdata[16];                     // Receive buffer
} RFM;

extern struct GlobalInformation         
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