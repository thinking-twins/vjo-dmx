// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001-2002 BY GREEN HIPPO 
//
// File:		vJoUI.h
// Project:		vJoUI
//
// Contents:	vJoUI dll header
//
//
//
// Author:		Vadim Gorbatenko
// Started:		02.06.03 13:21:43
//
// Version:		R4.0
#ifndef _VJOUI_DEF
#define _VJOUI_DEF

#ifdef	__cplusplus //only for C++

#define	VJOUI_VERSION	0x400

//vJoUI HostInfo ID
//        uInfoID						lData
#define	VJOUI_HI_MAIN_WND		0	// HOST window hWnd
#define	VJOUI_HI_VJENGINE		1	// CVJoEngine pointer
#define	VJOUI_HI_DRAGDROPSERVER	2	// CVJODragDropServer pointer
#define	VJOUI_HI_CALLBACKSERVER	3	// CVJOCallbackServer pointer
#define	VJOUI_HI_MINIMIZE_CPU_USAGE 4// TRUE/FALSE
#define	VJOUI_HI_MAXDEMOTIME	6	// sec
#define	VJOUI_HI_OVERLAYKEY		7	// key
#define	VJOUI_HI_MFCDOCUMENT	8	// Main CDocument
#define	VJOUI_HI_REGENTRYNAME	9	// LPCSTR name of registryEntry

//vJoUI objects
#define	VJUI_RENDERER		0	//lParam = HOST window hWnd
#define	VJUI_FXC_EDITOR		1
#define	VJUI_FXC_IACTL		2	//reserved
#define	VJUI_FXC_BROWSER	3
#define	VJUI_DFX_BROWSER	4
#define	VJUI_RES_BROWSER	5
#define	VJUI_TRANS_BROWSER	6
#define	VJUI_PREVIEW_WND	7
#define	VJUI_TREE_INSPECTOR	8
#define	VJUI_OVERLAYLIST	9
#define	VJUI_SSYSTEMVIEW	10

// misc control
#define	VJUI_TAB_WND		20
#define	VJUI_DOCKBAR		21

#define	VJUI_CDPLAYER		22
#define	VJUI_STATUSBARMODIFICATOR	23
#define	VJUI_ACCELMANG		24
#define	VJUI_SPASH			25



//engine dll export
extern "C" {
__declspec(dllexport)	LPVOID		vjuiCreateObject(UINT uObjID, LPARAM lParam);
__declspec(dllexport)	DWORD		vjuiLastError();
__declspec(dllexport)	DWORD		vjuiGetDllVersion();
__declspec(dllexport)	BOOL		vjuiSetHostInfo(UINT uInfoID, LPARAM lData);
__declspec(dllexport)	void		vjuiDoOnIdle(BOOL bUpdateCmdUI);
}

/////////////////// common objects and services /////////////////////////

//drag & drop
enum	_tag_drag_objs
{
		dragDfx,
		dragFxc,
		dragTransition,
		dragFolder,
		dragResource,
		dragList,
		dragClipboard,
		dragPLItem,
		dragExportFxc,
		dragPLCmd,
		
		dragUnknown

};

class DRAGDROPOBJ
{
public:
	DRAGDROPOBJ()	{m_lpDataName = NULL; m_lData = 0; m_lpNextElement = NULL;}
	DRAGDROPOBJ(DWORD dw, LONG ld, LPCSTR lp)
				{m_dwID = dw; m_lData = ld; 
				m_lpDataName =lp; 
				m_lpNextElement = NULL;}
	~DRAGDROPOBJ() { delete m_lpNextElement; }

	virtual	LPVOID	DoDrop(LPVOID lpDest)	{return NULL;}
	DRAGDROPOBJ *Add(DWORD dw, LONG ld, LPCSTR lp)
			{m_lpNextElement = new DRAGDROPOBJ(dw, ld, lp); return m_lpNextElement;}
	
	DWORD	m_dwID;
	LONG	m_lData;
	LPCSTR	m_lpDataName;
	DRAGDROPOBJ	*m_lpNextElement;
};

typedef VOID (*VJUIDRAGOBJECT)(DRAGDROPOBJ *pObj);

