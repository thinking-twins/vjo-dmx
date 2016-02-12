//SSDATA.H
//
// Sound System data header
// 
// originally designed for vJo


/************************************
  Author:	   Vadim Gorbatenko
  Started on 26.12.99 14:31:47
  Revised on 26.12.99 14:31:47
  Revised on 27.12.99 03:00:47
  Revised on 31.12.99 2:08:29
  Revised on 02.01.00 15:28:46
  Revised on 07.01.00 13:27:32
  Revised on 14.01.00 13:48:14 	(++SSD_AMPL_SYNCRO && SSD_AMPL_INSTANT)
  Revised on 15.01.00 16:40:09	(SOOBJECT members changed to float)
  Revised on 16.01.00 12:38:43
  Revised on 19.01.00 21:56:49	(MUSPARAM changed, space group changed,SOOBJECT members changed)
  Revised on 31.01.00 0:33:09	History staff removed!
  Revised on 11.02.00 1:10:33	final revision for version 1.00
  Revised on 01.05.00 21:06:12	SSD_BASSBEATPHASE added
  Comments: version	1.00
 ************************************/


#ifndef	SSDATA_DEF

#define	SSDATA_DEF

#ifndef	_WIN32
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef int                 INT;
typedef unsigned char       BYTE;
typedef char				CHAR;
typedef unsigned short      WORD;
typedef short				SHORT;
typedef float               FLOAT;
typedef unsigned int        UINT;
typedef DWORD far           *LPDWORD;
typedef FLOAT               *LPFLOAT;
#define VOID void
#endif

#define	CHANNEL_LEFT	0
#define	CHANNEL_RIGHT	1
#define	CHANNEL_ALL		-1

#define	UNKNOWN_POSITION	-1
#define	UNKNOWN_RECORD		-1
#define	UNKNOWN_LENGTH		-1
#define	UNKNOWN_DRIVE		-1



#pragma pack(1)

typedef	struct
{
	DWORD	dwId;		//GUID like
	FLOAT	fCenter;	//front position
	FLOAT	fPower;		//relative amplitude
	FLOAT	fFrequency;	//absolute frequency
} SOOBJECT;				//space-oriented object;

//linked list element definition
typedef struct
{
	DWORD	dwParamID;
	union	{/*value*/
		FLOAT			fValue;
		LONG			iValue;
		SOOBJECT		pSooValue;
		CHAR			pValue[16];
	};
	INT		iChannel;
	VOID	*lpNextParam;
}	MUSPARAM;


typedef struct
{
	INT		iRecordNo;			//#
	INT		dwRecordLenght;		//ms
	INT		dwCurrentRecordPos;	//ms
	CHAR	pRecordTitle[128];
}	MUSICRECORDINFO;

typedef struct
{
	CHAR	pMediaTitle[256];	//ie filepath or CD path
	CHAR	pMediaType[32];		//ie "CD","FILE","WAVEIN" itc
	MUSICRECORDINFO	pRecordInfo;
}	MUSICMEDIAINFO;

typedef struct
{
	//sound source info. All fields==0 if no processing done
	INT		iSourceSamplingRate;	//samples/sec(ie Hz)
	INT		iSourceChannelsCount;	//1 or 2
	INT		iSourceBitsPerSample;	//8 or 16 or ??
	
	//raw samples for extern DSP&analyser
	INT		iSamplesCount;
	BYTE	*lpRawSamplesBuffer;	//global ptr

} WAVRAWDATA;

typedef struct
{
	DWORD	dwDefaultHistoryGranularity;	//msec
	INT		iFFTCount;			//fft power array size 
	FLOAT	*lpFFTHz;			//fft frequencyes (global ptr)
	FLOAT	*lpFFTArray;		//size = iFiltersCount*wSourceChannelsCount
} DSPRAWDATA;

typedef struct
{
	WORD	wStructSize;	//versioning
	DWORD	dwTime;
	//Analyzer info
	CHAR	pAnalyzerInfo[128];
	DWORD	dwAnalyzerFlags;	//see flags below


	//general music source info
	MUSICMEDIAINFO	pMusMediaInfo;
	
	//sound raw data
	WAVRAWDATA	pWavRawData;

	//dsp raw data
	DSPRAWDATA	pDSPRawData;

	//analysis results(linked list)
	MUSPARAM	*lpMusParams;
	
	VOID		*lpNextDataHeader;//pointer to next data header

	VOID		*lpReservedPtr;	//
	//reserved
	BYTE		reserved[64];
}	SSDATAHEADER;

#pragma pack()

// Valid flags for dwAnalyzerFlags(any combination)
#define	PGROUP_BEATLINE		0x00000001	//beat-line recognition enabled
#define	PGROUP_SPACE		0x00000002	//space orientation processing enabled
#define	PGROUP_AMPLITUDE	0x00000004	//amplitude analysis enabled
#define	PGROUP_SPECTRUM		0x00000008	//spectrum analysis enabled
#define	PGROUP_PSYCHO		0x00000010	//psycho-analysis enabled
#define	PGROUP_CONTEXT		0x00000020	//context analysis enabled
#define	DSP_TONECOMP		0x10000000	// tone compensation on

