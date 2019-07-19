/******************************************************************************
 *	FILE: disp.c
 *
 *	PURPOSE: Contains helper functions for communication with the 
 *          display.  Taret device: PIC18F66K22, advanced 8 bit MCU.
 *
 *	AUTHOR: Clinton Guenther
 *
 *	:TODO:	      
 *
******************************************************************************/

// #include "spi.h"		//Include the header file for this module
#include "disp.h"

/*  DEFINE THE STRUCTURE USED IN THIS FILE */
// struct GlobalInformation sysinfo;

void DisplayOn ( void ) {
    uint8_t i;
    disp_enable = DISPLAY_ON;
    for (i=0;i<255;i++);
    
    disp_reset = 0;
    for (i=0;i<255;i++);
    disp_reset = 1;

    DispSPI1Write(0x30);
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x30);
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x30);
    for (i=0;i<20;i++);
    
    DispSPI1Write(0x06);
    for (i=0;i<20;i++);
}

void DispCursorHome( void ) {  
    uint8_t i;
    
    disp_reset = 0;
    for (i=0;i<255;i++);
    disp_reset = 1;

    disp_reg_sel = 0;
    
    // DispSPI1Write();
}

/* END OF FILE */