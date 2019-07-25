/******************************************************************************
*   FILE: config.h
*
*   PURPOSE: Configuration file specific to the RFM69 LoRa radio module 
*           much of this informaiton was borrowed from the radio head library
*           which can be found here https://github.com/LowPowerLab/RFM69 
*
*   DEVICE: PIC18F66K22
*
*   COMPILER: Microchip XC8 v1.32
*
*   IDE: MPLAB X v3.45
*
*   TODO:  
*
*   NOTE:
*
******************************************************************************/
#ifndef __RFM69_H_
#define __RFM69_H_

#include <xc.h>         //Part specific header file
#include <stdint.h>
#include <stdbool.h>
#include "struct.h"
#include "config.h"     //Project specific header file
#include "config.h"
#include "rfm69.h"
#include "spi.h"
#include "disp.h"
#include "main.h"


void RFMInitialize( uint8_t networkID, uint8_t nodeID );

void RFMencrypt(const char * key);

void RFMsetMode(uint8_t newMode);

void RFMSetHighPowerRegs(bool onOff);

void RFMsetHighPower (bool onOff);

uint8_t RFMreadTemperature(uint8_t calFactor);


/* SPI COMM HELP BITS */  //TODO remove these lines
// #define RFM_READ                        0x80
// #define RFM_WRITE                       0x00

/* RFM69 PARAMETERS */
//TODO do we need all of these?  Should we clean up?  Taken from github
#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access
#define RF69_MODE_SLEEP         0   // XTAL OFF
#define RF69_MODE_STANDBY       1   // XTAL ON
#define RF69_MODE_SYNTH         2   // PLL ON
#define RF69_MODE_RX            3   // RX MODE
#define RF69_MODE_TX            4   // TX MODE

/* LoRa RADIO FREQUENCY BANDS */
typedef enum RFM69_FREQ {
    RF69_315MHZ,
    RF69_433MHZ,
    RF69_868MHZ,
    RF69_915MHZ
} RFM69_FREQ;


/*  RF69 CONTROL BIT DEFINITIONS */

// RegOpMode
#define RF_OPMODE_SEQUENCER_OFF             0x80
#define RF_OPMODE_SEQUENCER_ON              0x00  // Default

#define RF_OPMODE_LISTEN_ON                 0x40
#define RF_OPMODE_LISTEN_OFF                0x00  // Default

#define RF_OPMODE_LISTENABORT               0x20

#define RF_OPMODE_SLEEP                     0x00
#define RF_OPMODE_STANDBY                   0x04  // Default
#define RF_OPMODE_SYNTHESIZER               0x08
#define RF_OPMODE_TRANSMITTER               0x0C
#define RF_OPMODE_RECEIVER                  0x10

// RegDataModul
#define RF_DATAMODUL_DATAMODE_PACKET            0x00  // Default
#define RF_DATAMODUL_DATAMODE_CONTINUOUS        0x40
#define RF_DATAMODUL_DATAMODE_CONTINUOUSNOBSYNC 0x60

#define RF_DATAMODUL_MODULATIONTYPE_FSK         0x00  // Default
#define RF_DATAMODUL_MODULATIONTYPE_OOK         0x08

#define RF_DATAMODUL_MODULATIONSHAPING_00       0x00  // Default
#define RF_DATAMODUL_MODULATIONSHAPING_01       0x01
#define RF_DATAMODUL_MODULATIONSHAPING_10       0x02
#define RF_DATAMODUL_MODULATIONSHAPING_11       0x03

#define RF_FRFMSB_915                           0xE4  // Default
#define RF_FRFMID_915                           0xC0  // Default
#define RF_FRFLSB_915                           0x00  // Default

// RegRxBw
#define RF_RXBW_DCCFREQ_000                 0x00
#define RF_RXBW_DCCFREQ_001                 0x20
#define RF_RXBW_DCCFREQ_010                 0x40  // Recommended default
#define RF_RXBW_DCCFREQ_011                 0x60
#define RF_RXBW_DCCFREQ_100                 0x80  // Reset value
#define RF_RXBW_DCCFREQ_101                 0xA0
#define RF_RXBW_DCCFREQ_110                 0xC0
#define RF_RXBW_DCCFREQ_111                 0xE0

