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
*FUNCTION: void SPIInit( void )
*PURPOSE: Initialize the SPI module
*PRECONDITION: SPI module not initialized
*POSTCONDITION: SPI module is now initialized
*RETURN: Nothing
********************************************************/
void SPIInit( void );

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
*FUNCTION: UCHAR PromStatus( void )
*PURPOSE: To read the status from the EEPROM status register
*PRECONDITION: None
*POSTCONDITION: Status of EEPROM is now known
*RETURN: Prom Status BYTE
********************************************************/
UCHAR PromStatus( void );

/********************************************************
*FUNCTION: void PromOn( void )
*PURPOSE: To apply power to the EEPROM
*PRECONDITION: EERPOM does not have power applied
*POSTCONDITION: EEPROM now has power applied
*RETURN: Nothing
********************************************************/
void PromOn( void );

/********************************************************
*FUNCTION: PromOff( void )
*PURPOSE: To removed power from the EEPROM
*PRECONDITION: EEPROM is powered on
*POSTCONDITION: EEPROM is powered off to save power
*RETURN: Nothing
********************************************************/
void PromOff( void );

/********************************************************
*FUNCTION: void EnableWrite( void )
*PURPOSE: Remove the write protection on the EEPROM
*PRECONDITION: User cannot write to the EEPROM
*POSTCONDITION: User can now write to the EEPROM
*RETURN: Nothing
********************************************************/
void EnableWrite( void );

/********************************************************
*FUNCTION: void WritePromStatus(UCHAR inst, UCHAR data)
*PURPOSE: To allow user to write to the EEPROM status register
*PRECONDITION: None
*POSTCONDITION: "data" now written to EEPROM status register
*RETURN: Nothing
********************************************************/
void WritePromStatus(UCHAR inst, UCHAR data);

/********************************************************
*FUNCTION: void DisableWrite( void )
*PURPOSE: To removed write access to the EEPROM
*PRECONDITION: User might be able to write to the EEPROM
*POSTCONDITION: User can no longer write to the EEPROM
*RETURN: Nothing
********************************************************/
void DisableWrite( void );

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