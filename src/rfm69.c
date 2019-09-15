/******************************************************************************
*   FILE: rfm69.c
*
*   PURPOSE: Source file for RFM69HCW LoRa radio module 
*
*   TODO: 
*
*   NOTE:
*
******************************************************************************/

#include "rfm69.h"		    // Include the header file for this module

struct GlobalInformation gblinfo;
struct RFM rfm;

void RFMInitialize( void ) {

    RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE);

    tick100msDelay(1);                                  // Wait for sleep mode to take over from say, CAD
    
    // Check we are in sleep mode, with LORA set
    if (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) != (RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE))
    {
        DispSetContrast(50);
        DispRefresh();
        DispWriteString("NO DEVICE DET");
        tick100msDelay(20);        
    }

    // Set up FIFO
    // Set both to zero if we wish to use the entire 256 byte FIFO for both
    // transmit and receive operations.  However, going to set to defult locations
    // as this application need not require a huge FIFO
    RFMSPI2Write(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR, 0);
    // RFMSPI2Write(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0);
    RFMSPI2Write(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0x80);

    RFMsetMode(RFM_MODE_STANDBY);

    // Set up default configuration
    // No Sync Words in LORA mode.

    // Define Config 1 -> Signal bandwidth of 41.7kHz (default, bits 7:4) + Error Coding rate of 4/5 (default, bits 3:1) + Explicit Header Mode (default, bit 0)
    RFMSPI2Write(RH_RF95_REG_1D_MODEM_CONFIG1, 0x72);           
    
    // Define Config 2  -> Spreading factor of 7 (default, bits 7:4) + Hearder CRC set to on (bit 2)
    RFMSPI2Write(RH_RF95_REG_1E_MODEM_CONFIG2, 0x74);
    
    // Define Config 3 -> Set all to defaults 
    RFMSPI2Write(RH_RF95_REG_26_MODEM_CONFIG3, 0x00);           
    
    RFMsetPreambleLength(8); // Default is 8    
    
    // An innocuous ISM frequency, same as RF22's
    // This gets changed before entering the main application
    // Set to this for now, as it was done this way in an example I followed
    RFMsetFrequency(434.0);
    
    // Lowish power
    RFMsetTxPower(23,0);

    // Just a precaution, but clear IRQ flags by writing to register 
    RFMSPI2Write(RH_RF95_REG_12_IRQ_FLAGS, 0xFF); 

}

bool RFMsend(const char * data, uint8_t len)
{
    uint16_t i = 0;
    uint8_t temp_data;

    
    if (len > RFM_MAX_MESSAGE_LEN) {
        DispSetContrast(50);
        DispRefresh();
        DispWriteString("MSG LENGTH");
        tick100msDelay(20); 
        return false;
    } 

    // Make sure we not in TX mode.  
    // Note, After TX op, radio will automatically transition into standby
    while (RFMtxInProgress() && 500 > i++);   
    
    if(i >= 500){
        DispSetContrast(50);
        DispRefresh();
        DispWriteString("TIMOUT IN SEND");
        tick100msDelay(20);  
        return false;      
    }
    
    RFMsetMode(RFM_MODE_STANDBY);

    // Position at the beginning of the FIFO
    RFMSPI2Write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
    
    // The headers
    RFMSPI2Write(RFM_REG_00_FIFO, RFM_BROADCAST_ADDRESS);       // Enter destination address
    RFMSPI2Write(RFM_REG_00_FIFO, RFM_BROADCAST_ADDRESS);       // Source address
    RFMSPI2Write(RFM_REG_00_FIFO, 0x00);                        // Header ID
    RFMSPI2Write(RFM_REG_00_FIFO, 0x00);                        // Header flags
    
    // The message data
    RFMSPI2WriteBurst(RFM_REG_00_FIFO, data, len);
    
    RFMSPI2Write(RH_RF95_REG_22_TX_PALD_LG, len + RFM_HEADER_LEN);

    RFMsetMode(RFM_MODE_TX);        // Start the transmitter
    tick100msDelay(5);              // Likely not needed, but small delay for mode settle
    
    // while(!RFM_IRQ_PIN);        // This pin doesn't function as described in the datasheet 

    i = 0;
    while(true && 20 > i++){
        temp_data = RFMSPI2Read(RH_RF95_REG_12_IRQ_FLAGS);      // Read IRQ flags
        if(temp_data & RH_RF95_TX_DONE) break;
        tick100msDelay(1);         
    }
    
    
    if(i >= 20){
        DispSetContrast(50);
        DispRefresh();
        DispWriteString("TMOU FOR TXDONE");
        tick20msDelay(20);        

    }
    
    RFMsetMode(RFM_MODE_STANDBY);    // Just a precaution, as the module should transition automatically 

	RFMSPI2Write(RH_RF95_REG_12_IRQ_FLAGS, 0xFF); // Clear all IRQ flags

    return true;
}

