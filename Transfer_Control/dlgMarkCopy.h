#pragma once
#include "afxwin.h"


// CdlgMarkCopy ��ȭ �����Դϴ�.

class CdlgMarkCopy : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgMarkCopy)

public:
	CdlgMarkCopy(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CdlgMarkCopy();
	

	int CurModel, SelModel;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MARK_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbModelData;
	DECLARE_EVENTSINK_MAP()
	void ClickExit();
	void ClickExit2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
