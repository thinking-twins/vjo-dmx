// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		vjoDefines.h
// Project:		vje32
//
// Contents:	public definitions for vJo
//
//
//
// Author:		Vadim Gorbatenko
// Started:		22.02.00 14:10:12
//
// Version:	3.00	


/************************************
  REVISION LOG ENTRY
  Revision By: VG
  Revised on 20.05.00 13:41:28
  Revised on 02.09.01 0:58:20
 ************************************/


#ifndef __VJO_DEFINES__
#define __VJO_DEFINES__

/*
// removed by nema. use SettingsManager.dll instead!

#ifdef _DEBUG
#define	DEFAULT_PLUGING_DIR "Plugins\\debug"
#else
#define	DEFAULT_PLUGING_DIR "Plugins"
#endif
#define	DEFAULT_GALLERY_DIR  "Gallery"
#define	DEFAULT_RESOURCE_DIR "Resources"
#define	DEFAULT_VJO_DIR		 "c:\\vjo" 
*/

#define	DEFAULT_DFX_EXT		"DFX"
#define	DEFAULT_FXC_EXT		"FXC"
#define	DEFAULT_PLIST_EXT	"VJS"
#define	DEFAULT_OVERLAY_EXT	"VJO"
#define	DEFAULT_PKG_EXT		"FXP"

//global registry data
#define	REG_VJO_KEY			"vje32"				//HKEY_CURRENT_USER\\Software\\Visualight\\vje32
#define	REG_VJO_INSTALL		"InstallPath"		//HKEY_CURRENT_USER\\Software\\Visualight\\vje32\\InstallPath
#define	REG_VJO_VERSION		"EngineVersion"		//HKEY_CURRENT_USER\\Software\\Visualight\\vje32\\EngineVersion

//>v2.5
#define	HIDEFXPREFIX		"*"
#define	REG_VJO_USEPKGDB	"pkgdb"

#define PI 3.1415926535f
#define PI2  6.283185307f
typedef	__int64 EFFECTUID;

//v3.00 reserved UID's for kernel effects
#define	VJO_PRIVATE_UID_LAST	0xFFFFFFFFFFFFFFFF
#define	VJO_PRIVATE_UID_FIRST	(VJO_PRIVATE_UID_LAST-0xFFFF)


//vJo Raw data uid's
#define FXRAWDATA_SSYSTEM	 0x0001 //SSDATAHEADER
#define FXRAWDATA_MIDISYSTEM 0x0003 //MIDIDATAHEADER
#define FXRAWDATA_IADATA     0x0004 //CVJIASDATA
#define FXRAWDATA_REMOTEDATA 0x0005 //not for dfx

#define FXRAWDATA_VSYSTEM	 0x000A //VSDATAHEADER Channel#1
#define FXRAWDATA_VSYSTEMLAST	 FXRAWDATA_VSYSTEM+8 //Channel#N last

#define	VJE_CURRENT_VERSION	 0x355	//vjoGOLD

//default	HBITMAPS
#define HBITMAP_NO			0x0
#define HBITMAP_EMPTY		0xffffffff
#define HBITMAP_GENERIC		(HBITMAP_EMPTY-1)
#define HBITMAP_LAYERS		(HBITMAP_EMPTY-2)
#define HBITMAP_COLORS		(HBITMAP_EMPTY-3)
#define HBITMAP_3D			(HBITMAP_EMPTY-4)
#define HBITMAP_ALPHAMASK	(HBITMAP_EMPTY-5)
#define HBITMAP_MISC		(HBITMAP_EMPTY-6)
#define HBITMAP_RESOURCE	(HBITMAP_EMPTY-7)
#define HBITMAP_DRAWING		(HBITMAP_EMPTY-8)
#define HBITMAP_2D			(HBITMAP_EMPTY-9)
#define HBITMAP_FILTER		(HBITMAP_EMPTY-10)
#pragma message( "******* COMPILING VERSION 3.5 ********" ) //HERE IT IS
#endif
