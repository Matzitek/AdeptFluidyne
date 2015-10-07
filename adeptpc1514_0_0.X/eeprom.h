/* 
 * File:   eeprom.h
 * Author: chris
 *
 * Created on 30 July 2015, 10:17 AM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif


extern int __attribute__ ((space(eedata))) eeData[];
//extern int data, offset, new_data;


/* FUNCTION PROTOTYPES ++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
int ReadEEPROM(unsigned int ee_addr_offset);
void WriteEEPROM(unsigned int ee_addr_offset, unsigned int ee_data);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

