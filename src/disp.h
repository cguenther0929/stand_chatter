/******************************************************************************
*	FILE: disp.h
*
*	PURPOSE: Header file for display communication
*			Target device: PIC18F66K22 (advanced 8 bit MCU).   
*
*	AUTHOR: Clinton Guenther
*
*	:TODO:	
*
******************************************************************************/

#ifndef _H_DISP_H
#define _H_DISP_H

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "config.h"     //Project specific header file
#include "config.h"
#include "spi.h"


/********************************************************
*FUNCTION: void DispCursorHome( void )
*PURPOSE: Set the displays cursor to the home position
*PRECONDITION: SPI module and disp GPIO shall be initialized
*POSTCONDITION: Display's cursor now in the home position
*RETURN: Nothing
********************************************************/
void DispCursorHome( void );

void DisplayOn ( void );

#endif
/* END OF FILE */