// base virtual class for client part
class	CDropTarget
{
public:
	virtual	~CDropTarget()	{}

virtual	BOOL	QueryDrop(DRAGDROPOBJ&, CPoint pt/*screen coord*/, BOOL bMove = FALSE) = 0;
virtual	BOOL	Drop(DRAGDROPOBJ&, CPoint pt/*screen coord*/, BOOL bMove = FALSE) = 0;
virtual	HCURSOR	GetDROPCursor( DRAGDROPOBJ&, BOOL bMove = FALSE ) = 0;
virtual	void	OnEndDrag() {}
public:
	//member
	CWnd	*m_DropWnd;
	CRect	m_wndRect;//for caching
protected:
	
	CDropTarget()	{m_DropWnd = NULL;}
};

class	CVJODragDropServer
{
public:
	CVJODragDropServer()	{}
	virtual ~CVJODragDropServer()	{}
	virtual	void RegisterDropTarget(CDropTarget *) = 0;
	virtual	void UnRegisterDropTarget(CDropTarget *) = 0;
	virtual	BOOL DragObject(DRAGDROPOBJ &pObj) = 0;
};

class	CVJOCallbackServer
{
public:

	enum
	{	//uActionMsg			//lparam				lpResult
		VJOUI_SAVE_MODIFIED,	//not used				0/1
		VJOUI_PRINT_DBGSTRING,	//LPCSTR				not used
		VJOUI_LOCK_RENDERER,	//TRUE/FALSE 			not used
								//(if CVJORenderer 
								//object not exists)
		VJOUI_ENGINE_STATE,		//engine obj			not used
		VJOUI_ENGINE_ERROR,		//LPCSTR				not used
		VJOUI_ENGINE_STATUS,	//LPCSTR				not used
	};
	CVJOCallbackServer()	{}
	virtual	~CVJOCallbackServer()	{}
	virtual void CallHost(UINT uActionMsg, LPARAM lParam, LONG* lpResult) = 0;
};
//////////////////////////////////////////////////////

//notification messages: VJOUI sents to host window
enum	{
		VJOUI_NOTIFY_BASE =	(WM_USER+700),

		//FXC editor (CFXCEditor)
		VJOUI_TREE_QUERY_CHANGING,	//before changing, lpParam =&bResult, host returns 0 for continue
		VJOUI_TREE_CHANGED,			//after changing
		VJOUI_NODE_QUERY_CHANGING,	//before changing(setup), lpParam =&bResult, host returns 0 for continue
		VJOUI_NODE_CHANGED,			//after changing
		VJOUI_CUR_SLOT_CHANGED,		//select slot
		VJOUI_STATUS_STRING,		//lParam = LPCSTR
		
		};



class	CFXCEditor : public CWnd
{
public:
	virtual ~CFXCEditor() = 0;
	
	enum //flags for CreateEditor
	{
		flag_no_clipboard = 0x00000001, // default: with clipboard
		flag_storyboard = 0x00000002,
		flag_no_tooltip = 0x00000004,	// default: with tooltip
		flag_notify		= 0x00000008	// sends VJOUI_TREE_CHANGED, VJOUI_NODE_CHANGED 
	};

	virtual	BOOL CreateEditor(UINT flags, HWND hParentWnd) = 0;
	virtual void UpdateState() = 0;

	enum //states for SetState
	{
		state_collaps,
		state_zoom, //toggle
		state_show_clipboard,
		state_show_storybox,
		state_show_nodes_statistics,
		state_halt,
		state_activate,
		state_minimize_cpu_usage,
		state_modified,	//only "Get"
		state_autorun
	};

	virtual	BOOL	SetState(UINT uState, BOOL b) = 0;
	virtual	BOOL	QueryState(UINT uState, BOOL b) = 0; //allowed to change?
	virtual	BOOL	GetState(UINT uState) = 0;
	
	enum //commands	for ExecCommand
	{
		cmd_new,
		cmd_save,
		cmd_save_as,
		cmd_save_modified,//usually not for UI, just fo CDocument
		cmd_close,
		cmd_copy,
		cmd_cut,
		cmd_delete,
		cmd_select_all,
		cmd_paste,
		cmd_undo,

		cmd_fxc_setup,
		cmd_effect_setup,
		cmd_effect_help,
		cmd_effect_setup_vars
	};
	
	virtual	BOOL	ExecCommand(UINT uCommand) = 0;
	virtual	BOOL	QueryCommand(UINT uCommand) = 0;

