/******************************************************************************
 *	FILE: spi.c
 *
 *	PURPOSE: Contains all of the routines for SPI communication
 *			Target device: PIC18F66K22, advanced 8 bit MCU.
 *
 *	AUTHOR: Clinton Guenther
 *
 *	:TODO:	      
 *
******************************************************************************/

#include "spi.h"		//Include the header file for this module
#include "struct.h"               //Temporary.  Remove for final build

/*  DEFINE THE STRUCTURE USED IN THIS FILE */
struct GlobalInformation sysinfo;



void SPI1Init( void ){  // TODO copy this into CPI2Init
    
    /* CONFIGURE SSP1STAT REGISTER */
    SMP1 = 0;           // Input data is sampled at the middle of data output time
    CKE1 = 1;           // Transmit occurs on the transition from active to idle clock state 
    
    /* CONFIGURE SSP1CON1 p282 */
    CKP1 = 0                            // Idle clock state is low (default value)
    SSPCON1bits.SSPM = 0x0;             // 0b0000 SPI Master mode: clock = FOSC/4
    // SSPCON1bits.SSPM = 0x1;          // 0b0001 SPI Master mode: clock = FOSC/16
    // SSPCON1bits.SSPM = 0x2;          // 0b0010 SPI Master mode: clock = FOSC/64
    // SSPCON1bits.SSPM = 0x3;          // 0b0011 Master mode: clock = TMR2 output/2
    // SSPCON1bits.SSPM = 0xA;          // 0b1001 SPI Master mode: clock = FOSC/8

    SSPEN1 = 1;          // Enable SPI communication via bit in SSP1CON1 register p283
}

// void SPI2Init( void ){
    
//     RF69_SPI_CS = 1;        //Make sure part is not enabled
//     SSPEN = 0;

//     /* CONFIGURE SSPSTAT REGISTER */
//     CKE = 1;    //Transmit occurs on transition from active to idle close state.
                

//     /* CONFIGURE SSPCON1 */
//     SSPM3 = 0;       
//     SSPM2 = 0;          //See the SSPCON1 register on page 233
//     SSPM1 = 1;
//     SSPM0 = 1;      //0011 will yield a BAUD rate of TMR2/2
  
//     //CKP is fine by default

//     /* CONFIGURE SSPCON2 and SSPCON3 */
//     // Does not apply for SPI master communication

//     /* CONFIGURE APFCON */
//     // By default, the SS bit is mapped to RA5 (which is what we use)

//     /* SET THE BAUD RATE */
//     //SSPADD = 0xFF;      //Currently we are using timer 2 for this

//      SSPEN = 1;          //Enable SPI communication
// }


void SPIWrite(UCHAR inst, UCHAR addr, UCHAR data) {  // Working!!

    UINT i;                             //Use as a general variable
    UCHAR trash;                        //Use this to read the received data (should be done)
       
    RF69_SPI_CS = 0;                    //Slave select low
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
    RF69_SPI_CS = 1;     //Disable the chip
}

UCHAR SPIRead(UCHAR inst, UCHAR addr) { //I think this function is good
    UINT i;                             //Use as a general variable
    UCHAR data;        //Use this to read the received data (should be done)
    
    RF69_SPI_CS = 0;             //Slave select low
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
    RF69_SPI_CS = 1;     //Diable the chip

    return data;    //Return the 8bits of data

}

void WritePromStatus(UCHAR inst, UCHAR data) {
    UINT i;                             //Use as a general variable
    UCHAR trash;        //Use this to read the received data (should be done)

    RF69_SPI_CS = 0;             //Slave select low
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
    RF69_SPI_CS = 1;     //Disable the chip
}

UCHAR PromStatus( void ) { //I think this function is good
    UINT i;                             //Use as a general variable
    UCHAR data;        //Use this to read the received data (should be done)
        
    RF69_SPI_CS = 0;             //Slave select low
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
    RF69_SPI_CS = 1;     //Diable the chip

    return data;    //Return the 8bits of data
}

void DisableWrite( void ) {
    UINT i;             //Used as a counter
    UCHAR trash;        //Used to empty the SPI buffer
    RF69_SPI_CS = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst_WRDI;                 //Send the instruction

    while(BF != 1);      //Wait until data is in the buffer
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF

    for(i = 0; i<spidelay ; i++);       //Add a little delay
    RF69_SPI_CS = 1;     //Disable the chip
}

void EnableWrite( void ) {
    UINT i;             //Used as a counter
    UCHAR trash;        //Used to empty the SPI buffer
    RF69_SPI_CS = 0;             //Slave select low
    for(i = 0; i<spidelay ; i++);       //Add a little delay

    /* SEND THE INSTRUCTION */
    WCOL = 0;
    SSPOV=0;
    SSPBUF = inst_WREN;                 //Send the instruction

    while(BF != 1);      //Wait until data is in the buffer
    trash = SSPBUF; //BF is cleared by simply  reading received data from SSBUF

    for(i = 0; i<spidelay ; i++);       //Add a little delay
    RF69_SPI_CS = 1;     //Disable the chip
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