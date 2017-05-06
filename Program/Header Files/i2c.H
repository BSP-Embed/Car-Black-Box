#ifndef I2C_H
#define I2C_H

#include "includes.h"

#define I2C_DDR			DDRB
#define I2C_PORT		PORTB
#define I2C_PPIN		PINB	
#define SDA 			PB2
#define SCL 			PB3

void I2C_delay(void);
void I2C_clock(void);
void I2C_start(void);
void I2C_stop(void);
unsigned char I2C_write(unsigned char dat);
unsigned char I2C_read(void);
void I2C_ack(void);
void I2C_noack(void);

#endif
