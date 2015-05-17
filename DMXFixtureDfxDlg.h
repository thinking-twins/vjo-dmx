#if !defined(DFXDLG__9795830954697655__INCLUDED_)
#define DFXDLG__9795830954697655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#define WM_SET_STATUS				WM_APP + 0x12

class CDMXFixtureDfxDlg : public CDialog
{
public:

	CDMXFixtureDfxDlg(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CDownSampleDfxDlg)
	enum { IDD = IDD_DIALOG_DMXFIXTURE };
	unsigned int	m_nDmxChannel;
	float					m_fXOffset;
	float					m_fYOffset;
	BOOL					m_showReference;
	int						m_sampleMode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDownSampleDfxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	void EnableControls();

	//{{AFX_MSG(CDownSampleDfxDlg)
	virtual BOOL OnInitDialog();
	//afx_msg void OnCheckEqual0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
