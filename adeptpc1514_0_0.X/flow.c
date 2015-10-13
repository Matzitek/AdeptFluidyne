/* #############################################################################
 * EMFM main board firmware for Adept Fluidyne
 *
 * Module: flow.c
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
#include "flow.h"
#include "main.h"
#include "timing.h"

unsigned int adc_result;
char PIPE_EMTPY_FLAG;
float ph1_mean, ph2_mean;
float flow_k, flow_k_1;
float K;
float Pk_1;
float flow_rate;
float total;
float velocity;

/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void EmptyPipeCheck(void){
    
    ENABLE = 1;     // switch ADC reference on
    EP_EXCITE = 1;  // excite the electrode
    nENABLE = 0;    // enable the op amp
    Delay1ms(5);   // !!! Enable delay time. Check if this can be done elsewhere to save time!
    ADC_Read(EP_SENSE);
    asm("NOP");
    asm("NOP");
    nENABLE = 1;    // disable the op amp
    EP_EXCITE = 0;
    ENABLE = 0;     // switch ADC reference off
    
    // read the result and set/clear the flag
    //adc_result = ADC1BUF2;
    adc_result = EP_SENSE_ADC_RESULT;
    
    if(adc_result > EMPTY_PIPE_THRESHOLD){
        PIPE_EMTPY_FLAG = 0;
    }
    else{
        PIPE_EMTPY_FLAG = 1;
    } 
} 

/* MeasureFlow() ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* Excitation pulse width range: 
   3.125 Hz - T_E = 320 ms, T_PW = 160 ms
   6.250 Hz - T_E = 160 ms, T_PW = 80 ms
   12.50 Hz - T_E = 80 ms, T_PW = 40 ms
   25.00 Hz - T_E = 40 ms, T_PW = 20 ms */

void MeasureFlow(void){
    
    char loop, i, j, k;
    float delta;
    
    // clear the mean values
    ph1_mean = 0;
    ph2_mean = 0;
    j = 0;  // total sample counters for phases 1 and 2
    k = 0;
    
    
    ENABLE = 1;
    Delay1ms(150);    // to make sure 3V_COIL and 1.5V are stable 
    T2CONbits.TON = 1;
    
    for(loop = 1; loop <= 4; loop++){
        /* Phase 1: drive COIL_DRV_1_OUT high */
        COIL_DRV_1 = 1;
        TMR2 = 0;
        IFS0bits.T2IF = 0;
        Delay1ms(4);    // this must change for other excitation frequencies
        
        // now get the samples
        i = 0;
        
        while(i <= n_samples){
            ADC_Read(FLOW);
            ph1_mean += adc_result;
            j++;
            Delay1ms(1);    // samples are 1 ms apart
        }
        
        while(!IFS0bits.T2IF);  // wait for end of pulse
         
        /* Phase 2: drive COIL_DRV_2_OUT high, COIL_DRV_1_OUT low. */
        COIL_DRV_1 = 0;
        COIL_DRV_2 = 1;
        TMR2 = 0;
        IFS0bits.T2IF = 0;
        Delay1ms(4);    // this must change for other excitation frequencies
        
        // now get the samples
        i = 0;
        
        while(i <= n_samples){
            ADC_Read(FLOW);
            ph2_mean += adc_result;
            k++;
            Delay1ms(1);    // samples are 1 ms apart
        }

        while(!IFS0bits.T2IF);  // wait for end of pulse
        
        COIL_DRV_2 = 0;
    }
    
    COIL_DRV_2 = 0; // just to make sure
    COIL_DRV_1 = 0;
    T2CONbits.TON = 0;
    ENABLE = 0;
    
    /* Compute the mean values for phases 1 and 2 */
    ph1_mean = ph1_mean/j;
    ph2_mean = ph2_mean/k;
    delta = abs(ph1_mean - ph2_mean);
    flow_k = KalmanFilter(delta, flow_k_1);   // new measurement estimate
    flow_k_1 = flow_k;
    
    /* Apply calibration constants to flow_k to get actual flow */
}

/* ADC_Read() +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void ADC_Read(int ch_number){
    
    AD1CHS = ch_number;
    AD1CON1bits.ADON = 1;       // turn ADC ON
    AD1CON1bits.SAMP = 1;       // start sampling
    Delay10us();
    AD1CON1bits.SAMP = 0;       // start converting
    
    while(!AD1CON1bits.DONE);   // wait for conversion to complete
    
    switch(ch_number){
        case FLOW:
            adc_result = FLOW_ADC_RESULT;
            break;
            
        case EP_SENSE:
            adc_result = EP_SENSE_ADC_RESULT;
            break;
            
        // do others here    
    }
     
    AD1CON1bits.ADON = 0;       // turn ADC OFF
}

/* PGA_SetGain() ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void PGA_SetGain(int gain){
    
    int temp;
    
    temp = LATC & 0xFFF8;    // strip current gain bits 
    temp |= gain;   // add new gain bits
    LATC = temp;   // now set the gain
}

/* KalmanFilter() +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
float KalmanFilter(float zk, float xk_1){
    
    float xk, Pk, P1k;
    
    P1k = Pk_1 + Q;
    K = P1k/(P1k + R);
    xk = xk_1 + K*(zk - xk_1);
    Pk = P1k - K*P1k;
    xk_1 = xk;
    Pk_1 = Pk;
    return(xk);
}