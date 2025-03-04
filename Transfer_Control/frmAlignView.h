//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CommProc.h"
#include "PComm.h"

// CfrmAlignView 폼 뷰입니다.

class CfrmAlignView : public CFormView
{
	DECLARE_DYNCREATE(CfrmAlignView)

protected:
	CfrmAlignView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmAlignView();

public:
	enum { IDD = IDD_FORMVIEW_ALIGN };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CStatic	m_ModelEditDisplay_L;
	double  m_DispZoom;

	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;
	CBtnEnh m_btnManuStamp_TopUsed;
	CBtnEnh m_btnManuStamp_BtmUsed;

	
	CBtnEnh m_btnTopCam, m_btnAreaROI;
	CBtnEnh m_btnBotCam;
	CBtnEnh m_btnAlignStage1Mark;
	CBtnEnh m_btnAlignStage2Mark;
	CBtnEnh m_btnAlignRollMark;
	CBtnEnh m_btnMaskMark, m_btnPlateStampMark;
	CBtnEnh m_btnSel2X;
	CBtnEnh m_btnSel20X;
	CBtnEnh m_btnSel_1st;
	CBtnEnh m_btnSel2nd;
	CBtnEnh m_btnROI_Set, m_btnManuRollAlign;
	BOOL m_bAreaROI;
public:
	void ReCOM_Conn();
	//----------------------------------------
	// Multi-Thread
	CWinThread *m_pThread_1[3];
	BOOL	m_bExec_1[3];
	void RunThread_1(int nID);
	void KillThread_1(int nID);

	BOOL mAlignProcStart;
	int  mAlignProc_Step;
	void AlignProc_Start();
	bool isAlignProc_End();
	void AlignProc_End();
	//----------------------------------------

	CParameterFile*			ParamFile();
	CParameterFile*			Log();
	CMatrox*				m_pVision;
	CCommProc				LED_cnt;

	void DataInit();
	void DataSave();
	void SetLedVal();

	bool m_ROI_Mode, ComFlag1;
	bool proc_run_flag;
	BOOL bAlign_End_Flag;

	void Pattern_Model_Load(int model_no);
	void SetRoi_Confirm();
	
	int GetModelNo(int no1, int no2, int no3, int no4, int no5 ); //CamNo, LensNo, UsedNo, MarkNo, StageNo
	//---------------------------------------
	int ModelNo;
	int CamNo, UsedNo, LensNo, MarkNo, ModelGroupNo, Stage_Proc;
	int m_radioCameraIdx;

	int m_manuStamp_Camera;
	CRect	m_dispRect_L;
	CRect	disp_rect_L;

	//---------------------------------------
	LRESULT SetRoi_Confirm(WPARAM wParam,LPARAM lParam);
	LRESULT DataReload(WPARAM wParam,LPARAM lParam);
	LRESULT SetLedUpdate(WPARAM wParam,LPARAM lParam);
	//---------------------------------------
	BOOL MoveEndCheckNonDisp(int Axis) ;
	BOOL MoveEndCheck(int Axis, BOOL dispflag= TRUE) ;
	BOOL MovePos(int pos, BOOL Z_Flag, BOOL dispFlag= TRUE, BOOL AT_Flag= FALSE);

	BOOL RollAlignProc(BOOL AtMode= FALSE);
	BOOL PlateAlignProc(int stageNo);
	BOOL Align_Proc(bool dispflag= true);
	BOOL Move_AlignEndPos(int stage_no, double *retX, double *retY1);
	BOOL PP_Pos_Move(int no, bool dispflag= true);
	BOOL PP_Pos_Move_Test(int no, bool dispflag= true);
	BOOL NonAling_PP_Pos_Move(int no, bool dispflag= true);
	BOOL APP_Pos_Move(int no);
	void SetLed(int flagVal);

	void ResetEditCtrl();
	void RollAlignDataClear();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickJogSelectTopcam();
	void ClickJogSelectBotcam();
	void ClickJogSelectAlign();
	void ClickJogSelectMask();
	void ClickJogSelect2x();
	void ClickJogSelect20x();
	void ClickJogSelect1st();
	void ClickJogSelect2nd();
	void ClickMarkRoiSel();
	void ClickMarkSave();
	void ClickMarkTest();
	CComboBox m_ctrlSpeed;
	CComboBox m_ctrlAccuracy;
	double m_dScoreCertanty;
	double m_dScoreAcceptance;
	int m_TimeOut;
	double m_dOffsetX;
	double m_dOffsetY;
	int m_nBrushSize_L;
	CSliderCtrl m_ctrlBrushSlider_L, m_ctrlLedSlider;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	double m_dAlignMarkToMark;
	int m_iMarkMaxCount;
	double m_dAlignAccuracy_umXY;
	double m_dAlignAccuracy_umT;
	double m_dAlignOffset_X;
	double m_dAlignOffset_Y;
	double m_dAlignOffset_T;
	void ClickCmMove();
	void ClickMarkTest2();
	void ClickJogSelectAlign2();
	void ClickJogSelectAlign3();
	afx_msg void OnBnClickedButton2();
	void ClickJogSelectTused();
	void ClickJogSelectBused();
	void ClickMarkTest3();
	void ClickCmMove6();
	void ClickCmMarkLoad();
	double m_dOffsetTest_X;
	double m_dOffsetTest_Y;
	afx_msg void OnBnClickedButton5();
	void ClickJogSelectMask2();
	double m_dAlignMarkToMarkY;
	void ClickJogSelectMask3();
	afx_msg void OnBnClickedCheckMaskSet();
	BOOL m_bMaskEdit_L;
	afx_msg void OnBnClickedMaskEditApply();
	afx_msg void OnBnClickedMaskEditClear();
	afx_msg void OnNMCustomdrawBrushSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void ClickMarkTest5();
	afx_msg void OnBnClickedButton3();
	int m_iLedcontrolVal;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
	void ClickMarkRoiSel2();
	void ClickMarkRoiSel3();
	void ClickMarkRoiSel4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	int m_iMovePix_Sel;
	afx_msg void OnBnClickedButtonU();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedButtonL2();
	afx_msg void OnBnClickedButtonR2();
	int m_iMarkSearchCount;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	int m_iRollAlignModeFlag;
	void ClickMarkRoiSel5();
};
typedef struct tagTREADPARAMS_ALIGN { 
	BOOL *pContinue;
	CWnd *pWnd;
} TREADPARAMS_ALIGN;

UINT Thread_Align_Proc_1(LPVOID pParam);


