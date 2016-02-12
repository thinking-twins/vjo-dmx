// pkgSamples.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#define DECLARE_IMPLEMENT_BASE_CLASS
#include "pkgSamples.h"
#include "dfxpkg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CpkgSamplesApp, CWinApp)
	//{{AFX_MSG_MAP(CpkgSamplesApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CpkgSamplesApp::CpkgSamplesApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CpkgSamplesApp object

CpkgSamplesApp theApp;
DFXFACTORY	g_pfnFactories[MAX_PACKAGE_SIZE];
INT	g_iTotalFactories = 0;

//simplest registrator
void	RegisterDFXFactory(DFXFACTORY pfn)
{
	if(g_iTotalFactories<MAX_PACKAGE_SIZE)
		g_pfnFactories[g_iTotalFactories++] = pfn;
}


extern "C" PLUGINAPI CDFX *PKGEXPORT1(int ip)//vjopkgCreateNewDFXObject
{
	if(ip<0 || ip>=g_iTotalFactories)
		return NULL;
	return (* g_pfnFactories[ip])();
}

extern "C" PLUGINAPI INT PKGEXPORT2()//vjopkgGetSize
{
	return g_iTotalFactories;
}

extern "C" PLUGINAPI DWORD PKGEXPORT3()//vjopkgGetMemUsage
{
	return 0;
}

extern "C" PLUGINAPI LPCSTR PKGEXPORT4()		//vjopkgGetInfo
{
	static char *ptr = "LEDMatrix Output";
	return (LPCSTR)ptr;
}