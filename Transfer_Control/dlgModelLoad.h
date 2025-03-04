#if !defined(AFX_DLGMODELLOAD_H__839B8219_0B80_4EC3_9495_B6EECC6EBC23__INCLUDED_)
#define AFX_DLGMODELLOAD_H__839B8219_0B80_4EC3_9495_B6EECC6EBC23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgModelLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CdlgModelLoad dialog

class CdlgModelLoad : public CDialog
{
	// Construction
public:
	CdlgModelLoad(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CdlgModelLoad)
	enum { IDD = IDD_DIALOG_SELECT_MODEL };
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA

	void ListPrint(CString strText);
	void ListClear();
	void FileList();
	CString strChangeModelName;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdlgModelLoad)
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
	//{{AFX_MSG(CdlgModelLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickModelSave();
	afx_msg void OnClickOk();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickModelLoad();
	afx_msg void OnClickModelSave2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODELLOAD_H__839B8219_0B80_4EC3_9495_B6EECC6EBC23__INCLUDED_)
