#if !defined(DFXDLG__DMXOUTPUT__INCLUDED_)
#define DFXDLG__DMXOUTPUT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "common/DfxDlg.h"

const short DEFAULT_COLUMNS = 6;
const short DEFAULT_ROWS = 6;
const int		DMX_START_CHANNEL = 4;

struct MatrixSettings
{
	public:

	short Columns;
	short Rows;

	int DMXChannel;

	MatrixSettings(short Columns, short Rows, int DMXChannel)
	{
		this->Columns = Columns;
		this->Rows = Rows;
		this->DMXChannel = DMXChannel;
	}
	
	MatrixSettings()
	{
		Columns = DEFAULT_COLUMNS;
		Rows = DEFAULT_ROWS;
		DMXChannel = DMX_START_CHANNEL;
	}

	int numPixels() {
		return Columns * Rows; }
};

#define WM_UPDATE_CONTROL   WM_APP + 0x10
#define WM_SET_STATUS				WM_APP + 0x11

class CDMXOutputDfxDlg : public CDialog
{
public:
	CDMXOutputDfxDlg(CWnd* pParent = NULL);   // standard constructor

	//ddx data
	//{{AFX_DATA(CDMXOutputDfxDlg)
	enum { IDD = IDD_DMXDIALOG };
	int addressMode;
	int testMode;
	//}}AFX_DATA

	BYTE *dmxBuffer;
	MatrixSettings matrixSettings;

	//{{AFX_VIRTUAL(CDMXOutputDfxDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	LRESULT OnUpdateControl(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStatus(WPARAM wParam, LPARAM lParam);

	//{{AFX_MSG(CDMXOutputDfxDlg)
	virtual BOOL OnInitDialog();
	virtual void OnPaint();
	virtual void OnClose();
	afx_msg void OnDlgSettingsChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStatusBar statusBar;
};

//{{AFX_INSERT_LOCATION}}

#endif
