#include"EEPROM.h"

unsigned char EEPROM_Read(unsigned char addr)
{
	unsigned char ret;	

	I2C_start();            /* Start i2c bus */

	I2C_write(EEPROM_ID);   /* Connect to DS1307 */
	I2C_write(addr);		/* Request RAM address on DS1307 */
	
	I2C_start();			/* Start i2c bus */

	I2C_write(EEPROM_ID+1);	/* Connect to DS1307 for Read */
	ret = I2C_read();		/* Receive data */
	
	I2C_stop();				/* Stop i2c bus */

	 return ret;			
}
void EEPROM_Write(unsigned char Address, unsigned char Value)
{
	I2C_start(); 

	I2C_write(EEPROM_ID);	/* connect to DS1307 */ 
	I2C_write(Address);		/* Request RAM address */ 

	I2C_write(Value);			/* Write sec on RAM address  */
	I2C_stop();           		/* Stop i2c bus */

}	

