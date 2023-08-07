#pragma once


// PEID 对话框

class PEID : public CDialogEx
{
	DECLARE_DYNAMIC(PEID)

public:
	PEID(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PEID();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual VOID PEID::InitlizeNtHerder32(LPVOID lpBuffer64);
	virtual VOID PEID::InitlizeNtHerder64(LPVOID lpBuffer64);
	virtual VOID PEID::SaveNtHeader32(LPVOID lpBuffer32);
	virtual VOID PEID::SaveNtHeader64(LPVOID lpBuffer64);
private:
	CString CSt_Entry;
	CString CSt_Base;
	CString CSt_ImageSize;
	CString CSt_CodeEntry;
	CString CSt_DataEntry;
	CString CSt_Section;
	CString CSt_FileAlig;
	CString CSt_Magic;
	CString CSt_Version;
	CString CSt_SectionNumer;
	CString CSt_FileTime;
	CString CSt_HeaderSize;
	CString CSt_Characteristics;
	CString CSt_CheckSum;
	CString CSt_szOptionalHeader;
	CString CSt_RvaSizeNumer;
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};

