#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#define INTDDR				DDRD
#define INTPORT				PORTD
#define INT0_PIN			PD2
#define INT1_PIN			PD3

#define SENSOR_DDR			DDRD
#define SENSOR_PORT			PORTD
#define SENSOR_PIN			PIND
#define DOOR_PIN			PD4

#define SWITCH_DDR			DDRD
#define SWITCH_PORT			PORTD
#define SWITCH_IOPIN		PIND
#define SWITCH_PIN			PD2

//DEFINE CONSTANT
#define	INFO_EE_ADDR		0x01

#define THRESHOLD_DIST		0.5f			/* in meters */

#define FRONT				1
#define BACK				2

#define MSG1 				"Accident Occurred For the Vehicle KA12K333 Near NH212."

//DEFINE MACROS
#define PWMDC(x)			(OCR2 = (x) * 2.55)

//FUNCTION PROTOTYPES
static void		init		(void);
static void 	disptitl 	(void);
static void 	tmr1init	(void);
static void		EXT0init	(void);	
static void		inittest	(void);
static void		sensorinit	(void);
static void		movevehi	(void);
static void		stopvehi	(void);
static void 	chksens		(void);
static int16u 	measco2		(int8u disp);
static void 	logparam	(void);
static void 	dispparam	(void);
static void 	displog		(void);
static void 	incspeed	(void);
static void		DecSpeed	(void);
static void		CheckDist	(void);
static void		CheckColli	(void);
#endif
