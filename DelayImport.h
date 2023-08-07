#pragma once


// DelayImport 对话框

class DelayImport : public CDialogEx
{
	DECLARE_DYNAMIC(DelayImport)

public:
	DelayImport(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DelayImport();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual VOID InitDelayImport();
private:
	CListCtrl DelayImport_List;
};
