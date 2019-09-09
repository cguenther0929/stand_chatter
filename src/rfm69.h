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
#include <string.h>
#include "struct.h"
#include "config.h"     //Project specific header file
#include "main.h"
#include "spi.h"
#include "disp.h"

#define RFM_MODE_SLEEP      0   
#define RFM_MODE_STANDBY    1
#define RFM_MODE_RX         2
#define RFM_MODE_TX         3

/*
 * Function: void RFMInitialize( void )
 * --------------------
 * Initialize the radio module.  This entails making
 * multiple writes to various configuration registers
 *
 * returns: Nothing
 */
void RFMInitialize( void );

/*
 * Function: void RFMsetMode (uint8_t mode)
 * --------------------
 * Set the mode of the radio by passing in the desired
 * mode.  
 *
 * returns: Nothing
 */
void RFMsetMode(uint8_t mode);

/*
 * Function: bool RFMsend(const char * data, uint8_t len)
 * --------------------
 * Send a packet.  The packet (array) and the length of the 
 * packet shall be passed to the function.  If successful, 
 * the function will return true.  .  
 *
 * returns: True if successful, False if not
 */
bool RFMsend(const char * data, uint8_t len);

/*
 * Function: void GetRxData( void )
 * --------------------
 * It is known that a packet has arrived prior to calling.
 * The rxbuffer (in the structure) will be updated with data
 * from the new packet.
 *
 * returns: Nothing
 */
void GetRxData( void );

/*
 * Function: bool RFMsetFrequency (float centre)
 * --------------------
 * Set IF frequency of  the radio module
 *
 * returns: Nothing
 */
bool RFMsetFrequency (float centre);

/*
 * Function: void RFMsetTxPower(int8_t power, bool useRFO)
 * --------------------
 * Set transmit power of the radio
 *
 * returns: Nothing
 */
void RFMsetTxPower(int8_t power, bool useRFO);

/*
 * Function: void RFMsetPreambleLength(uint16_t bytes)
 * --------------------
 * Set preamble length for packets sent and received
 *
 * returns: Nothing
 */
void RFMsetPreambleLength(uint16_t bytes);

/*
 * Function: bool RFMtxInProgress( void )
 * --------------------
 * This function will be called to see if a transmission 
 * is in progress.  
 *
 * returns: True if a packet is being transmitted
 */
bool RFMtxInProgress( void );

/*
 * Function: bool ReceivedPacket( void )
 * --------------------
 * Checks to see if a packet has been received.  
 * if received, call function GetRxData.  
 *
 * returns: True if a packet is being transmitted
 */
bool ReceivedPacket( void );  

// This is the address that indicates a broadcast
#define RFM_BROADCAST_ADDRESS   0xff

// For this project, this is how large we'll allow the buffer to be
#define RECEIVE_BUFFER_SIZE     16

// Max number of octets the LORA Rx/Tx FIFO can hold
#define RFM_FIFO_SIZE           255

// This is the maximum number of bytes that can be carried by the LORA.
// We use some for headers, keeping fewer for RadioHead messages
#define RFM_MAX_PAYLOAD_LEN     RFM_FIFO_SIZE

// The length of the headers we add.
// The headers are inside the LORA's payload
#define RFM_HEADER_LEN          4

// This is the maximum message length that can be supported by this driver. 
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
// Here we allow for 1 byte message length, 4 bytes headers, user data and 2 bytes of FCS
#ifndef RFM_MAX_MESSAGE_LEN
 #define RFM_MAX_MESSAGE_LEN (RFM_MAX_PAYLOAD_LEN - RFM_HEADER_LEN)
#endif

// The crystal oscillator frequency of the module
#define RH_RF95_FXOSC 32000000.0

// The Frequency Synthesizer step = RH_RF95_FXOSC / 2^19
#define RH_RF95_FSTEP  (RH_RF95_FXOSC / 524288)

/* RFM69 PARAMETERS */
#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access

