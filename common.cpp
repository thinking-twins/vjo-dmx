#include "common.h"

#include "StdAfx.h"

#include "dfxAPI.h"

BYTE GetB(DWORD rgb) {
	return LOBYTE(rgb); }

BYTE GetG(DWORD rgb) {
	return LOBYTE(((WORD)(rgb)) >> 8); }

BYTE GetR(DWORD rgb) {
	return LOBYTE((rgb)>>16); }

DWORD MAKERGB(BYTE r, BYTE g, BYTE b) {
	return (DWORD)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)); }

DWORD MAKERGB(BYTE b) {
	return (DWORD)(((BYTE)(b)|((WORD)((BYTE)(b))<<8))|(((DWORD)(BYTE)(b))<<16)); }

long long milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	} else {
		return GetTickCount();
	}
}

DWORD GetPixel( LPDWORD buffer, int offset ) {
	return buffer[offset]; }

DWORD GetPixel(LPDWORD ptr, int width, int height, int x, int y)
{
	if(x < 0 || x >= width || y < 0 || y >= height)
		return 0x0;

	return ptr[y * width + x];
}

void SetPixel(CScreen *screen, int x, int y, DWORD col)
{
	//if(x < 0 || x >= m_nXRes || y < 0 || y >= m_nYRes)
	//	return;

	LPDWORD buffer = (LPDWORD) screen->GetBuffer();

	buffer[y * screen->GetSize().cx + x] = col;
}
