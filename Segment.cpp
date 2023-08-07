// Segment.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "Segment.h"
#include "afxdialogex.h"


// Segment 对话框

IMPLEMENT_DYNAMIC(Segment, CDialogEx)

Segment::Segment(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

Segment::~Segment()
{
}

void Segment::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, SegListCtrl);
}


BEGIN_MESSAGE_MAP(Segment, CDialogEx)
END_MESSAGE_MAP()


// Segment 消息处理程序


BOOL Segment::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SegListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	SegListCtrl.InsertColumn(0, L"名称", 0, 100);
	SegListCtrl.InsertColumn(1, L"VOffset", 0, 100);
	SegListCtrl.InsertColumn(2, L"VSize", 0, 100);
	SegListCtrl.InsertColumn(3, L"ROffset", 0, 100);
	SegListCtrl.InsertColumn(4, L"RSize", 0, 100);
	SegListCtrl.InsertColumn(5, L"标志", 0, 100);
	// TODO:  在此添加额外的初始化
	InitlizeSegListCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void Segment::InitlizeSegListCtrl()
{
	LPVOID lpBuffer;
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	PE_DOS(FileName, &lpBuffer);
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)lpBuffer;
	PIMAGE_NT_HEADERS pImageNtHeader = (PIMAGE_NT_HEADERS)((DWORD)lpBuffer + pImageDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pImageSectionHeader = IMAGE_FIRST_SECTION(pImageNtHeader);
	PIMAGE_FILE_HEADER pImageFileHeader = (PIMAGE_FILE_HEADER)&pImageNtHeader->FileHeader;

	ULONG uIndex = 0;
	for (int i = 0; i < pImageFileHeader->NumberOfSections; i++)
	{
		PWCHAR pwcName = NULL;
		DWORD dwSize = sizeof(pImageSectionHeader[i].Name)*2;
		pwcName = (PWCHAR)malloc(dwSize);
		swprintf(pwcName, dwSize, L"%hs", (PCHAR)pImageSectionHeader[i].Name);
		SegListCtrl.InsertItem(uIndex, pwcName);
		free(pwcName);

		dwSize = (sizeof(&pImageSectionHeader[i].Characteristics) + 10) * 2;
		pwcName = (PWCHAR)malloc(dwSize);
		wsprintf(pwcName, L"0x%X", pImageSectionHeader[i].VirtualAddress);
		SegListCtrl.SetItemText(uIndex, 1, pwcName);
		
		wsprintf(pwcName, L"0x%X", pImageSectionHeader[i].SizeOfRawData);
		SegListCtrl.SetItemText(uIndex, 2, pwcName);

		wsprintf(pwcName, L"0x%X", pImageSectionHeader[i].PointerToRawData);
		SegListCtrl.SetItemText(uIndex, 3, pwcName);

		wsprintf(pwcName, L"0x%X", pImageSectionHeader[i].Misc.VirtualSize);
		SegListCtrl.SetItemText(uIndex, 4, pwcName);

		wsprintf(pwcName, L"0x%X", pImageSectionHeader[i].Characteristics);
		SegListCtrl.SetItemText(uIndex, 5, pwcName);
		
		free(pwcName);
		uIndex++;
	}
}

