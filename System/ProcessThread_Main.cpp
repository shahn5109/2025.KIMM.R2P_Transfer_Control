// ProcessThread_Main.cpp: implementation of the CProcessThread_Main class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Transfer_Control.h"
#include "ProcessThread_Main.h"
#include "MainFrm.h"
#include <direct.h>	// Dos Command. //
#include "stdio.h"
#include "dlgManuAlign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSystemClass pSystem;
CParameterFile* CProcessThread_Main::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CProcessThread_Main::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcessThread_Main::CProcessThread_Main()
{
	if(pSystem.IsMotionSystemInitial()== ERR_SYSTEM_SUCCESS || SIMULATION_MODE)	IsIO_Init_Flag= true;
	else														IsIO_Init_Flag= false;

//	if(IsIO_Init_Flag) m_pMotion= pSystem.GetMotionComponent();

	m_pThread= NULL;
	m_bThreadLife = FALSE;
	m_bThreadTerminate = FALSE;
	m_eAutoStep= STEP_MAIN_INIT;
	m_eOldAutoStep= STEP_MAIN_DUMMY;

	m_eAutoDispStep= STEP_DISP_INIT;
	m_eOldAutoDispStep= STEP_DISP_DUMMY;

	bAIR_CheckFlag= bEMO_CheckFlag= false;
	bNoz_CheckFlag= false;

	bEndStopFlag= FALSE;

	bAIR_CheckFlag= false;
	bZAlm_CheckFlag= false;
	bServo_CheckFlag= false;
}

CProcessThread_Main::~CProcessThread_Main()
{

}
//===================================================================================
void CProcessThread_Main::ProcessStart()
//-----------------------------------------------------------------------------------
{
	if(pSystem.IsMotionSystemInitial()== ERR_SYSTEM_SUCCESS || SIMULATION_MODE)	IsIO_Init_Flag= true;
	else																		IsIO_Init_Flag= false;

	if(m_bThreadLife)
	{
		ProcessStop();
		Sleep(100);
	}
	m_bThreadTerminate = FALSE;
	m_bThreadLife = TRUE;	
	m_eAutoStep= STEP_MAIN_INIT;
	m_eOldAutoStep= STEP_MAIN_DUMMY;
	RecipeProc_Step= 0;
	m_pThread = AfxBeginThread(threadFunction, this);
}
//===================================================================================
void CProcessThread_Main::ProcessStop()
//-----------------------------------------------------------------------------------
{
	DWORD exitcode;

	m_bThreadLife = FALSE;
	Sleep(100);
	RecipeProc_Step= 0;

	if(!m_bThreadTerminate)
	{
		if(m_pThread != NULL)
		{
			exitcode = ::WaitForSingleObject(m_pThread->m_hThread, 3000);
			
			if(exitcode == WAIT_TIMEOUT)
				::TerminateThread(m_pThread->m_hThread, exitcode);
		}
	}
	m_pThread = NULL;
}
//===================================================================================
void CProcessThread_Main::setStep(EStepMain estepNext)
//-----------------------------------------------------------------------------------
{
	m_eAutoStep = estepNext;
}
//===================================================================================
EStepMain CProcessThread_Main::GetStep()
//-----------------------------------------------------------------------------------
{
	return m_eAutoStep;
}
//===================================================================================
void CProcessThread_Main::StepInitial()
//-----------------------------------------------------------------------------------
{
	m_eAutoStep= STEP_MAIN_INIT; //m_eOldAutoStep= 0;
}
//===================================================================================
BOOL CProcessThread_Main::IsAutoRun()
//-----------------------------------------------------------------------------------
{
	return m_bAutoRun;
}
//===================================================================================
void CProcessThread_Main::StepProcessStart()
//-----------------------------------------------------------------------------------
{
	m_eAutoStep= STEP_MAIN_START; //m_eOldAutoStep= 0;
	bEndStopFlag= FALSE;
	RecipeProc_Step= 0;
}
//===================================================================================
void CProcessThread_Main::StepProcessStop()
//-----------------------------------------------------------------------------------
{
	double ToPosition1	= 0.0;
	m_eAutoStep= STEP_MAIN_INIT; //m_eOldAutoStep= 0;

	pSystem.GetMotionComponent()->Sync_Stop();
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	TRUE);

//	if(ParamFile()->System_data.iWORK_MODE_SELECT== _RUN_MODE1 || 
//		ParamFile()->System_data.iWORK_MODE_SELECT== _RUN_MODE2) {


		pSystem.GetMotionComponent()->Z_ControlDisable();
		pSystem.GetMotionComponent()->Sync_Stop();
		pSystem.GetMotionComponent()->MotorAllStop();
		g_AutoRunAlignFlag= FALSE;

