/* #############################################################################
 * EMFM main board firmware for Adept Fluidyne
 *
 * Module: main.c
 * Version 1.0
 *
 * Author:  Chris de Villiers
 *          Matzitek Innovative Technology (Pty) Ltd.
 * July 2015
 *
 * NOTES:
 *
 * 1.
 *
 * 2.
 *
 * #############################################################################
 */

// PIC24F32KA304 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// FBS
#pragma config BWRP = OFF               // Boot Segment Write Protect (Disabled)
#pragma config BSS = OFF                // Boot segment Protect (No boot program flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Write Protect (General segment may be written)
#pragma config GSS0 = OFF               // General Segment Code Protect (No Protection)

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8MHz FRC oscillator With Postscaler (FRCDIV))
#pragma config SOSCSRC = ANA            // SOSC Source Type (Analog Mode for use with crystal)
#pragma config LPRCSEL = HP             // LPRC Oscillator Power and Accuracy (High Power, High Accuracy Mode)
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output disabled)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary Oscillator configured for high-power operation)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32768)
#pragma config FWPSA = PR128            // WDT Prescaler bit (WDT prescaler ratio of 1:128)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bits (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected(windowed WDT disabled))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware, SBOREN bit disabled)
#pragma config LVRCFG = OFF             //  (Low Voltage regulator is not available)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Use Default SCL1/SDA1 Pins For I2C1)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (RA5 input pin disabled,MCLR pin enabled)

// FICD
#pragma config ICS = PGx3               // ICD Pin Placement Select bits (EMUC/EMUD share PGC3/PGD3)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

#include "p24F32KA304.h"
//#include <stdio.h>
//#include <stdlib.h>
#include "main.h"
//#include "math.h"
#include "display.h"
#include "eeprom.h"
#include "timing.h"
#include "flow.h"
#include "menu.h"

/* DECLARATIONS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//char outbuffer[MAXOUTBUF];
//char inbuffer[MAXINBUF];
volatile char UP_BUTTON_PRESSED;
volatile char SHIFT_BUTTON_PRESSED;
volatile char ENTER_BUTTON_PRESSED;
volatile char BUTTON_PRESS_OK;
char UPDATE_TOP_LEVEL_MENU;
char n_samples;
int data, t2_pr_value, i, j; //, k;
char k, f_excite_index;
int pga_gain;
int total_res;
int flow_res;

// ADC conversion results
volatile unsigned int flow_raw;

/* Default values */
char menu_row_pointer; 
//char disp_menu_level;



