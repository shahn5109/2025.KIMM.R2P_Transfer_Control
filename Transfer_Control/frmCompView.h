//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once

#include "../Common/GridCtrl_src/GridCtrl.h"


// CfrmCompView 폼 뷰입니다.

class CfrmCompView : public CFormView
{
	DECLARE_DYNCREATE(CfrmCompView)

protected:
	CfrmCompView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmCompView();

public:
	enum { IDD = IDD_FORMVIEW_X_AXIS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;

	CStatic	m_ModelEditDisplay_L;
	double  m_DispZoom;

	CBtnEnh m_btnROI_Set;
	CBtnEnh m_btnTopSel, m_btnBtmSel;
	CBtnEnh m_lblRunDisable_X, m_lblRunEnable_X;
	BOOL X_Enable_Flag;
	//---------------------------------------------------------------------------------
#define _GRID_COL	5
#define _GRID_ROW	1
	CGridCtrl m_ctrlGrid1;
	void GridInitC1_1();
	void GridDispC1_1();
	//---------------------------------------------------------------------------------
	void CalCompData();

public:
	void DispUpdate();
	void JogDispUpdate();
	void DataUpdate();
	void DataSave();
	void DataSave2();

	BOOL MoveEndCheck(int Axis) ;
	void MovePos(int pos, BOOL Z_Flag);

	CParameterFile*			ParamFile();
	CParameterFile*			Log();
	CMatrox*				m_pVision;
	COMP_DATA				CompCal_data;
	double CalData[_MAX_COMP_CAL_DATA_COUNT];
public:
	//---------------------------------------
	int ModelNo;
	int CamNo, LensNo, Stage_Proc;
	bool m_ROI_Mode;
	int m_radioCameraIdx;

	CRect	m_dispRect_L;
	CRect	disp_rect_L;

	//---------------------------------------
	LRESULT SetRoi_Confirm(WPARAM wParam,LPARAM lParam);
	LRESULT DataReload(WPARAM wParam,LPARAM lParam);

	LRESULT TopLoadImage(WPARAM wParam,LPARAM lParam);
	LRESULT BtmLoadImage(WPARAM wParam,LPARAM lParam);

	LRESULT CompDataUpdate(WPARAM wParam,LPARAM lParam);
	BOOL TopLoadOkFlag, BtmLoadOkFlag;
	//---------------------------------------
	void Pattern_Model_Load(int model_no);
	void SetRoi_Confirm();
	//---------------------------------------
	// Multi-Thread
	CWinThread *m_pThread_1[3];
	BOOL	m_bExec_1[3];
	void RunThread_1(int nID);
	void KillThread_1(int nID);
	//----------------------------------------
	BOOL mCompProcStart;
	int  mCompProc_Step;
	BOOL bComp_End_Flag;
	bool proc_run_flag;
	void CompProc_Start();
	bool isCompProc_End();
	void CompProc_End();
	double CalBtmPosData();

	void LoadCompData();
	void SaveCompData();
	void BuildCompData();
	BOOL bTopCompEnable;
	BOOL bBtmCompEnable;

	void DownLoad_X_Data();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	CComboBox m_ctrlSpeed;
	CComboBox m_ctrlAccuracy;
	double m_dScoreCertanty;
	double m_dScoreAcceptance;
	int m_TimeOut;
	double m_dOffsetX;
	double m_dOffsetY;
	int m_nBrushSize_L;
	CSliderCtrl m_ctrlBrushSlider_L, m_ctrlLedSlider;
	int m_iMovePix_Sel;
	int m_iLedcontrolVal;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_EVENTSINK_MAP()
	void ClickJogSelectAlign();
	void ClickJogSelectMask3();
	void ClickMarkRoiSel();
	void ClickMarkTest();
	void ClickMarkSave();
	afx_msg void OnBnClickedButtonU();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedButtonL2();
	afx_msg void OnBnClickedButtonR2();
	double m_dCompPitch;
	int m_iCompCountX;
	int m_iCompCountY;
	afx_msg void OnEnChangeEditSmoothness6();
	double m_dCompRectSize;
	afx_msg void OnBnClickedMaskEditApply3();
	afx_msg void OnBnClickedMaskEditApply4();
	afx_msg void OnBnClickedMaskEditApply5();
	void ClickZTestStart5();
	void ClickZTestStart6();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	BOOL m_bTopCompEnable;
	BOOL m_bBtmCompEnable;
	void ClickMarkSave2();
	void ClickZTestStart();
	void ClickZTestStart2();
	void ClickCmMove11();
	void ClickCmMove17();
	void ClickCmMove2();
	void ClickOpuRollZ30();
	void ClickOpuRollZ32();
};
typedef struct tagTREADPARAMS_COMP { 
	BOOL *pContinue;
	CWnd *pWnd;
} TREADPARAMS_COMP;

UINT Thread_Comp_Proc_1(LPVOID pParam);