//	}
	RecipeProc_Step= 0;
}
//===================================================================================
void CProcessThread_Main::StepDispenserStart()
//-----------------------------------------------------------------------------------
{
	m_eAutoDispStep= STEP_DISP_STEP_START;
}
//===================================================================================
void CProcessThread_Main::StepDispenserStop()
//-----------------------------------------------------------------------------------
{
// 	pSystem.GetMotionComponent()->DispenserStop();
	m_eAutoDispStep= STEP_DISP_INIT;
}
//===================================================================================
BOOL CProcessThread_Main::TimeOutCheck()
//-----------------------------------------------------------------------------------
{
	if(GetTickCount() - dwCheckTimeStart_Axis > (DWORD)ParamFile()->System_data._HOME_TIME_OUT)  {
		pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
		pSystem.GetMotionComponent()->HomeSearchErrorProc(); 
		return TRUE;
	}
	return FALSE;
}
//===================================================================================
void CProcessThread_Main::threadJob()
//-----------------------------------------------------------------------------------
{
	if(pSystem.IsMotionSystemInitial()== ERR_SYSTEM_SUCCESS || SIMULATION_MODE)	IsIO_Init_Flag= true;
	else																		IsIO_Init_Flag= false;

//	DWORD aaa= (DWORD)( ((ParamFile()->Model_data.dPrint_Work_Length / ParamFile()->Model_data.dPrint_Speed)+2.0) * 1000 );

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
//	if(!pFrm->isView_Init()) return;

	if(!pSystem.GetMotionComponent()->isMotion_AlarmCheck() && PROGRAM_EXIT) return ;
	if(!pFrm->isView_Init()) return;

	if(g_ServoCheckFlag && !pSystem.GetMotionComponent()->isMotion_AlarmCheck())
	{
//		TRACE("FFFFFF\n");
		if(!bZAlm_CheckFlag) 
		{
			pSystem.GetMotionComponent()->Z_ControlDisable();
			StepInitial();
			
			bZAlm_CheckFlag= true;
			ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_ERROR;
			pSystem.GetMotionComponent()->MotorAll_EMG_Stop();
			
			pSystem.GetMotionComponent()->MotorAllStop();
			pSystem.GetMotionComponent()->MotorAllStop();
			
			pSystem.GetMotionComponent()->Sync_Stop();

			pFrm->GetWindowHandler_MainView()->StopProcess();
			
			pSystem.GetMotionComponent()->EMG_Proc();
			
			pSystem.GetIO()->Buzzer_Error();
			MyMessageBox(_T("축 알람 발생."), _T("STOP"), M_ICONERROR);
			pSystem.GetIO()->BuzzerStop();
		}
		return ;
	}
	else 
	{
		bZAlm_CheckFlag= false;
	}

	if(g_ServoCheckFlag)
	{
		if(GetTickCount()- ServoCheckTime> 3000)
		{
			ServoCheckTime= GetTickCount();
			if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_STAGE_X)  || !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL) ||
				!pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Z) ||
				!pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z1) || !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z2))
			{
				if(!bServo_CheckFlag)
				{
					pSystem.GetMotionComponent()->Z_ControlDisable();
					bServo_CheckFlag= true;
					ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_ERROR;
					pSystem.GetMotionComponent()->MotorAll_EMG_Stop();
					
					pSystem.GetMotionComponent()->MotorAllStop();
					
					pSystem.GetMotionComponent()->Sync_Stop();
					
					pFrm->GetWindowHandler_MainView()->StopProcess();
					
					pSystem.GetMotionComponent()->EMG_Proc();
					if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_STAGE_X))					MyMessageBox(_T("Stage X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL))				MyMessageBox(_T("ROLL 축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_X))			MyMessageBox(_T("하부 카메라 X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Y))			MyMessageBox(_T("하부 카메라 X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Z))			MyMessageBox(_T("하부 카메라 X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_UPPER_CAM_Y))			MyMessageBox(_T("상부 카메라 X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_UPPER_CAM_Y))			MyMessageBox(_T("상부 카메라 X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_ALIGN_Y1))				MyMessageBox(_T("Align Y1축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_ALIGN_Y2))				MyMessageBox(_T("Aling Y2 X축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z1))			MyMessageBox(_T("상부 Roll-Z1 축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
					else if( !pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z2))			MyMessageBox(_T("상부 Roll-Z1 축 서버 알람 발생!."), _T("EMG-STOP"), M_ICONERROR);
				}
				return ;
			}
			else
			{
				bServo_CheckFlag= false;
			}
		}
	}
	else bServo_CheckFlag= false;


	if(!pSystem.GetIO()->isAir_Check())
	{
		if(!bAIR_CheckFlag) 
		{
//			pSystem.GetMotionComponent()->Z_ControlDisable();
			StepInitial();
			
			bAIR_CheckFlag= true;
			ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_ERROR;
			pSystem.GetMotionComponent()->MotorAll_EMG_Stop();
			
			pSystem.GetMotionComponent()->MotorAllStop();
	
			pSystem.GetMotionComponent()->Sync_Stop();
			
			pFrm->GetWindowHandler_MainView()->StopProcess();
			
			pSystem.GetMotionComponent()->EMG_Proc();
			
			pSystem.GetIO()->Buzzer_Error();
			MyMessageBox(_T("Main Air를 확인 하십시요."), _T("STOP"), M_ICONERROR);
			pSystem.GetIO()->BuzzerStop();
		}
		return ;
	}
	else 
	{
		bAIR_CheckFlag= false;
	}

	if(pSystem.GetIO()->isEMO_Check())
	{
		if(!bEMO_CheckFlag) 
		{
			pSystem.GetMotionComponent()->Z_ControlDisable();
			StepInitial();

			bEMO_CheckFlag= true;
			ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_ERROR;
			pSystem.GetMotionComponent()->MotorAll_EMG_Stop();

			pSystem.GetMotionComponent()->MotorAllStop();
			
			pSystem.GetMotionComponent()->Sync_Stop();

			pFrm->GetWindowHandler_MainView()->StopProcess();

			pSystem.GetMotionComponent()->EMG_Proc();

			pSystem.GetIO()->Buzzer_Error();
			MyMessageBox(_T("비상 정지 버튼이 눌렸습니다."), _T("EMG-STOP"), M_ICONERROR);
			pSystem.GetIO()->BuzzerStop();
		}
		return ;
	}
	else 
	{
		bEMO_CheckFlag= false;
	}

	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_PAUSE) return;
	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_ERROR) return;
	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_STOP)
	{
		bFirstStartFlag= false;
		StepInitial();
		return;
	}
/*
//	int ParamFile()->System_data.iWORK_MODE_SELECT
#define _MAX_MODE			4
#define _RUN_MODE1			0
#define _RUN_MODE2			1
#define _RUN_MODE3			2
#define _RUN_MODE4			3
//	int ParamFile()->System_data.iWORK_PROCESS_MODE;
#define _PROCESS_AUTO_RUN		0
#define _PROCESS_CYCLE_RUN		1

//	int ParamFile()->System_data.iCURRENT_RUN_STATUS;
#define _CURRENT_RUN_STOP		0
#define _CURRENT_RUN_RUN		1
#define _CURRENT_RUN_PAUSE		2
#define _CURRENT_RUN_ERROR		9

//	BOOL ParamFile()->System_data.bEND_STOP_FLAG;
#define _CURRENT_ENDSTOP_DIS		FALSE
#define _CURRENT_ENDSTOP_ENA		TRUE
*/

// 	switch(ParamFile()->System_data.iWORK_MODE_SELECT)
// 	{
// 		case _RUN_MODE1:
// 		case _RUN_MODE2:
// 		case _RUN_MODE3:
// 		case _RUN_MODE4:
			Recipe_doRunStep();
// 			break;
// 		default:
// 			break;
// 	}
}
//===================================================================================
void CProcessThread_Main::doRunStep()
//-----------------------------------------------------------------------------------
{
	CString strText;
	BOOL ret= FALSE;
	int iDrySt= 0;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	if(m_eAutoDispStep != m_eOldAutoDispStep)
	{
		strText.Format(_T("[CProcessThread_Main] m_eAutoDispStep : [%04d] -> [%04d] "),m_eOldAutoDispStep, m_eAutoDispStep);
		Log()->SaveLogFile(_THREAD_LOG, strText);
//		TRACE(strText);		TRACE("\n");
		m_eOldAutoDispStep = m_eAutoDispStep;
	}
}
//===================================================================================
UINT CProcessThread_Main::threadFunction(LPVOID pParam)
//-----------------------------------------------------------------------------------
{
	CProcessThread_Main* pThis;
	pThis = (CProcessThread_Main*) pParam;

	pThis->Log()->SaveLogFile(_SYSTEM_LOG, _T("[Thread] CProcessThread_Main Run"));
//	TRACE("_SYSTEM_LOG, [Thread] CProcessThread_Main Run\n");

	while(pThis->m_bThreadLife)
	{
		if(pSystem.GetMotionComponent()->isMotionInitial() || SIMULATION_MODE) {
			pThis->threadJob();
		}
		else 		Sleep(100);
		Sleep(10);
	}

	pThis->m_bThreadTerminate = TRUE;
	pThis->Log()->SaveLogFile(_SYSTEM_LOG, _T("[Thread] CProcessThread_Main Stop"));
	TRACE("_SYSTEM_LOG, [Thread] CProcessThread_Main Stop\n");

	return 0;
}

