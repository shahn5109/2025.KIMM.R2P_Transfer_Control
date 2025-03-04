#pragma once
#include "afxwin.h"


// CdlgMarkCopy 대화 상자입니다.

class CdlgMarkCopy : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgMarkCopy)

public:
	CdlgMarkCopy(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgMarkCopy();
	

	int CurModel, SelModel;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MARK_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbModelData;
	DECLARE_EVENTSINK_MAP()
	void ClickExit();
	void ClickExit2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
