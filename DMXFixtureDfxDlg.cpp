#include "stdafx.h"
#include "pkgSamples.h"
#include "DMXFixtureDfxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDMXFixtureDfxDlg::CDMXFixtureDfxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMXFixtureDfxDlg::IDD, pParent)
	, m_nDmxChannel(1)
	, m_fXOffset(0.5)
	, m_fYOffset(0.5)
	, m_showReference(true)
	, m_sampleMode(0)
{
	//{{AFX_DATA_INIT(CDownSampleDfxDlg)
	//}}AFX_DATA_INIT
}

void CDMXFixtureDfxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDownSampleDfxDlg)
	DDX_Text(pDX, IDC_FIXTURE_DMXCHANNEL, m_nDmxChannel);
	DDX_Text(pDX, IDC_FIXTURE_XOFFSET, m_fXOffset);
	DDX_Text(pDX, IDC_FIXTURE_XOFFSET, m_fYOffset);
	//int cbState = m_showReference ? BST_CHECKED : BST_UNCHECKED;
	DDX_Check(pDX, IDC_SHOW_REFERENCE, m_showReference);
	DDX_CBIndex(pDX, IDC_SAMPLE_MODE, m_sampleMode);

	//DDX_Check(pDX, IDC_CHECK_EQUAL0, m_bEqual0);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDMXFixtureDfxDlg, CDialog)
	//{{AFX_MSG_MAP(CDownSampleDfxDlg)
	//ON_BN_CLICKED(IDC_CHECK_EQUAL0, OnCheckEqual0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDMXFixtureDfxDlg::EnableControls()
{

}

BOOL CDMXFixtureDfxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	PlaceDfxDialog(GetSafeHwnd());

	EnableControls();
	
	return TRUE;
}