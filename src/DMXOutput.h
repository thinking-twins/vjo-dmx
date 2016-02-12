#pragma once

#include <string>

struct ThreadParams
{
	typedef PVOID	FT_HANDLE;

	FT_HANDLE Handle;
	BYTE *data;
	DWORD length;
	bool running;
};

class DMXOutput
{

public:

	static DMXOutput& getInstance();

	void initDMX();
	void setDmxValue(int channel, BYTE value);
	void clearDMXChannels(BYTE val = 0x00);

	BYTE *getBuffer() {
		return buffer; }

public:

	LPCTSTR Status;

protected:

	bool running;

	BYTE buffer[511];
	BOOL done;
	INT bytesWritten;

	CWinThread *dmxWriterThread;

	ThreadParams tParams;

private:
        
	DMXOutput();
	~DMXOutput();

  // Dont forget to declare these two. You want to make sure they
  // are unaccessable otherwise you may accidently get copies of
  // your singleton appearing.
  DMXOutput(DMXOutput const&);              // Don't Implement
  void operator=(DMXOutput const&); // Don't implement
	void writeBufferToDMX();
};