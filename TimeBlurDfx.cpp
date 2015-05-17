/* ________________________________________________________________________________________
	
	THIS FILE IS PART OF THE VISUALJOCKEY, COPYRIGHT (C) BY VISUALIGHT, ALL RIGHTS RESERVED 

	Sample for dynamic inputscreensize:
	- how to make the resolution of an input dynamic:
		SIZE	GetInputScreenRes(int i, SIZE szOutput);
   ______________________________________________________________________________________*/


#include "stdafx.h"
#include <math.h>
#include "pkgSamples.h"
#include "dfxAPI.h"
#include "common/WrapDynamicVars.h"
#include "common.h"

class CTimeBlurDfx: public CDFX
{
	DECLARE_EFFECT()
public:
	~CTimeBlurDfx();
	CTimeBlurDfx();
	
	BOOL	UpdateConfig();
	BOOL	Initialize();
	BOOL	SetupDialog();
	BOOL	Render(CScreen **ppInput, CScreen *pOutput);
	HBITMAP	GetLabelBitmap();
	DWORD	GetMemoryUsage();
	SIZE	GetInputScreenRes(int i, SIZE szOutput);
	SIZE	GetOutputScreenRes();

protected:
	BOOL	GetConfigData(CConfigData *p);
	void	SetConfigData(CConfigData *p);

	int     m_nXRes;
	int     m_nYRes;

	float    m_fFactor;
	//bool		m_gradation;

	DWORD		m_nMemUsage;

private:

	void SetPixel(LPDWORD ptr, int x, int y, DWORD col);
	DWORD GetPixel(LPDWORD ptr, int x, int y);

	LPDWORD mBuffer;
};

BEGIN_INPUTSCREENS_DESC(CTimeBlurDfx)
//{{256,256}, "RGB Bitmap" } you can also declare stratic inputscreenssizes, i.e. [256x256] like this!
{{-1,-1}, "RGB Bitmap" }	// this usually means, that the size of the input will be equal with the size of the output.
							// we need this too, if we want to make the inputsize dynamic
							// for more input screens, just add more E.G.
/*
{{-1,-1}, "RGB 0"},
{{-1,-1}, "RGB 1"},
{{-1,-1}, "RGB 2"}
*/


END_INPUTSCREENS_DESC(CTimeBlurDfx)

BEGIN_OUTPUTSCREENS_DESC(CTimeBlurDfx)
{-1, -1}, ""
END_OUTPUTSCREENS_DESC(CTimeBlurDfx)

BEGIN_EFFECT_DESC(CTimeBlurDfx, VJO_PRIVATE_UID_FIRST - 0xFB13, (350<<16), "Thinking-Twins\\TimeEcho", "Thinking-Twins GbR", "Smooth color fadings between frames")
END_EFFECT_DESC(CTimeBlurDfx) 

REGISTER_DFXFACTORY(CTimeBlurDfx)

CTimeBlurDfx::CTimeBlurDfx()
{
	m_nMemUsage = sizeof(CTimeBlurDfx);
	m_nXRes = 0;
	m_nYRes = 0;

	m_fFactor = 0.5;

	mBuffer = NULL;
}

CTimeBlurDfx::~CTimeBlurDfx()
{
	if(mBuffer)
		delete mBuffer;
}

HBITMAP	CTimeBlurDfx::GetLabelBitmap()			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (HBITMAP) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_TIMEBLUR_LOGO), IMAGE_BITMAP, 0, 0, 0);
}

void	CTimeBlurDfx::SetConfigData(CConfigData *p)
{
	p->SetInt("!", 1);
	//p->SetInt("C", m_correction);
	//p->SetInt("G", m_gradation);
}

BOOL CTimeBlurDfx::GetConfigData(CConfigData *p)
{
	if(!p->GetInt("!", 0)) return FALSE;

	//m_correction = (bool) p->GetInt("C", (int) m_correction);
	//m_gradation = (bool) p->GetInt("G", (int) m_gradation);

	return TRUE;
}

