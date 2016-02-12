// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		vjoEvent.h
// Project:		vje32
//
// Contents:	Public vJo Event definition
//				Public vJo Driver definition
//
//
//
// Author:		Vadim Gorbatenko
// Started:		05.10.00 14:21:59
//
// Version:		3.50


/************************************
  REVISION LOG ENTRY
  Revision By: VG
  Revised on 12.10.01 15:37:26 : v2.50 
  Revised on 17.12.01 16:28:08 : v3.00
  Revised on 11.04.02 23:44:30	:v3.20 drivers package
  Revised on 03.12.02 20:35:00	:v3.50
  Comments: ...
 ************************************/


#ifndef	_VJOEVENT_DEF
#define _VJOEVENT_DEF

#ifndef	DECLARE_BUILT_IN_DRIVER
#define	VJODRVAPI	__declspec(dllexport)
#define	VJODRVEXPORT	CreateNewVJODObject
#define	VJODRVEXPORT2	GetVJODID
#define	VJODRVEXPORT3	GetPkgVersion

#define	VJODRVEXPORT_PKG		GetVJODFactoryInst
#define	VJODRVEXPORT2_PKG	GetVJODIDInst

#define	VJODRVFACTORYNAME	"CreateNewVJODObject"
#define	VJODRVIDGENNAME		"GetVJODID"
#define	VJODRVGETVERSION	"GetPkgVersion"

#define	VJODRVFACTORYNAME_PKG	"GetVJODFactoryInst"
#define	VJODRVIDGENNAME_PKG		"GetVJODIDInst"

#define	VJODRVEXT	"vjd"
typedef DWORD  (* VJODRVSELFIDPROC)( );
typedef DWORD  (* VJODRVSELFIDPROCPKG)( int );

#endif

class VJOEVENT
{
public:
	DWORD		dwEventID;
	DWORD		dwEventSourceID;//driver ID
	DWORD		dwEventFlags;
	DWORD		dwEventTime;	//msec
	LONG		lEventParam;
	LPCSTR		lpEventName;
	LONG		lDrvData;		//reserved for driver
	LONG		lDevManData;	//reserved for device manager
}; 

typedef	VJOEVENT *LPVJOEVENT;
//flags for events state(HIWORD)
#define	VJEVENT_ENABLED				0x40000000
#define	VJEVENT_SIGNALED			0x80000000
//flags for events action (LOWORD)
#define	VJEVENT_INCREMENT			0x00000001	 //default action, lEventParam = No. of steps(default=1)
#define	VJEVENT_ABSPOSITION			0x00000002	 //lEventParam = position, this flag controlled by driver
#define	VJEVENT_ACTION				0x00000003	 //this flag controlled by driver
#define	VJEVENT_ROOTPOSITION		0x00000004 //lEventParam = position in root list, this flag controlled by driver
//v2.50
#define	VJEVENT_TOGGLEOVERLAY		0x00000005 //lEventParam not used (switch overlay off/on)
#define	VJEVENT_IABUTTON			0x00000006 //lEventParam = button( 1 ... VJE_MAXIABUTTONS)
#define	VJEVENT_JUMPTO				0x00000007 //lEventParam = mark#
#define	VJEVENT_SELECTTRANS			0x00000008 //lEventParam = transition#
#define	VJEVENT_SELECTRNDTRANS		0x00000009 //lEventParam not used (random transition)
#define	VJEVENT_SELECTOVERLAY		0x0000000A //lEventParam = overlay#
#define	VJEVENT_SELECTRNDOVERLAY	0x0000000B //lEventParam  not used (random overlay)
#define	VJEVENT_IASWITCHOFF			0x0000000C //lEventParam = switch( 1 ... 16)
#define	VJEVENT_IASWITCHON			0x0000000D //lEventParam = switch( 1 ... 16)
//v3.5G
#define	VJEVENT_NEXTOVERLAY			0x0000000E //lEventParam  not used
#define	VJEVENT_PREVOVERLAY			0x0000000F //lEventParam  not used
//v4 gold
#define	VJEVENT_NEXTTRANS			0x00000011 //lEventParam  not used - ok
#define	VJEVENT_PREVTRANS			0x00000012 //lEventParam  not used - ok
#define	VJEVENT_SELECTRNDMARK		0x00000013 //lEventParam not used (random mark)
#define	VJEVENT_JUMPFROMMARKRANGE	0x00000014 //lEventParam = mark#-end
#define	VJEVENT_JUMPTOMARKRANGE		0x00000015 //lEventParam = start-mark#
#define	VJEVENT_OVERLAYON			0x00000016 //lEventParam  not used - ok
#define	VJEVENT_OVERLAYOFF			0x00000017 //lEventParam  not used - ok


#define	VJEVENT_LASTACTION	VJEVENT_PREVOVERLAY

