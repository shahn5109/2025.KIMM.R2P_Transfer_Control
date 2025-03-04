//{{AFX_INCLUDES()
#include "comigraph.h"
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


#include "resource.h"

// CfrmMainView 폼 뷰입니다.

class CfrmMainView : public CFormView
{
	DECLARE_DYNCREATE(CfrmMainView)

protected:
	CfrmMainView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmMainView();

public:
	enum { IDD = IDD_FORMVIEW_MAIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CListCtrl	m_ListCtrl;

	CComiGraph	m_cnGraph1;

	CBtnEnh m_btnFoot_CW;
	CBtnEnh m_btnReady;

	CBtnEnh	m_lblBase[_MAX_RUN_PARAM];
	CBtnEnh	m_lblRunEnable[_MAX_RUN_PARAM];

	CBtnEnh	m_lblRun_RollMode_Roll[_MAX_RUN_PARAM];

	CBtnEnh m_lblAlignMode_None[_MAX_RUN_PARAM];
	CBtnEnh	m_lblAlignMode_Auto[_MAX_RUN_PARAM];
	CBtnEnh	m_lblAlignMode_Manu[_MAX_RUN_PARAM];

	CBtnEnh m_lblRun_Speed[_MAX_RUN_PARAM];
	CBtnEnh m_lblRun_Length[_MAX_RUN_PARAM];
	CBtnEnh m_lblRun_Force[_MAX_RUN_PARAM];
	CBtnEnh m_lblRun_DownSpeed[_MAX_RUN_PARAM];
	CBtnEnh m_lblRun_UpSpeed[_MAX_RUN_PARAM];
	CBtnEnh m_lblRun_Delay[_MAX_RUN_PARAM];


	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;

	int Max_X1_Cnt,		Max_X2_Cnt,		Max_X3_Cnt,		Max_X4_Cnt;
	void GraphDataInital() ;
	double Graph_Y1_Max[7];
	double Graph_Y2_Max[7];
	double Graph_Y3_Max[7];
	double Graph_Y4_Max[7];
	int AD_BufferRecCount1, AD_BufferRecCount2, AD_BufferRecCount3, AD_BufferRecCount4;
	bool bGr_Flag1, m_bFootSW_Flag;

	int ListCnt;

public:
	void ListUpdate();

	void DataUpdate();
	void DataSave();
	bool AutoRunStart();
	void StopProcess();
	void PauseProcess();

	void EndGraph(int ch);

	LRESULT DataReload(WPARAM wParam,LPARAM lParam);
	LRESULT LoadCell_Get_Data1(WPARAM wParam,LPARAM lParam);
	LRESULT LoadCell_Get_Data2(WPARAM wParam,LPARAM lParam);
	LRESULT LoadCell_Get_Data3(WPARAM wParam,LPARAM lParam);
	LRESULT LoadCell_Get_Data4(WPARAM wParam,LPARAM lParam);
	LRESULT LoadCell_Get_Data5(WPARAM wParam,LPARAM lParam);

	CParameterFile*			ParamFile();
	CParameterFile*			Log();

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
	void ClickOriginReturn3();
	void ClickOriginReturn();
	void ClickOriginReturn2();
	void ClickReady();
	void DblClickCommand();
	void DblClickCommand2();
	void ClickVacuumOn4();
	void ClickVacuumOn2();
	afx_msg void OnBnClickedButton1();
	void ClickVacOnoff1();
	void ClickVacOnoff2();
	void ClickVacOnoff4();
	void ClickVacOnoff5();
	void DblClickCurpos34();
	void DblClickComigraphctrl1();
	void ClickCmManualSpeed();
	void ClickCmMove11();
	void ClickCmMove12();
	void ClickCmMove13();
	BOOL m_Wait_Flag;
	afx_msg void OnBnClickedCheck2();
	void ClickDataSave9();
};


