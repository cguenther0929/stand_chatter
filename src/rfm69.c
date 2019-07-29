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

void RFMInitialize( uint8_t networkID, uint8_t nodeID ) {

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

    RFMsetMode(RF95_MODE_IDLE);

    // Set up default configuration
    // No Sync Words in LORA mode.
    //setModemConfig(Bw125Cr48Sf4096); // slow and reliable?
    RFMsetModemConfig(Bw125Cr45Sf128); // Radio default
    
    
    RFMsetPreambleLength(8); // Default is 8
    
    // An innocuous ISM frequency, same as RF22's
    RFMsetFrequency(434.0);
    
    // Lowish power
    RFMsetTxPower(13,0);

     
//     uint8_t RFM_CONFIG[][2] =
//     {
//         { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
//         { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
//         { REG_BITRATEMSB, RF_BITRATEMSB_55555}, // default: 4.8 KBPS
//         { REG_BITRATELSB, RF_BITRATELSB_55555},
//         { REG_FDEVMSB, RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
//         { REG_FDEVLSB, RF_FDEVLSB_50000},      
//         { REG_FRFMSB, (uint8_t) RF_FRFMSB_915},
//         { REG_FRFMID, (uint8_t) RF_FRFMID_915},
//         { REG_FRFLSB, (uint8_t) RF_FRFLSB_915},
//         { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
//         { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
//         { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
//         { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
//         { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
//         //{ REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE }, // default 3 preamble bytes 0xAAAAAA
//         { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
//         { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
//         { REG_SYNCVALUE2, networkID }, // NETWORK ID
//         { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
//         { REG_PAYLOADLENGTH, 66 }, // in variable length mode: the max frame size, not used in TX
//         //{ REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
//         { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
//         //{ REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
//         { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
//         {255, 0}
//   };

    // gblinfo.our_address = nodeID;

    // uint8_t i = 0;
    // do {
    //     RFMSPI2Write(REG_SYNCVALUE1, 0xAA); 
    //     i++;
    // } while (RFMSPI2Read(REG_SYNCVALUE1) != 0xAA && i < 50);

    // if(i >= 49) {
    //     DispSetContrast(50);
    //     DispRefresh();
    //     DispWriteString("TIMEOUT AA SYNC");
    //     tick100msDelay(20);
    // }
    
    // i = 0;
    // do {
    //     RFMSPI2Write(REG_SYNCVALUE1, 0x55); 
    //     i++;
    // } while (RFMSPI2Read(REG_SYNCVALUE1) != 0x55 && i < 50);
    
    // if(i >= 49) {
    //     DispSetContrast(50);
    //     DispRefresh();
    //     DispWriteString("TIMEOUT 55 SYNC");
    //     tick100msDelay(20);
    // }
        
    // for (uint8_t i = 0; RFM_CONFIG[i][0] != 255; i++)           // Send the massive configuration array
    //     RFMSPI2Write(RFM_CONFIG[i][0], RFM_CONFIG[i][1]);

  
    // // Remove encryption just as done in the RadioHead Library
    // // Disable it during initialization so we always start from a known state.
    // RFMencrypt(0);   

    // RFMsetHighPower(true);          // For RFM69, must be in high power mode to transmit

    // RFMsetMode(RF69_MODE_STANDBY);

    // i = 0;
    // while (((RFMSPI2Read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) && i < 20) {     // wait for ModeReady
    //     i++;
    // } 
    
    // if(i >= 19) {
    //     DispSetContrast(50);
    //     DispRefresh();
    //     DispWriteString("TIMEOUT MODE RDY");
    //     tick100msDelay(20);
    // }

}

// To enable encryption: radio.encrypt("ABCDEFGHIJKLMNOP");
// To disable encryption: radio.encrypt(null) or radio.encrypt(0)
// KEY HAS TO BE 16 bytes !!!
// void RFMencrypt(const char* key) {
//     // #if defined(RF69_LISTENMODE_ENABLE)
//     //   _haveEncryptKey = key;
//     // #endif
//     //   RFMsetMode(RF69_MODE_STANDBY);
    
//     // if (key != 0)
//     // {
//     // #if defined(RF69_LISTENMODE_ENABLE)
//     //     memcpy(_encryptKey, key, 16);
//     // #endif
//     //     select();
//     //     RFMSPI2Write(REG_AESKEY1 | 0x80);
//     //     for (uint8_t i = 0; i < 16; i++)
//     //         RFMSPI2Write(key[i]);
//     //     unselect();
//     // }
//     RFMSPI2Write(REG_PACKETCONFIG2, (RFMSPI2Read(REG_PACKETCONFIG2) & 0xFE) | (key ? 1 : 0));
// }

