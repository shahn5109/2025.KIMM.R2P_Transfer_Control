#if !defined(AFX_DLGMODELNAMEINPUT_H__7391A5DF_009C_4ECE_98D3_18036F0267BE__INCLUDED_)
#define AFX_DLGMODELNAMEINPUT_H__7391A5DF_009C_4ECE_98D3_18036F0267BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgModelNameInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CdlgModelNameInput dialog

class CdlgModelNameInput : public CDialog
{
	// Construction
public:
	CdlgModelNameInput(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CdlgModelNameInput)
	enum { IDD = IDD_DIALOG_MODELNAME_MODIFY };
	CString	m_strAfterModelName;
	//}}AFX_DATA

	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush, m_YellowBrush, m_BlueBrush, m_RedBrush, m_GrayBrush;
	CString strBeforeModelNAme;
	CString	strAfterModelName;
	int inputmodelType;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdlgModelNameInput)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CParameterFile* ParamFile();	
	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CdlgModelNameInput)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnClickOk();
	afx_msg void OnClickCancel();
	afx_msg void OnClickLabelCurModelname();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODELNAMEINPUT_H__7391A5DF_009C_4ECE_98D3_18036F0267BE__INCLUDED_)
