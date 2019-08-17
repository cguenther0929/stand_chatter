/******************************************************************************
*   FILE: rfm69.c
*
*   PURPOSE: Source file for RFM69HCW LoRa radio module 
*
*   DEVICE: PIC18F66K22
*
*   COMPILER: Microchip XC8 v1.32
*
*   IDE: MPLAB X v3.45
*
*   TODO:  TBD
*
*   NOTE:
*
******************************************************************************/

/* 
CAN GET A LOT OF INFO FROM HERE:  https://github.com/LowPowerLab/RFM69/blob/master/RFM69.cpp
Check out the massive x by 2 config array on line 66 -- we might want to do the same thing

Here's a link directly to the radiohead git repo https://github.com/PaulStoffregen/RadioHead

*/
#include "rfm69.h"		    // Include the header file for this module

struct GlobalInformation gblinfo;

void RFMInitialize( void ) {

    // These are indexed by the values of ModemConfigChoice
    // Stored in flash (program) memory to save SRAM

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
    // We configure so that we can use the entire 256 byte FIFO for either receive
    // or transmit, but not both at the same time
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
    // RFMSPI2Write(RH_RF95_REG_1E_MODEM_CONFIG2, 0x70);        //TODO remove   // 0101 0100    -> Spreading factor of 7 (default, bits 7:4) + Hearder CRC set to on (bit 2)
    
    // Define Config 3 -> Set all to defaults 
    RFMSPI2Write(RH_RF95_REG_26_MODEM_CONFIG3, 0x00);           
    
    
    RFMsetPreambleLength(8); // Default is 8    
    
    // An innocuous ISM frequency, same as RF22's
    RFMsetFrequency(434.0);
    
    // Lowish power
    RFMsetTxPower(23,0);

    // TODO added by CJG.  Not sure if we need this
    RFMSPI2Write(RH_RF95_REG_12_IRQ_FLAGS, 0xFF); // Clear all IRQ flags

}

bool RFMsend(const char * data, uint8_t len)
{
    uint16_t i = 0;
    
    // if (len > RFM_MAX_MESSAGE_LEN) return false;
    if (len > RFM_MAX_MESSAGE_LEN) {
        DispSetContrast(50);
        DispRefresh();
        DispWriteString("MSG LENGTH");
        tick100msDelay(20); 
    } 

    // Make sure we not in TX mode.  After TX radio will automatically transition into standby mode
    while (RFMtxInProgress() && 500 > i++);   
    
    if(i >= 199){
        DispSetContrast(50);
        DispRefresh();
        DispWriteString("TIMOUT IN SEND");
        tick100msDelay(20);        

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

    RFMsetMode(RFM_MODE_TX);    // Start the transmitter
                                // when Tx is done, interruptHandler will fire and radio mode will return to STANDBY
    
    while(!RFM_IRQ_PIN);        // May want to make this interrupt driven for better operation

    tick100msDelay(10);         //TODO only in here because we can't truck previous while loop yet

    RFMsetMode(RFM_MODE_STANDBY);    // Start the transmitter


    return true;
}

void GetRxData(uint8_t * rxdata) {
    
    uint8_t len = RFMSPI2Read(RH_RF95_REG_13_RX_NB_BYTES);

    RFMSPI2Write(RH_RF95_REG_0D_FIFO_ADDR_PTR, RFMSPI2Read(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));
    RFMSPI2ReadBurst(RFM_REG_00_FIFO, rxdata, len);
	RFMSPI2Write(RH_RF95_REG_12_IRQ_FLAGS, 0xFF); // Clear all IRQ flags

    RFMsetMode(RFM_MODE_STANDBY);
}

bool ReceivedPacket( void ) {
    uint8_t temp_data;
    
    if (gblinfo.rfmmode == RFM_MODE_TX) return false;
    
    if (!(gblinfo.rfmmode == RFM_MODE_RX)) {
        RFMsetMode(RFM_MODE_RX);                            // If not already in RX mode, put in this mode
        return false;
    }

    temp_data = RFMSPI2Read(RH_RF95_REG_12_IRQ_FLAGS);      // Read IRQ flags
    
    // DispRefresh();                                      //TODO this chunk of code is for debug only
    // DispWriteString("IRQ REG");
    // DispLineTwo();
    // DispWrite8b(temp_data);
    // tick100msDelay(10);
    
    if(temp_data & RH_RF95_RX_DONE && (temp_data & RH_RF95_VALID_HEADER)) return true;        //RX Done bit and valid header bits set


    return false; 
}

void RFMsetMode(RadioOpMode mode) {
  
    if (mode == gblinfo.rfmmode)
        return;

    gblinfo.rfmmode = mode;
    
    switch (mode) {
        case RFM_MODE_TX:      //TODO temp updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_TX);
            // RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Table 63. p41. Bit [7:6] 01 == Interrupt on TxDone
            break;
        case RFM_MODE_RX:      //TODO temp updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_RXCONTINUOUS);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Table 63. p41 + Reg p99: Bit [7:6] 00 == Interrupt on RxDone
            break;
        // case RFM_MODE_STANDBY:     
            // RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
            break;
        case RFM_MODE_SLEEP:       //TODO temp note, updated   1111 1MMM
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_SLEEP);
            break;
        case RFM_MODE_STANDBY:        //TODO temp, updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, (RFMSPI2Read(RH_RF95_REG_01_OP_MODE) & 0xF8) | RH_RF95_MODE_STDBY);
	        // RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x00);            // Interrupt on RxDone  TODO do we want this here?
            // DispSetContrast(50);
            // DispRefresh();
            // DispWriteString("SET MODE IDLE");
            // tick100msDelay(20);  
            break;
        default:
            return;
    }

}


