// IO.cpp: implementation of the CIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Transfer_Control.h"
#include "IO.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSystemClass pSystem;
CParameterFile* CIO::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CIO::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIO::CIO()
{
	On_Off_Check_Flag= false;
}

CIO::~CIO()
{

}

//===================================================================================
BOOL CIO::isEMO_Check()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
// 	if(pSystem.GetMotionComponent()->InputBit(_INPUT_EMO_SWITCH))
// 		return TRUE;
// 	else					
		return FALSE;
}
//===================================================================================
BOOL CIO::isPower_Check()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
#ifdef _INPUT_MAIN_POWER_ON	
	if(pSystem.GetMotionComponent()->InputBit(_INPUT_MAIN_POWER_ON))
		return TRUE;
	else					
		return FALSE;
#endif
	return TRUE;
}
//===================================================================================
BOOL CIO::isMotorPower_Check()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	if(pSystem.GetMotionComponent()->InputBit(_INPUT_MotorPower))
		return TRUE;
	else					
		return FALSE;
}
//===================================================================================
BOOL CIO::isAir_Check()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
 	if(pSystem.GetMotionComponent()->InputBit(_INPUT_Air1_On))
 		return TRUE;
 	else					
		return FALSE;
}
//==================================================================================
BOOL CIO::MotorPower_On()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_MotorPower, _ON);
	return TRUE;
}
//==================================================================================
BOOL CIO::MotorPower_Off()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_MotorPower, _OFF);
	return TRUE;
}

//==================================================================================
BOOL CIO::InputValveSel()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	return TRUE;
}
//==================================================================================
BOOL CIO::OutputValveSel()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	return TRUE;
}
//==================================================================================
BOOL CIO::VentValveOpen()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	return TRUE;
}
//==================================================================================
BOOL CIO::VentValveClose()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	return TRUE;
}
//==================================================================================
int CIO::GetValvePos()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}

	return 0;
}
//===================================================================================
BOOL CIO::Vac_On(int ch)
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	return TRUE;
}
//===================================================================================
BOOL CIO::Vac_Off(int ch)
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	return TRUE;
}

//===================================================================================
BOOL CIO::Vac_On()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	return TRUE;
}
//===================================================================================
BOOL CIO::Vac_Off()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	return TRUE;
}

void CIO::Vac_Switch_Check()
{

}
//===================================================================================
BOOL CIO::isAIR_ON_Check()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}

#ifdef _INPUT_AIR_ON
	if(pSystem.GetMotionComponent()->InputBit(_INPUT_AIR_ON) )
		return TRUE;
	else					
		return FALSE;
#endif
	return TRUE;
}
//===================================================================================
BOOL CIO::LVDT1_Cylinder_UP()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	
	return TRUE;
}
//===================================================================================
BOOL CIO::LVDT1_Cylinder_DOWN()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	
	return TRUE;
}
//===================================================================================
BOOL CIO::LVDT2_Cylinder_UP()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	
	return TRUE;
}
//===================================================================================
BOOL CIO::LVDT2_Cylinder_DOWN()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
	
	return TRUE;
}
//===================================================================================
BOOL CIO::BLK_Roll_ElecCruch_ON()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
/*
	OutputBit(_OUTPUT_ELEC_CLUCH, _ON);
	if(ParamFile()->System_data.iCluchDelayTime > 0) Sleep(ParamFile()->System_data.iCluchDelayTime);
*/
	return TRUE;
}
//===================================================================================
BOOL CIO::BLK_Roll_ElecCruch_OFF()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	
/*
	if(ParamFile()->System_data.iCluchDelayTime > 0) Sleep(ParamFile()->System_data.iCluchDelayTime);
	OutputBit(_OUTPUT_ELEC_CLUCH, _OFF);
	if(ParamFile()->System_data.iCluchDelayTime > 0) Sleep(ParamFile()->System_data.iCluchDelayTime);
	
*/
	return TRUE;
}


//===================================================================================
BOOL CIO::RollWinderEndDone()
//-----------------------------------------------------------------------------------
{ 
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
//	if(isDone(_AXIS_ROLL_WINDER) && (isWinder_StepEnd()== _STEP_SUCCESS || isWinder_StepEnd()== _STEP_NONE))
			return TRUE;

//	return FALSE;
}
//===================================================================================
void CIO::Cleaning_V_Open()
//-----------------------------------------------------------------------------------
{ 
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		return ;
	}
}
//===================================================================================
void CIO::Cleaning_V_Close()
//-----------------------------------------------------------------------------------
{ 
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return ;
	}
}

//===================================================================================
BOOL CIO::isCleaning_V_Open()
//-----------------------------------------------------------------------------------
{ 
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		return FALSE;
	}
	return FALSE;
}

void CIO::SetBuzzer(int flag)
{

}
void CIO::BuzzerStop()
{
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_Buzzer1, _OFF);
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_Buzzer2, _OFF);
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_Buzzer3, _OFF);
}
void CIO::Buzzer_Error()
{
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_Buzzer1, _ON);
}
void CIO::Buzzer_Start()
{
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_Buzzer2, _ON);
}
//===================================================================================
BOOL CIO::LoadCell_Reset()
//-----------------------------------------------------------------------------------
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_LoadCell_Reset, _ON); Sleep(200);	pSystem.GetMotionComponent()->OutputBit(_OUTPUT_LoadCell_Reset, _OFF);
	return TRUE;
}

//===================================================================================
BOOL CIO::isStage1_VacOn()
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	if(pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac1) || pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac2) || pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac3))
		return TRUE;
	return FALSE;
}
//===================================================================================
BOOL CIO::isStage2_VacOn()
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	if(pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac4) || pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac4))
		return TRUE;
	return FALSE;
}
BOOL CIO::isStage3_VacOn()
{
	if(!pSystem.GetMotionComponent()->isMotionInitial()) 
	{
		//		CString strText;	strText.Format("[ERROR] Motion_SYS-Open Error!");	Log()->SaveLogFile(_MOTION_LOG, strText);	AfxMessageBox(strText); 
		return FALSE;
	}
	if(pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac5) || pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Vac5))
		return TRUE;
	return FALSE;
}
