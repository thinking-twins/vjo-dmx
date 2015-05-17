#pragma once

class CScreen; 

BYTE GetB(DWORD rgb);

BYTE GetG(DWORD rgb);

BYTE GetR(DWORD rgb);

DWORD GetPixel(DWORD *buffer, int offset);
DWORD GetPixel(LPDWORD ptr, int width, int height, int x, int y);

DWORD MAKERGB(BYTE r, BYTE g, BYTE b);

DWORD MAKERGB(BYTE b);

long long milliseconds_now();

void SetPixel(CScreen *screen, int x, int y, DWORD col);