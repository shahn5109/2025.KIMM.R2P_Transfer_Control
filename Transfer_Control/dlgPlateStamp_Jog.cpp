// dlgPlateStamp_Jog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgPlateStamp_Jog.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern CSystemClass pSystem;
CParameterFile* CdlgPlateStamp_Jog::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgPlateStamp_Jog::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }


// CdlgPlateStamp_Jog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgPlateStamp_Jog, CDialogEx)

CdlgPlateStamp_Jog::CdlgPlateStamp_Jog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgPlateStamp_Jog::IDD, pParent)
	, m_dStamp_Mark(0)
	, m_dPosT1(0)
	, m_dPosT2(0)
	, m_dPosZ2_1(0)
	, m_dPosZ2_2(0)
{
	CurRoll_Pos_1= 0.0; CurZ1_Pos_1= CurZ2_Pos_1= 0.0; 
	CurRoll_Pos_2= 0.0; CurZ1_Pos_2= CurZ2_Pos_2= 0.0;
	CurX_Pos_1= 0.0; CurY_Pos_1= 0.0;
	CurX_Pos_2= 0.0; CurY_Pos_2= 0.0;
	retVal= 0.0;
}

CdlgPlateStamp_Jog::~CdlgPlateStamp_Jog()
{
}

void CdlgPlateStamp_Jog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
	DDX_Text(pDX, IDC_STAMP_MARK_X, m_dStamp_Mark);
	DDX_Text(pDX, IDC_EDIT_OFFSET_33, m_dPosT1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_35, m_dPosT2);
	DDX_Text(pDX, IDC_EDIT_OFFSET_34, m_dPosZ2_1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_36, m_dPosZ2_2);
}


BEGIN_MESSAGE_MAP(CdlgPlateStamp_Jog, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgPlateStamp_Jog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CdlgPlateStamp_Jog 메시지 처리기입니다.
//===================================================================================
BOOL CdlgPlateStamp_Jog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(m_calib_Mode== 0) //X-> Roll사용
	{
		GetDlgItem(IDB_JOG_Z2_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z2_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_HIDE);
		m_dStamp_Mark= ParamFile()->Model_data.dStamp_Len_X;
	}
	else
	{
		GetDlgItem(IDB_JOG_PB_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_PB_CCW)->ShowWindow(SW_HIDE);
		m_dStamp_Mark= ParamFile()->Model_data.dStamp_Len_Y;
	}
	GetDlgItem(IDB_JOG_SELECT_7)->EnableWindow(FALSE);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));

	Jog_Mode= _MID_SPEED;
	m_btnJogModeSel1.SetValue(1);

	SetTimer(dlgManuStampCalib_TIMER_ID, dlgManuStampCalib_TIMER_INTERVAL, NULL);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
//===================================================================================
void CdlgPlateStamp_Jog::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(dlgManuStampCalib_TIMER_ID);

}

//===================================================================================
BOOL CdlgPlateStamp_Jog::DestroyWindow()
{
	KillTimer(dlgManuAlign_TIMER_ID);
	pSystem.GetVisionComponent()->SetTempImageCtrl( NULL );

	return CDialogEx::DestroyWindow();
}

//===================================================================================
void CdlgPlateStamp_Jog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
BOOL CdlgPlateStamp_Jog::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){ 
		if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)){          return TRUE;        }   
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//===================================================================================
void CdlgPlateStamp_Jog::OnTimer(UINT_PTR nIDEvent)
{

	CString strText;
	double cur_pos= 0.0;
	pSystem.GetMotionComponent()->Position_RealAll();
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL]);		SetDlgItemText(IDC_CURPOS_1, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z1]);	SetDlgItemText(IDC_CURPOS_11, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z2]);	SetDlgItemText(IDC_CURPOS_12, strText.GetBuffer(0));

	CDialogEx::OnTimer(nIDEvent);
}
//===================================================================================
BEGIN_EVENTSINK_MAP(CdlgPlateStamp_Jog, CDialogEx)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_EXIT, DISPID_CLICK, CdlgPlateStamp_Jog::ClickExit, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_EXIT2, DISPID_CLICK, CdlgPlateStamp_Jog::ClickExit2, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_T_CW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_T_CW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_PB_CW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_PB_CW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_PB_CCW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_PB_CCW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_SELECT_0, DISPID_CLICK, CdlgPlateStamp_Jog::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_SELECT_1, DISPID_CLICK, CdlgPlateStamp_Jog::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_SELECT_2, DISPID_CLICK, CdlgPlateStamp_Jog::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_SELECT_6, DISPID_CLICK, CdlgPlateStamp_Jog::ClickJogSelect6, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_SELECT_7, DISPID_CLICK, CdlgPlateStamp_Jog::ClickJogSelect7, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_CM_MOVE6, DISPID_CLICK, CdlgPlateStamp_Jog::ClickCmMove6, VTS_NONE)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z2_CCW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z2_CCW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z2_CW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z2_CW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CdlgPlateStamp_Jog::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgPlateStamp_Jog, IDB_JOG_Z_CW, DISPID_MOUSEUP, CdlgPlateStamp_Jog::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
//===================================================================================

