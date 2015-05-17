#include "StdAfx.h"

#include "DMXOutput.h"

#include "ftd2xx.h"

FT_HANDLE handle;
FT_STATUS status;

void write(FT_HANDLE handle, BYTE *data, DWORD length)
{
	if(handle == 0)
		return;

	data[0] = 0; //keep first byte low for dmx

	DWORD bytesWritten = 0;
	FT_STATUS status;
	status = FT_SetBreakOn(handle);
	status = FT_SetBreakOff(handle);
	status = FT_Write(handle, data, length, &bytesWritten);

	if(bytesWritten == 0) //device disconnected?
		handle = 0;
}

UINT WriterThread(LPVOID lp)
{
	ThreadParams *tParams = (ThreadParams *) lp;

	tParams->running = true;

	while(tParams->running)
	{
		write(tParams->Handle, tParams->data, tParams->length);

		Sleep(25);
	}

	return EXIT_SUCCESS;
}

LPCTSTR initMessage = _T("Initializing device...");
LPCSTR connectMessage = _T("Please connect USB->DMX device...");
LPCSTR errorMessage = _T("Error initializing device");
CString deviceMessage = ("Using device 0");
char deviceName[64];

void DMXOutput::initDMX()
{
	if(dmxWriterThread == NULL)
	{
		//start transmitting thread
		tParams.Handle = handle;
		tParams.data = buffer;
		tParams.length = sizeof(buffer);

		dmxWriterThread = AfxBeginThread(WriterThread, &tParams, THREAD_PRIORITY_NORMAL);
	}

	//list devices
	if(handle == 0) //handle should only be initialized once
	{
		Status = initMessage;

		status = 0;

		int deviceIndex = -1;

		FT_STATUS ftStatus;
		DWORD numDevs;
		ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
		if (ftStatus == FT_OK) {

			if(numDevs == 0)
				Status = connectMessage;

			// FT_ListDevices OK, number of devices connected is in numDevs
			for(DWORD devIndex = 0; devIndex < numDevs; devIndex++)
			{
				char Buffer[64]; // more than enough room!
				ftStatus = FT_ListDevices((PVOID) devIndex, Buffer, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
				if (ftStatus == FT_OK) {
					ftStatus = FT_ListDevices((PVOID) devIndex, deviceName, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);

					deviceIndex = devIndex;

					deviceMessage.Format("Using device %s", (LPCTSTR) deviceName); 
					Status = deviceMessage;
				}
			}
		}
		else {
			// FT_ListDevices failed
		}

		if(deviceIndex != -1)
		{
			status = FT_Open(deviceIndex, &handle);

			status = FT_ResetDevice(handle);
			status = FT_SetDivisor(handle, (char)12);  // set baud rate
			status = FT_SetDataCharacteristics(handle, FT_BITS_8, FT_STOP_BITS_2, FT_PARITY_NONE);
			status = FT_SetFlowControl(handle, (char)FT_FLOW_NONE, 0, 0);
			status = FT_ClrRts(handle);
			status = FT_Purge(handle, FT_PURGE_TX);
			status = FT_Purge(handle, FT_PURGE_RX);

			if(status != FT_OK)
			{
				Status = errorMessage;

				FT_Close(handle);
				handle = 0;
			}
			else
			{
				deviceMessage.Format("Device %s up and running...", (LPCTSTR) deviceName);
				Status = deviceMessage;

				tParams.Handle = handle;

				clearDMXChannels();
			}
		}
	}
}

void DMXOutput::setDmxValue( int channel, BYTE value )
{
	if(channel >= 511)
		return;

	if (buffer != NULL)
		buffer[channel] = value;
}

DMXOutput& DMXOutput::getInstance()
{
	static DMXOutput instance;

	return instance;
}

DMXOutput::~DMXOutput()
{
	if(dmxWriterThread != NULL)
	{
		AfxEndThread(EXIT_SUCCESS, true);
		dmxWriterThread = NULL;
	}

	if(handle != NULL)
	{
		clearDMXChannels(0);

		setDmxValue(6, 200);

		writeBufferToDMX();

		FT_Close(handle);
		handle = NULL;
	}
}

void DMXOutput::clearDMXChannels( BYTE val /*= 0x00*/ )
{
	//reset all dmx channels to 0
	memset(buffer, val, sizeof(buffer));
}

void DMXOutput::writeBufferToDMX()
{
	write(tParams.Handle, buffer, sizeof(buffer));
}

DMXOutput::DMXOutput()
{
	done = false;
	bytesWritten = 0;

	handle = NULL;
	dmxWriterThread = NULL;

	memset(buffer, 0, sizeof(buffer));
}