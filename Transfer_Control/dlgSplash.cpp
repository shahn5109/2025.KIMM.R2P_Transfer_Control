// CdlgSplash.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgSplash.h"
#include "afxdialogex.h"


extern CSystemClass pSystem;
CParameterFile* CdlgSplash::ParamFile()		{ return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgSplash::Log()			{ return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

#define _STEP1			0
#define _STEP2			1
#define _STEP3			2
#define _STEP4			3
#define _STEP5			4

/////////////////////////////////////////////////////////////////////////////
//		Initial Thread
/////////////////////////////////////////////////////////////////////////////
UINT Init(LPVOID pParam)
{
	CdlgSplash *pDlg = (CdlgSplash *)pParam;
	bool m_bSuccess[5] = { false, false, false, false, false };

	/////////////////////////////////////////////////////////////////////////////
	//	[STEP_1]	System Initialize...
	/////////////////////////////////////////////////////////////////////////////
	pDlg->GetDlgItem(IDC_ARROW_1)->ShowWindow(SW_SHOW);	pDlg->GetDlgItem(IDC_MESSAGE_1)->ShowWindow(SW_SHOW);
	Sleep(100);
	if (pSystem.Initialize() == ERR_SYSTEM_SUCCESS) {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("========================================================================"));
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[Program-Initialize] START!!!"));
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-1] System Initialize => ERR_SYSTEM_SUCCESS"));
		m_bSuccess[0] = true;
		pDlg->SetDlgItemText(IDC_ARROW_1, _T("V"));			pDlg->SetDlgItemText(IDC_FAIL_1, _T("Complete"));
		pDlg->GetDlgItem(IDC_ARROW_1)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_1)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_1)->ShowWindow(SW_SHOW);
	}
	else {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("========================================================================"));
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[Program-Initialize] START!!!"));
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-1] System Initialize => ERR_SYSTEM_FAIL!"));
		m_bSuccess[0] = false;
		pDlg->SetDlgItemText(IDC_ARROW_1, _T("X"));			pDlg->SetDlgItemText(IDC_FAIL_1, _T("Fail"));
		pDlg->GetDlgItem(IDC_ARROW_1)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_1)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_1)->ShowWindow(SW_SHOW);
	}
	Sleep(100);

	/////////////////////////////////////////////////////////////////////////////
	//	[STEP_2]	Peripheral B/D Initialize...
	/////////////////////////////////////////////////////////////////////////////
	pDlg->GetDlgItem(IDC_ARROW_2)->ShowWindow(SW_SHOW);	pDlg->GetDlgItem(IDC_MESSAGE_2)->ShowWindow(SW_SHOW);

	m_bSuccess[1] = false;
	if (pSystem.IO_SystemInitial())			m_bSuccess[1] = true;
	else									m_bSuccess[1] = false;

	if (m_bSuccess[1]) {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-2] Peripheral B/D Initialize => ERR_SYSTEM_SUCCESS"));
		pDlg->SetDlgItemText(IDC_ARROW_2, _T("V"));			pDlg->SetDlgItemText(IDC_FAIL_2, _T("Complete"));
		pDlg->GetDlgItem(IDC_ARROW_2)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_2)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_2)->ShowWindow(SW_SHOW);
	}
	else {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-2] Peripheral B/D Initialize => ERR_SYSTEM_FAIL!"));
		pDlg->SetDlgItemText(IDC_ARROW_2, _T("X"));			pDlg->SetDlgItemText(IDC_FAIL_2, _T("Fail"));
		pDlg->GetDlgItem(IDC_ARROW_2)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_2)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_2)->ShowWindow(SW_SHOW);
	}
	Sleep(100);
	/////////////////////////////////////////////////////////////////////////////
	//	[STEP_3]	System Configuration Data Readout...
	/////////////////////////////////////////////////////////////////////////////
	pDlg->GetDlgItem(IDC_ARROW_3)->ShowWindow(SW_SHOW);	pDlg->GetDlgItem(IDC_MESSAGE_3)->ShowWindow(SW_SHOW);
	m_bSuccess[2] = ERR_SYSTEM_SUCCESS;

	//	pDlg->ParamFile()->OpenSytemConfigFile();

	if (m_bSuccess[2] == ERR_SYSTEM_SUCCESS) {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-3] System Configuration Data Readout => ERR_SYSTEM_SUCCESS"));
		m_bSuccess[2] = true;
		pDlg->SetDlgItemText(IDC_ARROW_3, _T("V"));			pDlg->SetDlgItemText(IDC_FAIL_3, _T("Complete"));
		pDlg->GetDlgItem(IDC_ARROW_3)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_3)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_3)->ShowWindow(SW_SHOW);
	}
	else {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-3] System Configuration Data Readout => ERR_SYSTEM_FAIL!"));
		m_bSuccess[2] = false;
		pDlg->SetDlgItemText(IDC_ARROW_3, _T("X"));			pDlg->SetDlgItemText(IDC_FAIL_3, _T("Fail"));
		pDlg->GetDlgItem(IDC_ARROW_3)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_3)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_3)->ShowWindow(SW_SHOW);
	}
	Sleep(100);
	/////////////////////////////////////////////////////////////////////////////
	//	[STEP_4]	Model-Data Readout...
	/////////////////////////////////////////////////////////////////////////////
	pDlg->GetDlgItem(IDC_ARROW_4)->ShowWindow(SW_SHOW);	pDlg->GetDlgItem(IDC_MESSAGE_4)->ShowWindow(SW_SHOW);
	m_bSuccess[3] = ERR_SYSTEM_SUCCESS;

	pDlg->ParamFile()->OpenSytemConfigFile();
	//	pDlg->ParamFile()->OpenRecipeFile(pDlg->ParamFile()->strCurrentModelName);

	if (m_bSuccess[3] == ERR_SYSTEM_SUCCESS) {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-4] Model-Data Readout => ERR_SYSTEM_SUCCESS"));
		m_bSuccess[3] = true;
		pDlg->SetDlgItemText(IDC_ARROW_4,_T("V"));			pDlg->SetDlgItemText(IDC_FAIL_4, _T("Complete"));
		pDlg->GetDlgItem(IDC_ARROW_4)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_4)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_4)->ShowWindow(SW_SHOW);
	}
	else {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-4] Model-Data Readout => ERR_SYSTEM_FAIL!"));
		m_bSuccess[3] = false;
		pDlg->SetDlgItemText(IDC_ARROW_4, _T("X"));			pDlg->SetDlgItemText(IDC_FAIL_4, _T("Fail"));
		pDlg->GetDlgItem(IDC_ARROW_4)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_4)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_4)->ShowWindow(SW_SHOW);
	}
	Sleep(100);
	/////////////////////////////////////////////////////////////////////////////
	//	[STEP_5]	Final Initializing.....
	/////////////////////////////////////////////////////////////////////////////
	pDlg->GetDlgItem(IDC_ARROW_5)->ShowWindow(SW_SHOW);	pDlg->GetDlgItem(IDC_MESSAGE_5)->ShowWindow(SW_SHOW);
	m_bSuccess[4] = ERR_SYSTEM_SUCCESS;
	Sleep(100);
	if (m_bSuccess[4] == ERR_SYSTEM_SUCCESS) {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-5] Final Initializing => ERR_SYSTEM_SUCCESS"));
		m_bSuccess[4] = true;
		pDlg->SetDlgItemText(IDC_ARROW_5, _T("V"));			pDlg->SetDlgItemText(IDC_FAIL_5, _T("Complete"));
		pDlg->GetDlgItem(IDC_ARROW_5)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_5)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_5)->ShowWindow(SW_SHOW);
	}
	else {
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL-STEP-5] Final Initializing => ERR_SYSTEM_FAIL!"));
		m_bSuccess[4] = false;
		pDlg->SetDlgItemText(IDC_ARROW_5, _T("X"));			pDlg->SetDlgItemText(IDC_FAIL_5, _T("Fail"));
		pDlg->GetDlgItem(IDC_ARROW_5)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_FAIL_5)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_MESSAGE_5)->ShowWindow(SW_SHOW);
	}
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	pDlg->GetDlgItem(IDC_FINAL_MESSAGE)->ShowWindow(SW_SHOW);

	if (m_bSuccess[_STEP1] && m_bSuccess[_STEP2] && m_bSuccess[_STEP3] &&
		m_bSuccess[_STEP4] && m_bSuccess[_STEP5])
	{
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL] Complite!"));
		pDlg->SetDlgItemText(IDC_FINAL_MESSAGE, _T("프로그램 초기화가 완료 되었습니다."));
		Sleep(1000);
		::PostMessage(
			pDlg->m_hWnd,
			WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(pDlg->GetDlgItem(IDOK)->m_hWnd), BN_CLICKED),
			0
			);
		pDlg->bIsInitEnd = true;
		return 0L;
	}
	else {
		pDlg->GetDlgItem(IDOK)->ShowWindow(SW_SHOW);	pDlg->GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
		pDlg->Log()->SaveLogFile(_SYSTEM_LOG, _T("[INITIAL] FAIL!"));
	}
	pDlg->bIsInitEnd = true;
	return 0L;
}

