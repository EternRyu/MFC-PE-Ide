#pragma once
#include "pch.h"

typedef struct SendRamHandle
{
	HANDLE CFMHabdle;
	LPVOID lpMVO;
}SendRamHandle,* PSendRamHandle;


BOOL SendPipe(LPCWCHAR wcPipeline, LPVOID lpBuffer, DWORD BufferLength);
BOOL ReceivePipe(LPCWCHAR wcPipeline, LPVOID lpBuffer, DWORD BufferLength);

SendRamHandle SendRam(LPVOID lpData, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWCHAR lpName);
BOOL ReceiveRam(LPVOID lpData, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWCHAR lpName);
BOOL CloseSendRam(PSendRamHandle HandleRam);