//===================================================================================
void CProcessThread_Main::Recipe_doRunStep() 
	//-----------------------------------------------------------------------------------
{
	CString strText;
	BOOL ret= FALSE;
	int iDrySt= 0;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CString strSaveName, strMessage;

	double currPosX= 0.0;
	double ToPositionX=		0.0;	
	double ToPositionResY=	0.0;
	double ToPositionY1=	0.0;
	double ToPositionY2=	0.0;
	double ToPositionZ1=	0.0;
	double ToPositionZ2=	0.0;
	double ToPosition;
	double tarPosX= 0.0, tarPosY= 0.0;
	double ang_moveX= 0.0, ang_moveY= 0.0, ToPosition1= 0.0, ToPosition2= 0.0;

	double Blk_Roll_StartPos= 0.0;
	DWORD dwchkTime;
	double dchkTimeSum;

	int d1= 0, d2= 0, d3= 0, d4= 0, d5= 0;

	int mode= 0;
	int Stage_no= 0;

	double MarktoMarkGap_X= 0.0;
	//	DWORD sss;

	if(m_eAutoStep != m_eOldAutoStep)
	{
		strText.Format(_T("[Thread_Main][AUTO] m_eAutoStep : [%04d] -> [%04d] "), m_eOldAutoStep, m_eAutoStep);	Log()->SaveLogFile(_THREAD_LOG, strText);
		m_eOldAutoStep = m_eAutoStep;
	}
	switch(m_eAutoStep)
	{
	case STEP_MAIN_INIT:
		setStep(STEP_MAIN_START);
		break;
	case STEP_MAIN_START:
		pFrm->GetWindowHandler_MainView()->GraphDataInital();
		AlignDataClear();
		dwCheckTimeStart_PROC= GetTickCount();
		Stage_Proc_No= STAGE_1_ALIGN_PROC; Stage_No= _STAGE_1;		Stage_Pos_No= POSITION_STAGE1_ALIGN;
		setStep(STEP_MAIN_Z_SAFE_POS_START);
		break;
	case STEP_MAIN_Z_SAFE_POS_START:
		if( isAll_Done() ) 
		{
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("===========================  START ===========================")); 
			ToPositionZ1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPositionZ2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_Z_SAFE_POS_END);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_Z_SAFE_POS_END:
		if( isAll_Done() ) 
		{
			strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동 완료!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동 완료!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			setStep(STEP_MAIN_READY_POS_START);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_READY_POS_START:
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY);
		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition,	FALSE)) 
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y1, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y2, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_READY_POS_END);
		break;
	case STEP_MAIN_READY_POS_END:
		if( isAll_Done() ) 
		{
			strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동 완료!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동 완료!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(ParamFile()->Model_data.iRunEnable[SYNC_PROC])
			{
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------[Sync.-Run]  Stage #1, 2, 3  SKIP ----------")); 
				dwCheckTimeStart_PROC= GetTickCount();
				setStep(STEP_MAIN_SYNC_PROCESS_CHECK);
			}
			else	setStep(STEP_MAIN_STEP_RECIPE_START);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
//=====================================================================================
//Stage Sync.번 STAGE 시작 -------------------------
//-----------------------------------------------
	case STEP_MAIN_SYNC_PROCESS_CHECK:
		if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		{
			if( isAll_Done() ) 
			{
				if(ParamFile()->Model_data.iRunEnable[SYNC_PROC])
				{
					pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  STAGE Sync.  START ----------")); 
					ToPositionZ1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
					ToPositionZ2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
					strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
					if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
					{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

					strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
					if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(FALSE)) 
					{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
					dwCheckTimeStart_PROC= GetTickCount();
					setStep(STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_START);
				}
				else
				{
					pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  STAGE Sync.  SKIP ----------")); 
					dwCheckTimeStart_PROC= GetTickCount();
					setStep(STEP_MAIN_END_READY_POS_START1);
				}
			}
			else
			{
				if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
				{
					isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
				}
			}
		}
		else
		{
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  STAGE Sync.  SKIP ----------")); 
			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_END_READY_POS_START1);
		}

		break;
	case STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_START:
		if( isAll_Done() ) 
		{
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  STAGE Sync.   ----------")); 
			ToPositionX=	pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_SYNC);	
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  STAGE Sync.  위치로 이동 ----------")); 
			strText.Format(_T("[RUN] Get-Pos : X=%.3f, Y=%.3f"), ToPositionX, ToPositionResY);	Log()->SaveLogFile(_SYSTEM_LOG, strText);

			ToPositionY1=	pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_SYNC);	
			ToPositionY2=	pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_SYNC);	

			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPositionX,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y1, ToPositionY1,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y2, ToPositionY2,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_SYNC);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition,	FALSE)) 
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_CAL_START);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_CAL_START:
		if( isAll_Done() ) 
		{
			setStep(STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_START);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_CAL_END:
		if( isAll_Done() ) 
		{
			setStep(STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_START);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_START:
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T(" => [STAGE Sync.] Z축 Pisition-Mode")); 
		ToPositionZ1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_SYNC);
		ToPositionZ2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_SYNC);

		strText.Format(_T("[RUN] %s:%s ->모터 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
		{	strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_END);
		break;
	case STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_END:
		if( isAll_Done() ) 
		{
			setStep(STEP_MAIN_SYNC_Z_AXIS_START_POS_DELAY_END);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_SYNC_Z_AXIS_START_POS_DELAY_END:
		Sleep(1000);
		pSystem.GetMotionComponent()->ResetPos(_ACS_AXIS_ENC_1);
		setStep(STEP_MAIN_SYNC_PICKING_PROC_RUN_START);
		break;
	case STEP_MAIN_SYNC_PICKING_PROC_RUN_START:
		//pFrm->GetWindowHandler_Disp_View()->GetWindowHandler_GraphDisplayView()->StartGraph(SYNC_PROC);
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T(" => [STAGE Sync.] Sync. Start")); 
		strText.Format(_T("   Speed: %.3f, Len: %.3f, D: %.5f"),
			ParamFile()->Model_data.dRun_Speed[SYNC_PROC],		ParamFile()->Model_data.dRun_Length[SYNC_PROC],
			ParamFile()->System_data.dRoll_Diameter[SYNC_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[SYNC_PROC]);
		Log()->SaveLogFile(_SYSTEM_LOG, strText);

		dwCheckTimeStart_PROC= GetTickCount();
		if(pSystem.GetMotionComponent()->X_R_LinearMoveStart(SYNC_PROC)== FALSE)
		{	strText.Format(_T("[RUN] Sync. Run 에러 발생!") );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
		Start_Enc_Val= 0.0; End_Enc_Val= 0.0;	Cal_Enc_Val= 0.0; Cal_Diam= 0.0; Cal_Tmp= 0.0;
		Start_Enc_Val= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ENC_1); //ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]
		setStep(STEP_MAIN_SYNC_PICKING_PROC_RUN_END);
		break;
	case STEP_MAIN_SYNC_PICKING_PROC_RUN_END:
		if( isAll_Done() && pSystem.GetMotionComponent()->isSync_End()) 
		{
			End_Enc_Val= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ENC_1); //ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]
			pSystem.GetMotionComponent()->LinearMoveStop();
			pSystem.GetMotionComponent()->Z_ControlDisable();
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T(" => [STAGE Sync.] Sync. End")); 
			setStep(STEP_MAIN_SYNC_Z_AXIS_START_PROC_END_UP_START);
		}
		else
		{
			dchkTimeSum= (ParamFile()->Model_data.dRun_Length[SYNC_PROC] / ParamFile()->Model_data.dRun_Speed[SYNC_PROC]) * 1000.0;
			dwchkTime= (DWORD)(dchkTimeSum) + 10000;
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)(dwchkTime))
			{
				pSystem.GetMotionComponent()->LinearMoveStop();
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_SYNC_Z_AXIS_START_PROC_END_UP_START:
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T(" => [STAGE Sync.] Z축 - UP")); 

		ToPositionZ1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		ToPositionZ2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		strText.Format(_T("[RUN] %s:%s ->모터 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
		{	
			pFrm->GetWindowHandler_MainView()->EndGraph(SYNC_GRAPH);
			strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;
		}
		Sleep(300);
		pFrm->GetWindowHandler_MainView()->EndGraph(SYNC_GRAPH);
		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_SYNC_Z_AXIS_START_PROC_END_UP_END);
		break;
	case STEP_MAIN_SYNC_Z_AXIS_START_PROC_END_UP_END:
		if( isAll_Done() ) 
		{
			//Diam. Cal
			Cal_Enc_Val= Start_Enc_Val - End_Enc_Val;
			Cal_Diam= ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC] * _PI_;
			Cal_Tmp= Cal_Diam / ParamFile()->Model_data.dRun_Length[SYNC_PROC];
			Cal_Tmp= (Cal_Tmp * Cal_Enc_Val) ;
			Cal_Tmp= (Cal_Tmp/* +Cal_Diam*/) / _PI_;
			ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]= ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC] - Cal_Tmp;
			//			Cal_Tmp= (Cal_Tmp/* +Cal_Diam*/) / _PI_;
			ParamFile()->SaveSytemConfigFile();
			pFrm->GetWindowHandler_DataView()->SendMessage(WM_MSG_DATA_UPDATE, 0, 0);
			////////
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("-------   [STAGE Sync.]  End   -----------")); 
			setStep(STEP_MAIN_END_READY_POS_START1);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
		// Stage Sync. PROC_END & Z_UP_END ==================
		//=====================================================================================

