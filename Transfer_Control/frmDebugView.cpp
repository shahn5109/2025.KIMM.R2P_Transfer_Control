// frmDebugView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmDebugView.h"
#include "MainFrm.h"


// CfrmDebugView
extern CSystemClass pSystem;
CParameterFile* CfrmDebugView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmDebugView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmDebugView, CFormView)

	CfrmDebugView::CfrmDebugView()
	: CFormView(CfrmDebugView::IDD)
{

}

CfrmDebugView::~CfrmDebugView()
{
}

void CfrmDebugView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CfrmDebugView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CfrmDebugView::OnNMDblclkList2)
END_MESSAGE_MAP()


// CfrmDebugView 진단입니다.

#ifdef _DEBUG
void CfrmDebugView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmDebugView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmDebugView 메시지 처리기입니다.

//===================================================================================
void CfrmDebugView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_DebugView(this);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ListCtrl.DeleteColumn(0);
	m_ListCtrl.InsertColumn(0, _T("Message"), LVCFMT_LEFT, 950 , 0); 
	ListClear();

	SetTimer(frmDebugView_TIMER_ID, frmDebugView_TIMER_INTERVAL, NULL);
	g_bCfrmDebugViewInit= TRUE;
}

//===================================================================================
void CfrmDebugView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmDebugView_TIMER_ID);
	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmDebugView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmDebugView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmDebugView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CString strText;

	if(pSystem.GetMotionComponent()->isAllMotorHomeEnd())
	{
// 		if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN)
// 		{
			switch(ParamFile()->System_data.iCURRENT_RUN_STATUS)
			{
			case _CURRENT_RUN_STOP:
				strText.Format(_T("AUTO_STOP"));
				break;
			case _CURRENT_RUN_RUN:
				strText.Format(_T("AUTO_RUN"));
				break;
			case _CURRENT_RUN_PAUSE:
				strText.Format(_T("AUTO_PAUSE"));
				break;
			case _CURRENT_RUN_ERROR:
				strText.Format(_T("AUTO_ERROR"));
				break;
			case _CURRENT_RUN_ERROR1: // POWER OFF
				strText.Format(_T("AUTO_ERROR"));
				break;
			case _CURRENT_RUN_ERROR2: // MOTOR-ERROR
				strText.Format(_T("AUTO_ERROR"));
				break;
			default:
				strText.Format(_T("AUTO_???"));
				break;
			}
// 		}
// 		else
// 		{
// 			switch(ParamFile()->System_data.iWORK_PROCESS_MODE)
// 			{
// 			case _PROCESS_AUTO_RUN:		strText.Format(_T("AUTO_RUN"));
// 				break;
// 			case _PROCESS_CYCLE_RUN:	strText.Format(_T("CYCLE_RUN"));		
// 				break;
// 			default:					strText.Format(_T("_RUN_MODE ?"));
// 				break;
// 			}
// 		}
		SetDlgItemText(IDC_OPMODE_2, strText.GetBuffer(0));
	}
	else
	{
		strText.Format(_T("INIT...!?"));		SetDlgItemText(IDC_OPMODE_2, strText.GetBuffer(0));
	}

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
void CfrmDebugView::ListClear()
{
	m_ListCtrl.DeleteAllItems ();

	m_ListCtrl.SetBkColor(RGB(255,230,230));
	m_ListCtrl.SetTextBkColor(RGB(255,230,230));
	// 리스트 컨트롤의 행 선택,Flat,라인.. 스타일
	m_ListCtrl.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES);
}
//===================================================================================
void CfrmDebugView::ListPrint(CString strText)
{
	CString str; 
	int aaa= m_ListCtrl.GetItemCount();

	str.Format(_T("%s"), strText); 			m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), str); 
	//	m_ListCtrl.SetFocus();
	m_ListCtrl.EnsureVisible(aaa, FALSE);
	//	m_ListCtrl.SetItemState(aaa, LVIS_SELECTED, LVIS_SELECTED );
	//	m_ListCtrl.SetCurSel();
}
//===================================================================================


void CfrmDebugView::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	ListClear();
}