//predefined positions in lEventParam
#define	VJEVENT_POS_BEGING			0
#define	VJEVENT_POS_END				-1
#define	VJEVENT_POS_BLACKTREE		-2 //jump to system tree - "black color screen"
#define	VJEVENT_POS_WHITETREE		-3 //jump to system tree - "white color screen"
#define	VJEVENT_POS_COLORTREE		-4 //jump to system tree - "any color screen"
#define	VJEVENT_POS_WALLPAPERTREE	-5 //jump to system tree - "WALLPAPER screen"
#define	VJEVENT_POS_RANDOM			-6 //jump to random position
#define	VJEVENT_QUIT_LIST			-7 //jump out of list (for sublist), else jump to next
#define	VJEVENT_RESTART_LIST		-8 //jump to absolute #0, quit all sublists

#define	VJEVENT_POS_LASTVALUE		-128// jl changed VJEVENT_RESTART_LIST

#define	VJE_MAXIABUTTONS	8
class CVJoEngine;

// VJO DEVICES
//

// helpers & macros
class _dll_manage_state
{
public:
	_dll_manage_state()
	{
		m_hPrevResHandle = AfxGetResourceHandle();
		HINSTANCE	hNewResHandle;
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			hNewResHandle = AfxGetResourceHandle();
		}
		AfxSetResourceHandle(hNewResHandle);
	}
	~_dll_manage_state()
	{
		AfxSetResourceHandle(m_hPrevResHandle);
	}
protected:
	HINSTANCE m_hPrevResHandle;
};

#define	VJODRV_MANAGE_STATE()	_dll_manage_state state_mang

#define	DECLARE_VJODEVICEDRIVER(id)\
public:\
static	CVJODeviceDriver* CreateDriver();\
static	CBitmap	m_DeviceBitmap;\
static	LPVOID	m_pDriverInstance;\
static	CString	m_DeviceInfo;\
static	CHAR	*m_DeviceName;\
static	CHAR	*m_DeviceDataName;\
static	DWORD	m_dwRawDataID;\
static	UINT	m_uBmpRes;\
static	VJOEVENT m_pEvents[];\
static	INT		m_iEventsCount;\
enum	{ID = id};\
LPCSTR		GetDeviceInfo()		{return __getDeviceInfo();}\
LPCSTR		GetDeviceName()		{return __getDeviceName();}\
LPCSTR		GetDeviceDataName()		{return __getDeviceDataName();}\
CBitmap&	GetDeviceBitmap()	{return __getDeviceBitmap();}\
virtual DWORD	GetDriverID()		{return ID;}\
DWORD		GetDeviceRawDataID()	{return __getDrvRawDataID();}\
INT			GetEventsCount()	{return __getEventsCount();}\
LPVJOEVENT	GetEventPtr(int p)	{return __getEventPtr(p);}\
protected:\
virtual LPCSTR	__getDeviceInfo()	{return (LPCSTR)m_DeviceInfo;}\
virtual LPCSTR	__getDeviceName()	{return (LPCSTR)m_DeviceName;}\
virtual LPCSTR	__getDeviceDataName()	{return (LPCSTR)m_DeviceDataName;}\
virtual INT		__getEventsCount()	{return m_iEventsCount;}\
virtual LPVJOEVENT	__getEventPtr(int p)	{return (p>=0 && p<m_iEventsCount) ? m_pEvents+p : NULL;}\
virtual DWORD		__getDrvRawDataID()	{return m_dwRawDataID;}\
virtual CBitmap&	__getDeviceBitmap()	{return m_DeviceBitmap;}\
virtual void	__unreg()	{m_pDriverInstance=NULL;}

