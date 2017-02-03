#ifndef ALCOHOL_H
#define ALCOHOL_H

#include "includes.h"

#define alcodet()			((ALCO_SENS_PPIN & _BV(ALCO_SENS_PIN)) == 0)


void	AlcoSensInit(void);

#endif