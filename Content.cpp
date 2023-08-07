// Content.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "Content.h"
#include "afxdialogex.h"


// Content 对话框

IMPLEMENT_DYNAMIC(Content, CDialogEx)

Content::Content(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, CSt_IATTable(_T(""))
	, CSt_ExprotTable(_T(""))
	, CSt_RESOURCE(_T(""))
	, CSt_EXCEPTION(_T(""))
	, CSt_SECURITY(_T(""))
	, CSt_BASERELOC(_T(""))
	, CSt_DEBUG(_T(""))
	, CSt_ARCHITECTURE(_T(""))
	, CSt_GLOBALPTR(_T(""))
	, CSt_TLS(_T(""))
	, CSt_LOAD_CONFIG(_T(""))
	, CSt_BOUND_IMPORT(_T(""))
	, CSt_IAT(_T(""))
	, CSt_DELAY_IMPORT(_T(""))
	, CSt_COM_DESCRIPTOR(_T(""))
	, CSt_Save(_T(""))
{

}

Content::~Content()
{
}

void Content::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, CSt_IATTable);
	DDX_Text(pDX, IDC_EDIT1, CSt_ExprotTable);
	DDX_Text(pDX, IDC_EDIT3, CSt_RESOURCE);
	DDX_Text(pDX, IDC_EDIT4, CSt_EXCEPTION);
	DDX_Text(pDX, IDC_EDIT7, CSt_SECURITY);
	DDX_Text(pDX, IDC_EDIT8, CSt_BASERELOC);
	DDX_Text(pDX, IDC_EDIT9, CSt_DEBUG);
	DDX_Text(pDX, IDC_EDIT10, CSt_ARCHITECTURE);
	DDX_Text(pDX, IDC_EDIT11, CSt_GLOBALPTR);
	DDX_Text(pDX, IDC_EDIT12, CSt_TLS);
	DDX_Text(pDX, IDC_EDIT13, CSt_LOAD_CONFIG);
	DDX_Text(pDX, IDC_EDIT14, CSt_BOUND_IMPORT);
	DDX_Text(pDX, IDC_EDIT15, CSt_IAT);
	DDX_Text(pDX, IDC_EDIT16, CSt_DELAY_IMPORT);
	DDX_Text(pDX, IDC_EDIT17, CSt_COM_DESCRIPTOR);
	DDX_Text(pDX, IDC_EDIT18, CSt_Save);
}


BEGIN_MESSAGE_MAP(Content, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &Content::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Content::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON7, &Content::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &Content::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &Content::OnBnClickedButton10)
END_MESSAGE_MAP()


// Content 消息处理程序


BOOL Content::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LPVOID lpBuffer;
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &lpBuffer);
	InitlizeContent(lpBuffer);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

VOID Content::InitlizeContent(LPVOID lpBuffer)
{
	DWORD dwRet = Image32Or64(lpBuffer);
	if (dwRet == IMAGE_FILE_MACHINE_I386)
	{
		//x86
		IninlizeContent32(lpBuffer);
		UpdateData(FALSE);
	}
	if (dwRet == IMAGE_FILE_MACHINE_IA64)
	{
		//x64
		IninlizeContent64(lpBuffer);
		UpdateData(FALSE);
	}

}

