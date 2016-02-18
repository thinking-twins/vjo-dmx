/* ________________________________________________________________________________________
	
	THIS FILE IS PART OF THE VISUALJOCKEY, COPYRIGHT (C) BY VISUALIGHT, ALL RIGHTS RESERVED 
   ________________________________________________________________________________________

	Author:		Yannick Suter
	Started:	02/26/02 17:41:52

	NOTE:		Yannick Suter agrees that VISUALIGHT may use this plugin
				free for any commercial purpose

	More infos about visualJockey can be get at:
	Homepage:	http://www.visualJockey.com
	E-mail:		info@visualight.com
   ______________________________________________________________________________________*/

#include "stdafx.h"

#include "pkgSAMPLES.h"
#include "DMXOutputDfxDlg.h"
#include "utils/rgb.h"
#include "utils/time.h"

#include "DMXOutput.h"

#include <math.h>

class CDMXOutputDfx: public CDFX
{
	DECLARE_EFFECT()

public:

	CDMXOutputDfx();
	~CDMXOutputDfx();
	
	BOOL	UpdateConfig();
	BOOL	Initialize();
	BOOL	SetupDialog();
	BOOL	Render(CScreen **ppInput, CScreen *pOutput);
	HBITMAP	GetLabelBitmap();
	DWORD	GetMemoryUsage();

	void setDmxColorValue(int channel, COLORREF col);

	SIZE GetOutputScreenRes();

protected:

	BOOL	GetConfigData(CConfigData *p);
	void	SetConfigData(CConfigData *p);

	// draw routines
	//void	DrawBar(LPDWORD pBuffer, int nXPos, int nYPos, int nWidth, int nHeight, DWORD dwColor);

	CDMXOutputDfxDlg *mainDlg;

	MatrixSettings m_settings;

	int     video_width;
	int     video_height;
	int			video_area;

	DWORD   m_nMemUsage;

	DWORD GetPixel(LPDWORD ptr, int x, int y);
	void setStatus(LPCTSTR message);
	
	void runDMXTest();
	void nextTest(int index = -1);
	int m_addressMode;
};

//forward declarations
int getLEDMatrixChannel(int x, int y, int addressMode, int matrixColumns, int matrixRows, int dmxOffset);

//Visual jockey registration
BEGIN_INPUTSCREENS_DESC(CDMXOutputDfx)
{{-1,-1}, "RGB Bitmap" }	// this usually means, that the size of the input will be equal with the size of the output.
END_INPUTSCREENS_DESC(CDMXOutputDfx)

BEGIN_OUTPUTSCREENS_DESC(CDMXOutputDfx)
{-1,-1},  ""
END_OUTPUTSCREENS_DESC(CDMXOutputDfx)

BEGIN_EFFECT_DESC(CDMXOutputDfx, VJO_PRIVATE_UID_FIRST - 0xFB234D252, (350<<16) , "Thinking-Twins\\DMX Output", "Thinking-Twins GbR", "Map screen colors to dmx output")
END_EFFECT_DESC(CDMXOutputDfx) 

REGISTER_DFXFACTORY(CDMXOutputDfx)

const int LED_IGNORE = 0;

CDMXOutputDfx::CDMXOutputDfx()
: m_settings(DEFAULT_COLUMNS, DEFAULT_ROWS, DMX_START_CHANNEL)
{
	video_width = 0;
	video_height = 0;
	video_area = 0;
	m_nMemUsage = sizeof(CDMXOutputDfx);
	m_nMemUsage += sizeof(CDMXOutputDfxDlg);

	mainDlg = NULL;

	m_addressMode = 0;

	m_settings.DMXChannel += LED_IGNORE * 3;

	mainDlg = new CDMXOutputDfxDlg();
}

BOOL CDMXOutputDfx::Initialize()
{
	CWnd* main = CWnd::GetDesktopWindow();

	if(mainDlg != NULL)
	{
		mainDlg->dmxBuffer = DMXOutput::getInstance().getBuffer();
		mainDlg->matrixSettings = m_settings;
		mainDlg->addressMode = m_addressMode;
	}

	SIZE sz = m_pEngine->GetOutputRes();
	video_width = sz.cx;
	video_height = sz.cy;
	video_area=video_width*video_height;

	//dmx system is initialized on first render

	return TRUE;
}

HBITMAP	CDMXOutputDfx::GetLabelBitmap()			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (HBITMAP) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_DMX_LOGO), IMAGE_BITMAP, 0, 0, 0);
}

void CDMXOutputDfx::SetConfigData(CConfigData *p)
{
	p->SetInt("!", 1);
	p->SetInt("A", m_addressMode);
	p->SetInt("MCOL", m_settings.Columns);
	p->SetInt("MROW", m_settings.Rows);
}

BOOL CDMXOutputDfx::GetConfigData(CConfigData *p)
{
	if(!p->GetInt("!", 0)) return FALSE;

	m_addressMode = p->GetInt("A", 0);
	m_settings.Columns = p->GetInt("MCOL", DEFAULT_COLUMNS);
	m_settings.Rows = p->GetInt("MROW", DEFAULT_ROWS);

	return TRUE;
}

