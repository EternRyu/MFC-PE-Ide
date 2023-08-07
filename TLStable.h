#pragma once


// TLStable 对话框

class TLStable : public CDialogEx
{
	DECLARE_DYNAMIC(TLStable)

public:
	TLStable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TLStable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual VOID InitlizeTlsTable();
private:
	CListCtrl TLSTable_list;
};
