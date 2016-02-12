/* ________________________________________________________________________________________
	
	THIS FILE IS PART OF THE VISUALJOCKEY, COPYRIGHT (C) BY VISUALIGHT, ALL RIGHTS RESERVED 

	Sample for dynamic inputscreensize:
	- how to make the resolution of an input dynamic:
		SIZE	GetInputScreenRes(int i, SIZE szOutput);
   ______________________________________________________________________________________*/


#include "stdafx.h"
#include "pkgSamples.h"
#include "DownSampleDfxDlg.h"
#include "dfxAPI.h"

class CDownSampleDfx: public CDFX
{
	DECLARE_EFFECT()
public:
	~CDownSampleDfx();
	CDownSampleDfx();
	
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

	int     m_nX0; //horizontal pixel resolution
	int			m_nY0; //vertical pixel resolution
	BOOL		m_bBorder; //show grid

	DWORD	m_nMemUsage;

private:

	void FillRect(LPDWORD ptr, DWORD color, int cx, int cy, int width, int height);
	void SetPixel(LPDWORD ptr, int x, int y, DWORD col);
	DWORD GetPixel(LPDWORD ptr, int x, int y);
};

BEGIN_INPUTSCREENS_DESC(CDownSampleDfx)
//{{256,256}, "RGB Bitmap" } you can also declare stratic inputscreenssizes, i.e. [256x256] like this!
{{-1,-1}, "RGB Bitmap" }	// this usually means, that the size of the input will be equal with the size of the output.
							// we need this too, if we want to make the inputsize dynamic
							// for more input screens, just add more E.G.
/*
{{-1,-1}, "RGB 0"},
{{-1,-1}, "RGB 1"},
{{-1,-1}, "RGB 2"}
*/


END_INPUTSCREENS_DESC(CDownSampleDfx)

BEGIN_OUTPUTSCREENS_DESC(CDownSampleDfx)
{-1, -1}, ""
END_OUTPUTSCREENS_DESC(CDownSampleDfx)

BEGIN_EFFECT_DESC(CDownSampleDfx, VJO_PRIVATE_UID_FIRST - 0xFB234D251, (350<<16), "Thinking-Twins\\DownSample", "Thinking-Twins GbR", "Scale down to a low resolution")
END_EFFECT_DESC(CDownSampleDfx) 

REGISTER_DFXFACTORY(CDownSampleDfx)

CDownSampleDfx::CDownSampleDfx()
{
	m_nMemUsage = sizeof(CDownSampleDfx);
	m_nXRes = 0;
	m_nYRes = 0;
	m_bBorder = FALSE;

	m_nX0 = 8;
	m_nY0 = 8;
}

CDownSampleDfx::~CDownSampleDfx()
{
}

HBITMAP	CDownSampleDfx::GetLabelBitmap()			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (HBITMAP) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_DOWNSAMPLE_LOGO), IMAGE_BITMAP, 0, 0, 0);
}

void CDownSampleDfx::SetConfigData(CConfigData *p)
{
	p->SetInt("!", 1);
	p->SetInt("E", m_bBorder);
	p->SetInt("X0", m_nX0);
	p->SetInt("Y0", m_nY0);
}

BOOL CDownSampleDfx::GetConfigData(CConfigData *p)
{
	if(!p->GetInt("!", 0)) return FALSE;

	m_bBorder = p->GetInt("E", m_bBorder);
	
	m_nX0 = p->GetInt("X0", m_nX0);
	m_nY0 = p->GetInt("Y0", m_nY0);
	
	return TRUE;
}

BOOL	CDownSampleDfx::SetupDialog()
{
	VJO_MANAGE_STATE();
	CDownSampleDfxDlg	dlg;

	dlg.m_bEqual0 = m_bBorder;
	dlg.m_nX0 = m_nX0;
	dlg.m_nY0 = m_nY0;

	g_hVJOPreviewWnd = m_pEngine->GetOutputWindow();
	if (dlg.DoModal() != IDOK) return FALSE;

	m_bBorder = dlg.m_bEqual0;
	m_nX0 = dlg.m_nX0;
	m_nY0 = dlg.m_nY0;

	return 	TRUE;
}

