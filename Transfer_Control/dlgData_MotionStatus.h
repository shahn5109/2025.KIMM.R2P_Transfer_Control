//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

#pragma once


// CdlgData_MotionStatus 대화 상자입니다.

class CdlgData_MotionStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgData_MotionStatus)

public:
	CdlgData_MotionStatus(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgData_MotionStatus();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MOTOR_STATUS };

public:
	CBtnEnh	m_btnRDY[_MAX_AXIS];
	CBtnEnh	m_btnSvrOn[_MAX_AXIS];
	CBtnEnh	m_btnPlim[_MAX_AXIS];
	CBtnEnh	m_btnNlim[_MAX_AXIS];
	CBtnEnh	m_btnOrg[_MAX_AXIS];

	double m_dSW_Limit_High[_MAX_AXIS];
	double m_dSW_Limit_Low[_MAX_AXIS];

	void DataDispUpdate();

	void DataUpdate();
	void DataSave();

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickExit1();
	afx_msg void OnClickDataSave();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};
