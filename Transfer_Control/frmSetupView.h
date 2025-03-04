//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

#pragma once



// CfrmSetupView 폼 뷰입니다.

class CfrmSetupView : public CFormView
{
	DECLARE_DYNCREATE(CfrmSetupView)

protected:
	CfrmSetupView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmSetupView();

public:
	enum { IDD = IDD_FORMVIEW_SETUP };
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

public:
	void DataUpdate();
	void DataSave();

	LRESULT DataReload(WPARAM wParam,LPARAM lParam);

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
	int m_HOME_TIME_OUT;
	int m_AXIS_MOVE_TIME_OUT;
	double m_dTop_Cam_PixelToum_2X;
	double m_dTop_Cam_PixelToum_20X;
	double m_dBot_Cam_PixelToum_2X;
	double m_dBot_Cam_PixelToum_20X;
	int m_iImageGrab_Delay;
	DECLARE_EVENTSINK_MAP()
	void ClickDataSave();
	double m_dZ_Axis_Interlock;
	afx_msg void OnBnClickedButton2();
	double m_X_SAFE_FREEZONE_L;
	double m_X_SAFE_FREEZONE_H;
	double m_X_SAFE_ZONE_L1;
	double m_X_SAFE_ZONE_L2;
	double m_dX_Axis_Interlock;
	double m_AF_Stepmm_20X;
	double m_AF_Stepmm_2X;
	int m_iAF_TotalCount;
	int iCAM_20X_USED_FLAG;
	double m_dHome_Offset_BX;
	double m_Q_Offset_2X;
	double m_Q_Offset_20X;
	double m_Roll_Offset_2X;
	double m_Roll_Offset_20X;
	double m_Q_Offset_2X_Y;
	double m_Q_Offset_20X_Y;
	double m_Roll_Offset_2X_Y;
	double m_Roll_Offset_20X_Y;
	afx_msg void OnBnClickedRadio1();
	int m_Led_Remote;
	afx_msg void OnBnClickedRadio3();
	double m_dRollAlign_ZPos;
	int m_iRollTeachingDataUsed;
	afx_msg void OnBnClickedRadio7();
	double m_dRollRun_BZ_Pos;
	double m_dRectSizeX;
	double m_dRectSizeY;
	afx_msg void OnBnClickedRadio18();
	int m_iRoll_AngleAlignMode;
	double m_dRoll_AngleAlign_Pitch;
	int m_iRoll_AngleAlign_Count;
	double m_dZ2HomeOffsetVal;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck3();
	BOOL m_bRollAF_Enable;
};


