/* 
 * File:   menu.h
 * Author: chris
 *
 * Created on 20 August 2015, 9:48 AM
 */

#ifndef MENU_H
#define	MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Menu level hierarchy and passwords*/    
#define TOP_MENU_RUN        (1 << 0)  // 0001 
    
#define TOP_MENU_PROGRAM    (1 << 1)  // 0010 
#define PROG_CONFIG         (1 << 0)  // 0001
#define PROG_CONFIG_READ 
#define PROG_CONFIG_EDIT
#define PROG_CONFIG_EXIT
    
#define PROG_CALIBRATE          (1 << 1)  // 0010 
#define PROG_CAL_PW1            "0000"   
#define PROG_CAL_PW2    
#define PROG_CAL_PW2
    
#define PROG_CAL_EP_THRESHOLD
#define PROG_CAL_EX_FREQUENCY
#define PROG_CAL_CONVERTER_K    
#define PROG_CAL_TUBE_K1
#define PROG_CAL_TUBE_K2
#define PROG_CAL_TUBE_K2_V 
    
    
#define PROG_EXIT           (1 << 2)  // 0100     
    
#define TOP_MENU_CLEAR      (1 << 2)  // 0100 
#define TOP_MENU_INFO       (1 << 3)  // 1000 
   
    
/* Menu levels */    
#define TOP_LEVEL       0
#define SUB_LEVEL1      1  
#define SUB_LEVEL2      2    
#define SUB_LEVEL3      3   // needed?
    
/* Top menu options */    
#define RUN_OPT         1
#define PROG_OPT        2
#define CLR_OPT         3
#define INFO_OPT        4    
    
//#define LEAVE_OUT_CODE   // use this to leave out large sections of code        
    
/* Flag declarations */    
extern volatile char UP_BUTTON_PRESSED;
extern volatile char SHIFT_BUTTON_PRESSED;
extern volatile char ENTER_BUTTON_PRESSED;
extern volatile char BUTTON_PRESS_OK;
    
/* Variable declarations */
extern char top_menu_item;
extern char prog_menu_item;

/* Display character patterns */
extern const unsigned char char_zero[14][3];
extern const unsigned char char_one[14][3];
extern const unsigned char char_two[14][3];
extern const unsigned char char_three[14][3];
extern const unsigned char char_four[14][3];
extern const unsigned char char_five[14][3];
extern const unsigned char char_six[14][3];
extern const unsigned char char_seven[14][3];
extern const unsigned char char_eight[14][3];
extern const unsigned char char_nine[14][3];
extern const unsigned char char_space[14][3];
extern const unsigned char char_plus[14][3];
extern const unsigned char char_minus[14][3];

/* Function prototypes */
void TopMenuItemRun(void);
void TopMenuItemProgram(void);
void TopMenuItemClear(void);
void TopMenuItemInfo(void);
void InputPasswordClear(void);
void PasswordEnter(void);
void CalibrateMain(void);
void CalibrateFactor(void);
void CalibrateZero(void);
void CheckUpShiftButtons(char max_items);
void ClearTotal(void);
void ClearHours(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_H */

