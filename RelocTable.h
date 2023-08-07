#pragma once


// RelocTable 对话框

class RelocTable : public CDialogEx
{
	DECLARE_DYNAMIC(RelocTable)

public:
	RelocTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~RelocTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void InitlizeFRT();
private:
	CListCtrl FRT_List;
	CListCtrl SRT_List;
public:
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
};
