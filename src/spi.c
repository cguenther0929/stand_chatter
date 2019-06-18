/******************************************************************************
 *	FILE: spi.c
 *
 *	PURPOSE: Contains all of the routines for SPI communication
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

#include "spi.h"		//Include the header file for this module
#include "uart.h"
#include "struct.h"               //Temporary.  Remove for final build

/*  DEFINE THE STRUCTURE USED IN THIS FILE */
struct GlobalInformation sysinfo;



void SPIInit( void ){
    
    cs = 1; //Make sure part is not enabled
    SSPEN = 0;

    /* CONFIGURE SSPSTAT REGISTER */
    CKE = 1;    //Transmit occurs on transition from active to idle close state.
                

    /* CONFIGURE SSPCON1 */
    SSPM3 = 0;       
    SSPM2 = 0;          //See the SSPCON1 register on page 233
    SSPM1 = 1;
    SSPM0 = 1;      //0011 will yield a BAUD rate of TMR2/2
  
    //CKP is fine by default

    /* CONFIGURE SSPCON2 and SSPCON3 */
    // Does not apply for SPI master communication

    /* CONFIGURE APFCON */
    // By default, the SS bit is mapped to RA5 (which is what we use)

    /* SET THE BAUD RATE */
    //SSPADD = 0xFF;      //Currently we are using time 2 for this

     SSPEN = 1;          //Enable SPI communication
}


void SPIWrite(UCHAR inst, UCHAR addr, UCHAR data) {  //Working!!

    UINT i;                             //Use as a general variable
    UCHAR trash;        //Use this to read the received data (should be done)
       
    cs = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst;                      //Send the instruction
      
    while(BF != 1);      //Wait until data is in the buffer (received)
    trash = SSPBUF;     //BF is cleared by simply  reading received data from SSBUF

    /* SEND THE ADDRESS */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = addr;                      //Send the instruction
   
    while(BF != 1);      //Wait until data is in the buffer (received)
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF
    for(i = 0; i<200 ; i++);       //Add a little delay

    /* SEND THE DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = data;                      //Send the instruction
    
    while(BF != 1);      //Wait until data is in the buffer (received)
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF

    for(i = 0; i<spidelay ; i++);       //Add a little delay
    cs = 1;     //Disable the chip
}

UCHAR SPIRead(UCHAR inst, UCHAR addr) { //I think this function is good
    UINT i;                             //Use as a general variable
    UCHAR data;        //Use this to read the received data (should be done)
    
    cs = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst;                      //Send the instruction

    while(BF != 1);      //Wait until data is in the buffer
    data = SSPBUF; //BF is cleared by simply  reading received data from SSBUF
    for(i = 0; i<200 ; i++);       //Add a little delay

    /* SEND THE ADDRESS */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = addr;                      //Send the instruction
    while(BF != 1);      //Wait until data is in the buffer
    data = SSPBUF;  //BF is cleared by simply  reading received data from SSBUF
    for(i = 0; i<200 ; i++);       //Add a little delay

    /* GRAB FIRST BYTE OF DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = 0x55;                      //Transmit garbage to grab data
    while(BF != 1);      //Wait until data is in the buffer
    sysinfo.gdcalstate = SSPBUF; //This is data the user is interested in

    /* GRAB SECOND BYTE OF DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = 0x55;                      //Transmit garbage to grab data
    while(BF != 1);      //Wait until data is in the buffer
    sysinfo.trcalstate = SSPBUF; //This is data the user is interested in

    /* GRAB THIRD BYTE OF DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = 0x55;                      //Transmit garbage to grab data
    while(BF != 1);      //Wait until data is in the buffer
    sysinfo.parkdistance = SSPBUF; //This is data the user is interested in

    /* GRAB FOURTH BYTE OF DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = 0x55;                      //Transmit garbage to grab data
    while(BF != 1);      //Wait until data is in the buffer
    sysinfo.gddistance = SSPBUF; //This is data the user is interested in

    /* GRAB FIFTH BYTE OF DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = 0x55;                      //Transmit garbage to grab data
    while(BF != 1);      //Wait until data is in the buffer
    sysinfo.stopdistance = SSPBUF; //This is data the user is interested in
   
    for(i = 0; i<spidelay ; i++);       //Add a little delay
    cs = 1;     //Diable the chip

    return data;    //Return the 8bits of data

}

void WritePromStatus(UCHAR inst, UCHAR data) {
    UINT i;                             //Use as a general variable
    UCHAR trash;        //Use this to read the received data (should be done)

    cs = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;               //Datasheet defines that we should clear these values
    SSPOV=0;
    SSPBUF = inst;                      //Send the instruction
    
    while(BF != 1);      //Wait until data is in the buffer
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF
    for(i = 0; i<200 ; i++);       //Add a little delay

    /* SEND THE DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = data;                      //Send the data
  
    while(BF != 1);      //Wait until data is in the buffer
    WCOL = 0;
    SSPOV=0;
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF
   
    for(i = 0; i<spidelay ; i++);       //Add a little delay
    cs = 1;     //Disable the chip
}

UCHAR PromStatus( void ) { //I think this function is good
    UINT i;                             //Use as a general variable
    UCHAR data;        //Use this to read the received data (should be done)
        
    cs = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst_RDSR;                      //Send the instruction

    while(BF != 1);      //Wait until data is in the buffer
    data = SSPBUF; //BF is cleared by simply  reading received data from SSBUF
    
    /* SEND JUNK TO GET DATA */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = 0x55;                      //Send the instruction
    
    while(BF != 1);      //Wait until data is in the buffer
    data = SSPBUF;  //BF is cleared by simply  reading received data from SSBUF
    
    for(i = 0; i<spidelay ; i++);       //Add a little delay
    cs = 1;     //Diable the chip

    return data;    //Return the 8bits of data
}

void DisableWrite( void ) {
    UINT i;             //Used as a counter
    UCHAR trash;        //Used to empty the SPI buffer
    cs = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst_WRDI;                 //Send the instruction

    while(BF != 1);      //Wait until data is in the buffer
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF

    for(i = 0; i<spidelay ; i++);       //Add a little delay
    cs = 1;     //Disable the chip
}

void EnableWrite( void ) {
    UINT i;             //Used as a counter
    UCHAR trash;        //Used to empty the SPI buffer
    cs = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst_WREN;                 //Send the instruction

    while(BF != 1);      //Wait until data is in the buffer
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF

    for(i = 0; i<spidelay ; i++);       //Add a little delay
    cs = 1;     //Disable the chip
}

void PromOn(void) {
    prompwr = 1; //Pin to provide power
}

void PromOff(void) {
    prompwr = 0;    //Pin to provide power  
}

void SPIBurnDelay(void) {
    UINT i;        //Used as a counter
    for(i=0;i<700;i++);  //Add small delay
}
/* END OF FILE */