	virtual	LPCSTR	GetCurItemName() = 0;
	virtual	HEFFECT	GetCurItemHandle() = 0;
	
	enum	//metrics for GetMetrics
	{
		metrics_icon_cx,
		metrics_icon_cy,

	};
	virtual	LONG	GetMetrics(UINT uMetricsID) = 0;
	virtual	BOOL	SetMetrics(UINT uMetricsID, LONG iValue) = 0;
	
	virtual	void	SetStreamCallback(VJENODECALLBACKPROC pProc, LPARAM lParam) = 0;

};

class	CFXCGalleryBrowser : public CWnd
{
public:
	virtual ~CFXCGalleryBrowser() {}
	
	virtual BOOL	CreateBrowser (UINT flags, HWND hParentWnd) = 0;
	virtual void	UpdateState() = 0;
	
	
	enum //commands	for ExecCommand
	{
		
		cmd_copy,
		cmd_cut,
		cmd_delete,
		cmd_select_all,
		cmd_paste,
		

		cmd_effect_property,
		cmd_forward,
		cmd_back,
		cmd_newfolder,
		cmd_insertfolder,
		cmd_export,
		cmd_import,
		cmd_find,
		cmd_find_next
	};
	
	virtual	BOOL	ExecCommand(UINT uCommand) = 0;
	virtual	BOOL	QueryCommand(UINT uCommand) = 0;

	enum //states for SetState
	{
		state_nc_mode,
		state_ie_mode,
		
		state_halt,
		state_activate,
		state_minimize_cpu_usage,
		state_prompt_for_delete,
		state_safe_delete
	};

	virtual	BOOL	SetState(UINT uState, BOOL b) = 0;
	virtual	BOOL	QueryState(UINT uState, BOOL b) = 0; //allowed to change?
	virtual	BOOL	GetState(UINT uState) = 0;

};


class	CDFXGalleryBrowser : public CWnd
{
public:
	virtual ~CDFXGalleryBrowser() {}
	
	virtual BOOL	CreateBrowser (UINT flags, HWND hParentWnd) = 0;
	virtual void	UpdateState() = 0;
	
	
	enum //commands	for ExecCommand
	{
		
		cmd_cut,
		cmd_delete,
		cmd_paste,
		

		cmd_effect_property,
		cmd_forward,
		cmd_back,
		cmd_newfolder,
		cmd_find,
		cmd_find_next
	};
	
	virtual	BOOL	ExecCommand(UINT uCommand) = 0;
	virtual	BOOL	QueryCommand(UINT uCommand) = 0;

	enum //states for SetState
	{
		state_std_mode,
		state_ie_mode,
		state_ie_sing_expand,

		state_halt,
		state_activate,
		state_minimize_cpu_usage
	};

	virtual	BOOL	SetState(UINT uState, BOOL b) = 0;
	virtual	BOOL	QueryState(UINT uState, BOOL b) = 0; //allowed to change?
	virtual	BOOL	GetState(UINT uState) = 0;

};

class	CResourceBrowser : public CWnd
{
public:
	virtual ~CResourceBrowser() {}
	
	virtual BOOL	CreateBrowser (UINT flags, HWND hParentWnd) = 0;
	virtual void	UpdateState() = 0;
	
	
	enum //commands	for ExecCommand
	{
		cmd_forward,
		cmd_back,
	};
	
	virtual	BOOL	ExecCommand(UINT uCommand) = 0;
	virtual	BOOL	QueryCommand(UINT uCommand) = 0;


};


class	CVJOPreviewWindow : public CWnd
{
public:
	CVJOPreviewWindow() {}
	virtual ~CVJOPreviewWindow() {}

	virtual BOOL	CreatePreview(const RECT& rect, HWND pParentWnd)  = 0;
	virtual void	Enable(BOOL ) = 0;
	virtual	HWND	GetPreviewWndHandle() = 0;

};

#define	VJOUI_SETFGROUND_PANE	(WM_USER+0x1953)	//lParam = HWND of pane

