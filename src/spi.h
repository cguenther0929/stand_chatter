/******************************************************************************
*	FILE: spi.h
*
*	PURPOSE: Header file for SPI communication
*			Target device: PIC16F15xx 8 bit MCU.   
*
*	AUTHOR: Clinton Guenther
*
*	:TODO:	
*
*	REVISION HISTORY:
*	2/3/13 REV 0.01 - Original file created by C. Guenther
*
******************************************************************************/

#ifndef __SPI_H_
#define __SPI_H_

// #include "htc.h"
#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "isr.h"
#include "config.h"     //Project specific header file
#include "timer.h"
#include "rfm69.h"
#include "main.h"

#define spidelay        50             //Define a small delay


/********************************************************
*FUNCTION: void SPI1Init( void )
*PURPOSE: Initialize the SPI module
*PRECONDITION: SPI module not initialized
*POSTCONDITION: SPI module is now initialized
*RETURN: Nothing
********************************************************/
void SPI1Init( void );

void SPI2Init( void );  //TODO need to comment

void RFMSPI2Write(uint8_t addr, uint8_t data);

void RFMSPI2WriteBurst(uint8_t addr, const char * data, uint8_t len);

void RFMSPI2ReadBurst(uint8_t addr, uint8_t * data, uint8_t len);

uint8_t RFMSPI2Read(uint8_t addr);      // TODO need to comment

void DispSPI1Write(uint8_t data);        //TODO need to comment

/********************************************************
*FUNCTION: uint8_t SPI1Read(uint8_t addr)
*PURPOSE: Read a byte of data
*PRECONDITION: None
*POSTCONDITION: One byte of data read over SPI interface
*RETURN: Nothing
********************************************************/
uint8_t SPI1Read(uint8_t addr);

/********************************************************
*FUNCTION: void SPIBurnDelay(void)
*PURPOSE: Give chip time to burn data to memory
*PRECONDITION: None
*POSTCONDITION: Time alloted so the EEPROM can burn information 
            to memory
*RETURN: Nothing
********************************************************/
void SPIBurnDelay(void);

#endif
/* END OF FILE */