//=======================================================================================================================
//==================================================================
// Recipe구동 동작 시작
//==================================================================
case STEP_MAIN_STEP_RECIPE_START://									= 6000,
	switch(ParamFile()->Recipe_data[RecipeProc_Step].iCommand)
	{
	case _OP_NONE://				0
	case _OP_PROC_END://			12
		setStep(STEP_MAIN_END_READY_POS_START1/*STEP_MAIN_STEP_PRINT_END_COUNT_CHECK*/);
		break;
	case _OP_MOVE_WAIT_ALL:		
	case _OP_MOVE_WAIT_X:		
	case _OP_MOVE_WAIT_Y:		
	case _OP_MOVE_WAIT_RZ:		
	case _OP_PROC_STAGE1:		
	case _OP_PROC_STAGE2:		
	case _OP_PROC_STAGE3:		
	case _OP_PROC_PLATE_ALIGN:		
	case _OP_PROC_VACUUM:		
	case _OP_PROC_DELAY:		
	case _OP_PROC_MESSAGE:		
		setStep(STEP_MAIN_STEP_RECIPE_COMMAND_START);
		break;
	default:
		setStep(STEP_MAIN_END_READY_POS_START1/*STEP_MAIN_STEP_PRINT_END_COUNT_CHECK*/);
	}
	break;
case STEP_MAIN_STEP_RECIPE_COMMAND_START://							= 6010,
	if(isAll_Done()) 
	{
		switch(ParamFile()->Recipe_data[RecipeProc_Step].iCommand)
		{
		case _OP_NONE://				0
		case _OP_PROC_END://			12
			setStep(STEP_MAIN_END_READY_POS_START1);
			break;
		case _OP_MOVE_WAIT_ALL:	
			setStep(STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_START);
			break;
		case _OP_MOVE_WAIT_X:		
			setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
			break;
		case _OP_MOVE_WAIT_Y:		
			setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
			break;
		case _OP_MOVE_WAIT_RZ:		
			setStep(STEP_MAIN_STEP_RECIPE_Z_READY_POS_MOVE_PROC_START);
			break;
		case _OP_PROC_STAGE1:		
			Stage_Proc_No= STAGE_1_ALIGN_PROC; Stage_No= _STAGE_1;		Stage_Pos_No= POSITION_STAGE1_ALIGN;
			setStep(STEP_MAIN_STAGE_PROCESS_CHECK);
			break;
		case _OP_PROC_STAGE2:		
			Stage_Proc_No= STAGE_2_ALIGN_PROC; Stage_No= _STAGE_2;		Stage_Pos_No= POSITION_STAGE2_ALIGN;
			setStep(STEP_MAIN_STAGE_PROCESS_CHECK);
			break;
		case _OP_PROC_STAGE3:	
			Stage_Proc_No= STAGE_3_ALIGN_PROC; Stage_No= _STAGE_3;		Stage_Pos_No= POSITION_STAGE3_ALIGN;
			setStep(STEP_MAIN_STAGE_PROCESS_CHECK);
			break;
		case _OP_PROC_PLATE_ALIGN:		
			if	   ((int)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val1)== 1)	setStep(STEP_MAIN_STAGE_ALIGN_PROCESS_START); //Recipe_data[cnt].dOP_Val1== 1)("Stage Align"));
			else if((int)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val1)== 2)	setStep(STEP_MAIN_STAGE_ALIGN_PROCESS_START);
			else if((int)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val1)== 3)	setStep(STEP_MAIN_STAGE_ALIGN_PROCESS_START);
			else																	setStep(STEP_MAIN_ROLL_ALIGN_PROCESS_CHECK); //Recipe_data[cnt].dOP_Val1== 0)(_T("Roll-Align"));
			break;
		case _OP_PROC_VACUUM:		
			if((int)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val1)== 0)	pSystem.GetIO()->Vac_Off(_STAGE_1);
			else																pSystem.GetIO()->Vac_On(_STAGE_1);
			if((int)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val2)== 0)	pSystem.GetIO()->Vac_Off(_STAGE_2);
			else																pSystem.GetIO()->Vac_On(_STAGE_2);
			if((int)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val3)== 0)	pSystem.GetIO()->Vac_Off(_STAGE_3);
			else																pSystem.GetIO()->Vac_On(_STAGE_3);
			setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
			break;
		case _OP_PROC_DELAY:		
			Sleep((DWORD)(ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val1));
			setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
			break;
		case _OP_PROC_MESSAGE:		
			strMessage.Format(L"[Info.] %s", ParamFile()->Recipe_data[RecipeProc_Step].chrOP_Val);
			MyMessageBox(strMessage.GetBuffer(0), L"[RECIPE] Message", M_ICONINFORMATION);
			setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
			break;
		default:
			setStep(STEP_MAIN_END_READY_POS_START1);
		}
	}
	dwCheckTimeStart= GetTickCount();
break;
//----------------------------------------------------------------------------------
case STEP_MAIN_STEP_RECIPE_Z_READY_POS_MOVE_PROC_START:
	if( isAll_Done() ) 
	{
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  RECIPE_Z_READY_POS_MOVE ----------")); 
		ToPositionZ1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		ToPositionZ2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(FALSE)) 
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_STEP_RECIPE_Z_READY_POS_MOVE_PROC_END);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STEP_RECIPE_Z_READY_POS_MOVE_PROC_END:
	if( isAll_Done() ) 
	{
		setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_START:
	if( isAll_Done() ) 
	{
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  RECIPE_ALL_READY_POS_MOVE ----------")); 
		ToPositionZ1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		ToPositionZ2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(FALSE)) 
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_Z1_END);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_Z1_END:
	if( isAll_Done() ) 
	{
		strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동 완료!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);

		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY);
		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition,	FALSE)) 
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y1, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y2, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_ALL_END);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_ALL_END:
	if( isAll_Done() ) 
	{
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  RECIPE_ALL_READY_POS_MOVE -> END ----------")); 
		setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;

//-----------------------------------------------
// Roll-ALIGN
//-----------------------------------------------
case STEP_MAIN_ROLL_ALIGN_PROCESS_CHECK:
	if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC])
	{
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[ROLL] ===   ALIGN   ===")); 
		setStep(STEP_MAIN_ROLL_ALIGN_PROCESS_START);
		break;
	}
	else
	{
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ ROLL - Process Skip..... ]")); 
		dwCheckTimeStart_PROC= GetTickCount(); 
		setStep(STEP_MAIN_STAGE1_ALIGN_PROCESS_CHECK);
	}
	break;