/* MAIN PROGRAM +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void main(void) {

    /* Power-up initialization. */
    MCU_init();
    
    /* Get timing related parameters from EEPROM
     * then initialize timers. */
       
    data = ReadEEPROM(14);  // ID of F_excite
    
    switch(data){
        case 1:
            t2_pr_value = PR2_VAL_3125; // F_excite = 3.125 Hz
            break;
            
        case 2:
            t2_pr_value = PR2_VAL_6250; // F_excite = 6.25 Hz
            break;
         
        case 3:
            t2_pr_value = PR2_VAL_12500; // F_excite = 12.5 Hz
            break;
        
        case 4:
            t2_pr_value = PR2_VAL_25000; // F_excite = 25 Hz
            n_samples = 12;
            break;
    }
    
    f_excite_index = 4; //for testing - changing excitation frequency
    TIMERS_Config(t2_pr_value);
    
    /* Initialize variable with default values */
    flow_k_1 = 0;           // for first time entry into Kalman filter
    Pk_1 = 1; 
    data = ReadEEPROM(16);  // totalizer display resolution from EEPROM
    total_res = data;
    flow_res = 0;           // no decimal places
    
    /* Power-up defaults */
    pga_gain = 1;   // this must be read from EEPROM
    PGA_SetGain(pga_gain);
    icon_row_list[0] = BATT_FULL;
    icon_row_list[1] = GSM_NO_SIGNAL;
    icon_row_list[2] = LOGGING_OFF;
    menu_row_pointer = RUN_OPT;
    //disp_menu_level = TOP_LEVEL;
    IFS3bits.RTCIF = 0;
    SHIFT_BUTTON_PRESSED = 0;
    UP_BUTTON_PRESSED = 0; 
    ENTER_BUTTON_PRESSED = 0;
    BUTTON_PRESS_OK = 0;
    UPDATE_TOP_LEVEL_MENU = 0;
    
    // set up date and time with zero values
    date_time[0] = 0x0015;  // year
    date_time[1] = 0x0000;  // month/day
    date_time[2] = 0x0000;  // week day/hours
    date_time[3] = 0x0000;  // minutes/seconds
    RTCC_Set();
    //ClearDisplay(0);
    //ClearDisplayRow(0);
    //SetUpIconRow();
    EPD_Init();
    ClearDisplayRow(0);
    SetUpIconRow();
    EPD_Update(ROW1_START, ROW1_END);
    SetUpDisplayString(ARROW_YES, top_menu_run_str, 4);
    EPD_Update(ROW2_START, ROW2_END);
    SetUpDisplayString(ARROW_NO, top_menu_prog_str, 8);
    EPD_Update(ROW3_START, ROW3_END);
    SetUpDisplayString(ARROW_NO, top_menu_clr_str, 6);
    EPD_Update(ROW4_START, ROW4_END);
    SetUpDisplayString(ARROW_NO, top_menu_info_str, 5);
    EPD_Update(ROW5_START, ROW5_END);
    EPD_PowerOff();
    
    // initialize for testing
    flow_rate = 0;
    total = 0;
    velocity = 0;
  

    /* Main program loop */
    for(;;){
        if(IFS3bits.RTCIF){
            IFS3bits.RTCIF = 0;
            RTCC_Read();
            SetUpIconRow();
            EPD_Init();
            EPD_Update(ROW1_START, ROW1_END);
            EPD_PowerOff(); 
            //Delay1ms(1000);
        }
        
        /* Check if key was pressed */
        if(BUTTON_PRESS_OK){
            
            BUTTON_PRESS_OK = 0;
            
            if(ENTER_BUTTON_PRESSED){
                /* Move to the first sub-level */
                ENTER_BUTTON_PRESSED = 0;
                
                switch(menu_row_pointer){
                    
                    case RUN_OPT:
                        //disp_menu_level = SUB_LEVEL1;
                        TopMenuItemRun();
                        menu_row_pointer = RUN_OPT;
                        break;
                        
                    case PROG_OPT:
                        //disp_menu_level = SUB_LEVEL1;
                        TopMenuItemProgram();
                        menu_row_pointer = RUN_OPT;
                        break; 
                        
                    case CLR_OPT:
                        //disp_menu_level = SUB_LEVEL1;
                        TopMenuItemClear();
                        menu_row_pointer = RUN_OPT;
                        break;  
                        
                    case INFO_OPT:
                        //disp_menu_level = SUB_LEVEL1;
                        TopMenuItemInfo();
                        menu_row_pointer = RUN_OPT;
                        break;    
                }
                
                EPD_Init();
                SetUpDisplayString(ARROW_YES, top_menu_run_str, 4);
                EPD_Update(ROW2_START, ROW2_END);
                SetUpDisplayString(ARROW_NO, top_menu_prog_str, 8);
                EPD_Update(ROW3_START, ROW3_END);
                SetUpDisplayString(ARROW_NO, top_menu_clr_str, 6);
                EPD_Update(ROW4_START, ROW4_END);
                SetUpDisplayString(ARROW_NO, top_menu_info_str, 5);
                EPD_Update(ROW5_START, ROW5_END);
                EPD_PowerOff();
            }else{
                    
                if(UP_BUTTON_PRESSED){
                    menu_row_pointer--;

                    if(menu_row_pointer < RUN_OPT){
                        menu_row_pointer = INFO_OPT;
                    }

                    UPDATE_TOP_LEVEL_MENU = 1;
                }

                if(SHIFT_BUTTON_PRESSED){
                    menu_row_pointer++;

                    if(menu_row_pointer > INFO_OPT){
                        menu_row_pointer = RUN_OPT;
                    }

                    UPDATE_TOP_LEVEL_MENU = 1;
                }  

                if(UPDATE_TOP_LEVEL_MENU){
                    UPDATE_TOP_LEVEL_MENU = 0;
                    EPD_Init();
                    
                    switch(menu_row_pointer){
                        case RUN_OPT:
                            SetUpDisplayString(ARROW_YES, top_menu_run_str, 4);
                            EPD_Update(ROW2_START, ROW2_END);
                            SetUpDisplayString(ARROW_NO, top_menu_prog_str, 8);
                            EPD_Update(ROW3_START, ROW3_END);
                            SetUpDisplayString(ARROW_NO, top_menu_clr_str, 6);
                            EPD_Update(ROW4_START, ROW4_END);
                            SetUpDisplayString(ARROW_NO, top_menu_info_str, 5);
                            EPD_Update(ROW5_START, ROW5_END);
                            break;
                            
                        case PROG_OPT:
                            SetUpDisplayString(ARROW_NO, top_menu_run_str, 4);
                            EPD_Update(ROW2_START, ROW2_END);
                            SetUpDisplayString(ARROW_YES, top_menu_prog_str, 8);
                            EPD_Update(ROW3_START, ROW3_END);
                            SetUpDisplayString(ARROW_NO, top_menu_clr_str, 6);
                            EPD_Update(ROW4_START, ROW4_END);
                            SetUpDisplayString(ARROW_NO, top_menu_info_str, 5);
                            EPD_Update(ROW5_START, ROW5_END);
                            break;
                            
                        case CLR_OPT:
                            SetUpDisplayString(ARROW_NO, top_menu_run_str, 4);
                            EPD_Update(ROW2_START, ROW2_END);
                            SetUpDisplayString(ARROW_NO, top_menu_prog_str, 8);
                            EPD_Update(ROW3_START, ROW3_END);
                            SetUpDisplayString(ARROW_YES, top_menu_clr_str, 6);
                            EPD_Update(ROW4_START, ROW4_END);
                            SetUpDisplayString(ARROW_NO, top_menu_info_str, 5);
                            EPD_Update(ROW5_START, ROW5_END);
                            break;  
                            
                        case INFO_OPT:
                            SetUpDisplayString(ARROW_NO, top_menu_run_str, 4);
                            EPD_Update(ROW2_START, ROW2_END);
                            SetUpDisplayString(ARROW_NO, top_menu_prog_str, 8);
                            EPD_Update(ROW3_START, ROW3_END);
                            SetUpDisplayString(ARROW_NO, top_menu_clr_str, 6);
                            EPD_Update(ROW4_START, ROW4_END);
                            SetUpDisplayString(ARROW_YES, top_menu_info_str, 5);
                            EPD_Update(ROW5_START, ROW5_END);
                            break;    
                    }
                   
                    EPD_PowerOff();
                }
            }
        }
    }
}


