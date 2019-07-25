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
#include <stdio.h>
#include "struct.h"
#include "config.h"     //Project specific header file
#include "config.h"
#include "spi.h"
#include "main.h"

#define LF			1
#define CR			2

/********************************************************
*FUNCTION: void DispCursorHome( void )
*PURPOSE: Set the displays cursor to the home position
*PRECONDITION: SPI module and disp GPIO shall be initialized
*POSTCONDITION: Display's cursor now in the home position
*RETURN: Nothing
********************************************************/
void DispCursorHome( void );

void DispClear( void );

void DispInit ( void );

void DispWriteChar (uint8_t c);

void DispWriteString(const char * y);

void DispLineTwo (void );

void DispSetContrast(uint8_t percentage);

void DispRefresh( void );

void DispWriteFloat (float number);

void DispWrite8b (uint8_t number);

#endif
/* END OF FILE */