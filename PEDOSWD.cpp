// PEDOSWD.cpp: 实现文件
//

#include "pch.h"
#include "PE.h"
#include "PEDOSWD.h"
#include "afxdialogex.h"
#include "Communication.h"

PWCHAR pwcName[] = { L"e_magic",L"e_cblp",L"e_cp",L"e_crlc" ,L"e_cparhdr" ,L"e_minalloc" ,L"e_maxalloc" ,L"e_ss" ,L"e_sp" ,L"e_csum" ,L"e_ip" ,L"e_cs" ,L"e_lfarlc",L"e_ovno",L"e_res[4]" ,L"e_oemid" ,L"e_oeminfo" ,L"e_res2[10]" ,L"e_lfanew" };

// PEDOSWD 对话框

IMPLEMENT_DYNAMIC(PEDOSWD, CDialog)

PEDOSWD::PEDOSWD(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

PEDOSWD::~PEDOSWD()
{
}

void PEDOSWD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, DosList);
}


BEGIN_MESSAGE_MAP(PEDOSWD, CDialog)
END_MESSAGE_MAP()


// PEDOSWD 消息处理程序


BOOL PEDOSWD::OnInitDialog()
{
	CDialog::OnInitDialog();
	DosList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	DosList.InsertColumn(0, L"类型", 0, 100);//插入顶部文本
	DosList.InsertColumn(1, L"名称", 0, 100);
	DosList.InsertColumn(2, L"值", 0, 100);
	IMAGE_DOS_HEADER IDH = {0};
	ReceiveRam(&IDH, 0,256, L"FileMappingObject");
	InitListControl(IDH);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

VOID PEDOSWD::InitListControl(IMAGE_DOS_HEADER ImageDosHerder)
{
	DosList.DeleteAllItems();
	ULONG uIndex = 0;
	DWORD dwIndex = 0;
	WCHAR wcVlaue[260];
	for (DWORD i = 0; i < 18; i++)
	{
		if (i==14 || i==17)
		{
			if (i == 14)
			{
				DosList.InsertItem(uIndex, L"PDWORD");
				DosList.SetItemText(uIndex, 1, pwcName[uIndex]);
				wsprintf(wcVlaue, L"0x%X", ((DWORD)((DWORD)(&ImageDosHerder) + dwIndex)));
				DosList.SetItemText(uIndex, 2, wcVlaue);
				uIndex++;
				dwIndex += 8;
			}
			if (i == 17)
			{
				DosList.InsertItem(uIndex, L"PDWORD");
				DosList.SetItemText(uIndex, 1, pwcName[uIndex]);
				wsprintf(wcVlaue, L"0x%X", ((DWORD)((DWORD)(&ImageDosHerder) + dwIndex)));
				DosList.SetItemText(uIndex, 2, wcVlaue);
				uIndex++;
				dwIndex += 20;
			}
		}
		else
		{
			DosList.InsertItem(uIndex, L"WORD");
			DosList.SetItemText(uIndex, 1, pwcName[uIndex]);
			wsprintf(wcVlaue, L"0x%X", *((PWORD)((DWORD)(&ImageDosHerder) + dwIndex)));
			DosList.SetItemText(uIndex, 2, wcVlaue);
			uIndex++;
			dwIndex += 2;
		}
	}
	DosList.InsertItem(uIndex, L"DWORD");
	DosList.SetItemText(uIndex, 1, pwcName[uIndex]);
	wsprintf(wcVlaue, L"0x%X", *((PDWORD)((DWORD)(&ImageDosHerder) + dwIndex)));
	DosList.SetItemText(uIndex, 2, wcVlaue);
	return VOID();
}


