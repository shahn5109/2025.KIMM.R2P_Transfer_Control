// dlgOrigin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgOrigin.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern CSystemClass pSystem;
CParameterFile* CdlgOrigin::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgOrigin::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgOrigin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgOrigin, CDialogEx)

CdlgOrigin::CdlgOrigin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgOrigin::IDD, pParent)
{
	dwCheckTimeStart_Total= 0;
	dwCheckTimeStart_Axis= 0;
}

CdlgOrigin::~CdlgOrigin()
{
}

void CdlgOrigin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_AXIS_STAGE_X, m_btnStage_X);
	DDX_Control(pDX, IDB_AXIS_BLK_ROLL, m_btnBlk_Roll);
	DDX_Control(pDX, IDB_AXIS_ROLL_Z, m_btnRoll_Z);
	DDX_Control(pDX, IDB_AXIS_ALIGN_STAGE, m_btnAlign_Y);
	DDX_Control(pDX, IDB_AXIS_TOP_CAM_Y, m_btnTop_Cam_Y);
	DDX_Control(pDX, IDB_AXIS_TOP_CAM_Z, m_btnTop_Cam_Z);
	DDX_Control(pDX, IDB_AXIS_BOT_CAM_X, m_btnBot_Cam_X);
	DDX_Control(pDX, IDB_AXIS_BOT_CAM_Y, m_btnBot_Cam_Y);
	DDX_Control(pDX, IDB_AXIS_BOT_CAM_Z, m_btnBot_Cam_Z);
	DDX_Control(pDX, IDB_READY2, m_btnPower);
}


BEGIN_MESSAGE_MAP(CdlgOrigin, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CdlgOrigin::OnBnClickedButtonCheck)
END_MESSAGE_MAP()


