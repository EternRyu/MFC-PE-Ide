// PEID.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "PEID.h"
#include "afxdialogex.h"

LPVOID glpBuffer=NULL;
// PEID 对话框

IMPLEMENT_DYNAMIC(PEID, CDialogEx)

PEID::PEID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, CSt_Entry(_T(""))
	, CSt_Base(_T(""))
	, CSt_ImageSize(_T(""))
	, CSt_CodeEntry(_T(""))
	, CSt_DataEntry(_T(""))
	, CSt_Section(_T(""))
	, CSt_FileAlig(_T(""))
	, CSt_Magic(_T(""))
	, CSt_Version(_T(""))
	, CSt_SectionNumer(_T(""))
	, CSt_FileTime(_T(""))
	, CSt_HeaderSize(_T(""))
	, CSt_Characteristics(_T(""))
	, CSt_CheckSum(_T(""))
	, CSt_szOptionalHeader(_T(""))
	, CSt_RvaSizeNumer(_T(""))
{

}

PEID::~PEID()
{
}

void PEID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, CSt_Entry);
	DDX_Text(pDX, IDC_EDIT2, CSt_Base);
	DDX_Text(pDX, IDC_EDIT3, CSt_ImageSize);
	DDX_Text(pDX, IDC_EDIT4, CSt_CodeEntry);
	DDX_Text(pDX, IDC_EDIT5, CSt_DataEntry);
	DDX_Text(pDX, IDC_EDIT6, CSt_Section);
	DDX_Text(pDX, IDC_EDIT7, CSt_FileAlig);
	DDX_Text(pDX, IDC_EDIT8, CSt_Magic);
	DDX_Text(pDX, IDC_EDIT9, CSt_Version);
	DDX_Text(pDX, IDC_EDIT10, CSt_SectionNumer);
	DDX_Text(pDX, IDC_EDIT11, CSt_FileTime);
	DDX_Text(pDX, IDC_EDIT12, CSt_HeaderSize);
	DDX_Text(pDX, IDC_EDIT13, CSt_Characteristics);
	DDX_Text(pDX, IDC_EDIT14, CSt_CheckSum);
	DDX_Text(pDX, IDC_EDIT15, CSt_szOptionalHeader);
	DDX_Text(pDX, IDC_EDIT16, CSt_RvaSizeNumer);
}


BEGIN_MESSAGE_MAP(PEID, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &PEID::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &PEID::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &PEID::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &PEID::OnBnClickedButton7)
END_MESSAGE_MAP()


// PEID 消息处理程序


BOOL PEID::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CHAR FileName[MAX_PATH];
	ZeroMemory(FileName, MAX_PATH);
	ReceiveRam(FileName, 0, 256, L"FilePath");
	if (*FileName==NULL)
	{
		AfxMessageBox(L"FilePath is NULL", MB_OK, NULL);
	}
	//PIMAGE_DOS_HEADER ImageDosHeaderFile=(PIMAGE_DOS_HEADER)malloc(sizeof(IMAGE_DOS_HEADER));
	PE_DOS(FileName, &glpBuffer);
	DWORD dwRet = Image32Or64(glpBuffer);
	if (dwRet == NULL)
	{
		return FALSE;
	}
	if (dwRet == IMAGE_FILE_MACHINE_I386)
	{
		InitlizeNtHerder32(glpBuffer);
	}
	if (dwRet == IMAGE_FILE_MACHINE_IA64)
	{
		InitlizeNtHerder64(glpBuffer);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//关闭对话框回车结束窗体
void PEID::OnOK()
{
	return;
}


//确定按钮
void PEID::OnBnClickedButton4()
{
	free(glpBuffer);
	CDialog::EndDialog(0);
}

//保存
void PEID::OnBnClickedButton5()
{
	SaveNtHeader64(glpBuffer);

}

//区段按钮
void PEID::OnBnClickedButton6()
{
	Segment SegDialgo;
	SegDialgo.DoModal();
}

//目录按钮
void PEID::OnBnClickedButton7()
{
	Content ContentDialgo;
	ContentDialgo.DoModal();
}
