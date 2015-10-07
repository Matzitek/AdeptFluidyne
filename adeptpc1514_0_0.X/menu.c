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
    
    /* Clear all rows except icon row */
    ClearDisplay(0);
    //ClearDisplay(3);
    //ClearDisplay(4);
    //ClearDisplay(5);
    SetUpIconRow();
    
    /* Display "Configuration" in 3rd row */
    EPD_Init();
    EPD_Update(ROW1_START, ROW1_END);
    SetUpDisplayRow(3, disp_menu_level);
    EPD_Update(ROW3_START, ROW3_END);
    EPD_PowerOff();
    disp_menu_level = TOP_LEVEL;
    
}

/* TopMenuItemClear +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Clear menu item on the top level menu.           */

void TopMenuItemClear(void){
    
}

/* TopMenuItemInfo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* This function handles the Info menu item on the top level menu.            */

void TopMenuItemInfo(void){
    
}