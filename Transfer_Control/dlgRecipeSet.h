//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


// CdlgRecipeSet 대화 상자입니다.

class CdlgRecipeSet : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgRecipeSet)

public:
	CdlgRecipeSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgRecipeSet();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RECIPE_SET };

public:
	CListCtrl	m_ListCtrl;
	double	m_dStage1_Len;
	double	m_dStage2_Len;
	double	m_dStage3_Len;
	double	m_dStage1_Speed;
	double	m_dStage2_Speed;
	double	m_dStage3_Speed;
	double	m_dStage1_Y_Offset;
	double	m_dStage2_Y_Offset;
	double	m_dStage3_Y_Offset;
	double	m_dStage1_X_Offset;
	double	m_dStage2_X_Offset;
	double	m_dStage3_X_Offset;
	int	m_iDelayTime;
	CString	m_strMessage;
	int		m_iVacOnOffSel1;
	int		m_iVacOnOffSel2;
	int		m_iVacOnOffSel3;
	CBtnEnh	m_btnAlignR;
	CBtnEnh	m_btnAlign1;

	int m_iOP_Command;
	double m_dOP_Val1;
	double m_dOP_Val2;
	double m_dOP_Val3;
	double m_dOP_Val4;
	double m_dOP_Val5;
	wchar_t m_chrOP_Val[255];
	int ListCnt;
	int SelAlignVal;

	void Data_Update();
	void Data_Save();
	void DispDisable() ;
	void ListUpdate();

	CParameterFile*			Log();
	CParameterFile*			ParamFile();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdlgRecipeSet)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CdlgRecipeSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickRecipeSel0();
	afx_msg void OnClickRecipeSel1();
	afx_msg void OnClickRecipeSel2();
	afx_msg void OnClickRecipeSel3();
	afx_msg void OnClickRecipeSel4();
	afx_msg void OnClickRecipeSel5();
	afx_msg void OnClickRecipeSel6();
	afx_msg void OnClickRecipeSel7();
	afx_msg void OnClickRecipeSel8();
	afx_msg void OnClickRecipeSel9();
	afx_msg void OnClickRecipeSel10();
	afx_msg void OnClickResultAdd2();
	afx_msg void OnClickResultAdd3();
	afx_msg void OnClickResultAdd();
	afx_msg void OnClickResultDel();
	afx_msg void OnClickResultDelall();
	afx_msg void OnClickResultSave();
	afx_msg void OnClickModelData();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickOk4();
	afx_msg void OnClickEnable3();
	afx_msg void OnClickEnable7();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
