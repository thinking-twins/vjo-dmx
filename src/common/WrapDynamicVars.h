#ifndef __WRAP_DYNAMIC_VARS__
#define __WRAP_DYNAMIC_VARS__

inline float CleverAmp(FLOAT fMinValue, FLOAT fMaxValue, FLOAT fInit)
{
	fMaxValue  -= fMinValue;
	fInit -= fMinValue;
	fMinValue = 0.0f;

	// shrink amplitude, so that it doesn't get it clipped by default!
	float fMaxAmp = (fMaxValue - fMinValue)*0.5f;
	float fMiddle = fMinValue + fMaxAmp;
	if (fInit > fMiddle) return fInit- fMiddle;
	else if (fInit == fMiddle) return fMiddle;
	else return fInit;
}

//-----------------------

inline UINT Register(CProxyEngine* pEngine, LPVOID lpVarAddress, LPCSTR lpVarName, FLOAT fMinValue, FLOAT fMaxValue, FLOAT fDefAmp, FLOAT fMinPeriod, FLOAT fMaxPeriod, FLOAT fDefPeriod, DWORD dwFlags)
{
	FXVAR vars;
	vars.lpVarName = lpVarName;
	vars.lpVarAddress = lpVarAddress;	
	vars.flMinValue = fMinValue;
	vars.flMaxValue = fMaxValue;
	vars.flMinPeriod = fMinPeriod;
	vars.flMaxPeriod = fMaxPeriod;
	vars.flDefPeriod = fDefPeriod;
	vars.flDefAmplitude = fDefAmp;
	vars.dwFlags = dwFlags;
	return pEngine->RegisterVariable(&vars); 
}

// ---------------------------------------

inline UINT RegisterFloat(CProxyEngine* pEngine, LPVOID lpVarAddress, LPCSTR lpVarName, FLOAT fMinValue = 0.0f, FLOAT fMaxValue = 1.0f, FLOAT fMinPeriod = 200.0f, FLOAT fMaxPeriod = 10000.0f, FLOAT fDefPeriod = 2000.0f, DWORD dwFlags = FXVAR_FLOAT|FXVAR_PERIODIC|FXVAR_DEFPERIOD|FXVAR_DEFAMLPITUDE)
{
	float fA = CleverAmp(fMinValue, fMaxValue, *((float*)lpVarAddress));
	return Register(pEngine, lpVarAddress, lpVarName, fMinValue, fMaxValue, fA, fMinPeriod, fMaxPeriod, fDefPeriod, dwFlags);
}

inline UINT RegisterFloatCos(CProxyEngine* pEngine, LPVOID lpVarAddress, LPCSTR lpVarName, FLOAT fMinValue = 0.0f, FLOAT fMaxValue = 1.0f, FLOAT fMinPeriod = 200.0f, FLOAT fMaxPeriod = 10000.0f, FLOAT fDefPeriod = 2000.0f)
{
	return RegisterFloat(pEngine, lpVarAddress, lpVarName, fMinValue, fMaxValue, fMinPeriod, fMaxPeriod, fDefPeriod, FXVAR_FLOAT|FXVAR_PERIODIC|FXVAR_DEFPERIOD|FXVAR_DEFPERIODIC|FXVAR_DEFCOS|FXVAR_DEFAMLPITUDE);
}

inline UINT RegisterBool(CProxyEngine* pEngine, LPVOID lpVarAddress, LPCSTR lpVarName, FLOAT fMinPeriod = 200.0f, FLOAT fMaxPeriod = 10000.0f, FLOAT fDefPeriod = 2000.0f, DWORD dwFlags = FXVAR_BOOL|FXVAR_PERIODIC|FXVAR_DEFPERIOD)
{
	return Register(pEngine, lpVarAddress, lpVarName, 0.0f, 1.0f, 0.5f, fMinPeriod, fMaxPeriod, fDefPeriod, dwFlags);
}


#endif