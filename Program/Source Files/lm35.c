#include "lm35.h"

extern int8u lcdptr;

double meastemp(int8u disp){
	double temp;
	char tempstr[10];
	temp = (TEMP_CONST * adcget(0));
	if (disp) {
		ftoa(temp, tempstr, 1);
		lcdptr = disp;
		lcdwc(lcdptr);
		lcdws("    ");
		lcdptr = disp;
		lcdwc(lcdptr);
		lcdws(tempstr);
	}
	return temp;
}