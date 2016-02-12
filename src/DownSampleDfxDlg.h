#if !defined(DFXDLG__9795830965181655__INCLUDED_)
#define DFXDLG__9795830965181655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
class CDownSampleDfxDlg : public CDialog
{
public:
	CDownSampleDfxDlg(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CDownSampleDfxDlg)
	enum { IDD = IDD_DIALOG_INPUTSIZE };
	int		m_nX0;
	int		m_nY0;
	BOOL	m_bEqual0;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDownSampleDfxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//void EnableControls();

	//{{AFX_MSG(CDownSampleDfxDlg)
	virtual BOOL OnInitDialog();
	//afx_msg void OnCheckEqual0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
