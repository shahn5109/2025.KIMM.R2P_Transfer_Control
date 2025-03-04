// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// 모든 권리 보유.

// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Transfer_Control.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//===================================================================================
extern CSystemClass pSystem;
CParameterFile* CMainFrame::Log() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CMainFrame::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
//===================================================================================
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);

	m_pfrmTitleView= NULL;
	m_pfrmMainView= NULL;
	m_pfrmManualView= NULL;
	m_pfrmSetupView= NULL;
	m_pfrmDebugView= NULL;
	m_pfrmSubMenuView= NULL;
	m_pfrmTeachView= NULL;
	m_pfrmAlignView= NULL;
	m_pfrmCameraView= NULL;
	m_pfrmCalibView= NULL;
	m_pfrmDataView= NULL;
	m_pfrmStampView= NULL;
	m_pfrmCompView= NULL;
	g_bCfrmDataViewInit= NULL;

	CurrViewNo= 0;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//----------------------------------------------------------------------------------

	int Year;
	int Mon;
	int Day;
	GetReleaseDate(&Year, &Mon, &Day);
//	Year = Year % 1000;

/*
	CString strTitle;
	strTitle.Format(_T("     Roll To Roll System & Align 프로그램                  [ Build : %02d/%02d/20%02d   %s     Ver 1.0.1 ]"), / *strYear, strMon,* /
		Mon, Day, Year, chTime);//, 1/ *BUILDCOUNT_NUM* /);
	//		chDate, chTime, BUILDCOUNT_NUM);
*/

	CString strTitle;

//  	strTitle.Format(_T("     Transfer System & Align 프로그램                  [ Build : %d/%d/%d   %s]",
//  		Mon, Day, Year, chTime));//, 1/*BUILDCOUNT_NUM*/);
// 	//		chDate, chTime, BUILDCOUNT_NUM);
// 	::SetWindowText(m_hWnd, strTitle.GetBuffer(0)); 

	TCHAR aaa[500];
	_stprintf(aaa,		_T("     Transfer System & Align 프로그램                       [ Build : %d-%02d-%02d   %s ]"),
		Year, Mon, Day, chTime);//, 1/*BUILDCOUNT_NUM*/);
	::SetWindowText(m_hWnd, aaa);

	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//----------------------------------------------------------------------------------
	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

/*
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
*/

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	return 0;
}
//===================================================================================
void CMainFrame::GetReleaseDate(int *Year, int *Mon, int *Day)
{
	// 프로그램 Releae 날짜 구함

/*
	TCHAR Date[50];

	_tcscpy(Date, (wchar_t *)(__DATE__));
	_tcscpy(chDate, (wchar_t *)(__DATE__));

	_tcscpy(chTime, (wchar_t *)(__TIME__));

	TCHAR *Month[] =
	{
		_T("Jan"),
		_T("Feb"),
		_T("Mar"),
		_T("Apr"),
		_T("May"),
		_T("Jun"),
		_T("Jul"),
		_T("Aug"),
		_T("Sep"),
		_T("Oct"),
		_T("Nov"),
		_T("Dec")
	};

	TCHAR *aMon;// = strtok(NULL, _T(" "));
	TCHAR *sDay;// = strtok(NULL, _T(" "));
	TCHAR *sYear;// = strtok(NULL, _T(" "));

// 	for (int i = 0; i < sizeof(Month) / sizeof(Month[0]); i++)
// 	{
// 		if (!strcmp(aMon, Month[i]))
// 		{
// 			*Mon = i + 1;
// 			break;
// 		}
// 	}

	*Mon = _wtoi(sYear);
	*Year = _wtoi(sYear);
	*Day = _wtoi(sDay);
*/
	TCHAR Date[50];
// 	strcpy(Date, __DATE__);
// 	strcpy(chDate, __DATE__);
// 	strcpy(chTime, __TIME__);
	_tcscpy(Date, _T(__DATE__));
	_tcscpy(chDate, _T(__DATE__));
	_tcscpy(chTime, _T(__TIME__));

	TCHAR *Month[] =
	{
		_T("Jan"),
		_T("Feb"),
		_T("Mar"),
		_T("Apr"),
		_T("May"),
		_T("Jun"),
		_T("Jul"),
		_T("Aug"),
		_T("Sep"),
		_T("Oct"),
		_T("Nov"),
		_T("Dec")
	};


	TCHAR *aMon = wcstok((Date),  _T(" "));
	TCHAR *sDay = wcstok(NULL, _T(" "));
	TCHAR *sYear = wcstok(NULL, _T(" "));

	for (int i = 0; i < sizeof(Month) / sizeof(Month[0]); i++)
	{
		if (!wcscmp(aMon, Month[i]))
		{
			*Mon = i + 1;
			break;
		}
	}

	*Year = _wtoi(sYear);
	*Day  = _wtoi(sDay);

}
//===================================================================================
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_SYSMENU;
	cs.style &= ~(FWS_ADDTOTITLE);

	cs.x = 0;
	cs.y = 0;
	cs.cx= 1920;
	cs.cy= 1080; //768