#define RF_RXBW_MANT_16                     0x00  // Reset value
#define RF_RXBW_MANT_20                     0x08
#define RF_RXBW_MANT_24                     0x10  // Recommended default

#define RF_RXBW_EXP_0                       0x00
#define RF_RXBW_EXP_1                       0x01
#define RF_RXBW_EXP_2                       0x02
#define RF_RXBW_EXP_3                       0x03
#define RF_RXBW_EXP_4                       0x04
#define RF_RXBW_EXP_5                       0x05  // Recommended default
#define RF_RXBW_EXP_6                       0x06  // Reset value
#define RF_RXBW_EXP_7                       0x07

// RegDioMapping1
#define RF_DIOMAPPING1_DIO0_00              0x00  // Default
#define RF_DIOMAPPING1_DIO0_01              0x40
#define RF_DIOMAPPING1_DIO0_10              0x80
#define RF_DIOMAPPING1_DIO0_11              0xC0

#define RF_DIOMAPPING1_DIO1_00              0x00  // Default
#define RF_DIOMAPPING1_DIO1_01              0x10
#define RF_DIOMAPPING1_DIO1_10              0x20
#define RF_DIOMAPPING1_DIO1_11              0x30

#define RF_DIOMAPPING1_DIO2_00              0x00  // Default
#define RF_DIOMAPPING1_DIO2_01              0x04
#define RF_DIOMAPPING1_DIO2_10              0x08
#define RF_DIOMAPPING1_DIO2_11              0x0C

#define RF_DIOMAPPING1_DIO3_00              0x00  // Default
#define RF_DIOMAPPING1_DIO3_01              0x01
#define RF_DIOMAPPING1_DIO3_10              0x02
#define RF_DIOMAPPING1_DIO3_11              0x03

// RegDioMapping2
#define RF_DIOMAPPING2_DIO4_00              0x00  // Default
#define RF_DIOMAPPING2_DIO4_01              0x40
#define RF_DIOMAPPING2_DIO4_10              0x80
#define RF_DIOMAPPING2_DIO4_11              0xC0

#define RF_DIOMAPPING2_DIO5_00              0x00  // Default
#define RF_DIOMAPPING2_DIO5_01              0x10
#define RF_DIOMAPPING2_DIO5_10              0x20
#define RF_DIOMAPPING2_DIO5_11              0x30

#define RF_DIOMAPPING2_CLKOUT_32MHZ         0x00
#define RF_DIOMAPPING2_CLKOUT_16MHZ         0x01
#define RF_DIOMAPPING2_CLKOUT_8MHZ          0x02
#define RF_DIOMAPPING2_CLKOUT_4MHZ          0x03
#define RF_DIOMAPPING2_CLKOUT_2MHZ          0x04
#define RF_DIOMAPPING2_CLKOUT_1MHZ          0x05  // Reset value
#define RF_DIOMAPPING2_CLKOUT_RC            0x06
#define RF_DIOMAPPING2_CLKOUT_OFF           0x07  // Recommended default

// RegIrqFlags1
#define RF_IRQFLAGS1_MODEREADY              0x80
#define RF_IRQFLAGS1_RXREADY                0x40
#define RF_IRQFLAGS1_TXREADY                0x20
#define RF_IRQFLAGS1_PLLLOCK                0x10
#define RF_IRQFLAGS1_RSSI                   0x08
#define RF_IRQFLAGS1_TIMEOUT                0x04
#define RF_IRQFLAGS1_AUTOMODE               0x02
#define RF_IRQFLAGS1_SYNCADDRESSMATCH       0x01

// RegIrqFlags2
#define RF_IRQFLAGS2_FIFOFULL               0x80
#define RF_IRQFLAGS2_FIFONOTEMPTY           0x40
#define RF_IRQFLAGS2_FIFOLEVEL              0x20
#define RF_IRQFLAGS2_FIFOOVERRUN            0x10
#define RF_IRQFLAGS2_PACKETSENT             0x08
#define RF_IRQFLAGS2_PAYLOADREADY           0x04
#define RF_IRQFLAGS2_CRCOK                  0x02
#define RF_IRQFLAGS2_LOWBAT                 0x01  // not present on RFM69/SX1231

