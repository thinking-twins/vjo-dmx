// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY GREEN HIPPO 
//
// File:		vjoTFX.h
// Project:		vjo
//
// Contents:	vJo Transitions FX public header
//
//
//
// Author:		Vadim Gorbatenko
// Started:		04.10.01 23:01:47
//
// Version:		

#ifndef _VJOTFX_DEF
#define _VJOTFX_DEF

#ifndef	PLUGINAPI
#define	PLUGINAPI	__declspec(dllexport)
#endif


class CConfigData;
class CScreen;
//base class for Transitions
class CTransition
{
protected:
		CTransition() {m_dwDelay = m_dwRecentTime = 0; m_iPreviewCounter = 0; m_bForceMaxFPS = TRUE;}
public:

	virtual	~CTransition() {;}
virtual	CTransition	*Dup() = 0;
virtual	void	GetConfig(CConfigData *s);
virtual	void	SetConfig(CConfigData *s);
virtual	DWORD	GetMemoryUsage()	{return 0;}
		
virtual	BOOL		ConfirmRendering(int iChannel)=0;
virtual	void		Reset() = 0;
virtual	void		StartTransition() = 0;
virtual	CScreen *	Render(CScreen *screenOutput, CScreen **screensInputs, int numInputs, DWORD dwCurTime) = 0;
		//info
virtual	LPCSTR		GetName() = 0;
virtual	DWORD		GetID() = 0;
virtual	BOOL		HasExtDialog()	{return FALSE;}
virtual	BOOL		ExtDialog()		{return FALSE;}
virtual	BOOL		Configure();
virtual	DWORD		GetProgress()	{return 0; }
		DWORD		GetDuration()	{return m_dwDelay;}
		BOOL		m_bForceMaxFPS;

protected:
	static	CString m_Name;
	static	DWORD	m_dwID;		
	
	DWORD	m_dwDelay;
	int		m_iPreviewCounter;
	
	DWORD	m_dwRecentTime;
	float	m_fAlpha;
	BOOL	m_Key;
};
	
#define	DECLARE_TRANSITION_INFO()\
protected:\
	static	CString m_Name;\
	static	DWORD	m_dwID;


#define	DECLARE_TRANSITION_OVERRIDES(transClass)\
	CTransition	*Dup()	{return new transClass;}\
	void		Reset();\
	void		StartTransition();\
	BOOL		ConfirmRendering(int iChannel);\
	CScreen *	Render(CScreen *screenOutput, CScreen **screensInputs, int numInputs, DWORD);\
	LPCSTR		GetName()	{return m_Name;}\
	DWORD		GetID()		{return m_dwID;}\


#define	DESCRIBE_TRANSITION(transClass, id, name)\
	CString transClass::m_Name = name;\
	DWORD	transClass::m_dwID = id;

	

#define	TFXEXPORT1	tfxCreateNewTFXObject
#define	TFXFACTORY	"tfxCreateNewTFXObject"
#define	TFXEXPORT2	tfxGetNumTFX
#define	TFXGETNUMTFX "tfxGetNumTFX"
#define	TFXEXPORT3	tfxGetInfo
#define	TFXGETINFO	"tfxGetInfo"
#define	TFXSETENGINE "tfxSetEngine"
#define	TFXEXPORT4	tfxSetEngine

#define	TFXDLL		"vjotfx.dll"

extern "C" PLUGINAPI CTransition *TFXEXPORT1(int ip);	//tfxCreateNewTFXObject
extern "C" PLUGINAPI INT TFXEXPORT2();					//tfxGetNumTFX
extern "C" PLUGINAPI LPCSTR TFXEXPORT3();				//tfxGetInfo
extern "C" PLUGINAPI VOID TFXEXPORT4(LPVOID);			//tfxSetEngine


#endif