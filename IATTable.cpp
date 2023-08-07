// IATTable.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "IATTable.h"
#include "afxdialogex.h"


// IATTable 对话框

IMPLEMENT_DYNAMIC(IATTable, CDialogEx)

IATTable::IATTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

IATTable::~IATTable()
{
}

void IATTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, IAT_List);
	DDX_Control(pDX, IDC_LIST2, Thunk_List);
}


BEGIN_MESSAGE_MAP(IATTable, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &IATTable::OnNMDblclkList1)
END_MESSAGE_MAP()


// IATTable 消息处理程序


BOOL IATTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	IAT_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	IAT_List.InsertColumn(0, L"DLLName", 0, 110);
	IAT_List.InsertColumn(1, L"OriginalFirstThunk", 0, 100);
	IAT_List.InsertColumn(2, L"TimeDataFlag", 0, 100);
	IAT_List.InsertColumn(3, L"ForwarderChain", 0, 100);
	IAT_List.InsertColumn(4, L"NameOffset", 0, 100);
	IAT_List.InsertColumn(5, L"FirstThink", 0, 100);

	Thunk_List.InsertColumn(0, L"ThunkRVA", 0, 100);
	Thunk_List.InsertColumn(1, L"ThunkOffset", 0, 100);
	Thunk_List.InsertColumn(2, L"ThunkValue", 0, 100);
	Thunk_List.InsertColumn(3, L"Hint", 0, 100);
	Thunk_List.InsertColumn(4, L"APIName", 0, 250);


	InilizeIATTable();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

DWORD dwFlag = 0;
LPVOID lpBuffer;

