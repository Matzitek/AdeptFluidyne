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
#include "flow.h"
#include <string.h>

char top_menu_item;

/* Passwords */
const char *edit_config_pw = "4321";
const char *calibrate_pw_1 = "3845";
const char *calibrate_pw_2 = "8486";    // factor
const char *calibrate_pw_3 = "7878";    // zero cal
const char *clear_pw = "0741";
char input_pw[5];
char menu_item;





/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* TopMenuItemRun +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Run menu item on the top level menu.         */

void TopMenuItemRun(void){
    
    char EXIT_FLAG, sign;
    char dec_places;
    
    /* Clear all rows except icon row */
    EPD_Init();
    ClearDisplayRow(2);
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    ClearDisplayRow(5);
    EPD_PowerOff();
    EXIT_FLAG = 0;
    BUTTON_PRESS_OK = 0;
    sign = PLUS;    // for testing
    
   /* Stay in the Run display until ENTER key pressed */
    while(!EXIT_FLAG){
        
        /* Increment variables for testing */
        flow_rate = flow_rate + 5.5;
        total = total + flow_rate;
        velocity = flow_rate/2;
        
        /* Do the flow rate. First find
         * the appropriate resolution. */
        
        if((flow_rate >= 0) & (flow_rate < 1)){
            dec_places = 4;
        }
        
        if((flow_rate >= 1) & (flow_rate < 10)){
            dec_places = 3;
        }
        
        if((flow_rate >= 10) & (flow_rate < 100)){
            dec_places = 2;
        }
        
        if((flow_rate >= 100) & (flow_rate < 1000)){
            dec_places = 1;
        }
        
        if((flow_rate >= 1000) & (flow_rate < 10000)){
            dec_places = 0;
        }
        
        EPD_Init();
        ClearDisplayRow(3);
        SetupNumericDisplay(PLUS, dec_places, flow_rate);
        EPD_Update(ROW3_START, ROW3_END);
        EPD_Update(ROW3_START, ROW3_END);
        //EPD_PowerOff();
        
        /* Do the total. */  
        //EPD_Init();
        ClearDisplayRow(4);
        SetupNumericDisplay(NO_SIGN, total_res, total);
        //SetupNumericDisplay(NO_SIGN, 1, total);
        EPD_Update(ROW4_START, ROW4_END);
        EPD_Update(ROW4_START, ROW4_END);
        //EPD_PowerOff();
        
        /* Do the velocity. First find
         * the appropriate resolution. */
        
        if((velocity >= 0) & (velocity < 1)){
            dec_places = 4;
        }
        
        if((velocity >= 1) & (velocity < 10)){
            dec_places = 3;
        }
        
        if((velocity >= 10) & (velocity < 100)){
            dec_places = 2;
        }
        
        if((velocity >= 100) & (velocity < 1000)){
            dec_places = 1;
        }
        
        if((velocity >= 1000) & (velocity < 10000)){
            dec_places = 0;
        }
        
        //EPD_Init();
        ClearDisplayRow(5);
        SetupNumericDisplay(NO_SIGN, dec_places, velocity);
        EPD_Update(ROW5_START, ROW5_END);
        EPD_Update(ROW5_START, ROW5_END);
        EPD_PowerOff();
        
        // make this a function to update row 1
        if(IFS3bits.RTCIF){
            IFS3bits.RTCIF = 0;
            RTCC_Read();
            EPD_Init();
            ClearDisplayRow(1);
            SetUpIconRow();
            EPD_Update(ROW1_START, ROW1_END);
            EPD_Update(ROW1_START, ROW1_END);
            EPD_PowerOff(); 
        }
        
        Delay1ms(2000);
        
        if(BUTTON_PRESS_OK){    // only way to get out is with key press!
            BUTTON_PRESS_OK = 0;
            
            if(ENTER_BUTTON_PRESSED){
                ENTER_BUTTON_PRESSED = 0; 
                EXIT_FLAG = 1;
            }else{
                // wrong button pressed
                SHIFT_BUTTON_PRESSED = 0;
                UP_BUTTON_PRESSED = 0;
            }
            
        } 
    }    
}

/* TopMenuItemProgram +++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Program menu item on the top level menu.         */

