#ifndef SMS_H
#define SMS_H

//INCLUDE HEADER FILES
#include "includes.h"

//FUNCTION PROTOTYPE

void GPSinit	(void);
void sendloc	(const char *PNum, const char *GSMMsg);
void SendOTP	(const char *PNum,const char *GSMMsg);
void SendLinkLoc(const char *PhNum, const char *GSMMsg);

#endif