void CdlgPlateStamp_Jog::ClickExit()
{
	OnOK();
}
//===================================================================================

void CdlgPlateStamp_Jog::ClickExit2()
{
	OnCancel();
}
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogStageTCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

/*
	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 2.0) return;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
*/
			double SetSpeed;
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Z, _DIR_CW, SetSpeed);
//	}
}


void CdlgPlateStamp_Jog::MouseUpJogStageTCw(short Button, short Shift, long x, long y)
{
//	if(Jog_Mode!= _PTP_MOVE) 
//	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Z);
//	}
}
//===================================================================================


void CdlgPlateStamp_Jog::MouseDownJogStageTCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

/*
	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 2.0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
*/
			double SetSpeed;
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Z, _DIR_CCW, SetSpeed);
//	}
}

//===================================================================================
void CdlgPlateStamp_Jog::MouseUpJogStageTCcw(short Button, short Shift, long x, long y)
{
// 	if(Jog_Mode!= _PTP_MOVE) 
// 	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Z);
//	}
}
//===================================================================================
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogPbCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_PB_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
/*
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
*/
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL, _DIR_CCW, SetSpeed);
//	}
}


void CdlgPlateStamp_Jog::MouseUpJogPbCw(short Button, short Shift, long x, long y)
{
//	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogPbCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_PB_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
/*
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
*/
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL, _DIR_CW, SetSpeed);
//	}
}


void CdlgPlateStamp_Jog::MouseUpJogPbCcw(short Button, short Shift, long x, long y)
{
//	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
// 	if(Jog_Mode== _PTP_MOVE) 
// 	{
// 		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(iStageJog_Select== STAGE_SEL_1)
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -m_dPtpMove_mm, _SPD_MODE);		
// 		else
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, m_dPtpMove_mm, _SPD_MODE);		
// 	}
// 	else
// 	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_X, _DIR_CW, SetSpeed);
//	/}
}


void CdlgPlateStamp_Jog::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
{
// 	if(Jog_Mode!= _PTP_MOVE) 
// 	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_X);
//	}
}
//===================================================================================


void CdlgPlateStamp_Jog::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

 	double SetSpeed;
// 	if(Jog_Mode== _PTP_MOVE) {
// 		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(iStageJog_Select== STAGE_SEL_1)
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, m_dPtpMove_mm, _SPD_MODE);		
// 		else
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -m_dPtpMove_mm, _SPD_MODE);		
// 	}
// 	else
// 	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_X, _DIR_CCW, SetSpeed);
//	}
}


void CdlgPlateStamp_Jog::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
{
// 	if(Jog_Mode!= _PTP_MOVE) 
// 	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_X);
//	}
}
//===================================================================================


void CdlgPlateStamp_Jog::MouseDownJogCameraYCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_CAMERA_Y_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

 	double SetSpeed;
// 	if(Jog_Mode== _PTP_MOVE) {
// 		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, -m_dPtpMove_mm, _SPD_MODE);		
// 	}
// 	else
// 	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Y, _DIR_CCW, SetSpeed);
// 	}
}


void CdlgPlateStamp_Jog::MouseUpJogCameraYCcw(short Button, short Shift, long x, long y)
{
// 	if(Jog_Mode!= _PTP_MOVE) 
// 	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
//	}
}
//===================================================================================


void CdlgPlateStamp_Jog::MouseDownJogCameraYCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_CAMERA_Y_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
// 	if(Jog_Mode== _PTP_MOVE) {
// 		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, m_dPtpMove_mm, _SPD_MODE);		
// 	}
// 	else
// 	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Y, _DIR_CW, SetSpeed);
//	}
}


void CdlgPlateStamp_Jog::MouseUpJogCameraYCw(short Button, short Shift, long x, long y)
{
// 	if(Jog_Mode!= _PTP_MOVE) 
// 	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
//	}
}
//===================================================================================


void CdlgPlateStamp_Jog::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
}
//===================================================================================

void CdlgPlateStamp_Jog::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
}
//===================================================================================

