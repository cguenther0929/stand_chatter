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

#include "type.h"
#include "htc.h"
#include "config.h"

/* DEFINE THE EEPROM INSTRUCTIONS */
#define promread        0x03            //Clock this in to read the chip
#define promwrite       0x02            //Clock this in to write
#define disablewrite    0x04            //Disable write operations
#define enablewrite     0x06            //Enable write operations
#define statusreg       0x05            //Instruction to read status reg
#define statuswrite     0x01            //Instruction to write status reg

#define spidelay        50             //Define a small delay


/********************************************************
*FUNCTION: void SPI1Init( void )
*PURPOSE: Initialize the SPI module
*PRECONDITION: SPI module not initialized
*POSTCONDITION: SPI module is now initialized
*RETURN: Nothing
********************************************************/
void SPI1Init( void );

/********************************************************
*FUNCTION: void SPIWrite(UCHAR inst, UCHAR addr, UCHAR data)
*PURPOSE: To write data out the SPI port
*PRECONDITION: None
*POSTCONDITION: Data is sent out the SPI port
*RETURN: Nothing
********************************************************/
void SPIWrite(UCHAR inst, UCHAR addr, UCHAR data);

/********************************************************
*FUNCTION: UCHAR SPIRead(UCHAR inst, UCHAR addr)
*PURPOSE: To perform a sequential read of the EEPROM
        EEPROM only seems to work if you perform a sequential read
*PRECONDITION: None
*POSTCONDITION: Data is read from the EEPROM
*RETURN: Nothing
********************************************************/
UCHAR SPIRead(UCHAR inst, UCHAR addr);

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