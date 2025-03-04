//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

#pragma once


// CdlgData_MotionSpeed 대화 상자입니다.

class CdlgData_MotionSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgData_MotionSpeed)

public:
	CdlgData_MotionSpeed(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgData_MotionSpeed();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MOTOR_SPEED };


	double	m_dWork_Speed[_MAX_AXIS];
	double	m_dWork_Accel[_MAX_AXIS];
	double	m_dOrigin_Speed[_MAX_AXIS];
	double	m_dJog_MaxSpeed[_MAX_AXIS];
	double	m_dJog_MidSpeed[_MAX_AXIS];
	double	m_dJog_MinSpeed[_MAX_AXIS];
	double	m_dPulsePerMM[_MAX_AXIS];

	CBrush m_DialogBrush;

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
	afx_msg void OnClickTouchInput();
	afx_msg void OnClickDataSave();
	afx_msg void OnClickExit();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
};
