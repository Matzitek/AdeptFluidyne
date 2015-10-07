/* #############################################################################
 * EMFM main board firmware for Adept Fluidyne
 *
 * Module: timing.c
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
 *
 * #############################################################################
 */

#include "p24F32KA304.h"
//#include "main.h"
#include "timing.h"
#include "display.h"

int year;
int mnth_day;
int wday_hrs;
int min_sec;
int date_time[4];
char yr1, yr10, day1, day10, month1, month10, hours1, hours10,
        seconds1, seconds10, mins1, mins10;

/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TIMERS_Config(int t2_pr){
    
    /* Configure TIMER1. 
     * TIMER1 is used for 1 ms timer in function Delay1ms(). */
    T1CON = 0; // 1:1 prescaler
    PR1 = PR1_VALUE;     // 1 ms time-out
    T1CONbits.TON = 1;     // Timer 1 ON

    /* Configure Timer2.
     * TIMER2 is used for excitation pulses. */
    T2CON = 0;
    T2CONbits.TCKPS = 2;    // 1:64 prescaler
    PR2 = t2_pr;            // this value must be initialized from EEPROM
    
    /* Configure Timer 3 */
    
}
/* Delay1ms +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* This function provides a time delay in multiples of 1 ms */

void Delay1ms(int delay){

    int delay_count;

    if (IFS0bits.T1IF == 1){
        IFS0bits.T1IF = 0;
    }

    TMR1 = 0;
    delay_count = delay;

    while (delay_count > 0){
        //asm("CLRWDT");

        while (IFS0bits.T1IF == 0);

        IFS0bits.T1IF = 0;
        TMR1 = 0;
        delay_count--;
    }
}


/* Delay10us ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* This function provides a delay of roughly 10 us as
 * required by the display initialization. The number
 of asm("NOP")s must be changed if Fcy is not 1MHz. */
void Delay10us(void){
    //char step;
    asm("NOP");
    //step = 2;

    //while(step > 0){
    //    step--;
    //}
}

void RTCC_WriteEnable(void){
    asm volatile("push w7");
    asm volatile("push w8");
    asm volatile("disi #5");
    asm volatile("mov #0x55, w7");
    asm volatile("mov w7, _NVMKEY");
    asm volatile("mov #0xAA, w8");
    asm volatile("mov w8, _NVMKEY");
    asm volatile("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile("pop w8");
    asm volatile("pop w7");
}



 /* how to use this function
    date_time[0] = 0x0015;  // year
    date_time[1] = 0x0916;  // month/day
    date_time[2] = 0x04011;  // week day/hours
    date_time[3] = 0x0000;  // minutes/seconds
    RTCC_Set(); */
    
void RTCC_Set(void){
    
    char i;
    
    RCFGCALbits.RTCPTR = 3;
    
    for(i = 0; i <= 3; i++){
        RTCVAL = date_time[i];
    }
}

void RTCC_Read(void){
    
    RCFGCALbits.RTCPTR = 3;
    year = RTCVAL;
    mnth_day = RTCVAL;
    wday_hrs = RTCVAL;
    min_sec = RTCVAL;
    yr1 = year & 0x000F;
    yr10 = year >> 4;
    day1 = mnth_day & 0x000F;
    day10 = (mnth_day >> 4) & 0x000F;
    month1 = (mnth_day >> 8) & 0x000F;
    month10 = (mnth_day >> 12) & 0x000F;
    hours1 = wday_hrs & 0x000F;
    hours10 = (wday_hrs >> 4) & 0x000F;
    seconds1 = min_sec & 0x000F;
    seconds10 = (min_sec >> 4) & 0x000F;
    mins1 = (min_sec >> 8) & 0x000F; 
    mins10 = (min_sec >> 12) & 0x000F;
    date_time_list[0] = day10;
    date_time_list[1] = day1;
    date_time_list[2] = month10;
    date_time_list[3] = month1;
    date_time_list[4] = yr10;
    date_time_list[5] = yr1;
    date_time_list[6] = hours10;
    date_time_list[7] = hours1;
    date_time_list[8] = mins10;
    date_time_list[9] = mins1;
}
