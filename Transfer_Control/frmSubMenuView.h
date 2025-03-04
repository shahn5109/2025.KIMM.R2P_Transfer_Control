//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

#pragma once



// CfrmSubMenuView 폼 뷰입니다.

class CfrmSubMenuView : public CFormView
{
	DECLARE_DYNCREATE(CfrmSubMenuView)

protected:
	CfrmSubMenuView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmSubMenuView();

public:
	enum { IDD = IDD_FORMVIEW_SUBMENU };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CBtnEnh	m_btnMain;
	CBtnEnh	m_btnMenu;
	CBtnEnh	m_btnSetup;
	CBtnEnh	m_btnTeach;
	CBtnEnh	m_btnAlign;
	CBtnEnh	m_btnCalib;
	CBtnEnh	m_btnStamp;
	CBtnEnh	m_btnComp;
	CBtnEnh	m_btnLed1, m_btnLed2, m_btnLed3, m_btnLed4;
	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;

	int iLed_Calib;
public:
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
	void ClickYPos0();
	void ClickYPos1();
	void ClickYPos2();
	void ClickYPos3();
	void ClickYPos4();
	void ClickYPos5();
	void ClickYPos6();
	void ClickOriginReturn2();
	void ClickOriginReturn3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	void ClickLedSel1();
	void ClickLedSel2();
	void ClickLedSel3();
	void ClickLedSel4();
	void ClickYPos14();
};


