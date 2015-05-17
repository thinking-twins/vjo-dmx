#if !defined(DFXDLG__97958309289181655__INCLUDED_)
#define DFXDLG__97958309289181655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
class CColorCorrectionDfxDlg : public CDialog
{
public:
	CColorCorrectionDfxDlg(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CColorCorrectionDfxDlg)
	enum { IDD = IDD_DIALOG_COLORCORRECTION };
	BOOL	m_correction;
	BOOL		m_gradation;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CColorCorrectionDfxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	void EnableControls();

	//{{AFX_MSG(CColorCorrectionDfxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckEqual0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
