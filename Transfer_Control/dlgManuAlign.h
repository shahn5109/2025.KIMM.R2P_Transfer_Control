//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once

#include "../Common/ImageView/Include/xImageViewCtrl.h"

// CdlgManuAlign 대화 상자입니다.

class CdlgManuAlign : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgManuAlign)

public:
	CdlgManuAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgManuAlign();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANU_ALIGN };

public:
	CBtnEnh m_btnTitle, m_btnAlignModeSel, m_btnAlignModeSel1;
	CBtnEnh m_radio2X;
	CBtnEnh m_radio20X;
	CBtnEnh m_btnSelectPoint;
	double m_dPtpMove_mm;

	CBtnEnh	m_btnJogModeSel1;
	CBtnEnh m_btnStageSel1;

	int Jog_Mode;
	int iStageJog_Select;
	bool Align_Pos_Set_Flag;
	int iSelect_B_Layer;

	int iStamInstRunFlag;

	CxImageViewCtrl		m_ImageCtrl;
	CxImageViewManager	m_ImageMng;

	CPoint				m_pt1st;
	CPoint				m_pt2nd;
	int					m_nClickAble;

	int mTopCameraNo, m_SelectMarkNo;
	int iSelMarkPos;

	//---------------------------------------------
	int m_StageNo, CameraStageUsedNo; //사용 카메라 스테이지 번호
	//---------------------------------------------
	int m1st_Mark_Number, m2nd_Mark_Number;

	BOOL MoveEndCheck(int Axis) ;
	void DispUpdate();
	BOOL Cal_Align();
	BOOL MoveEndCheckNonDisp(int Axis) ;
	BOOL Cal_Alig_Manual();


	double Select_Pos_X1,Select_Pos_Y1;
	double Select_Pos_X2,Select_Pos_Y2;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	static void APIENTRY OnDrawExt( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData );
	static BOOL APIENTRY OnFireMouseEvent(  DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void ClickExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
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
	void ClickJogSelect3();
	void ClickJogSelect4();
	void ClickStageJogSel1();
	void ClickStageJogSel2();
	virtual BOOL DestroyWindow();
	void DblClickJogSelect3();
	void DblClickJogSelect4();
	void ClickJogSelect6();
	void ClickJogSelect7();
	void ClickStageJogSel3();
	void ClickStageJogSel4();
	double m_dCamPos_1st_X;
	double m_dCamPos_1st_Y;
	double m_dCamPos_2nd_X;
	double m_dCamPos_2nd_Y;
	double m_dResult_Len;
	double m_dResult_Deg;
	void ClickExit2();
	void ClickJogSelect0();
	void ClickJogSelect1();
	void ClickJogSelect2();
	void ClickJogSelect8();
	void ClickJogSelect9();
	void ClickCmMove();
	void ClickCmMove6();
	void ClickJogSelect10();
	void ClickJogSelect11();
	void ClickCmMove7();
	void ClickJogSelect12();
	afx_msg void OnBnClickedRadio1();
	int m_iAlignExc;
	double m_dStX_1;
	double m_dStY_1;
	double m_dStX_2;
	double m_dStY_2;
	void ClickCmMove8();
	BOOL m_bOverlay_Disable;
	afx_msg void OnBnClickedCheck1();
	void ClickCmMove9();
	BOOL m_bRollZ_Enable;
	afx_msg void OnBnClickedCheck3();
	void MouseDownJogZCcw(short Button, short Shift, long x, long y);
	void MouseUpJogZCcw(short Button, short Shift, long x, long y);
	void MouseDownJogZCw(short Button, short Shift, long x, long y);
	void MouseUpJogZCw(short Button, short Shift, long x, long y);
	void DblClickCmMarkLoad();
	double m_dCur_Mark_Rect;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	int m_iPixSel;
	afx_msg void OnBnClickedButtonT();
	afx_msg void OnBnClickedButtonB();
	afx_msg void OnBnClickedButtonL();
	afx_msg void OnBnClickedButtonR();
	void MouseDownJogStageTCw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCcw2(short Button, short Shift, long x, long y);
	void ClickJogCameraYCcw3();
	double m_dPitchSpeed;
	BOOL m_bPitchSpeedSel;
};
