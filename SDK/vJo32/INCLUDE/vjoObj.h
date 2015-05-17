// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		vjoObj.h
// Project:		vje32
//
// Contents:	VJO public objects interfaces base classes for
//				Autopilot
//				Mixer
//
// Author:		Vadim Gorbatenko
// Started:		06.06.00 14:05:41
//
// Version:		3.50



/************************************
  REVISION LOG ENTRY
  Revision By: VG
  Revised on 12.10.01 15:38:04 : v2.50
  Revised on 12.11.01 12:19:53 : v3.00
  Revised on 13.06.02 0:48:17  : v3.30
  Comments: ...
 ************************************/
#ifndef _VJOOBJ_DEF
#define _VJOOBJ_DEF

#define	VJEOBJ_KERNEL			0x0000
#define	VJEOBJ_MONITOR			0x0001
#define	VJEOBJ_AUTOPILOT		0x0002
#define	VJEOBJ_MIXER			0x0003
#define	VJEOBJ_DEVMANG			0x0004
#define	VJEOBJ_GALLERY			0x0005
#define	VJEOBJ_TREEEDITOR		0x0006
#define	VJEOBJ_AUTOPILOTEDITOR	0x0007
#define	VJEOBJ_TREE				0x0008
#define	VJEOBJ_MINISOCKET		0x0009
#define	VJEOBJ_OVERLAY			0x000B
#define	VJEOBJ_DDDISPATCHER		0x000D

	//not an objects! just functions
#define	VJEOBJ_CALLBACK_STATUS	0x1001	
#define	VJEOBJ_CALLBACK_ERROR	0x1002
#define	VJEOBJ_CALLBACK_DEBUG	0x1003
	//not an objects! just values
#define	VJEOBJ_CURRENT_TIME		0x2001	
#define	VJEOBJ_RENDERING_TIME	0x2002	
#define	VJEOBJ_RENDERING_STEP	0x2003	
#define	VJEOBJ_CURRENTVERSION	0x2004	

	//commands
#define	VJEOBJ_LOCK				0x3001	
#define	VJEOBJ_UNLOCK			0x3002

#ifndef _VJOEAPI_DEF
typedef	LPVOID		HEFFECT;
typedef DWORD		EINFO;
#endif

class VJOEVENT;
typedef LPVOID	HPLITEM;

class CIAutoPilot
{
public:
virtual	~CIAutoPilot()	= 0;

virtual	BOOL	ProcessEvent(VJOEVENT* )	= 0;
virtual	BOOL	IsUploaded()		= 0;
virtual	BOOL	IsRunning()			= 0;
virtual	BOOL	Run(BOOL)			= 0;
virtual	HPLITEM	GetCurrentItem()	= 0;
virtual	LPCSTR	GetPlayListPath()	= 0;
virtual	BOOL	IsTimeLine(LPCSTR lpPath)	= 0;
virtual	BOOL	LoadList(byte,LPCSTR)	= 0;
virtual	BOOL	UnloadList()		= 0;
virtual	DWORD	GetRunTime()		= 0;
//v3.3
virtual	HPLITEM	SetCurrentItem(HPLITEM)	= 0;
//v3.5
virtual	void	EmptyCache()	= 0;
virtual BOOL	ResetItem(HPLITEM) = 0;
};

// returned by	CIAutoPilotEditor:GetItemInfo
#define PL_KIND_UNKNOWN 0
#define PL_KIND_FXC		1
#define PL_KIND_FOLDER	2
#define PL_KIND_SUBLIST 3
#define PL_KIND_TIMLINE	0x100 //flag
#define PL_KIND_LINK	0x200 //flag

//v2.5
#define	PL_KIND_COMMENT		4
#define	PL_KIND_CMD			5
#define	PL_KIND_OVERLAY		6
#define	PL_KIND_JUMP		7

//v3.50
typedef UINT	HPLCOMMAND;

typedef LONG (*VJEEVENTCALLBACKPROC)(VJOEVENT *pEvent, LPARAM lParam);