class	CVJOUITabControl : public CWnd
{
public:
	CVJOUITabControl () {}
	virtual ~CVJOUITabControl() {}
	virtual	VOID	SetSlashMode(BOOL) =0;
	virtual	BOOL	GetSlashMode() =0;
	virtual BOOL	AddTab(LPCSTR title, DWORD lParam, BOOL bActivate = FALSE) = 0;
	virtual BOOL	RemoveTab(DWORD lParam) = 0;
	virtual BOOL	ActivateTab(DWORD lParam) = 0;
	virtual BOOL	IsTabEnabled(DWORD lParam) = 0;
	virtual BOOL	EnableTab(DWORD lParam, BOOL bEnb) = 0;
	virtual DWORD	GetActiveTab() = 0;
	virtual int		GetTabsCount() = 0;
	virtual DWORD	GetTabId(int p) = 0;
	virtual DWORD	GetTabId(char *p) = 0;
	virtual LPCSTR	GetActiveTabName() = 0;

	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, BOOL bVis=TRUE) = 0;


};

#define	DBV_HASGRIPPER	0x00001


class CVJOUIDockBarView : public CControlBar
{
	DECLARE_DYNAMIC(CVJOUIDockBarView);
public:
	CVJOUIDockBarView () {}
	virtual ~CVJOUIDockBarView() {}
    virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
        CSize sizeDefault, DWORD styleEx, UINT nID,
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP) = 0;
	
	virtual void	ShowCaption(BOOL b) = 0;
	virtual BOOL	IsCaptionVisible() = 0;

	virtual	BOOL	AddView(CRuntimeClass* pViewClass, LPCSTR lpTitle, CCreateContext* pContext) = 0;
	virtual	BOOL	AddView(CWnd *view, LPCSTR lpTitle) = 0;
	virtual	BOOL	DetachView(CWnd *view, CWnd *newParent) =0;
	virtual	void	SetMinSize( CSize sz) = 0;

	virtual	INT		GetViewsCount() = 0;
	virtual	CWnd	*GetActiveView() = 0;
	virtual	INT		GetActiveViewID() = 0;
	virtual	CWnd	*GetView(int p) = 0;
	virtual	CWnd	*GetView(char * p) = 0;
	virtual	VOID	SetActiveView(CWnd *) = 0;
	virtual	LPCSTR	GetActiveTabName() = 0;
	virtual	VOID	EnableView(CWnd *p, BOOL b) = 0;
	virtual	BOOL	IsViewEnabled(CWnd *p) = 0;

	virtual void LoadState(LPCTSTR lpszProfileName) = 0;
    virtual void SaveState(LPCTSTR lpszProfileName) = 0;


};

class	CTreeInspector : public CWnd
{
public:
	CTreeInspector()	{}
	virtual	~CTreeInspector()	{}
	enum
	{
		flag_no_local_output= 0x0001,
		flag_no_resources	= 0x0002,
		flag_no_variables	= 0x0004,
	};
	
	virtual BOOL	CreateInspector (UINT flags, HWND hParentWnd) = 0;
};

class	COverlayList : public CWnd
{
public:
	COverlayList()	{}
	virtual	~COverlayList()	{}
	
	virtual BOOL	CreateList (UINT flags/*reserved, must be 0*/, HWND hParentWnd) = 0;
	
	enum //commands	for ExecCommand
	{
		
		cmd_delete,
		cmd_move_down,
		cmd_move_top
	};
	
	virtual	BOOL	ExecCommand(UINT uCommand) = 0;
	virtual	BOOL	QueryCommand(UINT uCommand) = 0;
	virtual	BOOL	AddItem(EINFO id) = 0;

};

class	CSSysView : public CWnd
{
public:
	CSSysView() {}
	virtual ~CSSysView() {}
	enum
	{
		flag_popup =0x0001 //default: child window
	};
	
	virtual BOOL	CreateView(UINT flags, HWND hParentWnd) = 0;
	enum
	{
		cmd_zoom_in,
		cmd_zoom_out
	};

	virtual	BOOL	ExecCommand(UINT uCommand) = 0;
	virtual	BOOL	QueryCommand(UINT uCommand) = 0;
};

class	CCDPlayer : public CWnd
{
public:
	CCDPlayer() {}
	virtual ~CCDPlayer() {}
	virtual BOOL	CreatePlayer(HWND hParent, RECT *pRect) = 0;
	enum
	{
	state_show_erros,
	state_stop_on_exit,
	};
	virtual	BOOL	SetState(UINT uState, BOOL bValue) = 0;
	virtual	BOOL	GetState(UINT uState) = 0;

};

