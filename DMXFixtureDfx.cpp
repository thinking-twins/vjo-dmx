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

#include <vector>
#include <math.h>

#include "pkgSAMPLES.h"
#include "DMXFixtureDfxDlg.h"
#include "common.h"
#include "common/WrapDynamicVars.h"

#include "DMXOutput.h"

class DMXFixture
{

public:

	unsigned int Channels;
	unsigned int ChannelOffset;

	float XOffset;
	float YOffset;

protected:

	std::vector<int> m_nRegisteredChannels; // needed if you want to unregister a variable! (can happen if the dynamic variable depends on the static plugin-settings...)
	std::vector<float> m_fChannelValues;

	std::string Title;

public:
	
	DMXFixture(const std::string &title, unsigned int channelOffset, unsigned int channels = 1)
	{
		this->Title = title;
		this->ChannelOffset = channelOffset;
		this->Channels = channels;
	}

	void setOffset(float x, float y)
	{
		XOffset = x;
		YOffset = y;
	}
};

class CDMXFixtureDfx : public CDFX
{
	DECLARE_EFFECT()

public:

	CDMXFixtureDfx();
	~CDMXFixtureDfx();
	
	BOOL	UpdateConfig();
	BOOL	Initialize();
	BOOL	SetupDialog();
	BOOL	Render(CScreen **ppInput, CScreen *pOutput);
	HBITMAP	GetLabelBitmap();
	DWORD	GetMemoryUsage();

	void runDMXTest();

protected:

	BOOL	GetConfigData(CConfigData *p);
	void	SetConfigData(CConfigData *p);
	void setStatus(LPCTSTR message);
	void nextTest( int index = -1 );

//	CDMXFixtureDfxDlg *mainDlg;

	DWORD m_nMemUsage;

	int m_nStartChannel;

	long long testStartTime;
	int testIndex;
	long long testCounter; //milliseconds since test start
	bool autoRun;

	bool useColorAsInput;

	bool showFixtures; //displays the pixel-value position for all fixtures

	CString lastStatus;

	std::vector<DMXFixture> m_dmxFixtures;
};

//forward declarations
long long milliseconds_now();

//Visual jockey registration
BEGIN_INPUTSCREENS_DESC(CDMXFixtureDfx)
{{-1,-1}, "RGB Bitmap" }	// this usually means, that the size of the input will be equal with the size of the output.
END_INPUTSCREENS_DESC(CDMXFixtureDfx)

BEGIN_OUTPUTSCREENS_DESC(CDMXFixtureDfx)
{-1,-1},  ""
END_OUTPUTSCREENS_DESC(CDMXFixtureDfx)

BEGIN_EFFECT_DESC(CDMXFixtureDfx, VJO_PRIVATE_UID_FIRST - 0xFB234E389, (350<<16) , "Thinking-Twins\\DMX Fixture", "Thinking-Twins GbR", "Map variables to dmx channels")
END_EFFECT_DESC(CDMXFixtureDfx) 

REGISTER_DFXFACTORY(CDMXFixtureDfx)

//const int DMX_NUM_CHANNELS = 2;
//const int DMX_START_CHANNEL = 1;

CDMXFixtureDfx::CDMXFixtureDfx()
: testStartTime(0)
, testIndex(-1)
, testCounter(0)
, autoRun(false)
, useColorAsInput(true)
, showFixtures(true)
{
	m_nMemUsage = sizeof(CDMXFixtureDfx);
	m_nMemUsage += sizeof(CDMXFixtureDfxDlg);

	//mainDlg = NULL;

	m_nStartChannel = 0;
	
	//m_nRegisteredChannels.resize(DMX_NUM_CHANNELS, 0);
	//m_fChannelValues.resize(DMX_NUM_CHANNELS, 0.0f);
}

BOOL CDMXFixtureDfx::Initialize()
{
	CWnd* main = CWnd::GetDesktopWindow();

	/*if(mainDlg != NULL)
	{
	}*/

	SIZE sz = m_pEngine->GetOutputRes();

	//dmx system is initialized on first render
	showFixtures = true;

	//hack: reinit all fixtures
	m_dmxFixtures.clear();

	if(true)
	{
		//register the strobe as fixture
		m_dmxFixtures.push_back(DMXFixture("Strobe Input1", 1));
		m_dmxFixtures[0].setOffset(0.25, 0.75);

		m_dmxFixtures.push_back(DMXFixture("Strobe Input2", 2));
		m_dmxFixtures[1].setOffset(0.75, 0.25);
	}

	return TRUE;
}