// Register names (LoRa Mode, from table 85)
#define RFM_REG_00_FIFO                                 0x00
#define RH_RF95_REG_01_OP_MODE                          0x01
#define RH_RF95_REG_02_RESERVED                         0x02
#define RH_RF95_REG_03_RESERVED                         0x03
#define RH_RF95_REG_04_RESERVED                         0x04
#define RH_RF95_REG_05_RESERVED                         0x05
#define RH_RF95_REG_06_FRF_MSB                          0x06
#define RH_RF95_REG_07_FRF_MID                          0x07
#define RH_RF95_REG_08_FRF_LSB                          0x08
#define RH_RF95_REG_09_PA_CONFIG                        0x09
#define RH_RF95_REG_0A_PA_RAMP                          0x0a
#define RH_RF95_REG_0B_OCP                              0x0b
#define RH_RF95_REG_0C_LNA                              0x0c
#define RH_RF95_REG_0D_FIFO_ADDR_PTR                    0x0d
#define RH_RF95_REG_0E_FIFO_TX_BASE_ADDR                0x0e
#define RH_RF95_REG_0F_FIFO_RX_BASE_ADDR                0x0f
#define RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR             0x10
#define RH_RF95_REG_11_IRQ_FLAGS_MASK                   0x11
#define RH_RF95_REG_12_IRQ_FLAGS                        0x12
#define RH_RF95_REG_13_RX_NB_BYTES                      0x13
#define RH_RF95_REG_14_RX_HEADER_CNT_VALUE_MSB          0x14
#define RH_RF95_REG_15_RX_HEADER_CNT_VALUE_LSB          0x15
#define RH_RF95_REG_16_RX_PACKET_CNT_VALUE_MSB          0x16
#define RH_RF95_REG_17_RX_PACKET_CNT_VALUE_LSB          0x17
#define RH_RF95_REG_18_MODEM_STAT                       0x18
#define RH_RF95_REG_19_PKT_SNR_VALUE                    0x19
#define RH_RF95_REG_1A_PKT_RSSI_VALUE                   0x1A
#define RH_RF95_REG_1B_RSSI_VALUE                       0x1B
#define RH_RF95_REG_1C_HOP_CHANNEL                      0x1C
#define RH_RF95_REG_1D_MODEM_CONFIG1                    0x1D         
#define RH_RF95_REG_1E_MODEM_CONFIG2                    0x1E         
#define RH_RF95_REG_1F_SYMB_TIMEOUT_LSB                 0x1F
#define RH_RF95_REG_20_PREAMBLE_MSB                     0x20            // Default value is 0x00
#define RH_RF95_REG_21_PREAMBLE_LSB                     0x21            // Default value is 0x08
#define RH_RF95_REG_22_TX_PALD_LG                       0x22
#define RH_RF95_REG_23_MAX_PAYLOAD_LENGTH               0x23
#define RH_RF95_REG_24_HOP_PERIOD                       0x24
#define RH_RF95_REG_25_FIFO_RX_BYTE_ADDR                0x25
#define RH_RF95_REG_26_MODEM_CONFIG3                    0x26

#define RH_RF95_REG_40_DIO_MAPPING1                     0x40
#define RH_RF95_REG_41_DIO_MAPPING2                     0x41
#define RH_RF95_REG_42_VERSION                          0x42

#define RH_RF95_REG_4B_TCXO                             0x4B
#define RH_RF95_REG_4D_PA_DAC                           0x4D
#define RH_RF95_REG_5B_FORMER_TEMP                      0x5B
#define RH_RF95_REG_61_AGC_REF                          0x61
#define RH_RF95_REG_62_AGC_THRESH1                      0x62
#define RH_RF95_REG_63_AGC_THRESH2                      0x63
#define RH_RF95_REG_64_AGC_THRESH3                      0x64

// RH_RF95_REG_01_OP_MODE                             0x01
#define RH_RF95_LONG_RANGE_MODE                       0x80
#define RH_RF95_ACCESS_SHARED_REG                     0x40
#define RH_RF95_MODE                                  0x07
#define RH_RF95_MODE_SLEEP                            0x00
#define RH_RF95_MODE_STDBY                            0x01
#define RH_RF95_MODE_FSTX                             0x02
#define RH_RF95_MODE_TX                               0x03
#define RH_RF95_MODE_FSRX                             0x04
#define RH_RF95_MODE_RXCONTINUOUS                     0x05
#define RH_RF95_MODE_RESERVE_1                        0x06
#define RH_RF95_MODE_RESERVE_2                        0x07

// RH_RF95_REG_09_PA_CONFIG                           0x09
#define RH_RF95_PA_SELECT                             0x80
#define RH_RF95_MAX_POWER                             0x70
#define RH_RF95_OUTPUT_POWER                          0x0f

// RH_RF95_REG_0A_PA_RAMP                             0x0a
#define RH_RF95_LOW_PN_TX_PLL_OFF                     0x10
#define RH_RF95_PA_RAMP                               0x0f
#define RH_RF95_PA_RAMP_3_4MS                         0x00
#define RH_RF95_PA_RAMP_2MS                           0x01
#define RH_RF95_PA_RAMP_1MS                           0x02
#define RH_RF95_PA_RAMP_500US                         0x03
#define RH_RF95_PA_RAMP_250US                         0x0
#define RH_RF95_PA_RAMP_125US                         0x05
#define RH_RF95_PA_RAMP_100US                         0x06
#define RH_RF95_PA_RAMP_62US                          0x07
#define RH_RF95_PA_RAMP_50US                          0x08
#define RH_RF95_PA_RAMP_40US                          0x09
#define RH_RF95_PA_RAMP_31US                          0x0a
#define RH_RF95_PA_RAMP_25US                          0x0b
#define RH_RF95_PA_RAMP_20US                          0x0c
#define RH_RF95_PA_RAMP_15US                          0x0d
#define RH_RF95_PA_RAMP_12US                          0x0e
#define RH_RF95_PA_RAMP_10US                          0x0f

