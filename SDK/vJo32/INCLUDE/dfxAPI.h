// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		dfxAPI.h
// Project:		vje32
//
// Contents:	Public DFX API
//
//
//
// Author:		Vadim Gorbatenko
// Started:		22.02.00 14:13:00
//
// Version:		3.50


/************************************
  REVISION LOG ENTRY
  Revision By: VG
  Revised on 20.05.00 13:45:15
  Revised on 18.01.01 20:16:19	API changed
  Revised on 22.01.01 23:56:09	API changed
  Revised on 11.04.01 12:20:42	API extended
  Revised on 07.05.01 15:29:46	API extended
  Revised on 23.05.01 16:06:57	API extended
  Revised on 12.10.01 15:32:27	v2.50, API extended
  ================= VERSION 3.00 ===================
  Revised on 11.11.01 16:01:30	V3.00
  Revised on 21.12.01 0:36:38
  Revised on 14.01.02 0:16:06
  ================= VERSION 3.50 ===================
  Revised on 13.05.03 18:26:31	Version control. Uses m_EffectFlags
  ================= VERSION 3.50 GOLD =======
  Revised on 01.10.04			Added new data-memebers to CScreen
 ************************************/

#ifndef _DFXAPI_DEF
#define	_DFXAPI_DEF

#define	DFX_CURRENT_VERSION	350

#define	PLUGINAPI	__declspec(dllexport)
#define	DFXEXPORT	CreateNewDFXObject
#define	PLUGINFACTORYNAME	"CreateNewDFXObject"

typedef struct
{
	SIZE	resolution;
	CHAR	*title;	
} _screen_info;




#define	DECLARE_INPUTSCREENS()\
protected:\
		static _screen_info	m_InputScreensInfo[];\
		static INT			m_InputScreensCount;\
virtual	INT		 __getInputScreensCount()		{return m_InputScreensCount;}\
virtual	SIZE	__getInputScreenRes(int p)		{return m_InputScreensInfo[p].resolution;}\
virtual	CHAR	*__getInputScreenName(int p)	{return m_InputScreensInfo[p].title;}

#define	DECLARE_OUTPUTSCREENS()\
		static _screen_info	m_OutputScreensInfo;\
virtual	INT		 __getOutputScreensCount()	{return 1;}\
virtual	SIZE	__getOutputScreenRes()		{return m_OutputScreensInfo.resolution;}\

#define	DECLARE_EFFECT_INFO()\
protected:\
	static	CHAR			*m_EffectName;\
	static	CHAR			*m_EffectAuthor;\
	static	CHAR			*m_EffectInfo;\
	static	DWORD			m_EffectFlags;\
	static	EFFECTUID		m_EffectUID;\
virtual	CHAR	*__getEffectName()	{return m_EffectName;}\
virtual	CHAR	*__getEffectAuthor()	{return m_EffectAuthor;}\
virtual	CHAR	*__getEffectInfo()	{return m_EffectInfo;}\
virtual	DWORD	__getEffectFlags()	{return m_EffectFlags;}\
virtual	EFFECTUID __getEffectUID()	{return m_EffectUID;}\



