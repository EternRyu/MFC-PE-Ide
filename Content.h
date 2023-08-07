#pragma once


// Content 对话框

class Content : public CDialogEx
{
	DECLARE_DYNAMIC(Content)

public:
	Content(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Content();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual VOID InitlizeContent(LPVOID lpBuffer);
	virtual BOOL IninlizeContent64(LPVOID lpBuffer);
	virtual BOOL IninlizeContent32(LPVOID lpBuffer);
private:
	CString CSt_IATTable;
public:
	afx_msg void OnBnClickedButton2();
private:
	CString CSt_ExprotTable;
	CString CSt_RESOURCE;
	CString CSt_EXCEPTION;
	CString CSt_SECURITY;
	CString CSt_BASERELOC;
	CString CSt_DEBUG;
	CString CSt_ARCHITECTURE;
	CString CSt_GLOBALPTR;
	CString CSt_TLS;
	CString CSt_LOAD_CONFIG;
	CString CSt_BOUND_IMPORT;
	CString CSt_IAT;
	CString CSt_DELAY_IMPORT;
	CString CSt_COM_DESCRIPTOR;
	CString CSt_Save;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
};
