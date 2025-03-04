// dlgRecipePrintEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgRecipePrintEdit.h"
#include "afxdialogex.h"


// CdlgRecipePrintEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgRecipePrintEdit, CDialogEx)

CdlgRecipePrintEdit::CdlgRecipePrintEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgRecipePrintEdit::IDD, pParent)
{
	m_dOp1 = 0.0;
	m_dOp2 = 0.0;
	m_dOpX = 0.0;
	m_dOpY = 0.0;
	m_dOpT = 0.0;
}

CdlgRecipePrintEdit::~CdlgRecipePrintEdit()
{
}

void CdlgRecipePrintEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdlgRecipePrintEdit)
	DDX_Text(pDX, IDC_EDIT_X_LEN_1, m_dOp1);
	DDX_Text(pDX, IDC_EDIT_RUN_SPEED_1, m_dOp2);
	DDX_Text(pDX, IDC_EDIT8, m_dOpX);
	DDX_Text(pDX, IDC_EDIT9, m_dOpY);
	DDX_Text(pDX, IDC_EDIT10, m_dOpT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CdlgRecipePrintEdit, CDialogEx)
	//{{AFX_MSG_MAP(CdlgRecipePrintEdit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CdlgRecipePrintEdit::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgRecipePrintEdit::OnBnClickedButton1)
END_MESSAGE_MAP()


// CdlgRecipePrintEdit 메시지 처리기입니다.
/////////////////////////////////////////////////////////////////////////////
// CdlgRecipePrintEdit message handlers

BOOL CdlgRecipePrintEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CdlgRecipePrintEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString str, str1, str2, str3, str11, str12, str13;
	if(iCMDNo== _OP_PROC_STAGE1)		{	
		str.Format(_T("Stage #1 동작"));
		str1.Format(_T("Stage #1 Work-Length")); str2.Format(_T("Stage #1 Work-Speed"));
		str11.Format(_T("mm"));		str12.Format(_T("mm/s"));	str13.Format(_T("mm"));
		dT= 0.0;
	}
	else if(iCMDNo== _OP_PROC_STAGE2)	{
		str.Format(_T("Stage #2 동작"));
		str1.Format(_T("Stage #2 Work-Length")); str2.Format(_T("Stage #2 Work-Speed"));
		str11.Format(_T("mm"));		str12.Format(_T("mm/s"));	str13.Format(_T("mm"));
		dT= 0.0;
	}
	else if(iCMDNo== _OP_PROC_STAGE3)	{
		str.Format(_T("Stage #3 동작"));
		str1.Format(_T("Stage #3 Work-Length")); str2.Format(_T("Stage #3 Work-Speed"));
		str11.Format(_T("mm"));		str12.Format(_T("mm/s"));	str13.Format(_T("mm"));
		dT= 0.0;
	}
	SetDlgItemText(IDB_RECIPE_SEL_12, str.GetBuffer(0));

	SetDlgItemText(IDS_STATIC_16, str1.GetBuffer(0));
	SetDlgItemText(IDS_STATIC_TXT2, str2.GetBuffer(0));

	SetDlgItemText(IDC_STATIC_1, str11.GetBuffer(0));
	SetDlgItemText(IDC_STATIC_2, str12.GetBuffer(0));

	//	SetDlgItemText(IDC_STATIC_3, str3.GetBuffer(0));
	//	SetDlgItemText(IDC_STATIC_5, str13.GetBuffer(0));

	m_dOp1= dOp1;
	m_dOp2= dOp2;
	//	m_dOp3= dOp3;
	m_dOpX= dX;
	m_dOpY= dY;
	m_dOpT= dT;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CdlgRecipePrintEdit::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

}

void CdlgRecipePrintEdit::OnOK() 
{
	UpdateData(TRUE);
	dOp1= m_dOp1;
	dOp2= m_dOp2;
	//	dOp3= m_dOp3;
	dX= m_dOpX;
	dY= m_dOpY;
	dT= m_dOpT;

	CDialog::OnOK();
}


void CdlgRecipePrintEdit::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CdlgRecipePrintEdit::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
