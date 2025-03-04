// frmTitleView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmTitleView.h"
#include "MainFrm.h"


// CfrmTitleView
extern CSystemClass pSystem;
CParameterFile* CfrmTitleView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmTitleView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmTitleView, CFormView)

CfrmTitleView::CfrmTitleView()
	: CFormView(CfrmTitleView::IDD)
{

}

CfrmTitleView::~CfrmTitleView()
{
}

void CfrmTitleView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfrmTitleView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CfrmTitleView 진단입니다.

#ifdef _DEBUG
void CfrmTitleView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmTitleView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmTitleView 메시지 처리기입니다.

//===================================================================================
void CfrmTitleView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_frmTitleView(this);

	BackColor= RGB(153, 179, 233);
	m_brush.CreateSolidBrush(BackColor); 	brush1.CreateSolidBrush(RGB(255, 0, 0)); 
	m_GreenBrush.CreateSolidBrush(RGB(212, 255, 212));	m_BlueBrush.CreateSolidBrush(RGB(212, 212, 255));
	m_YellowBrush.CreateSolidBrush(RGB(255, 255, 212));	m_RedBrush.CreateSolidBrush(RGB(255, 212, 212));
	m_GrayBrush.CreateSolidBrush(RGB(196, 196, 196));
	m_WhiteBrush.CreateSolidBrush(RGB(255, 255, 255));

	CTime time = CTime::GetCurrentTime();
	CString str;

	str.Format(_T("%04d-%02d-%02d %s %02d:%02d:%02d [  ]"), time.GetYear(), time.GetMonth(), time.GetDay(),
		time.Format("%p"), time.GetHour(), time.GetMinute(), time.GetSecond());
	SetDlgItemText(IDC_STATIC_DATE, str.GetBuffer(0));

	bEMO_CheckFlag= false;

	SetTimer(frmTitleView_TIMER_ID,			frmTitleView_TIMER_INTERVAL, NULL);

	g_bCfrmTitleViewInit= true;
}
//===================================================================================
void CfrmTitleView::OnDestroy()
{
	CFormView::OnDestroy();

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();

	KillTimer(frmTitleView_TIMER_ID);
}

//===================================================================================
void CfrmTitleView::OnSize(UINT nType, int cx, int cy)
{
//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmTitleView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT); 
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_DATE ||
		pWnd->GetDlgCtrlID()==IDC_STATIC_DATE2 ||
		pWnd->GetDlgCtrlID()==IDC_STATIC_DATE3) {	pDC->SetTextColor(RGB(0, 0, 0));		return (HBRUSH)m_WhiteBrush;	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmTitleView::OnTimer(UINT_PTR nIDEvent)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	CTime time = CTime::GetCurrentTime();
	CString str;
	if(!PROGRAM_EXIT) 
	{
		str.Format(_T("%04d-%02d-%02d %s %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(),
			time.Format("%p"), time.GetHour(), time.GetMinute(), time.GetSecond());

		SetDlgItemText(IDC_STATIC_DATE, str.GetBuffer(0));
	}

	CFormView::OnTimer(nIDEvent);
}

//===================================================================================
BEGIN_EVENTSINK_MAP(CfrmTitleView, CFormView)
	ON_EVENT(CfrmTitleView, ID_BTN_EXIT, DISPID_CLICK, CfrmTitleView::ClickBtnExit, VTS_NONE)
END_EVENTSINK_MAP()

//===================================================================================
void CfrmTitleView::ClickBtnExit()
{
	CMainFrame* frame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	frame->AppExitProc();
}
//===================================================================================