// CdlgSplash 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgSplash, CDialogEx)

	//===================================================================================
	CdlgSplash::CdlgSplash(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgSplash::IDD, pParent)
{
	for (int i = 0; i < 10; i++) SplashInitStep[i] = 0;
	bIsSuccess = FALSE;
	bIsInitEnd = false;
}

//===================================================================================
CdlgSplash::~CdlgSplash()
{
}

//===================================================================================
void CdlgSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}


//===================================================================================
BEGIN_MESSAGE_MAP(CdlgSplash, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()
//===================================================================================


// CdlgSplash 메시지 처리기입니다.
//===================================================================================
BOOL CdlgSplash::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;//	rect.top= rect.top - 10;//top=172, b=411,l=637, r=1021

	GetWindowRect(rect);
	//	rect.top= rect.top - 10;
	//	int disp_OffsetX= 1280 ;
	int disp_OffsetY = (1080 / 2) - (rect.Height() / 2) - 50;
	int disp_OffsetX = (1920 / 2) - (rect.Width() / 2) - 30;
	//	int disp_OffsetY= (1024 / 2) - (rect.Height() / 2) - 50;
	MoveWindow(CRect(_SPLASH_X_POS + disp_OffsetX, _SPLASH_Y_POS + disp_OffsetY, _SPLASH_X_POS + 384 + disp_OffsetX, _SPLASH_Y_POS + 289 + disp_OffsetY)); // H=239, W=384

	::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetForegroundWindow(this->GetSafeHwnd());
	::SetActiveWindow(this->GetSafeHwnd());


	m_ctrlProgress.SetRange(0, 100);
	m_bIsPaint = false;

	//	GetDlgItem(IDC_ARROW_1)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDC_ARROW_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ARROW_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ARROW_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ARROW_5)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_FAIL_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FAIL_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FAIL_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FAIL_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FAIL_5)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_MESSAGE_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MESSAGE_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MESSAGE_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MESSAGE_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MESSAGE_5)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_FINAL_MESSAGE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//===================================================================================
void CdlgSplash::OnDestroy()
{
	//	CRect rect;
	//	GetWindowRect(rect);
	//	rect.top= rect.top - 10;//top=172, b=411,l=637, r=1021

	CDialog::OnDestroy();

	// TODO: Add your message handler code here

}

//===================================================================================
HBRUSH CdlgSplash::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_ARROW_1 ||
		pWnd->GetDlgCtrlID() == IDC_ARROW_2 ||
		pWnd->GetDlgCtrlID() == IDC_ARROW_3 ||
		pWnd->GetDlgCtrlID() == IDC_ARROW_4 ||
		pWnd->GetDlgCtrlID() == IDC_ARROW_5)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	if (pWnd->GetDlgCtrlID() == IDC_FAIL_1 ||
		pWnd->GetDlgCtrlID() == IDC_FAIL_2 ||
		pWnd->GetDlgCtrlID() == IDC_FAIL_3 ||
		pWnd->GetDlgCtrlID() == IDC_FAIL_4 ||
		pWnd->GetDlgCtrlID() == IDC_FAIL_5)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	return hbr;
}

//===================================================================================
void CdlgSplash::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: Add your message handler code here

}

//===================================================================================
void CdlgSplash::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (!m_bIsPaint) {
		m_bIsPaint = true;
		bIsSuccess = FALSE;
		AfxBeginThread(Init, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
}


//===================================================================================
void CdlgSplash::OnTimer(UINT_PTR nIDEvent) {

	CDialog::OnTimer(nIDEvent);
}

//===================================================================================
BOOL CdlgSplash::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		if ((pMsg->wParam == VK_ESCAPE))	return TRUE;
		//		else if(pMsg->wParam == 13) {	
		if (!bIsInitEnd)				return TRUE;
		//    	}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//===================================================================================