HBITMAP	CDMXFixtureDfx::GetLabelBitmap()			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (HBITMAP) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_DMX_LOGO), IMAGE_BITMAP, 0, 0, 0);
}

void CDMXFixtureDfx::SetConfigData(CConfigData *p)
{
	p->SetInt("!", 1);

	p->SetInt("NUM_FIXTURES", m_dmxFixtures.size());

	for(unsigned int i = 0; i < m_dmxFixtures.size(); i++)
	{
		DMXFixture &fixture = m_dmxFixtures.at(i);

		char name[64];
		
		sprintf_s(name, sizeof(name), "DF%i_C", i);
		p->SetInt(name, fixture.ChannelOffset);

		sprintf_s(name, sizeof(name), "DF%i_X", i);
		p->SetFloat(name, fixture.XOffset);

		sprintf_s(name, sizeof(name), "DF%i_Y", i);
		p->SetFloat(name, fixture.YOffset);
	}
}

BOOL CDMXFixtureDfx::GetConfigData(CConfigData *p)
{
	if(!p->GetInt("!", 0)) 
		return FALSE;

	int numFixtures = p->GetInt("NUM_FIXTURES", 0);

	for(int i = 0; i < numFixtures; i++)
	{
		char title[64];
		sprintf_s(title, sizeof(title), "FIXTURE%i", i);

		//TODO: sprintf with i
		int dmxChannel = p->GetInt("DF0_C", 0);

		DMXFixture fixture(title, dmxChannel);
		fixture.setOffset(p->GetFloat("DF0_X", 0.5f), p->GetFloat("DF0_Y", 0.5f));

		m_dmxFixtures.push_back(fixture);
	}

	return TRUE;
}

BOOL CDMXFixtureDfx::SetupDialog()
{
	VJO_MANAGE_STATE();

	CDMXFixtureDfxDlg mainDlg;

	mainDlg.DoModal();

	return TRUE;
}

BOOL CDMXFixtureDfx::UpdateConfig()
{
	CConfigData *pConfigData = m_pEngine->GetCurrentConfig();

	if(!GetConfigData(pConfigData)) 
		SetConfigData(pConfigData);

	// Register Variables
	// If varibles already registered, engine just updates a pointers
	//NOTE VJO variables are BOOL or float.
	
	// note: save this uid if you need dynamic re-assigne, for example
	// if list of variables depends from settings in your main dialogs,
	// you must use m_pEngine->Unregister(uid) to remove unnecessary variables

	//for(std::vector<int>::const_iterator iter = m_nRegisteredChannels.begin(); iter = m_nRegisteredChannels.end(); iter++)
	//{
	//	if(*iter == 0)
	//		continue;

	//	m_pEngine->UnregisterVariable(*iter);

	//	*iter = 0;
	//}		

	//char name[64];

	//for(int i = 0; i < DMX_NUM_CHANNELS; i++)
	//{
		//if(m_nRegisteredChannels[i] != 0)
		//	continue;

//		sprintf_s(name, sizeof(name), "DMX channel %i", DMX_START_CHANNEL + i);

		//m_nRegisteredChannels[i] = RegisterFloat(m_pEngine, &m_fChannelValues[i], name, 0.0f, 255.0f, 500.0f, 10000.0f, 1000.0f);
	//}

	//RegisterBool(m_pEngine, &useColorAsInput, "UseColorInput");

	//m_nUID0 = RegisterBool(m_pEngine, &m_bChangeColors, "Show Bitmap", 500.0f, 2000.0f, 1000.0f); // values define the range of the period and default-period

	return TRUE;
}

void CDMXFixtureDfx::setStatus(LPCTSTR message)
{
	//BOOL res = PostMessage(mainDlg->GetSafeHwnd(), WM_SET_STATUS, 0, (LPARAM) (LPCTSTR) message);

	//lastStatus = message;
}

#define MAX_DMX_CHANNELS 108

#define MS_PER_SEC 1000
#define MS_PER_SEC_PART 250
#define MS_PER_SEC_FAST 100

void CDMXFixtureDfx::runDMXTest()
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
	else
		testIndex = 2; //start again
}

void CDMXFixtureDfx::nextTest( int index /*= -1*/ )
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

