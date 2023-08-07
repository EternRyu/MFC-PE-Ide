// ResourceTbale.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "ResourceTbale.h"
#include "afxdialogex.h"

List* ResourceTree=NULL;
PEFileData ResourceData;
// ResourceTbale 对话框

IMPLEMENT_DYNAMIC(ResourceTbale, CDialogEx)

ResourceTbale::ResourceTbale(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG10, pParent)
	, CSt_RNameC(_T(""))
	, CSt_RIDC(_T(""))
	, CSt_CurrentName(_T(""))
	, CSt_CurrentID(_T(""))
	, CSt_CurrentRVA(_T(""))
	, CSt_CurrentOffset(_T(""))
	, CSt_CurrentSize(_T(""))
{

}

ResourceTbale::~ResourceTbale()
{
}

void ResourceTbale::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, RTreeCon);
	DDX_Text(pDX, IDC_EDIT1, CSt_RNameC);
	DDX_Text(pDX, IDC_EDIT2, CSt_RIDC);
	DDX_Text(pDX, IDC_EDIT3, CSt_CurrentName);
	DDX_Text(pDX, IDC_EDIT4, CSt_CurrentID);
	DDX_Text(pDX, IDC_EDIT5, CSt_CurrentRVA);
	DDX_Text(pDX, IDC_EDIT6, CSt_CurrentOffset);
	DDX_Text(pDX, IDC_EDIT7, CSt_CurrentSize);
}


BEGIN_MESSAGE_MAP(ResourceTbale, CDialogEx)
END_MESSAGE_MAP()


// ResourceTbale 消息处理程序


