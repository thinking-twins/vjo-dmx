// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY VISUALIGHT 
//
// File:		DfxDlg.cpp
// Project:
//
// Contents:	
//
//
//
// Author:		Vadim Gorbatenko
// Started:		11.05.01 12:53:13
//
// Version:		

#include "stdafx.h"
#include "DfxDlg.h"

HWND	g_hVJOPreviewWnd = NULL;

void	PlaceDfxDialog(HWND hDialog)
{
	if(g_hVJOPreviewWnd && hDialog)
	{
		RECT	rcDialog;
		RECT	rcPreview;
		
		GetWindowRect(hDialog, &rcDialog);
		GetWindowRect(g_hVJOPreviewWnd, &rcPreview);
		OffsetRect(&rcDialog, 0, rcPreview.top - rcDialog.bottom);
		if(rcPreview.top <= rcDialog.bottom)
		{
			if(rcDialog.top >=0)
			::SetWindowPos(hDialog, NULL, rcDialog.left, rcDialog.top, 0,0, SWP_NOZORDER|SWP_NOSIZE);
		}
	}
}
