// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY GREEN HIPPO 
//
// File:		vjoRCtl.h
// Project:		vje32
//
// Contents:	vJo Remote Control definitions
//
//
//
// Author:		Vadim Gorbatenko
// Started:		14.07.01 14:22:49
//
// Version:		1.00

#ifndef _VJORCTL_DEF
#define _VJORCTL_DEF

#ifndef __VJO_DEFINES__
typedef	__int64 EFFECTUID;
#endif

typedef	BYTE	RFRETCODE;

//objects ID's
#define	VJERCOBJ_SERVER		0x000
#define	VJERCOBJ_ENGINE		0x001
#define	VJERCOBJ_FXCGALLERY	0x002
#define	VJERCOBJ_AUTOPILOT	0x003
#define	VJERCOBJ_TREE		0x004


#define MKRCHDR(client_id, obj, func, argbytes)                              \
		((DWORD)(BYTE)(argbytes) | ((DWORD)(BYTE)(func) << 8) |   \
		((DWORD)(BYTE)(obj) << 16) | ((DWORD)(BYTE)(client_id) << 24 ))

#define	_FUNCID(hdr)	((BYTE)(hdr>>8))
#define	_OBJID(hdr)		((BYTE)(hdr>>16))
#define	_CLID(hdr)		((BYTE)(hdr>>24))
#define	_NUMB(hdr)		((BYTE)(hdr))

#define	APINFO_ROOT		0x00	//root item
#define	APINFO_TLINE	0x80	//timeline
#define	APINFO_UNAVAIL	0x40	//unavailble item
#define	APINFO_LINK		0x20	//link
						//low 4 bits
#define	APINFO_FXC		0x01	//fxc
#define	APINFO_FOLDER	0x02	//[sub]folder
#define	APINFO_SUBLIST	0x03	//sublist
#define	APINFO_COMMENT	0x04	//comment
#define	APINFO_CMD		0x05	//cmd
#define	APINFO_OVERLAY	0x06	//overlay


//		FUNCTION		ARGUMENT				RETDATA		DESC

//engine
#define	RF_E_VERSION	0	//no				WORD		gets engine version (0x240 ==v2.40)
#define	RF_E_FLAGS		1	//no				DWORD		gets engine flags
#define	RF_E_OBJECTNAME	2	//no				STRING		gets current object name
#define	RF_E_RUN		3	//WORD				RFRETCODE	starts/stops rendering
#define	RF_E_RTIME		4	//no				DWORD		gets engine run-time(ms)
//v2.5
#define	RF_E_LOADOL		5	//INT64(ID)			RFRETCODE	load overlayfxc (send 0 for unload)
#define	RF_E_ENABLEOL	6	//WORD				RFRETCODE	enable/disable overlayfxc
#define	RF_E_OLINFO		7	//no				INT64		fxc ID	gets active overlay info
							//					WORD		folder index
							//					STRING		fxc NAME

		//engine RC funcions
#define	RF_E_SESBEGIN	10	//STRING			RFRETCODE	client begins session
#define	RF_E_SESEND		11	//no			*NO RETCODE*	client ends session
#define	RF_E_SESRESUME	RF_E_SESBEGIN						//client resumes session
		
//server: response sent only via VJDS_RESPONSE port(client_id ignored)
#define	RF_S_SESCONFIRM	 0	//WORD(session#)	STRING		utility request: confirm session#
#define	RF_S_SESTERM	 1	//WORD(session#)	RFRETCODE	utility request: term session#
#define	RF_S_NUMSESIONS	 2	//no				WORD		utility request: number of sessions
#define	RF_S_GETFREEPORT 3	//no				WORD		utility request: get free port

//fxcgallery
#define	RF_G_SIZE		0	//no				WORD		gets fxc gallery size
#define	RF_G_NUMFOLDERS	1	//no				WORD		gets number of folders 
#define	RF_G_FOLDERINFO	2	//WORD(folder)		STRING		gets  gallery folder info(path)
#define	RF_G_ITEMINFO	3	//WORD(item)		INT64		fxc ID
							//					WORD		folder index
							//					STRING		fxc NAME
//fxctree
#define	RF_T_LOAD		0	//INT64(ID)			RFRETCODE	load fxc 
#define	RF_T_UNLOAD		1	//no				RFRETCODE	unload fxc 
#define	RF_T_OBJECTINFO	2	//no				INT64		fxc ID			gets active fxc info
							//					WORD		folder index
							//					STRING		fxc NAME

//autopilot
#define	RF_A_OBJECTINFO	0	//no				BYTE		flags			gets active playlist info
							//					DWORD		numItems
							//					DWORD		curItem
#define	RF_A_ITEMINFO	1	//DWORD(item)		BYTE		flags			gets playlist item info
							//					DWORD		lifetime
							//					STRING		name
#define	RF_A_POSTEVENT	2	//DWORD(ID)			RFRETCODE	post event
							//DWORD(Flags)
							//DWORD(Param)


//Remark: if engine returns == sizeof(RFRETCODE) instead of necessary data, 
//this mean it returns exactly RFRETCODE value
//
//retcodes
#define	RES_NOERROR			0
#define	RES_BUSY			1
#define	RES_NOTFOUND		2
#define	RES_PARAMOUTOFRANGE	3
#define	RES_ILLEGALCALL		4
#define	RES_NOTIMPLEMENTED	5
#define	RES_INVALIDOBJECT	6
#define	RES_INVALIDFUCTION	7
#define	RES_INVALIDPARAM	8
#define	RES_FAILURE			9


//engine flags
#define	RVJEMODE_PRESENTATION	0x80000000 //full screen mode
#define	RVJEMODE_PLAYLIST		0x20000000 //playlist
#define	RVJEMODE_RENDERFXC		0x10000000 //fxc
#define	RVJEMODE_SAFEMODE		0x0400000C //SAFE MODE
#define	RVJEMODE_DEMOMODE		0x01000000 //DEMO MODE
#define	RVJEMODE_RUN			0x08000000 //rendering enabled
#define	RVJEMODE_ENABLEOL		0x04000000 //overlay enabled
#define	RVJEMODE_DUALHEAD		0x00400000 //diagnostic flag: ignored during startup
#define	RVJEMODE_WINDOWED		0x00800000 //windowed(non-exclusive) (used with VJEMODE_PRESENTATION)

//special code: client must call RF_E_SESRESUME
#define	RES_CONFIRMSESSION	0xff					//asks client to confirm current session
#define	RES_ABORTSESSION	(RES_CONFIRMSESSION-1)	//notify clients if server shutting down

// remark about RF_S___ commmands
// 1. Port VJDS_RESPONSE cannot be opened by client in exclusive mode
#endif