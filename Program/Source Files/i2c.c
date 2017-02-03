#include"i2c.h"

void I2C_delay(void){
	_delay_us(100);
}
void I2C_clock(void) {
	I2C_delay();
    I2C_PORT |=  _BV(SCL);		
	I2C_delay();    
	I2C_PORT &= ~ _BV(SCL);		
}
void I2C_start(void) {
	I2C_DDR		|= _BV(SCL) | _BV(SDA);			
	I2C_PORT	|=  _BV(SDA); 		
	I2C_PORT	|=  _BV(SCL); 		
	I2C_delay(); 
	I2C_PORT	&= ~ _BV(SDA); 	
	I2C_delay(); 
	I2C_PORT	&= ~ _BV(SCL); 	
}
void I2C_stop(void){
	I2C_DDR		|= _BV(SDA);		
	I2C_PORT	|=  _BV(SCL); 			
	I2C_delay();
	I2C_PORT	&= ~ _BV(SDA); 		
	I2C_delay();
	I2C_PORT	|=  _BV(SDA); 			
}
int8u I2C_write(int8u x) {
	int8u i, data_bit;		
	I2C_DDR |= _BV(SDA);		

	for (i = 0; i < 8; i++)	{			
		if (x & 0x80)
			I2C_PORT |=  _BV(SDA);
		else
			I2C_PORT &=  ~_BV(SDA);		/* Send data_bit to SDA */
		I2C_clock();      				/* Call for send data to i2c bus */
		x <<= 1;  
	}
	I2C_PORT &=  ~_BV(SDA); 
	I2C_delay();	
	I2C_PORT |=  _BV(SCL);  
	I2C_delay();	
	I2C_DDR &= ~_BV(SDA);				
	if (I2C_PPIN & _BV(SDA))		/* Check acknowledge */
			data_bit = 1;
		else
			data_bit = 0;
	I2C_PORT &= ~ _BV(SCL); 	
	return data_bit;			 	
}
int8u I2C_read(void) {
	int8u rd_bit, i, x = 0;	
	I2C_DDR &= ~_BV(SDA);		
	for (i = 0; i < 8; i++) {
		I2C_delay();
        I2C_PORT |=  _BV(SCL);		
		I2C_delay(); 
		if (I2C_PPIN & _BV(SDA)) 
			rd_bit = 1;
		else
			rd_bit = 0;
		x <<= 1;		
		x = x | rd_bit;		/* Keep bit data in x */
        I2C_PORT &= ~_BV(SCL);		
	}
	return x;
}
void I2C_ack(void){
    I2C_PORT &= ~ _BV(SDA);		
	I2C_delay();    
	I2C_clock();					
	I2C_PORT |=  _BV(SDA);  		
}
void I2C_noack(void){
    I2C_PORT |=  _BV(SDA);	
	I2C_delay();
	I2C_clock();	
    I2C_PORT |=  _BV(SCL);
}
				