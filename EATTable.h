#pragma once


// EATTable 对话框

class EATTable : public CDialogEx
{
	DECLARE_DYNAMIC(EATTable)

public:
	EATTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EATTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void EATTable::InitlizeEATTable();
	friend DWORD WINAPI InitlizeEATNameTable(LPVOID lp);
private:
	CListCtrl FunInfo_List;
	CString CSt_EATTableOffset;
	CString CSt_EigenValue;
	CString CSt_Base;
	CString CSt_Name;
	CString CSt_NameStr;
	CString CSt_FunNumer;
	CString CSt_FunNameNumer;
	CString CSt_FunAddress;
	CString CSt_FunNameAddr;
	CString CSt_FunNameOrd;
public:
	afx_msg void OnBnClickedButton1();
};