class CIAutoPilotEditor
{
public:
virtual	~CIAutoPilotEditor()	= 0;

virtual LPCSTR	fullKey(LPCSTR section)=0; //jl
virtual LPCSTR	ReadGalleryRegString(LPCSTR section, LPCSTR key)=0; //jl

virtual BOOL	CreateList(BOOL bAsTimeLine = FALSE)	 = 0;
virtual BOOL	SaveList(LPCSTR lpPath = FALSE, BOOL bKeepLinks=TRUE) = 0;
virtual BOOL	IsModified() = 0;

virtual HPLITEM	InsertItem(EFFECTUID pUID, HPLITEM hAfter)	 = 0;
virtual HPLITEM	InsertItem(LPCSTR lpPath, HPLITEM pAfter, LPCSTR lpTitle=NULL, BOOL bAsLink=TRUE)	= 0;
virtual BOOL	DeleteItem(HPLITEM hPos)			 = 0;
virtual BOOL	MoveItem(HPLITEM hPos, HPLITEM hAfter)	 = 0;

virtual EINFO	GetItem(HPLITEM)	= 0;
virtual HPLITEM	GetItem(int iPos)	= 0;	//relative to current sub-root
virtual HPLITEM	GetRootItem()		= 0;
virtual HPLITEM	GetParentItem(HPLITEM)	= 0;
virtual HPLITEM	GetNextItem(HPLITEM)	= 0;
virtual HPLITEM	GetChildItem(HPLITEM)	= 0;
virtual	UINT	GetItemInfo(HPLITEM, BOOL* bValid, LPCSTR *ppPath, LPCSTR *ppTitle, DWORD* dwLifeTime, DWORD* dwMinLifeTime, DWORD *dwRelIndex)  = 0;
virtual	UINT	GetItemInfo(HPLITEM, BOOL* bValid, LPCSTR *ppPath, LPCSTR *ppTitle, DWORD* dwLifeTime, DWORD* dwMinLifeTime, DWORD *dwRelIndex, LPCSTR *ppAuthor)  = 0; // jl
virtual BOOL	SetItemLifeTime(HPLITEM, DWORD dwLifeTime)	 = 0;

virtual DWORD	GetDeadTimeValue()			    = 0;
virtual VOID	SetDeadTimeValue(DWORD dwTime)	= 0;
//v2.5
virtual HPLITEM	InsertItemDlg(HPLITEM hAfter, UINT kind)=0;//
virtual	BOOL	Configure(HPLITEM, BOOL bQuery)	= 0;
virtual	BOOL	Copy(HPLITEM hPos, BOOL bQuery)		= 0;
virtual	BOOL	Cut(HPLITEM hPos, BOOL bQuery)		= 0;
virtual	HPLITEM	Paste(HPLITEM pAfter, BOOL bQuery)	= 0;
virtual	HPLITEM	DupItem(HPLITEM hPos, HPLITEM hAfter) = 0;//offclipboard
virtual BOOL	SetItemKind(HPLITEM, UINT kind)		 = 0;//assign overlay/not overlay

virtual	BOOL	MarkItem(HPLITEM, BOOL)	= 0;
virtual	UINT	GetItemMark(HPLITEM) = 0;
virtual	BOOL	EnableItem(HPLITEM, BOOL)	= 0;
virtual	BOOL	IsItemEnabled(HPLITEM) = 0;
virtual	int		GetMarksCount()	= 0;
virtual	BOOL	RearrangeMarks()	= 0;

//v3.5
virtual INT		GetCommandsCount() = 0;
virtual HBITMAP	GetCommandsBitmap() = 0;
virtual	BOOL	GetCommandInfo(int indx, HPLCOMMAND &uCommand, CString& name, CString& desc, int& bitmap) = 0;
virtual HPLITEM	InsertCommand(HPLITEM hAfter, HPLCOMMAND cmd, CString& roots)=0;//
virtual BOOL	InstallEventCallback(VJEEVENTCALLBACKPROC pFn, LONG lParam) = 0;
};