class	CStatusBarModificator
{
public:
	CStatusBarModificator() {}
	virtual ~CStatusBarModificator() {}
	enum
	{
		pane_progress_bar		= 0x00000001,
		pane_dev_manager_bar	= 0x00000002,
		pane_sound_level_meter	= 0x00000004,
		pane_engine_mode		= 0x00000008,
		pane_window_resolution  = 0x00000010,
		pane_engine_resolution	= 0x00000020,
		pane_engine_memory		= 0x00000040,
		pane_engine_runtime		= 0x00000080,
		pane_system_time		= 0x00000100
	};
	enum
	{
		pane_all_vjo_panes = 0xffff
	};
	virtual BOOL	CreateModificator(CStatusBar& pBar, DWORD flagsPanes) = 0;
	virtual VOID	UpdateDeviceIcon(DWORD dwDeviceID)=0;
	virtual	VOID	ShowProgress(int percent, LPCSTR title)=0;

};

class CAccelManager
{
public:
	CAccelManager() {}
	virtual ~CAccelManager()  {}
	virtual BOOL	Init(CFrameWnd *pMainFrame, LPCSTR pRegSection) = 0;
	virtual VOID	Customize(CFrameWnd *pMainFrame) = 0;
	virtual BOOL	FindAccel(UINT uCmd, UINT& uKey) = 0;
};


class CVJoEngine;
#define	DEFAULT_FPS	0xffff
#define	DEFAULT_KEY	0
class	CVJORenderer
{
public:
	CVJORenderer() {}
	virtual ~CVJORenderer() {}
	//thread model
	enum
	{
		tm_UseSingleThread,
		tm_UseCurrentThread, //starts engine within current thread. render within separate thread
		tm_UseSeparateThread//starts and run engine within separate thread
	};
	//startup
	enum
	{
		start_printdebug  = 0x0001,  //default: no debug window
		start_showdebug   = 0x0002,  //default: debug window is hidden
		start_fullScreen  = 0x0004, //default: startup in editmode
		start_renderingOn = 0x0008 //default: off
	};

	virtual BOOL	Startup(UINT threadModel, BOOL bDemoMode, UINT flags=0) = 0;
	virtual BOOL	Reset(UINT threadModel) = 0;
	virtual	void	SetFPS(UINT fps) =0; //for current mode
	virtual void	ShutDown() = 0;
	virtual void	SetLogFile(LPCSTR pFile) = 0; //can be called before startup
	virtual BOOL	ShowDialog(UINT flags) =0;
	virtual CVJoEngine *GetEngine() =0;

	virtual DWORD	GetRenderingThreadID() = 0;
	virtual DWORD	GetStartupThreadID() = 0;
	virtual BOOL	IsDualHeadAvailable() = 0;
	virtual	void	GetObjectTitle(CString& title)=0;
	//for UI button
	virtual BOOL	Run( BOOL b, BOOL bQuery ) =0;
	virtual BOOL	IsRunning() =0;
	virtual	BOOL	PushRunningState(BOOL bDisable=TRUE)=0;
	virtual	void	PopRunningState()=0;
	
	//engine
	enum
	{
		run_edit,
		run_present_wnd,
		run_present_fs
	};

	virtual	INT		GetRunMode() = 0;
	virtual BOOL	SetEditMode(UINT fps=DEFAULT_FPS, HWND hTarget = NULL) = 0;
	virtual BOOL	SetPresentationWindowed(UINT keyForExit=DEFAULT_KEY, UINT fps=DEFAULT_FPS, HWND hTarget = NULL) = 0;
	virtual BOOL	SetPresentationFullScreen(UINT keyForExit=DEFAULT_KEY, UINT fps=DEFAULT_FPS) = 0;
	virtual	BOOL	IsModeCompleted(BOOL bWait, BOOL bPumpMessages) = 0; //for multithreading

	//debug output window
	virtual	CWnd	*GetDebugWindow()=0;
};

class CVJOSplashScreen : public CWnd
{
public:
	CVJOSplashScreen() {}
	virtual ~CVJOSplashScreen() {}
	virtual BOOL CreateSplashScreen(HBITMAP h, CWnd* pParentWnd = NULL) = 0;
	virtual BOOL PreTranslateAppMessage(MSG* pMsg) = 0;
	virtual void Hide() = 0;
	virtual void PrintStateString(LPCSTR, LPCRECT rc=NULL) = 0;

};

#endif
#endif