#define	BEGIN_INPUTSCREENS_DESC(effectClass)\
		_screen_info effectClass::m_InputScreensInfo[] = {\

#define	END_INPUTSCREENS_DESC(effectClass) };\
		INT effectClass::m_InputScreensCount = sizeof(m_InputScreensInfo)/sizeof(_screen_info);

#define	BEGIN_OUTPUTSCREENS_DESC(effectClass)\
		_screen_info effectClass::m_OutputScreensInfo = {

#define	END_OUTPUTSCREENS_DESC(effectClass) };

#define	BEGIN_EFFECT_DESC(effectClass, uuid, flags, name, author, info)\
		CHAR* effectClass::m_EffectName = name;\
		CHAR* effectClass::m_EffectAuthor = author;\
		CHAR* effectClass::m_EffectInfo = info;\
		EFFECTUID effectClass::m_EffectUID = uuid;\
		DWORD effectClass::m_EffectFlags = (DFX_CURRENT_VERSION<<16)|(flags&0xffff);\

#define	END_EFFECT_DESC(effectClass)

#define DECLARE_EFFECT()\
		DECLARE_INPUTSCREENS()\
		DECLARE_OUTPUTSCREENS()\
		DECLARE_EFFECT_INFO()

#define	DEFINE_NOINPUTSCREENS_DESC(effectClass)\
		_screen_info effectClass::m_InputScreensInfo[1];\
		INT effectClass::m_InputScreensCount =0;


////////////////////// DFX PUBLIC INTEFACE //////////////////////////////////////////////
class CScreen;
class CConfigData;
class CProxyEngine;

class CDFX
{
	DECLARE_EFFECT()

public:

virtual ~CDFX()		{}

virtual	BOOL	UpdateConfig()
					{return TRUE;}
virtual BOOL	Initialize()
					{return TRUE;}
virtual BOOL	Render(CScreen **input, CScreen *output)
					{return TRUE;}


		//	info part	
virtual	BOOL	SetupDialog()				{return FALSE;}
		LPCSTR	GetAuthorName()				{return __getEffectAuthor();}
virtual	LPCSTR	GetEffectName()				{return __getEffectName();}
		LPCSTR	GetGreetings()				{return __getEffectInfo();}
		DWORD	GetFlags()					{return __getEffectFlags();}
virtual	SIZE	GetInputScreenRes(int i, SIZE szOutput)	{return __getInputScreenRes(i);}
virtual	LPCSTR	GetInputScreenName(int i)	{return (LPCSTR)__getInputScreenName(i);}
		INT		GetInputScreensCount()		{return __getInputScreensCount();}
		SIZE	GetOutputScreenRes()		{return __getOutputScreenRes();}
		EFFECTUID GetEffectUID()			{return __getEffectUID();}
virtual HBITMAP	GetLabelBitmap()			{return HBITMAP_NO;} //bitmap must be 64x64!
virtual	BOOL	ConfirmRenderSource(int i)	{return TRUE;}
virtual	DWORD	GetInputState()				{return 0xffffffff;} 
virtual	DWORD	GetMemoryUsage()			{return 0;}

virtual	LONG	SendEffectMessage(UINT uMsg, LPARAM lParam)	{return 0;}
virtual	VOID	ProcessRawData(UINT uiData, LPVOID lpData)	{}
virtual	BOOL	SetResourcePath(LPCSTR lpPath, BOOL bQuery)	{return 0;}
virtual	LPCSTR	GetResourcePath(int indx)					{return NULL;}
virtual	void	OnEnterIdleState()	{} //called when dfx becomes nonactive(cached) in playlist
		CProxyEngine		*m_pEngine;
protected:
		CDFX()		{m_pEngine = NULL;}
};


// Note:
// Every instance of dfx NEEDS a codes for implementaion of static members for base class
// todo so, just add:
//	#define DECLARE_IMPLEMENT_BASE_CLASS
//	#include "dfxAPI.h"
// Add this definition only in ONE file of your dfx project!
// This will adds the follow codes into you file:

#ifdef DECLARE_IMPLEMENT_BASE_CLASS
BEGIN_INPUTSCREENS_DESC(CDFX)
{{-1,-1}, NULL}
END_INPUTSCREENS_DESC(CDFX)

BEGIN_OUTPUTSCREENS_DESC(CDFX)
{-1,-1}, NULL
END_OUTPUTSCREENS_DESC(CDFX)

BEGIN_EFFECT_DESC(CDFX, 0, 0, " ", " ", " ")
END_EFFECT_DESC(CDFX) 
#endif

class CScreen
{
public:

virtual				~CScreen()					= 0;
virtual	void		SwapBuffers(CScreen *other) = 0;
virtual	LPVOID		GetBuffer()					= 0;
virtual	SIZE		GetSize()					= 0;

	DWORD	m_dwUserFlags;
	DWORD	m_dwUserData;
};


typedef struct
{
	UINT	uDriverID;	//public driver ID
	UINT	uMessageID;	//message ID
	LPVOID	pMessage;	//message data
	LONG	lResult;	//result
} DFX4DRVMSG;


//see idfx.h for list of available interfaces

typedef	struct
{
	LPCSTR	lpVarName;		// The name of variable: displayed in dialog
	LPVOID	lpVarAddress; 	// Actual address
	FLOAT	flMinValue;		// Minimal value of variable (for FXVAR_FLOAT & FXVAR_RGB)
	FLOAT	flMaxValue;		// Maximal value of variable (for FXVAR_FLOAT & FXVAR_RGB)
	
	FLOAT	flMinPeriod;	// Minimal period of variable
	FLOAT	flMaxPeriod;	// Maximal period of variable
	FLOAT	flDefPeriod;	// Define initial period
	FLOAT	flDefAmplitude;	// Define initial amplitude [0...(flMaxValue-flMinValue)/2]
	DWORD	dwFlags;		// FXVAR_xxx
} FXVAR;


class CProxyEngine
{
public:
	virtual ~CProxyEngine() = 0;
	
	virtual HWND	GetOutputWindow() = 0;
	virtual	DWORD	GetEngineFlags() = 0;
	virtual	CDFX	*GetSourceFX(int) = 0;
	virtual	CDFX	*GetTargetFX() = 0;
	virtual DWORD	GetNodeID()	= 0; //ie "in-tree-coordinate", this ID is unique for every slot inside fxc tree
	virtual	CDFX	*GetNodeByID(DWORD) = 0;
	virtual LONG	GetExternInterface(UINT uid) = 0;
	virtual	CConfigData *GetCurrentConfig() = 0;
	virtual SIZE	GetOutputRes() = 0;	
	virtual SIZE	GetInputRes(int) = 0;	
	virtual	DWORD	GetRenderStepsCount() = 0;
	virtual	DWORD	GetEngineTime(BOOL bNonVirtual = FALSE) = 0;
	virtual	INT		GetDeltaTime() = 0;
	virtual	LONG	SendEngineMessage(UINT uMsg, LPARAM lParam) = 0;

	//descrete stream
	virtual	UINT	RegisterVariable(FXVAR *pVar) = 0;
	virtual	BOOL	UnregisterVariable(UINT) = 0;
	virtual	BOOL	ConfigureVariable(UINT) = 0;
	
	//raw data stream
	virtual	BOOL	RegisterRawDataTarget(UINT uiData) = 0;
	virtual	BOOL	UnregisterRawDataTarget(UINT uiData) = 0;

	
	//temporary buffer( for Render() only)
	virtual	LPVOID	GetTmpBuffer(int nBytes) = 0;

	//permanent JL stores
	virtual void	SignalModified() = 0;
	virtual BOOL	SetComStat(int position, int value) = 0;
	virtual int		GetComStat(int position) = 0;
	virtual void	SetPreviewScreen(DWORD* pScreen, int screen, int width, int height)	= 0;
	virtual void	GetPreviewScreen(DWORD* pScreen, int screen, int width, int height)	= 0;
	virtual CString GetUserDirectory(int dir, int pos) = 0;
	virtual void	SetUserDirectory(int dir, int pos, CString directory) = 0;

	virtual CString	GetRootDirectory() = 0;
};

//SendEngineMessage ID's						lParam
#define	DFXMSG_QUERYDRIVER			0x00100	//	DFX4DRVMSG *
#define	DFXMSG_CALLDRIVER			0x00101	//	DFX4DRVMSG *
#define	DFXMSG_CONFIRMBUFFEROUTPUT	0x00102	//	NULL
#define	DFXMSG_LOCKRENDERER			0x00103	//	TRUE/FALSE
#define	DFXMSG_GETENGINEVERSION		0x00300	//	version (as defined in vjodefines.h)
#define DFXMSG_GETENGINERENDERSTATUS 0x00369 // TRUE/FALSE (Jean-Luc)

//Engine messages ID
#define	VJEMSG_MODE_CHANGING		0x00010 //	sent right before mode changed (no params)
#define	VJEMSG_MODE_CHANGED			0x00011 //	sent right after mode changed (no params)
#define	VJEMSG_PLAYLISTPOS_CHANGED	0x00012 //	sent to fxc when it becomes active(no params)
#define	VJEMSG_ATTACHRENDERTHREAD	0x00013 //	sent right after new thread for rendering has created (no params)
#define	VJEMSG_DETACHRENDERTHREAD	0x00014 //	sent right befo thread for rendering has destroyed (no params)

#define VJEMSG_RENDERERSTATE		0x0022		// TRUE/FALSE	notifys about starting or stopping rendering

#ifndef _VJOEAPI_DEF
//some engine flags
#define	VJEMODE_PRESENTATION	0x80000000 //screen mode (==VJEMODE_FULLSCREEN without VJEMODE_WINDOWED)
#define	VJEMODE_DIBOUTPUT		0x40000000 //output to dib
#define	VJEMODE_PLAYLIST		0x20000000 //single fxc
#define	VJEMODE_RENDERFXC		0x10000000 //playlist
#define	VJEMODE_DUALHEAD		0x00400000 //diagnostic flag: ignored during startup
#define	VJEMODE_WINDOWED		0x00800000 //windowed(non-exclusive) (used with VJEMODE_PRESENTATION)
#endif

#define	FXVARALLVARS	0xFFFFFFFF

//kind of variable 
#define	FXVAR_FLOAT		0
#define	FXVAR_BOOL		1

//which members of FXVAR are valid
#define	FXVAR_PERIODIC	0x08	//allow periodic function, flMinPeriod & flMaxPeriod are valid 
#define	FXVAR_DEFPERIOD	0x10	//flDefPeriod is valid when periodic function allowed
#define	FXVAR_YESNO		0x20	//for BOOL:  "Yes/No",   default "On/Off"
#define	FXVAR_DEFAMLPITUDE 0x040	//flDefAmplitude is valid when periodic function allowed
#define	FXVAR_DEFPERIODIC 0x080	//force to create periodic funcion as default(must have FXVAR_PERIODIC flag!)
#define	FXVAR_DEFCOS	0x1000	//init sine function

class CConfigData
{
public:
	virtual			~CConfigData()	= 0;
	virtual	int		GetInt(LPCSTR name, int defaultValue)		= 0;
	virtual	LPCSTR	GetString(LPCSTR name, LPCSTR defaultValue)	= 0;
	virtual	float	GetFloat(LPCSTR name, float defaultValue)	= 0;
	virtual	LPVOID	GetBuffer(LPCSTR name, int& bufSize)		= 0;

	virtual	BOOL	SetInt(LPCSTR name, int Value)		= 0;
	virtual	BOOL	SetString(LPCSTR name, LPCSTR Value)= 0;
	virtual	BOOL	SetFloat(LPCSTR name, float Value)	= 0;
	virtual	BOOL	SetBuffer(LPCSTR name, LPVOID buf, int bufSize) = 0;
	
	virtual	LPCSTR	GetFilePath(LPCSTR name, LPCSTR defaultValue) = 0;
	virtual	LPCSTR	GetFileExt(LPCSTR name, LPCSTR defaultValue)  = 0;
	virtual	BOOL	SetFilePath(LPCSTR name, LPCSTR lpPath, LPCSTR lpExt) = 0;
	virtual	BOOL	GetEngineResourcePathList(CStringArray& lst) = 0;
					//IDCANCEL,IDOK, or 0 if not supported
	virtual	INT		SelectResourceDlg(CString& pResult, BOOL bDirOnly = FALSE, LPCSTR lpInitialDir=NULL) = 0;

	virtual CString	GetFilePath2(LPCSTR res_name, LPCSTR defaultValue, CProxyEngine* m_pEngine) = 0;//jl added
	virtual	BOOL	SetFilePath2(LPCSTR res_name, LPCSTR lpPath, LPCSTR lpExt, CProxyEngine* m_pEngine) = 0;//jl added
};

//
// correct routing for DLL state managing
//
class _vjo_manage_state
{
public:
	_vjo_manage_state()
	{
		m_hPrevResHandle = AfxGetResourceHandle();
		HINSTANCE	hNewResHandle;
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			hNewResHandle = AfxGetResourceHandle();
		}
		AfxSetResourceHandle(hNewResHandle);
	}
	~_vjo_manage_state()
	{
		AfxSetResourceHandle(m_hPrevResHandle);
	}
protected:
	HINSTANCE m_hPrevResHandle;
};
#ifdef	_AFXDLL
#define	VJO_MANAGE_STATE()	_vjo_manage_state state_mang
#else
#define	VJO_MANAGE_STATE()
#endif


//future extentions >V3.00
//data id for  SendEffectMessage(UINT uMsg, LPARAM lParam)
//							uMsg		lParam		fx returns

#define	FXM_USER		0x2000	//all custom messages must be >=FXM_USER

//data id for  SendEngineMessage(UINT uMsg, LPARAM lParam)
//							uMsg		lParam		engine returns

#endif