/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* System initialisation after power-up */
void MCU_init(void){

    /* Configure the clock */
    OSCCONbits.COSC = 7;    // select FRCDIV
    CLKDIVbits.RCDIV = 0;   // divide 8 MHz FRC by 1: Fcy = 4 MHz
    
    
    
    /* Configure I/O pins */
    ANSAbits.ANSA1 = 1;     // RA1 is AN1 input (FLOW)
    ANSAbits.ANSA2 = 0;
    TRISAbits.TRISA2 = 0;   // RA2 is GSM_POW output
    GSM_POW = 0;
    ANSAbits.ANSA3 = 0;
    TRISAbits.TRISA3 = 0;   // RA3 is nENABLE output
    nENABLE = 1;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA6 = 1;   // RA6 is INT2 input (SW2 - SHIFT_BUTTON - is also 'down' button)
    TRISAbits.TRISA7 = 1;   // RA7 is CN9 input (SW1 - UP_BUTTON)
    TRISAbits.TRISA9 = 0;   // RA9 is nDISP_CS output
    nDISP_CS = 0;
    TRISAbits.TRISA10 = 0;  // RA10 is COIL_DRV_1 output
    COIL_DRV_1 = 0;
    TRISAbits.TRISA11 = 0;  // RA11 is COIL_DRV_2 output
    COIL_DRV_2 = 0;

    ANSBbits.ANSB0 = 1;     // RB0 is AN2 input (EP_SENSE)
    ANSBbits.ANSB1 = 1;     // RB1 is AN3 input (BATT_MON)
    ANSBbits.ANSB2 = 0;
    TRISBbits.TRISB2 = 1;   // RB2 is DISP_BUSY input
    ANSBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;   // RB3 is ENABLE output
    ENABLE = 0;
    ANSBbits.ANSB12 = 0;
    ANSBbits.ANSB15 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB12 = 1;  // RB12 is CN14 input (SW3 - ENTER_BUTTON)
    TRISBbits.TRISB9 = 0;   // RB9 is display DISCHARGE output
    DISCHARGE = 0;
    ANSBbits.ANSB14 = 1;    // RB14 = AN10 input (VPROPI)
    //ANSBbits.ANSB15 = 0;
    TRISBbits.TRISB15 = 0;  // RB15 is EP_EXCITE output
    EP_EXCITE = 0;

    ANSCbits.ANSC0 = 0;
    TRISCbits.TRISC0 = 0;   // RC0 is PGA_GAIN_0 output
    LATCbits.LATC0 = 1;     // set gain = 1
    ANSCbits.ANSC1 = 0;
    TRISCbits.TRISC1 = 0;   // RC1 is PGA_GAIN_1 output
    LATCbits.LATC1 = 0;
    ANSCbits.ANSC2 = 0;
    TRISCbits.TRISC2 = 0;   // RC2 is PGA_GAIN_2 output
    LATCbits.LATC2 = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC8 = 0;   // RC8 (OC2) is PULSE_OUT
    PULSE_OUT = 0;
    TRISCbits.TRISC9 = 0;   // RC9 is nDISP_RESET
    nDISP_RESET = 0;
    
    /* Start the RTCC */
    OSCCONbits.SOSCEN = 1;  // enable oscillator for RTCC
    RTCC_WriteEnable();
    RCFGCALbits.RTCEN = 1;
    ALCFGRPTbits.AMASK = 2;    // alarm mask every second
    ALCFGRPTbits.ARPT = 0xFF;   // repeat 10x for testing
    ALCFGRPTbits.CHIME = 1;
    ALCFGRPTbits.ALRMPTR = 0;   // point at seconds
    ALRMVAL = 1;          // interrupt every 3 s
    //IEC3bits.RTCIE = 1;
    ALCFGRPTbits.ALRMEN = 1;
    
    //RCFGCALbits.RTCWREN = 0;

    /* Configure 12-bit ADC */
    /* ADC module OFF, no operation in idle mode (ADSIDL=1)
     * 12-bit operation, unsigned int result
     * Manual conversion trigger (SSRC<3:0> = 0000
     * Manual start of sampling (ASAM=0)
     * +Ref. = Vref (VCFG<1:0> = 01), -Ref = AVss (NVCFG0 = 0)
     * S/H inputs connected to normal inputs (OFFCAL = 0)
     * Result in buffer location determined by channel (BUFREGEN = 1)
     * Inputs not scanned, interrupt after every sample
     * Clock source is TCY, TAD = 2*TCY */
    AD1CON1 = 0x2400;
    AD1CON2 = 0x4800;
    AD1CON3 = 1;
    AD1CON5 = 0;  // not used

    /* Configure SPI1 module (AD7790 interface) */

    /* Configure SPI2 module (display interface).
     * Master mode, 8 bits, prescalers 1:1, clock idle state low.
     * Not enabled here, but in display update function. */
    //SPI2CON1 = 0x003F;  // CKE = 0, CKP = 0. Does not work
    SPI2CON1 = 0x013F;  // CKE = 1, CKP = 0. Works
    //SPI2CON1 = 0x007F;  // CKE = 0, CKP = 1. Works
    //SPI2CON1 = 0x017F;  // CKE = 1, CKP = 1. Does not work
    SPI2CON1bits.SMP = 1;
    SPI2CON2 = 0;


    /* Configure UART */

    /* Configure interrupts */
    CNPU1bits.CN9PUE = 1;   // enable pull-up on CN9
    CNPD1bits.CN9PDE = 0;
    CNEN1bits.CN9IE = 1;    // enable CN9 interrupt (SW1)
    CNPU1bits.CN14PUE  = 1; // enable pull-up on CN14
    CNPD1bits.CN14PDE = 0;
    CNEN1bits.CN14IE = 1;   // enable CN14 interrupt (SW3)
    IFS1bits.CNIF = 0;      // make sure it is cleared
    IEC1bits.CNIE = 1;      // enable input change notification interrupts
    INTCON2bits.INT2EP = 1; // falling edge interrupt on INT2 (SW2)
    IFS1bits.INT2IF = 0;
    IEC1bits.INT2IE = 1;    // enable INT2 interrupt
    
}