//
// Music parameters definition( wParamID in MUSPARAM)
// QUICK parameters marked with '*'
// 
//parameter id								Value					Note
		//beat-line group
#define	SSD_BASSBEAT				1	//	long, period, msec		(for all channels)
#define	SSD_MIDDLEBEAT				2	//	long, period, msec		(for all channels)
#define	SSD_HIGHBEAT				3	//	long, period, msec		(for all channels)
#define	SSD_BASSBEATPHASE			4	//	float, 0-6.28			(for all channels)

		//space group  
#define	SSD_SCENEWIDTH				20	//	0-1.0(max)				(for all channels)
#define	SSD_SCENEDEPTH				21	//	0-1.0(max)				(for all channels)
#define	SSD_SOURCEHEIGHT			22	//*	0-1.0(max)				(for all channels)
#define	SSD_SOURCECENTER			23	//*	-1.0(L) to 1.0(R)		(for all channels)
#define	SSD_SOURCEPOWER				24	//*	0-1.0(max)				(for all channels)

#define	SSD_SOUNDSPARK				26	//SOOBJECT

		//amplitude group
#define	SSD_AMPL_AVERAGE			40	//	0-1.0(max)				iChannel = channel
#define	SSD_AMPL_INSTANT			41	//*	0-1.0(max)				iChannel = channel
#define	SSD_AMPL_GAIN_TENDENCY		42	//	-1.0 to +1.0f			(for all channels)
#define	SSD_AMPL_SILENT_STATE		44	//	not used				(for all channels)

		//spectrum group
#define	SSD_BASS					50	//*	0-1.0(max)				iChannel = channel
#define	SSD_MIDDLE					51	//*	0-1.0(max)				iChannel = channel
#define	SSD_HIGH					52	//*	0-1.0(max)				iChannel = channel
#define	SSD_SYNCRO					53	//*	not used				(for all channels)
#define	SSD_BRIGHTNESS				54	//*	0-1.0(max)				(for all channels)
#define	SSD_ACCENT					56	//*	0-1.0(max)				(for all channels)

		//psycho-analysis group - kind of magic
#define	SSD_PSYCHOTYPE				70	//pParam = (PTYPE_XXX)
#define	SSD_MUSICSTYLE				71	//pParam = (MSTYLE_XXX)
		//context analysis
#define	SSD_SOUNDCONTEXTCHANGED		80	//not used					(for all channels)
#define	SSD_MUSICCONTEXTCHANGED		81	//not used					(for all channels)

#define	SSD_USER_PARAM				0x1000

		//PSYCHOTYPE's definition by NEMA:-)
#define	PTYPE_AGGRESSION	"aggression"
#define	PTYPE_ROMANTICS		"romantics"
#define	PTYPE_SADNESS		"sadness"
#define	PTYPE_HAPPYNESS		"happyness"
		//PSYCHOTYPE's definition by NASH:-)
#define	PTYPE_SOLITUDE		"solitude"
#define	PTYPE_RAGE			"rage"
#define	PTYPE_SPEED			"speed"
#define	PTYPE_MADNESS		"madness"
#define	PTYPE_MELANCHOLY	"melancholy"
#define	PTYPE_FLYING		"flying"
#define	PTYPE_FUN			"fun"
#define	PTYPE_FUSS			"fuss"
#define	PTYPE_FANTASY		"fantasy"
#define	PTYPE_SPACE			"space"
#define	PTYPE_ANY			"unknown"
	
		//MUSICSTYLE definitions
#define	MSTYLE_ROCK			"rock"
#define	MSTYLE_HMETALL		"heavy metall"
#define	MSTYLE_JAZZ			"jazz"
#define	MSTYLE_TECHNO		"techno"
#define	MSTYLE_ACOUSTIC		"acoustic"
#define	MSTYLE_DISCO		"disco"
#define	MSTYLE_SYMPHONYC	"symphonic"
#define	MSTYLE_ANY			"music"


//ssystem error codes
#define	SK_NOERROR				0
#define	SK_NOINPUTDEVICE		1
#define	SK_NOTIMPLEMENTED		2
#define	SK_CANTOPENDEVICE		3
#define	SK_UNSUPPORTEDFORMAT	4
#define	SK_INVALIDMEDIA			5
#define	SK_INVALIDPARAMETER		6
#define	SK_NOTREADY				7
#define	SK_ILLEGALCALL			8
#define	SK_ERROR				9
// final note:
// additional notes about musparams value see at SSDATA.TXT or SSDATA.HTM
/////////////////////////////////////////////////////////// 

#endif