class CIGallery//both for fxc & dfx galleries
{
public:
virtual ~CIGallery()	= 0;

virtual	INT		GetDFXGallerySize()	= 0;
virtual	INT		GetFXCGallerySize()	= 0;
virtual	EINFO	GetDFXId(int index)	= 0;
virtual	EINFO	GetFXCId(int index)	= 0;
virtual	CEffectInfo	*GetEffectInfo(LPCSTR path)  = 0;
virtual	CEffectInfo *GetEffectInfo(EINFO id)	 = 0;
virtual	CEffectInfo *GetEffectInfo2(EINFO id)	 = 0;//jl
//virtual	BOOL	SetKarmaEffectInfo(LPCSTR path, int KIgroup, int KIrating, int KIspeed, int KItype)  = 0;//jl
virtual	BOOL	SetKarmaEffectInfo(EINFO id, int KIgroup, int KIrating, int KIspeed, int KItype)  = 0;//jl
virtual	EINFO	FindEffect(EFFECTUID)	= 0;

virtual	BOOL	 UnregisterEffect(EINFO id)		 = 0;
virtual	EINFO	 RegisterEffect(LPCSTR lpPath)	 = 0;

virtual	CStringArray*	GetFXCGalleryPathes()	 = 0;
virtual INT		GetFXCGalleryGroup()						= 0;
virtual	BOOL	SetFXCGalleryPathes(CStringArray& pathes)	 = 0;
virtual	BOOL	SelectCurrentFXCGalleryPath(int)			 = 0;
virtual	LPCSTR	GetCurrentFXCGalleryPath()					 = 0;
virtual	LPCSTR	GetDFXGalleryPath()							 = 0;
virtual	BOOL	SetDFXGalleryPath(LPCSTR lpPath)			 = 0;
//v2.3
virtual	BOOL	GetResourcePathes(CStringArray& pathes)		 = 0;
virtual	BOOL	SetResourcePathes(CStringArray& pathes)		 = 0;
virtual	BOOL	RenameEffectResources(LPCSTR lpFxc, LPCSTR lpTargetDir)  = 0;
//v3.50
virtual	BOOL	RenameEffectResources(LPCSTR lpFxc, CStringArray& pathes, CStringArray& lpTargetDir)  = 0;
};

class CITree
{
public:
virtual ~CITree() {}
virtual	BOOL	IsRunning()					 = 0;
virtual	BOOL	IsUploaded()				 = 0;
virtual	BOOL	Run(BOOL bOn=TRUE)			 = 0;
virtual	BOOL	Load(LPCSTR lpfxcPath)		 = 0; //load extern fxc file
virtual	BOOL	Load(EINFO fxcId)			 = 0; //load fxc from gallery
virtual	BOOL	Unload()					 = 0;
virtual	DWORD	GetRunTime()				 = 0;
virtual	CEffectInfo *GetTreeInfo(EINFO *ppId=NULL)  = 0;
//v3.5
virtual	void	Reset()						= 0;
virtual	int		CacheIt()					= 0;
virtual	int		GetCacheSize()				= 0;
virtual	void	DeleteFromCache(int)		= 0;
virtual	BOOL	LoadFromCache(int)			= 0;
virtual	int		IsCached()					= 0;
protected:
	CITree() {}
};

#define	IMIX_MODE_ENABLED		0x0001
#define	IMIX_MODE_MAXFPS		0x0002
#define	IMIX_MODE_RCHOICE		0x0004
#define	IMIX_MODE_MANUAL		0x0008

typedef LPVOID	HTRANSITION;

class CTransition;	//defined in vjotfx.h

class CIMixer
{
public:
virtual ~CIMixer()	= 0;

virtual	DWORD	GetMode()		 = 0;
virtual	BOOL	SetMode(DWORD)	 = 0;

virtual INT				GetAvalableTransitionCount() = 0;
virtual HTRANSITION		*GetAvalableTransitions()	 = 0;

virtual BOOL	GetTransitionsPreview(HTRANSITION hTrans, 
									  SIZE	sz,
									  LPBYTE lpTarget,
									  LPBYTE lpSource1,
									  LPBYTE lpSource2, DWORD dwRelTime)  = 0;

virtual BOOL		InsertTransListItem(HTRANSITION hTrans, int iAt) = 0;
virtual BOOL		InsertTransListItem(DWORD dwUID, int iAt)		 = 0;
virtual BOOL		DeleteTransListItem(int iPos)					 = 0;
virtual BOOL		MoveTransListItem(int iOldPos, int iNewPos)		 = 0;
virtual INT			GetTransListPosition()			 = 0;
virtual BOOL		SetTransListPosition(int iPos)	 = 0;
virtual INT			GetTransListSize()				 = 0;
virtual HTRANSITION	GetTransListItem(int pos)		 = 0;
virtual	BOOL		GetTransitionState(HTRANSITION hTrans,			//both for "in list" &
									  LPCSTR *lpName,				//for available
									  DWORD *dwDuration,
									  DWORD *dwUID)	 = 0;
virtual	BOOL		ConfigureTransition(HTRANSITION hTrans)			//for "in list only"
													= 0;
virtual	void		Reset()	{}
virtual	LPVOID		GetConfigBuffer(int &cb)		= 0;
virtual	void		SetConfigBuffer(LPVOID, int cb)	= 0;
//v3.5
virtual	BOOL		SetTransitionDuration(HTRANSITION hTrans, DWORD dwDuration) = 0;
virtual	HTRANSITION	RegisterExternTransition(CTransition *p) = 0;

};