case STEP_MAIN_ROLL_ALIGN_PROCESS_START:
	if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]!= ALIGN_NONE_MODE)
	{
		if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{
			if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE) 
			{
				MyMessageBox(_T("자동모드는 쿼츠 위에서 Roll-Align동작을 지원 안합니다."), _T("Align-Mode"), M_ICONINFORMATION);
				strText.Format(_T("자동모드는 쿼츠 위에서 Roll-Align동작을 지원 안합니다."));
				pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
			else
			{
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[ROLL] ===   Align - Start   ===")); 
				pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_2X);	
				pSystem.GetVisionComponent()->InspGrabContinue();
				pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_20X);		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
				pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, eROLL);
				g_AutoRunAlignFlag= TRUE;
				if(pFrm->GetWindowHandler_AlignView()->RollAlignProc(TRUE)== FALSE)
				{
					g_AutoRunAlignFlag= FALSE;
					pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
					pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);

					Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Roll [ALIGN_AUTO_MODE]"));
					ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);
					pSystem.GetVisionComponent()->InspGrabContinue();
					pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
					strText.Format(_T("[RUN] [ ALIGN-FAIL ] Roll [ALIGN_AUTO_MODE]!"));
					pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	
				}
				g_AutoRunAlignFlag= FALSE;
				pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
				setStep(STEP_MAIN_ROLL_ALIGN_PROCESS_END);
			}
		}
		else
		{
			if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE )
			{
				if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC] &&
					ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && 
				ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
				ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
				{
					CdlgManuAlign dlg;
					if(	ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
						ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
						dlg.m_StageNo= MANUAL_QT_ROLL_ALIGN_PROC;
					else	
					{
						MyMessageBox(_T("[ Roll 수동 Align-FAIL ] [ALIGN_MODE ???]"), _T("Align-Mode"), M_ICONINFORMATION);
						strText.Format(_T("[ Roll 수동 Align-FAIL ] [ALIGN_MODE ???]"));
						pSystem.AutoRunErrorProc(strText, m_eAutoStep);
						return;
					}

					dlg.CameraStageUsedNo= eBTM_CAM; dlg.iStamInstRunFlag= 0;
					pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, eROLL);

					if(dlg.DoModal()== IDCANCEL)
					{
						MyMessageBox(_T("[ Roll 수동 Align-FAIL ] [ALIGN_MODE]"), _T("Align-Mode"), M_ICONINFORMATION);
						strText.Format(_T("[ Roll 수동 Align-FAIL ] [ALIGN_MODE]"));
						pSystem.AutoRunErrorProc(strText, m_eAutoStep);
					}
					else
					{
						Stage_Proc_No= STAGE_3_ALIGN_PROC; Stage_No= _STAGE_3;		Stage_Pos_No= POSITION_STAGE3_ALIGN;
						setStep(STEP_MAIN_STAGE_PROCESS_CHECK);
						setStep(STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_START);
						
					}
				}
				else
				{
					MyMessageBox(_T("자동모드는 쿼츠 위에서 Roll-Align동작을 지원 안합니다."), _T("Align-Mode"), M_ICONINFORMATION);
					strText.Format(_T("자동모드는 쿼츠 위에서 Roll-Align동작을 지원 안합니다."));
					pSystem.AutoRunErrorProc(strText, m_eAutoStep);
	// 				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[ROLL] ===   수동 Align - Start   ===")); 
	// 				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ Roll - Align Skip..... ]")); 
	//				setStep(STEP_MAIN_STAGE1_ALIGN_PROCESS_CHECK);
				}
			}
			else
			{
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[ROLL] ===   수동 Align - Start   ===")); 
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ Roll - Align Skip..... ]")); 
//				setStep(STEP_MAIN_STAGE1_ALIGN_PROCESS_CHECK);
				setStep(STEP_MAIN_ROLL_ALIGN_PROCESS_END);
			}
		}
	}
	else
	{
		ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;

		pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
		pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
		
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ Roll - Align Skip..... ]")); 
		setStep(STEP_MAIN_STAGE1_ALIGN_PROCESS_CHECK);
	}
	break;
case STEP_MAIN_ROLL_ALIGN_PROCESS_END:
	dwCheckTimeStart_PROC= GetTickCount();
	setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
	break;

	//-----------------------------------------------
case STEP_MAIN_STAGE1_ALIGN_PROCESS_CHECK:
	setStep(STEP_MAIN_STAGE1_ALIGN_PROCESS_START);
	break;
case STEP_MAIN_STAGE1_ALIGN_PROCESS_START:
	setStep(STEP_MAIN_STAGE2_ALIGN_PROCESS_CHECK);
	break;
case STEP_MAIN_STAGE1_ALIGN_PROCESS_END:
	dwCheckTimeStart_PROC= GetTickCount();
	setStep(STEP_MAIN_STAGE2_ALIGN_PROCESS_CHECK);
	break;
	//-----------------------------------------------
case STEP_MAIN_STAGE2_ALIGN_PROCESS_CHECK:
	setStep(STEP_MAIN_STAGE3_ALIGN_PROCESS_CHECK);
	break;
case STEP_MAIN_STAGE2_ALIGN_PROCESS_START:
	setStep(STEP_MAIN_STAGE3_ALIGN_PROCESS_CHECK);
	break;
case STEP_MAIN_STAGE2_ALIGN_PROCESS_END:
	dwCheckTimeStart_PROC= GetTickCount();
	setStep(STEP_MAIN_STAGE3_ALIGN_PROCESS_CHECK);
	break;
	//-----------------------------------------------
case STEP_MAIN_STAGE3_ALIGN_PROCESS_CHECK:
	setStep(STEP_MAIN_STAGE_ALIGN_PROCESS_START);
	break;
case STEP_MAIN_STAGE3_ALIGN_PROCESS_START:
	setStep(STEP_MAIN_STAGE_ALIGN_PROCESS_START);
	break;
case STEP_MAIN_STAGE3_ALIGN_PROCESS_END:
	dwCheckTimeStart_PROC= GetTickCount();
	setStep(STEP_MAIN_STAGE_ALIGN_PROCESS_START);
	break;
	//------------------------------------------------------------------------------------------------
case STEP_MAIN_STAGE_ALIGN_PROCESS_START:
	if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC])
	{		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[STAGE #1] ===   ALIGN   ===")); 	}
	else
	{		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ STAGE #1 - Process Skip..... ]")); 	}
	dwCheckTimeStart_PROC= GetTickCount();
	if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC])
	{		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[STAGE #2] ===   ALIGN   ===")); 	}
	else
	{		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ STAGE #2 - Process Skip..... ]")); 	}
	dwCheckTimeStart_PROC= GetTickCount();
	if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC])
	{		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[STAGE #3] ===   ALIGN   ===")); 	}
	else
	{		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("---   [ STAGE #3 - Process Skip..... ]")); 	}
	dwCheckTimeStart_PROC= GetTickCount();

	g_AutoRunAlignFlag= TRUE;
	if(pFrm->GetWindowHandler_AlignView()->Align_Proc(false)== FALSE)
	{
		dwCheckTimeStart_PROC= GetTickCount();
		g_AutoRunAlignFlag= FALSE;
		pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
		pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);

		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] STAGE [ALIGN_AUTO_MODE]"));
		pSystem.GetVisionComponent()->InspGrabContinue();
		pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
		strText.Format(_T("[RUN] [ ALIGN-FAIL ] STAGE [STAGE ALIGN_AUTO_MODE]!"));
		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	
	}
	dwCheckTimeStart_PROC= GetTickCount();
	g_AutoRunAlignFlag= FALSE;
	pFrm->GetWindowHandler_DebugView()->ListPrint(_T("[STAGE] ===   Align - End   ===")); 
	setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);

	break;