/* INTERRUPTS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* Input change notification interrupt handler */
void __attribute__((__interrupt__, __no_auto_psv__)) _CNInterrupt (void){

    if(IFS1bits.CNIF){
        IFS1bits.CNIF = 0; 
        IEC1bits.CNIE = 0;  // temporary interrupt disable

        /* Check which button was pressed
         * UP_BUTTON is Increment 
         * 
         * ENTER_BUTTON is Enter */
        
        if(!UP_BUTTON){ // UP_BUTTON pressed
            
            BUTTON_PRESS_OK = 0;
            
            while((!BUTTON_PRESS_OK) | IFS1bits.CNIF){ // do debounce checking
                
                while(!UP_BUTTON);  // wait for key to be released

                BUTTON_PRESS_OK = 1;    // assume OK

                if(IFS1bits.CNIF){
                    IFS1bits.CNIF = 0;
                    BUTTON_PRESS_OK = 0;    // still not OK
                }
            }
            
            /* What to do when this button is pressed? */
            UP_BUTTON_PRESSED = 1;
            SHIFT_BUTTON_PRESSED = 0;
            ENTER_BUTTON_PRESSED = 0;
        }
        
        if (!ENTER_BUTTON){  // ENTER_BUTTON pressed
            
            BUTTON_PRESS_OK = 0;
            
            while((!BUTTON_PRESS_OK) | IFS1bits.CNIF){ // do debounce checking
                
                while(!ENTER_BUTTON);  // wait for key to be released

                BUTTON_PRESS_OK = 1;    // assume OK

                if(IFS1bits.CNIF){
                    IFS1bits.CNIF = 0;
                    BUTTON_PRESS_OK = 0;    // still not OK
                }
            }
            
            /* What to do when this button is pressed? */            
            ENTER_BUTTON_PRESSED = 1;
            UP_BUTTON_PRESSED = 0;
            SHIFT_BUTTON_PRESSED = 0;
        }

    }
    
    IFS1bits.CNIF = 0;  // to make sure
    IEC1bits.CNIE = 1;  // enable interrupt again
}


