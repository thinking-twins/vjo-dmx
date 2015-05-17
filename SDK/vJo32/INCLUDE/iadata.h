// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY GREEN HIPPO 
//
// File:		iadata.h
// Project:		vje32
//
// Contents:	IA Data specification	
//
//
//
// Author:		Vadim Gorbatenko
// Started:		20.11.01 11:47:05
//
// Version:		3.0
#ifndef	_IADATA_DEF
#define _IADATA_DEF

#define	IAVERSION	300

#define	IADATA_FLOAT   0
#define	IADATA_BOOL    1

typedef struct
{
	LPCSTR pChannelName;
	LPCSTR pChannelDesc;
	LPCSTR pSrcName;
	unsigned char uDataFlags;
	union
	{
		BOOL  bValue;
		float flValue;
	};
} IACHANNEL;

typedef struct
{
	DWORD		dwVersion;	//versioning
	INT			numChannels;
	IACHANNEL	*pChannels;
	DWORD		dwTableID;	//if IA data list changed, this ID also changed, so dfx must re-map it again
	DWORD		dwReserved;
} IADATAHEADER;

#endif