//------------------------------------------------------------------------------------------------
//=====================================================================================
//STAGE Sync. 동작 시작 -------------------------
//-----------------------------------------------
case STEP_MAIN_STAGE_PROCESS_CHECK:
	if( isAll_Done() ) 
	{
		if(ParamFile()->Model_data.iRunEnable[Stage_Proc_No])
		{
			strText.Format(_T("----------  STAGE #%d  START ----------"), Stage_No+1);			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			ToPositionZ1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPositionZ2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_START);
		}
		else
		{
			strText.Format(_T("----------  STAGE #%d  SKIP ----------"), Stage_No+1); 	pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
		}
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_START:
	if( isAll_Done() ) 
	{
		strText.Format(_T("----------  STAGE #%d  위치로 이동 ----------"), Stage_No+1); pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		if(ParamFile()->Model_data.iAlignMode[Stage_Proc_No]== ALIGN_NONE_MODE)
		{
			strText.Format(_T("----------  STAGE #%d  Align-SKIP ----------"), Stage_No+1); 	pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			if(ParamFile()->System_data.iRollTeachingDataUsed)
			{
				strText.Format(_T("----------  STAGE #%d  Teaching-Data기준으로 ----------"), Stage_No+1); 	pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
				ToPositionX=	pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, Stage_Pos_No);	
				strText.Format(_T("[RUN] Get-Pos : X=%.3f, Y=%.3f"), ToPositionX, ToPositionResY);	Log()->SaveLogFile(_SYSTEM_LOG, strText);
				ToPositionY1=	pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, Stage_Pos_No);	
				ToPositionY2=	pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, Stage_Pos_No);	

				ToPositionX= ToPositionX + ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val3;
				ToPositionY1= ToPositionY1 + ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val4;	ToPositionY2= ToPositionY2 + ParamFile()->Recipe_data[RecipeProc_Step].dOP_Val4;

				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPositionX,	FALSE)) 
				{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y1, ToPositionY1,	FALSE)) 
				{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y2, ToPositionY2,	FALSE)) 
				{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			}
			else
			{
				strText.Format(_T("----------  STAGE #%d  카메라-View 기준으로 ----------"), Stage_No+1); 	pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
				if(!pFrm->GetWindowHandler_AlignView()->NonAling_PP_Pos_Move(Stage_Proc_No, false))
				{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			}
		}
		else
		{
			strText.Format(_T("----------  STAGE #%d  Align-Data 기준 ----------"), Stage_No+1); 	pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			if(!pFrm->GetWindowHandler_AlignView()->PP_Pos_Move(Stage_Proc_No, false))
			{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
		}

		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, Stage_Pos_No);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition,	FALSE)) 
		{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition,	FALSE))
		{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition,	FALSE)) 
		{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition,	FALSE)) 
		{	strText.Format(_T("[RUN] %s -> 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }

		dwCheckTimeStart_PROC= GetTickCount();
		setStep(STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_START);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_START:
	if( isAll_Done() ) 
	{
		setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START);
		if(g_Wait_Flag)
		{
			ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_PAUSE;
			pFrm->GetWindowHandler_MainView()->PauseProcess();
		}
/*
		if(ParamFile()->Model_data.iAlignMode[Stage_Proc_No]== ALIGN_NONE_MODE)
		{
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START);
		}
		else
		{
//				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  STAGE #%d Y,T 보정 위치로 이동 ----------"), Stage_No+1); 
// 				pFrm->GetWindowHandler_AlignView()->Move_AlignEndPos(Stage_Proc_No, &ToPositionX, &ToPositionResY);
// 				if(!pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, ToPositionResY, _SPD_MODE, FALSE)) 
// 				{	strText.Format(_T("[RUN] %s -> Y-INC. 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
// 				dwCheckTimeStart_PROC= GetTickCount();
// 				setStep(STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_END);
 		}
*/
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_END:
	if( isAll_Done() ) 
	{
		setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START);
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;

case STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START:
	ToPositionZ1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, Stage_Pos_No);
	ToPositionZ2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, Stage_Pos_No);
	strText.Format(_T("[RUN] %s:%s ->모터 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);

	if(ToPositionZ1-ParamFile()->Model_data.dZ_SlowDownLen <= 0.0 || ToPositionZ2-ParamFile()->Model_data.dZ_SlowDownLen <= 0.0)	
		{	setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START_SLOW);	return; }
	else
		{	ToPositionZ1= ToPositionZ1 - ParamFile()->Model_data.dZ_SlowDownLen; ToPositionZ2= ToPositionZ2 - ParamFile()->Model_data.dZ_SlowDownLen; }

	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2, FALSE)) 
	{	strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
	strText.Format(_T(" => [STAGE #%d] Z축 Pisition-Mode"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
	dwCheckTimeStart_PROC= GetTickCount();

	if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC] &&
		ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && 
		ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
		ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
	{
	}
	else
	{
		currPosX= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		tarPosX= currPosX-ParamFile()->Model_data.dRun_Length[Stage_Proc_No];
// 		m_X_SAFE_FREEZONE_L= ParamFile()->System_data.X_SAFE_FREEZONE_L;
// 		m_X_SAFE_FREEZONE_H= ParamFile()->System_data.X_SAFE_FREEZONE_H;
// 		m_X_SAFE_ZONE_L1= ParamFile()->System_data.X_SAFE_ZONE_L1;
// 		m_X_SAFE_ZONE_L2= ParamFile()->System_data.X_SAFE_ZONE_L2;

		if( currPosX >= ParamFile()->System_data.X_SAFE_ZONE_L1 &&
			currPosX <= ParamFile()->System_data.X_SAFE_ZONE_L2 &&
			tarPosX >= ParamFile()->System_data.X_SAFE_ZONE_L1 &&
			tarPosX <= ParamFile()->System_data.X_SAFE_ZONE_L2 )
		{
			;
		}
		else
		{
			ToPosition1	= ParamFile()->Motion_data.dRollRun_BZ_Pos;
			if(ToPosition1 <= 0.0) pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE);
			Sleep(1000);
		}
	}

	
	setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END);
	// 		}
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END:
		if( isAll_Done()) 
		{
			pSystem.GetMotionComponent()->Z_ControlDisable();
			strText.Format(_T(" => [STAGE #%d] Z-Fast_Move. End"), Stage_No+1);		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START_SLOW);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (((pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, Stage_Pos_No)-ParamFile()->Model_data.dZ_SlowDownLen) / ParamFile()->Motion_data.dWork_Speed[_ACS_AXIS_IMP_ROLL_Z1]) + 5.0 ) * 1000.0)
			{
				pSystem.GetMotionComponent()->Z_ControlDisable();
				pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
	break;

case STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START_SLOW:
	ToPositionZ1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, Stage_Pos_No);
	ToPositionZ2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, Stage_Pos_No);
	
	strText.Format(_T("[RUN] %s:%s ->모터 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
	if(ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No]<= 0.0)
	{
		strText.Format(_T(" => [STAGE #%d] Z축 Down-Speed Zero!!!"), Stage_No+1); 	pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No]= 1.0;
	}
 	if(ParamFile()->Model_data.iZ_Control_Mode[Stage_Proc_No]== _FORCE_MODE)
 	{
		strText.Format(_T(" => [STAGE #%d] Sync. Start"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		strText.Format(_T("   스탬프 Down -> Up = S-Spd : %.3f[%.1f], 편측 Kg : %.3f/%.3f, Delay : %dms"),
			ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No],  		ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No],
			ParamFile()->Model_data.dRun_Force[Stage_Proc_No],ParamFile()->Model_data.dRun_Force2[Stage_Proc_No],				ParamFile()->Model_data.iRun_Delay[Stage_Proc_No]	);
		Log()->SaveLogFile(_SYSTEM_LOG, strText);

		dwCheckTimeStart_PROC= GetTickCount();
		if(pSystem.GetMotionComponent()->Stamp_Start(Stage_Proc_No)== FALSE)
		{	strText.Format(_T("[RUN] Sync. Run 에러 발생!") );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

		strText.Format(_T(" => [Stage #3] Z-Control 모드 -> Start"), Stage_No+1); pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END_SLOW);
 	}
 	else
 	{
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual_Speed(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No], FALSE)) 
		{	strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
		strText.Format(_T(" => [STAGE #%d] Z축 Pisition-Mode"), Stage_No); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
	}
	dwCheckTimeStart_PROC= GetTickCount();
	setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END_SLOW);
