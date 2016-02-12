// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY GREEN HIPPO 
//
// File:		vjostdds.h
// Project:		vje32
//
// Contents:	VJO STANDARD DATASTREAMS ID's
//
//
//
// Author:		Vadim Gorbatenko
// Started:		18.05.01 14:29:58
//
// Version:		3.00

#ifndef _VJDS_DEF
#define _VJDS_DEF

// Predefined data ID's 
// VJO Data streams					desc									client access
#define	VJDS_REQUEST		0x0001	//input request for vjoServer			writeonly
#define	VJDS_RESPONSE		0x000A	//output response from vjoServer(sys)	readonly
#define	VJDS_ENGINESTATE	0x0003	//engine state changes(DWORD)			readonly
#define	VJDS_ENGINEDEBUG	0x0004	//debug string							readonly
#define	VJDS_ENGINEERROR	0x0005	//error string							readonly

#define	VJDS_RESPONSEFIRST	(VJDS_RESPONSE+1)//output response from vjoServer		readonly,exclusive
#define	VJDS_RESPONSELAST	(VJDS_RESPONSEFIRST+63)
//note about VJDS_RESPONSE__: clent application must find and open a free port in a range 
//from VJDS_RESPONSEFIRST to VJDS_RESPONSELAST. It must open this port for exclusive reading

#define	VJDS_USER			0x1000	//for all non-standard data ID's must be placed in a range
									//0x1000 -0xFFFF
#define VJDS_MP3     (VJDS_USER+1)
#define VJDS_PREVIEW (VJDS_USER+2)
#endif