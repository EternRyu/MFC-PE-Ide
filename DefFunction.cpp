#pragma once
#include "pch.h"
#include "DefFunction.h"
#include "PEID.h"


BOOL PE_DOS(const PCHAR FilePath,LPVOID* lpBuffer)
{
	FILE* pFile;
	BOOL bRet = fopen_s(&pFile, FilePath, "rb");
	if (bRet)
	{
		return FALSE;
	}
	fseek(pFile, 0, SEEK_END);
	INT nFileLength = ftell(pFile);
	rewind(pFile);
	INT ImageLength = nFileLength * sizeof(char) + 1;
	PCHAR pBuffer = (PCHAR)malloc(ImageLength);
	*lpBuffer = (LPVOID)malloc(ImageLength);
	memset(pBuffer, 0, ImageLength);
	fread(pBuffer, 1, ImageLength, pFile);
	CopyMemory(*lpBuffer, pBuffer, ImageLength);
	free(pBuffer);
	fclose(pFile);
	return TRUE;
}

DWORD Image32Or64(LPVOID Image)
{
	if (Image==NULL)
	{
		return NULL;
	}
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)Image;
	PIMAGE_NT_HEADERS ImageNtUnknown = (PIMAGE_NT_HEADERS)((DWORD)Image + pImageDosHeader->e_lfanew);
	if (ImageNtUnknown->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
	{
		return IMAGE_FILE_MACHINE_I386;
	}
	if (ImageNtUnknown->FileHeader.Machine == IMAGE_FILE_MACHINE_IA64 || ImageNtUnknown->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
	{
		return IMAGE_FILE_MACHINE_IA64;
	}
	return NULL;
}


BOOL PEWrite(const PCHAR FilePath, LPVOID lpBuffer)
{
	FILE* pFile;

	BOOL bRet = fopen_s(&pFile, FilePath, "rb+");
	if (bRet)
	{
		return FALSE;
	}
	fseek(pFile, 0, SEEK_END);
	INT nFileLength = ftell(pFile);
	INT ImageLength = nFileLength * sizeof(char);
	fclose(pFile);
	fopen_s(&pFile, FilePath, "wb+");
	fwrite(lpBuffer, 1, ImageLength, pFile);
	fclose(pFile);
	return TRUE;
}


//x86
VOID PEID::InitlizeNtHerder32(LPVOID lpBuffer32)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer32;
	PIMAGE_NT_HEADERS32 ImageNt32 = (PIMAGE_NT_HEADERS32)((DWORD)lpBuffer32 + pImageDosHeader->e_lfanew);
	CSt_Entry.Format(TEXT("%X"), ImageNt32->OptionalHeader.AddressOfEntryPoint);
	CSt_Base.Format(TEXT("%X"), ImageNt32->OptionalHeader.ImageBase);
	CSt_ImageSize.Format(TEXT("%X"), ImageNt32->OptionalHeader.SizeOfImage);
	CSt_CodeEntry.Format(TEXT("%X"), ImageNt32->OptionalHeader.BaseOfCode);
	CSt_DataEntry.Format(TEXT("%X"), ImageNt32->OptionalHeader.BaseOfData);
	CSt_Section.Format(TEXT("%X"), ImageNt32->OptionalHeader.SectionAlignment);
	CSt_FileAlig.Format(TEXT("%X"), ImageNt32->OptionalHeader.FileAlignment);
	CSt_Magic.Format(TEXT("%X"), ImageNt32->OptionalHeader.Magic);
	CSt_Version.Format(TEXT("%X"), ImageNt32->OptionalHeader.MinorSubsystemVersion);
	CSt_SectionNumer.Format(TEXT("%X"), ImageNt32->FileHeader.NumberOfSections);
	CSt_FileTime.Format(TEXT("%X"), ImageNt32->FileHeader.TimeDateStamp);
	CSt_HeaderSize.Format(TEXT("%X"), ImageNt32->OptionalHeader.SizeOfHeaders);
	CSt_Characteristics.Format(TEXT("%X"), ImageNt32->FileHeader.Characteristics);
	CSt_CheckSum.Format(TEXT("%X"), ImageNt32->OptionalHeader.CheckSum);
	CSt_szOptionalHeader.Format(TEXT("%X"), ImageNt32->FileHeader.SizeOfOptionalHeader);
	CSt_RvaSizeNumer.Format(TEXT("%X"), ImageNt32->OptionalHeader.NumberOfRvaAndSizes);
	//DWORD Temp = _tcstoul(CSt_Entry,NULL,16);
	UpdateData(FALSE);
}

