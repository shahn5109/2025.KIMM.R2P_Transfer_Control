#pragma once
#include "afxcmn.h"



// CfrmDebugView 폼 뷰입니다.

class CfrmDebugView : public CFormView
{
	DECLARE_DYNCREATE(CfrmDebugView)

protected:
	CfrmDebugView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmDebugView();

public:
	enum { IDD = IDD_FORMVIEW_DEBUG };
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
	void ListClear();
	void ListPrint(CString strText);



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
	CListCtrl m_ListCtrl;
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
};


