// DelayImport.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "DelayImport.h"
#include "afxdialogex.h"


// DelayImport 对话框

IMPLEMENT_DYNAMIC(DelayImport, CDialogEx)

DelayImport::DelayImport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG9, pParent)
{

}

DelayImport::~DelayImport()
{
}

void DelayImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, DelayImport_List);
}


BEGIN_MESSAGE_MAP(DelayImport, CDialogEx)
END_MESSAGE_MAP()

PEFileData DelayData;
// DelayImport 消息处理程序


BOOL DelayImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DelayImport_List.InsertColumn(0, L"DLL_Name", 0, 100);
	DelayImport_List.InsertColumn(1, L"Attributes", 0, 80);
	DelayImport_List.InsertColumn(2, L"ModuleHandleRVA", 0, 80);
	DelayImport_List.InsertColumn(3, L"ImportAddressTableRVA", 0, 80);
	DelayImport_List.InsertColumn(4, L"ImportNameTableRVA", 0, 80);
	DelayImport_List.InsertColumn(4, L"BoundImportAddressTableRVA", 0, 80);
	DelayImport_List.InsertColumn(4, L"UnloadInformationTableRVA", 0, 80);
	DelayImport_List.InsertColumn(4, L"TimeDateStamp", 0, 80);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

VOID DelayImport::InitDelayImport()
{
	LPVOID lpBuffer;
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &DelayData.lpFileBuffer);
	DelayData.dwPlatform = Image32Or64(DelayData.lpFileBuffer);

	PIMAGE_DELAYLOAD_DESCRIPTOR Image_DelayTbale = NULL;
	PIMAGE_DOS_HEADER pImage_Dos = (PIMAGE_DOS_HEADER)DelayData.lpFileBuffer;
	if (DelayData.dwPlatform == IMAGE_FILE_MACHINE_I386)
	{
		PIMAGE_NT_HEADERS pImage_NT = (PIMAGE_NT_HEADERS)((DWORD)DelayData.lpFileBuffer + pImage_Dos->e_lfanew);
		//定位数据目录表中的TLS结构
		PIMAGE_DATA_DIRECTORY pImage_Data = (PIMAGE_DATA_DIRECTORY)(pImage_NT->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
		Image_DelayTbale = (PIMAGE_DELAYLOAD_DESCRIPTOR)(RvaToOffset32(pImage_Data->VirtualAddress, (PCHAR)DelayData.lpFileBuffer) + (PCHAR)DelayData.lpFileBuffer);
	}
	else
	{
		if (DelayData.dwPlatform == IMAGE_FILE_MACHINE_IA64)
		{
			PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)((DWORD)DelayData.lpFileBuffer + pImage_Dos->e_lfanew);
			//定位数据目录表中的TLS结构
			PIMAGE_DATA_DIRECTORY pImage_Data = (PIMAGE_DATA_DIRECTORY)(pImage_NT->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
			Image_DelayTbale = (PIMAGE_DELAYLOAD_DESCRIPTOR)(RvaToOffset64(pImage_Data->VirtualAddress, (PCHAR)DelayData.lpFileBuffer) + (PCHAR)DelayData.lpFileBuffer);
		}
	}
	if (Image_DelayTbale == NULL)
	{
		return;
	}
	ULONG uIndex = 0;
	while (Image_DelayTbale->DllNameRVA != NULL)
	{
		PCHAR szDllName = NULL;
		if (DelayData.dwPlatform == IMAGE_FILE_MACHINE_I386)
		{
			szDllName = (RvaToOffset32(Image_DelayTbale->DllNameRVA, (PCHAR)DelayData.lpFileBuffer) + (PCHAR)DelayData.lpFileBuffer);
		}
		else
		{
			szDllName = (RvaToOffset64(Image_DelayTbale->DllNameRVA, (PCHAR)DelayData.lpFileBuffer) + (PCHAR)DelayData.lpFileBuffer);
		}
		DWORD dwSize = (sizeof(szDllName) + 1) * 2;
		PWCHAR  wcDllName = (PWCHAR)malloc(dwSize);
		swprintf(wcDllName, dwSize, L"%hs", szDllName);
		DelayImport_List.InsertItem(uIndex, wcDllName);
		free(wcDllName);

		WCHAR wcTLSValue[26];
		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->Attributes);
		DelayImport_List.SetItemText(uIndex, 1, wcTLSValue);

		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->ModuleHandleRVA);
		DelayImport_List.SetItemText(uIndex, 2, wcTLSValue);

		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->ImportAddressTableRVA);
		DelayImport_List.SetItemText(uIndex, 3, wcTLSValue);

		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->ImportNameTableRVA);
		DelayImport_List.SetItemText(uIndex, 4, wcTLSValue);

		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->BoundImportAddressTableRVA);
		DelayImport_List.SetItemText(uIndex, 5, wcTLSValue);

		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->UnloadInformationTableRVA);
		DelayImport_List.SetItemText(uIndex, 6, wcTLSValue);

		wsprintf(wcTLSValue, L"0x%X", Image_DelayTbale->TimeDateStamp);
		DelayImport_List.SetItemText(uIndex, 7, wcTLSValue);

		uIndex++;
		Image_DelayTbale++;
	}

}
