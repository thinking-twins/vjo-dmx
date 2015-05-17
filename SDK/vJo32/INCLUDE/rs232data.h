// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY VISUALIGHT 
//
// File:		rs232data.h
// Project:		implementation of rs232 listener-driver for vJo 2.2
//
// Contents:	InterfaceStructure for RS232Support
//
//
//
// Author:		Yannick Suter
// Started:		09.05.00 17:19:00
//
// Version:		1.00

#ifndef RS232DATA_DEF
#define RS232DATA_DEF

typedef	struct
{
	unsigned char cBuffer[256];
} RS232DATAHEADER;

#endif