// 		}
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END_SLOW:
	if(ParamFile()->Model_data.iZ_Control_Mode[Stage_Proc_No]== _FORCE_MODE)
	{
		if( isAll_Done() && pSystem.GetMotionComponent()->isStampc_End()) 
		{
			if(ParamFile()->Model_data.iForceControlEnableFlag[Stage_Proc_No]!= 1)	
				pSystem.GetMotionComponent()->Z_ControlDisable();
			strText.Format(_T(" => [STAGE #%d] Z-Slow Move. End"), Stage_No+1); 			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DELAY_END);
		}
		else
		{
			double movetime= (pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, Stage_Pos_No) - pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1)) / ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No];
			if(GetTickCount()-dwCheckTimeStart_PROC > (movetime * 1000) + 60000)
			{
				pSystem.GetMotionComponent()->Z_ControlDisable();
				pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
	}
	else
	{
		if( isAll_Done()) 
		{
			pSystem.GetMotionComponent()->Z_ControlDisable();
			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
			strText.Format(_T(" => [STAGE #%d] Z-Slow Move. End"), Stage_No+1); 			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_POS_DELAY_END);
		}
		else
		{
			double movetime= (ParamFile()->Model_data.dZ_SlowDownLen) / ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No];
			if(GetTickCount()-dwCheckTimeStart_PROC > (movetime * 1000) + 10000)
			{
				pSystem.GetMotionComponent()->Z_ControlDisable();
				pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
	}
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_POS_DELAY_END:
//		Sleep(ParamFile()->Model_data.iRun_Delay[Stage_Proc_No]);
	if(ParamFile()->Model_data.iForceControlEnableFlag[Stage_Proc_No]!= 1)	
	{
		if(ParamFile()->Model_data.iDownEnd_WaitFlag)
		{
			ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_PAUSE;
			pFrm->GetWindowHandler_MainView()->PauseProcess();
			MyMessageBox(_T("자동 동작 대기 모드 전환 상태입니다."), _T("Wait!"), M_ICONINFORMATION);
		}
	}
	setStep(STEP_MAIN_STAGE_PICKING_PROC_RUN_START);
	break;
case STEP_MAIN_STAGE_PICKING_PROC_RUN_START:
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
	{
//		pFrm->GetWindowHandler_Disp_View()->GetWindowHandler_GraphDisplayView()->StartGraph(Stage_Proc_No);
		strText.Format(_T(" => [STAGE #%d] Sync. Start"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		strText.Format(_T("   Speed: %.3f, Len: %.3f, Diam: %.5f, 편측 Kg : %.3f/%.3f"),
			ParamFile()->Model_data.dRun_Speed[Stage_Proc_No],		ParamFile()->Model_data.dRun_Length[Stage_Proc_No],
			ParamFile()->System_data.dRoll_Diameter[Stage_Proc_No] + ParamFile()->System_data.dRoll_Diameter_AddVal[Stage_Proc_No], ParamFile()->Model_data.dRun_Force[Stage_Proc_No], ParamFile()->Model_data.dRun_Force2[Stage_Proc_No]);
		Log()->SaveLogFile(_SYSTEM_LOG, strText);

		if(ParamFile()->Model_data.iDownEnd_WaitFlag== 0)
		{
			if(ParamFile()->Model_data.iRun_Delay[Stage_Proc_No]> 0) Sleep(ParamFile()->Model_data.iRun_Delay[Stage_Proc_No]);
		}
		dwCheckTimeStart_PROC= GetTickCount();
		if(pSystem.GetMotionComponent()->X_R_LinearMoveStart(Stage_Proc_No)== FALSE)
		{	pSystem.GetMotionComponent()->Z_ControlDisable();	strText.Format(_T("[RUN] Sync. Run 에러 발생!") );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
		setStep(STEP_MAIN_STAGE_PICKING_PROC_RUN_END);
	}
	else
	{
		if(ParamFile()->Model_data.iZ_Control_Mode[Stage_Proc_No]== _FORCE_MODE)
		{
			strText.Format(_T(" => [STAGE #%d] Sync. Start"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			strText.Format(_T("   스탬프 Down -> Up = S-Spd : %.3f[%.1f], 편측 Kg : %.3f/%.3f, Delay : %dms"),
				ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No],  		ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No],
				ParamFile()->Model_data.dRun_Force[Stage_Proc_No],ParamFile()->Model_data.dRun_Force2[Stage_Proc_No],				ParamFile()->Model_data.iRun_Delay[Stage_Proc_No]	);
			Log()->SaveLogFile(_SYSTEM_LOG, strText);

			dwCheckTimeStart_PROC= GetTickCount();
			if(pSystem.GetMotionComponent()->Stamp_Start(Stage_Proc_No)== FALSE)
			{	strText.Format(_T("[RUN] Sync. Run 에러 발생!") );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
			strText.Format(_T(" => [STAGE #%d] 평 스탬프 모드 -> Sync. Start"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			setStep(STEP_MAIN_STAGE_PICKING_PROC_RUN_END);
		}
		else
		{
			pSystem.GetMotionComponent()->Grapg_Only_Start(Stage_Proc_No);
			Sleep(ParamFile()->Model_data.iRun_Delay[Stage_Proc_No]);
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_WAIT);
		}
	}
	break;
case STEP_MAIN_STAGE_PICKING_PROC_RUN_END:
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
	{
		if( isAll_Done() && pSystem.GetMotionComponent()->isSync_End()) 
		{
			pSystem.GetMotionComponent()->LinearMoveStop();
			pSystem.GetMotionComponent()->Z_ControlDisable();
			strText.Format(_T(" => [STAGE #%d] Sync. End"), Stage_No+1); 			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_WAIT);
		}
		else
		{
			dchkTimeSum= (ParamFile()->Model_data.dRun_Length[Stage_Proc_No] / ParamFile()->Model_data.dRun_Speed[Stage_Proc_No]) * 1000.0;
			dwchkTime= (DWORD)(dchkTimeSum) + ParamFile()->System_data._AXIS_MOVE_TIME_OUT;
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)(dwchkTime))
			{
				pSystem.GetMotionComponent()->Z_ControlDisable();
				pSystem.GetMotionComponent()->LinearMoveStop();
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
	}
	else
	{
		if( isAll_Done() && pSystem.GetMotionComponent()->isStampc_End()) 
		{
			pSystem.GetMotionComponent()->LinearMoveStop();
			pSystem.GetMotionComponent()->Z_ControlDisable();
			strText.Format(_T(" => [STAGE #%d] 평스템프 Sync. End"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_WAIT);
		}
		else
		{
			dchkTimeSum= (5.0 / ParamFile()->Model_data.dRun_DownSpeed[Stage_Proc_No]) * 1000.0;
			dwchkTime= (DWORD)(dchkTimeSum) + ParamFile()->System_data._AXIS_MOVE_TIME_OUT;
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)(dwchkTime))
			{
				pSystem.GetMotionComponent()->Z_ControlDisable();
			pSystem.GetMotionComponent()->LinearMoveStop();
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
	}
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_WAIT:
	pSystem.GetMotionComponent()->Stamp_Stop();
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]!= RUN_ROLL_MODE)
	{
		pSystem.GetMotionComponent()->Stamp_Stop();
		if(ParamFile()->Model_data.iWaitFlag[Stage_Proc_No])
		{
			ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_PAUSE;
			pFrm->GetWindowHandler_MainView()->PauseProcess();
			pFrm->GetWindowHandler_MainView()->EndGraph(Stage_No);
		}
	}
	else
		pSystem.GetMotionComponent()->LinearMoveStop();
	setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_SLOW);
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_SLOW:
// 		if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]!= RUN_ROLL_MODE)
// 		{
		if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]!= RUN_ROLL_MODE)
			pSystem.GetMotionComponent()->Stamp_Stop();
		strText.Format(_T(" => [STAGE #%d] Z축 1-Step - UP"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		if(ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No]<= 0.0)
		{
			strText.Format(_T(" => [STAGE #%d] Z축 Up-Speed Zero!!!"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
			ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No]= 1.0;
		}
		ToPositionZ1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, Stage_Pos_No);
		ToPositionZ2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, Stage_Pos_No);
		ToPositionZ1= ToPositionZ1 - ParamFile()->Model_data.dZ_SlowDownLen; ToPositionZ2= ToPositionZ2 - ParamFile()->Model_data.dZ_SlowDownLen;

		strText.Format(_T("[RUN] %s:%s ->모터 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);

//		if(ParamFile()->Model_data.iZ_Control_Mode[Stage_Proc_No]== _FORCE_MODE)
//		{
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual_Speed(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No], FALSE)) 
			{	
				pFrm->GetWindowHandler_MainView()->EndGraph(Stage_No);
				strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;
			}
