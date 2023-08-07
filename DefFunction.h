#pragma once
#include "pch.h"

typedef struct PEFileData
{
	DWORD dwPlatform;
	LPVOID lpFileBuffer;
}PEFileData,* PEFileData_PTR;


BOOL PE_DOS(const PCHAR FilePath, LPVOID* lpBuffer);
DWORD Image32Or64(LPVOID Image);
BOOL PEWrite(const PCHAR FilePath, LPVOID lpBuffer);
DWORD RvaToOffset32(DWORD dwRva, PCHAR pBuffer);
ULONGLONG RvaToOffset64(ULONGLONG dwRva, PCHAR pBuffer);
ULONGLONG FileOffset64(ULONGLONG dwRva, PCHAR pBuffer);
DWORD FileOffset32(DWORD dwRva, PCHAR pBuffer);