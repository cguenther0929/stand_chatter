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
*/
#include "rfm69.h"		    // Include the header file for this module

struct GlobalInformation gblinfo;

void RFMInitialize( uint8_t networkID, uint8_t nodeID ) {
    
    uint8_t RFM_CONFIG[][2] =
    {
        { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
        { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
        { REG_BITRATEMSB, RF_BITRATEMSB_55555}, // default: 4.8 KBPS
        { REG_BITRATELSB, RF_BITRATELSB_55555},
        { REG_FDEVMSB, RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
        { REG_FDEVLSB, RF_FDEVLSB_50000},      
        { REG_FRFMSB, (uint8_t) RF_FRFMSB_915},
        { REG_FRFMID, (uint8_t) RF_FRFMID_915},
        { REG_FRFLSB, (uint8_t) RF_FRFLSB_915},
        { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
        { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
        { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
        { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
        { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
        { REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE }, // default 3 preamble bytes 0xAAAAAA
        { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
        { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
        { REG_SYNCVALUE2, networkID }, // NETWORK ID
        { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
        { REG_PAYLOADLENGTH, 66 }, // in variable length mode: the max frame size, not used in TX
        { REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
        { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
        { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
        {255, 0}
  };

    // TODO need to figure out sync stuff
    // TODO may want to take some action when we timeout (i.e. i ctr reaches a high value)
    uint8_t i = 0;
    do {
        RFMSPI2Write(REG_SYNCVALUE1, 0xAA); 
        i++;
    } while (RFMSPI2Read(REG_SYNCVALUE1) != 0xAA && i < 20);
    
    i = 0;
    do {
        RFMSPI2Write(REG_SYNCVALUE1, 0x55); 
        i++;
    } while (RFMSPI2Read(REG_SYNCVALUE1) != 0x55 && i < 20);
    
        
    for (uint8_t i = 0; RFM_CONFIG[i][0] != 255; i++)           // Send the massive configuration array
        RFMSPI2Write(RFM_CONFIG[i][0], RFM_CONFIG[i][1]);

  
    // TODO for now we're just removing encryption just as down in the RadioHead Library
    // Disable it during initialization so we always start from a known state.
    RFMencrypt(0);   

    RFMsetHighPower(true);

    RFMsetMode(RF69_MODE_STANDBY);

    i = 0;
    // while (((RFMSPI2Read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) && i < 20) {     // wait for ModeReady
    //     i++;
    // }  // wait for ModeReady

}

// To enable encryption: radio.encrypt("ABCDEFGHIJKLMNOP");
// To disable encryption: radio.encrypt(null) or radio.encrypt(0)
// KEY HAS TO BE 16 bytes !!!
void RFMencrypt(const char* key) {
    // #if defined(RF69_LISTENMODE_ENABLE)
    //   _haveEncryptKey = key;
    // #endif
    //   setMode(RF69_MODE_STANDBY);
    
    // if (key != 0)
    // {
    // #if defined(RF69_LISTENMODE_ENABLE)
    //     memcpy(_encryptKey, key, 16);
    // #endif
    //     select();
    //     SPI.transfer(REG_AESKEY1 | 0x80);
    //     for (uint8_t i = 0; i < 16; i++)
    //         SPI.transfer(key[i]);
    //     unselect();
    // }
    RFMSPI2Write(REG_PACKETCONFIG2, (RFMSPI2Read(REG_PACKETCONFIG2) & 0xFE) | (key ? 1 : 0));
}

void RFMsetMode(uint8_t newMode) {
  
    if (newMode == gblinfo.rfmmode)
        return;

    gblinfo.rfmmode = newMode;
    
    switch (newMode) {
      case RF69_MODE_TX:
          RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
          RFMSetHighPowerRegs(true);
          break;
      case RF69_MODE_RX:
          RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
          RFMSetHighPowerRegs(false);
          break;
      case RF69_MODE_SYNTH:
          RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
          break;
      case RF69_MODE_STANDBY:
          RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
          break;
      case RF69_MODE_SLEEP:
          RFMSPI2Write(REG_OPMODE, (RFMSPI2Read(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
          break;
      default:
          return;
    }

}

void RFMSetHighPowerRegs (bool onOff) {
    RFMSPI2Write(REG_TESTPA1, onOff ? 0x5D : 0x55);
    RFMSPI2Write(REG_TESTPA2, onOff ? 0x7C : 0x70);
}

// for RFM69HW only: you must call setHighPower(true) after initialize() or else transmission won't work
void RFMsetHighPower (bool onOff) {
    RFMSPI2Write(REG_OCP, RF_OCP_ON);
    RFMSPI2Write(REG_PALEVEL, (RFMSPI2Read(REG_PALEVEL) & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON); // enable P1 & P2 amplifier stages
}

uint8_t RFMreadTemperature(uint8_t calFactor) // returns centigrade
{
    RFMsetMode(RF69_MODE_STANDBY);
    RFMSPI2Write(REG_TEMP1, RF_TEMP1_MEAS_START);
    while ((RFMSPI2Read(REG_TEMP1) & RF_TEMP1_MEAS_RUNNING));
    
    return ~RFMSPI2Read(REG_TEMP2) + COURSE_TEMP_COEF + calFactor; // 'complement' corrects the slope, rising temp = rising val
} 
