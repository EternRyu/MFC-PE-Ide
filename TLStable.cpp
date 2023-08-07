// TLStable.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "TLStable.h"
#include "afxdialogex.h"


// TLStable 对话框

IMPLEMENT_DYNAMIC(TLStable, CDialogEx)

TLStable::TLStable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

TLStable::~TLStable()
{
}

void TLStable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, TLSTable_list);
}


BEGIN_MESSAGE_MAP(TLStable, CDialogEx)
END_MESSAGE_MAP()


// TLStable 消息处理程序
PEFileData TlsData;

BOOL TLStable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TLSTable_list.InsertColumn(0, L"数据库开始VA", 0, 80);
	TLSTable_list.InsertColumn(1, L"数据库结束VA", 0, 80);
	TLSTable_list.InsertColumn(2, L"TLS索引变量", 0, 80);
	TLSTable_list.InsertColumn(3, L"填零大小", 0, 80);
	TLSTable_list.InsertColumn(4, L"特征值", 0, 80);
	InitlizeTlsTable();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

VOID TLStable::InitlizeTlsTable()
{

	LPVOID lpBuffer;
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &TlsData.lpFileBuffer);
	TlsData.dwPlatform = Image32Or64(TlsData.lpFileBuffer);

	PIMAGE_TLS_DIRECTORY Image_TLS = NULL;
	PIMAGE_DOS_HEADER pImage_Dos = (PIMAGE_DOS_HEADER)TlsData.lpFileBuffer;
	if (TlsData.dwPlatform == IMAGE_FILE_MACHINE_I386)
	{
		PIMAGE_NT_HEADERS pImage_NT = (PIMAGE_NT_HEADERS)((DWORD)TlsData.lpFileBuffer + pImage_Dos->e_lfanew);
		//定位数据目录表中的TLS结构
		PIMAGE_DATA_DIRECTORY pImage_Data = (PIMAGE_DATA_DIRECTORY)(pImage_NT->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_TLS);
		Image_TLS = (PIMAGE_TLS_DIRECTORY)(RvaToOffset32(pImage_Data->VirtualAddress, (PCHAR)TlsData.lpFileBuffer) + (PCHAR)TlsData.lpFileBuffer);
	}
	else
	{
		if (TlsData.dwPlatform == IMAGE_FILE_MACHINE_IA64)
		{
			PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)((DWORD)TlsData.lpFileBuffer + pImage_Dos->e_lfanew);
			//定位数据目录表中的TLS结构
			PIMAGE_DATA_DIRECTORY pImage_Data = (PIMAGE_DATA_DIRECTORY)(pImage_NT->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_TLS);
			Image_TLS = (PIMAGE_TLS_DIRECTORY)(RvaToOffset64(pImage_Data->VirtualAddress, (PCHAR)TlsData.lpFileBuffer) + (PCHAR)TlsData.lpFileBuffer);
		}
	}
	WCHAR wcTLSValue[26];
	ULONG uIndex = 0;
	wsprintf(wcTLSValue, L"0x%X", Image_TLS->StartAddressOfRawData);
	TLSTable_list.InsertItem(uIndex, wcTLSValue);

	wsprintf(wcTLSValue, L"0x%X", Image_TLS->EndAddressOfRawData);
	TLSTable_list.SetItemText(uIndex, 1, wcTLSValue);

	wsprintf(wcTLSValue, L"0x%X", Image_TLS->AddressOfIndex);
	TLSTable_list.SetItemText(uIndex, 2, wcTLSValue);

	wsprintf(wcTLSValue, L"0x%X", Image_TLS->SizeOfZeroFill);
	TLSTable_list.SetItemText(uIndex, 3, wcTLSValue);

	wsprintf(wcTLSValue, L"0x%X", Image_TLS->Characteristics);
	TLSTable_list.SetItemText(uIndex, 4, wcTLSValue);

	return VOID();
}
