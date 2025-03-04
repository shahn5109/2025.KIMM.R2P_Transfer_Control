#pragma once



// CfrmTitleView �� ���Դϴ�.

class CfrmTitleView : public CFormView
{
	DECLARE_DYNCREATE(CfrmTitleView)

protected:
	CfrmTitleView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CfrmTitleView();

public:
	enum { IDD = IDD_FORMVIEW_TITLE };
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
	bool					bEMO_CheckFlag;
	CParameterFile*			ParamFile();
	CParameterFile*			Log();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnInitialUpdate();
	DECLARE_EVENTSINK_MAP()
	void ClickBtnExit();
};