// CdlgOrigin 메시지 처리기입니다.
//===================================================================================
BOOL CdlgOrigin::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){ 
		if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)){          return TRUE;        }   
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//===================================================================================
BOOL CdlgOrigin::OnInitDialog() 
{
	CDialogEx::OnInitDialog();


	SetTimer(ORIGIN_TIMER_ID, ORIGIN_TIMER_INTERVAL, NULL);

	CString strTemp;
	strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	

	m_btnStage_X.SetPicture(strTemp.GetBuffer(0));
	m_btnBlk_Roll.SetPicture(strTemp.GetBuffer(0));
	m_btnRoll_Z.SetPicture(strTemp.GetBuffer(0));
	m_btnAlign_Y.SetPicture(strTemp.GetBuffer(0));
	m_btnTop_Cam_Y.SetPicture(strTemp.GetBuffer(0));
	m_btnTop_Cam_Z.SetPicture(strTemp.GetBuffer(0));
	m_btnBot_Cam_X.SetPicture(strTemp.GetBuffer(0));
	m_btnBot_Cam_Y.SetPicture(strTemp.GetBuffer(0));
	m_btnBot_Cam_Z.SetPicture(strTemp.GetBuffer(0));
	m_btnPower;


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//===================================================================================
void CdlgOrigin::OnDestroy() 
{
	CDialogEx::OnDestroy();

	KillTimer(ORIGIN_TIMER_ID);

}
//===================================================================================
HBRUSH CdlgOrigin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//===================================================================================
void CdlgOrigin::OnTimer(UINT nIDEvent) 
	//-----------------------------------------------------------------------------------
{
	CString strTemp;
	if(pSystem.IsMotionSystemInitial() || SIMULATION_MODE) 
	{

		if(pSystem.GetIO()->isMotorPower_Check())
		{
			m_btnPower.SetValue(1);
			m_btnPower.SetCaption(_T("전원 OFF"));
		}
		else
		{
			m_btnPower.SetValue(0);
			m_btnPower.SetCaption(_T("모터 전원 ON"));
		}

		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_STAGE_X))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnStage_X.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnStage_X.SetPicture(strTemp.GetBuffer(0)); }
		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z2))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnRoll_Z.SetPicture(strTemp.GetBuffer(0)); }
		else
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnRoll_Z.SetPicture(strTemp.GetBuffer(0)); }
		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnBlk_Roll.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnBlk_Roll.SetPicture(strTemp.GetBuffer(0)); }
		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_UPPER_CAM_Z))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnTop_Cam_Z.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnTop_Cam_Z.SetPicture(strTemp.GetBuffer(0)); }
		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_UPPER_CAM_Y))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnTop_Cam_Y.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnTop_Cam_Y.SetPicture(strTemp.GetBuffer(0)); }

		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_X))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnBot_Cam_X.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnBot_Cam_X.SetPicture(strTemp.GetBuffer(0)); }

		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Y))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnBot_Cam_Y.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnBot_Cam_Y.SetPicture(strTemp.GetBuffer(0)); }

		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Z))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnBot_Cam_Z.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnBot_Cam_Z.SetPicture(strTemp.GetBuffer(0)); }

		if(pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_ALIGN_Y1) && pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_ALIGN_Y2))		
		{ strTemp.Format(_T("%s\\Bin\\gear_ok.png"), _PROGRAM_ROOT_DIR);	m_btnAlign_Y.SetPicture(strTemp.GetBuffer(0)); }
		else																	
		{ strTemp.Format(_T("%s\\Bin\\gear_forbidden.png"), _PROGRAM_ROOT_DIR); m_btnAlign_Y.SetPicture(strTemp.GetBuffer(0)); }
		//-----------------------------	
		if(g_AxisHomeStat[_ACS_AXIS_STAGE_X]== _MT_END) 							m_btnStage_X.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_STAGE_X))	m_btnStage_X.SetBackColorInterior(0x0062FFF3); // Org-End
			else																	m_btnStage_X.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_IMP_ROLL]== _MT_END) 							m_btnBlk_Roll.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_IMP_ROLL))	m_btnBlk_Roll.SetBackColorInterior(0x0062FFF3); // Org-End
			else																	m_btnBlk_Roll.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Z]== _MT_END) 						m_btnTop_Cam_Z.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_UPPER_CAM_Z))	m_btnTop_Cam_Z.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnTop_Cam_Z.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Y]== _MT_END) 							m_btnTop_Cam_Y.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_UPPER_CAM_Y))	m_btnTop_Cam_Y.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnTop_Cam_Y.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_X]== _MT_END) 							m_btnBot_Cam_X.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_LOWER_CAM_X))	m_btnBot_Cam_X.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnBot_Cam_X.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Y]== _MT_END) 							m_btnBot_Cam_Y.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_LOWER_CAM_Y))	m_btnBot_Cam_Y.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnBot_Cam_Y.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]== _MT_END) 							m_btnBot_Cam_Z.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_LOWER_CAM_Z)) 	m_btnBot_Cam_Z.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnBot_Cam_Z.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_ALIGN_Y1]== _MT_END && g_AxisHomeStat[_ACS_AXIS_ALIGN_Y2]== _MT_END) 		
																						m_btnAlign_Y.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_ALIGN_Y1) && pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_ALIGN_Y2))	
																						m_btnAlign_Y.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnAlign_Y.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		if(g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]== _MT_END && g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]== _MT_END) 		
																						m_btnRoll_Z.SetBackColorInterior(0x00FF6F6F); // Org-End
		else 
		{
			if(pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isHomeReturnCheck(_ACS_AXIS_IMP_ROLL_Z2))	
																						m_btnRoll_Z.SetBackColorInterior(0x0062FFF3); // Org-End
			else																		m_btnRoll_Z.SetBackColorInterior(0x00BFFFFF); // Org-End
		}

		//-----------------------------	
	}
	CDialog::OnTimer(nIDEvent);
}