BOOL ResourceTbale::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ListInit(&ResourceTree);
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &ResourceData.lpFileBuffer);
	ResourceData.dwPlatform = Image32Or64(ResourceData.lpFileBuffer);
	ResourceTable((PCHAR)ResourceData.lpFileBuffer, ResourceData.dwPlatform);
	Sleep(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


VOID ResourceTbale::ResourceTable(PCHAR pBuffer, DWORD Flag)
{
	const char* g_ResType[0x11] = { "NULL","鼠标指针","位图","图标","菜单","对话框","字符串列表","字体目录","字体","快捷键","非格式化资源","消息列表","鼠标指针组","NULL","图标","NULL","版本信息" };

	PIMAGE_DOS_HEADER pImage_Dos = (PIMAGE_DOS_HEADER)pBuffer;
	PIMAGE_RESOURCE_DIRECTORY Image_ResourceTbale = NULL;
	if (Flag == IMAGE_FILE_MACHINE_I386)
	{
		PIMAGE_NT_HEADERS pImage_NT = (PIMAGE_NT_HEADERS)(pBuffer + pImage_Dos->e_lfanew);
		PIMAGE_OPTIONAL_HEADER pOptional = (PIMAGE_OPTIONAL_HEADER)&pImage_NT->OptionalHeader;
		PIMAGE_DATA_DIRECTORY pImage_Data = pOptional->DataDirectory + IMAGE_DIRECTORY_ENTRY_RESOURCE;
		Image_ResourceTbale = (PIMAGE_RESOURCE_DIRECTORY)(RvaToOffset32(pImage_Data->VirtualAddress, pBuffer) + pBuffer);
	}
	else
	{
		if (Flag == IMAGE_FILE_MACHINE_IA64)
		{
			PIMAGE_NT_HEADERS64 pImage_NT = (PIMAGE_NT_HEADERS64)(pBuffer + pImage_Dos->e_lfanew);
			PIMAGE_OPTIONAL_HEADER64 pOptional = (PIMAGE_OPTIONAL_HEADER64)&pImage_NT->OptionalHeader;
			PIMAGE_DATA_DIRECTORY pImage_Data = pOptional->DataDirectory + IMAGE_DIRECTORY_ENTRY_RESOURCE;
			Image_ResourceTbale = (PIMAGE_RESOURCE_DIRECTORY)(RvaToOffset64(pImage_Data->VirtualAddress, pBuffer) + pBuffer);
		}
	}
	
	DWORD dwResNum = Image_ResourceTbale->NumberOfNamedEntries + Image_ResourceTbale->NumberOfIdEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pImage_RDEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(Image_ResourceTbale + 1);


	//printf_s("根目录名称入口:%04X\n", Image_ResourceTbale->NumberOfNamedEntries);
	//printf_s("根目录ID入口:%04X\n", Image_ResourceTbale->NumberOfIdEntries);

	for (int i = 0; i < dwResNum; i++)
	{
		PBranchContent PBC = (PBranchContent)malloc(sizeof(BranchContent));
		if (pImage_RDEntry->NameIsString != 1)
		{
			if (pImage_RDEntry->Id < 0x10)
			{
				DWORD dwSize = (strlen(g_ResType[pImage_RDEntry->Id]) + 1) * 2;
				PBC->wcBranchName = (PWCHAR)malloc(dwSize);
				swprintf(PBC->wcBranchName, dwSize, L"%hs", g_ResType[pImage_RDEntry->Id]);
				//printf_s("ResType:%s\n", g_ResType[pImage_RDEntry->Id]);
			}
		}
		else
		{
			//PIMAGE_RESOURCE_DIR_STRING_U pResName = (PIMAGE_RESOURCE_DIR_STRING_U)(pImage_RDEntry->NameOffset + (DWORD)Image_ResourceTbale);
			//wchar_t* EpName = new wchar_t[pResName->Length + 1];
			//memset(EpName, 0, sizeof(wchar_t) * (pResName->Length + 1));
			//wcsncpy_s(EpName, pResName->Length + 1, pResName->NameString, pResName->Length);

			//printf_s("ResType:%d\n", g_ResType[pImage_RDEntry->Id]);
			DWORD dwSize = (strlen(g_ResType[pImage_RDEntry->Id]) + 1) * 2;
			PBC->wcBranchName = (PWCHAR)malloc(dwSize);
			swprintf(PBC->wcBranchName, dwSize, L"%d", g_ResType[pImage_RDEntry->Id]);
			
		}
		if (pImage_RDEntry->DataIsDirectory == 1)
		{
			PIMAGE_RESOURCE_DIRECTORY pSecond = (PIMAGE_RESOURCE_DIRECTORY)(pImage_RDEntry->OffsetToDirectory + (DWORD)Image_ResourceTbale);
			DWORD dwSecondCount = pSecond->NumberOfIdEntries + pSecond->NumberOfNamedEntries;
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pSecondEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pSecond + 1);
			
			PBC->NumberOfNamedEntries = pSecond->NumberOfNamedEntries;
			PBC->NumberOfIdEntries = pSecond->NumberOfIdEntries;
			List* ItemNode_PTR=NULL;
			ListInit(&ItemNode_PTR);
			PBC->ItemEntry = (PVOID)ItemNode_PTR;
			//printf_s("名称入口:%04X\n", pSecond->NumberOfNamedEntries);
			//printf_s("ID入口:%04X\n", pSecond->NumberOfIdEntries);
			for (int i = 0; i < dwSecondCount; i++)
			{
				PItem NewItem = (PItem)malloc(sizeof(Item));
				if (pSecondEntry->NameIsString != 1)
				{
					//printf_s("ID:%d\n", pSecondEntry->Id);
					DWORD dwSize = (sizeof(pSecondEntry->Id) + 1) * 2;
					NewItem->Name = (PWCHAR)malloc(dwSize);
					swprintf(NewItem->Name, dwSize, L"%d", pSecondEntry->Id);
				}
				else
				{
					/*
					wchar_t* pName = new wchar_t[pResName->Length + 1];
					memset(pName, 0, sizeof(wchar_t) * (pResName->Length + 1));
					wcsncpy_s(pName, pResName->Length + 1, pResName->NameString, );
					printf_s("Name:%s\n", pName);
					delete[]pName;*/
					PIMAGE_RESOURCE_DIR_STRING_U pResName = (PIMAGE_RESOURCE_DIR_STRING_U)(pImage_RDEntry->NameOffset + (DWORD)Image_ResourceTbale);
					DWORD dwSize = pResName->Length;
					NewItem->Name = (PWCHAR)malloc(dwSize);
					CopyMemory(NewItem->Name, pResName->NameString, pResName->Length);
				}
				if (pSecondEntry->DataIsDirectory == 1)
				{
					PIMAGE_RESOURCE_DIRECTORY pThird = (PIMAGE_RESOURCE_DIRECTORY)(pSecondEntry->OffsetToDirectory + (DWORD)Image_ResourceTbale);
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pThirdEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pThird + 1);

					if (pThirdEntry->DataIsDirectory != 1)
					{
						PIMAGE_RESOURCE_DATA_ENTRY pStcData = (PIMAGE_RESOURCE_DATA_ENTRY)(pThirdEntry->OffsetToData + (DWORD)Image_ResourceTbale);
						PCHAR pResBuf = NULL;
						
						if (Flag == IMAGE_FILE_MACHINE_I386)
						{
							pResBuf = (PCHAR)(RvaToOffset32(pStcData->OffsetToData, pBuffer) + pBuffer);
							NewItem->Offset = RvaToOffset32(pStcData->OffsetToData, pBuffer);
						}
						else
						{
							if (Flag == IMAGE_FILE_MACHINE_IA64)
							{
								pResBuf = (PCHAR)(RvaToOffset64(pStcData->OffsetToData, pBuffer) + pBuffer);
								NewItem->Offset = RvaToOffset64(pStcData->OffsetToData, pBuffer);
							}
						}
						NewItem->RAV = pStcData->OffsetToData;
						NewItem->Size = pStcData->Size;
					}
				}
				pSecondEntry++;

				InserList(ItemNode_PTR, (Data*)NewItem);
			}
		}
		pImage_RDEntry++;
		InserList(ResourceTree, (Data*)PBC);
	}
}