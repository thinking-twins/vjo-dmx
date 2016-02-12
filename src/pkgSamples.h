// pkgSamples.h : main header

#ifndef __pkgSamplesPKG__
#define __pkgSamplesPKG__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SDK/vJo32/include/dfxAPI.h"

#define	MAX_PACKAGE_SIZE	1024
/////////////////////////////////////////////////////////////////////////////
// CpkgSamplesApp
// See pkgSamples.cpp for the implementation of this class
//

class CpkgSamplesApp : public CWinApp
{
public:
	CpkgSamplesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CpkgSamplesApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CpkgSamplesApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

typedef CDFX * (* DFXFACTORY)( );

void	RegisterDFXFactory(DFXFACTORY);

class	vjopkg_self_registrator
{
public:
	vjopkg_self_registrator(DFXFACTORY factory)	
	{
		RegisterDFXFactory(factory);
	}
	BOOL regOK;

};

#define	REGISTER_DFXFACTORY(class_name) \
	CDFX* class_name##_factory() {return new class_name;}\
	vjopkg_self_registrator sr_init##class_name((DFXFACTORY)class_name##_factory);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