void RFMsetMode(RadioOpMode mode) {
  
    if (mode == gblinfo.rfmmode)
        return;

    gblinfo.rfmmode = mode;
    
    switch (mode) {
        case RF69_MODE_TX:      //TODO temp updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
            break;
        case RF69_MODE_RX:      //TODO temp updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_RXCONTINUOUS);
	        RFMSPI2Write(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone
            break;
        case RF69_MODE_STANDBY:     
            // RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
            break;
        case RF95_MODE_SLEEP:       //TODO temp note, updated
            // RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
            break;
        case RF95_MODE_IDLE:        //TODO temp, updated
            RFMSPI2Write(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_STDBY);
            break;
        default:
            return;
    }

}

// void RFMSetHighPowerRegs (bool onOff) {
//     RFMSPI2Write(REG_TESTPA1, onOff ? 0x5D : 0x55);
//     RFMSPI2Write(REG_TESTPA2, onOff ? 0x7C : 0x70);
// }

// // for RFM69HW only: you must call setHighPower(true) after initialize() or else transmission won't work
// void RFMsetHighPower (bool onOff) {
//     RFMSPI2Write(REG_OCP, RF_OCP_ON);
//     RFMSPI2Write(REG_PALEVEL, (RFMSPI2Read(REG_PALEVEL) & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON); // enable P1 & P2 amplifier stages
// }

// uint8_t RFMreadTemperature(uint8_t calFactor) // returns centigrade  //TODO, might need to delete this
// {
//     RFMsetMode(RF69_MODE_STANDBY);
//     RFMSPI2Write(REG_TEMP1, RF_TEMP1_MEAS_START);
//     while ((RFMSPI2Read(REG_TEMP1) & RF_TEMP1_MEAS_RUNNING));
    
//     return ~RFMSPI2Read(REG_TEMP2) + COURSE_TEMP_COEF + calFactor; // 'complement' corrects the slope, rising temp = rising val
// } 


// bool RFMcanSend( void ) {       //TODO, temp note, not updated
//   if (gblinfo.rfmmode == RF69_MODE_RX && gblinfo.payloadlen == 0 && RFMreadRSSI() < CSMA_LIMIT) // if signal stronger than -100dBm is detected assume channel activity
//   {
//     RFMsetMode(RF69_MODE_STANDBY);
//     return true;
//   }
//   return false;
// }

// void RFMSend(uint16_t toAddress, const void* buffer, uint8_t bufferSize) {      //TODO, temp note, not updated
//     uint8_t i = 0;
    
//     RFMSPI2Write(REG_PACKETCONFIG2, (RFMSPI2Read(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
//     // while (!RFMcanSend() && millis() - now < RF69_CSMA_LIMIT_MS) receiveDone(); //TODO we need to add this hardening back in.  Basically, we can't send if a receive operation isn't complete 
//     while (!RFMcanSend() && i < 50) i++;
    
//     RFMsetMode(RF69_MODE_STANDBY); // turn off receiver to prevent reception while filling fifo
//     while ((RFMSPI2Read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
    
//     RFMSPI2Write(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Packet Sent"
//     if (bufferSize > RF69_MAX_DATA_LEN) bufferSize = RF69_MAX_DATA_LEN;

//     // control byte
//     uint8_t CTLbyte = 0x00;
//     // if (sendACK)
//     // CTLbyte = RFM69_CTL_SENDACK;
//     // else if (requestACK)
//     // CTLbyte = RFM69_CTL_REQACK;

//     if (toAddress > 0xFF) CTLbyte |= (toAddress & 0x300) >> 6; //assign last 2 bits of address if > 255
//     if (gblinfo.our_address > 0xFF) CTLbyte |= (gblinfo.our_address & 0x300) >> 8;   //assign last 2 bits of address if > 255

//     // write to FIFO
//     RFMSPI2WriteByte(REG_FIFO | 0x80);
//     RFMSPI2WriteByte(bufferSize + 3);
//     RFMSPI2WriteByte((uint8_t)toAddress);
//     RFMSPI2WriteByte((uint8_t)gblinfo.our_address);
//     RFMSPI2WriteByte(CTLbyte);

//     for (i = 0; i < bufferSize; i++)
//         RFMSPI2WriteByte(((uint8_t*) buffer)[i]);

//     // no need to wait for transmit mode to be ready since its handled by the radio
//     RFMsetMode(RF69_MODE_TX);
    
//     i = 0;
//     while (RFM_IRQ_PIN == 0 && i < 200 ) i++; // wait for DIO0 to turn HIGH signalling transmission finish

//     if (i >= 199) {          //TODO can delete this DEBUG code.  
//         DispRefresh();
//         DispWriteString("ERROR!!");
//         tick100msDelay(50);
//     }
    
//     RFMsetMode(RF69_MODE_STANDBY);
// }

// Set one of the canned FSK Modem configs
// Returns true if its a valid choice
bool RFMsetModemConfig(ModemConfigChoice index) {       //TODO temp note, new RH function 
    if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return false;

    ModemConfig cfg;
    memcpy(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(ModemConfig));
    
    setModemRegisters(&cfg);

    return true;
}

// Sets registers from a canned modem configuration structure
void RFMsetModemRegisters(const ModemConfig* config) {
    RFMSPI2Write(RH_RF95_REG_1D_MODEM_CONFIG1, config->reg_1d);
    RFMSPI2Write(RH_RF95_REG_1E_MODEM_CONFIG2, config->reg_1e);
    RFMSPI2Write(RH_RF95_REG_26_MODEM_CONFIG3, config->reg_26);
}

// int16_t RFMreadRSSI( void ) {
//     int16_t rssi = 0;
//     //   if (forceTrigger)
//     //   {
//     //     // RSSI trigger not needed if DAGC is in continuous mode
//     //     writeReg(REG_RSSICONFIG, RF_RSSI_START);
//     //     while ((readReg(REG_RSSICONFIG) & RF_RSSI_DONE) == 0x00); // wait for RSSI_Ready
//     //   }
//     rssi = -RFMSPI2Read(REG_RSSIVALUE);
//     rssi >>= 1;
//     gblinfo.rssi_lvl = rssi;
//     return rssi;
// }

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

