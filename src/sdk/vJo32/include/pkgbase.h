// THIS FILE IS PART OF THE VJO, COPYRIGHT (C) 2001 BY GREEN HIPPO 
//
// File:		pkgbase.h
// Project:		vje32
//
// Contents:	
//
//
//
// Author:		Vadim Gorbatenko
// Started:		13.09.01 17:53:16
//
// Version:		

#ifndef	PKGBASE_DEF
#define	PKGBASE_DEF

#ifndef __VJO_DEFINES__
typedef	__int64 EFFECTUID;
#endif

#include <io.h>


#define	PKGBMAGIC	0x732A

typedef struct
{
	DWORD	m_dwMagic;
	DWORD	m_dwReserved1;
	DWORD	m_dwReserved2;
	DWORD	m_dwReserved3;
} PKGBHDR;

typedef	struct 
{
	char		m_FXName[128];
	char		m_FXVers[8];
	EFFECTUID	m_UID;
	BOOL		m_bEnableUse;
	BOOL		m_bHide;
	DWORD		m_dwReserved;
	
} PKGBRECORD;


class	CPKGBase
{
public:

	CPKGBase()	{m_nMaxSize = 0; m_nSize = 0; m_pBuffer = NULL;}
	~CPKGBase()	{delete m_pBuffer;}

	BOOL	Load(LPCSTR lpPath);
	BOOL	Create(LPCSTR lpPath, int nMaxSz);
	BOOL	Save();

inline	void	Erase()		{m_nSize = 0;}	
inline	INT		GetSize()	{return m_nSize;}
	
inline	PKGBRECORD	*GetRecord(int i)	{return &m_pBuffer[i];}
	
inline	LPCSTR GetFXName(int i)		{return m_pBuffer[i].m_FXName;}
inline	LPCSTR GetFXVers(int i)		{return m_pBuffer[i].m_FXVers;}
inline	BOOL   IsFXLoadable(int i)	{return m_pBuffer[i].m_bEnableUse;}
inline	BOOL   IsFXVisible(int i)	{return !m_pBuffer[i].m_bHide;}

	INT		FindRecord(EFFECTUID uid);
	PKGBRECORD	*AddRecord(LPCSTR lpName, EFFECTUID uid, LPCSTR lpVersion);
	
protected:
	CString		m_Name;
	INT			m_nSize;
	INT			m_nMaxSize;
	PKGBRECORD	*m_pBuffer;
};

void doQsPKGBR(PKGBRECORD	*item, int left, int right)
{
	register i,j;
	PKGBRECORD	x, y;
	i=left;	j=right;
	x=item[(left+right)/2];

	do {
		while(item[i].m_UID < x.m_UID && i<right)	i++;
		while(x.m_UID < item[j].m_UID && j>left)	j--;

		if(i<=j) {
			memcpy(&y, item+i, sizeof(y));
			memcpy(item+i, item+j, sizeof(y));
			memcpy(item+j, &y, sizeof(y));
			item[j]	= y;
			i++; j--;
			}
	} while (i<=j);
	if(left < j)	doQsPKGBR(item, left, j);
	if(i < right)	doQsPKGBR(item, i, right);
}

int binSearchPKGBR(PKGBRECORD *item, int count, EFFECTUID key)
{
	if(count<1)	return -1;
	if(count == 1)	return key==item[0].m_UID?0:-1;

	int low=0, mid, high=count-1;
	while(low <= high)
	{
		mid=(low+high)/2;
		if(key<item[mid].m_UID)
		{
			high=mid-1;
		} else if(item[mid].m_UID<key)
		{
			low=mid+1;
		} else
		{
			return mid;
		}
	}
	return -1;
}

BOOL	CPKGBase::Load(LPCSTR lpPath)
{
	delete m_pBuffer;
	m_pBuffer = NULL;
	m_Name = lpPath;
	m_nSize = m_nMaxSize =0;
	FILE *f = fopen(lpPath, "rb");
	if(!f)	return FALSE;
	int len = _filelength(fileno(f));
	if( len < (sizeof(PKGBHDR)+sizeof(PKGBRECORD)))
	{
		fclose(f); return FALSE;
	}
	PKGBHDR hdr;
	if(!fread(&hdr, sizeof(PKGBHDR), 1, f) || hdr.m_dwMagic!=PKGBMAGIC)
	{
		fclose(f); return FALSE;
	}
	len-=sizeof(PKGBHDR);
	m_nMaxSize = m_nSize =  len/sizeof(PKGBRECORD);
	m_pBuffer = new PKGBRECORD[m_nSize];
	if(fread(m_pBuffer, 1, len, f)!=(size_t)len)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
		m_nSize = m_nMaxSize =0;
		fclose(f); return FALSE;
	}
	fclose(f); return TRUE;
}

BOOL	CPKGBase::Create(LPCSTR lpPath, int nMaxSz)
{
		delete m_pBuffer;
		m_pBuffer = new PKGBRECORD[nMaxSz];
		memset(m_pBuffer, 0, nMaxSz*sizeof(PKGBRECORD));
		m_nSize = 0;
		m_nMaxSize = nMaxSz;
		m_Name = lpPath;
		return TRUE;
}

BOOL	CPKGBase::Save()
{
	if(!m_nSize || !m_pBuffer)
		return TRUE;
	//sort
	doQsPKGBR(m_pBuffer, 0, m_nSize-1);
	FILE *f = fopen(m_Name, "wb");
	if(!f)	return FALSE;

	PKGBHDR hdr;
	memset(&hdr, 0, sizeof hdr);
	hdr.m_dwMagic=PKGBMAGIC;
	fwrite(&hdr, sizeof(PKGBHDR), 1, f);
	fwrite(m_pBuffer, 1, m_nSize*sizeof(PKGBRECORD), f);
	fclose(f);
	return TRUE;
}	

int CPKGBase::FindRecord(EFFECTUID uid)
{
	return  binSearchPKGBR(m_pBuffer, m_nSize, uid);
}

PKGBRECORD	*CPKGBase::AddRecord(LPCSTR lpName, EFFECTUID uid, LPCSTR lpVersion)
{
	if(m_nSize == m_nMaxSize)
		return NULL;
	m_pBuffer[m_nSize].m_bEnableUse = 1;
	m_pBuffer[m_nSize].m_bHide = 0;
	strcpy(m_pBuffer[m_nSize].m_FXName, lpName);
	strcpy(m_pBuffer[m_nSize].m_FXVers, lpVersion);
	m_pBuffer[m_nSize].m_UID = uid;
	m_pBuffer[m_nSize++].m_dwReserved =0;
	return GetRecord(m_nSize-1);
}

#endif