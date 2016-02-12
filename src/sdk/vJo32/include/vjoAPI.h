// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		vjoAPI.h
// Project:		vje32
//
// Contents:	vJo engine public API definition
//
//
//
// Author:		Vadim Gorbatenko
// Started:		21.02.00 14:00:52
//
// Version:		3.50



#ifndef _VJOEAPI_DEF
#define _VJOEAPI_DEF

#ifdef	__cplusplus //only for C++


typedef	LPVOID		HEFFECT;
typedef DWORD		EINFO;

#define	METAEFFECT_FLAG			0x80000000
#define	FXC_PROTECTED			0x40000000
#define	FXC_LOCKED				0x20000000

//Engine mode flags (startup)
#define	VJEMODE_PRESENTATION	0x80000000 //[re]start in presentation mode
#define	VJEMODE_DIBOUTPUT		0x40000000 //force output to dib
#define	VJEMODE_PLAYLIST		0x20000000 //diagnostic flag: ignored during startup
#define	VJEMODE_RENDERFXC		0x10000000 //diagnostic flag: ignored during startup
#define	VJEMODE_DEMOMODE		0x01000000 //DEMO MODE
#define	VJEMODE_NOEDIT			0x02000000 //disallowed: editing, saving
//#define						0x04000000 //reserved
//#define						0x08000000 //reserved
#define	VJEMODE_SAFEMODE		0x00100000 //SAFE MODE: no fullscreen, no NET, no devices!
#define	VJEMODE_SERVERMODE		0x00200000 //Server mode: remote controller available(flag ignored during startup)
#define	VJEMODE_DUALHEAD		0x00400000 //diagnostic flag: ignored during startup
#define	VJEMODE_WINDOWED		0x00800000 //windowed(non-exclusive) (used with VJEMODE_PRESENTATION)

//Engine callback types
#define	VJE_CALLBACK_STATUS		0x0001
#define	VJE_CALLBACK_ERROR		0x0002
#define	VJE_CALLBACK_DEBUG		0x0003
#define	VJE_CALLBACK_STATE		0x0004

//state id for VJE_CALLBACK_STATE same as VJEOBJ_xxx in vjoObj.h


typedef LONG (*VJECALLBACKPROC)(LPARAM lpData, LPARAM lParam);

//messages
#define	VJEMSG_ACTIVATE_PLAYLIST_VIEW		(WM_USER+800)
#define	VJEMSG_ACTIVATE_TRANSLIST_VIEW		(WM_USER+801)
#define	VJEMSG_ACTIVATE_OVERLAYLIST_VIEW	(WM_USER+802)
#define	VJEMSG_ACTIVATE_FXCTREE_VIEW		(WM_USER+803)
#define	VJEMSG_ACTIVATE_EVENTSLIST_VIEW		(WM_USER+804)
#define	VJEMSG_DEVICE_STATE_CHANGED			(WM_USER+805)	//lParam = DeviceID

class CEffectInfo {
public:
	EFFECTUID	m_EffectUID;		//unique identifier

	DWORD		m_EffectFlags;
	HBITMAP		m_hLogoBitmap;
	DWORD		m_ParentDSN;
	CTime		m_EffectCreationTime;
	
	CString		m_EffectAuthor;
	CString		m_EffectGreetings;
	CString		m_EffectPath;
	CString		m_EffectHelpFilePath;

	CString		m_EffectName;
	CString		m_EffectFullName;

	CStringArray m_InputScreensDesc;
	CStringArray m_Resources;
	CDWordArray	 m_Links;
    DWORD		m_dwInputState;
};

typedef	struct
{
	BOOL	bBusy;			   // engine renders now!
	HWND	hPrimaryWnd;	   // same as GetPrimaryWnd()
	DWORD	dwMode;			   // same as GetModeFlags()
	DWORD	dwKernelTime;	   // kernel time
	DWORD	dwRunTime;		   // runtime
	DWORD	dwRenderSteps;	   // steps done
	DWORD	dwMemoryUsed;	   // RAM bytes in use
	SIZE	szFullScreenRes;   // FullScreen Resolution
	SIZE	szRendererRes;	   // Renderer Resolution
	DWORD	dwAspectRatio;	   // Aspect Ratio LOWORD: lowbyte=x, highbyte =y; HIWORD 0/1 center/stretch
	CHAR	pstrBuildInfo[32]; // engine build info
	CHAR	pstrVersInfo[32];  // engine version info
	CHAR	pstrPath[MAX_PATH];// rendered object file name
	DWORD	dwReserved;
} VJESTATE;

