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
struct RFM rfm;

void main()
{
    uint8_t * data;
    uint8_t temp_data;
    uint8_t rfm_temp;
    uint8_t i;
    bool status;
    
    SetUp();
    
    PrintSplashScreen();

    //TODO the following is just for testing!
    strcpy(rfm.rxdata,"Hello World");

    gblinfo.disp_tmr_active = true;             // Begin the timer to kill display after timeout

    uint8_t msg_order = 0;
    char pre_loaded_message[NUM_MESSAGES][16];
    strcpy(pre_loaded_message[msg_order++], "Seeing anything?"); 
    strcpy(pre_loaded_message[msg_order++], "I'm surrounded");   
    strcpy(pre_loaded_message[msg_order++], "Did you shoot?");   
    strcpy(pre_loaded_message[msg_order++], "Need help, man?");  
    strcpy(pre_loaded_message[msg_order++], "Yes, need help.");  
    strcpy(pre_loaded_message[msg_order++], "Just bagged one!"); 
    strcpy(pre_loaded_message[msg_order++], "Leaving in 10.");   
    strcpy(pre_loaded_message[msg_order++], "On the way down."); 
    strcpy(pre_loaded_message[msg_order++], "Fell and hurt!");   
    strcpy(pre_loaded_message[msg_order], "Ignore prev msg!");  
    
    while (true) {
        if(gblinfo.flag20ms) {
            gblinfo.flag20ms = false;
            EvaluateButtonInputs();
            EvaluateState(pre_loaded_message);
        }
        
        if(gblinfo.flag100ms) {
            gblinfo.flag100ms = false;
            (gblinfo.splash_screen_tmr_active)?(gblinfo.splash_screen_tmr++):(gblinfo.splash_screen_tmr=0);
        }

        if(gblinfo.flag500ms) {
            gblinfo.flag100ms = false;
        }


        if(gblinfo.flag1000ms) {
            gblinfo.flag1000ms = false;
            DisplayDwellTmr(DISP_TMR_CNT);
            health_led = ~health_led;
        }

    }

}   //END Main()


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

void PrintSplashScreen( void ) {
    float battery_voltage = 0.0;
    
    DispSetContrast(60);
    DispRefresh();
    DispWriteString("Stand Chatter");
    DispLineTwo();
    DispWriteString("FW v"); DispWriteChar(MAJVER + 0x30); DispWriteChar(MINVER + 0x30); DispWriteChar(BUGVER + 0x30);
    tick100msDelay(10);
    
    battery_voltage = GetBatteryVoltage();

    DispRefresh();
    DispWriteString("Battery Voltage");
    DispLineTwo();
    DispWriteFloat(battery_voltage);
    tick100msDelay(10);

    DispRefresh();
    DispWriteString("Waiting for");
    DispLineTwo();
    DispWriteString("message");

}

