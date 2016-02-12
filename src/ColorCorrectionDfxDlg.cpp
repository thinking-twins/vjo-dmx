#include "stdafx.h"
#include "pkgSamples.h"
#include "ColorCorrectionDfxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorCorrectionDfxDlg::CColorCorrectionDfxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorCorrectionDfxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorCorrectionDfxDlg)
	//}}AFX_DATA_INIT
}

void CColorCorrectionDfxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorCorrectionDfxDlg)
	DDX_Check(pDX, IDC_CHECK_GRADATION, m_gradation);
	DDX_Check(pDX, IDC_CHECK_CORRECTION, m_correction);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorCorrectionDfxDlg, CDialog)
	//{{AFX_MSG_MAP(CColorCorrectionDfxDlg)
	//ON_BN_CLICKED(IDC_CHECK_EQUAL0, OnCheckEqual0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColorCorrectionDfxDlg::EnableControls()
{
	//m_bEqual0 = ((CButton*)GetDlgItem(IDC_CHECK_EQUAL0))->GetCheck();
	GetDlgItem(IDC_CHECK_GRADATION)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_CORRECTION)->EnableWindow(true);
}

BOOL CColorCorrectionDfxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	PlaceDfxDialog(GetSafeHwnd());

	((CButton*)GetDlgItem(IDC_CHECK_CORRECTION))->SetCheck(m_correction);
	((CButton*)GetDlgItem(IDC_CHECK_GRADATION))->SetCheck(m_gradation);

	EnableControls();
	
	return TRUE;
}

void CColorCorrectionDfxDlg::OnCheckEqual0() 
{
	EnableControls();
}