void TopMenuItemProgram(void){
    
    char EXIT_FLAG, pw_test;
    int pw_test_result;
        
    /* Clear all rows except icon row */
    EPD_Init();
    ClearDisplayRow(2);
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    ClearDisplayRow(5);
    EPD_PowerOff();
    menu_item = 1;
    EXIT_FLAG = 0;
    BUTTON_PRESS_OK = 0;
    
    while(!EXIT_FLAG){
        
        switch(menu_item){
            case 1:
                /* Display "Configuration" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prog_config_str, 14);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
              
            case 2:    
                /* Display "Calibration" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prog_calibrate_str, 10);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
                
            case 3:    
                /* Display "Exit" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prog_exit_str, 5);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;    
        }
        
        /* Wait for key press */
        while(!BUTTON_PRESS_OK);    // only way to get out is with key press!
        
        BUTTON_PRESS_OK = 0;
        CheckUpShiftButtons(3);
        
        if(ENTER_BUTTON_PRESSED){
            ENTER_BUTTON_PRESSED = 0;
            
            switch(menu_item){
                case 1: // configuration
                    break;
                    
                case 2: // calibration
                    InputPasswordClear();
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
                        
                        /* Check if ENTER button pressed in case user
                         * wants to get out without entering password. */
                        if(ENTER_BUTTON_PRESSED){
                            ENTER_BUTTON_PRESSED = 0;
                            EXIT_FLAG = 1;
                            break;
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
                    }    
                    
                case 3: // exit
                    EXIT_FLAG = 1;
            }
            
        }
    }  
    //InputPasswordClear();
}

/* TopMenuItemClear +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Clear menu item on the top level menu.           */

void TopMenuItemClear(void){
    
    int pw_test_result;
    char old_menu_item, EXIT_FLAG;
    
    /* Clear all rows except icon row */
    EPD_Init();
    ClearDisplayRow(2);
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    ClearDisplayRow(5);
    EPD_PowerOff();
    InputPasswordClear();
    PasswordEnter();
    
    /* Check if the CLEAR password was entered */
    
    pw_test_result = strcmp(input_pw, clear_pw);

    if(pw_test_result == 0){
        // password match
        
        menu_item = 1;
        EXIT_FLAG = 0;
        BUTTON_PRESS_OK = 0;

        while(!EXIT_FLAG){

            switch(menu_item){
                case 1:
                    /* Display "Clear Total" in 3rd row */
                    EPD_Init();
                    ClearDisplayRow(3);
                    SetUpDisplayString(ARROW_NO, clr_total_str, 12);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_PowerOff();
                    break;

                case 2:    
                    /* Display "Clear Hours" in 3rd row */
                    EPD_Init();
                    ClearDisplayRow(3);
                    SetUpDisplayString(ARROW_NO, clr_hours_str, 12);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_PowerOff();
                    break;

                case 3:    
                    /* Display "Exit" in 3rd row */
                    EPD_Init();
                    ClearDisplayRow(3);
                    SetUpDisplayString(ARROW_NO, prog_exit_str, 5);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_PowerOff();
                    break;    
            }
            
            /* Wait for key press */
            while(!BUTTON_PRESS_OK);    // only way to get out is with key press!

            BUTTON_PRESS_OK = 0;
            CheckUpShiftButtons(3);

            if(ENTER_BUTTON_PRESSED){
                ENTER_BUTTON_PRESSED = 0;
                
                if(menu_item == 3){
                    // exit
                    EXIT_FLAG = 1;
                    break;  // out of while loop
                }
                
                /* Offer Yes/No options.
                 * NO -> menu_item == 1, YES -> menu_item == 2. */
                old_menu_item = menu_item;  // save for later
                menu_item = 1;
                
                while(!EXIT_FLAG){
                    EPD_Init();
                    ClearDisplayRow(4);

                    switch(menu_item){
                        case 1:
                            SetUpDisplayString(ARROW_NO, no_str, 3);
                            break;

                        case 2:
                            SetUpDisplayString(ARROW_NO, yes_str, 4);
                            break;
                    }

                    EPD_Update(ROW4_START, ROW4_END);
                    EPD_Update(ROW4_START, ROW4_END);
                    EPD_PowerOff();
                    
                    /* Wait for key press */
                    while(!BUTTON_PRESS_OK);    // only way to get out is with key press!

                    BUTTON_PRESS_OK = 0;
                    CheckUpShiftButtons(2);
                    
                    if(ENTER_BUTTON_PRESSED){
                        ENTER_BUTTON_PRESSED = 0;
                        EXIT_FLAG = 1;

                        switch(old_menu_item){
                            case 1:
                                if(menu_item == 2){ // YES selected
                                    ClearTotal();
                                }
                                break;

                            case 2:
                                if(menu_item == 2){ // YES selected
                                    ClearHours();
                                }
                                break;
                        }
                    }    
                } 
                
                EXIT_FLAG = 0;  // to return to previous while loop
            }
            
            EPD_Init();
            ClearDisplayRow(4);
            EPD_PowerOff();
        }
    }else{
        InputPasswordClear();        
    }     
}

/* TopMenuItemInfo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Info menu item on the top level menu.            */

void TopMenuItemInfo(void){
    
    char EXIT_FLAG;
        
    /* Clear all rows except icon row */
    EPD_Init();
    ClearDisplayRow(2);
    ClearDisplayRow(3);
    ClearDisplayRow(4);
    ClearDisplayRow(5);
    EPD_PowerOff();
    menu_item = 1;
    EXIT_FLAG = 0;
    BUTTON_PRESS_OK = 0;
    
    while(!EXIT_FLAG){
        
        switch(menu_item){
            case 1:
                /* Display "Product Name" in 3rd row
                 * and the name in 4th row (to be defined). */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, prod_name_str, 13);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
              
            case 2:    
                /* Display "Firmware Version" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, fw_version_str, 12);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
                
            case 3:    
                /* Display "Run h:min" in 3rd row and
                 * numbers in 4th row. */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, run_hr_min_str, 10);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;    
                
            case 4:    
                /* Display "On h:min" in 3rd row and
                 * numbers in 4th row. */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, on_hr_min_str, 9);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;   
                
            case 5:    
                /* Display "Interruptions" in 3rd row */
                EPD_Init();
                ClearDisplayRow(3);
                SetUpDisplayString(ARROW_NO, intrupts_str, 14);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;     
                
            case 6:    
                    /* Display "Exit" in 3rd row */
                    EPD_Init();
                    ClearDisplayRow(3);
                    SetUpDisplayString(ARROW_NO, prog_exit_str, 5);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_Update(ROW3_START, ROW3_END);
                    EPD_PowerOff();
                    break;    
        }
        
        /* Wait for key press */
        while(!BUTTON_PRESS_OK);    // only way to get out is with key press!
        
        BUTTON_PRESS_OK = 0;
        CheckUpShiftButtons(6);
        
        if(ENTER_BUTTON_PRESSED){
            ENTER_BUTTON_PRESSED = 0;
            EXIT_FLAG = 1;
        }
    }    
    
}


