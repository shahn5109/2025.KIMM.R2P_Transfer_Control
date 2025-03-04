//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once



// CfrmCalibView 폼 뷰입니다.

class CfrmCalibView : public CFormView
{
	DECLARE_DYNCREATE(CfrmCalibView)

protected:
	CfrmCalibView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmCalibView();

public:
	enum { IDD = IDD_FORMVIEW_CALIB };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	double m_dPtpMove_mm;

	CBtnEnh	m_btnJogModeSel1;
	CBtnEnh	m_btnReady;
	CBtnEnh m_btnStageSel1, m_btnZ_InterLock;

	CBtnEnh m_btnCopySel[_MAX_AXIS];

	CBtnEnh	m_cCurPos[_MAX_AXIS];
	double m_dPosition[_MAX_AXIS];
	double m_dPosition_Offset[_MAX_AXIS];

	int Pos_Sel;
	int Jog_Mode;
	int iStageJog_Select;

	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;

public:
	void DispUpdate();
	void JogDispUpdate();
	void DataUpdate();
	void DataSave();

	BOOL MoveEndCheck(int Axis) ;
	BOOL MoveEndCheckNonDisp(int Axis) ;
	BOOL MovePos(int pos, BOOL Z_Flag, BOOL dispFlag);

	BOOL TopCam_Calib();
	BOOL BotCam_Calib();

	bool proc_run_flag;

	CParameterFile*			ParamFile();
	CParameterFile*			Log();
	CMatrox*				m_pVision;

	LRESULT DataReload(WPARAM wParam,LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double m_dTmp;
	DECLARE_EVENTSINK_MAP()
	void ClickYPos0();
	void ClickYPos8();
	void ClickYPos1();
	void ClickYPos9();
	void ClickYPos10();
	void ClickButtonJogPopup();
	void ClickStageJogSel1();
	void ClickStageJogSel2();
	void ClickJogSelect0();
	void ClickJogSelect1();
	void ClickJogSelect2();
	void ClickJogSelect3();
	void ClickPosSet2();
	void ClickCmMove();
	void ClickCmMove2();
	void ClickZWaitPos();
	void ClickZWaitPos7();
	void ClickCmManualSpeed();
	void ClickCmManualSpeed2();
	void ClickDataSave();
	void MouseDownJogXCw(short Button, short Shift, long x, long y);
	void MouseUpJogXCw(short Button, short Shift, long x, long y);
	void MouseDownJogXCcw(short Button, short Shift, long x, long y);
	void MouseUpJogXCcw(short Button, short Shift, long x, long y);
	void MouseDownJogPbCw(short Button, short Shift, long x, long y);
	void MouseUpJogPbCw(short Button, short Shift, long x, long y);
	void MouseDownJogPbCcw(short Button, short Shift, long x, long y);
	void MouseUpJogPbCcw(short Button, short Shift, long x, long y);
	void MouseDownJogZ1Ccw(short Button, short Shift, long x, long y);
	void MouseUpJogZ1Ccw(short Button, short Shift, long x, long y);
	void MouseDownJogZ2Ccw(short Button, short Shift, long x, long y);
	void MouseUpJogZ2Ccw(short Button, short Shift, long x, long y);
	void MouseDownJogZ2Cw(short Button, short Shift, long x, long y);
	void MouseUpJogZ2Cw(short Button, short Shift, long x, long y);
	void MouseDownJogZCcw(short Button, short Shift, long x, long y);
	void MouseUpJogZCcw(short Button, short Shift, long x, long y);
	void MouseDownJogZCw(short Button, short Shift, long x, long y);
	void MouseUpJogZCw(short Button, short Shift, long x, long y);
	void MouseDownJogZ1Cw(short Button, short Shift, long x, long y);
	void MouseUpJogZ1Cw(short Button, short Shift, long x, long y);
	void MouseDownJogStageYCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageYCcw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageYCw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageYCw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCcw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCw(short Button, short Shift, long x, long y);
	void MouseDownJogCameraYCcw(short Button, short Shift, long x, long y);
	void MouseUpJogCameraYCcw(short Button, short Shift, long x, long y);
	void MouseDownJogCameraYCw(short Button, short Shift, long x, long y);
	void MouseUpJogCameraYCw(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCw(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCw(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCcw(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCcw(short Button, short Shift, long x, long y);
	afx_msg void OnBnClickedButton2();
	void ClickVacuumStage1On();
	void ClickVacuumStage1Off();
	void ClickVacuumStage2On();
	void ClickVacuumStage2Off();
	double m_dTopCam20To2_X;
	double m_dTopCam20To2_Z;
	double m_dBotCam20To2_X;
	double m_dBotCam20To2_Z;
	double m_dRotCen_PosCamY_20X;
	double m_dRotCen_PosStageX_20X;
	void ClickCal1();
	double m_dTopCam20To2_Y;
	double m_dBotCam20To2_Y;
	double m_dStartX1;
	double m_dEndX;
	double m_dStartY;
	double m_dEndY;
	double m_dAngle;
	void ClickZWaitPos4();
	void ClickStageJogSel5();
	afx_msg void OnBnClickedButton3();
	void ClickJogXCw2();
	void MouseDownJogXCw2(short Button, short Shift, long x, long y);
	void MouseUpJogXCw2(short Button, short Shift, long x, long y);
	void MouseDownJogXCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogXCcw2(short Button, short Shift, long x, long y);
	void ClickCmMove5();
	void ClickCmMove4();
	double m_dStage1ToStage2_Dist;
	double m_dStage2ToStage3_Dist;
	double m_dQuartz_PosX;
	double m_dQuartz_PosY;
	double m_dRollPlateStamp_PosX;
	double m_dRollPlateStamp_PosY;
	double m_dBot_Cam_20X_Offset_X;
	double m_dBot_Cam_20X_Offset_Y;
	void ClickCal2();
	double m_dStampMoveOffset_X;
	void ClickCal3();
};