BOOL Content::IninlizeContent64(LPVOID lpBuffer)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer;
	PIMAGE_NT_HEADERS64 pImageNtHeader = (PIMAGE_NT_HEADERS64)((DWORD)lpBuffer + pImageDosHeader->e_lfanew);
	PIMAGE_DATA_DIRECTORY pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT);
	PIMAGE_IMPORT_DESCRIPTOR pImageImportDerscriptor = (PIMAGE_IMPORT_DESCRIPTOR)(RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
	CSt_IATTable.Format(TEXT("%X"), (DWORD)pImageDataDirectory->VirtualAddress);

	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
	PIMAGE_EXPORT_DIRECTORY pImageExportDerscriptor = (PIMAGE_EXPORT_DIRECTORY)(RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
	CSt_ExprotTable.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//异常（例外）
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXCEPTION);
	CSt_EXCEPTION.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//安全
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_SECURITY);
	CSt_SECURITY.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//重定位
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC);
	CSt_BASERELOC.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//调试
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_DEBUG);
	CSt_DEBUG.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//版权
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_ARCHITECTURE);
	CSt_ARCHITECTURE.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//全局指针
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_GLOBALPTR);
	CSt_GLOBALPTR.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//TLS表
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_TLS);
	CSt_TLS.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//载入配置
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
	CSt_LOAD_CONFIG.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//载入范围
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
	CSt_BOUND_IMPORT.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//IAT
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IAT);
	CSt_IAT.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//延迟输入
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
	CSt_DELAY_IMPORT.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//COM
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR);
	CSt_COM_DESCRIPTOR.Format(TEXT("%X"), RvaToOffset64(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//保留
	//pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
	CSt_Save.Format(TEXT("%X"), L"00000000");

	return TRUE;
}

BOOL Content::IninlizeContent32(LPVOID lpBuffer)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer;
	PIMAGE_NT_HEADERS pImageNtHeader = (PIMAGE_NT_HEADERS)((DWORD)lpBuffer + pImageDosHeader->e_lfanew);
	PIMAGE_DATA_DIRECTORY pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT);
	PIMAGE_IMPORT_DESCRIPTOR pImageImportDerscriptor = (PIMAGE_IMPORT_DESCRIPTOR)(RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
	CSt_IATTable.Format(TEXT("%X"), (DWORD)pImageDataDirectory->VirtualAddress);

	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
	//PIMAGE_EXPORT_DIRECTORY pImageExportDerscriptor = (PIMAGE_EXPORT_DIRECTORY)(RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer) + (PCHAR)lpBuffer);
	CSt_ExprotTable.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));

	//资源
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_RESOURCE);
	CSt_RESOURCE.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//异常（例外）
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXCEPTION);
	CSt_EXCEPTION.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//安全
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_SECURITY);
	CSt_SECURITY.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//重定位
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC);
	CSt_BASERELOC.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//调试
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_DEBUG);
	CSt_DEBUG.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//版权
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_ARCHITECTURE);
	CSt_ARCHITECTURE.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//全局指针
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_GLOBALPTR);
	CSt_GLOBALPTR.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//TLS表
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_TLS);
	CSt_TLS.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//载入配置
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
	CSt_LOAD_CONFIG.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//载入范围
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
	CSt_BOUND_IMPORT.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//IAT
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IAT);
	CSt_IAT.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//延迟输入
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT);
	CSt_DELAY_IMPORT.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//COM
	pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR);
	CSt_COM_DESCRIPTOR.Format(TEXT("%X"), RvaToOffset32(pImageDataDirectory->VirtualAddress, (PCHAR)lpBuffer));
	//保留
	//pImageDataDirectory = (PIMAGE_DATA_DIRECTORY)(pImageNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_EXPORT);
	CSt_Save.Format(TEXT("%X"), L"00000000");
	return TRUE;
}

//IATTable
void Content::OnBnClickedButton2()
{
	IATTable IATTableDialog;
	IATTableDialog.DoModal();
}

//EATTable
void Content::OnBnClickedButton3()
{
	EATTable EATTableDialog;
	EATTableDialog.DoModal();
}

//RelocTable
void Content::OnBnClickedButton7()
{
	RelocTable RelocTableDialog;
	RelocTableDialog.DoModal();
}

//TLSTable
void Content::OnBnClickedButton9()
{
	TLStable TLSTableDialog;
	TLSTableDialog.DoModal();
}

//延迟输入
void Content::OnBnClickedButton10()
{
	ResourceTbale ResourceTableDialog;
	ResourceTableDialog.DoModal();
}