VOID PEID::SaveNtHeader32(LPVOID lpBuffer32)
{
	UpdateData(TRUE);
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer32;
	PIMAGE_NT_HEADERS32 ImageNt32 = (PIMAGE_NT_HEADERS32)((DWORD)lpBuffer32 + pImageDosHeader->e_lfanew);
	ImageNt32->OptionalHeader.AddressOfEntryPoint = _tcstoul(CSt_Entry, NULL, 16);
	ImageNt32->OptionalHeader.ImageBase = _tcstoul(CSt_Base, NULL, 16);
	ImageNt32->OptionalHeader.SizeOfImage = _tcstoul(CSt_ImageSize, NULL, 16);
	ImageNt32->OptionalHeader.BaseOfCode = _tcstoul(CSt_CodeEntry, NULL, 16);
	ImageNt32->OptionalHeader.BaseOfData = _tcstoul(CSt_DataEntry, NULL, 16);
	ImageNt32->OptionalHeader.SectionAlignment = _tcstoul(CSt_Section, NULL, 16);
	ImageNt32->OptionalHeader.FileAlignment = _tcstoul(CSt_FileAlig, NULL, 16);
	ImageNt32->OptionalHeader.Magic = _tcstoul(CSt_Magic, NULL, 16);
	ImageNt32->OptionalHeader.MinorSubsystemVersion = _tcstoul(CSt_Version, NULL, 16);
	ImageNt32->FileHeader.NumberOfSections = _tcstoul(CSt_SectionNumer, NULL, 16);
	ImageNt32->FileHeader.TimeDateStamp = _tcstoul(CSt_FileTime, NULL, 16);
	ImageNt32->OptionalHeader.SizeOfHeaders = _tcstoul(CSt_HeaderSize, NULL, 16);
	ImageNt32->FileHeader.Characteristics = _tcstoul(CSt_Characteristics, NULL, 16);
	ImageNt32->OptionalHeader.CheckSum = _tcstoul(CSt_CheckSum, NULL, 16);
	ImageNt32->FileHeader.SizeOfOptionalHeader = _tcstoul(CSt_szOptionalHeader, NULL, 16);
	ImageNt32->OptionalHeader.NumberOfRvaAndSizes = _tcstoul(CSt_RvaSizeNumer, NULL, 16);

	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PEWrite(FileName, lpBuffer32);
}

//x86格式 x64需要ULONGULONG 
DWORD RvaToOffset32
(
	DWORD dwRva, //数据目录表的Virtual Address
	PCHAR pBuffer //读取到的PE文件缓冲区
)
{
	PIMAGE_DOS_HEADER pImgae_Dos = (PIMAGE_DOS_HEADER)pBuffer; //Dos Herder
	PIMAGE_NT_HEADERS pImage_NT = (PIMAGE_NT_HEADERS)(pBuffer + pImgae_Dos->e_lfanew); //PE Herder
	PIMAGE_SECTION_HEADER pImage_Section = IMAGE_FIRST_SECTION(pImage_NT); //区段表
	if (dwRva < pImage_Section[0].VirtualAddress)
	{
		return dwRva;
	}
	for (int i = 0; i < pImage_NT->FileHeader.NumberOfSections; i++)
	{
		if (dwRva >= pImage_Section[i].VirtualAddress && dwRva <= pImage_Section[i].VirtualAddress + pImage_Section[i].Misc.VirtualSize)
		{
			//dwRva - pImage_Section[i].VirtualAddress  数据目录表基址到区段目录表基址的偏移
			//pImage_Section[i].PointerToRawData 区段到文件头的偏移

			return dwRva - pImage_Section[i].VirtualAddress + pImage_Section[i].PointerToRawData; //数据目录表RVA
		}
	}
	return 0;
}

