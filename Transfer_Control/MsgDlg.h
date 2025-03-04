#pragma once
#include "afxwin.h"

//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

// CMsgDlg 대화 상자입니다.
#include "resource.h"

class CMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgDlg)

public:
	CMsgDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMsgDlg();

public:
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_MYMSGBOX };

public:
	CBrush m_brBckColor;
	TCHAR m_strNo[50];
	TCHAR m_strYes[50];
	int m_textHeight;
	int m_cy;
	int m_cx;
	TCHAR m_title[100];
	TCHAR m_message[500];
	UINT m_icontype;
	UINT m_type;

	bool m_bFlag;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBtnEnh	m_bMyAbort;
	CBtnEnh	m_bMyCancel;
	CBtnEnh	m_bMyCancel3;
	CBtnEnh	m_bMyIgnore;
	CBtnEnh	m_bMyNo;
	CBtnEnh	m_bMyOk;
	CBtnEnh	m_bMyOk2;
	CBtnEnh	m_bMyRetry;
	CBtnEnh	m_bMyRetry3;
	CBtnEnh	m_bMyYes;
	CBtnEnh	m_MyIcon;
	CBtnEnh	m_lblMsg;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void OnClickMyabort();
	void OnClickMyok();
	void OnClickMyyes();
	void OnClickMyretry3();
	void OnClickMyok2();
	void OnClickMyretry();
	void OnClickMycancel();
	void OnClickMycancel3();
	void OnClickMyno();
	void OnClickMyignore();

};
