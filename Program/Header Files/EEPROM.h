//INCLUDE HEADER FILES
#ifndef _EEPROM_H
#define _EEPROM_H

#include "i2c.h" 

#define EEPROM_ID 0xA0	


unsigned char EEPROM_Read(unsigned char addr);
void EEPROM_Write(unsigned char Address, unsigned char Value);

#endif