// RegRssiThresh
#define RF_RSSITHRESH_VALUE                 0xE4  // Default

// RegRxTimeout1    
#define RF_RXTIMEOUT1_RXSTART_VALUE         0x00  // Default

// RegRxTimeout2    
#define RF_RXTIMEOUT2_RSSITHRESH_VALUE      0x00  // Default

// RegPreamble  
#define RF_PREAMBLESIZE_MSB_VALUE           0x00  // Default
#define RF_PREAMBLESIZE_LSB_VALUE           0x03  // Default

// RegSyncConfig
#define RF_SYNC_ON                          0x80  // Default
#define RF_SYNC_OFF                         0x00

#define RF_SYNC_FIFOFILL_AUTO               0x00  // Default -- when sync interrupt occurs
#define RF_SYNC_FIFOFILL_MANUAL             0x40

#define RF_SYNC_SIZE_1                      0x00
#define RF_SYNC_SIZE_2                      0x08
#define RF_SYNC_SIZE_3                      0x10
#define RF_SYNC_SIZE_4                      0x18  // Default
#define RF_SYNC_SIZE_5                      0x20
#define RF_SYNC_SIZE_6                      0x28
#define RF_SYNC_SIZE_7                      0x30
#define RF_SYNC_SIZE_8                      0x38

#define RF_SYNC_TOL_0                       0x00  // Default
#define RF_SYNC_TOL_1                       0x01
#define RF_SYNC_TOL_2                       0x02
#define RF_SYNC_TOL_3                       0x03
#define RF_SYNC_TOL_4                       0x04
#define RF_SYNC_TOL_5                       0x05
#define RF_SYNC_TOL_6                       0x06
#define RF_SYNC_TOL_7                       0x07

// RegSyncValue1-8
#define RF_SYNC_BYTE1_VALUE                 0x00  // Default
#define RF_SYNC_BYTE2_VALUE                 0x00  // Default
#define RF_SYNC_BYTE3_VALUE                 0x00  // Default
#define RF_SYNC_BYTE4_VALUE                 0x00  // Default
#define RF_SYNC_BYTE5_VALUE                 0x00  // Default
#define RF_SYNC_BYTE6_VALUE                 0x00  // Default
#define RF_SYNC_BYTE7_VALUE                 0x00  // Default
#define RF_SYNC_BYTE8_VALUE                 0x00  // Default


// RegPacketConfig1
#define RF_PACKET1_FORMAT_FIXED             0x00  // Default
#define RF_PACKET1_FORMAT_VARIABLE          0x80

#define RF_PACKET1_DCFREE_OFF               0x00  // Default
#define RF_PACKET1_DCFREE_MANCHESTER        0x20
#define RF_PACKET1_DCFREE_WHITENING         0x40

#define RF_PACKET1_CRC_ON                   0x10  // Default
#define RF_PACKET1_CRC_OFF                  0x00

#define RF_PACKET1_CRCAUTOCLEAR_ON          0x00  // Default
#define RF_PACKET1_CRCAUTOCLEAR_OFF         0x08

#define RF_PACKET1_ADRSFILTERING_OFF            0x00  // Default
#define RF_PACKET1_ADRSFILTERING_NODE           0x02
#define RF_PACKET1_ADRSFILTERING_NODEBROADCAST  0x04

// RegPayloadLength
#define RF_PAYLOADLENGTH_VALUE          0x40  // Default

// RegBroadcastAdrs
#define RF_BROADCASTADDRESS_VALUE       0x00

// RegAutoModes
#define RF_AUTOMODES_ENTER_OFF                  0x00  // Default
#define RF_AUTOMODES_ENTER_FIFONOTEMPTY         0x20
#define RF_AUTOMODES_ENTER_FIFOLEVEL            0x40
#define RF_AUTOMODES_ENTER_CRCOK                0x60
#define RF_AUTOMODES_ENTER_PAYLOADREADY         0x80
#define RF_AUTOMODES_ENTER_SYNCADRSMATCH        0xA0
#define RF_AUTOMODES_ENTER_PACKETSENT           0xC0
#define RF_AUTOMODES_ENTER_FIFOEMPTY            0xE0

