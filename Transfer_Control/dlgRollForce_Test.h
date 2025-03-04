//{{AFX_INCLUDES()
#include "comigraph.h"
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once

#include "../Common/ImageView/Include/xImageViewCtrl.h"
#include "afxcmn.h"

// CdlgRollForce_Test 대화 상자입니다.

class CdlgRollForce_Test : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgRollForce_Test)

public:
	CdlgRollForce_Test(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgRollForce_Test();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANU_ROLL_TEST };

	CBtnEnh m_btnTitle;
	CBtnEnh m_radio2X;
	CBtnEnh m_radio20X;
	int mTopCameraNo, m_SelectMarkNo;

	CBtnEnh	m_btnJogModeSel1;
	CBtnEnh m_btnTestStart, m_btnTestStop;
	CBtnEnh	m_btnZ_Cnt_Disable, m_btnZ_Cnt_Enable, m_btnSync_Cnt_Disable, m_btnSync_Cnt_Enable, m_btnZMap_Cnt_Disable, m_btnZMap_Cnt_Enable;
	int Jog_Mode;

	BOOL MoveEndCheck(int Axis) ;
	void DispUpdate();

	double m_dRoll_Diameter_AddVal, m_dRoll_Diameter;

	CxImageViewCtrl		m_ImageCtrl;
	CxImageViewManager	m_ImageMng;

	CComiGraph	m_cnGraph1;
	void GraphDataInital();
	void EndGraph(int ch);
	LRESULT LoadCell_Get_Data3(WPARAM wParam,LPARAM lParam);
	double Graph_Y1_Max[7];
	double Graph_Y2_Max[7];
	double Graph_Y3_Max[7];
	double Graph_Y4_Max[7];
	int AD_BufferRecCount1, AD_BufferRecCount2, AD_BufferRecCount3, AD_BufferRecCount4;
	bool bGr_Flag1, m_bFootSW_Flag;
	int Max_X1_Cnt,		Max_X2_Cnt,		Max_X3_Cnt,		Max_X4_Cnt;

	//----------------------------------------
	// Multi-Thread
	CWinThread *m_pThread_1[2];
	BOOL	m_bExec_1[2];
	void RunThread_1(int nID);
	void KillThread_1(int nID);
	BOOL Thread_Start_Flag;
	int Thread_Z_Test_Step;
	double dZ_Test_Move_Dist;//(1)
	double dZ_Test_Move_Speed;//(0.1)
	double dZ_Test_Move_Speed_Up;//(0.1)
	double dZ_Test_Move_Delay;//(1000)
	void TestError();
	bool b_Graph_Start;
	bool bGrapgRealDisp;
	double dCurrZ1_Pos, dCurrZ2_Pos;
	bool bBotX_TestMove;
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
	BOOL m_bRollZ_Enable;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void ClickJogSelect0();
	void ClickJogSelect1();
	void ClickJogSelect2();
	void ClickExit();
	void ClickExit2();
	void MouseDownJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCw(short Button, short Shift, long x, long y);
	void MouseDownJogZCcw(short Button, short Shift, long x, long y);
	void MouseUpJogZCcw(short Button, short Shift, long x, long y);
	void MouseDownJogZCw(short Button, short Shift, long x, long y);
	void MouseUpJogZCw(short Button, short Shift, long x, long y);
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedButton1();
	void ClickZWaitPos3();
	void ClickOpuZCntDis();
	void ClickOpuZCntEna();
	void ClickDataSet();
	double m_dZ_ControlForce_L;
	double m_dZ_ControlForce_R;
	void MouseDownJogStageXCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCcw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCw2(short Button, short Shift, long x, long y);
	void MouseDownJogCameraYCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogCameraYCcw2(short Button, short Shift, long x, long y);
	void MouseDownJogCameraYCw2(short Button, short Shift, long x, long y);
	void MouseUpJogCameraYCw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCw3(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCw3(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCcw3(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCcw3(short Button, short Shift, long x, long y);
	void ClickZWaitPos5();
	void ClickJogSelect3();
	double m_dPtpMove_mm;
	afx_msg void OnBnClickedButton6();
	double m_dInc_MoveSpeed;
	void MouseDownJogStageYCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageYCcw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageYCw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageYCw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCw2(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCcw2(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCcw2(short Button, short Shift, long x, long y);
	void ClickOpuZCntDis2();
	void ClickOpuZCntEna2();
	void MouseDownJogZ1Ccw(short Button, short Shift, long x, long y);
	void MouseUpJogZ1Ccw(short Button, short Shift, long x, long y);
	void MouseDownJogZ2Ccw(short Button, short Shift, long x, long y);
	void MouseUpJogZ2Ccw(short Button, short Shift, long x, long y);
	void MouseDownJogZ1Cw(short Button, short Shift, long x, long y);
	void MouseUpJogZ1Cw(short Button, short Shift, long x, long y);
	void MouseDownJogZ2Cw(short Button, short Shift, long x, long y);
	void MouseUpJogZ2Cw(short Button, short Shift, long x, long y);
	void ClickJogSelect4();
	void ClickJogSelect13();
	void DblClickJogSelect4();
	void DblClickJogSelect13();
	void ClickJogSelect6();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DblClickComigraphctrl1();
	int m_iLedcontrolVal1;
	int m_iLedcontrolVal2;
	CSliderCtrl m_ctrlLedSlider1;
	CSliderCtrl m_ctrlLedSlider2;
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	void ClickZWaitPos6();
	void ClickJogZCw3();
	double m_dZ_Test_Move_Dist;
	double m_dZ_Test_Move_Speed;
	int m_dZ_Test_Move_Delay;
	afx_msg void OnBnClickedCheck5();
	BOOL m_bGrapgRealDisp;
	afx_msg void OnBnClickedButton7();
	void ContainerChangeColorZWaitPos7(long Color);
	void ClickZWaitPos7();
	double m_dZ1_Pos;
	double m_dZ2_Pos;
	void ClickJogZCw2();
	int m_iZ_ControlOption;
	double m_dBotX_MoveDistmm;
	void ClickZTestStart2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio24();
	double m_dZ_Test_Move_Speed_Up;
	void ClickCal4();
	void ClickOpuZCntDis3();
	void ClickOpuZCntEna3();
	afx_msg void OnBnClickedButton15();
	void ClickDataSave();
	BOOL m_iZ_Control_Z1Hap;
	afx_msg void OnBnClickedCheck4();
	void ClickZWaitPos61();
};
typedef struct tagTREADPARAMS_Z_TEST { 
	BOOL *pContinue;
	CWnd *pWnd;
} TREADPARAMS_Z_TEST;

UINT Thread_Z_Test_Proc_1(LPVOID pParam);


