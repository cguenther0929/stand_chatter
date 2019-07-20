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

#ifndef _H_SPI_H
#define _H_SPI_H

// #include "htc.h"
#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "isr.h"
#include "config.h"     //Project specific header file
#include "timer.h"
#include "config.h"
#include "rfm69.h"

/* DEFINE THE EEPROM INSTRUCTIONS */
// #define promread        0x03            //Clock this in to read the chip
// #define promwrite       0x02            //Clock this in to write
// #define disablewrite    0x04            //Disable write operations
// #define enablewrite     0x06            //Enable write operations
// #define statusreg       0x05            //Instruction to read status reg
// #define statuswrite     0x01            //Instruction to write status reg

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

/********************************************************
*FUNCTION: void SPI1Write(uint8_t addr, uint8_t data)
*PURPOSE: To write data out the SPI port
*PRECONDITION: None
*POSTCONDITION: Data is sent out the SPI port
*RETURN: Nothing
********************************************************/
void SPI1Write(uint8_t addr, uint8_t data);


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