#define RF_AUTOMODES_EXIT_OFF                   0x00  // Default
#define RF_AUTOMODES_EXIT_FIFOEMPTY             0x04
#define RF_AUTOMODES_EXIT_FIFOLEVEL             0x08
#define RF_AUTOMODES_EXIT_CRCOK                 0x0C
#define RF_AUTOMODES_EXIT_PAYLOADREADY          0x10
#define RF_AUTOMODES_EXIT_SYNCADRSMATCH         0x14
#define RF_AUTOMODES_EXIT_PACKETSENT            0x18
#define RF_AUTOMODES_EXIT_RXTIMEOUT             0x1C

#define RF_AUTOMODES_INTERMEDIATE_SLEEP         0x00  // Default
#define RF_AUTOMODES_INTERMEDIATE_STANDBY       0x01
#define RF_AUTOMODES_INTERMEDIATE_RECEIVER      0x02
#define RF_AUTOMODES_INTERMEDIATE_TRANSMITTER   0x03

// RegFifoThresh
#define RF_FIFOTHRESH_TXSTART_FIFOTHRESH        0x00  // Reset value
#define RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY      0x80  // Recommended default

#define RF_FIFOTHRESH_VALUE                     0x0F  // Default

// RegPacketConfig2
#define RF_PACKET2_RXRESTARTDELAY_1BIT          0x00  // Default
#define RF_PACKET2_RXRESTARTDELAY_2BITS         0x10
#define RF_PACKET2_RXRESTARTDELAY_4BITS         0x20
#define RF_PACKET2_RXRESTARTDELAY_8BITS         0x30
#define RF_PACKET2_RXRESTARTDELAY_16BITS        0x40
#define RF_PACKET2_RXRESTARTDELAY_32BITS        0x50
#define RF_PACKET2_RXRESTARTDELAY_64BITS        0x60
#define RF_PACKET2_RXRESTARTDELAY_128BITS       0x70
#define RF_PACKET2_RXRESTARTDELAY_256BITS       0x80
#define RF_PACKET2_RXRESTARTDELAY_512BITS       0x90
#define RF_PACKET2_RXRESTARTDELAY_1024BITS      0xA0
#define RF_PACKET2_RXRESTARTDELAY_2048BITS      0xB0
#define RF_PACKET2_RXRESTARTDELAY_NONE          0xC0
#define RF_PACKET2_RXRESTART                    0x04

#define RF_PACKET2_AUTORXRESTART_ON             0x02  // Default
#define RF_PACKET2_AUTORXRESTART_OFF            0x00

#define RF_PACKET2_AES_ON                       0x01
#define RF_PACKET2_AES_OFF                      0x00  // Default

// RegAesKey1-16
#define RF_AESKEY1_VALUE                        0x00  // Default
#define RF_AESKEY2_VALUE                        0x00  // Default
#define RF_AESKEY3_VALUE                        0x00  // Default
#define RF_AESKEY4_VALUE                        0x00  // Default
#define RF_AESKEY5_VALUE                        0x00  // Default
#define RF_AESKEY6_VALUE                        0x00  // Default
#define RF_AESKEY7_VALUE                        0x00  // Default
#define RF_AESKEY8_VALUE                        0x00  // Default
#define RF_AESKEY9_VALUE                        0x00  // Default
#define RF_AESKEY10_VALUE                       0x00  // Default
#define RF_AESKEY11_VALUE                       0x00  // Default
#define RF_AESKEY12_VALUE                       0x00  // Default
#define RF_AESKEY13_VALUE                       0x00  // Default
#define RF_AESKEY14_VALUE                       0x00  // Default
#define RF_AESKEY15_VALUE                       0x00  // Default
#define RF_AESKEY16_VALUE                       0x00  // Default

// RegTemp1
#define RF_TEMP1_MEAS_START                     0x08
#define RF_TEMP1_MEAS_RUNNING                   0x04
// not present on RFM69/SX1231          
#define RF_TEMP1_ADCLOWPOWER_ON                 0x01  // Default
#define RF_TEMP1_ADCLOWPOWER_OFF                0x00

