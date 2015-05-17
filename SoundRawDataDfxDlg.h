#if !defined(DFXDLG__101474171228117250__INCLUDED_)
#define DFXDLG__101474171228117250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "common/DfxDlg.h"

class CSoundRawDataDfxDlg : public CDialog
{
public:
	CSoundRawDataDfxDlg(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CSoundRawDataDfxDlg)
	enum { IDD = IDD_DIALOG1 };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSoundRawDataDfxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CSoundRawDataDfxDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