class CVJoPreviewEngine
{
public:
		// Notes:
		// 1. This class does not uses AUTOPILOT && vJoDevices.
		// 2. You can create & run any count of this objects 
		// 3. This object can be created with using  CVJoEngine::CreatePreview()
virtual			~CVJoPreviewEngine()=0;
virtual BOOL	RenderToDIB(BITMAPINFOHEADER *lpBmp, HGLOBAL *phBuf) = 0;
virtual	BOOL	LoadFXC(LPCSTR lpfxcPath)			= 0;	//load extern fxc file
virtual	BOOL	UnloadFXC()							= 0;

};

class CVJoEngine //base class
{
public:

virtual	~CVJoEngine() = 0;
		// main control
virtual	BOOL	Startup(HWND hPrimaryWnd, DWORD modeFlags)	= 0;
virtual	BOOL	Render(BOOL bQuery=FALSE)					= 0;
virtual	BOOL	Shutdown()									= 0;

	// more control & sevice
virtual BOOL	RenderToDIB(BITMAPINFOHEADER *lpBmp, HGLOBAL *phBuf, DWORD timeValue=0) = 0;
virtual LPVOID	GetRecentBitsBuffer(BITMAPINFOHEADER *lpBmp)			= 0; 
virtual CVJoPreviewEngine	*CreatePreview()		= 0;
virtual	BOOL	GetEngineState(VJESTATE *lpState)	= 0;
virtual	LPVOID  GetEngineObject(UINT uObjID)		= 0;
virtual	LONG    SendCommandString(LPCSTR cmd)		= 0;
virtual	BOOL	SetupEngineDlg()					= 0;

virtual	BOOL	Load(LPCSTR)  = 0;					//uniform loader
virtual	VOID	Unload()  = 0;						//uniform unloader
virtual	BOOL	EnableRendering(BOOL bEnb=TRUE)  = 0;//uniform enable/disable
virtual	BOOL	IsRenderingEnabled()  = 0;

virtual	INT		GetOverlayTreeCount()		 = 0;
virtual	BOOL	GetOverlayTreeInfo(int indx, CString& name, CString& path, BOOL &bAvailable, EFFECTUID& uid)	 = 0;
virtual	BOOL	GetOverlayTreeInfo2(int indx, CString& name, CString& path, CString& author, BOOL &bAvailable, EFFECTUID& uid)	 = 0;
virtual	BOOL	AddOverlayTree(EFFECTUID fxcId)	= 0;
virtual	BOOL	AddOverlayTree(EFFECTUID fxcId,int pos)	= 0; //jl
virtual	VOID	RemoveOverlayTree(int indx)		= 0;
virtual	BOOL	LoadOverlayTreeFromList(int)	= 0;
virtual	void	UnLoadOverlayTree()			 = 0;

virtual	EINFO	GetOverlayTree(INT *listPos=NULL) = 0;
virtual	BOOL	EnableOverlayRendering(BOOL bEnb=TRUE)= 0;
virtual	BOOL	IsOverlayRenderingEnabled()			 = 0;
virtual	BOOL	IsOverlayEngineEnabled()			 = 0;
virtual VOID	ReplaceOverlayTree(int indx, int newPos) = 0;
virtual VOID	AttachRenderingThread(BOOL) = 0;
//jl karma added
virtual VOID	ClearOverlayTree() = 0;
virtual bool	LoadOverlayTree(CString path) = false;
virtual bool	SaveOverlayTree(CString path) = false;
	//jl karma added
virtual void	SetKarmaVar(int indx, int variable)			= 0;
virtual int		GetKarmaVar(int indx)						= 0;
virtual void	CopyKarmaScreen(DWORD* pScreen, int screen, int width, int height)	= 0;
virtual void	GetKarmaScreen(DWORD* pScreen, int screen, int width, int height)	= 0;
virtual DWORD	GetKarmaScreenXY(int screen, int xpos, int ypos)	= 0;
virtual	CString	GetUserDirectory(int dir, int pos) = 0;
virtual	void	SetUserDirectory(int dir, int pos, CString directory) = 0;
virtual	void	PrintDebug(CString text) = 0;

virtual CString GetRootDirectory() = 0;

		HWND	GetPrimaryWnd()	 {return m_hPrimaryWnd;}
		DWORD	GetModeFlags()	 {return m_dwMode;}
		DWORD	m_dwDSN;
protected:
	HWND			m_hPrimaryWnd;
	DWORD			m_dwMode;

};


//engine dll export
extern "C" {
__declspec(dllexport)	CVJoEngine	*vjeCreateEngineInstance();
__declspec(dllexport)	void		vjeSetCallback(VJECALLBACKPROC cb, LPARAM lParam, UINT uCB);
__declspec(dllexport)	LPCSTR		vjeGetLastError();
}


#endif
#endif