void EvaluateState( char pre_loaded_message[NUM_MESSAGES][16]) {
    uint8_t temp_data;

    if(ReceivedPacket()) {
        temp_data = gblinfo.current_state;
        GetRxData();
        gblinfo.current_state = temp_data;  
        if (gblinfo.current_state == STATE_IDLE_DISP) {
            DispRefresh();
            DispWriteString("NEW MSG:");
            DispLineTwo();
            DispWriteString(rfm.rxdata);
        }
    }
    
    switch(gblinfo.current_state) {
        // TODO add code that checks to see if we have a new message
        case STATE_IDLE_DISP:
            
            if ((gblinfo.btn_lt_pressed || gblinfo.btn_rt_pressed || gblinfo.btn_both_pressed) && (disp_enable == DISPLAY_OFF)) {    // Simply turn display back on
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
            }
            
            else if((gblinfo.btn_lt_pressed) && (disp_enable == DISPLAY_ON)) {
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
                
                // Transition into display send-able messages 
                gblinfo.msg_to_send = 0;
                DispSetContrast(60);
                DispRefresh();
                DispWriteString("SEND");
                DispLineTwo();
                DispWriteString(pre_loaded_message[gblinfo.msg_to_send]);

                gblinfo.current_state = STATE_SELECT_MSG;
            }

            else if((gblinfo.btn_rt_pressed || gblinfo.btn_both_pressed ) && (disp_enable == DISPLAY_ON)) {
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
            }

            if((gblinfo.disp_seconds_ctr >= MAX_DISP_DWELL) && (disp_enable == DISPLAY_ON)) {
                DispWtLnOne("LAST RECEIVED:");
                DispWtLnTwo(rfm.rxdata);
                DisplayDwellTmr(DISP_TMR_DISABLE);         // Send one to reset the counter
                disp_enable = DISPLAY_OFF;
            }
            break;

        case STATE_SELECT_MSG:
            
            if ((gblinfo.btn_lt_pressed || gblinfo.btn_rt_pressed || gblinfo.btn_both_pressed) && (disp_enable == DISPLAY_OFF)) {    // Simply turn display back on
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
            }
            
            else if(gblinfo.btn_both_pressed && (disp_enable == DISPLAY_ON)) {          
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter

                // Transition to idle state and dispaly last received message
                gblinfo.msg_to_send = 0;
                DispSetContrast(60);
                DispRefresh();
                DispWriteString("LAST RECEIVED:");
                DispWtLnTwo(rfm.rxdata);
                
                gblinfo.current_state = STATE_IDLE_DISP;
            }
            
            else if ((gblinfo.btn_lt_pressed) && (disp_enable == DISPLAY_ON)) {
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
                
                (gblinfo.msg_to_send >= (NUM_MESSAGES-1))?(gblinfo.msg_to_send = 0):(gblinfo.msg_to_send++);
                DispWtLnTwo(pre_loaded_message[gblinfo.msg_to_send]);

            }
            
            else if ((gblinfo.btn_rt_pressed) && (disp_enable == DISPLAY_ON)) {
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
                
                // DispRefresh();          //TODO remove these three lines
                // DispWriteString("DBG MOVING STATE");
                // tick100msDelay(10);
                
                // Transition to confirming sending message
                DispCursorHome();
                DispWriteString("Confirm Sending?");            // Selected message should still be on line two
                gblinfo.current_state = STATE_CONFIRM_MSG;

            }
            
            if((gblinfo.disp_seconds_ctr >= MAX_DISP_DWELL) && (disp_enable == DISPLAY_ON)) {
                DispWtLnOne("LAST RECEIVED:");
                DispWtLnTwo(rfm.rxdata);
                DisplayDwellTmr(DISP_TMR_DISABLE);         // Send one to reset the counter
                disp_enable = DISPLAY_OFF;
                gblinfo.current_state = STATE_IDLE_DISP;
            }
            
            break;
        
        case STATE_CONFIRM_MSG:        
            if ((gblinfo.btn_lt_pressed || gblinfo.btn_rt_pressed || gblinfo.btn_both_pressed) && (disp_enable == DISPLAY_OFF)) {    // Simply turn display back on
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
            }
            
            else if(gblinfo.btn_both_pressed && (disp_enable == DISPLAY_ON)) {      // User wishes to cancel sending 
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter

                // Transition to idle state and dispaly last received message
                gblinfo.msg_to_send = 0;
                DispSetContrast(60);
                DispRefresh();
                DispWriteString("LAST RECEIVED:");
                DispWtLnTwo(rfm.rxdata);
                
                gblinfo.current_state = STATE_IDLE_DISP;
            }
            
            else if(gblinfo.btn_lt_pressed && (disp_enable == DISPLAY_ON)) {      // Do nothing, but wake display 
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
            }
            
            else if ((gblinfo.btn_rt_pressed) && (disp_enable == DISPLAY_ON)) {     // User confirms to send message
                gblinfo.btn_lt_pressed       = false;
                gblinfo.btn_rt_pressed       = false;
                gblinfo.btn_both_pressed    = false;
                DisplayDwellTmr(DISP_TMR_RST);         // Send one to reset the counter
                
                // Transition to confirming sending message
                DispWtLnOne("Sending...");
                gblinfo.splash_screen_tmr_active = true;
                gblinfo.current_state = STATE_TRANSMIT_MSG;
                RFMsend(pre_loaded_message[gblinfo.msg_to_send],sizeof(pre_loaded_message[gblinfo.msg_to_send]));
            }
            
            if((gblinfo.disp_seconds_ctr >= MAX_DISP_DWELL) && (disp_enable == DISPLAY_ON)) {
                gblinfo.msg_to_send = 0;
                DispWtLnOne("LAST RECEIVED:");
                DispWtLnTwo(rfm.rxdata);
                DisplayDwellTmr(DISP_TMR_DISABLE);         // Send one to reset the counter
                disp_enable = DISPLAY_OFF;
                gblinfo.current_state = STATE_IDLE_DISP;
            }
            
            break;

        case STATE_TRANSMIT_MSG:
            if(gblinfo.splash_screen_tmr >= SPLASH_SCREEN_DWELL) {
                gblinfo.current_state = STATE_IDLE_DISP;
                gblinfo.splash_screen_tmr_active = false;
                gblinfo.msg_to_send = 0;
                DispRefresh();
                DispWriteString("LAST RECEIVED:");
                DispWtLnTwo(rfm.rxdata);
            }
            
            break;

        default:
            DispRefresh();
            DispWtLnOne("STATE ERROR");
            gblinfo.current_state = STATE_IDLE_DISP;
            break;
    }

}

