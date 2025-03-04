//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


// CdlgOrigin ��ȭ �����Դϴ�.

class CdlgOrigin : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgOrigin)

public:
	CdlgOrigin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CdlgOrigin();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ORIGIN };

public:
	CBtnEnh	m_btnStage_X;
	CBtnEnh	m_btnBlk_Roll;
	CBtnEnh	m_btnRoll_Z;
	CBtnEnh	m_btnAlign_Y;
	CBtnEnh	m_btnTop_Cam_Y;
	CBtnEnh	m_btnTop_Cam_Z;
	CBtnEnh	m_btnBot_Cam_X;
	CBtnEnh	m_btnBot_Cam_Y;
	CBtnEnh	m_btnBot_Cam_Z;
	CBtnEnh	m_btnPower;

	DWORD dwCheckTimeStart_Total;
	DWORD dwCheckTimeStart_Axis;


	int Auto_Origin_Sequence();

	int HomeReturnCheck(long axis);


private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickOk();
	afx_msg void OnBnClickedButtonCheck();
	void ClickUnitInitAllSelect();
	void ClickServoOn();
	void ClickServoOff();
	void ClickReady2();
	void ClickInitRun();
};