//===================================================================================
void CdlgOrigin::OnBnClickedButtonCheck()
{
/*
#define _ACS_AXIS_STAGE_X			0
#define _ACS_AXIS_ENC_1	 			1
#define _ACS_AXIS_IMP_ROLL 			2
#define _ACS_AXIS_3					3

#define _ACS_AXIS_UPPER_CAM_Y		4
#define _ACS_AXIS_LOWER_CAM_X		5
#define _ACS_AXIS_LOWER_CAM_Y		6
#define _ACS_AXIS_LOWER_CAM_Z		7

#define _ACS_AXIS_UPPER_CAM_Z		8
#define _ACS_AXIS_9					9
#define _ACS_AXIS_ALIGN_Y1			10
#define _ACS_AXIS_ALIGN_Y2			11

#define _ACS_AXIS_IMP_ROLL_Z1		12
#define _ACS_AXIS_IMP_ROLL_Z2		13
#define _ACS_AXIS_14				14
#define _ACS_AXIS_15				15
*/

	int i;

	for(i= 0; i< _MAX_AXIS; i++)
	{
		if(pSystem.GetMotionComponent()->isHomeReturnCheck(i))	
		{		pSystem.GetMotionComponent()->AxisHomeStat[i]= g_AxisHomeStat[i]= _MT_END;	}
	}
}

//===================================================================================
BEGIN_EVENTSINK_MAP(CdlgOrigin, CDialogEx)
	ON_EVENT(CdlgOrigin, IDOK, DISPID_CLICK, CdlgOrigin::ClickOk, VTS_NONE)
	ON_EVENT(CdlgOrigin, IDB_UNIT_INIT_ALL_SELECT, DISPID_CLICK, CdlgOrigin::ClickUnitInitAllSelect, VTS_NONE)
	ON_EVENT(CdlgOrigin, IDB_SERVO_ON, DISPID_CLICK, CdlgOrigin::ClickServoOn, VTS_NONE)
	ON_EVENT(CdlgOrigin, IDB_SERVO_OFF, DISPID_CLICK, CdlgOrigin::ClickServoOff, VTS_NONE)
	ON_EVENT(CdlgOrigin, IDB_READY2, DISPID_CLICK, CdlgOrigin::ClickReady2, VTS_NONE)
	ON_EVENT(CdlgOrigin, IDB_INIT_RUN, DISPID_CLICK, CdlgOrigin::ClickInitRun, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================


void CdlgOrigin::ClickOk()
{
	OnOK();
}


//===================================================================================
void CdlgOrigin::ClickUnitInitAllSelect()
{
	m_btnStage_X.SetValue(TRUE);
	m_btnBlk_Roll.SetValue(TRUE);
	m_btnRoll_Z.SetValue(TRUE);
	m_btnAlign_Y.SetValue(TRUE);
	m_btnTop_Cam_Y.SetValue(TRUE);
	m_btnTop_Cam_Z.SetValue(TRUE);
	m_btnBot_Cam_X.SetValue(TRUE);
	m_btnBot_Cam_Y.SetValue(TRUE);
	m_btnBot_Cam_Z.SetValue(TRUE);
}

//===================================================================================
void CdlgOrigin::ClickServoOn()
{
	if(m_btnStage_X.GetValue())		pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_STAGE_X);
	if(m_btnBlk_Roll.GetValue())	pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_IMP_ROLL);
	if(m_btnTop_Cam_Z.GetValue())	pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_UPPER_CAM_Z);
	if(m_btnTop_Cam_Y.GetValue())	pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_UPPER_CAM_Y);
	if(m_btnBot_Cam_X.GetValue())	pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_LOWER_CAM_X);
	if(m_btnBot_Cam_Y.GetValue())	pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_LOWER_CAM_Y);
	if(m_btnBot_Cam_Z.GetValue())	pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_LOWER_CAM_Z);
	if(m_btnAlign_Y.GetValue())	{ pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_ALIGN_Y1); pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_ALIGN_Y2); }
	if(m_btnRoll_Z.GetValue())	{ pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_IMP_ROLL_Z1); pSystem.GetMotionComponent()->ServoOn(_ACS_AXIS_IMP_ROLL_Z2); }
	m_btnStage_X.SetValue(FALSE);
	m_btnBlk_Roll.SetValue(FALSE);
	m_btnRoll_Z.SetValue(FALSE);
	m_btnAlign_Y.SetValue(FALSE);
	m_btnTop_Cam_Y.SetValue(FALSE);
	m_btnTop_Cam_Z.SetValue(FALSE);
	m_btnBot_Cam_X.SetValue(FALSE);
	m_btnBot_Cam_Y.SetValue(FALSE);
	m_btnBot_Cam_Z.SetValue(FALSE);
}

