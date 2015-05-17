#include "stdafx.h"
#include "SoundRawDataDfxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSoundRawDataDfxDlg::CSoundRawDataDfxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundRawDataDfxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundRawDataDfxDlg)
	//}}AFX_DATA_INIT
}

void CSoundRawDataDfxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundRawDataDfxDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSoundRawDataDfxDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundRawDataDfxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSoundRawDataDfxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	PlaceDfxDialog(GetSafeHwnd());
	return TRUE;
}
