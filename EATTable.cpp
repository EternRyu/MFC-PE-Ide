// EATTable.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "EATTable.h"
#include "afxdialogex.h"


// EATTable 对话框

IMPLEMENT_DYNAMIC(EATTable, CDialogEx)

LPVOID lpEBuffer=NULL;


typedef struct ThreadDATA
{
	PIMAGE_EXPORT_DIRECTORY pEATData;
	EATTable* ClassPTR;
}ThreadDATA,*PThreadDATA;

ThreadDATA TD;

EATTable::EATTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	, CSt_EATTableOffset(_T(""))
	, CSt_EigenValue(_T(""))
	, CSt_Base(_T(""))
	, CSt_Name(_T(""))
	, CSt_NameStr(_T(""))
	, CSt_FunNumer(_T(""))
	, CSt_FunNameNumer(_T(""))
	, CSt_FunAddress(_T(""))
	, CSt_FunNameAddr(_T(""))
	, CSt_FunNameOrd(_T(""))
{

}

EATTable::~EATTable()
{
}

void EATTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, FunInfo_List);
	DDX_Text(pDX, IDC_EDIT12, CSt_EATTableOffset);
	DDX_Text(pDX, IDC_EDIT13, CSt_EigenValue);
	DDX_Text(pDX, IDC_EDIT14, CSt_Base);
	DDX_Text(pDX, IDC_EDIT15, CSt_Name);
	DDX_Text(pDX, IDC_EDIT16, CSt_NameStr);
	DDX_Text(pDX, IDC_EDIT1, CSt_FunNumer);
	DDX_Text(pDX, IDC_EDIT3, CSt_FunNameNumer);
	DDX_Text(pDX, IDC_EDIT4, CSt_FunAddress);
	DDX_Text(pDX, IDC_EDIT5, CSt_FunNameAddr);
	DDX_Text(pDX, IDC_EDIT6, CSt_FunNameOrd);
}


BEGIN_MESSAGE_MAP(EATTable, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &EATTable::OnBnClickedButton1)
END_MESSAGE_MAP()


// EATTable 消息处理程序


BOOL EATTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	FunInfo_List.InsertColumn(0, L"序号", 0, 110);
	FunInfo_List.InsertColumn(1, L"RVA", 0, 100);
	FunInfo_List.InsertColumn(2, L"偏移", 0, 100);
	FunInfo_List.InsertColumn(3, L"函数名", 0, 300);
	InitlizeEATTable();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


VOID  EATTable::InitlizeEATTable()
{
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &lpEBuffer);
	PCHAR szName = NULL;
	PIMAGE_DATA_DIRECTORY pImageDataDirectory = NULL;
	PIMAGE_EXPORT_DIRECTORY pImageExportDerscriptor = NULL;
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpEBuffer;

	if (Image32Or64(lpEBuffer) == IMAGE_FILE_MACHINE_I386)
	{
		PIMAGE_NT_HEADERS pImageNtHeader = (PIMAGE_NT_HEADERS)((DWORD)lpEBuffer + pImageDosHeader->e_lfanew);
		pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
		pImageExportDerscriptor = (PIMAGE_EXPORT_DIRECTORY)(RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);
		CSt_EATTableOffset.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpEBuffer));
		szName = (PCHAR)(RvaToOffset32(pImageExportDerscriptor->Name, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);
	}
	else
	{
		PIMAGE_NT_HEADERS64 pImageNtHeader = (PIMAGE_NT_HEADERS64)((DWORD)lpEBuffer + pImageDosHeader->e_lfanew);
		pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
		pImageExportDerscriptor = (PIMAGE_EXPORT_DIRECTORY)(RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);
		CSt_EATTableOffset.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpEBuffer));
		szName = (PCHAR)(RvaToOffset64(pImageExportDerscriptor->Name, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);
	}
	CSt_EigenValue.Format(TEXT("%X"), pImageExportDerscriptor->Characteristics);
	CSt_Base.Format(TEXT("%X"), pImageExportDerscriptor->Base);
	CSt_Name.Format(TEXT("%X"), pImageExportDerscriptor->Name);

	DWORD dwSizeStr = (strlen(szName) + 1) * 2;
	PWCHAR pwStrName = (PWCHAR)malloc(dwSizeStr);
	swprintf(pwStrName, dwSizeStr, L"%hs", szName);
	CSt_NameStr.Format(TEXT("%s"), pwStrName);
	free(pwStrName);

	CSt_FunNumer.Format(TEXT("%X"), pImageExportDerscriptor->NumberOfFunctions);
	CSt_FunNameNumer.Format(TEXT("%X"), pImageExportDerscriptor->NumberOfNames);
	CSt_FunAddress.Format(TEXT("%X"), pImageExportDerscriptor->AddressOfFunctions);
	CSt_FunNameAddr.Format(TEXT("%X"), pImageExportDerscriptor->AddressOfNames);
	CSt_FunNameOrd.Format(TEXT("%X"), pImageExportDerscriptor->AddressOfNameOrdinals);
	UpdateData(FALSE);

	//InitlizeEATNameTable(pImageExportDerscriptor);

	
	TD.pEATData = pImageExportDerscriptor;
	TD.ClassPTR = this;

	

	CreateThread(NULL, 0, InitlizeEATNameTable, &TD, 0, NULL);
	//HANDEL hThread=AfxBeginThread(InitlizeEATNameTable, pImageExportDerscriptor, NULL, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}



