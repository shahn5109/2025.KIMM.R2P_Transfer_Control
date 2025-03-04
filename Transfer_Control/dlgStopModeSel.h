//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


// CdlgStopModeSel 대화 상자입니다.

class CdlgStopModeSel : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgStopModeSel)

public:
	CdlgStopModeSel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgStopModeSel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STOP_SEL };

	CBtnEnh	m_btnStartNPause;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();
	afx_msg void OnClickEndStop();
	afx_msg void OnClickPause();
	afx_msg void OnClickEnd();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};