//	cs.style = WS_POPUP | WS_MAXIMIZE;


	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
//===================================================================================
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pfrmCameraView				= RUNTIME_CLASS(CfrmCameraView);

	m_pfrmTitleView					= RUNTIME_CLASS(CfrmTitleView);
	m_pfrmMainView					= RUNTIME_CLASS(CfrmMainView);
	m_pfrmDebugView					= RUNTIME_CLASS(CfrmDebugView);
	m_pfrmSubMenuView				= RUNTIME_CLASS(CfrmSubMenuView);

	m_pfrmManualView				= RUNTIME_CLASS(CfrmManualView);
	m_pfrmSetupView					= RUNTIME_CLASS(CfrmSetupView);
	m_pfrmTeachView					= RUNTIME_CLASS(CfrmTeachView);
	m_pfrmCalibView					= RUNTIME_CLASS(CfrmCalibView);
	m_pfrmDataView					= RUNTIME_CLASS(CfrmDataView);
	m_pfrmAlignView					= RUNTIME_CLASS(CfrmAlignView);
	m_pfrmStampView					= RUNTIME_CLASS(CfrmStampView);
	m_pfrmCompView					= RUNTIME_CLASS(CfrmCompView);

	// Row : 2, Column : 1
	if (!m_wndSplitter1.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to Create splitter bar");		return FALSE;
	}
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1,   // our parent window is the first splitter
		1, 2,   //가-2, 세-1                                         // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,              // style, WS_BORDER is needed
		m_wndSplitter1.IdFromRowCol(1, 0)))             // new splitter is in the first row, 3rd column of first splitter
	{
		;
	}
	if (!m_wndSplitter3.CreateStatic(&m_wndSplitter2,   // our parent window is the first splitter
		2, 1,   //가-2, 세-1                                         // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,              // style, WS_BORDER is needed
		m_wndSplitter2.IdFromRowCol(0, 1)))             // new splitter is in the first row, 3rd column of first splitter
	{
		;
	}

	if (!m_wndSplitter4.CreateStatic(&m_wndSplitter3,   // our parent window is the first splitter
		1, 2,   //가-2, 세-1                                         // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,              // style, WS_BORDER is needed
		m_wndSplitter3.IdFromRowCol(0, 0)))             // new splitter is in the first row, 3rd column of first splitter
	{
		;
	}

	if (!m_wndSplitter1.CreateView(0, 0, m_pfrmTitleView, CSize(450, 70), pContext))	{ return FALSE; }
	if (!m_wndSplitter2.CreateView(0, 0, m_pfrmCameraView, CSize(450, 70), pContext))	{ return FALSE; }
	if (!m_wndSplitter3.CreateView(1, 0, m_pfrmDebugView, CSize(450, 70), pContext))	{ return FALSE; }
	if (!m_wndSplitter4.CreateView(0, 1, m_pfrmSubMenuView, CSize(450, 70), pContext))	{ return FALSE; }

	m_nMainPage[0] = m_wndSplitter4.AddView(0, 0, m_pfrmMainView, pContext);
	m_nMainPage[1] = m_wndSplitter4.AddView(0, 0, m_pfrmDataView, pContext);
	m_nMainPage[2] = m_wndSplitter4.AddView(0, 0, m_pfrmTeachView, pContext);
	m_nMainPage[3] = m_wndSplitter4.AddView(0, 0, m_pfrmSetupView, pContext); //m_pfrmGraphView
	m_nMainPage[4] = m_wndSplitter4.AddView(0, 0, m_pfrmAlignView, pContext);
	m_nMainPage[5] = m_wndSplitter4.AddView(0, 0, m_pfrmCalibView, pContext);
	m_nMainPage[6] = m_wndSplitter4.AddView(0, 0, m_pfrmStampView, pContext);
	m_nMainPage[7] = m_wndSplitter4.AddView(0, 0, m_pfrmCompView, pContext);

	//------------------------------------------------------------------------
	m_wndSplitter1.SetRowInfo(0, 70, 0);//		m_wndSplitter1.SetColumnInfo(0, 713, 0 );
	//	m_wndSplitter1.SetRowInfo(1, 700, 0 );		//m_wndSplitter0.SetColumnInfo(1, 100, 0 );
	//	m_wndSplitter1.SetColumnInfo(0, 1650, 0 );
	//------------------------------------------------------------------------
	m_wndSplitter2.SetColumnInfo(0, 530, 0);//_wndSplitter2.SetColumnInfo(1, 300, 0 );
	//------------------------------------------------------------------------
	m_wndSplitter3.SetRowInfo(0, 800, 0);//		m_wndSplitter1.SetColumnInfo(0, 713, 0 );
	m_wndSplitter4.SetColumnInfo(0, 1235, 0);//		m_wndSplitter1.SetColumnInfo(0, 713, 0 );
	//------------------------------------------------------------------------

	//	m_wndSplitter1.SetSplitterStyle(XT_SPLIT_NOFULLDRAG | XT_SPLIT_NOSIZE);// | XT_SPLIT_NOBORDER
	SwitchMainViews(0); //_MAIN_DISP_VIEW

	//	m_CamView->Vision_Init();

	return TRUE;
}
//===================================================================================
void CMainFrame::SwitchMainViews(int viewNumber)
//-----------------------------------------------------------------------------------
{
	/*	if(g_iCurrViewNo== _GRAPH_VIEW)
	GetWindowHandler_Graph_View()->SendMessage(WM_MSG_GRAPH_STOP,	0, 0);
	else if(g_iCurrViewNo== _OPC_DATA_VIEW)
	GetWindowHandler_OpcData_View()->ComLiveTransOff() ;
	if(viewNumber== _SETUP_VIEW)
	{
	GetWindowHandler_Setup_View()->SendMessage(WM_MSG_DATA_UPDATE,	0, 0);
	}
	*/

	g_iCurrViewNo = viewNumber;
	//	if(viewNumber== 5)
	//	{
	//		m_wndSplitter2.ShowView(viewNumber + 1);
	//		return;
	//	}

	m_wndSplitter4.ShowView(viewNumber + 1);
	if(viewNumber== _TEACH_DISP_VIEW)
		if(g_bCfrmTeachViewInit) GetWindowHandler_TeachView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	CurrViewNo= viewNumber;

}
//===================================================================================
int CMainFrame::GetCurrViewNo()
	//-----------------------------------------------------------------------------------
{
	return CurrViewNo;
}
//=================================================================================== 
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
//-----------------------------------------------------------------------------------
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = 1920;
	lpMMI->ptMinTrackSize.y = 1080;
	lpMMI->ptMaxTrackSize.x = 1920;
	lpMMI->ptMaxTrackSize.y = 1080;

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}
//===================================================================================
void CMainFrame::AppExitProc()
	//-----------------------------------------------------------------------------------
{
	//	ParamFile()->SaveFile(ParamFile()->System_data.strCurrentModelName);	

	if (MyMessageBox(_T("프로그램을 종료하시겠습니까?"), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[PROGRAM] 1st => Finish Application Program.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[PROGRAM] 1st => Finish Application Program.!"));
	if (MyMessageBox(_T("[재 확인]. 프로그램을 종료하시겠습니까?"), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[PROGRAM] 2nd => Finish Application Program.! => CANCEL"));
		return;
	}

	PROGRAM_EXIT= TRUE;

	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;


	//-----------------
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
	//-----------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE);

	Sleep(500);
	
	pSystem.SystemClose();

	Sleep(100);
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[PROGRAM] Finish Application Program.!"));
	Log()->SaveLogFile(_SYSTEM_LOG, _T("************************************************************************"));

	CMainFrame::OnClose();
	//	this->SendMessage(WM_CLOSE);
	Sleep(1000); 
	exit(0);
}

//===================================================================================
bool CMainFrame::isView_Init()
{
	if(	g_bCfrmTitleViewInit &&
		g_bCfrmSubMenuViewInit &&
		g_bCfrmMainViewInit &&
		g_bCfrmDataViewInit &&
		g_bCfrmCameraViewInit &&
		g_bCfrmTeachViewInit &&
		g_bCfrmAlignViewInit &&
		g_bCfrmCalibViewInit &&
		g_bCfrmDebugViewInit &&
		g_bCfrmCompViewInit &&
//		g_bCfrmStampViewInit &&
		g_bCfrmSetupViewInit)
		return true;
	return false;
}

//===================================================================================
void CMainFrame::Data_Reload()
{
	if(g_bCfrmTeachViewInit) GetWindowHandler_TeachView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	if(g_bCfrmMainViewInit) GetWindowHandler_MainView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	if(g_bCfrmDataViewInit) GetWindowHandler_DataView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	if(g_bCfrmAlignViewInit) GetWindowHandler_AlignView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	if(g_bCfrmSetupViewInit) GetWindowHandler_SetupView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	if(g_bCfrmStampViewInit) GetWindowHandler_StampView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
	if(g_bCfrmCalibViewInit) GetWindowHandler_CalibView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
}