class CVJODeviceDriver;

class CIDeviceManager
{
public:
virtual ~CIDeviceManager()	= 0;
virtual INT		GetDevListSize()			 = 0;
virtual INT		GetInstalledDevListSize()	 = 0;
virtual BOOL	GetDevListItemState(int pos, LPCSTR *lpName, BOOL *installed, DWORD *devflag, DWORD *devID, CBitmap **lable) = 0;
virtual CVJODeviceDriver	*GetDevice(int pos)	= 0;
virtual BOOL	ConfigureDevice(int pos)	 = 0;
virtual	INT		GetEventListSize()			 = 0;
virtual	BOOL	GetEventListItemState(int pos, DWORD *pdwID, DWORD *pdwSourceID, DWORD *pdwFlags, LONG *lParam, LPCSTR *lpEventName, LPCSTR *lpActionName = NULL)  = 0;
virtual	BOOL	SetEventListItemState(int pos, DWORD dwFlags, LONG lParam)  = 0;
virtual	BOOL	MoveEventListItem(int iOldPos, int iNewPos)  = 0;
virtual	BOOL	ConfigureEvent(int pos) = 0;
virtual	INT		GetRecentEvent()		= 0;
virtual	LPVOID	GetConfigBuffer(int &cb, BOOL bAll=FALSE/*ie events only!*/) = 0;
virtual	void	SetConfigBuffer(LPVOID, int cb)	= 0;
//v2.5
virtual	BOOL	ConfigureExternEventDlg(VJOEVENT *)   = 0;
//v3.3
virtual	LPCSTR	DescribeExternEvent(VJOEVENT *)   = 0;

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


typedef VOID (*VJENODECALLBACKPROC)(LPVOID lpScreen, SIZE sz, LPARAM lParam);

#define	VAR_SETUP_ALL	-1

//v3.5
#define	NODE_STATE_ENABLED		0x00000001
#define	NODE_STATE_HASBPARAM	0x00000002 //node has attached with BOOL IA stream
#define	NODE_STATE_HASVPARAM	0x00000004//node has attached with FLOAT IA stream
//
class	CITreeEditor
{
public:

virtual	~CITreeEditor()	= 0;

virtual BOOL	GetNodeBypass(HEFFECT hNode) = 0;
virtual void	SetNodeBypass(HEFFECT hNode, BOOL Bypass) = 0;

virtual	BOOL	Select(HEFFECT root, HEFFECT end) = 0;
virtual	BOOL	UnSelect(BOOL bQuery = FALSE)	  = 0;
virtual	BOOL	IsSelected(HEFFECT effect)		  = 0;

//Clipboard & selection
virtual BOOL	SetCurClipboard(int iClipboard)	  = 0;
virtual INT		GetCurClipboard()			 = 0;
virtual INT		GetMaxClipboards()			 = 0;
virtual BOOL	ClearClipboard()			 = 0;

virtual	BOOL	Copy(BOOL bQuery = FALSE)	 = 0;
virtual	BOOL	Cut(BOOL bQuery = FALSE)	 = 0;
virtual	BOOL	Delete( BOOL bQuery = FALSE) = 0;

virtual	BOOL	Paste(HEFFECT parent, int InputScreen, BOOL bQuery = FALSE)	 = 0;

virtual	BOOL	Undo(BOOL bQuery = FALSE) = 0;
virtual	void	ClearUndo()	= 0;

//offclipboard operations
virtual	BOOL	Swap(HEFFECT e1, HEFFECT e2, BOOL bNodeOnly, BOOL bQuery = FALSE)	  = 0;
virtual	BOOL	Move(HEFFECT oldp, HEFFECT newp, BOOL bNodeOnly, BOOL bQuery = FALSE) = 0;

virtual	BOOL	Insert(EINFO, HEFFECT hParent, int InputScreen, BOOL bQuery=FALSE)	 = 0;
virtual	BOOL	Insert(HEFFECT, HEFFECT hParent, int InputScreen, BOOL bQuery=FALSE) = 0;

virtual	HEFFECT	GetChild(HEFFECT hParent, int InputScreen)	   = 0;

virtual	BOOL	 IsModified()						= 0;
virtual	BOOL	 CreateFXC(CEffectInfo *lpDefault)	= 0;	//create new fxc
virtual	BOOL	 Save()								= 0;
virtual	BOOL	 SaveAs(LPCSTR lpName)				= 0;
virtual	BOOL	 SaveAs(LPCSTR lpName,int groupColour) = 0;
//virtual	BOOL	 SaveAs2(LPCSTR lpPath, LPCSTR lpName,int groupColour) = 0;
virtual	BOOL	 SaveAs2(LPCSTR lpPath, LPCSTR lpName,int groupColour, int groupRating) = 0;
virtual	BOOL	 LoadResource(HEFFECT  hPtr, LPCSTR lpPath, BOOL bQuery=FALSE)	= 0;
virtual	BOOL	 SetupNodeDlg(HEFFECT  hPtr)		= 0;
virtual	BOOL	 SetupFxc()							= 0;
virtual	BOOL	 SetupPostit()						= 0;
virtual	CEffectInfo	 *GetNodeInfo(HEFFECT  hPtr)	= 0;

//v2.30 extension
virtual	BOOL	IsNodeCollapsed(HEFFECT hNode)		 = 0;
virtual	void	CollapseNode(HEFFECT hNode, BOOL b) = 0;
//v2.50 extension
virtual	BOOL	SetStreamCallback(HEFFECT hNode, VJENODECALLBACKPROC pFnc, LPARAM lParam)	 = 0;
virtual	BOOL	Connect(HEFFECT hDestOutput, HEFFECT hNode, int input, BOOL bQuery) = 0;
virtual HEFFECT	GetConnectionSrcPoint(HEFFECT hNode) = 0;

//v3.1  extension
virtual	INT		GetNumVariables(HEFFECT hNode ) = 0;
virtual	BOOL	GetVariable(HEFFECT hNode, int indx, LPCSTR *lpName, BOOL *bBool) = 0;
virtual	BOOL	SetupVariable(HEFFECT hNode, int indx) = 0;
//v3.5  extension
virtual	DWORD	GetNodeCPUUsage(HEFFECT  hPtr)	= 0;
virtual	DWORD	GetNodeState(HEFFECT  hPtr)	= 0;
virtual	BOOL	InsertLink(EINFO, HEFFECT hParent, int InputScreen, BOOL bQuery=FALSE) = 0;
//v3.5G  extension
virtual	INT	GetNodeConfig(HEFFECT  hPtr, LPBYTE buf)	= 0;
virtual	BOOL SetNodeConfig(HEFFECT  hPtr, LPBYTE buf, int nBytes)	= 0;
virtual	BOOL GetVariableEx(HEFFECT hNode, int indx, LPCSTR *lpName, BOOL *bBool, int *nCtlParams, BOOL *bFunc) = 0;
virtual BOOL CrackVariable(HEFFECT hNode, int indx, int nCtlParams, PARAMID newParam) =0;
virtual PARAMID GetCtlParam(HEFFECT hNode, int indx, int nCtlParam) = 0;

};

#ifndef _VJMS_DEF
#define	VJMS_PACKET_SIZE	256
#define	VJMS_MAXPORTS		256
#define	VJMS_MAXLISTENER	64
typedef LPVOID HPORT;
typedef VOID  (* VJMSLISTENER)(HPORT hPort, LPARAM );

#define	VJMSPORTREAD			0x1
#define	VJMSPORTWRITE			0x2
#define	VJMSPORTEXCLUSIVEREAD	0x4
#define	VJMSPORTEXCLUSIVEWRITE	0x8
#define	VJMSDISBLNOLIST			0x10 //disable if no listener

#endif

// v2.3
class CIMiniSocket
{
public:
virtual	~CIMiniSocket()	= 0;
	
virtual	HPORT	CreatePort(DWORD dwDataID, DWORD dwMode)  = 0;
virtual	HPORT	FindPort(DWORD	dwDataID)  = 0;
virtual	HPORT	GetPort(int indx) = 0;
virtual	int		GetPortsCount()	  = 0;
virtual	BOOL	GetPortInfo(HPORT hPort, DWORD *pDataID, INT *pCurQueueSize, DWORD *pPacketsPassed)	 = 0;

virtual	BOOL	AddListener	(HPORT hPort, VJMSLISTENER pFnc, LPARAM lParam) = 0;
virtual	BOOL	RemListener	(LPARAM lParam, HPORT hPort )  = 0;

virtual	DWORD	GetPortMode(HPORT hPort)	= 0;
virtual	DWORD	GetDataID(HPORT hPort)		= 0;
virtual	INT		GetData(HPORT hPort, LPBYTE pData, INT nOffs, DWORD dwTimeout=0)  = 0;
virtual	BOOL	PutData(HPORT hPort, LPBYTE pData, DWORD dwBytes)	  = 0;
};

// v2.4

typedef VOID (*VJEMONITORCALLBACKPROC)(LPDWORD pBuf, int iWidth, int iHeight, LPARAM lParam);

class CIMonitor
{
public:

virtual	~CIMonitor() = 0;
virtual	BOOL	SetCallback(VJEMONITORCALLBACKPROC pfn, LPARAM lUserParam) = 0;
// v3.5
virtual	INT		GetNumAdapters() = 0;
virtual	LPCSTR	GetAdapterName(int n) = 0;
virtual	void	ShowProgressBar(int pcent) = 0;
virtual	void	RenderWallpaper(LPCSTR lpPath=NULL) = 0;
virtual VJEMONITORCALLBACKPROC GetCallback(LPARAM &lUserParam) = 0;
};



class CIVJEDispatcher
{
public:
virtual ~CIVJEDispatcher() = 0;
		//raw data dispatcher
virtual void	PutRawDataPtr(INT rawDataID, LPVOID ptr) = 0;
		//params dispatcher
virtual SRCID	RegisterSource(LPCSTR lpSrcName, BOOL bBoolParam, DWORD driverID) = 0;
virtual BOOL	UnregisterSource(SRCID uSrc) = 0;
virtual PARAMID	RegisterParameter(SRCID uSrc, LPCSTR lpParamName, LPCSTR lpParamDesc, UINT uParamsFlags = 0) = 0;
virtual HPARAM	UpdateParam(HPARAM hParam, PARAMID uParam, FLOAT flParam) = 0;
virtual HPARAM	UpdateParam(HPARAM hParam, PARAMID uParam, BOOL bParam) = 0;
		//for extern clients
virtual INT		GetNumSources() = 0;
virtual BOOL	GetSourceInfo(int indx,  SRCID *uSrc, LPCSTR *lppName, INT *pParams) = 0;
virtual PARAMID GetParamInfo(int indx, SRCID uSrc, LPCSTR *lppName, LPCSTR *lppDesc, BOOL *bBool, UINT *uFlags=NULL) =0;
virtual HPARAM	UpdateVariable(HPARAM hParam, PARAMID uParam, BOOL *pVariable) = 0;
virtual HPARAM	UpdateVariable(HPARAM hParam, PARAMID uParam, FLOAT *pVariable) = 0;
virtual BOOL	RegisterRawDataTarget(INT uid) = 0; //just ref. counter
virtual void	UnRegisterRawDataTarget(INT uid) = 0;//just ref. counter
virtual LPVOID	GetRawDataPtr(INT uid) = 0;
virtual HPARAM	LockBoolParamUpdate(HPARAM hParam, PARAMID uParam, BOOL bLock) = 0;
virtual HPARAM	LockFloatParamUpdate(HPARAM hParam, PARAMID uParam, BOOL bLock) = 0;

};

#endif
