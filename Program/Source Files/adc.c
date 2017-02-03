#include "adc.h"

void adcinit(void)
{
	ADMUX = 0x00;							//CHANNEL 0 MUX3...0 = 000
	ADMUX |= _BV(REFS0);					//5V Avcc INTERNAL REFERENCE WITH EXTERNAL CAPACITOR

	#ifdef REF_VOL_256
		ADMUX |= _BV(REFS1);
	#endif
	
	ADMUX &= ~_BV(ADLAR);					//ADC RIGHT ADJUST RESULT
	
	ADCSRA = 0x00;
	ADCSRA |= _BV(ADPS1) | _BV(ADPS2) | _BV(ADEN);	//PRESCALAR BY 128 GIVES 115KHz WITH 7.3728MHz Fcpu

}
int16u adcget(int8u ChNum)
{
	 int16u adcval = 0;
	 int8u i, MSB, LSB;

	adcinit();	
	ADMUX = (ADMUX & 0xe0) | ChNum;
	
	for (i = 0; i < AVERAGEVALUE; i++)	{
		ADCSRA |= _BV(ADSC);					//START CONVERSION
		while (!(ADCSRA & _BV(ADIF))); 		//WAIT FOR CONVERSION TO COMPLETE
		adcval +=  ADCL + (ADCH * 256);
		ADCSRA |= _BV(ADIF);					//CLEAR FLAG BY WRITING 1 TO THE FLAG BIT
	}

	adcval >>= 3;
			
	return adcval;
	
}
