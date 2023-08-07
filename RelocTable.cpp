// RelocTable.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "RelocTable.h"
#include "afxdialogex.h"

PEFileData FileData;
List* RelocDataList = NULL;
// RelocTable 对话框

IMPLEMENT_DYNAMIC(RelocTable, CDialogEx)

RelocTable::RelocTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{

}

RelocTable::~RelocTable()
{
}

void RelocTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, FRT_List);
	DDX_Control(pDX, IDC_LIST3, SRT_List);
}


BEGIN_MESSAGE_MAP(RelocTable, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &RelocTable::OnNMDblclkList1)
END_MESSAGE_MAP()


// RelocTable 消息处理程序


BOOL RelocTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	FRT_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	FRT_List.InsertColumn(0, L"索引", 0, 50);
	FRT_List.InsertColumn(1, L"区段", 0, 100);
	FRT_List.InsertColumn(2, L"RVA", 0, 100);
	FRT_List.InsertColumn(3, L"项目", 0, 100);

	SRT_List.InsertColumn(0, L"索引", 0, 50);
	SRT_List.InsertColumn(1, L"RVA", 0, 100);
	SRT_List.InsertColumn(2, L"区段", 0, 100);
	SRT_List.InsertColumn(3, L"偏移", 0, 100);
	InitlizeFRT();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

typedef struct _TYPE
{
	WORD Offset : 12;  //大小为两bit重定位的偏移
	WORD Type : 4;  //重定位类型
}TYPE, * PTYPE;

void RelocTable::InitlizeFRT()
{
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &FileData.lpFileBuffer);
	FileData.dwPlatform = Image32Or64(FileData.lpFileBuffer);

	PIMAGE_DOS_HEADER pImage_Dos = (PIMAGE_DOS_HEADER)FileData.lpFileBuffer;
	
	PIMAGE_BASE_RELOCATION Image_RelocTbale = NULL;
	PIMAGE_SECTION_HEADER pSection = NULL;
	if (FileData.dwPlatform == IMAGE_FILE_MACHINE_I386)
	{
		PIMAGE_NT_HEADERS pImage_NT = (PIMAGE_NT_HEADERS)((DWORD)FileData.lpFileBuffer + pImage_Dos->e_lfanew);
		PIMAGE_DATA_DIRECTORY pImage_Data = (PIMAGE_DATA_DIRECTORY)(pImage_NT->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC);
		Image_RelocTbale = (PIMAGE_BASE_RELOCATION)(RvaToOffset32(pImage_Data->VirtualAddress, (PCHAR)FileData.lpFileBuffer) + (PCHAR)FileData.lpFileBuffer);
		pSection = IMAGE_FIRST_SECTION(pImage_NT);
	}
	if (FileData.dwPlatform == IMAGE_FILE_MACHINE_IA64)
	{
		PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)((DWORD)FileData.lpFileBuffer + pImage_Dos->e_lfanew);
		PIMAGE_DATA_DIRECTORY pImage_Data = (PIMAGE_DATA_DIRECTORY)(pImage_NT->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC);
		Image_RelocTbale = (PIMAGE_BASE_RELOCATION)(RvaToOffset64(pImage_Data->VirtualAddress, (PCHAR)FileData.lpFileBuffer) + (PCHAR)FileData.lpFileBuffer);
		pSection = IMAGE_FIRST_SECTION(pImage_NT);
	}
	
	DWORD dwStuSize = sizeof(IMAGE_BASE_RELOCATION);
	ULONG uIndex = 0;
	ListInit(&RelocDataList);
	while (Image_RelocTbale->SizeOfBlock != 0)
	{
		//找到本0x1000个字节的起始位置
		Data* RelocNode = (Data*)malloc(sizeof(Data));
		//RelocNode->dwCount = (Image_RelocTbale->SizeOfBlock - 8) / 2;
		DWORD dwRva = Image_RelocTbale->VirtualAddress;

		RelocNode->Hint = uIndex;
		RelocNode->Address= (DWORD)malloc(dwStuSize);
		CopyMemory((PIMAGE_BASE_RELOCATION)RelocNode->Address,Image_RelocTbale , dwStuSize);
		
		InserList(RelocDataList, RelocNode);

		WCHAR wcIndes[8];
		swprintf(wcIndes, sizeof(ULONG), L"%d", uIndex);
		FRT_List.InsertItem(uIndex, wcIndes);

		DWORD StrSize = (strlen((PCHAR)pSection->Name)+1)*2;
		PWCHAR wcName = (PWCHAR)malloc(StrSize);
		swprintf(wcName, StrSize, L"%hs", pSection->Name);
		FRT_List.SetItemText(uIndex, 1, wcName);
		free(wcName);

		
		WCHAR RAV[26];
		wsprintf(RAV, L"0x%X", dwRva);
		FRT_List.SetItemText(uIndex, 2, RAV);
		
		WCHAR wcStr1[54];
		WCHAR wcStr2[26];
		wsprintf(wcStr1, L"%X H/", Image_RelocTbale->SizeOfBlock);
		wsprintf(wcStr2, L"%d D", Image_RelocTbale->SizeOfBlock);
		wcscat_s(wcStr1, wcStr2);
		FRT_List.SetItemText(uIndex, 3, wcStr1);
		
		//找到下一个0x1000个字节的结构体
		Image_RelocTbale = (PIMAGE_BASE_RELOCATION)((PCHAR)Image_RelocTbale + Image_RelocTbale->SizeOfBlock);
		if (Image_RelocTbale->VirtualAddress >= pSection[1].VirtualAddress)
		{
			pSection = &pSection[1];
		}
		uIndex++;
	}
}



