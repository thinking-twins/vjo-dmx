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
#include "SoundRawDataDfxDlg.h"

// you need this include to access the sound rawstream resource
#include "SDK/vjo32/SSYSTEM/include/ssdata.h"

#define SPECTRUM_SIZE 256
#define WAVEFORM_SIZE 512

//this is where the sound data will be stored
struct SoundData {
	// right / left channel
	byte	Waveform[2][512];
	byte	Spectrum[256];
};

class CSoundRawDataDfx: public CDFX
{
	DECLARE_EFFECT()
public:
	~CSoundRawDataDfx();
	CSoundRawDataDfx();
	
	BOOL	UpdateConfig();
	BOOL	Initialize();
	BOOL	SetupDialog();
	BOOL	Render(CScreen **ppInput, CScreen *pOutput);
	HBITMAP	GetLabelBitmap();
	DWORD	GetMemoryUsage();

	// this procedure will be called by the engine, if soundstream has been registred correctly before
	// -> make sure, you declare it to be public!
	void	ProcessRawData(UINT uiData, LPVOID lpData);

protected:
	BOOL	GetConfigData(CConfigData *p);
	void	SetConfigData(CConfigData *p);

	// draw routines
	void	DrawBar(LPDWORD pBuffer, int nXPos, int nYPos, int nWidth, int nHeight, DWORD dwColor);

	int     video_width;
	int     video_height;
	int		video_area;

	DWORD   m_nMemUsage;

	// sound rawstream access
	BOOL	m_bRegisteredRawTarget;

	// sound rawstream data
	bool m_bFirstRun;
	SoundData m_SoundData;
	bool m_bWaveform;
	bool m_bSpectrum;
};

DEFINE_NOINPUTSCREENS_DESC(CSoundRawDataDfx)

BEGIN_OUTPUTSCREENS_DESC(CSoundRawDataDfx)
{-1,-1},  ""
END_OUTPUTSCREENS_DESC(CSoundRawDataDfx)

BEGIN_EFFECT_DESC(CSoundRawDataDfx, VJO_PRIVATE_UID_FIRST - 0xFB234E388, (350<<16) , "Thinking-Twins\\Sound RawData", "Thinking-Twins GbR", "Sound Processing Unit")
END_EFFECT_DESC(CSoundRawDataDfx) 

REGISTER_DFXFACTORY(CSoundRawDataDfx)

CSoundRawDataDfx::CSoundRawDataDfx()
{
	video_width = 0;
	video_height = 0;
	video_area = 0;
	m_nMemUsage = sizeof(CSoundRawDataDfx);

	//this MUST be true, to prevent a crash with the sound system
	m_bFirstRun=true;

	//set what data we are interested in
	m_bWaveform=true;
	m_bSpectrum=true;

	// init sound rawdata access
	m_bRegisteredRawTarget = FALSE;
}

CSoundRawDataDfx::~CSoundRawDataDfx()
{
	// if registered don't forget to unregister the datastream
	if(m_bRegisteredRawTarget){
		m_pEngine->UnregisterRawDataTarget(FXRAWDATA_SSYSTEM);
		m_bRegisteredRawTarget = FALSE;
	}
}

HBITMAP	CSoundRawDataDfx::GetLabelBitmap()			
{
	//this returns a vjo internal bitmap for the plugin
	return (HBITMAP)HBITMAP_GENERIC;

	//if you want to use your own image for the plugin use the following code:
	//where fx_switch is your bitmap. it must be 64x64, 256 colours
	//see enclosed plugin bitmap text for details on VJO default bitmap schemes

/*
	return (HBITMAP) LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(FX_SWITCH),IMAGE_BITMAP, 0, 0, 0);
*/
}

void CSoundRawDataDfx::SetConfigData(CConfigData *p)
{
	p->SetInt("!", 1);
}

BOOL CSoundRawDataDfx::GetConfigData(CConfigData *p)
{
	if(!p->GetInt("!", 0)) return FALSE;

	return TRUE;
}

BOOL CSoundRawDataDfx::SetupDialog()
{
	VJO_MANAGE_STATE();
	CSoundRawDataDfxDlg	dlg;
	
	g_hVJOPreviewWnd = m_pEngine->GetOutputWindow();
	if (dlg.DoModal() != IDOK) return FALSE;
	
	return 	TRUE;
}