BOOL	CTimeBlurDfx::SetupDialog()
{
	VJO_MANAGE_STATE();
	//CTimeBlurDfxDlg	dlg;

	//dlg.m_correction = m_correction;
	//dlg.m_gradation = m_gradation;

	//g_hVJOPreviewWnd = m_pEngine->GetOutputWindow();
	//if (dlg.DoModal() != IDOK) return FALSE;

	//m_correction = dlg.m_correction;
	//m_gradation = dlg.m_gradation;

	return 	TRUE;
}

BOOL CTimeBlurDfx::UpdateConfig()
{
	CConfigData *pConfigData = m_pEngine->GetCurrentConfig();

	if(!GetConfigData(pConfigData))	
		SetConfigData(pConfigData);

	RegisterFloat(m_pEngine, &m_fFactor, _T("Factor"), 0.0f, 1.0f, 500.0f, 10000.0f, 1000.0f);

	return TRUE;
}

BOOL	CTimeBlurDfx::Initialize()
{
	SIZE sz = m_pEngine->GetOutputRes();
	m_nXRes = sz.cx;
	m_nYRes = sz.cy;

	if(!mBuffer)
	{
		int size = m_nXRes * m_nYRes * sizeof(DWORD);

		m_nMemUsage += size;

		mBuffer = (LPDWORD) new BYTE[size];
		memset(mBuffer, 0, size);
	}

	return TRUE;
}

// --------------- helpers -----------------

void CTimeBlurDfx::SetPixel(LPDWORD ptr, int x, int y, DWORD col)
{
	if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
		return;

		ptr[y * m_nXRes + x] = col;
}

DWORD CTimeBlurDfx::GetPixel(LPDWORD ptr, int x, int y)
{
	if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
		return 0x0;

	return ptr[y * m_nXRes + x];
}

//int size = 0;

DWORD subtractRGB(DWORD col1, DWORD col2, BYTE val)
{
	BYTE r = (BYTE) max(GetR(col2), GetR(col1) - (int) val);
	BYTE g = (BYTE) max(GetG(col2), GetG(col1) - (int) val);
	BYTE b = (BYTE) max(GetB(col2), GetB(col1) - (int) val);

	return MAKERGB(r, g, b);
}

BYTE getMax(DWORD col) {
	return max(max(GetR(col), GetG(col)), GetB(col)); }

BYTE getMin(DWORD col) {
	return min(min(GetR(col), GetG(col)), GetB(col)); }

BYTE getBrightness(DWORD col) {
	return getMax(col); }

BOOL	CTimeBlurDfx::Render(CScreen **ppInput, CScreen *pOutput)
{
	CScreen *in = *ppInput;

	SIZE inSize = in->GetSize();

	SIZE outSize = pOutput->GetSize();

	DWORD *src = (DWORD*) in->GetBuffer();
	DWORD* pDest = (DWORD*) pOutput->GetBuffer();

	int delta = m_pEngine->GetDeltaTime();

	for(int x = 0; x < m_nXRes; x++)
	{
		for(int y = 0; y < m_nYRes; y++)
		{
			DWORD color = GetPixel(src, x, y);
			DWORD bufferColor = GetPixel(mBuffer, x, y);

			BYTE brightness1 = getBrightness(color);
			BYTE brightness2 = getBrightness(bufferColor);

			if(brightness1 < brightness2)
			{
				float diff = (float) 0.006 * pow((float) (brightness2 - brightness1), 1.5f);

				BYTE sub = (BYTE) ceil( m_fFactor * diff);

				color = subtractRGB(bufferColor, color, sub);
			}

			SetPixel(pDest, x, y, color);
			SetPixel(mBuffer, x, y, color);
		}
	}

	//todo: just copy pDest to mBuffer
	return TRUE;
}

SIZE CTimeBlurDfx::GetInputScreenRes(int i, SIZE szOutput)
{
	return CDFX::GetInputScreenRes(i, szOutput);
}

SIZE CTimeBlurDfx::GetOutputScreenRes()
{
	return CDFX::GetOutputScreenRes();
}

DWORD	CTimeBlurDfx::GetMemoryUsage()
{
	return m_nMemUsage;
}