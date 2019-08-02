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

static const MODEM_CONFIG_TABLE[] =
{
    //  1d,     1e,      26
    { 0x72,   0x74,    0x00}, // Bw125Cr45Sf128 (the chip default)
    { 0x92,   0x74,    0x00}, // Bw500Cr45Sf128
    { 0x48,   0x94,    0x00}, // Bw31_25Cr48Sf512
    { 0x78,   0xc4,    0x00}, // Bw125Cr48Sf4096

};

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
    RFMSPI2Write(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 0);

    RFMsetMode(RFM_MODE_IDLE);

    // Set up default configuration
    // No Sync Words in LORA mode.
    //setModemConfig(Bw125Cr48Sf4096); // slow and reliable?
    RFMsetModemConfig(Bw125Cr45Sf128); // Radio default
    
    
    RFMsetPreambleLength(8); // Default is 8
    
    // An innocuous ISM frequency, same as RF22's
    RFMsetFrequency(434.0);
    
    // Lowish power
    RFMsetTxPower(13,0);

}

void RFMsetMode(RadioOpMode mode) {
  
    if (mode == gblinfo.rfmmode)
        return;

    gblinfo.rfmmode = mode;
    
    switch (mode) {
        case RFM_MODE_TX:      //TODO temp updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
            break;
        case RFM_MODE_RX:      //TODO temp updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_RXCONTINUOUS);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone
            break;
        case RFM_MODE_STANDBY:     
            // RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
            break;
        case RFM_MODE_SLEEP:       //TODO temp note, updated
            // RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
            break;
        case RFM_MODE_IDLE:        //TODO temp, updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_STDBY);
            break;
        default:
            return;
    }

}

bool RFMsend(const char * data, uint8_t len)
{
    uint8_t i = 0;
    
    if (len > RFM_MAX_MESSAGE_LEN) return false;

    //TODO do we want to insert a blocking function here that verifies we're okay 
    // to transmit a packet. 
    // while (RFMtxInProgress() && 50 > i++);    
    
    RFMsetMode(RFM_MODE_IDLE);

    // Position at the beginning of the FIFO
    RFMSPI2Write(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
    
    // The headers
    RFMSPI2Write(RFM_REG_00_FIFO, RFM_BROADCAST_ADDRESS);       // Enter destination address
    RFMSPI2Write(RFM_REG_00_FIFO, RFM_BROADCAST_ADDRESS);          // Source address
    RFMSPI2Write(RFM_REG_00_FIFO, 0x01);                        // Header ID
    RFMSPI2Write(RFM_REG_00_FIFO, 0x00);                        // Header flags
    
    // The message data
    RFMSPI2WriteBurst(RFM_REG_00_FIFO, data, len);
    
    RFMSPI2Write(RH_RF95_REG_22_PAYLOAD_LENGTH, len + RFM_HEADER_LEN);

    RFMsetMode(RFM_MODE_TX);    // Start the transmitter
                                // when Tx is done, interruptHandler will fire and radio mode will return to STANDBY
    
    //TODO do we want to block here until IRQ flag goes high to indicate transmit is finished?
    
    return true;
}

// Set one of the canned FSK Modem configs
// Returns true if its a valid choice
bool RFMsetModemConfig(ModemConfigChoice index) {       //TODO temp note, new RH function 
    if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return false;

    ModemConfig cfg;
    memcpy(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(ModemConfig));
    
    RFMsetModemRegisters(&cfg);

    return true;
}

// Sets registers from a canned modem configuration structure
void RFMsetModemRegisters(const ModemConfig* config) {
    RFMSPI2Write(RH_RF95_REG_1D_MODEM_CONFIG1, config->reg_1d);
    RFMSPI2Write(RH_RF95_REG_1E_MODEM_CONFIG2, config->reg_1e);
    RFMSPI2Write(RH_RF95_REG_26_MODEM_CONFIG3, config->reg_26);
}

bool RFMsetFrequency (float centre) {       //TODO, temp note, RH function
    
    // Frf = FRF / FSTEP
    uint32_t frf = (centre * 1000000.0) / RH_RF95_FSTEP;
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

