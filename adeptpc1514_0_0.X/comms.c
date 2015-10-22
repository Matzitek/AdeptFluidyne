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
#include "main.h"
#include "comms.h"

char outbuffer[MAXOUTBUF];
volatile char inbuffer[MAXINBUF];
volatile char rx_index;
volatile char inbuff_count;
char outbuff_count;

void UART_TransmitData(void)
{
	unsigned char index;

	index = 0;

	while(outbuffer[index] != 0){
	  while(!U1STAbits.TRMT);	// wait while buffer or shift register not empty
	  while(U1STAbits.UTXBF);   // wait while tx buffer full
	  U1TXREG = outbuffer[index];   // write a byte to the buffer
	  index++;
	}
}


/*================================================================	*/
void ClearInBuffer(void){
    
  int i;

  for(i=0; i < MAXINBUF; i++){
      inbuffer[i] = 0;
  }

    inbuff_count = 0;
    rx_index = 0;
}


/*================================================================	*/
void AppendCRLF(void){

    char temp;
    
    temp = strlen(outbuffer);
    outbuffer[temp] = CR;
    outbuffer[temp+1] = LF;
    outbuffer[temp+2] = 0;
}