// RegTestLna           
#define RF_TESTLNA_NORMAL                       0x1B
#define RF_TESTLNA_HIGH_SENSITIVITY             0x2D

// RegTestDagc          
#define RF_DAGC_NORMAL                          0x00  // Reset value
#define RF_DAGC_IMPROVED_LOWBETA1               0x20
#define RF_DAGC_IMPROVED_LOWBETA0               0x30  // Recommended default

// RegBitRate (bits/sec) example bit rates
#define RF_BITRATEMSB_1200            0x68
#define RF_BITRATELSB_1200            0x2B
#define RF_BITRATEMSB_2400            0x34
#define RF_BITRATELSB_2400            0x15
#define RF_BITRATEMSB_4800            0x1A  // Default
#define RF_BITRATELSB_4800            0x0B  // Default
#define RF_BITRATEMSB_9600            0x0D
#define RF_BITRATELSB_9600            0x05
#define RF_BITRATEMSB_19200           0x06
#define RF_BITRATELSB_19200           0x83
#define RF_BITRATEMSB_38400           0x03
#define RF_BITRATELSB_38400           0x41

#define RF_BITRATEMSB_38323           0x03
#define RF_BITRATELSB_38323           0x43

#define RF_BITRATEMSB_34482           0x03
#define RF_BITRATELSB_34482           0xA0

#define RF_BITRATEMSB_76800           0x01
#define RF_BITRATELSB_76800           0xA1
#define RF_BITRATEMSB_153600          0x00
#define RF_BITRATELSB_153600          0xD0
#define RF_BITRATEMSB_57600           0x02
#define RF_BITRATELSB_57600           0x2C
#define RF_BITRATEMSB_115200          0x01
#define RF_BITRATELSB_115200          0x16
#define RF_BITRATEMSB_12500           0x0A
#define RF_BITRATELSB_12500           0x00
#define RF_BITRATEMSB_25000           0x05
#define RF_BITRATELSB_25000           0x00
#define RF_BITRATEMSB_50000           0x02
#define RF_BITRATELSB_50000           0x80
#define RF_BITRATEMSB_100000          0x01
#define RF_BITRATELSB_100000          0x40
#define RF_BITRATEMSB_150000          0x00
#define RF_BITRATELSB_150000          0xD5
#define RF_BITRATEMSB_200000          0x00
#define RF_BITRATELSB_200000          0xA0
#define RF_BITRATEMSB_250000          0x00
#define RF_BITRATELSB_250000          0x80
#define RF_BITRATEMSB_300000          0x00
#define RF_BITRATELSB_300000          0x6B
#define RF_BITRATEMSB_32768           0x03
#define RF_BITRATELSB_32768           0xD1
// custom bit rates
#define RF_BITRATEMSB_55555           0x02
#define RF_BITRATELSB_55555           0x40
#define RF_BITRATEMSB_200KBPS         0x00
#define RF_BITRATELSB_200KBPS         0xa0