DWORD WINAPI InitlizeEATNameTable(LPVOID lp)
{
	PThreadDATA TD = (PThreadDATA)lp;
	
	PDWORD PD_EAT = NULL;
	PDWORD PD_ENT = NULL;
	PWORD PD_PID = NULL;
	if (Image32Or64(lpEBuffer) == IMAGE_FILE_MACHINE_I386)
	{
		PD_EAT = (PDWORD)(RvaToOffset32(TD->pEATData->AddressOfFunctions, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);//导出地址表
		PD_ENT = (PDWORD)(RvaToOffset32(TD->pEATData->AddressOfNames, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);//导出名称表
		PD_PID = (PWORD)(RvaToOffset32(TD->pEATData->AddressOfNameOrdinals, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);//导出序号表
	}
	else
	{
		PD_EAT = (PDWORD)(RvaToOffset64(TD->pEATData->AddressOfFunctions, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);//导出地址表
		PD_ENT = (PDWORD)(RvaToOffset64(TD->pEATData->AddressOfNames, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);//导出名称表
		PD_PID = (PWORD)(RvaToOffset64(TD->pEATData->AddressOfNameOrdinals, (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);//导出序号表
	}
	DWORD dwNumOfFun = TD->pEATData->NumberOfFunctions; //函数总数
	DWORD dwNumOfName = TD->pEATData->NumberOfNames; //函数名称总数
	DWORD dwBase = TD->pEATData->Base; //基

	ULONG uIndex = 0;
	for (DWORD i = 0; i < dwNumOfFun; i++)
	{
		if (PD_EAT[i] == 0)
		{
			continue;
		}
		DWORD ID = 0;
		for (; ID < dwNumOfName; ID++)
		{
			if (PD_PID[ID] == i)
			{
				break;
			}
		}
		if (ID == dwNumOfName)
		{
			WCHAR wcBuffer[MAX_PATH];
			//printf_s("ID:%x--Address:0x%08X--Name[NULL]\n", i + dwBase, PD_EAT[i]);
			wsprintf(wcBuffer, L"0x%X", i + dwBase);
			TD->ClassPTR->FunInfo_List.InsertItem(uIndex, wcBuffer);

			wsprintf(wcBuffer, L"0x%X", PD_EAT[i]);
			TD->ClassPTR->FunInfo_List.SetItemText(uIndex, 1, wcBuffer);

			wsprintf(wcBuffer, L"0x%X", RvaToOffset64(PD_ENT[ID], (PCHAR)lpEBuffer));
			TD->ClassPTR->FunInfo_List.SetItemText(uIndex, 2, wcBuffer);

			TD->ClassPTR->FunInfo_List.SetItemText(uIndex, 3, L"Unknown");
			uIndex++;

		}
		else
		{
			PCHAR szFuncName;
			DWORD FOA;
			if (Image32Or64(lpEBuffer) == IMAGE_FILE_MACHINE_I386)
			{
				szFuncName = (PCHAR)(RvaToOffset32(PD_ENT[ID], (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);
				FOA = FileOffset32(PD_EAT[i],(PCHAR)lpEBuffer);
			}
			else
			{
				szFuncName = (PCHAR)(RvaToOffset64(PD_ENT[ID], (PCHAR)lpEBuffer) + (PCHAR)lpEBuffer);
				FOA = FileOffset64(PD_EAT[i],(PCHAR)lpEBuffer);
			}
			WCHAR wcBuffer[MAX_PATH];
			wsprintf(wcBuffer, L"%X", i + dwBase);
			TD->ClassPTR->FunInfo_List.InsertItem(uIndex, wcBuffer);

			wsprintf(wcBuffer, L"0x%X", PD_EAT[i]);
			TD->ClassPTR->FunInfo_List.SetItemText(uIndex, 1, wcBuffer);

			wsprintf(wcBuffer, L"0x%X", PD_EAT[i]-FOA);
			TD->ClassPTR->FunInfo_List.SetItemText(uIndex, 2, wcBuffer);

			DWORD szSize = (strlen(szFuncName) + 1) * 2;
			swprintf(wcBuffer, szSize, L"%hs", szFuncName);
			//wsprintf(wcBuffer, L"0x%X", szFuncName);
			TD->ClassPTR->FunInfo_List.SetItemText(uIndex, 3, wcBuffer);
			uIndex++;
		}
		
	}
	return 0;
}


//确定
void EATTable::OnBnClickedButton1()
{
	CDialog::EndDialog(0);
}


