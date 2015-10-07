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

char top_menu_item;


/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* TopMenuItemRun +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Run menu item on the top level menu.         */

void TopMenuItemRun(void){
    
}

/* TopMenuItemProgram +++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Program menu item on the top level menu.         */

void TopMenuItemProgram(void){
    
    char menu_item, EXIT_FLAG;
    
    menu_item = 1;
    EXIT_FLAG = 0;
    
    /* Clear all rows and update icon row */
    ClearDisplay(0);
    SetUpIconRow();
    EPD_Init();
    EPD_Update(ROW1_START, ROW1_END);
    EPD_PowerOff();
    
    while(!EXIT_FLAG){
        
        switch(menu_item){
            case 1:
                /* Display "Configuration" in 3rd row */
                EPD_Init();
                SetUpDisplayString(ARROW_NO, prog_config_str, 14);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
              
            case 2:    
                /* Display "Calibration" in 3rd row */
                EPD_Init();
                SetUpDisplayString(ARROW_NO, prog_calibrate_str, 10);
                EPD_Update(ROW3_START, ROW3_END);
                EPD_PowerOff();
                break;
                
            case 3:    
                /* Display "Exit" in 3rd row */
                EPD_Init();
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
            
            if(menu_item == 3){
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