void IATTable::InilizeIATTable()
{
	
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &lpBuffer);
	PIMAGE_DATA_DIRECTORY pImageDataDirectory = NULL;
	PIMAGE_IMPORT_DESCRIPTOR pImageImportDerscriptor = NULL;
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer;
	if (Image32Or64(lpBuffer)== IMAGE_FILE_MACHINE_I386)
	{
		PIMAGE_NT_HEADERS pImageNtHeader = (PIMAGE_NT_HEADERS)((DWORD)lpBuffer + pImageDosHeader->e_lfanew);
		pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT);
		pImageImportDerscriptor = (PIMAGE_IMPORT_DESCRIPTOR)(RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
		dwFlag = IMAGE_FILE_MACHINE_I386;
	}
	else
	{
		PIMAGE_NT_HEADERS64 pImageNtHeader = (PIMAGE_NT_HEADERS64)((DWORD)lpBuffer + pImageDosHeader->e_lfanew);
		pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT);
		pImageImportDerscriptor = (PIMAGE_IMPORT_DESCRIPTOR)(RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
		dwFlag = IMAGE_FILE_MACHINE_IA64;
	}
	
	
	
	ULONG uIndex = 0;
	while (pImageImportDerscriptor->Name != NULL)
	{	
		PCHAR szDllName = NULL;
		if (dwFlag== IMAGE_FILE_MACHINE_I386)
		{
			szDllName = (PCHAR)(RvaToOffset32(pImageImportDerscriptor->Name, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
		}
		else
		{
			szDllName = (PCHAR)(RvaToOffset64(pImageImportDerscriptor->Name, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
		}

		PWCHAR pwcDllName= NULL;
		DWORD dwSize = (strlen(szDllName) + 1) * 2;
		pwcDllName = (PWCHAR)malloc(dwSize);
		swprintf(pwcDllName, dwSize, L"%hs", szDllName);
		IAT_List.InsertItem(uIndex, pwcDllName);
		free(pwcDllName);

		dwSize= sizeof(ULONGLONG) * 4;
		pwcDllName = (PWCHAR)malloc(dwSize);
		wsprintf(pwcDllName, L"0x%X", pImageImportDerscriptor->OriginalFirstThunk);
		IAT_List.SetItemText(uIndex, 1, pwcDllName);

		wsprintf(pwcDllName, L"0x%X", pImageImportDerscriptor->TimeDateStamp);
		IAT_List.SetItemText(uIndex, 2, pwcDllName);

		wsprintf(pwcDllName, L"0x%X", pImageImportDerscriptor->ForwarderChain);
		IAT_List.SetItemText(uIndex, 3, pwcDllName);

		wsprintf(pwcDllName, L"0x%X", pImageImportDerscriptor->Name);
		IAT_List.SetItemText(uIndex, 4, pwcDllName);

		wsprintf(pwcDllName, L"0x%X", pImageImportDerscriptor->FirstThunk);
		IAT_List.SetItemText(uIndex, 5, pwcDllName);


		free(pwcDllName);
		uIndex++;
		pImageImportDerscriptor++;
	}

}


//List 双击响应函数
void IATTable::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION Position = IAT_List.GetFirstSelectedItemPosition();
	if (Position == NULL)
	{
		*pResult = 0;
		return;
	}
	//获取选中行号
	DWORD Index = IAT_List.GetNextSelectedItem(Position);
	//获取选中行第Y列内容
	CString csValue = IAT_List.GetItemText(Index, 1);
	DWORD dwValue = _tcstoul(csValue, NULL, 16);
	Thunk_List.DeleteAllItems();//清空
	Thunk(dwValue);
	*pResult = 0;
}

VOID IATTable::Thunk(DWORD OriginalFirstThunk)
{
	if (lpBuffer==NULL)
	{
		return;
	}
	PIMAGE_THUNK_DATA pImage_Thunk = NULL;
	if (dwFlag == IMAGE_FILE_MACHINE_I386)
	{
		pImage_Thunk = (PIMAGE_THUNK_DATA)(RvaToOffset32(OriginalFirstThunk, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
	}
	else
	{
		pImage_Thunk = (PIMAGE_THUNK_DATA)(RvaToOffset64(OriginalFirstThunk, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
	}
	DWORD ValueIndex = 0;
	DWORD ltemIntex = 0;
	ULONG64 ImportOffset = 0;
	//被导入函数序号
	while (pImage_Thunk->u1.Ordinal != 0)
	{
		if (dwFlag == IMAGE_FILE_MACHINE_I386)
		{
			ImportOffset = RvaToOffset32(OriginalFirstThunk, (PCHAR)lpBuffer);
		}
		else
		{
			ImportOffset = RvaToOffset64(OriginalFirstThunk, (PCHAR)lpBuffer);
		}
		
	
		if ((pImage_Thunk->u1.Ordinal & 0x80000000) != 1)
		{
			PIMAGE_IMPORT_BY_NAME pImage_ByNaem = NULL;
			if (dwFlag == IMAGE_FILE_MACHINE_I386)
			{
				pImage_ByNaem = (PIMAGE_IMPORT_BY_NAME)(RvaToOffset32(pImage_Thunk->u1.AddressOfData, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
			}
			else
			{
				pImage_ByNaem = (PIMAGE_IMPORT_BY_NAME)(RvaToOffset64(pImage_Thunk->u1.AddressOfData, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
			}
			PWCHAR pwcBuffer = (PWCHAR)malloc(sizeof(OriginalFirstThunk + ValueIndex) * 8);
			wsprintf(pwcBuffer, L"0x%X", OriginalFirstThunk + ValueIndex);
			Thunk_List.InsertItem(ltemIntex, pwcBuffer);
			free(pwcBuffer);

			pwcBuffer = (PWCHAR)malloc((sizeof(ImportOffset + ValueIndex) * 2) * 4);
			wsprintf(pwcBuffer, L"0x%X", ImportOffset + ValueIndex);
			Thunk_List.SetItemText(ltemIntex, 1, pwcBuffer);
			free(pwcBuffer);

			pwcBuffer = (PWCHAR)malloc(sizeof(pImage_Thunk->u1.Function) * 8);
			wsprintf(pwcBuffer, L"0x%X", pImage_Thunk->u1.Function);
			Thunk_List.SetItemText(ltemIntex, 2, pwcBuffer);
			free(pwcBuffer);

			pwcBuffer = (PWCHAR)malloc((sizeof(pImage_ByNaem->Hint) * 2) * 4);
			wsprintf(pwcBuffer, L"0x%X", pImage_ByNaem->Hint);
			Thunk_List.SetItemText(ltemIntex, 3, pwcBuffer);
			free(pwcBuffer);

			
			DWORD dwBufferSize = (strlen(pImage_ByNaem->Name)+1)*2;
			pwcBuffer = (PWCHAR)malloc(dwBufferSize);
			swprintf(pwcBuffer, dwBufferSize, L"%hs", pImage_ByNaem->Name);
			Thunk_List.SetItemText(ltemIntex, 4, pwcBuffer);
			free(pwcBuffer);


			ltemIntex++;
		}
		ValueIndex += 4;
		if (dwFlag == IMAGE_FILE_MACHINE_I386)
		{
			pImage_Thunk++;
		}
		else
		{
			pImage_Thunk += 2;
		}
	}
}
