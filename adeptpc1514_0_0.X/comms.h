/* 
 * File:   comms.h
 * Author: chris
 *
 * Created on 19 October 2015, 9:26 AM
 */

#ifndef COMMS_H
#define	COMMS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define MAXOUTBUF       20
#define MAXINBUF        20
#define CR              0x0D
#define LF              0x0A
#define COMMA           0x2C
#define QUOTE           0x22
#define COLON           0x3A
#define ZERO            0x30    

extern char outbuffer[];
extern volatile char inbuffer[];
extern volatile char rx_index;
extern volatile char inbuff_count;
extern char outbuff_count;

void UART_TransmitData(void);
void ClearInBuffer(void);
void AppendCRLF(void);


#ifdef	__cplusplus
}
#endif

#endif	/* COMMS_H */

