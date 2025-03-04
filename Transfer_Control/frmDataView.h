//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once



// CfrmDataView 폼 뷰입니다.

class CfrmDataView : public CFormView
{
	DECLARE_DYNCREATE(CfrmDataView)

protected:
	CfrmDataView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmDataView();

public:
	CBtnEnh	m_lblRunEnable[_MAX_RUN_PARAM];
	CBtnEnh	m_lblRunDisable[_MAX_RUN_PARAM];
	CBtnEnh	m_lblRun_RollMode_Roll[_MAX_RUN_PARAM];
	CBtnEnh	m_lblRun_RollMode_Plate[_MAX_RUN_PARAM];
	CBtnEnh	m_lblAlignMode_Auto[_MAX_RUN_PARAM];
	CBtnEnh	m_lblAlignMode_Manu[_MAX_RUN_PARAM];
	CBtnEnh m_lblAlignMode_None[_MAX_RUN_PARAM];
	CBtnEnh m_btnRollPosMode_Dis, m_btnRollPosMode_En;
	
	int m_iWaitFlag[_MAX_RUN_PARAM];
	
	double m_dRun_Offset_X[_MAX_RUN_PARAM];
	double m_dRun_Offset_Y[_MAX_RUN_PARAM];

	double m_dAlignMarkToMark[_MAX_RUN_PARAM];
	double m_dAlignMarkToMarkY[_MAX_RUN_PARAM];

	double m_dRoll_Diameter[_MAX_RUN_PARAM];
	double m_dRoll_Diameter_AddVal[_MAX_RUN_PARAM];
	double m_dRun_Speed[_MAX_RUN_PARAM];
	double m_dRun_Length[_MAX_RUN_PARAM];
	double m_dRun_Force[_MAX_RUN_PARAM];
	double m_dRun_Force2[_MAX_RUN_PARAM];
	double m_dRun_DownSpeed[_MAX_RUN_PARAM];
	double m_dRun_UpSpeed[_MAX_RUN_PARAM];
	int m_iRun_Delay[_MAX_RUN_PARAM];
	double m_dZ_Base_Position[_MAX_RUN_PARAM];

	int m_iRunEnable[_MAX_RUN_PARAM];
	int m_iAlignEnable[_MAX_RUN_PARAM];
	int m_iAlignMode[_MAX_RUN_PARAM];
	int m_iRun_RollMode[_MAX_RUN_PARAM];

	BOOL m_iForceControlEnableFlag[_MAX_RUN_PARAM];
	double m_dZ_Control_Fast_Length[_MAX_RUN_PARAM];
	double m_dZ_Control_Slow_Length[_MAX_RUN_PARAM];
	int m_iZ_Control_Mode[_MAX_RUN_PARAM];

	CBtnEnh m_btnPos_Mode_S1;
	CBtnEnh m_btnForce_Mode_S1;
	CBtnEnh m_btnPos_Mode_S2;
	CBtnEnh m_btnForce_Mode_S2;
	CBtnEnh m_btnPos_Mode_S3;
	CBtnEnh m_btnForce_Mode_S3;


	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;

	int m_iDownEnd_WaitFlag;

public:
	void DataUpdate();
	void DataSave();

	LRESULT DataReload(WPARAM wParam,LPARAM lParam);

	CParameterFile*			ParamFile();
	CParameterFile*			Log();

public:
	enum { IDD = IDD_FORMVIEW_DATA };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	DECLARE_EVENTSINK_MAP()
	void ClickOpuRollZ27();
	void ClickOpuRollZ25();
	void ClickOpuRollZ1();
	void ClickOpuRollZ4();
	void ClickOpuRollZ39();
	void ClickOpuRollZ2();
	void ClickOpuRollZ3();
	void ClickOpuRollZ29();
	void ClickOpuRollZ31();
	void ClickOpuRollZ37();
	void ClickOpuRollZ33();
	void ClickOpuRollZ35();
	void ClickOpuRollZ30();
	void ClickOpuRollZ32();
	void ClickOpuRollZ38();
	void ClickOpuRollZ34();
	void ClickOpuRollZ36();
	void ClickDataSave();
	void ClickOpuS1AD2();
	void ClickOpuS1AA2();
	void ClickOpuS1AD3();
	void ClickOpuS1AA3();
	void ClickOpuRollZ40();
	void ClickOpuRollZ41();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	double m_dStamp_Len_X;
	double m_dStamp_Len_Y;
	double m_dQuartz_PosX;
	double m_dQuartz_PosY;
	afx_msg void OnBnClickedButton2();
	double dRollPlateStamp_PosX;
	double dRollPlateStamp_PosY;
	void ClickOpuRollZ42();
	void ClickOpuRollZ43();
	void ClickOpuS2AD2();
	void ClickOpuS2AA2();
	void ClickOpuS2AM2();
	void ClickOpuS1AD4();
	void ClickOpuS1AA4();
	afx_msg void OnBnClickedCheck4();
	void ClickOpuRollZ52();
	void ClickOpuRollZ53();
	double m_dZ_SlowDownLen;
	int m_iRoll_AngleAlignMode;
	double m_dRoll_AngleAlign_Pitch;
	int m_iRoll_AngleAlign_Count;
	double m_dLED_Pitch_X;
	double m_dLED_Pitch_Y;
	double m_dShift_CamY;
};