#define	BEGIN_EVENTS_DESC(drvClass)\
		VJOEVENT drvClass::m_pEvents[] = {\

#define	END_EVENTS_DESC(drvClass) };\
		INT drvClass::m_iEventsCount = sizeof(m_pEvents)/sizeof(VJOEVENT);

#define	DEFINE_NOEVENTS_DESC(drvClass)\
		VJOEVENT drvClass::m_pEvents[1];\
		INT drvClass::m_iEventsCount =0;

#define	BEGIN_DRIVER_DESC(drvClass, name, dname, dataID, uBitmap )\
	CHAR	*drvClass::m_DeviceName = name;\
	CHAR	*drvClass::m_DeviceDataName = dname;\
	DWORD	drvClass::m_dwRawDataID = dataID;\
	LPVOID	drvClass::m_pDriverInstance = NULL;\
	UINT	drvClass::m_uBmpRes = uBitmap;

#define	END_DRIVER_DESC(drvClass)

#define IMPLEMENT_DRIVER_DYNCREATE(drvClass,info) \
	CBitmap	drvClass::m_DeviceBitmap;\
	CString	drvClass::m_DeviceInfo = info;\
	CVJODeviceDriver* drvClass::CreateDriver() { if( drvClass::m_pDriverInstance) return NULL;\
		CVJODeviceDriver *p= new drvClass;drvClass::m_pDriverInstance=p; if(!(HBITMAP)drvClass::m_DeviceBitmap) \
		{_dll_manage_state ms; drvClass::m_DeviceBitmap.LoadBitmap(drvClass::m_uBmpRes); } return p;}


//Requests
#define	VJDR_GETEVENTS	0x01
#define	VJDR_GETPARAMS	0x02
#define	VJDR_GETRAWDATA	0x04
#define	VJDR_ALL	(VJDR_GETEVENTS|VJDR_GETPARAMS|VJDR_GETRAWDATA)

typedef	struct
{
	INT		ref;	//reference counter
	DWORD	rdid;	//raw data id
} RDATAREFINFO;
//messages for driverproc						lpParam
#define	VJDMSG_RAWDATATREFCOUNTER	0x0001//	&nRefCounter or &RDATAREFINFO
#define	VJDMSG_ENGINESTATECHANGED	0x0002//	&rendering on/off
#define	VJDMSG_ISDRIVERHIDDEN		0x0003//	NULL

/*
 * NOTE: All Message Numbers below 0x0400 are RESERVED.
 *
 * Private Driver Messages Start Here:
 */
#define	VJDMSG_USER					0x400
// BASE CLASS DECLARATION
//
class	CVJODeviceDriver
{
	DECLARE_VJODEVICEDRIVER(0)

public:

virtual ~CVJODeviceDriver() {__unreg();}
virtual BOOL	Startup(CVJoEngine *lpEngine=NULL)	{return 0;}
virtual BOOL	Shutdown()	{return 0;}
virtual BOOL	IsStarted()	{return 0;}

virtual	LPVOID	GetConfigData(INT &ub) {return 0;}
virtual	BOOL	SetConfigData(LPVOID lp, INT ub) {return 0;}
virtual	DWORD	GetMemoryUsage() {return 0;}
virtual	void	ResetState() {;}
virtual	void	VerifyEvents() {;}


virtual	BOOL	SetupCommon()	{return 0;}
virtual	BOOL	SetupEvent(DWORD id, BOOL bQuery)	{return 0;}
virtual	LPCSTR	DescribeEvent(VJOEVENT *lpEvent)	{return 0;}
virtual	void	DriversListChanged() {return ;}

//main
virtual	UINT	Poll(UINT uRequest) {return 0;}
virtual	LPVOID	GetRawDataPtr() {return 0;}

//called by device manager
virtual	LPVJOEVENT PerformAction(VJOEVENT *lpEvent) {return 0;}//for VJEVENT_ACTION
virtual	void	EventProcessed(VJOEVENT *lpEvent)	{return;}//notification

virtual	LONG	DriverProc(UINT uMsg, LPVOID lpParam) {return 0;}//general driver proc
protected:
	CVJODeviceDriver()	{;}//see note 0
};

typedef CVJODeviceDriver * (* VJODRVFACTORY)( );

typedef VJODRVFACTORY (* VJODRVFACTORYPKG)( int );

#define DECLARE_EXPORT(drvClass)\
extern "C" {\
		VJODRVAPI	CVJODeviceDriver *VJODRVEXPORT() {return drvClass::CreateDriver();}\
		VJODRVAPI	DWORD	VJODRVEXPORT2() {return drvClass::ID;}\
}

#define SIMPLE_EVENT(drv, UID, name,d)		{MAKELONG(UID,drv::ID), drv::ID, VJEVENT_INCREMENT|VJEVENT_ENABLED, 0, 1, name, d}
#define SPECIAL_EVENT(drv, UID, name, act,  d)	{MAKELONG(UID,drv::ID), drv::ID, VJEVENT_ABSPOSITION|VJEVENT_ENABLED, 0, act, name, d}
#define DISSABLED_EVENT(drv, UID, name,d)		{MAKELONG(UID,drv::ID), drv::ID, VJEVENT_ENABLED, 0, 1, name, d}
#define DRIVERACT_EVENT(drv, UID, name, d)	{MAKELONG(UID,drv::ID), drv::ID, VJEVENT_ACTION|VJEVENT_ENABLED, 0, 0, name, d}

// More Info:
// Every instance of driver NEEDS a codes for implementaion of static members for base class
// todo so, just add:
//	#define DECLARE_IMPLEMENT_BASE_VJODCLASS
//	#include "vjoEvent.h"
// Add this definition only in ONE file of your project!
// This will adds the follow codes into you file:
#ifndef	DECLARE_BUILT_IN_DRIVER
	#ifdef DECLARE_IMPLEMENT_BASE_VJODCLASS
	DEFINE_NOEVENTS_DESC(CVJODeviceDriver)
	BEGIN_DRIVER_DESC(CVJODeviceDriver, "", "", 0, 0)
	END_DRIVER_DESC(CVJODeviceDriver) 
	IMPLEMENT_DRIVER_DYNCREATE(CVJODeviceDriver, "")
	#endif
#endif
//add string for built-in driver
//#define DECLARE_BUILT_IN_DRIVER 
//
#endif