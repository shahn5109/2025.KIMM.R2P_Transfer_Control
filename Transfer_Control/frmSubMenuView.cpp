// frmSubMenuView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmSubMenuView.h"
#include "MainFrm.h"

#include "dlgData_MotionStatus.h"
#include "dlgManual_IO_Monitor.h"

// CfrmSubMenuView
extern CSystemClass pSystem;
CParameterFile* CfrmSubMenuView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmSubMenuView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmSubMenuView, CFormView)

	CfrmSubMenuView::CfrmSubMenuView()
	: CFormView(CfrmSubMenuView::IDD)
{
	iLed_Calib= 0;
}

CfrmSubMenuView::~CfrmSubMenuView()
{
}

void CfrmSubMenuView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_Y_POS0, m_btnMain);
	DDX_Control(pDX, IDB_Y_POS1, m_btnMenu);
	DDX_Control(pDX, IDB_Y_POS2, m_btnSetup);
	DDX_Control(pDX, IDB_Y_POS3, m_btnTeach);
	DDX_Control(pDX, IDB_Y_POS4, m_btnAlign);
	DDX_Control(pDX, IDB_Y_POS12, m_btnCalib);
	DDX_Control(pDX, IDB_Y_POS6, m_btnStamp);
	DDX_Control(pDX, IDB_Y_POS14, m_btnComp);

	DDX_Control(pDX, IDB_LED_SEL1, m_btnLed1);
	DDX_Control(pDX, IDB_LED_SEL2, m_btnLed2);
	DDX_Control(pDX, IDB_LED_SEL3, m_btnLed3);
	DDX_Control(pDX, IDB_LED_SEL4, m_btnLed4);
}

BEGIN_MESSAGE_MAP(CfrmSubMenuView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CfrmSubMenuView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CfrmSubMenuView::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CfrmSubMenuView::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CfrmSubMenuView::OnBnClickedButton8)
END_MESSAGE_MAP()


// CfrmSubMenuView 진단입니다.

#ifdef _DEBUG
void CfrmSubMenuView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmSubMenuView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmSubMenuView 메시지 처리기입니다.

//===================================================================================
void CfrmSubMenuView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_SubMenuView(this);

	m_btnMain.SetValue(1);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); 	m_btnComp.SetValue(0);

	g_bCfrmSubMenuViewInit= TRUE;

	m_btnLed1.SetValue(0);	m_btnLed2.SetValue(0);	m_btnLed3.SetValue(1);	m_btnLed4.SetValue(0);
	g_Led_Sel_Num= ePLATE_3;

	SetTimer(frmSubMenuView_TIMER_ID, frmSubMenuView_TIMER_INTERVAL, NULL);
}

//===================================================================================
void CfrmSubMenuView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmSubMenuView_TIMER_ID);

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmSubMenuView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmSubMenuView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmSubMenuView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN) 
	{
		/*GetDlgItem(IDB_Y_POS2)->EnableWindow(FALSE);*/		GetDlgItem(IDB_Y_POS4)->EnableWindow(FALSE);		GetDlgItem(IDB_Y_POS12)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_Y_POS2)->EnableWindow(TRUE);		GetDlgItem(IDB_Y_POS4)->EnableWindow(TRUE);		GetDlgItem(IDB_Y_POS12)->EnableWindow(TRUE);
	}

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
BEGIN_EVENTSINK_MAP(CfrmSubMenuView, CFormView)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS0, DISPID_CLICK, CfrmSubMenuView::ClickYPos0, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS1, DISPID_CLICK, CfrmSubMenuView::ClickYPos1, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS2, DISPID_CLICK, CfrmSubMenuView::ClickYPos2, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS3, DISPID_CLICK, CfrmSubMenuView::ClickYPos3, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS4, DISPID_CLICK, CfrmSubMenuView::ClickYPos4, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS12, DISPID_CLICK, CfrmSubMenuView::ClickYPos5, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS6, DISPID_CLICK, CfrmSubMenuView::ClickYPos6, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_ORIGIN_RETURN2, DISPID_CLICK, CfrmSubMenuView::ClickOriginReturn2, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_ORIGIN_RETURN3, DISPID_CLICK, CfrmSubMenuView::ClickOriginReturn3, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_LED_SEL1, DISPID_CLICK, CfrmSubMenuView::ClickLedSel1, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_LED_SEL2, DISPID_CLICK, CfrmSubMenuView::ClickLedSel2, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_LED_SEL3, DISPID_CLICK, CfrmSubMenuView::ClickLedSel3, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_LED_SEL4, DISPID_CLICK, CfrmSubMenuView::ClickLedSel4, VTS_NONE)
	ON_EVENT(CfrmSubMenuView, IDB_Y_POS14, DISPID_CLICK, CfrmSubMenuView::ClickYPos14, VTS_NONE)