// RegFdev - frequency deviation (Hz)
#define RF_FDEVMSB_2000             0x00
#define RF_FDEVLSB_2000             0x21
#define RF_FDEVMSB_5000             0x00  // Default
#define RF_FDEVLSB_5000             0x52  // Default
#define RF_FDEVMSB_7500             0x00
#define RF_FDEVLSB_7500             0x7B
#define RF_FDEVMSB_10000            0x00
#define RF_FDEVLSB_10000            0xA4
#define RF_FDEVMSB_15000            0x00
#define RF_FDEVLSB_15000            0xF6
#define RF_FDEVMSB_20000            0x01
#define RF_FDEVLSB_20000            0x48
#define RF_FDEVMSB_25000            0x01
#define RF_FDEVLSB_25000            0x9A
#define RF_FDEVMSB_30000            0x01
#define RF_FDEVLSB_30000            0xEC
#define RF_FDEVMSB_35000            0x02
#define RF_FDEVLSB_35000            0x3D
#define RF_FDEVMSB_40000            0x02
#define RF_FDEVLSB_40000            0x8F
#define RF_FDEVMSB_45000            0x02
#define RF_FDEVLSB_45000            0xE1
#define RF_FDEVMSB_50000            0x03
#define RF_FDEVLSB_50000            0x33
#define RF_FDEVMSB_55000            0x03
#define RF_FDEVLSB_55000            0x85
#define RF_FDEVMSB_60000            0x03
#define RF_FDEVLSB_60000            0xD7
#define RF_FDEVMSB_65000            0x04
#define RF_FDEVLSB_65000            0x29
#define RF_FDEVMSB_70000            0x04
#define RF_FDEVLSB_70000            0x7B
#define RF_FDEVMSB_75000            0x04
#define RF_FDEVLSB_75000            0xCD
#define RF_FDEVMSB_80000            0x05
#define RF_FDEVLSB_80000            0x1F
#define RF_FDEVMSB_85000            0x05
#define RF_FDEVLSB_85000            0x71
#define RF_FDEVMSB_90000            0x05
#define RF_FDEVLSB_90000            0xC3
#define RF_FDEVMSB_95000            0x06
#define RF_FDEVLSB_95000            0x14
#define RF_FDEVMSB_100000           0x06
#define RF_FDEVLSB_100000           0x66
#define RF_FDEVMSB_110000           0x07
#define RF_FDEVLSB_110000           0x0A
#define RF_FDEVMSB_120000           0x07
#define RF_FDEVLSB_120000           0xAE
#define RF_FDEVMSB_130000           0x08
#define RF_FDEVLSB_130000           0x52
#define RF_FDEVMSB_140000           0x08
#define RF_FDEVLSB_140000           0xF6
#define RF_FDEVMSB_150000           0x09
#define RF_FDEVLSB_150000           0x9A
#define RF_FDEVMSB_160000           0x0A
#define RF_FDEVLSB_160000           0x3D
#define RF_FDEVMSB_170000           0x0A
#define RF_FDEVLSB_170000           0xE1
#define RF_FDEVMSB_180000           0x0B
#define RF_FDEVLSB_180000           0x85
#define RF_FDEVMSB_190000           0x0C
#define RF_FDEVLSB_190000           0x29
#define RF_FDEVMSB_200000           0x0C
#define RF_FDEVLSB_200000           0xCD
#define RF_FDEVMSB_210000           0x0D
#define RF_FDEVLSB_210000           0x71
#define RF_FDEVMSB_220000           0x0E
#define RF_FDEVLSB_220000           0x14
#define RF_FDEVMSB_230000           0x0E
#define RF_FDEVLSB_230000           0xB8
#define RF_FDEVMSB_240000           0x0F
#define RF_FDEVLSB_240000           0x5C
#define RF_FDEVMSB_250000           0x10
#define RF_FDEVLSB_250000           0x00
#define RF_FDEVMSB_260000           0x10
#define RF_FDEVLSB_260000           0xA4
#define RF_FDEVMSB_270000           0x11
#define RF_FDEVLSB_270000           0x48
#define RF_FDEVMSB_280000           0x11
#define RF_FDEVLSB_280000           0xEC
#define RF_FDEVMSB_290000           0x12
#define RF_FDEVLSB_290000           0x8F
#define RF_FDEVMSB_300000           0x13
#define RF_FDEVLSB_300000           0x33

