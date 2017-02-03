#ifndef ADC_H
#define ADC_H

//INCLUDE HEADER FILE
#include "includes.h"

//USER DEFINED DATA TYPE

//DEFINE CONSTANT
#define	CHANNEL1		0x00
#define	CHANNEL2		0x01

#define AVERAGEVALUE 	8

//FUNCTION PROTOTYPE
void 	adcinit	(void);
int16u 	adcget	(int8u ChNum);

#endif

