/* 
 * File:   main.h
 * Author:  Chris de Villiers
 *          Matzitek Innovative Technology (Pty) Ltd.
 *
 * July 2015
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define GSM_POW         LATAbits.LATA2
#define GSM_POW         LATAbits.LATA2
#define nENABLE         LATAbits.LATA3
#define nDISP_CS        LATAbits.LATA9
#define COIL_DRV_1      LATAbits.LATA10
#define COIL_DRV_2      LATAbits.LATA11

#define DISP_BUSY       LATBbits.LATB2
#define ENABLE          LATBbits.LATB3
#define DISCHARGE       LATBbits.LATB9
#define EP_EXCITE       LATBbits.LATB15

#define PULSE_OUT       LATCbits.LATC8
#define nDISP_RESET     LATCbits.LATC9

#define UP_BUTTON       PORTAbits.RA7   // switch 1
#define SHIFT_BUTTON    PORTAbits.RA6   // switch 2
#define ENTER_BUTTON    PORTBbits.RB12  // switch 3

//#define MAXOUTBUF       20
#define MAXINBUF        20
 //   
#define TRUE            1
#define FALSE           0    
    
struct flag_bits {
    volatile unsigned int UP_BUTTON_PRESSED      : 1;
    volatile unsigned int SHIFT_BUTTON_PRESSED   : 1;
    volatile unsigned int ENTER_BUTTON_PRESSED   : 1;
    volatile unsigned int BUTTON_PRESS_OK        : 1;
    volatile unsigned int UPDATE_MENU            : 1; 
    volatile unsigned int PIPE_EMTPY_FLAG        : 1;
    volatile unsigned int EXIT_FLAG              : 1;
    volatile unsigned int RX_END_FLAG            : 1;
};    
   

/* Menu constant strings */


/* FUNCTION PROTOTYPES ++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void MCU_init(void);
void SetParameters(void);
void GenerateDisplayPattern(void);
void SendCommandToGSM(char command, char data);
void GetUserInput(void);




#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

