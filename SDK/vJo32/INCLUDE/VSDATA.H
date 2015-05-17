// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		vsdata.h
// Project:		vje32
//
// Contents:	
//
//
//
// Author:		Vadim Gorbatenko
// Started:		11.04.00 13:28:04
//
// Version:	3.50

#ifndef VSDATA_DEF
#define VSDATA_DEF

typedef	struct
{
	SIZE	szImageSize;
	UINT	uiBPP;
	LPVOID	lpBuffer;
	DWORD	dwBitFormat;
} RAWIMAGE;

enum
{
	BI_YUV2 = 0x32595559,
	BI_YVU9	= 0x39555659,
	BI_YV12	= 0x30323449,
	BI_UYVY	= 0x59565955

};

typedef	struct
{
	DWORD		dwStructSize;//versioning
	DWORD		dwTime;
	
	CHAR		*pImageSource;//description of source
	
	RAWIMAGE	pRawImage;

	//reserved
	BYTE		reserved[64];

}	VSDATAHEADER;

//messages to driver. VJDMSG_USER is defined in vjoevent.h
//													param			return
#define	VLABDRV_GET_NUMCHANNELS		(VJDMSG_USER+1)		//no			int
#define	VLABDRV_GET_CHANNELNAME		(VJDMSG_USER+2)		//*channel		LPCSTR
#define	VLABDRV_GET_CHANNELDATAID	(VJDMSG_USER+3)		//*channel		DWORD
#define	VLABDRV_CONFIGURE_CHANNEL	(VJDMSG_USER+4)		//*channel		int
		

#endif