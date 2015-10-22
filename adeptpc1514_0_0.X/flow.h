/* 
 * File:   flow.h
 * Author: chris
 *
 * Created on 03 August 2015, 10:55 AM
 */

#ifndef FLOW_H
#define	FLOW_H

#ifdef	__cplusplus
extern "C" {
#endif

extern unsigned int adc_result;
extern char PIPE_EMTPY_FLAG;
extern char n_samples;
extern float ph1_mean, ph2_mean;
extern float flow_k, flow_k_1;
extern float K; // Kalman filter gain
extern float Pk_1;
extern float flow_rate;
extern float total;
extern float velocity;
extern int pga_gain;
extern int total_res;   // totalizer display resolution
extern int flow_res;   // flow rate and velocity display resolution
extern struct flag_bits flags;

/* ADC input channels */   
#define FLOW                    0x0001   // AN1
#define EP_SENSE                0x0002   // AN2
#define BATT_MON                0x0003   // AN3    
#define VPROPI                  0x000A   // AN10  
#define FLOW_ADC_RESULT         ADC1BUF1
#define EP_SENSE_ADC_RESULT     ADC1BUF2
#define EMPTY_PIPE_THRESHOLD    250    // find correct value

/* Flow velocity and rate display resolutions (decimal places)*/


// covariances for Kalman filter
#define Q   1
#define R   1

    
/* FUNCTION PROTOTYPES ++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void ADC_Read(int ch_number);    
void EmptyPipeCheck(void);    
void MeasureFlow(void);
void PGA_SetGain(int gain);
float KalmanFilter(float zk, float xk_1);

#ifdef	__cplusplus
}
#endif

#endif	/* FLOW_H */

