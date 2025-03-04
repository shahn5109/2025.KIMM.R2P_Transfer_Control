//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once

//#include "CtrlInclude.h"

// CdlgAutoRunInfo 대화 상자입니다.

class CdlgAutoRunInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgAutoRunInfo)

public:
	CdlgAutoRunInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgAutoRunInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_START_INFO };

// 	CXPGroupBox	m_GroupBox1;
// 	CXPGroupBox	m_GroupBox2;
// 	CXPGroupBox	m_GroupBox4;
// 	CXPGroupBox	m_GroupBox3;
	BOOL	m_bVac_AutoStartFlag;
	int		m_iVac_AutoStart_Delay;
	CBtnEnh	m_btnMode5;
	CBtnEnh	m_btnMode6;
	int		m_iPROCESS_CYCLE_COUNT;
	CString	m_strReportPath;
	CString	m_strUserName;
	CString	m_strUseInk;
	CString	m_strUseBlanket;
	double	m_dUseTemp;
	double	m_dUseHumidity;
	CString	m_strMemo;
	BOOL	m_bImageLogDisplay;
	double	m_dPressure;
	double	m_dWaitTime_Off;
	double	m_dWaitTime_Set;
	double	m_dStart_Speed;
	double	m_dApply_Speed;
	double	m_dPrint_Speed;
	CBrush m_DialogBrush;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnClickTopFeedMode5();
	afx_msg void OnClickTopFeedMode6();
	virtual void OnOK();
	afx_msg void OnClickTopFeedMode7();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickTopFeedMode8();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
