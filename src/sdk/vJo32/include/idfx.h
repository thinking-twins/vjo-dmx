// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		idfx.h
// Project:		vje32
//
// Contents:	dfx extern interfaces
//
//
//
// Author:		Vadim Gorbatenko (NASH)
// Started:		07.04.01 14:07:15
//
// Version:		3.00

/************************************
  REVISION LOG ENTRY
  Revision By: VG
  Revised on 12.10.01 15:38:33 : v2.50
  Revised on 11.11.01 21:04:34 : v3.00
  Comments: ...
 ************************************/


#ifndef	_IDFX_DEF
#define	_IDFX_DEF

#define	__IDFSPEC__	0x300

#ifndef _VJOEAPI_DEF
//vJo engine flags
#define	VJEMODE_FULLSCREEN		0x80000000 //full screen mode
#define	VJEMODE_DIBOUTPUT		0x40000000 //output to dib
#define	VJEMODE_PLAYLIST		0x20000000 //runs playlist
#define	VJEMODE_RENDERFXC		0x10000000 //runs single fxc
#define	VJEMODE_DEMOMODE		0x01000000 //DEMO MODE
#define	VJEMODE_NOEDIT			0x02000000 //editing prohibited
#define	VJEMODE_SAFEMODE		0x04000000 //SAFE MODE: no fullscreen, no NET, no devices!
#define	VJEMODE_SERVERMODE		0x00100000 //vJo in Server mode: remote controller available
#endif



// available interfaces & objects ID for dfx's
#define	IDFX_VJEAUTOPILOT	0x201	//vJo Autopilot
#define	IDFX_SOCKET			0x202	//vJo Socket
#define	IDFX_DISPATCHER		0x203	//vJo Data Dispatcher

// virtual base classes

// special version of autopilot wrapper for dfx
class	CIDFXAutoPilot
{
public:
	virtual ~CIDFXAutoPilot() = 0;
	virtual	void PostEvent(DWORD	dwEventID, DWORD dwEventFlags, LONG	lEventParam) = 0;
	virtual	BOOL IsPlaylist()	 = 0;
	virtual	BOOL IsTimeLine()	 = 0;
};



#ifndef _VJMS_DEF
#define	VJMS_PACKET_SIZE	256
typedef LPVOID HPORT;
#endif

// special version of socket wrapper for dfx
class	CIDFXSocket
{
public:
	virtual ~CIDFXSocket() = 0;
	virtual HPORT	CreatePortForWriting(DWORD dwDataID) = 0;
	virtual HPORT	FindPort(DWORD	dwDataID) = 0;
	virtual BOOL	GetPortInfo(HPORT hPort, DWORD *pDataID, INT *pNumBlocks, DWORD *pBlocksPassed) = 0;
	virtual INT		GetData(HPORT hPort, LPBYTE pDataBlock, INT nBlockIndx) = 0;
	virtual BOOL	PutData(HPORT hPort, LPBYTE pData, int nBytes) = 0;
};

#ifndef	_IFXVAR_DEF
typedef	DWORD	HPARAM;
typedef	DWORD	PARAMID;
typedef	DWORD	SRCID;

#define	UNKNOWNHANDLE 0
#define	INVALIDHANDLE ((HPARAM)-1)
#define	ADDSRCREF ((HPARAM)-2)
#define	REMSRCREF ((HPARAM)-3)
#define	GETSRCREF ((HPARAM)-4)

#define	PARAM_USEGRAVITY	0x01
#define	PARAM_USEFRICTION	0x02
#define	PARAM_USEINERTIA	0x04

#endif

class CIDFXDispatcher
{
public:

virtual ~CIDFXDispatcher() = 0;
		//params dispatcher
virtual SRCID	RegisterSource(LPCSTR lpSrcName, BOOL bBoolParam) = 0;
virtual BOOL	UnregisterSource(SRCID uSrc) = 0;
virtual PARAMID	RegisterParameter(SRCID uSrc, LPCSTR lpParamName, LPCSTR lpParamDesc, UINT uParamsFlags = 0) = 0;
virtual HPARAM	UpdateParam(HPARAM hParam, PARAMID uParam, FLOAT flParam) = 0;
virtual HPARAM	UpdateParam(HPARAM hParam, PARAMID uParam, BOOL bParam) = 0;

};


#endif