/* InputPasswordClear ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function clears the previous password entry so that it does not show the
 * next time.                                                                      */
void InputPasswordClear(void){
    
    char index;
    
    EPD_Init();
    ClearDisplayRow(4);
    EPD_PowerOff();
        
    for(index = 0; index < 4; index++){
        input_pw[index] = 0x30;
        pw_display_str[index + 1] = char_zero;
    }
    
    input_pw[index] = 0;
    pw_display_str[0] = char_space;
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
    EPD_Update(ROW3_START, ROW3_END);
    SetUpDisplayString(ARROW_NO, pw_display_str, 5);
    EPD_Update(ROW4_START, ROW4_END);
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
                case 0: pw_display_str[digit_posn + 1] = char_zero;
                break;
                
                case 1: pw_display_str[digit_posn + 1] = char_one;
                break;
                
                case 2: pw_display_str[digit_posn + 1] = char_two;
                break;
                
                case 3: pw_display_str[digit_posn + 1] = char_three;
                break;
                
                case 4: pw_display_str[digit_posn + 1] = char_four;
                break;
                
                case 5: pw_display_str[digit_posn + 1] = char_five;
                break;
                
                case 6: pw_display_str[digit_posn + 1] = char_six;
                break;
                
                case 7: pw_display_str[digit_posn + 1] = char_seven;
                break;
                
                case 8: pw_display_str[digit_posn + 1] = char_eight;
                break;
                
                case 9: pw_display_str[digit_posn + 1] = char_nine;
                break;
            }
            
            EPD_Init();
            //ClearDisplayRow(4);
            SetUpDisplayString(ARROW_NO, pw_display_str, 5);
            EPD_Update(ROW4_START, ROW4_END);
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
        
        if(ENTER_BUTTON_PRESSED){ 
            ENTER_BUTTON_PRESSED = 0;
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

void CheckUpShiftButtons(char max_items){
    
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

        if(menu_item > max_items){
            menu_item = 1;
        }            
    }
}

void ClearTotal(void){
    
}

void ClearHours(void){
    
}