void CdlgPlateStamp_Jog::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
}
//===================================================================================

 
void CdlgPlateStamp_Jog::ClickJogSelect6()
{
	CurZ1_Pos_1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	CurZ2_Pos_1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	CurRoll_Pos_1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
	CurX_Pos_1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
	CurY_Pos_1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	GetDlgItem(IDB_JOG_SELECT_7)->EnableWindow(TRUE);
	GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_HIDE);

// 	double CurRoll_Pos_1, CurZ_Pos_1;
// 	double CurRoll_Pos_2, CurZ_Pos_2;
// 
// 	double CurX_Pos_1, CurY_Pos_1;
// 	double CurX_Pos_2, CurY_Pos_2;
//	CString strText;
	//strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL]);		SetDlgItemText(IDC_CURPOS_1, strText.GetBuffer(0));
	m_dPosT1= CurRoll_Pos_1;
	m_dPosT2= 0.0;
	m_dPosZ2_1= CurZ2_Pos_1;
	m_dPosZ2_2= 0.0;
	UpdateData(FALSE);
}
//===================================================================================

void CdlgPlateStamp_Jog::ClickJogSelect7()
{
	CurZ1_Pos_2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	CurZ2_Pos_2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	CurRoll_Pos_2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
	CurX_Pos_2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
	CurY_Pos_2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);

	GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_SHOW);
	GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_SHOW);

	m_dPosT1= CurRoll_Pos_1;
	m_dPosT2= CurZ2_Pos_2;
	m_dPosZ2_1= CurZ2_Pos_1;
	m_dPosZ2_2= CurRoll_Pos_2;

	if(m_calib_Mode== 0) //X-> Roll사용
	{
		retVal= (CurRoll_Pos_1 + CurRoll_Pos_2) / 2.0;
	}
	else
	{

	}
	UpdateData(FALSE);
}
//===================================================================================


void CdlgPlateStamp_Jog::ClickCmMove6()
{
}
//===================================================================================
BOOL CdlgPlateStamp_Jog::MoveEndCheck(int Axis) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
	
	ProgWnd.SetRange(0, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
  
	ProgWnd.SetText( _T("%s\n  잠시만 기다려 주세요..."), strText);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
/*	pFrm->m_ProgWait->TotalTime= ParamFile()->System_data._AXIS_MOVE_TIME_OUT;
	pFrm->m_ProgWait->strTitle.Format("%s\n 잠시만 기다려 주세요...", strText);
	pFrm->m_ProgWait->strMsg= strText;
	pFrm->ProgWaitView(true);
	////////////////////////////////////////////////////////////////////////////////
	*/
	
	DWORD st= GetTickCount();
	DWORD progst= GetTickCount();
	((CTransfer_ControlApp *)AfxGetApp())->DoEvents();
	
	while(1) 
	{
		ProgWnd.SetPos(GetTickCount() - st);
		if(pSystem.GetMotionComponent()->isDone(Axis)) 
		{
				return TRUE;
		}
		Sleep(9);
		
		if(/*!pFrm->m_ProgWait->ProgEnable || */GetTickCount() - dwCheckTimeStart_Axis > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)  
		{
				pSystem.GetMotionComponent()->MoveStop(Axis); 
				return FALSE;
		}
		/// 사용자가 캔슬 버튼을 눌렀다.
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
		if ( ProgWnd.Cancelled() )
		{
			if     (Axis== _ACS_AXIS_IMP_ROLL_Z1 || Axis== _ACS_AXIS_IMP_ROLL_Z2)	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
			else if(Axis== _ACS_AXIS_ALIGN_Y1 || Axis== _ACS_AXIS_ALIGN_Y2)			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
			else 			pSystem.GetMotionComponent()->MoveStop(Axis); 

			Log()->SaveLogFile(_MOTION_LOG, _T("모션 동작이 취소 되었습니다......"));
			return FALSE;
		}
	}
	return FALSE;
}
//===================================================================================


void CdlgPlateStamp_Jog::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogZ2Ccw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z2_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL_Z2, _DIR_CCW, SetSpeed);
}

void CdlgPlateStamp_Jog::MouseUpJogZ2Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogZ2Cw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z2_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL_Z2, _DIR_CW, SetSpeed);
}

void CdlgPlateStamp_Jog::MouseUpJogZ2Cw(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogZCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, _DIR_CCW, SetSpeed);
}

void CdlgPlateStamp_Jog::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CdlgPlateStamp_Jog::MouseDownJogZCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, _DIR_CW, SetSpeed);
}

void CdlgPlateStamp_Jog::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
