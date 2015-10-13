/* #############################################################################
 * EMFM main board firmware for Adept Fluidyne
 *
 * Module: eeprom.c
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
#include "eeprom.h"

/* The EEPROM is initialized to default values by the compiler.
 * Word 1 (offset = 0): 100 (DN100)
 * Word 2 (offset = 2): 1 (flow unit = m^3/h)
 * Word 3 (offset = 4): 1 (totaliser unit = m^3)
 * Word 4 (offset = 6): 1 (totaliser resolution = 1 decimal place)
 * Word 5 (offset = 8): 1 (totaliser unit = m^3)
 * Word 6 (offset = 10): 1 (low flow cut-off = 0.04 m/s)?
 * Word 7 (offset = 12): 1 (unidirectional flow)
 * Word 8 (offset = 14): 3 (F_excite = 12.5 Hz)
 * Word 9 (offset = 16): 0 (Totalizer resolution = no decimal point) */

int __attribute__ ((space(eedata))) eeData[] = {100, 1, 1, 1, 1, 1, 1, 4, 0};

/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
int ReadEEPROM(unsigned int ee_addr_offset){
    unsigned int data, offset;
    
    TBLPAG = __builtin_tblpage(&eeData);
    offset = __builtin_tbloffset(&eeData);
    data = __builtin_tblrdl(offset + ee_addr_offset);
    return (data);
}

void WriteEEPROM(unsigned int ee_addr_offset, unsigned int ee_data){
    unsigned int offset;
    
    NVMCON = 0x4004;    // Set up NVMCON to write one word of data EEPROM
    TBLPAG = __builtin_tblpage(&eeData);
    offset = __builtin_tbloffset(&eeData);
    __builtin_tblwtl(offset + ee_addr_offset, ee_data);
    asm volatile ("disi #5");   // disable interrupts for 5 instructions
    __builtin_write_NVM();
    while(NVMCONbits.WR);
}