// RH_RF95_REG_0B_OCP                                 0x0b
#define RH_RF95_OCP_ON                                0x20
#define RH_RF95_OCP_TRIM                              0x1f

// RH_RF95_REG_0C_LNA                                 0x0c
#define RH_RF95_LNA_GAIN                              0xe0
#define RH_RF95_LNA_BOOST                             0x03
#define RH_RF95_LNA_BOOST_DEFAULT                     0x00
#define RH_RF95_LNA_BOOST_150PC                       0x11

// RH_RF95_REG_11_IRQ_FLAGS_MASK                      0x11
#define RH_RF95_RX_TIMEOUT_MASK                       0x80
#define RH_RF95_RX_DONE_MASK                          0x40
#define RH_RF95_PAYLOAD_CRC_ERROR_MASK                0x20
#define RH_RF95_VALID_HEADER_MASK                     0x10
#define RH_RF95_TX_DONE_MASK                          0x08
#define RH_RF95_CAD_DONE_MASK                         0x04
#define RH_RF95_FHSS_CHANGE_CHANNEL_MASK              0x02
#define RH_RF95_CAD_DETECTED_MASK                     0x01

// RH_RF95_REG_12_IRQ_FLAGS                           0x12
#define RH_RF95_RX_TIMEOUT                            0x80
#define RH_RF95_RX_DONE                               0x40
#define RH_RF95_PAYLOAD_CRC_ERROR                     0x20
#define RH_RF95_VALID_HEADER                          0x10
#define RH_RF95_TX_DONE                               0x08
#define RH_RF95_CAD_DONE                              0x04
#define RH_RF95_FHSS_CHANGE_CHANNEL                   0x02
#define RH_RF95_CAD_DETECTED                          0x01

// RH_RF95_REG_18_MODEM_STAT                          0x18
#define RH_RF95_RX_CODING_RATE                        0xe0
#define RH_RF95_MODEM_STATUS_CLEAR                    0x10
#define RH_RF95_MODEM_STATUS_HEADER_INFO_VALID        0x08
#define RH_RF95_MODEM_STATUS_RX_ONGOING               0x04
#define RH_RF95_MODEM_STATUS_SIGNAL_SYNCHRONIZED      0x02
#define RH_RF95_MODEM_STATUS_SIGNAL_DETECTED          0x01

// RH_RF95_REG_1C_HOP_CHANNEL                         0x1c
#define RH_RF95_PLL_TIMEOUT                           0x80
#define RH_RF95_RX_PAYLOAD_CRC_IS_ON                  0x40
#define RH_RF95_FHSS_PRESENT_CHANNEL                  0x3f

// RH_RF95_REG_1D_MODEM_CONFIG1                       0x1d
#define RH_RF95_BW                                    0xc0
#define RH_RF95_BW_125KHZ                             0x00
#define RH_RF95_BW_250KHZ                             0x40
#define RH_RF95_BW_500KHZ                             0x80
#define RH_RF95_BW_RESERVED                           0xc0
#define RH_RF95_CODING_RATE                           0x38
#define RH_RF95_CODING_RATE_4_5                       0x00
#define RH_RF95_CODING_RATE_4_6                       0x08
#define RH_RF95_CODING_RATE_4_7                       0x10
#define RH_RF95_CODING_RATE_4_8                       0x18
#define RH_RF95_IMPLICIT_HEADER_MODE_ON               0x04
#define RH_RF95_RX_PAYLOAD_CRC_ON                     0x02
#define RH_RF95_LOW_DATA_RATE_OPTIMIZE                0x01

// RH_RF95_REG_1E_MODEM_CONFIG2                       0x1e
#define RH_RF95_SPREADING_FACTOR                      0xf0
#define RH_RF95_SPREADING_FACTOR_64CPS                0x60
#define RH_RF95_SPREADING_FACTOR_128CPS               0x70
#define RH_RF95_SPREADING_FACTOR_256CPS               0x80
#define RH_RF95_SPREADING_FACTOR_512CPS               0x90
#define RH_RF95_SPREADING_FACTOR_1024CPS              0xa0
#define RH_RF95_SPREADING_FACTOR_2048CPS              0xb0
#define RH_RF95_SPREADING_FACTOR_4096CPS              0xc0
#define RH_RF95_TX_CONTINUOUS_MOE                     0x08
#define RH_RF95_AGC_AUTO_ON                           0x04
#define RH_RF95_SYM_TIMEOUT_MSB                       0x03

// RH_RF95_REG_4D_PA_DAC                              0x4d
#define RH_RF95_PA_DAC_DISABLE                        0x04
#define RH_RF95_PA_DAC_ENABLE                         0x07



#endif
/* END OF FILE */