BOOL CSoundRawDataDfx::UpdateConfig()
{
	CConfigData *pConfigData = m_pEngine->GetCurrentConfig();
	if(!GetConfigData(pConfigData)) SetConfigData(pConfigData);
	
	// if we haven't registred the sound datastream yet, do it now
	m_bRegisteredRawTarget = m_pEngine->RegisterRawDataTarget(FXRAWDATA_SSYSTEM);
//	if (!m_bRegisteredRawTarget) m_bRegisteredRawTarget = m_pEngine->RegisterRawDataTarget(FXRAWDATA_SSYSTEM);

	return TRUE;
}


BOOL CSoundRawDataDfx::Initialize()
{
	SIZE sz = m_pEngine->GetOutputRes();
	video_width = sz.cx;
	video_height = sz.cy;
	video_area=video_width*video_height;

	//because there is a bug in the original sound data code, we need to track if it is the first run
	//you MUST set it to true to begin with or the sound routines will crash!!!
	m_bFirstRun=true;

	m_bWaveform=true;
	m_bSpectrum=true;

	return TRUE;
}

BOOL CSoundRawDataDfx::Render(CScreen **ppInput, CScreen *pOutput)
{
	DWORD* pDest = (DWORD*)pOutput->GetBuffer();

	// clear output buffer (black-screen)
	memset(pDest, 0, video_area*4);

	for(int x=0;x<255;x++){
		int spectrum=m_SoundData.Spectrum[x]/2;
		DrawBar(pDest, x, 0,1,spectrum, RGB(0,255,0));

		int leftwave=(m_SoundData.Waveform[0][x*2]/2)+128;
		int rightwave=(m_SoundData.Waveform[1][x*2]/2)+128;

		pDest[x+leftwave*video_width]=RGB(255,0,0);
		pDest[x+rightwave*video_width]=RGB(255,0,255);
	}

	return TRUE;
}

void CSoundRawDataDfx::DrawBar(LPDWORD pBuffer, int nXPos, int nYPos, int nWidth, int nHeight, DWORD dwColor)
{
	int nX, nY;

	if (nHeight < 0 || nWidth < 0)	return;

	if ((nXPos+nWidth >= video_width) || (nYPos+nHeight >= video_height) || (nXPos+nWidth < 0) || (nYPos+nHeight < 0))
		return;

	for (nY = 0; nY < nHeight; nY++){
		for (nX = 0; nX < nWidth; nX++){
			pBuffer[((nYPos+nY)*video_width)+(nXPos+nX)] = dwColor;
		}
	}
}

DWORD CSoundRawDataDfx::GetMemoryUsage()
{
	return m_nMemUsage;
}

void CSoundRawDataDfx::ProcessRawData(UINT uiData, LPVOID lpData)
{
	if(uiData == FXRAWDATA_SSYSTEM && !m_bFirstRun){
        SSDATAHEADER *pSoundDataHeader = (SSDATAHEADER *)lpData;

		int nOffest;
		int nIndex;
		int fx;

		// wave data
		if (m_bWaveform) {
			int nSamples = pSoundDataHeader->pWavRawData.iSamplesCount;

			fx = (int)((float)nSamples/512.0f);

			short *pRightChannel = (short *)pSoundDataHeader->pWavRawData.lpRawSamplesBuffer;
			short *pLeftChannel  = pRightChannel + 1;

			nOffest = 0;
			for (nIndex = 0; nIndex < 512; nIndex++, nOffest+=2){//fx) {
				m_SoundData.Waveform[0][nIndex] = (byte)(*(pRightChannel+nOffest)>>8)+128;
				m_SoundData.Waveform[1][nIndex] = (byte)(*(pLeftChannel+nOffest)>>8)+128;
			}
		}

		// spectrum data
		if (m_bSpectrum) {
			float af = 255.0f / 60000.0f;
			fx = (int)((float)pSoundDataHeader->pDSPRawData.iFFTCount/255.0f);
					
			nOffest = 0;
			for (nIndex = 0; nIndex < 256; nIndex++, nOffest+=fx) {
				float f = *(pSoundDataHeader->pDSPRawData.lpFFTArray+nOffest)*af;

				m_SoundData.Spectrum[nIndex] = (byte)(f);
			}
		}
	};
	m_bFirstRun=false;
}