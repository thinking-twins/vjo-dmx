//
// SSystem.h
// Public header
//

#ifndef SSYSTEM_DEF

#define SSYSTEM_DEF

#include "SSDATA.h"

#define SSYSAPI	 __declspec(dllexport)


#ifdef	__cplusplus
extern "C" {
#endif
SSYSAPI	LPVOID	CreateSSObject(); //return pointer to CSSystem object
#ifdef	__cplusplus
}
#endif

typedef LONG (*SSCALLBACKPROC)(SSDATAHEADER *lpData, LPARAM lParam);

//BeatControl codes
#define BC_DISABLE	0	// disable internal beat detector, follow BC_HIT. return new state
#define BC_ENABLE	1	// enable  internal beat detector. return new state
#define BC_STATE	2	// get state. return BC_DISABLE/BC_ENABLE
#define BC_STATE	2	// get state. return BC_DISABLE/BC_ENABLE
#define BC_HIT		3	// hit. uParam means period if state == BC_DISABLE. return -1 if failed
#define BC_DIV2		4	// force to increase beat period twice(for internal beat detector). must be in a BC_ENABLE state. return -1 if failed
#define BC_MUL2		5	// force to decrease beat period twice(for internal beat detector). must be in a BC_ENABLE state. return -1 if failed
#define BC_RESET	6	// reset internal beat detector. must be in a BC_ENABLE state. return -1 if failed
#define BC_BITMUL	7	// get state. return BC_DIV2/BC_MUL2/BC_DISABLE


#ifdef	__cplusplus
class  CSSystem
{
public:
	virtual ~CSSystem();
	
	virtual SSDATAHEADER *GetData(DWORD tm=0) = 0;
	virtual BOOL SetCallback(SSCALLBACKPROC cb, LPARAM lParam, UINT uCB=0) = 0;

	virtual VOID	ShutDown() = 0;
	virtual BOOL	StartUpWaveIn() = 0;
	virtual BOOL	StartUpDevice(LPCSTR cmdString) = 0;

	virtual INT		GetErrorCode() = 0;	//declared in "ssdata.h"
	virtual BOOL	IsRunning() = 0;
	virtual BOOL	StartDialog() = 0;
	virtual CHAR	*GetConfigNameString() = 0;
	virtual BOOL	SetConfigNameString(CHAR *) = 0;
	virtual FLOAT	GetProcessorUtilization() = 0;
	virtual	BOOL	Enable(BOOL bEnb) = 0;
	virtual	VOID	Reset() = 0;/*just set zero all was done*/
	//v3.4
	virtual LONG	BeatControl(UINT uCode, UINT uParam) = 0;
protected:
		CSSystem();

};
#endif


#endif