BOOL CDMXOutputDfx::SetupDialog()
{
	VJO_MANAGE_STATE();

	mainDlg->DoModal();

	m_addressMode = mainDlg->addressMode;
	m_settings.Columns = mainDlg->matrixSettings.Columns;
	m_settings.Rows = mainDlg->matrixSettings.Rows;

	return TRUE;
}

SIZE CDMXOutputDfx::GetOutputScreenRes()
{
	SIZE s;

	s.cx = 800;
	s.cy = 600;

	return s;
}

BOOL CDMXOutputDfx::UpdateConfig()
{
	CConfigData *pConfigData = m_pEngine->GetCurrentConfig();
	if(!GetConfigData(pConfigData)) SetConfigData(pConfigData);

	return TRUE;
}

void CDMXOutputDfx::setDmxColorValue(int channel, COLORREF col)
{
	DMXOutput::getInstance().setDmxValue(channel, GetR(col));
	DMXOutput::getInstance().setDmxValue(channel + 1, GetG(col));
	DMXOutput::getInstance().setDmxValue(channel + 2, GetB(col));
}

CString lastStatus;

void CDMXOutputDfx::setStatus(LPCTSTR message)
{
	BOOL res = PostMessage(mainDlg->GetSafeHwnd(), WM_SET_STATUS, 0, (LPARAM) (LPCTSTR) message);

	lastStatus = message;
}

int getLEDStripChannel(int offset, int dmxOffset)
{
	return dmxOffset + offset * 3;
}

//apply the sequential effects just for up to x channels
#define MAX_DMX_CHANNELS 50

#define MS_PER_SEC 1000
#define MS_PER_SEC_PART 250
#define MS_PER_SEC_FAST 100

long long testStartTime = 0;
int testIndex = -1;
long long testCounter = 0; //milliseconds since test start
bool autoRun = false;

void CDMXOutputDfx::runDMXTest()
{
	testCounter = milliseconds_now() - testStartTime;

	__int64 sec = testCounter / MS_PER_SEC;
	__int64 secPart = testCounter / MS_PER_SEC_PART;
	__int64 fastCounter = testCounter / MS_PER_SEC_FAST;

	int startIndex = 2;

	if(testIndex == startIndex)
	{
		DMXOutput::getInstance().clearDMXChannels();

		DMXOutput::getInstance().setDmxValue((int) secPart, 0xFF);

		if(secPart >= MAX_DMX_CHANNELS)
			nextTest();
	}
	else if(testIndex == ++startIndex)
	{
		for(int i = 1; i < secPart;i++)
			DMXOutput::getInstance().setDmxValue(i, 0xFF);

		if(secPart >= MAX_DMX_CHANNELS)
			nextTest();
	}
	else if(testIndex == ++startIndex)
	{
		DMXOutput::getInstance().clearDMXChannels((BYTE) fastCounter);

		if(fastCounter > 0xFF)
			nextTest();
	}
	else if(testIndex == ++startIndex)
	{
		DMXOutput::getInstance().clearDMXChannels();

		for(int i = 0; i < MAX_DMX_CHANNELS; i++)
		{
			int c = getLEDStripChannel(i, m_settings.DMXChannel);
			BYTE val = ((i + sec) % 2) ? 0x00 : 0xFF;

			DMXOutput::getInstance().setDmxValue(c, val);
			DMXOutput::getInstance().setDmxValue(c + 1, val);
			DMXOutput::getInstance().setDmxValue(c + 2, val);
		}

		if(sec > 10)
			nextTest();
	}
	else if(testIndex == ++startIndex) //linear brightness
	{
		DMXOutput::getInstance().clearDMXChannels();

		for(int i = 0; i < m_settings.numPixels(); i++)
		{
			BYTE val = (BYTE) ceil(i * (0xFF / (float) (m_settings.numPixels() - 1)));

			int c = getLEDStripChannel(i, m_settings.DMXChannel);

			DWORD color = MAKERGB(val);
			//color = applyCorrection(color, cc, g);

			setDmxColorValue(c, color);
		}

		if(fastCounter > 0xFF)
			nextTest();
	}
	else if(testIndex == ++startIndex)
	{
		for(int c = m_settings.DMXChannel; c < m_settings.DMXChannel + m_settings.numPixels() * 3; c+=3)
		{
			DWORD color = MAKERGB((BYTE) fastCounter, (BYTE) fastCounter, (BYTE) fastCounter);
			setDmxColorValue(c, color);
		}
	}
	else
		testIndex = 2; //start again
}

void CDMXOutputDfx::nextTest( int index /*= -1*/ )
{
	if(testIndex == -1 || (index != -1 && index != testIndex))
	{
		autoRun = index == 1;

		if(autoRun)
			testIndex = 2;
		else
			testIndex = index;
	}
	else
	{
		if(index != -1)
			testIndex = index;
	}

	if(index == -1 && autoRun)
		testIndex++;

	testStartTime = milliseconds_now();
	testCounter = 0;

	DMXOutput::getInstance().clearDMXChannels();
}

