#pragma once


// CdlgRecipePrintEdit 대화 상자입니다.

class CdlgRecipePrintEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgRecipePrintEdit)

public:
	CdlgRecipePrintEdit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgRecipePrintEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_PRINT_EDIT };
	double	m_dOp1;
	double	m_dOp2;
	double	m_dOpX;
	double	m_dOpY;
	double	m_dOpT;

	double dOp1, dOp2, dOp3, dX, dY, dT;
	int iCMDNo;


// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CdlgRecipePrintEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CdlgRecipePrintEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
