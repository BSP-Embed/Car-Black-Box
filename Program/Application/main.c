/*	Main.c: Application program for Car Black Box		*/
/*	Written By	: Prashanth BS (info@bspembed.com)		*/
/*	Demo at		: www.youtube.com/c/bspembed			*/

#include "main.h"

struct {
	volatile int8u chkdist	: 1;
	volatile int8u monit	: 1;
	volatile int8u colli	: 1;
	volatile int8u stop		: 1;
	volatile int8u move		: 1;
} Flag;

volatile int16u ultpulse;
volatile int8u collisens;

extern lcdptr;

const char *stat[] = { 
	"Door Opened",
	"Alcohol Detected",
	"Front collision",
	"Back collision" 
};

int main(void) {
	init();
	while (TRUE) {
		if (Flag.chkdist) 
			CheckDist();
		if (Flag.colli) 
			CheckColli();
		if (Flag.monit) {
			meastemp(0x82);
			measco2(1);
			chksens();
			Flag.monit = FALSE;
		}
		sleep_cpu();
	}
	return 0;
}
static void init(void) {
	buzinit();
	ledinit();
	SENSOR_DDR 		&= ~_BV(DOOR_PIN);
	SENSOR_PORT		|= _BV(DOOR_PIN);
	ALSENS_DDR		&= ~_BV(ALSENS_PIN);
	ALSENS_PORT		|= _BV(ALSENS_PIN);
	SWITCH_DDR		&= ~_BV(SWITCH_PIN);
	SWITCH_PORT		|= _BV(SWITCH_PIN);
	beep(2,100);
	lcdinit();
	if ((SWITCH_IOPIN & _BV(SWITCH_PIN)) == 0)
		displog();
	motorinit();
	uartinit();
	tmr1init();
	adcinit();
	EXT0init();
	PWMinit(); 
	GSMinit();
	ultinit();
	inittest();
	disptitl(); 
	dispparam();
	sei();
	sleep_enable();
	sleep_cpu();
}
static void CheckDist(void) {
	double dist;
	char s[10];
	if (MeasDist(0xc9) < THRESHOLD_DIST) {
		beep(3,75);
		if (!Flag.stop) {
			Flag.stop = TRUE;
			Flag.move = FALSE;
			lcdclr();
			lcdws(" Object Detected");
			lcdr2();
			lcdws("  Slowing Down");
			DecSpeed();
			stopvehi();
			dispparam();
		}
	} else 
		if (!Flag.move) {
			Flag.move = TRUE;
			Flag.stop = FALSE;
			movevehi();
			dispparam();
		} 
}
static void CheckColli(void) {
		buzon();
		Flag.colli = FALSE;
		stopvehi();
		lcdclr();
		lcdws("COLLI OCC: ");
		collisens == FRONT ? lcdws("FRONT") : lcdws("BACK");
		logparam();
		GSMsndmsg(OPhNum, MSG1);
		lcdclr();
		lcdws("Accident Occur'd");
		lcdr2();
		lcdws("Vehicle Stopped!");
		ledon();
		cli();
		sleep_enable();
		sleep_cpu();
		for(;;);
}
static void DecSpeed(void) {
	int8u i;
	for (i = 100; i > 0; i--) {
		PWMDC(i);
		dlyms(20);
	}
}
static void incspeed(void) {
	int8u i;
	for (i = 0; i < 100; i++) {
		PWMDC(i);
		dlyms(10);
	} 
}
static void disptitl(void) { 
	lcdclr();
	lcdws(" VEHI BLACK BOX");
	dlyms(1000);
}
static void tmr1init(void) {
	TCNT1H   = 225;					/* overflows at every 250msec */
	TCNT1L   = 123;
	TIMSK   |= _BV(TOIE1);			/* ENABLE OVERFLOW INTERRUPT */
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS12);			 /* PRESCALAR BY 256 */
}
static void EXT0init(void) {
	INTDDR 	&= ~_BV(INT0_PIN);
	INTPORT |= _BV(INT0_PIN);
	INTDDR 	&= ~_BV(INT1_PIN);
	INTPORT |= _BV(INT1_PIN);
	GICR	|= _BV(INT0) | _BV(INT1);		/* ENABLE EXTERNAL INTERRUPT */
	MCUCR	|= _BV(ISC01) | _BV(ISC11);		/* FALLING EDGE INTERRUPT */
}
ISR(TIMER1_OVF_vect) { 
	static int8u i,j,k;
	TCNT1H = 225;
	TCNT1L = 123;
	if (++i >= 20) i = 0;
	switch(i) {
		case 0: case 2: ledon();  break;
		case 1: case 3: ledoff(); break;
	} 
	if (++j >= 2) {
		j = 0;
		Flag.chkdist = TRUE;
	}
	if (++k >= 8) {
		k = 0;
		Flag.monit = TRUE;
	}
}
ISR(INT0_vect) { 
	collisens = FRONT;
	Flag.colli = TRUE;
	GICR &= ~_BV(INT0);
}
ISR(INT1_vect) { 
	collisens = BACK;
	Flag.colli = TRUE;
	GICR &= ~_BV(INT1);
} 
static void inittest(void){
	lcdclr();
	lcdws("Chk Door:");
	if (SENSOR_PIN & _BV(DOOR_PIN)) {
		lcdws("NOT OK");
		while (SENSOR_PIN & _BV(DOOR_PIN)) {
			beep(1,75);
			dlyms(500);
		}
		} else {
		lcdws("OK");
		dlyms(1000);
	}
	lcdclr();
	lcdws("Chk Alcohol:");
	if ((ALSENS_IPIN & _BV(ALSENS_PIN)) == 0x00) {
		lcdws("N'OK");
		while ((ALSENS_IPIN & _BV(ALSENS_PIN)) == 0x00) {
			beep(1,75);
			dlyms(500);
		}
	} else {
		lcdws("OK");
		dlyms(1000);
	}
}
static void movevehi(void) {
	lcdclr();
	lcdws(" Vehicle Moving");
	mot1on();
	dlyms(500);
	incspeed();
}
static void stopvehi(void) {
	lcdclr();
	lcdws("Vehicle Stopped");
	PWMDC(0);
	mot1off();
	dlyms(500);
}
static void chksens(void) {
	if (SENSOR_PIN & _BV(DOOR_PIN)) 
		beep(1,150);
	if (ALSENS_IPIN & _BV(ALSENS_PIN) == 0) 
		beep(1,150);
}
static void displog(void) {
	int8u i, status[7];
	char s[8];
	double dist;
	int16u co2;
	for (i = 0; i < 7; ++i) {
		status[i] = EEPROM_Read(INFO_EE_ADDR+i);
		dlyms(10);
	}
	lcdclr();
	lcdws ("LOG Parameters:");
	for (i = 0; i < 2; ++i) 
		if (status[i]) {
			lcdclrr(1);
			lcdws(stat[i]);
			dlyms(2000);
			lcdclrr(1);
		}
	if (status[2] == 1) {
		lcdclrr(1);
		lcdws(stat[2]);
		dlyms(2000);
	} else if (status[2] == 2) {
		lcdclrr(1);
		lcdws(stat[3]);
		dlyms(2000);
	}
	lcdclrr(1);
	lcdws("Temp :   c");
	lcdwint(0xc6,status[3]);
	dlyms(2000);
	lcdclrr(1);
	lcdws("Dist :    mts");
	dist = status[4] / 100.0;
	ftoa(dist,s,1);
	lcdptr = 0xc6;
	lcdws(s);
	dlyms(2000);
	lcdclrr(1);
	lcdws("CO2 :    ppm");
	co2 = (status[5] * 256) + status[6];
	itoa(co2,s);
	lcdptr = 0xc6;
	lcdws(s);
	dlyms(2000);
	lcdclr();
	lcdws("  Please Reset");
	beep(2,250);
	sleep_enable();
	sleep_cpu();
}
static void logparam(void) {
	int8u i, status[7];
	float dist;
	int16u co2;
	for (i = 0; i < 7; ++i)
		status[i] = 0;
	if (SENSOR_PIN & _BV(DOOR_PIN))
		status[0] = 1;
	if (ALSENS_IPIN & _BV(ALSENS_PIN) == 0)
		status[1] = 1;
	status[2] = collisens;
	status[3] = (int8u)meastemp(0);
	dist = MeasDist(0) * 100;
	status[4] = (int8u) (dist);
	co2 = measco2(0);
	status[5] = (co2 / 256);
	status[6] = (int8u)(co2 % 256);
	for (i = 0; i < 7; ++i) {
		EEPROM_Write(INFO_EE_ADDR+i, status[i]);
		dlyms(50);
	}
}
static int16u measco2(int8u disp){
	int16u co2;
	char s[8];
	co2 = 400 + (50 - (adcget(1) * STEP_SIZE * 100));
	if (disp) {
		itoa(co2,s);
		lcdptr = 0x8a;
		lcdws("   ");
		lcdptr = 0x8a;
		lcdws(s);
	}
	return co2;
}
static void dispparam(void){
	lcdclr();
	lcdws("T:      C:   ppm");
	lcdptr = 0x86;
	lcdwc(lcdptr);
	lcdwd(0xdf);
	lcdr2();
	lcdws("VehiDist:    mts");
}