DWORD FileOffset32
(
	DWORD dwRva, //数据目录表的Virtual Address
	PCHAR pBuffer //读取到的PE文件缓冲区
)
{
	PIMAGE_DOS_HEADER pImgae_Dos = (PIMAGE_DOS_HEADER)pBuffer; //Dos Herder
	PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)(pBuffer + pImgae_Dos->e_lfanew); //PE Herder
	PIMAGE_SECTION_HEADER pImage_Section = IMAGE_FIRST_SECTION(pImage_NT); //区段表
	if (dwRva < pImage_Section[0].VirtualAddress)
	{
		return 0;
	}
	for (int i = 0; i < pImage_NT->FileHeader.NumberOfSections; i++)
	{
		if (dwRva >= pImage_Section[i].VirtualAddress && dwRva <= pImage_Section[i].VirtualAddress + pImage_Section[i].Misc.VirtualSize)
		{
			//dwRva - pImage_Section[i].VirtualAddress  数据目录表基址到区段目录表基址的偏移
			//pImage_Section[i].PointerToRawData 区段到文件头的偏移

			return pImage_Section[i].VirtualAddress - pImage_Section[i].PointerToRawData; //数据目录表RVA
		}
	}
}

//---------------------------------------------


//x64
VOID PEID::InitlizeNtHerder64(LPVOID lpBuffer64)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer64;
	PIMAGE_NT_HEADERS64 ImageNt64 = (PIMAGE_NT_HEADERS64)((DWORD)lpBuffer64 + pImageDosHeader->e_lfanew);
	CSt_Entry.Format(TEXT("%X"), ImageNt64->OptionalHeader.AddressOfEntryPoint);
	CSt_Base.Format(TEXT("%X"), ImageNt64->OptionalHeader.ImageBase);
	CSt_ImageSize.Format(TEXT("%X"), ImageNt64->OptionalHeader.SizeOfImage);
	CSt_CodeEntry.Format(TEXT("%X"), ImageNt64->OptionalHeader.BaseOfCode);
	CSt_DataEntry.Format(TEXT("%X"), ImageNt64->OptionalHeader.ImageBase);
	CSt_Section.Format(TEXT("%X"), ImageNt64->OptionalHeader.SectionAlignment);
	CSt_FileAlig.Format(TEXT("%X"), ImageNt64->OptionalHeader.FileAlignment);
	CSt_Magic.Format(TEXT("%X"), ImageNt64->OptionalHeader.Magic);
	CSt_Version.Format(TEXT("%X"), ImageNt64->OptionalHeader.MinorSubsystemVersion);
	CSt_SectionNumer.Format(TEXT("%X"), ImageNt64->FileHeader.NumberOfSections);
	CSt_FileTime.Format(TEXT("%X"), ImageNt64->FileHeader.TimeDateStamp);
	CSt_HeaderSize.Format(TEXT("%X"), ImageNt64->OptionalHeader.SizeOfHeaders);
	CSt_Characteristics.Format(TEXT("%X"), ImageNt64->FileHeader.Characteristics);
	CSt_CheckSum.Format(TEXT("%X"), ImageNt64->OptionalHeader.CheckSum);
	CSt_szOptionalHeader.Format(TEXT("%X"), ImageNt64->FileHeader.SizeOfOptionalHeader);
	CSt_RvaSizeNumer.Format(TEXT("%X"), ImageNt64->OptionalHeader.NumberOfRvaAndSizes);
	UpdateData(FALSE);
}