void DisplayDwellTmr( uint8_t action ) {
    
    
    switch (action) {
        case DISP_TMR_RST:
            gblinfo.disp_tmr_active = true;
            gblinfo.disp_seconds_ctr = 0;
            disp_enable = DISPLAY_ON;
            break;

        case DISP_TMR_CNT:
            
            if(gblinfo.disp_tmr_active && (gblinfo.disp_seconds_ctr < MAX_DISP_DWELL)) 
                gblinfo.disp_seconds_ctr++;
            else if (!gblinfo.disp_tmr_active)
                gblinfo.disp_seconds_ctr = 0;

            break;

        case DISP_TMR_ENABLE:
            gblinfo.disp_tmr_active = true;
            gblinfo.disp_seconds_ctr = 0;
            break;
        
        case DISP_TMR_DISABLE:
            gblinfo.disp_tmr_active = true;
            gblinfo.disp_seconds_ctr = 0;
            break;
    }
    
}

void EvaluateButtonInputs ( void ) {
    if(PB_LT_PIN == BUTTON_PUSHED && PB_RT_PIN == BUTTON_PUSHED) {
        (gblinfo.btn_lt_press_ctr > 2)?(gblinfo.btn_lt_press_ctr -= 2):(gblinfo.btn_lt_press_ctr = 0);
        (gblinfo.btn_rt_press_ctr > 2)?(gblinfo.btn_rt_press_ctr -= 2):(gblinfo.btn_rt_press_ctr = 0);
        
        gblinfo.btn_both_press_ctr++;
    }
    else if(PB_LT_PIN == BUTTON_PUSHED && PB_RT_PIN == BUTTON_RELEASED) {
        (gblinfo.btn_rt_press_ctr > 2)?(gblinfo.btn_rt_press_ctr -= 2):(gblinfo.btn_rt_press_ctr = 0);
        (gblinfo.btn_both_press_ctr > 2)?(gblinfo.btn_both_press_ctr -= 2):(gblinfo.btn_both_press_ctr = 0);
        
        gblinfo.btn_lt_press_ctr++;
    }
    else if(PB_LT_PIN == BUTTON_RELEASED && PB_RT_PIN == BUTTON_PUSHED) {
        (gblinfo.btn_lt_press_ctr > 2)?(gblinfo.btn_lt_press_ctr -= 2):(gblinfo.btn_lt_press_ctr = 0);
        (gblinfo.btn_both_press_ctr > 2)?(gblinfo.btn_both_press_ctr -= 2):(gblinfo.btn_both_press_ctr = 0);
        
        gblinfo.btn_rt_press_ctr++;
    }
    else {
        (gblinfo.btn_lt_press_ctr > 2)?(gblinfo.btn_lt_press_ctr -= 2):(gblinfo.btn_lt_press_ctr = 0);
        (gblinfo.btn_rt_press_ctr > 2)?(gblinfo.btn_rt_press_ctr -= 2):(gblinfo.btn_rt_press_ctr = 0);
        (gblinfo.btn_both_press_ctr > 2)?(gblinfo.btn_both_press_ctr -= 2):(gblinfo.btn_both_press_ctr = 0);

    }

    if(gblinfo.btn_lt_press_ctr >= BUTTON_DEBOUNCE_TICKS) {
        
        gblinfo.btn_lt_press_ctr = 0;
        
        gblinfo.btn_lt_pressed       = true;
        
        gblinfo.btn_rt_pressed       = false;
        gblinfo.btn_both_pressed    = false;
    }
    
    else if(gblinfo.btn_rt_press_ctr >= BUTTON_DEBOUNCE_TICKS) {
        
        gblinfo.btn_rt_press_ctr = 0;
        
        gblinfo.btn_rt_pressed       = true;
        
        gblinfo.btn_lt_pressed       = false;
        gblinfo.btn_both_pressed    = false;
    }
    
    else if(gblinfo.btn_both_press_ctr >= BUTTON_DEBOUNCE_TICKS) {
        
        gblinfo.btn_both_press_ctr = 0;
        
        gblinfo.btn_both_pressed    = true;
        
        gblinfo.btn_lt_pressed       = false;
        gblinfo.btn_rt_pressed       = false;
    }

}

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

    /* SELECT CURRENT OPERATION STATE */
    gblinfo.current_state               = STATE_IDLE_DISP;
    gblinfo.disp_seconds_ctr            = 0;
    gblinfo.msg_to_send                 = 0;
    gblinfo.disp_tmr_active             = false;
    gblinfo.splash_screen_tmr_active    = false;

    /* BUTTON INITIALIZATION */
    gblinfo.btn_rt_press_ctr            = 0;
    gblinfo.btn_lt_press_ctr            = 0;
    gblinfo.btn_rt_press_ctr            = 0;
    gblinfo.btn_both_press_ctr          = 0;
    gblinfo.btn_lt_pressed              = false;
    gblinfo.btn_rt_pressed              = false;
    gblinfo.btn_both_pressed            = false;
    
    /* INITIAL CONDITION OF HEALTH LED */
    health_led = LED_OFF;

    /* TURN OFF THE DISPLAY */
    disp_enable = DISPLAY_OFF;

    /* SETUP INTERRUPTS */
    Init_Interrupts();   

    /* INITIALIZE SPI INTERFACE FOR DISPLAY */
    SPI1Init();

    /* INITIALIZE SPI INTERFACE FOR RADIO MODULE */
    SPI2Init();

    AnalogRefSel(REF2D048, EXTREF);             // Use internal 2.048V reference and External VREF pin for negative reference -- page 216/380
    InitA2D(RIGHT_JUSTIFIED, 4);                // Set up AD (Justification, Acq Time TADs) ==> (Right, 4 TAD) -- page 361/550

    /* SETUP ANALOG CHANNELS */  
    ANCON0 = 0x00;                      // Analog channels 7-0 are configured for digital inputs. p.363     
    ANCON1 = 0x00;                      // Analog channel 10-8 are configred for digital inputs. p.364
    EnableAnalogCh(BAT_VOLTAGE_CH);     // Channel for battery voltage level
    
    gblinfo.tick20ms = 0;               // Initialize 10ms tick counter
    gblinfo.tick100ms = 0;              // Initialize 100ms tick counter 
    gblinfo.tick500ms = 0;              // Initialize 500ms tick counter
    gblinfo.tick1000ms = 0;             // Initialize 1000ms tick counter

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

    RFMInitialize( );        // Takes parameters netwrokID and node ID  //TODO I'm not sure if these values are okay?
    if(!RFMsetFrequency(915.0)) {
        DispSetContrast(60);
        DispRefresh();
        DispWriteString("SET FREQ FAILED");
        tick100msDelay(35);
    }
}
/* END OF FILE */