BOOL CDownSampleDfx::UpdateConfig()
{
	CConfigData *pConfigData = m_pEngine->GetCurrentConfig();
	if(!GetConfigData(pConfigData))	SetConfigData(pConfigData);

	return TRUE;
}

BOOL	CDownSampleDfx::Initialize()
{
	SIZE sz = m_pEngine->GetOutputRes();
	m_nXRes = sz.cx;
	m_nYRes = sz.cy;

	return TRUE;
}

// --------------- helpers -----------------

void CDownSampleDfx::FillRect(LPDWORD ptr, DWORD color, int cx, int cy, int width, int height)
{
	for(int x=cx; x < cx + width; x++)
		for(int y=cy; y < cy + height; y++)
			SetPixel(ptr, x, y, color);
}

void CDownSampleDfx::SetPixel(LPDWORD ptr, int x, int y, DWORD col)
{
	if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
		return;

		ptr[y * m_nXRes + x] = col;
}

DWORD CDownSampleDfx::GetPixel(LPDWORD ptr, int x, int y)
{
	if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
		return 0x0;

	return ptr[y * m_nXRes + x];
}

class vec2
{
 
public:

	vec2(float X = 0, float Y = 0)
	{
		x = X;
		y = Y;
	};

	~vec2() {} ;
 
	float x, y;
 
	vec2 operator*(float scalar) const
	{
		return vec2(x * scalar, y * scalar);
	}
 
	vec2 operator+(const vec2 &vect) const
	{
		return vec2(x + vect.x, y + vect.y);
	}
 
	vec2 operator-(const vec2 &vect) const
	{
		return vec2(x - vect.x, y - vect.y);
	}
};

BOOL CDownSampleDfx::Render(CScreen **ppInput, CScreen *pOutput)
{
	CScreen *in = *ppInput;

	SIZE inSize = in->GetSize();

	SIZE outSize = pOutput->GetSize();

	//if(outSize.cx * outSize.cy != size)
	//{
	//	TRACE("size changed to %ix%i", outSize.cx, outSize.cy);
	//	size = outSize.cx * outSize.cy;
	//}

	DWORD *src = (DWORD*) in->GetBuffer();
	DWORD* pDest = (DWORD*) pOutput->GetBuffer();

	//prevent crash
	if(m_nX0 == 0 || m_nY0 == 0)
		return FALSE;

	int colWidth = outSize.cx / m_nX0;
	int colHeight = outSize.cy / m_nY0;

	int tileSize = colWidth > colHeight ? colHeight : colWidth;

	if(tileSize == 0)
		return FALSE;

	int numTilesX = outSize.cx / tileSize;
	int numTilesY = outSize.cy / tileSize;

	int leftOffset = (int) (0.5 * (outSize.cx - tileSize * numTilesX));
	int bottomOffset = (int) (0.5 * (outSize.cy - tileSize * numTilesY));

	// clear output buffer (black-screen)
	memset(pDest, 0, m_nXRes * m_nYRes * 4);

	DWORD color = 0;

	static vec2 samples[1] = { vec2(0.5, 0.5) };

	int drawSize = tileSize;

	if(m_bBorder)
	{
		leftOffset += 1;
		bottomOffset += 1;
		drawSize -= 2;
	}

	for(int x = 0; x < m_nX0; x++)
	{
		for(int y = 0; y < m_nY0; y++)
		{
			//sample some pixels
			for(int s = 0; s < 1; s++)
				color = GetPixel(src, (int) (x * tileSize + tileSize * samples[s].x), (int) (y * tileSize + tileSize * samples[s].y));

			FillRect(pDest, color, leftOffset + x * tileSize, bottomOffset + y * tileSize, drawSize, drawSize);
		}
	}

	return TRUE;
}

SIZE CDownSampleDfx::GetInputScreenRes(int i, SIZE szOutput)
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

SIZE CDownSampleDfx::GetOutputScreenRes()
{
	SIZE s;

	s.cx = m_nX0;
	s.cy = m_nY0;

	return s;
}

DWORD	CDownSampleDfx::GetMemoryUsage()
{
	return m_nMemUsage;
}