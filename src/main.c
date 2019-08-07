/******************************************************************************
*   FILE: main.c
*
*   PURPOSE: Main source
*
*   DEVICE: PIC18F66K22
*
*   COMPILER: Microchip XC8 v1.32
*
*   IDE: MPLAB X v3.45
*
*   TODO:  When reading from timer one in 16bit mode, we must read from the LOW byte first!
*
*   NOTE:
*
******************************************************************************/

#include "main.h" //Include header file associated with main.c

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Disabled - Controlled by SRETEN bit)
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG    // SOSC Power Selection and mode Configuration bits (Digital (SCLKI) mode)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC = INTIO1    // Internal RC Oscillator with output on OSC2.  p45/550.
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = ON      // Power Up Timer (Enabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 1         // Brown-out Reset Voltage bits (2.7V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Disabled in HW
#pragma config WDTPS = 1024     // Watchdog Postscaler (1:1024)

// CONFIG3L
#pragma config RTCOSC = SOSCREF // RTCC Clock Select (RTCC uses SOSC)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 Mux (RC1)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RG5 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-03FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 04000-07FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 08000-0BFFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 0C000-0FFFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBRT0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBRT1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBRT2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBRT3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBRTB = OFF      // Table Read Protect Boot (Disabled)

struct GlobalInformation gblinfo;

void main()
{
    // uint8_t * data;
    uint8_t data[30];
    uint8_t test_data;
    uint8_t rfm_temp;
    uint8_t i;
    bool status;
    float battery_voltage;
    
    SetUp();
    //test_data = SPI1Read(REG_RSSITHRESH);
    battery_voltage = GetBatteryVoltage();

    DispSetContrast(60);
    DispRefresh();
    DispWriteString("Battery Voltage");
    DispLineTwo();
    DispWriteFloat(battery_voltage);
    tick100msDelay(35);

    // DispRefresh();
    // DispWriteString("BIN Print Tst");
    // DispLineTwo();
    // DispWrite8b(253); 
    // tick100msDelay(50);

    // *data = "Testing";
    for(i=0;i<10;i++) {

        DispRefresh();
        DispWriteString("Sending message...");
        tick100msDelay(10);
        
        status = RFMsend("HELLO",0x05);
        DispRefresh();
        DispWriteString("Done...");
        tick100msDelay(10);
    }    

    disp_enable = DISPLAY_OFF;
    /* END OF DEBUG CODE */
    
    while (true) {
        if(gblinfo.flag20ms) {
            gblinfo.flag20ms = false;
            Events20ms();
        }
        
        if(gblinfo.flag100ms) {
            gblinfo.flag100ms = false;
            Events100ms();
        }

        if(gblinfo.flag500ms) {
            gblinfo.flag100ms = false;
            Events500ms();
        }


        if(gblinfo.flag1000ms) {
            gblinfo.flag1000ms = false;
            Events1000ms();
        }
    }

}   //END Main()

