//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once

// CdlgManual_IO_Monitor 대화 상자입니다.

//#include "CtrlInclude.h"

class CdlgManual_IO_Monitor : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgManual_IO_Monitor)

public:
	CdlgManual_IO_Monitor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgManual_IO_Monitor();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IO };

//	CXPGroupBox	m_GroupBox1;
//	CXPGroupBox	m_GroupBox2;
	CBtnEnh	m_bInPrev;
	CBtnEnh	m_bInNext;
	CBtnEnh	m_bOutPrev;
	CBtnEnh	m_bOutNext;

public:
	CBrush m_DialogBrush;

	int m_iInPage, m_iOutPage;
	int m_iInAddrBase, m_iOutAddrBase;

	void 	DisplayUpdate();

protected:

	CBtnEnh	m_lblInputTitle[DEF_IOMONITOR_MAX_TITLE_PER_PAGE];
	CBtnEnh	m_btnInput[DEF_IOMONITOR_MAX_IO_PER_PAGE];
	CBtnEnh	m_lblOutputTitle[DEF_IOMONITOR_MAX_TITLE_PER_PAGE];
	CBtnEnh	m_btnOutput[DEF_IOMONITOR_MAX_IO_PER_PAGE];

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickOk();
	afx_msg void OnClickPrevious();
	afx_msg void OnClickNext();
	afx_msg void OnClickPrevious2();
	afx_msg void OnClickNext2();
	afx_msg void OnClickIo16();
	afx_msg void OnClickIo17();
	afx_msg void OnClickIo18();
	afx_msg void OnClickIo19();
	afx_msg void OnClickIo20();
	afx_msg void OnClickIo21();
	afx_msg void OnClickIo22();
	afx_msg void OnClickIo23();
	afx_msg void OnClickIo24();
	afx_msg void OnClickIo25();
	afx_msg void OnClickIo26();
	afx_msg void OnClickIo27();
	afx_msg void OnClickIo28();
	afx_msg void OnClickIo29();
	afx_msg void OnClickIo30();
	afx_msg void OnClickIo31();
	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()
};
