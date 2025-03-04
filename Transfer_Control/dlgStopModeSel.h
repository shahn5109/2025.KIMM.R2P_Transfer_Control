//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


// CdlgStopModeSel ��ȭ �����Դϴ�.

class CdlgStopModeSel : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgStopModeSel)

public:
	CdlgStopModeSel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CdlgStopModeSel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STOP_SEL };

	CBtnEnh	m_btnStartNPause;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();
	afx_msg void OnClickEndStop();
	afx_msg void OnClickPause();
	afx_msg void OnClickEnd();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};
