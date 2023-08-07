#pragma once


// IATTable 对话框

class IATTable : public CDialogEx
{
	DECLARE_DYNAMIC(IATTable)

public:
	IATTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~IATTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void InilizeIATTable();
	virtual void IATTable::Thunk(DWORD OriginalFirstThunk);
private:
	CListCtrl IAT_List;
public:
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CListCtrl Thunk_List;
};