void SetUp(void)
{
    /* PIN DIRECTIONS FOR ANALOG SELECT */
    TRISA5 = input;
    
    /* PIN DIRECTION FOR LEDs */
    TRISB5 = output;                    // Output for system health LED.  

    /* PIN DIRECTION FOR PUSH BUTTONS */
    TRISB0 = input;                     // Push Button 1 input
    TRISB1 = input;                     // Push Button 2 input

    /* PIN DIRECTION FOR RADIO MODULE SPI AND GPIO */
    TRISD3 = output;                    // RFM Module reset signal
    TRISD4 = output;                    // RFM Module SPI MOSI signal
    TRISD5 = input;                     // RFM Module SPI MISO signal
    TRISD6 = output;                    // RFM module SPI SCK signal
    TRISD7 = output;                    // RFM module SPI #CS signal
    
    TRISE2 = input;                    // RFM DIO5 signal
    TRISE3 = input;                    // RFM DIO4 signal
    TRISE4 = input;                    // RFM DIO3 signal
    TRISE5 = input;                    // RFM DIO2 signal
    TRISE6 = input;                    // RFM DIO1 signal
    TRISE7 = input;                    // RFM DIO0 and IRQ signal

    /* PIN DIRECTION FOR DISPLAY SPI AND GPIO */
    TRISB4 = output;                    // Active low output enables display
    TRISC0 = output;                    // Display data/command outputs
    TRISC1 = output;                    // Display reset signal 
    TRISC2 = output;                    // Display SPI chip select signal
    TRISC3 = output;                    // Display SPI SCK signal
    TRISC5 = output;                    // Display SPI MOSI signal

    /* UNUSED PINS DEFINED AS OUTPUTS TO SAVE POWER */
    TRISA1 = output;
    TRISA3 = output;
    TRISA4 = output;
    TRISA5 = output;
    
    TRISB2 = output;
    TRISB3 = output;
    
    TRISC4 = output;
    TRISC6 = output;
    TRISC7 = output;
    
    TRISG4 = output;
    TRISG3 = output;
    TRISG2 = output;
    TRISG1 = output;
    TRISG0 = output;

    TRISF7 = output;
    TRISF6 = output;
    TRISF5 = output;
    TRISF4 = output;
    TRISF3 = output;
    TRISF2 = output;
    TRISF1 = output;
    
    TRISE1 = output;
    TRISE0 = output;
    
    TRISD2 = output;
    TRISD1 = output;
    TRISD0 = output;

    /* INITIAL CONDITION OF HEALTH LED */
    health_led = LED_OFF;

    /* TURN OFF THE DISPLAY */
    disp_enable = DISPLAY_OFF;

    Init_Interrupts();                          // Set up interrupts  

    /* INITIALIZE SPI INTERFACE FOR DISPLAY */
    SPI1Init();

    /* INITIALIZE SPI INTERFACE FOR DISPLAY */
    SPI2Init();

    AnalogRefSel(REF2D048, EXTREF);             // Use internal 2.048V reference and External VREF pin for negative reference -- page 216/380
    InitA2D(RIGHT_JUSTIFIED, 4);                // Set up AD (Justification, Acq Time TADs) ==> (Right, 4 TAD) -- page 361/550

    /* SETUP ANALOG CHANNELS */  
    ANCON0 = 0x00;                      // Analog channels 7-0 are configured for digital inputs. p.363     
    ANCON1 = 0x00;                      // Analog channel 10-8 are configred for digital inputs. p.364
    EnableAnalogCh(BAT_VOLTAGE_CH);     // Channel for battery voltage level
    
    gblinfo.tick20ms = 0;       // Initialize 10ms tick counter
    gblinfo.tick100ms = 0;      // Initialize 100ms tick counter 
    gblinfo.tick500ms = 0;      // Initialize 500ms tick counter
    gblinfo.tick1000ms = 0;     // Initialize 1000ms tick counter

    /* TIMER FOR APPLICATION INTERRUPTS */
    Timer0Init(TMR0_INTUP_SETTING, TMR0_PRESCALER, 0); //ARGS: interrupts = yes, prescaler = 2, clksource = FOSC/4
    Timer0On();             
    
    /* INITIALIZE DISPALY */
    tick100msDelay(5);
    DispInit();

    /* INITIALIZE RADIO MODULE */
    RFM_RST = 1;                    //Active low reset, so bring high
    
    tick100msDelay(5);
    RFM_RST = 0;
    tick100msDelay(5);
    RFM_RST = 1;                    //Active low reset, so bring high
    tick100msDelay(5);

    // gblinfo.payloadlen  = 0;
    // gblinfo.rssi_lvl    = 0;
    RFMInitialize( );        // Takes parameters netwrokID and node ID  //TODO I'm not sure if these values are okay?
    if(!RFMsetFrequency(915.0)) {
        DispSetContrast(60);
        DispRefresh();
        DispWriteString("SET FREQ FAILED");
        tick100msDelay(35);
    }
}

void tick100msDelay(uint16_t ticks)
{
    uint16_t i = 0;
    uint16_t tick = 0; //Used to lock time value
    for (i = ticks; i > 0; i--)
    {
        tick = gblinfo.tick100ms;
        while (tick == gblinfo.tick100ms); //Wait for time to wrap around (in one half tick1000mond)
    }
}

void tick20msDelay(uint16_t ticks)  
{
    uint16_t i = 0;
    uint16_t tick = 0; //Used to lock time value
    for (i = ticks; i > 0; i--)
    {
        tick = gblinfo.tick20ms;
        while (tick == gblinfo.tick20ms); //Wait for time to wrap around (in one half tick1000mond)
    }
}

float GetBatteryVoltage ( void ) {
    float battery_voltage;
    battery_voltage = ReadA2D(BAT_VOLTAGE_CH,1);
    battery_voltage = battery_voltage * (2.048/4096);
    battery_voltage *= 1.6667;                          // Convert to account for resistor divider
    
    return (battery_voltage);
}

/* END OF FILE */