// RegFdev - frequency deviation (Hz)
#define RF_FDEVMSB_2000             0x00
#define RF_FDEVLSB_2000             0x21
#define RF_FDEVMSB_5000             0x00  // Default
#define RF_FDEVLSB_5000             0x52  // Default
#define RF_FDEVMSB_7500             0x00
#define RF_FDEVLSB_7500             0x7B
#define RF_FDEVMSB_10000            0x00
#define RF_FDEVLSB_10000            0xA4
#define RF_FDEVMSB_15000            0x00
#define RF_FDEVLSB_15000            0xF6
#define RF_FDEVMSB_20000            0x01
#define RF_FDEVLSB_20000            0x48
#define RF_FDEVMSB_25000            0x01
#define RF_FDEVLSB_25000            0x9A
#define RF_FDEVMSB_30000            0x01
#define RF_FDEVLSB_30000            0xEC
#define RF_FDEVMSB_35000            0x02
#define RF_FDEVLSB_35000            0x3D
#define RF_FDEVMSB_40000            0x02
#define RF_FDEVLSB_40000            0x8F
#define RF_FDEVMSB_45000            0x02
#define RF_FDEVLSB_45000            0xE1
#define RF_FDEVMSB_50000            0x03
#define RF_FDEVLSB_50000            0x33
#define RF_FDEVMSB_55000            0x03
#define RF_FDEVLSB_55000            0x85
#define RF_FDEVMSB_60000            0x03
#define RF_FDEVLSB_60000            0xD7
#define RF_FDEVMSB_65000            0x04
#define RF_FDEVLSB_65000            0x29
#define RF_FDEVMSB_70000            0x04
#define RF_FDEVLSB_70000            0x7B
#define RF_FDEVMSB_75000            0x04
#define RF_FDEVLSB_75000            0xCD
#define RF_FDEVMSB_80000            0x05
#define RF_FDEVLSB_80000            0x1F
#define RF_FDEVMSB_85000            0x05
#define RF_FDEVLSB_85000            0x71
#define RF_FDEVMSB_90000            0x05
#define RF_FDEVLSB_90000            0xC3
#define RF_FDEVMSB_95000            0x06
#define RF_FDEVLSB_95000            0x14
#define RF_FDEVMSB_100000           0x06
#define RF_FDEVLSB_100000           0x66
#define RF_FDEVMSB_110000           0x07
#define RF_FDEVLSB_110000           0x0A
#define RF_FDEVMSB_120000           0x07
#define RF_FDEVLSB_120000           0xAE
#define RF_FDEVMSB_130000           0x08
#define RF_FDEVLSB_130000           0x52
#define RF_FDEVMSB_140000           0x08
#define RF_FDEVLSB_140000           0xF6
#define RF_FDEVMSB_150000           0x09
#define RF_FDEVLSB_150000           0x9A
#define RF_FDEVMSB_160000           0x0A
#define RF_FDEVLSB_160000           0x3D
#define RF_FDEVMSB_170000           0x0A
#define RF_FDEVLSB_170000           0xE1
#define RF_FDEVMSB_180000           0x0B
#define RF_FDEVLSB_180000           0x85
#define RF_FDEVMSB_190000           0x0C
#define RF_FDEVLSB_190000           0x29
#define RF_FDEVMSB_200000           0x0C
#define RF_FDEVLSB_200000           0xCD
#define RF_FDEVMSB_210000           0x0D
#define RF_FDEVLSB_210000           0x71
#define RF_FDEVMSB_220000           0x0E
#define RF_FDEVLSB_220000           0x14
#define RF_FDEVMSB_230000           0x0E
#define RF_FDEVLSB_230000           0xB8
#define RF_FDEVMSB_240000           0x0F
#define RF_FDEVLSB_240000           0x5C
#define RF_FDEVMSB_250000           0x10
#define RF_FDEVLSB_250000           0x00
#define RF_FDEVMSB_260000           0x10
#define RF_FDEVLSB_260000           0xA4
#define RF_FDEVMSB_270000           0x11
#define RF_FDEVLSB_270000           0x48
#define RF_FDEVMSB_280000           0x11
#define RF_FDEVLSB_280000           0xEC
#define RF_FDEVMSB_290000           0x12
#define RF_FDEVLSB_290000           0x8F
#define RF_FDEVMSB_300000           0x13
#define RF_FDEVLSB_300000           0x33

// RegPaLevel
#define RF_PALEVEL_PA0_ON           0x80  // Default
#define RF_PALEVEL_PA0_OFF          0x00
#define RF_PALEVEL_PA1_ON           0x40
#define RF_PALEVEL_PA1_OFF          0x00  // Default
#define RF_PALEVEL_PA2_ON           0x20
#define RF_PALEVEL_PA2_OFF          0x00  // Default

// RegOcp
#define RF_OCP_OFF                  0x0F
#define RF_OCP_ON                   0x1A  // Default

/* TODO WHAT IS THIS AND DO WE NEED IT */
#define null                    0
#define COURSE_TEMP_COEF        -90         // puts the temperature reading in the ballpark, user can fine tune the returned value
#define RF69_BROADCAST_ADDR     0
#define RF69_CSMA_LIMIT_MS      1000
#define RF69_TX_LIMIT_MS        1000
#define RF69_FSTEP              61.03515625         // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet)
#define RFM69_CTL_SENDACK       0x80
#define RFM69_CTL_REQACK        0x40
#define RFM69_ACK_TIMEOUT       30  // 30ms roundtrip req for 61byte packets