void GetRxData( void ) {
    
    uint8_t len = RFMSPI2Read(RH_RF95_REG_13_RX_NB_BYTES);

    RFMSPI2Write(RH_RF95_REG_0D_FIFO_ADDR_PTR, RFMSPI2Read(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
    RFMSPI2ReadBurst(RFM_REG_00_FIFO, rfm.rxdata, len);
    RFMSPI2Write(RH_RF95_REG_12_IRQ_FLAGS, 0xFF); // Clear all IRQ flags
    
    rfm.rcvd_msg_len = GetMsgLen(rfm.rxdata);
    
    if(rfm.rcvd_msg_len <= 16) {
        rfm.valid_msg_received = true;
    }
    else{
        strcpy(rfm.rxdata,'\0');
        rfm.valid_msg_received = false;
    }
    
    RFMsetMode(RFM_MODE_STANDBY);
}

bool ReceivedPacket( void ) {
    uint8_t temp_data;
    
    if (rfm.rfmmode == RFM_MODE_TX) return false;
    
    if (!(rfm.rfmmode == RFM_MODE_RX)) {
        RFMsetMode(RFM_MODE_RX);                            // If not already in RX mode, put in this mode
        return false;
    }

    temp_data = RFMSPI2Read(RH_RF95_REG_12_IRQ_FLAGS);      // Read IRQ flags
    
    if(temp_data & RH_RF95_RX_DONE && (temp_data & RH_RF95_VALID_HEADER)) return true;        //RX Done bit and valid header bits set

    return false; 
}

void RFMsetMode(uint8_t mode) {
  
    if (mode == rfm.rfmmode)
        return;

    rfm.rfmmode = mode;
    
    switch (mode) {
        case RFM_MODE_TX:           // Mode mask register: 1111 1MMM
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_TX);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Table 63. p41. Bit [7:6] 01 == Interrupt on TxDone
            break;
        case RFM_MODE_RX:           // Mode mask register: 1111 1MMM
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_RXCONTINUOUS);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Table 63. p41 + Reg p99: Bit [7:6] 00 == Interrupt on RxDone
            break;
        case RFM_MODE_SLEEP:       // Mode mask register: 1111 1MMM
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_SLEEP);
            break;
        case RFM_MODE_STANDBY:      // Mode mask register: 1111 1MMM
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_STDBY);
            break;
        default:
            return;
    }

}


bool RFMsetFrequency (float centre) {       
    
    uint32_t frf = (centre * 1000000.0) / RH_RF95_FSTEP;          // TODO can clean this up by replacing 61.** with RH_RF95_FSTEP
    RFMSPI2Write(RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    RFMSPI2Write(RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
    RFMSPI2Write(RH_RF95_REG_08_FRF_LSB, frf & 0xff);

    return true;
}

void RFMsetPreambleLength(uint16_t bytes) {
    RFMSPI2Write(RH_RF95_REG_20_PREAMBLE_MSB, bytes >> 8);
    RFMSPI2Write(RH_RF95_REG_21_PREAMBLE_LSB, bytes & 0xff);
}

bool RFMtxInProgress( void ) { 
    if (rfm.rfmmode == RFM_MODE_TX) return true;
    return false;
}

void RFMsetTxPower(int8_t power, bool useRFO) {         
    // Sigh, different behaviours depending on whther the module use PA_BOOST or the RFO pin
    // for the transmitter output
    if (useRFO) {
	    if (power > 14)
	        power = 14;
	    if (power < -1)
	        power = -1;
	    RFMSPI2Write(RH_RF95_REG_09_PA_CONFIG, RH_RF95_MAX_POWER | (power + 1));
    }
    else {
	    if (power > 23)
	        power = 23;
	    if (power < 5)
	        power = 5;

	    // For RH_RF95_PA_DAC_ENABLE, manual says '+20dBm on PA_BOOST when OutputPower=0xf'
	    // RH_RF95_PA_DAC_ENABLE actually adds about 3dBm to all power levels. We will us it
	    // for 21, 22 and 23dBm
	    if (power > 20)
	    {
	        RFMSPI2Write(RH_RF95_REG_4D_PA_DAC, RH_RF95_PA_DAC_ENABLE);
	        power -= 3;
	    }
	    else
	    {
	        RFMSPI2Write(RH_RF95_REG_4D_PA_DAC, RH_RF95_PA_DAC_DISABLE);
	    }

	    // RFM95/96/97/98 does not have RFO pins connected to anything. Only PA_BOOST
	    // pin is connected, so must use PA_BOOST
	    // Pout = 2 + OutputPower.
	    // The documentation is pretty confusing on this topic: PaSelect says the max power is 20dBm,
	    // but OutputPower claims it would be 17dBm.
	    // My measurements show 20dBm is correct
	    RFMSPI2Write(RH_RF95_REG_09_PA_CONFIG, RH_RF95_PA_SELECT | (power-5));
    }
}

uint8_t GetMsgLen(const char * msg) {
    uint8_t ctr     = 0;

    while(*msg != '\0'){
        ctr++;
        msg++;                           //Increment the pointer memory address
    }

    return(ctr);
}

