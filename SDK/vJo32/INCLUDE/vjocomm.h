// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY GREEN HIPPO 
//
// File:		vjocomm.h
// Project:
//
// Contents:	VJO COMMUNICATION PROTOCOL
//
//
//
// Author:		Vadim Gorbatenko
// Started:		14.05.01 20:28:49
//
// Version:		

#ifndef _VJOCOMM_DEF
#define _VJOCOMM_DEF

#define	VJC_MAX_PACKETLENGH	256	//maximum bytes per one datagram

#pragma pack(1)

typedef struct	//4 bytes
{
	unsigned char	uControl;
	unsigned char	uLength;
	unsigned short	uDataID;
}	VJCPACKETHEADER;

#pragma pack()

// datagram scheme
// <VJCPACKETHEADER>,[data buffer]
//

//uControl							uLength		data buffer		responce
#define	VJC_FAILED			0x00//	0			no				no
#define	VJC_OK				0x01//	0			no				no
#define	VJC_CONNECT			0x02//	0			no				VJC_OK/VJC_FAILED
#define	VJC_DISCONNECT		0x03//	0			no				no
#define	VJC_TRANSFER		0x04//	yes			uLength bytes	no

//flags for VJC_CONNECT
#define	VJC_WRITEMODE		0x10// for writing
#define	VJC_READMODE		0x20// for reading
#define	VJC_EXCLUSIVE		0x40// exclusive access
//
// Notes about uControl values and protocol of communication 
// ---------------------------------------------------------
//
// 1.Client-application must establish communication by sending datagram with uControl = VJC_CONNECT 
// and required uDataID value in VJCDATAHEADER. (uDataID is equal with engine's socket port)
//
// 2.Server does(or does not) response on this request. Possible responses are: 
// - server sends datagram with uControl = VJC_OK if communication  was established
// - server sends datagram with uControl = VJC_FAILED if communication was not established
//	
// After this moment client-application will receive all new datas (ulDataID) generated
// by server as datagrams with uControl=VJC_TRANSFER.
//
// 3.Client-application sends his datas (ulDataID) to the server as 
// a datagrams with uControl=VJC_TRANSFER. client must also define his unique ulSenderID value (same as for VJC_CONNECT)
//
// 4.Client must terminate communication session by sending of datagram with uControl = VJC_DISCONNECT 
//
// 5. Device must operate with datalength <= VJC_MAX_PACKETLENGH

#endif