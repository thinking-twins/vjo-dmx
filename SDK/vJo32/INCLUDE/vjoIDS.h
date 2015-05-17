// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		vjoIDS.h
// Project:		vje32
//
// Contents:	SHARED UIDS for device drivers & events
//
//
//
// Author:		Vadim Gorbatenko
// Started:		09.10.00 12:18:24
//
// Recent Version:		

//std drivers
#define	UID_TIMERDRV		1
#define	UID_SSYSTEMDRV		2
#define	UID_VSYSTEMDRV		3
#define	UID_MIDISYSTEMDRV	4
#define	UID_KEYBOARDDRV		5
#define UID_KBDMAPPER		6
#define UID_COMMUNICATORDRV 7
#define	UID_IDCTLDRV		8
#define UID_TIMEOUT         9
#define	UID_CMPDRV			10
#define UID_RS232LISTENDRV 0x100
#define	UID_HIPPODRIVER		0x101

//create events UID's  using MAKELONG(#enent_N, #driverUID)