END_EVENTSINK_MAP()

//===================================================================================
void CfrmSubMenuView::ClickYPos0()
{
	m_btnMain.SetValue(1);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(0); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_MAIN_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }
}

//===================================================================================
void CfrmSubMenuView::ClickYPos1()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(1);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(0); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_MENU_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }
}

//===================================================================================
void CfrmSubMenuView::ClickYPos2()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(1);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(0); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_TEACH_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }

}

//===================================================================================
void CfrmSubMenuView::ClickYPos3()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(1);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(0); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_SETUP_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }
}

//===================================================================================
void CfrmSubMenuView::ClickYPos4()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(1);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(0); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_ALIGN_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }
}

//===================================================================================
void CfrmSubMenuView::ClickYPos5()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(1); m_btnStamp.SetValue(0); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_CALIB_DISP_VIEW);
	if(ParamFile()->System_data.Led_Remote)		pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePHOTOMASK);
	iLed_Calib= 1;
}


void CfrmSubMenuView::ClickYPos6()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(1); m_btnComp.SetValue(0);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_STAMP_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }
}

void CfrmSubMenuView::ClickYPos14()
{
	m_btnMain.SetValue(0);	m_btnMenu.SetValue(0);	m_btnSetup.SetValue(0);
	m_btnTeach.SetValue(0);	m_btnAlign.SetValue(0);	m_btnCalib.SetValue(0); m_btnStamp.SetValue(0); m_btnComp.SetValue(1);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->SwitchMainViews(_COMP_DISP_VIEW);	
	if(ParamFile()->System_data.Led_Remote && iLed_Calib== 1) {	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num); iLed_Calib= 0; }
}


void CfrmSubMenuView::ClickOriginReturn2()
{
	CdlgManual_IO_Monitor dlg;
	dlg.DoModal();
}


void CfrmSubMenuView::ClickOriginReturn3()
{
	CdlgData_MotionStatus dlg;
	dlg.DoModal();
}


void CfrmSubMenuView::OnBnClickedButton1()
{
}


void CfrmSubMenuView::OnBnClickedButton6()
{
}


void CfrmSubMenuView::OnBnClickedButton7()
{
}


void CfrmSubMenuView::OnBnClickedButton8()
{
}


void CfrmSubMenuView::ClickLedSel1()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_1);
	g_Led_Sel_Num= ePLATE_1;
	m_btnLed1.SetValue(1);	m_btnLed2.SetValue(0);	m_btnLed3.SetValue(0);	m_btnLed4.SetValue(0);
}
void CfrmSubMenuView::ClickLedSel2()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_2);
	g_Led_Sel_Num= ePLATE_2;
	m_btnLed1.SetValue(0);	m_btnLed2.SetValue(1);	m_btnLed3.SetValue(0);	m_btnLed4.SetValue(0);
}
void CfrmSubMenuView::ClickLedSel3()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_3);
	g_Led_Sel_Num= ePLATE_3;
	m_btnLed1.SetValue(0);	m_btnLed2.SetValue(0);	m_btnLed3.SetValue(1);	m_btnLed4.SetValue(0);
}
void CfrmSubMenuView::ClickLedSel4()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, eROLL);
	g_Led_Sel_Num= eROLL;
	m_btnLed1.SetValue(0);	m_btnLed2.SetValue(0);	m_btnLed3.SetValue(0);	m_btnLed4.SetValue(1);
}

