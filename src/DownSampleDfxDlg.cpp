#include "stdafx.h"
#include "pkgSamples.h"
#include "DownSampleDfxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDownSampleDfxDlg::CDownSampleDfxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownSampleDfxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownSampleDfxDlg)
	//}}AFX_DATA_INIT
}

void CDownSampleDfxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownSampleDfxDlg)
	DDX_Text(pDX, IDC_EDIT_X0, m_nX0);
	DDX_Text(pDX, IDC_EDIT_Y0, m_nY0);
	DDX_Check(pDX, IDC_CHECK_EQUAL0, m_bEqual0);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDownSampleDfxDlg, CDialog)
	//{{AFX_MSG_MAP(CDownSampleDfxDlg)
	//ON_BN_CLICKED(IDC_CHECK_EQUAL0, OnCheckEqual0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDownSampleDfxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	PlaceDfxDialog(GetSafeHwnd());

	((CButton*)GetDlgItem(IDC_CHECK_EQUAL0))->SetCheck(m_bEqual0);
	
	return TRUE;
}
