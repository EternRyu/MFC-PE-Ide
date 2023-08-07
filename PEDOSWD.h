#pragma once


// PEDOSWD 对话框

class PEDOSWD : public CDialog
{
	DECLARE_DYNAMIC(PEDOSWD)

public:
	PEDOSWD(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PEDOSWD();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl DosList;
public:
	virtual BOOL OnInitDialog();
	VOID InitListControl(IMAGE_DOS_HEADER ImageDosHerder);
};
