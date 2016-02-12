#pragma once

#include <WinDef.h>

inline BYTE GetB(DWORD rgb) {
	return LOBYTE(rgb); }

inline BYTE GetG(DWORD rgb) {
	return LOBYTE(((WORD)(rgb)) >> 8); }

inline BYTE GetR(DWORD rgb) {
	return LOBYTE((rgb)>>16); }

inline DWORD MAKERGB(BYTE r, BYTE g, BYTE b) {
	return (DWORD)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)); }

inline DWORD MAKERGB(BYTE b) {
	return (DWORD)(((BYTE)(b)|((WORD)((BYTE)(b))<<8))|(((DWORD)(BYTE)(b))<<16)); }