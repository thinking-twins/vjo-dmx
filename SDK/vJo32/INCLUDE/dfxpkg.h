// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		dfxpkg.h
// Project:		
//
// Contents:	dfx package dll header
//
//
//
// Author:		Vadim Gorbatenko
// Started:		30.01.01 20:40:13
//
// Version:		

#ifndef _DFXPKG_DEF
#define	_DFXPKG_DEF
#ifndef	PLUGINAPI
#define	PLUGINAPI	__declspec(dllexport)
#endif

#define	PKGEXPORT1	vjopkgCreateNewDFXObject
#define	PKGFACTORY	"vjopkgCreateNewDFXObject"
#define	PKGEXPORT2	vjopkgGetSize
#define	PKGGETSIZE	"vjopkgGetSize"
#define	PKGEXPORT3	vjopkgGetMemUsage
#define	PKGGETMEMUSAGE	"vjopkgGetMemUsage"
#define	PKGEXPORT4	vjopkgGetInfo
#define	PKGGETINFO	"vjopkgGetInfo"

//GOLD optional export
#define	PKGEXPORT5	vjopkgNotifyEx
#define	PKGNOTIFYEX	"vjopkgNotifyEx"


extern "C" PLUGINAPI CDFX *PKGEXPORT1(int ip);	//vjopkgCreateNewDFXObject
extern "C" PLUGINAPI INT PKGEXPORT2();			//vjopkgGetSize
extern "C" PLUGINAPI DWORD PKGEXPORT3();		//vjopkgGetMemUsage
extern "C" PLUGINAPI LPCSTR PKGEXPORT4();		//vjopkgGetInfo
//GOLD
extern "C" PLUGINAPI LPVOID PKGEXPORT5(UINT uNotify, DWORD dwParam);		//vjopkgNotifyEx

typedef CDFX*	(*PKGFACTORYRPROC)(INT);
typedef INT		(*PKGGETSIZEPROC)();
typedef DWORD	(*PKGGETMEMPROC)();
typedef LPCSTR	(*PKGGETINFOPROC)();
//GOLD
typedef VOID	(*PKGNOTIFYPROC)(UINT , DWORD);

//notifications					uNotify	  dwParam									comments
#define	PKGNOTIFY_STARTUP_DONE	0x0001	//flags	from DFXAPI.H, ie VJEMODE_xxx range	sent after engines startup
#define	PKGNOTIFY_RESTART_DONE	0x0002	//flags	from DFXAPI.H, ie VJEMODE_xxx range	set after engine restarts
#define	PKGNOTIFY_OUTPUT_RES	0x0003	//loword = CX, hiword=CY					sent after messages declared above, informs about resolution
#define	PKGNOTIFY_SHUTDOWN		0x0004	//not used									sent before engines shutdown
#define	PKGNOTIFY_RENDERING		0x0005	//TRUE/FALSE								sent when rendering enabled or disabled
#define	PKGNOTIFY_RENDERSTEP	0x0006	//not used									sent before render step


#endif