//===================================================================================
void CdlgOrigin::ClickServoOff()
{
	if(m_btnStage_X.GetValue())		pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_STAGE_X);
	if(m_btnBlk_Roll.GetValue())	pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_IMP_ROLL);
	if(m_btnTop_Cam_Z.GetValue())	pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_UPPER_CAM_Z);
	if(m_btnTop_Cam_Y.GetValue())	pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_UPPER_CAM_Y);
	if(m_btnBot_Cam_X.GetValue())	pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_LOWER_CAM_X);
	if(m_btnBot_Cam_Y.GetValue())	pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_LOWER_CAM_Y);
	if(m_btnBot_Cam_Z.GetValue())	pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_LOWER_CAM_Z);
	if(m_btnAlign_Y.GetValue())	{ pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_ALIGN_Y1); pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_ALIGN_Y2); }
	if(m_btnRoll_Z.GetValue())	{ pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_IMP_ROLL_Z1); pSystem.GetMotionComponent()->ServoOff(_ACS_AXIS_IMP_ROLL_Z2); }
	m_btnStage_X.SetValue(FALSE);
	m_btnBlk_Roll.SetValue(FALSE);
	m_btnRoll_Z.SetValue(FALSE);
	m_btnAlign_Y.SetValue(FALSE);
	m_btnTop_Cam_Y.SetValue(FALSE);
	m_btnTop_Cam_Z.SetValue(FALSE);
	m_btnBot_Cam_X.SetValue(FALSE);
	m_btnBot_Cam_Y.SetValue(FALSE);
	m_btnBot_Cam_Z.SetValue(FALSE);
}
//===================================================================================


void CdlgOrigin::ClickReady2()
{
	if(m_btnPower.GetValue())	
	{
		pSystem.GetIO()->MotorPower_On();
		m_btnPower.SetCaption(_T("전원 OFF"));
	}	
	else
	{
		pSystem.GetIO()->MotorPower_Off();
		m_btnPower.SetCaption(_T("모터 전원 ON"));
	}
}