int getBrightness(DWORD rgb)
{
	return max(max(GetR(rgb), GetG(rgb)), GetB(rgb));
	/*
   return (int) sqrt(
      GetR(rgb) * GetR(rgb) * .241 + 
      GetG(rgb) * GetG(rgb) * .691 + 
      GetB(rgb) * GetB(rgb) * .068);
			*/
}

BOOL CDMXFixtureDfx::Render(CScreen **ppInput, CScreen *pOutput)
{
	DWORD *pSrc = (DWORD*) (*ppInput)->GetBuffer();

	DWORD *pDest = (DWORD*) pOutput->GetBuffer();

	//just copy the input to the output
	memcpy_s(pDest, pOutput->GetSize().cx * pOutput->GetSize().cy * 4, pSrc, (*ppInput)->GetSize().cx * (*ppInput)->GetSize().cy * 4);

	//try to init dmx
	DMXOutput::getInstance().initDMX();

	//reflect changes to address mode dropdown
	//if(mainDlg->GetSafeHwnd() != NULL) 
	//{
	//	addressMode = mainDlg->addressMode;

	//	int dlgMode = mainDlg->testMode;

	//	if(dlgMode > 0 && testIndex != dlgMode)
	//	{
	//		if(dlgMode == 1 && !autoRun) //auto mode selected
	//			nextTest(1);

	//		if(dlgMode > 1) //specific test selected
	//			nextTest(dlgMode);
	//	}

	//	if(dlgMode == 0)
	//		testIndex = -1;
	//}

	if(testIndex > 0)
	{
		runDMXTest();
	}
	else //just assign variable values and pixel colors
	{
		//sample screen for overall brightness
		int sumBrightness = 0;
		int numSamples = 10;
					
		for(int x = 0; x < numSamples; x++)
		{
			for(int y = 0; y < numSamples; y++)
			{
				int sampleX = (*ppInput)->GetSize().cx * (x / (float) numSamples + 0.5 / numSamples);
				int sampleY = (*ppInput)->GetSize().cy * (y / (float) numSamples + 0.5 / numSamples);

				int brightness = getBrightness(GetPixel(pSrc, (*ppInput)->GetSize().cx, (*ppInput)->GetSize().cy, sampleX, sampleY));

				sumBrightness += brightness;
			}
		}

		int brightness = sumBrightness / (numSamples * numSamples);

		for(unsigned int i = 0; i < m_dmxFixtures.size(); i++)
		{
			DMXFixture &fixture = m_dmxFixtures.at(i);

			for(unsigned int c = 0; c < fixture.Channels; c++)
			{
				int channel = fixture.ChannelOffset + c; // DMX_START_CHANNEL + i;

				BYTE channelvalue = 0;

				if(useColorAsInput || true)
				{
					//channel 1: speed
					//channel 2: dimmer
					if(channel == 1)
						channelvalue = 1;
					else if(channel == 2)
					{
						channelvalue = (BYTE) max(0, min(255, 2.5 * (brightness - 128)));
					}

					//int x = (*ppInput)->GetSize().cx * fixture.XOffset;
					//int y = (*ppInput)->GetSize().cy * fixture.YOffset;

					//channelvalue = GetB(GetPixel(pSrc, (*ppInput)->GetSize().cx, (*ppInput)->GetSize().cy, x, y));

					// set marker pixel
					/*
					if(showFixtures)
					{
						SetPixel(pOutput, x-1, y, MAKERGB(0x00, 0x00, 0x00));
						SetPixel(pOutput, x, y, MAKERGB(0xFF, 0x00, 0xFF));
						SetPixel(pOutput, x+1, y, MAKERGB(0xFF, 0xFF, 0xFF));
					}
					*/
				}
				//else
					//channelvalue = (BYTE) m_fChannelValues[i];

				DMXOutput::getInstance().setDmxValue(channel, channelvalue);
			}
		}
	}

	//if(mainDlg != NULL)
	//{
	//	if(!lastStatus.IsEmpty())
	//		setStatus(lastStatus);
	//}

	return TRUE;
}

DWORD CDMXFixtureDfx::GetMemoryUsage()
{
	return m_nMemUsage;
}

CDMXFixtureDfx::~CDMXFixtureDfx()
{
	//running = false;

	//if(handle != NULL)
	//{
	//	clearDMXChannels();

	//	FT_Close(handle);
	//	handle = NULL;
	//}

	/*if(mainDlg != NULL)
		delete mainDlg;*/
}