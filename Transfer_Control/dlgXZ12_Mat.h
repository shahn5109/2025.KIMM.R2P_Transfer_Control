#pragma once


// CdlgXZ12_Mat 대화 상자입니다.

class CdlgXZ12_Mat : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgXZ12_Mat)

public:
	CdlgXZ12_Mat(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgXZ12_Mat();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_XZ_MAT };
	CListCtrl	m_ctrlDownload_1;
	CListCtrl	m_ctrlCorrect_1;
	CListCtrl	m_ctrlOriginal_1;
	double	m_selListData;
	double	m_selListData2;
	double	m_dKgPerum;
	double	m_dExcept_XPos;
	double	m_dExcept_XPos2;
	int		m_iCalSelNo;
	int		m_i1stVal;
	int		m_i2ndVal;
	double		m_dCalSumVal;

	int sel_listselno;

	int m_lListCount_Dn_1;
	int m_lListCount_Dn_2;
	int m_lListCount_Cor_1;
	int m_lListCount_Cor_2;
	int m_lListCount_Org_1;
	int m_lListCount_Org_2;

	BOOL MoveEndCheck(int Axis) ;

	void ListClear(int no);
	void Open_PMC_ORG_File();
	//	void Open_PMC_CORRECT_File();
	void Build_PMC_DOWNLOAD_File();

	void ResultUpdate_Org();
	void ResultUpdate_Mat() ;
	void ResultUpdate_Down();

	void CalMatdata();

	void OpenMatDataFile();
	void SaveMatDataFile();

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton14();
	afx_msg void OnDestroy();
	afx_msg void OnButtonLoadDataBuild();
	afx_msg void OnButtonGraph_Load();
	afx_msg void OnButtonMat_DownLoad();
	afx_msg void OnButtonOrg_DownLoad();
	afx_msg void OnDblclkListM1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnButtondataInit();
	afx_msg void OnButton3();
	virtual BOOL OnInitDialog();
};
