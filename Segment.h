#pragma once


// Segment 对话框

class Segment : public CDialogEx
{
	DECLARE_DYNAMIC(Segment)

public:
	Segment(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Segment();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void InitlizeSegListCtrl();
private:
	CListCtrl SegListCtrl;
};
