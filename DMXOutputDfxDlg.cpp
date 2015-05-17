#include "stdafx.h"

#include <math.h>

#include "DMXOutputDfxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDMXOutputDfxDlg::CDMXOutputDfxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMXOutputDfxDlg::IDD, pParent)
{
	//Create(IDD, NULL);

	//{{AFX_DATA_INIT(CDMXOutputDfxDlg)
	//}}AFX_DATA_INIT

	dmxBuffer = NULL;
	
	addressMode = 0;
	testMode = 0;
}

void CDMXOutputDfxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMXOutputDfxDlg)
	DDX_CBIndex(pDX, IDC_COMBO_ADDRESSMODE, addressMode);
	DDX_CBIndex(pDX, IDC_COMBO_DMXTEST, testMode);
	DDX_Text(pDX, IDC_MATRIXSIZE_COLUMNS, matrixSettings.Columns);
	DDX_Text(pDX, IDC_MATRIXSIZE_ROWS, matrixSettings.Rows);
	//}}AFX_DATA_MAP

	if(pDX->m_bSaveAndValidate)
	{
		
	}
}

BEGIN_MESSAGE_MAP(CDMXOutputDfxDlg, CDialog)
	//{{AFX_MSG_MAP(CDMXOutputDfxDlg)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRESSMODE, OnDlgSettingsChanged)
	//ON_BN_CLICKED(IDC_CB_GRADATION, OnDlgSettingsChanged)
	//ON_BN_CLICKED(IDC_CB_COLOR, OnDlgSettingsChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_DMXTEST, OnDlgSettingsChanged)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATE_CONTROL, OnUpdateControl)
	ON_MESSAGE(WM_SET_STATUS, OnSetStatus)
END_MESSAGE_MAP()

time_t last_update = 0;

LRESULT CDMXOutputDfxDlg::OnUpdateControl(WPARAM wParam, LPARAM lParam)
{
	if(time(NULL) - last_update < 1) // update each second
		return 0;

	last_update = time(NULL);

	Invalidate(false); // this is absolutely necessary to reflect all changes

	return 0;
}

LRESULT CDMXOutputDfxDlg::OnSetStatus(WPARAM wParam, LPARAM lParam)
{
	statusBar.SetPaneText(0, (LPCTSTR) lParam);

	return 0;
}

static UINT BASED_CODE indicators[] = { ID_STATUSBAR };

BOOL CDMXOutputDfxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	PlaceDfxDialog(GetSafeHwnd());

	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRESSMODE))->SetCurSel(addressMode);
	((CComboBox*)GetDlgItem(IDC_COMBO_DMXTEST))->SetCurSel(testMode);

	//add status bar
	statusBar.Create(this);
	statusBar.SetIndicators(indicators, sizeof(indicators));

	//CRect rect;
	//GetClientRect(&rect);
	statusBar.SetPaneInfo(0, ID_STATUSBAR, SBPS_STRETCH, 0);

	//This is where we actually draw it on the screen
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_STATUSBAR);

	statusBar.SetPaneText(0, _T("DMX Plugin Version 1.0"));

	return FALSE;
}

const int CHANNELS_PER_LED = 3;

void CDMXOutputDfxDlg::OnPaint() 
{
	if(IsIconic())
	{
		CDialog::OnPaint();
		return;
	}

	CPaintDC dc(this);

	dc.AssertValid();

	int columns = matrixSettings.Columns * CHANNELS_PER_LED;
	int rows = matrixSettings.Rows;

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(20, 100);
	rect.top = 20;

	int gridWidth = rect.Width() / columns;
	int gridHeight = rect.Height() / rows; 

	COLORREF backcolor = RGB(168, 168, 168);
	COLORREF textcolor = RGB(45, 45, 45);
  COLORREF textBgcolor = RGB(200, 200, 200);

	dc.FillSolidRect(&rect, backcolor);

	if(dmxBuffer == 0)
	{
		dc.SetTextColor(textcolor);
		dc.SetTextAlign(TA_CENTER);
		dc.TextOut(rect.CenterPoint().x, rect.CenterPoint().y, "No DMX Device selected");
		GetDlgItem(IDC_COMBO_ADDRESSMODE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ADDRESSMODE)->ShowWindow(SW_SHOW);
		dc.SetTextColor(textcolor);
		
		CFont newFont;
		newFont.CreatePointFont(58, "Arial", &dc);
		
		CFont* pOldFont = dc.SelectObject(&newFont);

		dc.SetTextAlign(TA_RIGHT);

		for(int i = 0; i < rows; i++)
		{
			static char val[10];
			sprintf_s(val, sizeof(val), "%02i", matrixSettings.DMXChannel + i * columns);
			dc.TextOut(rect.left - 2, rect.top + i * gridHeight + gridHeight / 3, val);
		}

		dc.SetTextAlign(TA_CENTER);
		dc.SetBkMode(TRANSPARENT);

		for(int r = 0; r < rows; r++)
		{
			for(int c = 0; c < columns; c++)
			{
				//show dmx channel value
				int channel = matrixSettings.DMXChannel + c + r * columns;

				if(channel >= 511)
					continue;

				BYTE bufferValue = dmxBuffer[channel];

				int x = rect.left + c * gridWidth;
				int y = rect.top + r * gridHeight;

				if(c % 3 == 0)
				{
					COLORREF col = RGB(dmxBuffer[channel], dmxBuffer[channel + 1], dmxBuffer[channel + 2]);
					dc.FillSolidRect(x + 1, y + 4, gridWidth * 3 - 12, gridHeight - 8, col);

					int hue = (dmxBuffer[channel] + dmxBuffer[channel + 1] + dmxBuffer[channel + 2]) / 3;

					if(hue > 125)
						textcolor = RGB(25, 25, 25);
					else
						textcolor = RGB(225, 225, 225);
				}

				x -= 5 * (c % 3);

				static char val[10];
				sprintf_s(val, sizeof(val), "%02X", bufferValue);

				dc.SetTextColor(textcolor);
				dc.TextOut(x + gridWidth / 2, y + gridHeight / 3, val);
			}
		}

		dc.SelectObject(pOldFont);
	}
}

void CDMXOutputDfxDlg::OnClose()
{
	UpdateData();

	CDialog::OnClose();
}

void CDMXOutputDfxDlg::OnDlgSettingsChanged()
{
	UpdateData();

	Invalidate(false);
}