//双击处理
void RelocTable::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION Position = FRT_List.GetFirstSelectedItemPosition();
	if (Position == NULL)
	{
		*pResult = 0;
		return;
	}
	//获取选中行号
	DWORD Index = FRT_List.GetNextSelectedItem(Position);
	//获取选中行第Y列内容
	CString csValue = FRT_List.GetItemText(Index, 0);
	DWORD dwValue = _tcstoul(csValue, NULL, 16);
	SRT_List.DeleteAllItems();//清空
	//PTYPE Value = (PTYPE)malloc(sizeof(TYPE));
	LPVOID lpValue = malloc(sizeof(LPVOID));
	FindList(RelocDataList, dwValue, &lpValue);
	PIMAGE_BASE_RELOCATION PBR = (PIMAGE_BASE_RELOCATION)lpValue;
	DWORD dwCount = (PBR->SizeOfBlock - 8) / 2;
	DWORD dwRva = PBR->VirtualAddress;
	PTYPE pRelocArr = (PTYPE)(PBR + 1);

	if (FileData.dwPlatform == IMAGE_FILE_MACHINE_I386)
	{
		for (int i = 0; i < dwCount; i++)
		{
			WCHAR wcStr[26];
			PDWORD pData = (PDWORD)(RvaToOffset32(pRelocArr[i].Offset, (PCHAR)FileData.lpFileBuffer) + (PCHAR)FileData.lpFileBuffer);
			DWORD pDataOffset = RvaToOffset32(pRelocArr[i].Offset, (PCHAR)FileData.lpFileBuffer);
			wsprintf(wcStr, L"%X", i);
			SRT_List.InsertItem(i, wcStr);

			wsprintf(wcStr, L"%X", pRelocArr[i].Offset + dwRva);
			SRT_List.SetItemText(i, 1, wcStr);

			wsprintf(wcStr, L"%X", *pData);
			SRT_List.SetItemText(i, 2, wcStr);

			wsprintf(wcStr, L"%X", pDataOffset);
			SRT_List.SetItemText(i, 3, wcStr);

		}
	}
	else
	{
		if (FileData.dwPlatform == IMAGE_FILE_MACHINE_IA64)
		{
			for (int i = 0; i < dwCount; i++)
			{
				WCHAR wcStr[26];
				PDWORD pData = (PDWORD)(RvaToOffset64(pRelocArr[i].Offset, (PCHAR)FileData.lpFileBuffer) + (PCHAR)FileData.lpFileBuffer);
				DWORD pDataOffset = RvaToOffset64(pRelocArr[i].Offset, (PCHAR)FileData.lpFileBuffer);
				wsprintf(wcStr, L"%X", i);
				SRT_List.InsertItem(i, wcStr);

				wsprintf(wcStr, L"%X", pRelocArr[i].Offset + dwRva);
				SRT_List.SetItemText(i, 1, wcStr);

				wsprintf(wcStr, L"%X", *pData);
				SRT_List.SetItemText(i, 2, wcStr);

				wsprintf(wcStr, L"%X", pDataOffset);
				SRT_List.SetItemText(i, 3, wcStr);
			}
		}
	}
	*pResult = 0;
}
