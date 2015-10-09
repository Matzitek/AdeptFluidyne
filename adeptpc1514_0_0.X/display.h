/* 
 * File:   display.h
 * Author: chris
 *
 * Created on 30 July 2015, 9:48 AM
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Display dot colour */
#define BLACK0          0x03
#define BLACK1          0x0C
#define BLACK2          0x30
#define BLACK3          0xC0
#define BLACK           0xFF
#define WHITE0          0x02
#define WHITE1          0x08
#define WHITE2          0x20
#define WHITE3          0x80
#define WHITE           0xAA
#define NOTHING0        0x01
#define NOTHING1        0x04
#define NOTHING2        0x10
#define NOTHING3        0x40
#define NOTHING         0x55
    
#define ROW1_START      5
#define ROW1_END        18
#define ROW2_START      25
#define ROW2_END        38
#define ROW3_START      43
#define ROW3_END        56
#define ROW4_START      61
#define ROW4_END        74
#define ROW5_START      79
#define ROW5_END        92  
    
/* Positions of the various icons in icon_row_list */    
#define BATT_FULL       0
#define BATT_HALF       1
#define BATT_LOW        2
#define GSM_NO_SIGNAL   3
#define GSM_SIGNAL1     4
#define GSM_SIGNAL2     5
#define GSM_SIGNAL3     6
#define LOGGING_OFF     7    
#define LOGGING_ON      8
    
#define ARROW_YES       1
#define ARROW_NO        0
    
    
/* Icon bits */
/*extern const unsigned char odd_dots[2][25]; */
    
extern const unsigned char char_zero[14][3];

extern const unsigned char char_one[14][3];

extern const unsigned char char_two[14][3];

extern unsigned char stage, scan, temp; //, i;
extern unsigned char scan_data[24];
extern unsigned char display_line_old[75];
extern unsigned char display_line_new[75];
extern unsigned char icon_row_list[16];
extern unsigned char date_time_list[10];
extern char menu_row_pointer;
extern char disp_menu_level;
extern const unsigned char epd_data[8];
extern unsigned char display_row[14][50];
extern const unsigned char *icons[9];
extern const unsigned char *numerals[10];
extern const unsigned char *top_menu_run_str[4];
extern const unsigned char *top_menu_prog_str[8];
extern const unsigned char *top_menu_clr_str[6];
extern const unsigned char *top_menu_info_str[5];
extern const unsigned char *prog_config_str[14];
extern const unsigned char *prog_calibrate_str[10];
extern const unsigned char *prog_exit_str[5];
extern const unsigned char *cal_excite_str[16];
extern const unsigned char *cal_convertk_str[12];
extern const unsigned char *cal_tubek1_str[8];
extern const unsigned char *cal_tubek2_str[8];
extern const unsigned char *cal_tubek2_vel_str[17];
extern const unsigned char *cal_factor_str[7];
extern const unsigned char *cal_zero_str[9];
extern const unsigned char *clr_total_str[12];
extern const unsigned char *clr_hours_str[12];

extern const unsigned char *password_str[9];
extern unsigned char *pw_str[5];

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


/* Function prototypes */
void EPD_Update(char start_line, char end_line);
void EPD_UpdateNoStages(void);
void EPD_Init(void);
unsigned char EPD_SPI_Read(unsigned char data);
void EPD_SPI_Write(unsigned char data);
void EPD_SPI_SendByte(unsigned char cog_register, unsigned char data);
void EPD_SPI_Send(unsigned char cog_register, unsigned char *p_disp_data,
    unsigned char length);
unsigned char SPI_R(unsigned char cog_register, unsigned char data);
unsigned char SPI_RID(void);
void InitScanData(char position);
void InitDisplayLines(char odd, char even);
void SendScanData();
void EPD_PowerOff(void);
void CopyNewImage(void);
void ShiftScanData(void);
void SetDisplayLine(unsigned char line);
//void SetUpDisplayRow(char row_number, char menu_level); //, unsigned char *digit_ptr);
void SetUpDisplayString(char ARROW, unsigned char *display_string[], char string_length);
void SetUpIconRow(void);
void ClearDisplayRow(char row);


#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_H */

