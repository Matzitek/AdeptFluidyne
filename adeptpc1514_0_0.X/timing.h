/* 
 * File:   timing.h
 * Author: chris
 *
 * Created on 30 July 2015, 10:13 AM
 */

#ifndef TIMING_H
#define	TIMING_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Timer PR values at Fcy = 4 MHz */ 
#define PR1_VALUE       4000    // TMR1 PR value for 1 ms time-out
#define PR2_VAL_3125    10000   // 160 ms pulse width (F_excite = 3.125 Hz)    
#define PR2_VAL_6250    5000    // 80 ms pulse width (F_excite = 6.25 Hz)   
#define PR2_VAL_12500   2500    // 40 ms pulse width (F_excite = 12.5 Hz)
#define PR2_VAL_25000   1250    // 20 ms pulse width (F_excite = 25 Hz)
 
    
/* Timer PR values at Fcy = 8 MHz */ 
/*#define PR1_VALUE       8000    // TMR1 PR value for 1 ms time-out
#define PR2_VAL_3125    20000   // 160 ms pulse width (F_excite = 3.125 Hz)    
#define PR2_VAL_6250    10000    // 80 ms pulse width (F_excite = 6.25 Hz)   
#define PR2_VAL_12500   5000    // 40 ms pulse width (F_excite = 12.5 Hz)
#define PR2_VAL_25000   2500    // 20 ms pulse width (F_excite = 25 Hz)    
*/
    
    
/* variables */  
extern int year;
extern int mnth_day;
extern int wday_hrs;
extern int min_sec;
extern int date_time[4];
extern char yr1, yr10, day1, day10, month1, month10, hours1, hours10,
        seconds1, seconds10, mins1, mins10;
  
/* FUNCTION PROTOTYPES ++++++++++++++++++++++++++++++++++++++++++++++++++++++ */    
void TIMERS_Config(int t2_pr);    
void Delay1ms(int delay);
void Delay10us(void);
void RTCC_WriteEnable(void);
void RTCC_Set(void);
void RTCC_Read(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMING_H */

