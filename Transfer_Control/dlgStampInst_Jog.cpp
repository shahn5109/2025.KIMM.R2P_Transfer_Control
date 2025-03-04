// dlgStampInst_Jog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgStampInst_Jog.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CdlgStampInst_Jog 대화 상자입니다.
extern CSystemClass pSystem;
CParameterFile* CdlgStampInst_Jog::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgStampInst_Jog::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

IMPLEMENT_DYNAMIC(CdlgStampInst_Jog, CDialogEx)

CdlgStampInst_Jog::CdlgStampInst_Jog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgStampInst_Jog::IDD, pParent)
	, m_bRollZ_Enable(FALSE)
{

}

CdlgStampInst_Jog::~CdlgStampInst_Jog()
{
}

void CdlgStampInst_Jog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK3, m_bRollZ_Enable);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
}


BEGIN_MESSAGE_MAP(CdlgStampInst_Jog, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK3, &CdlgStampInst_Jog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgStampInst_Jog::OnBnClickedButton1)
END_MESSAGE_MAP()


// CdlgStampInst_Jog 메시지 처리기입니다.


BOOL CdlgStampInst_Jog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
	Jog_Mode= _MID_SPEED;
	m_btnJogModeSel1.SetValue(1);

	pSystem.GetMotionComponent()->Sync_Mode_Set(STAGE_1_ALIGN_PROC);
	
	g_bZ_InterLockFlag= FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CdlgStampInst_Jog::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_bZ_InterLockFlag= TRUE;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
BEGIN_EVENTSINK_MAP(CdlgStampInst_Jog, CDialogEx)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_SELECT_0, DISPID_CLICK, CdlgStampInst_Jog::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_SELECT_1, DISPID_CLICK, CdlgStampInst_Jog::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_SELECT_2, DISPID_CLICK, CdlgStampInst_Jog::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CdlgStampInst_Jog::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CdlgStampInst_Jog::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CdlgStampInst_Jog::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CdlgStampInst_Jog::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CdlgStampInst_Jog::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CdlgStampInst_Jog::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CdlgStampInst_Jog::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_JOG_Z_CW, DISPID_MOUSEUP, CdlgStampInst_Jog::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgStampInst_Jog, IDB_EXIT, DISPID_CLICK, CdlgStampInst_Jog::ClickExit, VTS_NONE)
	ON_EVENT(CdlgStampInst_Jog, IDB_EXIT2, DISPID_CLICK, CdlgStampInst_Jog::ClickExit2, VTS_NONE)
	ON_EVENT(CdlgStampInst_Jog, IDB_Z_WAIT_POS3, DISPID_CLICK, CdlgStampInst_Jog::ClickZWaitPos3, VTS_NONE)
END_EVENTSINK_MAP()


void CdlgStampInst_Jog::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
}


void CdlgStampInst_Jog::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
}


void CdlgStampInst_Jog::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
}


void CdlgStampInst_Jog::ClickExit()
{
	pSystem.GetMotionComponent()->Sync_Mode_Reset();
	g_bZ_InterLockFlag= TRUE;
	EndDialog(IDOK);
}


void CdlgStampInst_Jog::ClickExit2()
{
	pSystem.GetMotionComponent()->Sync_Mode_Reset();
	g_bZ_InterLockFlag= TRUE;
	EndDialog(IDCANCEL);
}
//===================================================================================
void CdlgStampInst_Jog::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(iStageJog_Select== STAGE_SEL_1)
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -m_dPtpMove_mm, _SPD_MODE);		
// 		else
// 			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_STAGE_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_STAGE_X, _DIR_CCW, SetSpeed);
	}
}


void CdlgStampInst_Jog::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================


void CdlgStampInst_Jog::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
/*
		if(m_dPtpMove_mm== 0) return;
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -m_dPtpMove_mm, _SPD_MODE);		
*/
	}
	else
	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_STAGE_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_STAGE_X, _DIR_CW, SetSpeed);
	}
}


void CdlgStampInst_Jog::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================
//===================================================================================
void CdlgStampInst_Jog::MouseDownJogZCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(fabs(m_dPtpMove_mm)> 0.2) return;
// 		pSystem.GetMotionComponent()->INC_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, -m_dPtpMove_mm, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
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
}

void CdlgStampInst_Jog::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CdlgStampInst_Jog::MouseDownJogZCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(fabs(m_dPtpMove_mm)> 0.2) return;
// 		pSystem.GetMotionComponent()->INC_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, m_dPtpMove_mm, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
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
}

void CdlgStampInst_Jog::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}

void CdlgStampInst_Jog::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	if(m_bRollZ_Enable)
	{
		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
	}
}


void CdlgStampInst_Jog::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CdlgStampInst_Jog::ClickZWaitPos3()
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;


	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}

	strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY));

	strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;

	//-----------------
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	//-----------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
}
//===================================================================================
BOOL CdlgStampInst_Jog::MoveEndCheck(int Axis) 
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