//===================================================================================
void CdlgOrigin::ClickInitRun()
{
	int iret= 0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pSystem.GetIO()->isEMO_Check()) 
	{
		MyMessageBox(_T("비상 정지 버튼이 눌렸습니다."), _T("EMG-STOP"), M_ICONERROR);
		return ;
	}
	if(!pSystem.GetIO()->isMotorPower_Check()) 
	{
		MyMessageBox(_T("모터 전원이 OFF상태입니다."), _T("STOP"), M_ICONERROR);
		return ;
	}
	if(!pSystem.GetIO()->isAir_Check())
	{
		MyMessageBox(_T("Main Air를 확인 하십시요."), _T("STOP"), M_ICONERROR);
		return ;
	}
	g_ServoCheckFlag= FALSE;

	pSystem.GetMotionComponent()->ServoAllOn();
	Sleep(1000);
	
	if(!pSystem.GetMotionComponent()->isMotionServoOnCheck()) 
	{
		MyMessageBox(_T("모터 Servo-Off상태입니다."), _T("STOP"), M_ICONERROR);
		g_ServoCheckFlag= TRUE;
		return ;
	}

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[HOME] => 원점 복귀 작업.!"));

	if(	m_btnStage_X.GetValue()	||		m_btnBlk_Roll.GetValue()	||	m_btnRoll_Z.GetValue() ||
		m_btnAlign_Y.GetValue()	||		m_btnTop_Cam_Y.GetValue()	||	m_btnTop_Cam_Z.GetValue() ||
		m_btnBot_Cam_X.GetValue()	||	m_btnBot_Cam_Y.GetValue()	||	m_btnBot_Cam_Z.GetValue() 	)
	{
		;
	}
	else {
		MyMessageBox(_T("선택된 축이 없습니다."), _T("No Selected Axis"), M_ICONERROR);
		g_ServoCheckFlag= TRUE;
		return;
	}

	if (MyMessageBox(_T("선택된 모터의 원점 복귀 작업을 하시겠습니까?."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[HOME] => 원점 복귀 작업.! => CANCEL"));
		g_ServoCheckFlag= TRUE;
		return;
	}

	if(	m_btnStage_X.GetValue())
	{
		m_btnBot_Cam_Z.SetValue(1);		m_btnRoll_Z.SetValue(1);
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_STAGE_X]= g_AxisHomeStat[_ACS_AXIS_STAGE_X]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= _MT_START; 
	}
	if(	m_btnBlk_Roll.GetValue())
	{
		m_btnBot_Cam_Z.SetValue(1);		m_btnRoll_Z.SetValue(1);
		if(g_AxisHomeStat[_ACS_AXIS_STAGE_X]== _MT_END)
		{
			if(pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) > 10.0)
			{
				MyMessageBox(_T("Roll 원점복귀가 가능한 위치가 아닙니다.\nStage X축을 포함해서 원점 동작 하십시요."),
					 _T("Origin Failed"), M_ICONERROR);
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[HOME] => 원점복귀 동작이 실패"));
				return;
			}
		}
		else
		{
			m_btnStage_X.SetValue(1);
			pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_STAGE_X]= g_AxisHomeStat[_ACS_AXIS_STAGE_X]= _MT_START; 
		}
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= _MT_START; 
	}
	if(	m_btnAlign_Y.GetValue())
	{
		m_btnBot_Cam_Z.SetValue(1);		m_btnRoll_Z.SetValue(1);
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_ALIGN_Y1]= g_AxisHomeStat[_ACS_AXIS_ALIGN_Y1]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_ALIGN_Y2]= g_AxisHomeStat[_ACS_AXIS_ALIGN_Y2]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= _MT_START; 
	}
	if(	m_btnBot_Cam_X.GetValue())
	{
		m_btnBot_Cam_Z.SetValue(1);
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_X]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_X]= _MT_START; 
	}
	if(	m_btnBot_Cam_Y.GetValue())
	{
		m_btnBot_Cam_Z.SetValue(1);
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Y]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Y]= _MT_START; 
	}

	if(	m_btnBot_Cam_Z.GetValue())
	{
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_START; 
	}
	if(	m_btnTop_Cam_Y.GetValue())
	{
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_UPPER_CAM_Y]= g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Y]= _MT_START; 
	}
	if(	m_btnTop_Cam_Z.GetValue())
	{
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_UPPER_CAM_Z]= g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Z]= _MT_START; 
	}
	if(	m_btnRoll_Z.GetValue())
	{
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= _MT_START; 
		pSystem.GetMotionComponent()->AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= _MT_START; 
	}

	pSystem.GetMotionComponent()->Z2Home_Offset_Send();

	iret = Auto_Origin_Sequence();

	m_btnStage_X.SetValue(FALSE);
	m_btnBlk_Roll.SetValue(FALSE);
	m_btnRoll_Z.SetValue(FALSE);
	m_btnAlign_Y.SetValue(FALSE);
	m_btnTop_Cam_Y.SetValue(FALSE);
	m_btnTop_Cam_Z.SetValue(FALSE);
	m_btnBot_Cam_X.SetValue(FALSE);
	m_btnBot_Cam_Y.SetValue(FALSE);
	m_btnBot_Cam_Z.SetValue(FALSE);

	g_ServoCheckFlag= TRUE;
	if( iret == 0) { //원점검색 실패
		MyMessageBox(_T("원점복귀 동작이 실패했습니다."), _T("Origin Failed"), M_ICONERROR);
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[HOME] => 원점복귀 동작이 실패"));
	}
	else {
		MyMessageBox(_T("원점복귀가 완료되었습니다."), _T("Origin Finished"), M_ICONINFORMATION);
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[HOME] => 원점복귀가 완료"));
	}

}
//===================================================================================
int CdlgOrigin::HomeReturnCheck(long axis)
//-----------------------------------------------------------------------------------
{
/*
#ifdef _ACS_MAX_AXIS
	if(axis< _ACS_MAX_AXIS) {
		CString strText;	strText.Format("[ERROR] HomeReturnCheck 축 번호 에러!!! -> [Axis=%d]", axis);   AfxMessageBox("[HomeReturnCheck] 축 번호 에러!");   Log()->SaveLogFile(_ACS_MOTION_LOG, strText);	return 0;
	}
#endif
*/
	
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	DWORD dwEd;
	dwCheckTimeStart_Axis= GetTickCount();
	CString strText;
	
	strText.Format(_T("원점 복귀  작업 중 입니다. [%d]"), axis);
	
	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
	
	ProgWnd.SetRange(0, ParamFile()->System_data._HOME_TIME_OUT);
	ProgWnd.SetText(_T("%s\n 잠시만 기다려 주세요..."), strText);
	
						  /*
						  //	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
						  pFrm->m_ProgWait->TotalTime= ParamFile()->System_data._HOME_AXIS_TIME_OUT;
						  pFrm->m_ProgWait->strTitle.Format("%s\n 잠시만 기다려 주세요...", strText);
						  pFrm->m_ProgWait->strMsg= strText;
						  pFrm->ProgWaitView(true);
	*//////////////////////////////////////////////
	
	DWORD st= GetTickCount();
	((CTransfer_ControlApp *)AfxGetApp())->DoEvents();
	g_AxisHomeStat[axis]= _MT_RUN;
	DWORD progst= GetTickCount();
	
	while(1) {
		dwEd= GetTickCount()-st;
		ProgWnd.SetPos(GetTickCount() - st);
		if( g_AxisHomeStat[axis]== _MT_RUN) {
			if(  g_AxisHomeStat[axis]== _MT_RUN && 
				!pSystem.GetMotionComponent()->isHomeBusy(axis) && pSystem.GetMotionComponent()->isDone(axis)) 
			{
				if(!pSystem.GetMotionComponent()->HomeEndProc(axis)) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 완료 동작 에러"), pSystem.GetMotionComponent()->GetMotionName(axis), axis); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
				Sleep(100);	//ProgWnd.StepIt();
				pSystem.GetMotionComponent()->ResetPos(axis); pSystem.GetMotionComponent()->AxisHomeStat[axis]= g_AxisHomeStat[axis]= _MT_END;
				
				/////////////////////////////////////////////////////////////////
				return 1;
			}
		}
		//		}
		if(pSystem.GetIO()->isEMO_Check()) 
		{
			return 0;
		}
		if(GetTickCount() - dwCheckTimeStart_Axis > (DWORD)ParamFile()->System_data._HOME_TIME_OUT)  {
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[HOME-ERROR] => Time-Out [Axis]"));
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			return 0;
		}
		//ProgWnd.StepIt();
		/// 사용자가 캔슬 버튼을 눌렀다.
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
		if ( ProgWnd.Cancelled() )
		{
			if     (axis== _ACS_AXIS_IMP_ROLL_Z1 || axis== _ACS_AXIS_IMP_ROLL_Z2)	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
			else if(axis== _ACS_AXIS_ALIGN_Y1 || axis== _ACS_AXIS_ALIGN_Y2)			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
			else 			pSystem.GetMotionComponent()->MoveStop(axis); 
			pSystem.GetMotionComponent()->HomeSearchErrorProc();

			Log()->SaveLogFile(_MOTION_LOG, _T("원점 동작이 취소 되었습니다......"));
			return FALSE;
		}
		Sleep(9);
	}
	return 0;
}
//===================================================================================
int CdlgOrigin::Auto_Origin_Sequence()
	//-----------------------------------------------------------------------------------
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	CString strText;

	int Seq_Step= 0;
	BOOL m_bAlloriginselectflag= FALSE;
	BOOL HOME_FLAG_BZ= FALSE, HOME_FLAG_Z1= FALSE, HOME_FLAG_Z2= FALSE;
	BOOL HOME_FLAG_BX= FALSE, HOME_FLAG_BY= FALSE;
	BOOL HOME_FLAG_TY= FALSE, HOME_FLAG_TZ= FALSE, HOME_FLAG_R= FALSE;
	BOOL HOME_FLAG_X= FALSE, HOME_FLAG_AY= FALSE;

	dwCheckTimeStart_Total= GetTickCount();
	// STEP #1
	if(	m_btnBot_Cam_Z.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_LOWER_CAM_Z) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_BZ= TRUE;
	}
	if(	m_btnRoll_Z.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_IMP_ROLL_Z1) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));

		HOME_FLAG_Z1= TRUE;
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_IMP_ROLL_Z2) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_Z2= TRUE;
	}

	if(	m_btnBot_Cam_Z.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_LOWER_CAM_Z)) 
		{	
			if(HOME_FLAG_BZ== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_LOWER_CAM_Z) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_LOWER_CAM_Z)) 	
			{
				pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
				return 0;
			}
			else {				g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_END;			}

			Sleep(500);
			if(!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
			{
				g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]= _MT_NONE;
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 센서 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_LOWER_CAM_Z);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}
	if(	m_btnRoll_Z.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_IMP_ROLL_Z2)) 
		{	
			if(HOME_FLAG_Z1== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_IMP_ROLL_Z1) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(HOME_FLAG_Z2== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_IMP_ROLL_Z2) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_IMP_ROLL_Z1)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= _MT_END;			}
			if(!HomeReturnCheck(_ACS_AXIS_IMP_ROLL_Z2)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= _MT_END;			}

			Sleep(500);
			if(!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_IMP_ROLL_Z1) || !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_IMP_ROLL_Z2))
			{
				g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]= _MT_NONE; g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]= _MT_NONE;
				pSystem.GetMotionComponent()->HomeSearchErrorProc();
				strText.Format(_T("[HOME] => %s[%d] 원점 센서 감지 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); 
				pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
				return 0;
			}
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_IMP_ROLL_Z1);	
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_IMP_ROLL_Z2);	
			Sleep(500);
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}
	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X, POSITION_READY))
	{
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[HOME] => Z축이 초기화가 안되어 있습니다."));
		return 0;
	}
	//---------------------------------------------------------------------------------------------
	// STEP #2
	if(	m_btnTop_Cam_Y.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_UPPER_CAM_Y) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_TY= TRUE;
	}
	if(	m_btnTop_Cam_Z.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_UPPER_CAM_Z) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z), _ACS_AXIS_UPPER_CAM_Z); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z), _ACS_AXIS_UPPER_CAM_Z); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_TZ= TRUE;
	}
	if(g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]!= _MT_END || g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]!= _MT_END || g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]!= _MT_END)
	{	
		pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
		strText.Format(_T("[HOME-ERROR] => Z축 원점 미실시! %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		return 0;
	}
	if(	m_btnAlign_Y.GetValue()) 
	{
/*
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_ALIGN_Y2) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_R= TRUE;
*/
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_ALIGN_Y1) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_AY= TRUE;
	}
	//---
	if(	m_btnAlign_Y.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_ALIGN_Y1]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_ALIGN_Y1) && pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_ALIGN_Y2)) 
		{	
			if(HOME_FLAG_AY== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_ALIGN_Y1) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
/*
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_ALIGN_Y2) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0;
				}
*/
			}
			if(!HomeReturnCheck(_ACS_AXIS_ALIGN_Y1)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_ALIGN_Y1]= _MT_END;			}
			if(!HomeReturnCheck(_ACS_AXIS_ALIGN_Y2)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_ALIGN_Y2]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_ALIGN_Y1);	
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_ALIGN_Y2);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}

	//---------------------------------------------------------------------------------------------
	// STEP #3
	if(	m_btnBot_Cam_X.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_LOWER_CAM_X) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_BX= TRUE;
	}
	if(	m_btnBot_Cam_Y.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_LOWER_CAM_Y) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_BY= TRUE;
	}
	
	if(	m_btnStage_X.GetValue()) 
	{
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_STAGE_X) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_X= TRUE;
	}
	//---
	if(	m_btnTop_Cam_Y.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Y]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_UPPER_CAM_Y)) 
		{	
			if(HOME_FLAG_TY== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_UPPER_CAM_Y) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_UPPER_CAM_Y)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Y]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_UPPER_CAM_Y);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}
	if(	m_btnTop_Cam_Z.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Z]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_UPPER_CAM_Z)) 
		{	
			if(HOME_FLAG_TZ== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_UPPER_CAM_Z) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z), _ACS_AXIS_UPPER_CAM_Z); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_UPPER_CAM_Z)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_UPPER_CAM_Z]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z), _ACS_AXIS_UPPER_CAM_Z); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_UPPER_CAM_Z);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z), _ACS_AXIS_UPPER_CAM_Z); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}
	if(	m_btnStage_X.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_STAGE_X]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_STAGE_X)) 
		{	
			if(HOME_FLAG_X== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_STAGE_X) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_STAGE_X)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_STAGE_X]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_STAGE_X);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}
	//---------------------------------------------------------------------------------------------
	// STEP #4
	//---
	if(	m_btnBot_Cam_X.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_X]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_LOWER_CAM_X)) 
		{	
			if(HOME_FLAG_BX== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_LOWER_CAM_X) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_LOWER_CAM_X)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_X]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_LOWER_CAM_X);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}
	if(	m_btnBot_Cam_Y.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Y]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_LOWER_CAM_Y)) 
		{	
			if(HOME_FLAG_BY== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_LOWER_CAM_Y) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_LOWER_CAM_Y)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Y]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_LOWER_CAM_Y);	
		}
		else 
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}

	if( !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_IMP_ROLL_Z1) || !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_IMP_ROLL_Z2))
	{
		pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
		strText.Format(_T("[HOME-ERROR] => Z축 원점 센서 에러! %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		return 0;
	}

	if(g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]!= _MT_END || g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]!= _MT_END || g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]!= _MT_END)
	{	
		pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
		strText.Format(_T("[HOME-ERROR] => Z축 원점 미실시! %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		return 0;
	}

	if(	m_btnBlk_Roll.GetValue()) 
	{
		if(pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) > 10.0)
		{
			MyMessageBox(_T("Roll 원점복귀가 가능한 위치가 아닙니다.\nStage X축을 포함해서 원점 동작 하십시요."),
				_T("Origin Failed"), M_ICONERROR);
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[HOME] => 원점복귀 동작이 실패"));
			return 0;
		}
		if(g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]!= _MT_END || g_AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]!= _MT_END || g_AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]!= _MT_END)
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => Z축 원점 미실시! %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_IMP_ROLL) ) 
		{	
			pSystem.GetMotionComponent()->HomeSearchErrorProc(); 	
			strText.Format(_T("[HOME-ERROR] => %s[%d] -MotionHome"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
		strText.Format(_T("[HOME] => %s[%d] 시작"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
		HOME_FLAG_R= TRUE;
	}

	if(	m_btnBlk_Roll.GetValue()) 
	{
		if( g_AxisHomeStat[_ACS_AXIS_IMP_ROLL]== _MT_START && 
			pSystem.GetMotionComponent()->isMotion_AlarmCheck(_ACS_AXIS_IMP_ROLL)) 
		{	
			if(HOME_FLAG_R== FALSE) 
			{
				if(!pSystem.GetMotionComponent()->MotionHome(_ACS_AXIS_IMP_ROLL) ) 
				{
					pSystem.GetMotionComponent()->HomeSearchErrorProc();
					strText.Format(_T("[HOME] => %s[%d] 원점 명령 에러"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
					pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
					return 0;
				}
			}
			if(!HomeReturnCheck(_ACS_AXIS_IMP_ROLL)) 	{			pSystem.GetMotionComponent()->HomeSearchErrorProc(); return 0; }
			else {				g_AxisHomeStat[_ACS_AXIS_IMP_ROLL]= _MT_END;			}
			Sleep(500);
			strText.Format(_T("[HOME] => %s[%d] 완료"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_IMP_ROLL);	
		}
		else
		{
			pSystem.GetMotionComponent()->HomeSearchErrorProc();
			strText.Format(_T("[HOME] => %s[%d] 알람 발생"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); 
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
			return 0;
		}
	}

// 	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY);
// 	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 
	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return 1; }

// 	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_READY);
// 	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 
// 	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_READY);
// 	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }




	return 1;

}