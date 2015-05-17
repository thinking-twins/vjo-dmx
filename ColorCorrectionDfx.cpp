/* ________________________________________________________________________________________
	
	THIS FILE IS PART OF THE VISUALJOCKEY, COPYRIGHT (C) BY VISUALIGHT, ALL RIGHTS RESERVED 

	Sample for dynamic inputscreensize:
	- how to make the resolution of an input dynamic:
		SIZE	GetInputScreenRes(int i, SIZE szOutput);
   ______________________________________________________________________________________*/


#include "stdafx.h"
#include <math.h>
#include "pkgSamples.h"
#include "ColorCorrectionDfxDlg.h"
#include "dfxAPI.h"
#include "common.h"

class CColorCorrectionDfx: public CDFX
{
	DECLARE_EFFECT()
public:
	~CColorCorrectionDfx();
	CColorCorrectionDfx();
	
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

	bool    m_correction;
	bool		m_gradation;

	DWORD		m_nMemUsage;

private:

	void SetPixel(LPDWORD ptr, int x, int y, DWORD col);
	DWORD GetPixel(LPDWORD ptr, int x, int y);
};

BEGIN_INPUTSCREENS_DESC(CColorCorrectionDfx)
//{{256,256}, "RGB Bitmap" } you can also declare stratic inputscreenssizes, i.e. [256x256] like this!
{{-1,-1}, "RGB Bitmap" }	// this usually means, that the size of the input will be equal with the size of the output.
							// we need this too, if we want to make the inputsize dynamic
							// for more input screens, just add more E.G.
/*
{{-1,-1}, "RGB 0"},
{{-1,-1}, "RGB 1"},
{{-1,-1}, "RGB 2"}
*/


END_INPUTSCREENS_DESC(CColorCorrectionDfx)

BEGIN_OUTPUTSCREENS_DESC(CColorCorrectionDfx)
{-1, -1}, ""
END_OUTPUTSCREENS_DESC(CColorCorrectionDfx)

BEGIN_EFFECT_DESC(CColorCorrectionDfx, VJO_PRIVATE_UID_FIRST - 0xFB23, (350<<16), "Thinking-Twins\\ColorCorrection", "Thinking-Twins GbR", "Apply color correction and gradation")
END_EFFECT_DESC(CColorCorrectionDfx) 

REGISTER_DFXFACTORY(CColorCorrectionDfx)

CColorCorrectionDfx::CColorCorrectionDfx()
{
	m_nMemUsage = sizeof(CColorCorrectionDfx);
	m_nXRes = 0;
	m_nYRes = 0;

	m_correction = true;
	m_gradation = true;
}

CColorCorrectionDfx::~CColorCorrectionDfx()
{
}

HBITMAP	CColorCorrectionDfx::GetLabelBitmap()			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (HBITMAP) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_COLORCORRECTION), IMAGE_BITMAP, 0, 0, 0);
}

void	CColorCorrectionDfx::SetConfigData(CConfigData *p)
{
	p->SetInt("!", 1);
	p->SetInt("C", m_correction);
	p->SetInt("G", m_gradation);
}

BOOL CColorCorrectionDfx::GetConfigData(CConfigData *p)
{
	if(!p->GetInt("!", 0)) return FALSE;

	m_correction = (bool) p->GetInt("C", m_correction);
	m_gradation = (bool) p->GetInt("G", m_gradation);

	return TRUE;
}

BOOL	CColorCorrectionDfx::SetupDialog()
{
	VJO_MANAGE_STATE();
	CColorCorrectionDfxDlg	dlg;

	dlg.m_correction = m_correction;
	dlg.m_gradation = m_gradation;

	g_hVJOPreviewWnd = m_pEngine->GetOutputWindow();
	if (dlg.DoModal() != IDOK) return FALSE;

	m_correction = dlg.m_correction;
	m_gradation = dlg.m_gradation;

	return 	TRUE;
}

BOOL CColorCorrectionDfx::UpdateConfig()
{
	CConfigData *pConfigData = m_pEngine->GetCurrentConfig();
	if(!GetConfigData(pConfigData))	SetConfigData(pConfigData);

	return TRUE;
}

BOOL	CColorCorrectionDfx::Initialize()
{
	SIZE sz = m_pEngine->GetOutputRes();
	m_nXRes = sz.cx;
	m_nYRes = sz.cy;

	return TRUE;
}

// --------------- helpers -----------------

void CColorCorrectionDfx::SetPixel(LPDWORD ptr, int x, int y, DWORD col)
{
	if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
		return;

		ptr[y * m_nXRes + x] = col;
}

DWORD CColorCorrectionDfx::GetPixel(LPDWORD ptr, int x, int y)
{
	if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
		return 0x0;

	return ptr[y * m_nXRes + x];
}

DWORD applyCorrection(DWORD color, bool colorCorrection, bool gradation)
{
	BYTE r = GetR(color);
	BYTE g = GetG(color);
	BYTE b = GetB(color);

	//apply gradation
	if(gradation)
	{
		r = (BYTE) 255 * pow(r / 255.0f, 3);
		g = (BYTE) 255 * pow(g / 255.0f, 3);
		b = (BYTE) 255 * pow(b / 255.0f, 3);
	}

	//apply color correction
	if(colorCorrection)
	{
		BYTE m = min(r, min(g, b));
		float x = g / 255.0f;

		g = min(255, g + 255.0f * 0.2f * pow(x, 0.8f));

		if(g > 220)
		{
			r = min(180, r);
			b = min(180, b);
		}
	}

	return 0xFF000000 | MAKERGB(r, g, b);
}

//int size = 0;

BOOL	CColorCorrectionDfx::Render(CScreen **ppInput, CScreen *pOutput)
{
	CScreen *in = *ppInput;

	SIZE inSize = in->GetSize();

	SIZE outSize = pOutput->GetSize();

	DWORD *src = (DWORD*) in->GetBuffer();
	DWORD* pDest = (DWORD*) pOutput->GetBuffer();

	// clear output buffer (black-screen)
	//memset(pDest, 0, m_nXRes * m_nYRes * 4);

	for(int x = 0; x < m_nXRes; x++)
	{
		for(int y = 0; y < m_nYRes; y++)
		{
			DWORD color = GetPixel(src, x, y);

			color = applyCorrection(color, m_correction, m_gradation);

			SetPixel(pDest, x, y, color);
		}
	}

	return TRUE;
}

SIZE CColorCorrectionDfx::GetInputScreenRes(int i, SIZE szOutput)
{
	//old code:
	//if (szOutput.cx == -1 && szOutput.cy == -1){
	//	return CDFX::GetInputScreenRes(i, szOutput); // you MUST keep this! if szOutput equals [-1,-1], then the output-resolution is still undefined!
	//}

	//SIZE s;
	//if (i == 0){ // this is a common code for input-screen 0
	//	s.cx = m_nX0;
	//	s.cy = m_nY0;
	//	return s;
	//}

	return CDFX::GetInputScreenRes(i, szOutput);
}

SIZE CColorCorrectionDfx::GetOutputScreenRes()
{
	return CDFX::GetOutputScreenRes();
}

DWORD	CColorCorrectionDfx::GetMemoryUsage()
{
	return m_nMemUsage;
}