VOID PEID::SaveNtHeader64(LPVOID lpBuffer64)
{
	UpdateData(TRUE);
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer64;
	PIMAGE_NT_HEADERS64 ImageNt64 = (PIMAGE_NT_HEADERS64)((DWORD)lpBuffer64 + pImageDosHeader->e_lfanew);
	ImageNt64->OptionalHeader.AddressOfEntryPoint = _tcstoul(CSt_Entry, NULL, 16);
	ImageNt64->OptionalHeader.ImageBase = _tcstoul(CSt_Base, NULL, 16);
	ImageNt64->OptionalHeader.SizeOfImage = _tcstoul(CSt_ImageSize, NULL, 16);
	ImageNt64->OptionalHeader.BaseOfCode = _tcstoul(CSt_CodeEntry, NULL, 16);
	//ImageNt64->OptionalHeader.ImageBase = _tcstoul(CSt_DataEntry, NULL, 16);
	ImageNt64->OptionalHeader.SectionAlignment = _tcstoul(CSt_Section, NULL, 16);
	ImageNt64->OptionalHeader.FileAlignment = _tcstoul(CSt_FileAlig, NULL, 16);
	ImageNt64->OptionalHeader.Magic = _tcstoul(CSt_Magic, NULL, 16);
	ImageNt64->OptionalHeader.MinorSubsystemVersion = _tcstoul(CSt_Version, NULL, 16);
	ImageNt64->FileHeader.NumberOfSections = _tcstoul(CSt_SectionNumer, NULL, 16);
	ImageNt64->FileHeader.TimeDateStamp = _tcstoul(CSt_FileTime, NULL, 16);
	ImageNt64->OptionalHeader.SizeOfHeaders = _tcstoul(CSt_HeaderSize, NULL, 16);
	ImageNt64->FileHeader.Characteristics = _tcstoul(CSt_Characteristics, NULL, 16);
	ImageNt64->OptionalHeader.CheckSum = _tcstoul(CSt_CheckSum, NULL, 16);
	ImageNt64->FileHeader.SizeOfOptionalHeader = _tcstoul(CSt_szOptionalHeader, NULL, 16);
	ImageNt64->OptionalHeader.NumberOfRvaAndSizes = _tcstoul(CSt_RvaSizeNumer, NULL, 16);
	
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PEWrite(FileName, lpBuffer64);
}


ULONGLONG RvaToOffset64
(
	ULONGLONG dwRva, //数据目录表的Virtual Address
	PCHAR pBuffer //读取到的PE文件缓冲区
)
{
	PIMAGE_DOS_HEADER pImgae_Dos = (PIMAGE_DOS_HEADER)pBuffer; //Dos Herder
	PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)(pBuffer + pImgae_Dos->e_lfanew); //PE Herder
	PIMAGE_SECTION_HEADER pImage_Section = IMAGE_FIRST_SECTION(pImage_NT); //区段表
	if (dwRva < pImage_Section[0].VirtualAddress)
	{
		return dwRva;
	}
	for (int i = 0; i < pImage_NT->FileHeader.NumberOfSections; i++)
	{
		if (dwRva >= pImage_Section[i].VirtualAddress && dwRva <= pImage_Section[i].VirtualAddress + pImage_Section[i].Misc.VirtualSize)
		{
			//dwRva - pImage_Section[i].VirtualAddress  数据目录表基址到区段目录表基址的偏移
			//pImage_Section[i].PointerToRawData 区段到文件头的偏移

			return dwRva - pImage_Section[i].VirtualAddress + pImage_Section[i].PointerToRawData; //数据目录表RVA
		}
	}
	return 0;
}

ULONGLONG FileOffset64
(
	ULONGLONG dwRva, //数据目录表的Virtual Address
	PCHAR pBuffer //读取到的PE文件缓冲区
)
{
	PIMAGE_DOS_HEADER pImgae_Dos = (PIMAGE_DOS_HEADER)pBuffer; //Dos Herder
	PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)(pBuffer + pImgae_Dos->e_lfanew); //PE Herder
	PIMAGE_SECTION_HEADER pImage_Section = IMAGE_FIRST_SECTION(pImage_NT); //区段表
	if (dwRva < pImage_Section[0].VirtualAddress)
	{
		return 0;
	}
	for (int i = 0; i < pImage_NT->FileHeader.NumberOfSections; i++)
	{
		if (dwRva >= pImage_Section[i].VirtualAddress && dwRva <= pImage_Section[i].VirtualAddress + pImage_Section[i].Misc.VirtualSize)
		{
			//dwRva - pImage_Section[i].VirtualAddress  数据目录表基址到区段目录表基址的偏移
			//pImage_Section[i].PointerToRawData 区段到文件头的偏移

			return pImage_Section[i].VirtualAddress - pImage_Section[i].PointerToRawData; //数据目录表RVA
		}
	}
	return 0;
}