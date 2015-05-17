// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2000 BY GREEN HIPPO 
//
// File:		mdidata.h
// Project:		vje32
//
// Contents:	InterfaceStructure for MidiSupport
//
//
//
// Author:		Yannick Suter
// Started:		11.04.00 13:28:04
//
// Version:		1.00

#ifndef MIDIDATA_DEF
#define MIDIDATA_DEF

typedef	struct
{
	DWORD	dwStructSize;

	BOOL    bIsDrum;
	CHAR	nVelocity[256];
	CHAR	nPressure;
	int     nPitchWheel;
	int     nLastKey;

	CHAR	nControls[256];
	CHAR	nKey[256];
	int		nKeyCounter[256];

	int		nLastControlsValue;

} MIDIDATAHDR;

#define NUM_MIDI_CHANNELS 16

typedef struct
{
	MIDIDATAHDR Channel[NUM_MIDI_CHANNELS];
	float fSpeed;
} MIDIDATAHEADER;

#endif