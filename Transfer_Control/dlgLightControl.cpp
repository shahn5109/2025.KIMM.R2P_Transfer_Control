// dlgLightControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgLightControl.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern CSystemClass pSystem;
CParameterFile* CdlgLightControl::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgLightControl::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgLightControl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgLightControl, CDialogEx)

CdlgLightControl::CdlgLightControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgLightControl::IDD, pParent)
	, m_iLightVal1(0)
{

}

CdlgLightControl::~CdlgLightControl()
{
}

void CdlgLightControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlLedSlider1);
	DDX_Control(pDX, IDC_SLIDER2, m_ctrlLedSlider2);
	DDX_Control(pDX, IDC_SLIDER3, m_ctrlLedSlider3);
	DDX_Control(pDX, IDC_SLIDER4, m_ctrlLedSlider4);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS9, m_iLightVal1);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS10, m_iLightVal2);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS11, m_iLightVal3);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS12, m_iLightVal4);
}


BEGIN_MESSAGE_MAP(CdlgLightControl, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CdlgLightControl 메시지 처리기입니다.


BOOL CdlgLightControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrlLedSlider1.SetRange(0, 255) ;
	m_ctrlLedSlider2.SetRange(0, 255) ;
	m_ctrlLedSlider3.SetRange(0, 255) ;
	m_ctrlLedSlider4.SetRange(0, 255) ;
	m_iLightVal1= m_iLightVal2= m_iLightVal3= m_iLightVal4= 0;
// 	m_ctrlLedSlider1.SetPos(ParamFile()->System_data.iLED_Top_Val_2X[ModelGroupNo]) ;
// 	m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_2X[ModelGroupNo];
// 
// 		else
// 		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Top_Val_20X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_20X[ModelGroupNo];	}
// 		UpdateData(FALSE) ;
// 	}
// 	else
// 	{
// 		if(LensNo== e2X_LENS)
// 		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Btm_Val_2X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_2X[ModelGroupNo];	}
// 		else
// 		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Btm_Val_20X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_20X[ModelGroupNo];	}
// 		UpdateData(FALSE) ;
// 	}
// 	SetLed(ModelGroupNo);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
BEGIN_EVENTSINK_MAP(CdlgLightControl, CDialogEx)
	ON_EVENT(CdlgLightControl, IDB_EXIT, DISPID_CLICK, CdlgLightControl::ClickExit, VTS_NONE)
END_EVENTSINK_MAP()


void CdlgLightControl::ClickExit()
{
	OnOK();
}


void CdlgLightControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	if(pScrollBar)
	{
		if(((CWnd*)pScrollBar)== GetDlgItem(IDC_SLIDER1))
		{
			int SldVal= m_ctrlLedSlider1.GetPos();
			SetDlgItemInt(IDC_EDIT_TIMEOUT_ALL_AXIS9, SldVal, TRUE);
			pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH1, _ON, SldVal); 
		}
		else if(((CWnd*)pScrollBar)== GetDlgItem(IDC_SLIDER2))
		{
			int SldVal= m_ctrlLedSlider2.GetPos();
			SetDlgItemInt(IDC_EDIT_TIMEOUT_ALL_AXIS10, SldVal, TRUE);
			pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH2, _ON, SldVal); 
		}
		else if(((CWnd*)pScrollBar)== GetDlgItem(IDC_SLIDER3))
		{
			int SldVal= m_ctrlLedSlider3.GetPos();
			SetDlgItemInt(IDC_EDIT_TIMEOUT_ALL_AXIS11, SldVal, TRUE);
			pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH3, _ON, SldVal); 
		}
		else if(((CWnd*)pScrollBar)== GetDlgItem(IDC_SLIDER4))
		{
			int SldVal= m_ctrlLedSlider4.GetPos();
			SetDlgItemInt(IDC_EDIT_TIMEOUT_ALL_AXIS12, SldVal, TRUE);
			pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH4, _ON, SldVal); 
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
