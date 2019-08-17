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
// #include "struct.h"               //Temporary.  Remove for final build

/*  DEFINE THE STRUCTURE USED IN THIS FILE */
struct GlobalInformation sysinfo;

void SPI1Init( void ){  
    
    /* CONFIGURE SSP1STAT REGISTER */
    SMP1 = 0;           // Input data is sampled at the middle of data output time
    CKE1 = 1;           // Transmit occurs on the transition from active to idle clock state 
    
    /* CONFIGURE SSP1CON1 p282 */
    CKP1 = 0;                            // Idle clock state is low (default value)
    // SSPCON1bits.SSPM = 0x0;             // 0b0000 SPI Master mode: clock = FOSC/4
    SSPCON1bits.SSPM = 0x1;          // 0b0001 SPI Master mode: clock = FOSC/16
    // SSPCON1bits.SSPM = 0x2;          // 0b0010 SPI Master mode: clock = FOSC/64
    // SSPCON1bits.SSPM = 0x3;          // 0b0011 Master mode: clock = TMR2 output/2
    // SSPCON1bits.SSPM = 0xA;          // 0b1001 SPI Master mode: clock = FOSC/8

    SSPEN1 = 1;          // Enable SPI communication via bit in SSP1CON1 register p283
}

void SPI2Init( void ){ 
    
    /* CONFIGURE SSP2STAT REGISTER */
    SMP2 = 0;           // Input data is sampled at the middle of data output time
    CKE2 = 1;           // Transmit occurs on the transition from active to idle clock state 
    
    /* CONFIGURE SSP2CON1 */
    CKP2 = 0;                            // Idle clock state is low (default value)
    // SSP2CON1bits.SSPM = 0x0;             // 0b0000 SPI Master mode: clock = FOSC/4
    SSP2CON1bits.SSPM = 0x1;          // 0b0001 SPI Master mode: clock = FOSC/16
    // SSP2CON1bits.SSPM = 0x2;          // 0b0010 SPI Master mode: clock = FOSC/64
    // SSP2CON1bits.SSPM = 0x3;          // 0b0011 Master mode: clock = TMR2 output/2
    // SSP2CON1bits.SSPM = 0xA;          // 0b1001 SPI Master mode: clock = FOSC/8

    SSPEN2 = 1;          // Enable SPI communication via bit in SSP1CON1 register p283
}