// Set one of the canned FSK Modem configs
// Returns true if its a valid choice
// bool RFMsetModemConfig(ModemConfigChoice index) {       //TODO temp note, new RH function 
    // if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
    //     return false;

    // ModemConfig cfg;
    // memcpy(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(ModemConfig));
    
    // RFMsetModemRegisters(&cfg);

    // return true;
// }

// Sets registers from a canned modem configuration structure
// void RFMsetModemRegisters(const ModemConfig* config) {
    // RFMSPI2Write(RH_RF95_REG_1D_MODEM_CONFIG1, config->reg_1d);
    // RFMSPI2Write(RH_RF95_REG_1E_MODEM_CONFIG2, config->reg_1e);
    // RFMSPI2Write(RH_RF95_REG_26_MODEM_CONFIG3, config->reg_26);
// }

bool RFMsetFrequency (float centre) {       //TODO, temp note, RH function
    
    //FSTEP = FXOSC / 2^19 so FSTEP = 32MHz / 524288 = 61.03515625
    uint32_t frf = (centre * 1000000.0) / 61.03515625;          // TODO can clean this up by replacing 61.** with RH_RF95_FSTEP
    // float frf = (centre * 1000000.0) / RH_RF95_FSTEP;
    // frf = (uint32_t)frf;                                //Typecast to uint32
    RFMSPI2Write(RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    RFMSPI2Write(RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
    RFMSPI2Write(RH_RF95_REG_08_FRF_LSB, frf & 0xff);

    return true;
}

void RFMsetPreambleLength(uint16_t bytes) {
    RFMSPI2Write(RH_RF95_REG_20_PREAMBLE_MSB, bytes >> 8);
    RFMSPI2Write(RH_RF95_REG_21_PREAMBLE_LSB, bytes & 0xff);
}

bool RFMtxInProgress( void ) {  //TODO this needs work! 
    if (gblinfo.rfmmode == RFM_MODE_TX) return true;
    return false;
}

void RFMsetTxPower(int8_t power, bool useRFO) {         //TODO, temp note, RH function
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