DWORD CDMXOutputDfx::GetPixel(LPDWORD ptr, int x, int y)
{
	if(x < 0 || x > video_width || y < 0 || y > video_height)
		return 0x00;

	return ptr[y * video_width + x];
}

//void DrawRect(LPDWORD ptr, DWORD color, int cx, int cy, int width, int height)
//{
//	for(int x=cx; x < cx + width; x++)
//		for(int y=cy; y < cy + height; y++)
//			SetPixel(ptr, x, y, color);
//}

BOOL CDMXOutputDfx::Render(CScreen **ppInput, CScreen *pOutput)
{
	DWORD *pSrc = (DWORD*) (*ppInput)->GetBuffer();

	DWORD *pDest = (DWORD*) pOutput->GetBuffer();

	memcpy_s(pDest, video_area * 4, pSrc, video_area * 4);

	//draw led matrix
	//for(int c = 0; c < m_settings.Columns; c++)
	//	for(int r = 0; r < m_settings.Rows; r++)
	//	{
	//		
	//	}

	DMXOutput::getInstance().initDMX();

	int addressMode = m_addressMode;

	//reflect changes to address mode dropdown
	if(mainDlg->GetSafeHwnd() != NULL) 
	{
		addressMode = mainDlg->addressMode;

		int dlgMode = mainDlg->testMode;

		if(dlgMode > 0 && testIndex != dlgMode)
		{
			if(dlgMode == 1 && !autoRun) //auto mode selected
				nextTest(1);

			if(dlgMode > 1) //specific test selected
				nextTest(dlgMode);
		}

		if(dlgMode == 0)
			testIndex = -1;
	}

	if(testIndex > 0)
	{
		runDMXTest();
	}
	else //map rgb to led
	{
		int matrixColumns = m_settings.Columns;
		int matrixRows = m_settings.Rows;

		//write information to dmx buffer
		int colWidth = video_width / matrixColumns;
		int colHeight = video_height / matrixRows;

		int tileSize = colWidth > colHeight ? colHeight : colWidth;

		int numTilesX = video_width / tileSize;
		int numTilesY = video_height / tileSize;

		int leftOffset = (int) (0.5 * (video_width - tileSize * numTilesX));
		int bottomOffset = (int) (0.5 * (video_height - tileSize * numTilesY));

		for(int x = 0; x < matrixColumns; x++)
		{
			for(int y = 0; y < matrixRows; y++)
			{
				//sample color
				//DWORD color = GetPixel(pSrc, x * colSize + colSize / 2, y * rowSize + rowSize / 2);
				DWORD color = GetPixel(pSrc, leftOffset + x * tileSize + tileSize / 2, bottomOffset + y * tileSize + tileSize / 2);

				//determine dmx adress
				int channel = getLEDMatrixChannel(x, y, addressMode, matrixColumns, matrixRows, m_settings.DMXChannel);

				setDmxColorValue(channel, color);
			}
		}
	}

	if(mainDlg != NULL)
	{
		if(!lastStatus.IsEmpty())
			setStatus(lastStatus);

		::PostMessage(mainDlg->GetSafeHwnd(), WM_UPDATE_CONTROL, 0, 0);
	}

	return TRUE;
}

//HINT: Origin is at bottom left, starting with 0,0

// 0,1 - 1,1
//  |     |
// 0,0 - 1,0 - ...

int getLEDMatrixChannel(int x, int y, int addressMode, int matrixColumns, int matrixRows, int dmxOffset)
{
	int offset = -1;

#if _DEBUG
	addressMode = 3;
#endif

	if(addressMode == 0) //Snake Bottom Left
	{
		offset = y * matrixColumns;

		if(y % 2 == 0) //odd row
			offset += x;
		else
			offset += (matrixColumns - 1) - x;
	}
	else if(addressMode == 1) //Snake Top Right
	{
		offset = (matrixRows - y - 1) * matrixColumns;

		if((matrixRows - y) % 2 != 0) //odd row
			offset += matrixColumns - x - 1;
		else
			offset += x;
	}
	else if(addressMode == 2) //Snake Top Left
	{
		offset = (matrixRows - y - 1) * matrixColumns;

		if((matrixRows - y) % 2 == 0) //odd row
			offset += matrixColumns - x - 1;
		else
			offset += x;
	}
	else if(addressMode == 3) //4Dot-Modules Bottom Left
	{
		int segment = (y / 2) * (matrixColumns / 2); //4 * ((y / 2) * matrixColumns + (x / 2));

		bool flipped = ((y / 2) % 2 != 0);

		if(flipped) //odd row
			segment += ((matrixColumns / 2) - 1) - (x / 2);
		else
			segment += (x / 2);

		int local_offset = 0;

		x = x % 2;
		y = y % 2;

		if(!flipped)
			local_offset = (x == 0 ? y : 3 - y); 
		else
			local_offset = (x == 0 ? 2 + y : 1 - y);

		offset = segment * 4 + local_offset;
	}

	return dmxOffset + offset * 3;
}

DWORD CDMXOutputDfx::GetMemoryUsage()
{
	return m_nMemUsage;
}

CDMXOutputDfx::~CDMXOutputDfx()
{
	if(mainDlg != NULL)
		delete mainDlg;
}