/* INT2 interrupt handler.
 * SHIFT_BUTTON has dual role: Shift and Down. */
void __attribute__((__interrupt__, __no_auto_psv__)) _INT2Interrupt (void){
    
    if(IFS1bits.INT2IF ){
        IFS1bits.INT2IF = 0; 
        IEC1bits.INT2IE = 0;  // temporary interrupt disable
        
        if (!SHIFT_BUTTON){  // SHIFT_BUTTON pressed
            
            BUTTON_PRESS_OK = 0;
            
            while((!BUTTON_PRESS_OK) | IFS1bits.INT2IF){ // do debounce checking
                
                while(!SHIFT_BUTTON);  // wait for key to be released

                BUTTON_PRESS_OK = 1;    // assume OK

                if(IFS1bits.INT2IF){
                    IFS1bits.INT2IF = 0;
                    BUTTON_PRESS_OK = 0;    // still not OK
                }
            }
            
            /* What to do when this button is pressed? */
            SHIFT_BUTTON_PRESSED = 1;
            ENTER_BUTTON_PRESSED = 0;
            UP_BUTTON_PRESSED = 0;

        }
    } 
    
    IFS1bits.INT2IF = 0; 
    IEC1bits.INT2IE = 1;  // enable interrupt again
}

/* Address error trap interrupt handler */
//void _ISR _AddressError (void){
void __attribute__((__interrupt__, __no_auto_psv__)) _AddressError(void){
    
    if(INTCON1bits.ADDRERR){
        INTCON1bits.ADDRERR = 0;
    }
}


/*void _ISR _RTCCInterrupt (void){
    
    if(IFS3bits.RTCIF){
        IFS3bits.RTCIF = 0;
    }
    
}*/