void RFMSPI2Write(uint8_t addr, uint8_t data) {  
    uint8_t i;                          // Use as a general variable
    uint8_t rcvd_data;            // Use this to read the received data (should be done)
       
    RFM_SPI_CS = 0;                    // Slave select low
    for(i = 0; i < spidelay ; i++);       // Add a little delay

    rcvd_data = SSP2BUF;                // First clean out the buffer to clean up
    
    /* SEND THE ADDRESS */
    WCOL2 = 0;
    SSPOV2 = 0;
    SSP2BUF = (addr | 0x80);            // Send the instruction (0x80 for write and 0x00 for read)
   
    while(BF2 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP2BUF;                // BF2 is cleared by simply  reading received data from SSBUF
    // for(i = 0; i<20 ; i++);          // Add a little delay

    /* SEND THE DATA */
    WCOL2 = 0;
    SSPOV2 = 0;
    SSP2BUF = data;                     // Send the instruction
    
    while(BF2 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP2BUF;                // BF2 is cleared by simply  reading received data from SSBUF

    for(i = 0; i < spidelay ; i++);       // Add a little delay
    RFM_SPI_CS = 1;                    // Disable the chip
}

void RFMSPI2WriteBurst(uint8_t addr, const char * data, uint8_t len) {
    uint8_t i;                          // Use as a general variable
    uint8_t rcvd_data;                  // Use this to read the received data (should be done)
       
    RFM_SPI_CS = 0;                     // Slave select low
    for(i = 0; i<spidelay ; i++);       // Add a little delay

    rcvd_data = SSP2BUF;                // First clean out the buffer to clean up
    
    /* SEND THE ADDRESS */
    WCOL2 = 0;
    SSPOV2 = 0;
    SSP2BUF = (addr | 0x80);            // Send the instruction (0x80 for write and 0x00 for read)
   
    while(BF2 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP2BUF;                // BF2 is cleared by simply  reading received data from SSBUF

    /* SEND THE BURST OF DATA */
    while (len--) {
        WCOL2 = 0;
        SSPOV2 = 0;
        SSP2BUF = *data++;              // Send the instruction
        while(BF2 != 1);                // Wait until data is in the buffer (received)
        rcvd_data = SSP2BUF;            // BF2 is cleared by simply  reading received data from SSBUF
    }
    
    for(i = 0; i < spidelay ; i++);     // Add a little delay
    RFM_SPI_CS = 1;                     // Disable the chip
}

uint8_t RFMSPI2Read(uint8_t addr) {  
    
    uint8_t i;                          // Use as a general variable
    uint8_t rcvd_data;                  // Use this to read the received data (should be done)
       
    RFM_SPI_CS = 0;                     // Slave select low
    for(i = 0; i<spidelay ; i++);       // Add a little delay

    /* SEND THE ADDRESS */
    rcvd_data = SSP2BUF;                // First clean out the buffer to clean up
    WCOL2 = 0;
    SSPOV2 = 0;
    SSP2BUF = (addr & 0x7F);            // Send the instruction (0x80 for write and 0x00 for read)
   
    while(BF2 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP2BUF;                // BF2 is cleared by simply  reading received data from SSBUF
    // for(i = 0; i<20 ; i++);          // Add a little delay  TODO can we remove this delay line?

    /* SEND GARBAGE TO GRAB THE DATA */
    SSP2BUF = 0x55;                     // Send the instruction
    
    while(BF2 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP2BUF;                // BF2 is cleared by simply  reading received data from SSBUF

    for(i = 0; i < spidelay ; i++);     // Add a little delay
    RFM_SPI_CS = 1;                     // Disable the chip

    return (rcvd_data);
}

uint8_t RFMSPI2ReadBurst(uint8_t addr, uint8_t * data, uint8_t len) {  
    
    uint8_t i;                          // Use as a general variable
    uint8_t rcvd_data;                  // Use this to read the received data (should be done)
       
    RFM_SPI_CS = 0;                     // Slave select low
    for(i = 0; i<spidelay ; i++);       // Add a little delay

    /* SEND THE ADDRESS */
    rcvd_data = SSP2BUF;                // First clean out the buffer to clean up
    WCOL2 = 0;
    SSPOV2 = 0;
    SSP2BUF = (addr & 0x7F);            // Send the instruction (0x80 for write and 0x00 for read)
   
    while(BF2 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP2BUF;                // BF2 is cleared by simply  reading received data from SSBUF

    /* MUST EXTRACT HEADER INFO FIRST */
    i = 4;
    while(i--){
        WCOL2 = 0;
        SSPOV2 = 0;
        SSP2BUF = 0x55;                 // Send the instruction
        while(BF2 != 1);                // Wait until data is in the buffer (received)
        rcvd_data = SSP2BUF;              // BF2 is cleared by simply  reading received data from SSBUF
    }


    /* GRAB PACKET DATA  */
    while (len--){
        WCOL2 = 0;
        SSPOV2 = 0;
        SSP2BUF = 0x55;                 // Send the instruction
        while(BF2 != 1);                // Wait until data is in the buffer (received)
        *data++ = SSP2BUF;              // BF2 is cleared by simply  reading received data from SSBUF
    }
    
    for(i = 0; i < spidelay ; i++);     // Add a little delay
    RFM_SPI_CS = 1;                     // Disable the chip
}

void DispSPI1Write(uint8_t data) {  // TODO need to test 

    uint8_t i;                          // Use as a general variable
    uint8_t rcvd_data;            // Use this to read the received data (should be done)

    disp_spi_cs = 0;                    // Slave select low
    for(i = 0; i < spidelay ; i++);       // Add a little delay
    rcvd_data = SSP1BUF;                // Read the buffer to assure it is emply and BF is cleared

    /* SEND DATA */
    WCOL1 = 0;
    SSPOV1 = 0;
    SSP1BUF = data;                     // Send the instruction
   
    while(BF1 != 1);                    // Wait until data is in the buffer (received)
    rcvd_data = SSP1BUF;                // BF1 is cleared by simply  reading received data from SSBUF

    for(i = 0; i<spidelay ; i++);       //Add a little delay
    disp_spi_cs = 1;                    // Slave select low
}

uint8_t SPI1Read(uint8_t addr) { 
    uint8_t i;                             //Use as a general variable
    uint8_t data;        //Use this to read the received data (should be done)
    
    RFM_SPI_CS = 0;                         // Slave select low
    for(i = 0; i < spidelay ; i++);         // Add a little delay

    /* SEND THE ADDRESS */
    WCOL1 = 0;
    SSPOV1 = 0;
    SSP1BUF = addr;                 // Send the instruction
    while(BF1 != 1);                // Wait until data is in the buffer
    data = SSP1BUF;                 // BF1 is cleared by simply  reading received data from SSBUF
    for(i = 0; i<200 ; i++);        // Add a little delay

    /* GRAB FIRST BYTE OF DATA */
    WCOL1 = 0;
    SSPOV1=0;
    SSP1BUF = 0x55;                 // Transmit garbage to grab data
    while(BF1 != 1);                // Wait until data is in the buffer
    data = SSP1BUF;
   
    for(i = 0; i<spidelay ; i++);       // Add a little delay
    RFM_SPI_CS = 1;                     // Diable the chip

    return data;    //Return the 8bits of data

}

void SPIBurnDelay(void) {
    uint8_t i;        //Used as a counter
    for(i=0;i<255;i++);  //Add small delay
}
/* END OF FILE */