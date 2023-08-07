#pragma once

typedef struct Item
{
	PWCHAR Name;
	DWORD RAV;
	DWORD Offset;
	DWORD Size;
}Item,*PItem;

typedef struct BranchContent
{
	PWCHAR wcBranchName;
	WORD NumberOfNamedEntries;
	WORD NumberOfIdEntries;
	PVOID ItemEntry;
}BranchContent,*PBranchContent;



// ResourceTbale 对话框
class ResourceTbale : public CDialogEx
{
	DECLARE_DYNAMIC(ResourceTbale)

public:
	ResourceTbale(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ResourceTbale();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl RTreeCon;
private:
	CString CSt_RNameC;
	CString CSt_RIDC;
	CString CSt_CurrentName;
	CString CSt_CurrentID;
	CString CSt_CurrentRVA;
	CString CSt_CurrentOffset;
	CString CSt_CurrentSize;
public:
	virtual BOOL OnInitDialog();
	virtual VOID ResourceTable(PCHAR pBuffer, DWORD Flag);
};