/* DEFINE CONFIGURATION REGISTERS */
#define REG_FIFO                0x00
#define REG_OPMODE              0x01
#define REG_DATAMODUL           0x02
#define REG_BITRATEMSB          0x03
#define REG_BITRATELSB          0x04
#define REG_FDEVMSB             0x05
#define REG_FDEVLSB             0x06
#define REG_FRFMSB              0x07
#define REG_FRFMID              0x08
#define REG_FRFLSB              0x09
#define REG_OSC1                0x0A
#define REG_AFCCTRL             0x0B
#define REG_LOWBAT              0x0C
#define REG_LISTEN1             0x0D
#define REG_LISTEN2             0x0E
#define REG_LISTEN3             0x0F
#define REG_VERSION             0x10
#define REG_PALEVEL             0x11
#define REG_PARAMP              0x12
#define REG_OCP                 0x13
#define REG_AGCREF              0x14  // not present on RFM69/SX1231
#define REG_AGCTHRESH1          0x15  // not present on RFM69/SX1231
#define REG_AGCTHRESH2          0x16  // not present on RFM69/SX1231
#define REG_AGCTHRESH3          0x17  // not present on RFM69/SX1231
#define REG_LNA                 0x18
#define REG_RXBW                0x19
#define REG_AFCBW               0x1A
#define REG_OOKPEAK             0x1B
#define REG_OOKAVG              0x1C
#define REG_OOKFIX              0x1D
#define REG_AFCFEI              0x1E
#define REG_AFCMSB              0x1F
#define REG_AFCLSB              0x20
#define REG_FEIMSB              0x21
#define REG_FEILSB              0x22
#define REG_RSSICONFIG          0x23
#define REG_RSSIVALUE           0x24
#define REG_DIOMAPPING1         0x25
#define REG_DIOMAPPING2         0x26
#define REG_IRQFLAGS1           0x27
#define REG_IRQFLAGS2           0x28
#define REG_RSSITHRESH          0x29
#define REG_RXTIMEOUT1          0x2A
#define REG_RXTIMEOUT2          0x2B
#define REG_PREAMBLEMSB         0x2C
#define REG_PREAMBLELSB         0x2D
#define REG_SYNCCONFIG          0x2E
#define REG_SYNCVALUE1          0x2F
#define REG_SYNCVALUE2          0x30
#define REG_SYNCVALUE3          0x31
#define REG_SYNCVALUE4          0x32
#define REG_SYNCVALUE5          0x33
#define REG_SYNCVALUE6          0x34
#define REG_SYNCVALUE7          0x35
#define REG_SYNCVALUE8          0x36
#define REG_PACKETCONFIG1       0x37
#define REG_PAYLOADLENGTH       0x38
#define REG_NODEADRS            0x39
#define REG_BROADCASTADRS       0x3A
#define REG_AUTOMODES           0x3B
#define REG_FIFOTHRESH          0x3C
#define REG_PACKETCONFIG2       0x3D
#define REG_AESKEY1             0x3E
#define REG_AESKEY2             0x3F
#define REG_AESKEY3             0x40
#define REG_AESKEY4             0x41
#define REG_AESKEY5             0x42
#define REG_AESKEY6             0x43
#define REG_AESKEY7             0x44
#define REG_AESKEY8             0x45
#define REG_AESKEY9             0x46
#define REG_AESKEY10            0x47
#define REG_AESKEY11            0x48
#define REG_AESKEY12            0x49
#define REG_AESKEY13            0x4A
#define REG_AESKEY14            0x4B
#define REG_AESKEY15            0x4C
#define REG_AESKEY16            0x4D
#define REG_TEMP1               0x4E
#define REG_TEMP2               0x4F
#define REG_TESTLNA             0x58
#define REG_TESTPA1             0x5A  // only present on RFM69HW/SX1231H
#define REG_TESTPA2             0x5C  // only present on RFM69HW/SX1231H
#define REG_TESTDAGC            0x6F

#endif
/* END OF FILE */