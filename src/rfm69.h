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

/* RFM69 PARAMETERS */
//TODO do we need all of these?  Should we clean up?  Taken from github
#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access
#define RF69_MODE_SLEEP         0 // XTAL OFF
#define RF69_MODE_STANDBY       1 // XTAL ON
#define RF69_MODE_SYNTH         2 // PLL ON
#define RF69_MODE_RX            3 // RX MODE
#define RF69_MODE_TX            4 // TX MODE

/* LoRa RADIO FREQUENCY BANDS */
typedef enum RFM69_FREQ {
    RF69_315MHZ,
    RF69_433MHZ,
    RF69_868MHZ,
    RF69_915MHZ
} RFM69_FREQ;


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