//		}
//		ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
//		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition,	FALSE))
//		{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}
		Sleep(500);
		pFrm->GetWindowHandler_MainView()->EndGraph(Stage_No);
		dwCheckTimeStart_PROC= GetTickCount();
//		}
	setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_SLOW_END);
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_SLOW_END:
	if( isAll_Done() ) 
	{
		strText.Format(_T("-------   [STAGE #%d] 1-Step UP End   -----------"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START);
	}
	else
	{
		dchkTimeSum= (5.0 / ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No]) * 1000.0;
		dchkTimeSum= dchkTimeSum + 10000;
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)(dchkTimeSum))
		{
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START:
	ToPosition1	= 0.0;
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE);
	strText.Format(_T(" => [STAGE #%d] Z축 - UP"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
	if(ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No]<= 0.0)
	{
		strText.Format(_T(" => [STAGE #%d] Z축 Up-Speed Zero!!!"), Stage_No+1); 		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 
		ParamFile()->Model_data.dRun_UpSpeed[Stage_Proc_No]= 1.0;
	}
	ToPositionZ1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPositionZ2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	strText.Format(_T("[RUN] %s:%s ->모터 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
	{	
		pFrm->GetWindowHandler_MainView()->EndGraph(Stage_No);
		strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;
	}
	Sleep(300);
	dwCheckTimeStart_PROC= GetTickCount();
	setStep(STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_END);
	break;
case STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_END:
	if( isAll_Done() ) 
	{
		pFrm->GetWindowHandler_MainView()->EndGraph(Stage_No);
		strText.Format(_T("-------   [STAGE #%d]  End   -----------"), Stage_No+1); 
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0)); 

		pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
		dwCheckTimeStart= GetTickCount();
		setStep(STEP_MAIN_STEP_RECIPE_COMMAND_END);
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && 
			ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC] &&
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
			ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			setStep(STEP_MAIN_END_READY_POS_START1);
		}
	}
	else
	{
		if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
		{
			pFrm->GetWindowHandler_MainView()->EndGraph(Stage_No);
			isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
		}
	}
	break;
	// STAGE #%d PROC_END & Z_UP_END ==================
	//=====================================================================================

//----------------------------------------------------------------------------------
case STEP_MAIN_STEP_RECIPE_COMMAND_END://							= 6900,
	if(  isAll_Done() ) 
	{
		RecipeProc_Step++;
		setStep(STEP_MAIN_STEP_RECIPE_END_CHECK);
	}
	else
	{
		if(GetTickCount()- dwCheckTimeStart > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT+5000 )
		{
			strText.Format(L"[MODE] Motor 에러! : Time-Out [dPosition_Wait][%d/%d]",	ParamFile()->System_data._AXIS_MOVE_TIME_OUT, GetTickCount()- dwCheckTimeStart);
			pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;
		}
	}
	break;
	//----------------------------------------------------------------------------------
case STEP_MAIN_STEP_RECIPE_END_CHECK://								= 6990,
	if(ParamFile()->Recipe_data[RecipeProc_Step].iCommand< 0 || ParamFile()->Recipe_data[RecipeProc_Step].iCommand== _OP_PROC_END)
	{
		setStep(STEP_MAIN_END_READY_POS_START1);
	}
	else
		setStep(STEP_MAIN_STEP_RECIPE_START);
	break;
//==================================================================
// Recipe구동 동작 종료
//==================================================================

//=======================================================================================================================
	case STEP_MAIN_END_READY_POS_START1:
		if( isAll_Done() ) 
		{
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("----------  END-WAIT ----------")); 
			ToPositionZ1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPositionZ2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2)); 	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPositionZ1, ToPositionZ2,	FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return; }
			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_END_READY_POS_START2);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_END_READY_POS_START2:
		if( isAll_Done() ) 
		{
			strText.Format(_T("[RUN] %s:%s ->모터 대기 위치 이동 완료!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);

			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY);
			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition,	FALSE)) 
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y1, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			strText.Format(_T("[RUN] %s ->모터 대기 위치 이동"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2));	Log()->SaveLogFile(_SYSTEM_LOG, strText);
			ToPosition	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_ALIGN_Y2, ToPosition,	FALSE))
			{	strText.Format(_T("[RUN] %s ->대기 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2) );		pSystem.AutoRunErrorProc(strText, m_eAutoStep); return;	}

			dwCheckTimeStart_PROC= GetTickCount();
			setStep(STEP_MAIN_END_READY_POS_END);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;
	case STEP_MAIN_END_READY_POS_END:
		if( isAll_Done() ) 
		{
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("===========================  END ===========================")); 
			setStep(STEP_MAIN_END);
		}
		else
		{
			if(GetTickCount()-dwCheckTimeStart_PROC > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)
			{
				isDoneError();	pSystem.AutoRunErrorProc(strText, m_eAutoStep);
			}
		}
		break;

	case STEP_MAIN_END:
		pSystem.AutoRunEndProc();
		break;
	case STEP_MAIN_ERROR:
		setStep(STEP_MAIN_DUMMY);
		break;
	default:
		break;
	}
}

void CProcessThread_Main::AlignDataClear()
{

}
void CProcessThread_Main::isDoneError()
{
	CString strText;
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_STAGE_X))		{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL))		{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL) );	}
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_UPPER_CAM_Y))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_UPPER_CAM_Z))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_X))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_Y))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_ALIGN_Y1))		{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1) ); }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_ALIGN_Y2))		{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z2))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2) );	 }
	if( !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_Z))	{ strText.Format(_T("[RUN] %s -> 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z) );	 }
}

BOOL CProcessThread_Main::isAll_Done()
{
	if( pSystem.GetMotionComponent()->isDone(_ACS_AXIS_STAGE_X) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_Z) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_UPPER_CAM_Y) &&	pSystem.GetMotionComponent()->isDone(_ACS_AXIS_UPPER_CAM_Z) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_X) &&	pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_Y) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_ALIGN_Y1) &&		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_ALIGN_Y2) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1) &&
		pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_Z)) 
	{
		return TRUE;
	}
	return FALSE;
}
