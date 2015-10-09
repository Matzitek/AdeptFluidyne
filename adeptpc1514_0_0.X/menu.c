/* #############################################################################
 * EMFM main board firmware for Adept Fluidyne
 *
 * Module: menu.c
 * Version 1.0
 *
 * Author:  Chris de Villiers
 *          Matzitek Innovative Technology (Pty) Ltd.
 * July 2015
 *
 * NOTES:
 * 
 *
 * 1.
 *
 * 2.
 *
 * #############################################################################
 */

#include "p24F32KA304.h"
#include "main.h"
#include "menu.h"
#include "display.h"
#include <string.h>

char top_menu_item;

/* Passwords */
const char *edit_config_pw = "4321";
const char *calibrate_pw_1 = "3845";
const char *calibrate_pw_2 = "8486";    // factor
const char *calibrate_pw_3 = "7878";    // zero cal
const char *clear_pw = "0741";
char input_pw[5] = "0000";





/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* TopMenuItemRun +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Run menu item on the top level menu.         */

void TopMenuItemRun(void){
    
}

/* TopMenuItemProgram +++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Program menu item on the top level menu.         */

void TopMenuItemProgram(void){
    
    char menu_item, EXIT_FLAG, pw_test;
    int pw_test_result;
    
    menu_item = 1;
    EXIT_FLAG = 0;
    
    /* Clear all rows and update icon row */
    EPD_Init();
    ClearDisplayRow(2);
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    ClearDisplayRow(5);
    //SetUpIconRow();
    //EPD_Init();
    //EPD_Update(ROW1_START, ROW1_END);
    EPD_PowerOff();
    
    while(!EXIT_FLAG){
        
        switch(menu_item){
            case 1:
                /* Display "Configuration" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prog_config_str, 14);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
              
            case 2:    
                /* Display "Calibration" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prog_calibrate_str, 10);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
                
            case 3:    
                /* Display "Exit" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prog_exit_str, 5);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;    
        }
        
        /* Wait for key press */
        while(!BUTTON_PRESS_OK);    // only way to get out is with key press!
        
        BUTTON_PRESS_OK = 0;
        
        if(UP_BUTTON_PRESSED){
            UP_BUTTON_PRESSED = 0;
            menu_item--;
            
            if(menu_item < 1){
                menu_item = 1;
            }            
        }
        
        if(SHIFT_BUTTON_PRESSED){
            SHIFT_BUTTON_PRESSED = 0;
            menu_item++;
            
            if(menu_item > 3){
                menu_item = 1;
            }            
        }
        
        if(ENTER_BUTTON_PRESSED){
            ENTER_BUTTON_PRESSED = 0;
            
            switch(menu_item){
                case 1: // configuration
                    break;
                    
                case 2: // calibration
                    PasswordEnter();
                    
                    /* Check which of the three passwords was entered */
                    pw_test = 0;
                    
                    while(pw_test == 0){
                        pw_test_result = strcmp(input_pw, calibrate_pw_1);
                        
                        if(pw_test_result == 0){
                            pw_test = 1;    // first password match
                            break;
                        }
                        
                        pw_test_result = strcmp(input_pw, calibrate_pw_2);
                        
                        if(pw_test_result == 0){
                            pw_test = 2;    // 2nd password match
                            break;
                        }
                        
                        pw_test_result = strcmp(input_pw, calibrate_pw_3);
                        
                        if(pw_test_result == 0){
                            pw_test = 3;    // 3rd password match
                            break;
                        }
                        
                    }
                    
                    /* Carry out appropriate calibration function */
                    switch(pw_test){
                        case 1:
                            CalibrateMain();
                            break;
                            
                        case 2:
                            CalibrateFactor();
                            break;
                            
                        case 3:
                            CalibrateZero();
                            break;    
                    }
                    break;
                    
                case 3: // exit
                    EXIT_FLAG = 1;
            }
            
        }
    }
}

/* TopMenuItemClear +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Clear menu item on the top level menu.           */

void TopMenuItemClear(void){
    
}

/* TopMenuItemInfo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Info menu item on the top level menu.            */

void TopMenuItemInfo(void){
    
}


/* PasswordEnter ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles password entry for the various menus. It places the
 * password entered in input_pw.                           */
//char PasswordEnter(char *pw_string){
void PasswordEnter(void){    
    
    char digit_posn, ENTER_FLAG;
    char digit;
    
    digit_posn = 0;
    digit = 0;
    ENTER_FLAG = 0;
    BUTTON_PRESS_OK = 0;
    
    /* Display "Password" on row 3 and "0000" on row 4. */
    EPD_Init();
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    SetUpDisplayString(ARROW_NO, password_str, 9);
    EPD_Update(ROW3_START, ROW3_END);
    SetUpDisplayString(ARROW_NO, pw_str, 5);
    EPD_Update(ROW4_START, ROW4_END);
    EPD_PowerOff();
    
    /* Get the new password */
    while(!ENTER_FLAG){
         /* Wait for key press */
        while(!BUTTON_PRESS_OK);    // only way to get out is with key press!
        
        BUTTON_PRESS_OK = 0;
        
        if(UP_BUTTON_PRESSED){  // increment the digit in the current position
            UP_BUTTON_PRESSED = 0;
            
            digit++;
            
            if(digit > 9){  // roll over to 0
                digit = 0;
            }
            
            input_pw[digit_posn] = digit | 0x30;    // convert to ASCII
            
            /* Update pw_string */
            switch(digit){
                case 0: pw_str[digit_posn + 1] = char_zero;
                break;
                
                case 1: pw_str[digit_posn + 1] = char_one;
                break;
                
                case 2: pw_str[digit_posn + 1] = char_two;
                break;
                
                case 3: pw_str[digit_posn + 1] = char_three;
                break;
                
                case 4: pw_str[digit_posn + 1] = char_four;
                break;
                
                case 5: pw_str[digit_posn + 1] = char_five;
                break;
                
                case 6: pw_str[digit_posn + 1] = char_six;
                break;
                
                case 7: pw_str[digit_posn + 1] = char_seven;
                break;
                
                case 8: pw_str[digit_posn + 1] = char_eight;
                break;
                
                case 9: pw_str[digit_posn + 1] = char_nine;
                break;
            }
            
            EPD_Init();
            //ClearDisplayRow(4);
            SetUpDisplayString(ARROW_NO, pw_str, 5);
            EPD_Update(ROW4_START, ROW4_END);
            EPD_PowerOff();
                        
        }
        
        if(SHIFT_BUTTON_PRESSED){ // move right to next digit position
            SHIFT_BUTTON_PRESSED = 0;
            digit_posn++;
            
            if(digit_posn > 3){
               digit_posn = 0; 
            }
            
            digit = 0;
        }
        
        if(ENTER_BUTTON_PRESSED){   // check if passwords match
            ENTER_BUTTON_PRESSED = 0;
           // pw_result = 0;
            //pw_result = 1;
            ENTER_FLAG = 1;
            EPD_Init();
            ClearDisplayRow(4);
            EPD_PowerOff();
        }
        
    }
}


void CalibrateMain(void){
    
}


void CalibrateFactor(void){
    
}


void CalibrateZero(void){
    
}