#include "pch.h"
#include "Communication.h"

BOOL SendPipe(LPCWCHAR wcPipeline, LPVOID lpBuffer, DWORD BufferLength)
{
	HANDLE hPipe = CreateNamedPipe(wcPipeline, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, 0, 1, BufferLength, BufferLength, 0, NULL);//���������ܵ�
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//�����¼�
	if (hEvent == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	OVERLAPPED OVD;
	ZeroMemory(&OVD, sizeof(OVERLAPPED));
	OVD.hEvent = hEvent;
	BOOL bRet = ConnectNamedPipe(hPipe, &OVD);//ʹ�����ܵ������������ܹ��ȴ��ͻ��˽������ӵ������ܵ���ʵ��
	if (!bRet)
	{
		if (ERROR_IO_PENDING != GetLastError())
		{
			CloseHandle(hEvent);
			CloseHandle(hPipe);
			return FALSE;
		}
	}
	if (WAIT_FAILED == WaitForSingleObject(hEvent, INFINITE))//�ȴ��¼�
	{
		CloseHandle(hEvent);
		CloseHandle(hPipe);
		return FALSE;
	}

	DWORD dwWritelength;
	bRet = WriteFile(hPipe, lpBuffer, BufferLength, &dwWritelength, NULL);
	if (!bRet)
	{
		CloseHandle(hEvent);
		CloseHandle(hPipe);
		return FALSE;
	}
	return TRUE;
}


BOOL ReceivePipe(LPCWCHAR wcPipeline, LPVOID lpBuffer, DWORD BufferLength)
{
	BOOL bRet = WaitNamedPipe(wcPipeline, NMPWAIT_WAIT_FOREVER);//�ȴ������ܵ�
	if (!bRet)
	{
		return FALSE;
	}
	HANDLE hPipe = CreateFile(wcPipeline, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//�������ܵ�
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	CHAR szReadBuffer[100];
	DWORD dwReadLength = 0;
	bRet = ReadFile(hPipe, &lpBuffer, BufferLength, &dwReadLength, NULL);
	if (!bRet)
	{
		CloseHandle(hPipe);
		return FALSE;
	}
	return TRUE;
}

BOOL CloseSendRam(PSendRamHandle HandleRam)
{
	UnmapViewOfFile(HandleRam->lpMVO);//�ر�ӳ��
	CloseHandle(HandleRam->CFMHabdle);
	return TRUE;
}

SendRamHandle SendRam(LPVOID lpData, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWCHAR lpName)
{
	SendRamHandle HandleRam;
	//memcpy(HandleRam, 0, sizeof(SendRamHandle));
	HandleRam.CFMHabdle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
	if (HandleRam.CFMHabdle == NULL)
	{
		HandleRam = { 0 };
		return HandleRam;
	}
	HandleRam.lpMVO = MapViewOfFile(HandleRam.CFMHabdle, FILE_MAP_ALL_ACCESS, 0, 0, dwMaximumSizeLow);//����ӳ��
	if (HandleRam.lpMVO == NULL)
	{
		HandleRam = { 0 };
		CloseHandle(HandleRam.CFMHabdle);
		return HandleRam;
	}
	CopyMemory(HandleRam.lpMVO, &dwMaximumSizeLow, 4);
	CopyMemory(LPVOID((DWORD)HandleRam.lpMVO+4), lpData, dwMaximumSizeLow+4);//������������
	return HandleRam;
}

BOOL ReceiveRam(LPVOID lpData, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWCHAR lpName)
{
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, lpName);
	if (hMapFile == NULL)
	{
		return FALSE;
	}
	LPVOID lpBuffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, dwMaximumSizeLow);//����ӳ��
	if (lpBuffer == NULL)
	{
		CloseHandle(hMapFile);
		return FALSE;
	}
	DWORD dwSize;
	CopyMemory(&dwSize, lpBuffer, 4);
	memcpy(lpData, (LPVOID)((DWORD)lpBuffer+4), dwSize);
	UnmapViewOfFile(lpBuffer);//�ر�ӳ��
	CloseHandle(hMapFile);
	return TRUE;
}