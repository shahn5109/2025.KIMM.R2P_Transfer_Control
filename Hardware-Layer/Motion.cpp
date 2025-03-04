// Motion.cpp: implementation of the CMotion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Transfer_Control.h"
#include "Motion.h"
#include "Mainfrm.h"
#include "SystemInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSystemClass pSystem;
CParameterFile* CMotion::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CMotion::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

CMotion::CMotion()
{
	int i;
	ACS_DeviceOpenOK= false;

// 	for(i= 0; i< _ACS_MAX_AXIS; i++) {
// 		ACS_SYS.Set_Int_Val(_T("HomeFlag"), i, 0);
// 	}
	for(i= 0; i< _MAX_AXIS; i++) {
		g_AxisHomeStat[i]= AxisHomeStat[i]= _MT_NONE;
		CurPosition_Data[i]= 0.0;
	}
	for(i= 0; i< 7; i++) AD_OffsetVal[i]= 0.0;
}

CMotion::~CMotion()
{

}
//===================================================================================
BOOL	CMotion::Open()
//-----------------------------------------------------------------------------------
{
//	bool ret;
	CString strText;
	CString strIP;
	int port;
	
	Motion_Initial= false;
	ACS_DeviceOpenOK= false;

	strIP.Format(_T("%s"), _LAYER_MC4U_IP);	port= _LAYER_MC4U_PORT;

	strIP.Format(_T("%s"), _LAYER_MC4U_IP);	port= _LAYER_MC4U_PORT;
	if(!ACS_SYS.Init(strIP, port))
	{
		ACS_DeviceOpenOK= false;
		strText.Format(_T("[ERROR] ACS_SYS -> Open Error! [IP: %s, port: %d]"), strIP, port);
		Log()->SaveLogFile(_MOTION_LOG, strText);
		AfxMessageBox(strText);
//		return FALSE;
	}
	else 
	{
	//	ACS_SYS.Set_Int_Val("Y_HomeState", 0, 1);
		
		Motion_Initial= TRUE;
		ACS_DeviceOpenOK= true;
		ACS_SYS.BufferStop(_STAGE_Z_CONTROL_BUFFER_NO);
		ACS_SYS.BufferStop(_STAGE_SYNC_BUFFER_NO);
	}
	//--------------------------------------------------------------------------------------------
	return TRUE;
}
//===================================================================================
BOOL	CMotion::Close()
//-----------------------------------------------------------------------------------
{

	Motion_Initial= FALSE;

	if(ACS_DeviceOpenOK)
	{
		ACS_SYS.BufferStop(_STAGE_Z_CONTROL_BUFFER_NO);
		ACS_SYS.BufferStop(_STAGE_SYNC_BUFFER_NO);
	
		ACS_SYS.Close();		Log()->SaveLogFile(_MOTION_LOG, _T("[ACS_SYS] Close!"));
		ACS_DeviceOpenOK= false;
	}
	
	return TRUE;
}
//===================================================================================
BOOL CMotion::isMotionInitial()
//-----------------------------------------------------------------------------------
{
	return Motion_Initial;
}
//===================================================================================
void CMotion::EMG_Proc()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	
	HomeFlag_Clear();
	Z_ControlDisable();
}

//===================================================================================
void CMotion::HomeFlag_Clear()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	
	int i;
	for(i= 0; i< _ACS_MAX_AXIS; i++) 
	{
		ACS_SYS.BufferStop(i);
	}

	for(i= 0; i< _MAX_AXIS; i++) 
	{
		g_AxisHomeStat[i]= AxisHomeStat[i]= _MT_NONE;
	}
/*
	for(i= 0; i< _ACS_MAX_AXIS; i++) 
	{
		ACS_SYS.Set_Int_Val(_T("HomeFlag"), i, 0);
	}
*/
}

//===================================================================================
BOOL CMotion::isMotionReadyCheck()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; } 
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	CString stText;
	int i;

	for(i= 0; i< _MAX_AXIS; i++)
	{
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_5 || i== _ACS_AXIS_6 || i== _ACS_AXIS_7 || i== _ACS_AXIS_ENC_1) continue;
#else
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_9 || i== _ACS_AXIS_14 || i== _ACS_AXIS_15 || i== _ACS_AXIS_ENC_1) continue;
#endif
		if(AxisHomeStat[i] != _MT_END)	
		{
			stText.Format(_T("원점 작업을 먼저 수행하십시요!. [%d-%s]"), i, GetMotionName(i));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
			MyMessageBox(stText.GetBuffer(0), _T("Origin Failed"), M_ICONERROR);
			return FALSE;
		}
		if(!isServoOn(i))
		{
				stText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), i, GetMotionName(i));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
				MyMessageBox(stText.GetBuffer(0), _T("Motion_ReadyCheck"), M_ICONERROR);
			return FALSE;
		}
		if(!isMotion_AlarmCheck(i))
		{
				stText.Format(_T("축 알람 상태!\n[%d-%s 축 ]"), i, GetMotionName(i));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
				MyMessageBox(stText.GetBuffer(0), _T("Motion_ReadyCheck"), M_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}
//===================================================================================
BOOL CMotion::isMotionServoOnCheck()
	//-----------------------------------------------------------------------------------
{
	//	//if(SIMULATION_MODE) { return TRUE; } 
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	CString stText;
	int i;

	for(i= 0; i< _MAX_AXIS; i++)
	{
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_5 || i== _ACS_AXIS_6 || i== _ACS_AXIS_7 || i== _ACS_AXIS_ENC_1) continue;
#else
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_9 || i== _ACS_AXIS_14 || i== _ACS_AXIS_15 || i== _ACS_AXIS_ENC_1) continue;
#endif
		if(!isServoOn(i))
		{
			stText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), i, GetMotionName(i));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
			MyMessageBox(stText.GetBuffer(0), _T("Motion_ReadyCheck"), M_ICONERROR);
			return FALSE;
		}
		if(!isMotion_AlarmCheck(i))
		{
			stText.Format(_T("축 알람 상태!\n[%d-%s 축 ]"), i, GetMotionName(i));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
			MyMessageBox(stText.GetBuffer(0), _T("Motion_ReadyCheck"), M_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}
//===================================================================================
BOOL CMotion::isMotion_ReadyCheck(long nAxis, bool flag)
//-----------------------------------------------------------------------------------
{
	//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	CString stText;
	if(	!isHomeReturnCheck(nAxis))	//isServoOn(int axis)
	{
		if(flag)
		{
			if(AxisHomeStat[nAxis]== _MT_END ) g_AxisHomeStat[nAxis]= AxisHomeStat[nAxis]= _MT_NONE;
			stText.Format(_T("원점 작업을 먼저 수행하십시요!. [%d:%s]"), nAxis, GetMotionName(nAxis));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
			MyMessageBox(stText.GetBuffer(0), _T("Origin Failed"), M_ICONERROR);
		}
		return FALSE;
	}
	if(!isServoOn(nAxis))
	{
		if(flag)
		{
			stText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), nAxis, GetMotionName(nAxis));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
			MyMessageBox(stText.GetBuffer(0), _T("Motion_ReadyCheck"), M_ICONERROR);
		}
		return FALSE;
	}
	if(!isMotion_AlarmCheck(nAxis))
	{
		if(flag)
		{
			stText.Format(_T("축 알람 상태!\n[%d-%s 축 ]"), nAxis, GetMotionName(nAxis));	Log()->SaveLogFile(_MOTION_LOG, stText.GetBuffer(0));
			MyMessageBox(stText.GetBuffer(0), _T("Motion_ReadyCheck"), M_ICONERROR);
		}
		return FALSE;
	}
	return TRUE;
}

//===================================================================================
BOOL CMotion::isMotion_AlarmCheck() //false=알람
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	BOOL flag= FALSE;
	int i;
	for(i= 0; i< _ACS_MAX_AXIS; i++) 
	{	
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_5 || i== _ACS_AXIS_6 || i== _ACS_AXIS_7 || i== _ACS_AXIS_ENC_1) continue;
#else
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_9 || i== _ACS_AXIS_14 || i== _ACS_AXIS_15 || i== _ACS_AXIS_ENC_1) continue;

		bool retSt= ACS_SYS.isAlarm(i);
		if(i>= 4 && i<= 7)
		{
			if(retSt) retSt= false; else retSt= true;
		}
		if(retSt)	
		{
//			CString stra;
//			stra.Format(_T("%d : TRUE\n"), i);
//			TRACE(stra.GetBuffer(0));
			return FALSE;
		}
#endif
//			return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL CMotion::isMotion_AlarmCheck(long axis) //false=알람
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	BOOL flag= FALSE;
//	if(!ACS_SYS.isAlarm(axis))		return FALSE;

#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	if(axis== _ACS_AXIS_3 || axis== _ACS_AXIS_5 || axis== _ACS_AXIS_6 || axis== _ACS_AXIS_7 || axis== _ACS_AXIS_ENC_1) return TRUE;
#else
	if(axis== _ACS_AXIS_3 || axis== _ACS_AXIS_9 || axis== _ACS_AXIS_14 || axis== _ACS_AXIS_15 || axis== _ACS_AXIS_ENC_1) return TRUE;
#endif
	bool retSt= ACS_SYS.isAlarm(axis);
	if(axis>= 4 && axis<= 7)
	{
		if(retSt) retSt= false; else retSt= true;
	}
	if(retSt)	
	{
// 		CString stra;
// 		stra.Format(_T("%d : TRUE\n"), axis);
// 		TRACE(stra.GetBuffer(0));
		return FALSE;
	}

	return TRUE;
}

//===================================================================================
BOOL CMotion::isDone(long axis) //완료 시 TRUE리턴
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	bool ret= false;

	ret= ACS_SYS.isDone(axis);	

	if(isServoOn(axis)== _OFF)	return TRUE;
	if(ret)						return TRUE;
	else						return FALSE;
}
//===================================================================================
BOOL CMotion::isHomeBusy(long axis) //동작중이면 TRUE리턴
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	if(!ACS_DeviceOpenOK)  return FALSE;
	
	int ret= 0;

	ret = ACS_SYS.Get_Int_Val(_T("HomeFlag"), axis);

	//--------------------------------------------------------
	if(ret== 1)
	{
		if(isDone(axis)) return FALSE;
	}
	return TRUE;
}
//===================================================================================
void CMotion::HomeSearchErrorProc() 
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;
	
	MotorAllStop();
	for(int i= 0; i< _ACS_MAX_AXIS; i++) {
		ACS_SYS.BufferStop(i);
	}
}
//===================================================================================
BOOL CMotion::HomeEndProc(long axis) 
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	return TRUE;
}
//===================================================================================
BOOL CMotion::isHomeReturnCheck(long axis, bool flag)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	int ret= 0;
	
	ret = ACS_SYS.Get_Int_Val(_T("HomeFlag"), axis);

	if(ret== 1)
	{
		return TRUE;
	}
	else		
	{
		if(AxisHomeStat[axis]== _MT_END ) g_AxisHomeStat[axis]= AxisHomeStat[axis]= _MT_NONE;
		return FALSE;
	}
}
//===================================================================================
BOOL CMotion::isAllMotorHomeEnd()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_5 || i== _ACS_AXIS_6 || i== _ACS_AXIS_7 || i== _ACS_AXIS_ENC_1) continue;
#else
		if(i== _ACS_AXIS_3 || i== _ACS_AXIS_9 || i== _ACS_AXIS_14 || i== _ACS_AXIS_15 || i== _ACS_AXIS_ENC_1) continue;
#endif
		if(AxisHomeStat[i] != _MT_END)	
		{
			return FALSE;
		}
		if(!isServoOn(i))
		{
			return FALSE;
		}
		if(!isMotion_AlarmCheck(i))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//===================================================================================
BOOL CMotion::MotionHome(long axis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	BOOL ret= FALSE;

	ServoOn(axis);

	if(axis== _ACS_AXIS_LOWER_CAM_X)
	{
		double aaa= ACS_SYS.Set_Real_Val(_T("g_rHome_Offset_Val"), axis, ParamFile()->System_data.dHome_Offset_BX);
		if(aaa> 0)	ret= TRUE;
		else		ret= FALSE;
	}
	ret= ACS_SYS.Set_Int_Val(_T("HomeFlag"), axis, 0);		Sleep(500);		ret= ACS_SYS.BufferRun(axis);		

	Sleep(500);
	if(ret) return TRUE;
	else	return FALSE;
}
//===================================================================================
BOOL CMotion::OutputBit(long no, long status)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

// 	if(no< _COMI_INOUT_START_NO)
// 	{
		ACS_SYS.OutputBit(no, status);
// 	}
// 	else
// 	{
// 		COMI_SYS.OutputBit(no-_COMI_INOUT_START_NO, status);
//	}
	return TRUE;
}
//===================================================================================
BOOL CMotion::isOutputBit(long no)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
// 	if(no< _COMI_INOUT_START_NO)
// 	{
		if(ACS_SYS.isOutputBit(no)) return TRUE;
		else						return FALSE;
// 	}
// 	else
// 	{
// 		if(COMI_SYS.isOutputBit(no-_COMI_INOUT_START_NO))	return TRUE;
// 		else												return FALSE;
// 	}

	return FALSE;
}

//===================================================================================
BOOL CMotion::OutputToggle(long no)
//-----------------------------------------------------------------------------------
{

//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
// 	if(no< _COMI_INOUT_START_NO)
// 	{
		if(isOutputBit(no))			OutputBit(no, _OFF);
		else						OutputBit(no, _ON);
// 	}
// 	else
// 	{
// 		if(COMI_SYS.isOutputBit(no-_COMI_INOUT_START_NO))	COMI_SYS.OutputBit(no-_COMI_INOUT_START_NO, _OFF);
// 		else												COMI_SYS.OutputBit(no-_COMI_INOUT_START_NO, _ON);
// 	}
	return FALSE;
}
//===================================================================================
long CMotion::InputBit(long no)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return 0; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0;
	
// 	if(no< _COMI_INOUT_START_NO)
// 	{
		if(ACS_SYS.InputBit(no))	return 1;
		else						return 0;
// 	}
// 	else
// 	{
// 		if(COMI_SYS.InputBit(no-_COMI_INOUT_START_NO))	return 1;
// 		else											return 0;
// 	}
	return 0;
}
// /*
// //===================================================================================
// long CMotion::ComiInputBitAll()
// //-----------------------------------------------------------------------------------
// {
// 	//	//if(SIMULATION_MODE) { return 0; }
// 	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0;
// 	
// 	return COMI_SYS.InputBitAll();
// }
// */
//===================================================================================
BOOL CMotion::isORG_SensorCheck(long paAxis)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

#ifdef _SIMUL_MODE_ENABLE 
	return TRUE;
#endif
	
	BOOL flag= FALSE;

	switch(paAxis)
	{
	case _ACS_AXIS_UPPER_CAM_Z: if(InputBit(_INPUT_Upper_Vision_Z_Org)) flag= TRUE; break;
	case _ACS_AXIS_UPPER_CAM_Y: if(InputBit(_INPUT_Upper_Vision_Y_Org)) flag= TRUE; break;
	case _ACS_AXIS_LOWER_CAM_X: if(InputBit(_INPUT_Lower_Vision_X_Org)) flag= TRUE; break;
	case _ACS_AXIS_LOWER_CAM_Y: if(InputBit(_INPUT_Lower_Vision_Y_Org)) flag= TRUE; break;
	case _ACS_AXIS_LOWER_CAM_Z: if(InputBit(_INPUT_Lower_Vision_Z_Org)) flag= TRUE; break;
	case _ACS_AXIS_ALIGN_Y1:	if(InputBit(_INPUT_AlignStage_Y1_Org)) flag= TRUE; break;
	case _ACS_AXIS_ALIGN_Y2:	if(InputBit(_INPUT_AlignStage_Y2_Org)) flag= TRUE; break;
	case _ACS_AXIS_IMP_ROLL_Z1: if(InputBit(_INPUT_Impress_Z1_Org)) flag= TRUE; break;
	case _ACS_AXIS_IMP_ROLL_Z2: if(InputBit(_INPUT_Impress_Z1_Org)) flag= TRUE; break;
	}
	return flag;
}
BOOL	CMotion::isELN_SensorCheck(long paAxis)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	BOOL flag= FALSE;
// 	if(paAxis< _COMI_START_AXIS)
// 	{
		if(ACS_SYS.isELN_SensorCheck(paAxis))						return TRUE;
// 	}
// 	else
// 	{
// 		if(COMI_SYS.isELN_SensorCheck(paAxis-_COMI_START_AXIS))		return TRUE;
// 	}
	return FALSE;
}
BOOL	CMotion::isELP_SensorCheck(long paAxis)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	BOOL flag= FALSE;
// 	if(paAxis< _COMI_START_AXIS)
// 	{
		if(ACS_SYS.isELP_SensorCheck(paAxis))						return TRUE;
// 	}
// 	else
// 	{
// 		if(COMI_SYS.isELP_SensorCheck(paAxis-_COMI_START_AXIS))		return TRUE;
// 	}
 	return FALSE;
}
BOOL	CMotion::isALM_SensorCheck(long paAxis)
{
	return isMotion_AlarmCheck(paAxis);
}

//===================================================================================
void CMotion::ResetPos(long axis) 
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	
	//--------------------------------------------------------
	ACS_SYS.SetFPosition(axis, 0.0);
	ACS_SYS.SetRPosition(axis, 0.0);
}
//===================================================================================
double CMotion::Get_Position(long axis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return 0.0; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0.0;
	
	double rrr= 0.0;

	//--------------------------------------------------------
	rrr= ACS_SYS.GetFPosition(axis);
//			rrr= ACS_SYS.GetRPosition(_ACS_AXIS_SLOTDIE_X);
	return rrr;
}
double CMotion::Get_APosition(long axis)
	//-----------------------------------------------------------------------------------
{
	//	//if(SIMULATION_MODE) { return 0.0; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0.0;

	double rrr= 0.0;

	//--------------------------------------------------------
	rrr= ACS_SYS.GetAPosition(axis);
	//			rrr= ACS_SYS.GetRPosition(_ACS_AXIS_SLOTDIE_X);
	return rrr;
}
void CMotion::Position_RealAll()
{
	int i;
	for(i= 0; i< _MAX_AXIS; i++)
		CurPosition_Data[i]= Get_Position(i);
}
//===================================================================================
double CMotion::Get_Velocity(long axis)
//-----------------------------------------------------------------------------------
{
	//	//if(SIMULATION_MODE) { return 0.0; }
	if (!Motion_Initial || PROGRAM_EXIT == TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0.0;

	double rrr = 0.0;

	//--------------------------------------------------------
	rrr = ACS_SYS.GetFVelocity(axis);
	//			rrr= ACS_SYS.GetRPosition(_ACS_AXIS_SLOTDIE_X);
	return rrr;
}
//===================================================================================
bool CMotion::EndDone(long axis, int msTime)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return 0; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0;
	
	bool ret= false;
	DWORD st= GetTickCount();
	
	ret= ACS_SYS.EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
	if(GetTickCount()- st >= (DWORD)(msTime))	return false;
	return ret;
}

//===================================================================================
void CMotion::ServoOn(long axis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;

	ACS_SYS.ServoOn(axis);
}
//===================================================================================
void CMotion::ServoOff(long axis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;
	
	ACS_SYS.ServoOff(axis);
}
//===================================================================================
long CMotion::isServoOn(long axis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return 0; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0;
	if(!ACS_DeviceOpenOK)  return 0;
	
	bool ret= false;

	ret= ACS_SYS.isServoOn(axis);
	
	if(ret) return _ON;
	else	return _OFF;
}
//===================================================================================
void CMotion::ServoAllOn()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;
	
//	int j;
//	for(j= 0; j< _ACS_MAX_AXIS; j++) ACS_SYS.ServoOn(j);
	ACS_SYS.ServoOn(_ACS_AXIS_STAGE_X);		ACS_SYS.ServoOn(_ACS_AXIS_IMP_ROLL);
	ACS_SYS.ServoOn(_ACS_AXIS_UPPER_CAM_Z);	ACS_SYS.ServoOn(_ACS_AXIS_UPPER_CAM_Y);
	ACS_SYS.ServoOn(_ACS_AXIS_LOWER_CAM_X);	ACS_SYS.ServoOn(_ACS_AXIS_LOWER_CAM_Y);	ACS_SYS.ServoOn(_ACS_AXIS_LOWER_CAM_Z);
	ACS_SYS.ServoOn(_ACS_AXIS_ALIGN_Y1);	ACS_SYS.ServoOn(_ACS_AXIS_ALIGN_Y2);
	ACS_SYS.ServoOn(_ACS_AXIS_IMP_ROLL_Z1);	ACS_SYS.ServoOn(_ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CMotion::ServoAllOff()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;
	
//	int j;
	//	for(j= 0; j< _ACS_MAX_AXIS; j++) ACS_SYS.ServoOn(j);
	ACS_SYS.ServoOff(_ACS_AXIS_STAGE_X);		ACS_SYS.ServoOff(_ACS_AXIS_IMP_ROLL);
	ACS_SYS.ServoOff(_ACS_AXIS_UPPER_CAM_Z);	ACS_SYS.ServoOff(_ACS_AXIS_UPPER_CAM_Y);
	ACS_SYS.ServoOff(_ACS_AXIS_LOWER_CAM_X);	ACS_SYS.ServoOff(_ACS_AXIS_LOWER_CAM_Y);	ACS_SYS.ServoOff(_ACS_AXIS_LOWER_CAM_Z);
	ACS_SYS.ServoOff(_ACS_AXIS_ALIGN_Y1);		ACS_SYS.ServoOff(_ACS_AXIS_ALIGN_Y2);
	ACS_SYS.ServoOff(_ACS_AXIS_IMP_ROLL_Z1);	ACS_SYS.ServoOff(_ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
BOOL CMotion::MoveStop(long axis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	ACS_SYS.MoveStop(axis);
	
	return TRUE;
}
//===================================================================================
BOOL CMotion::Move_EMG_Stop(long axis)
	//-----------------------------------------------------------------------------------
{
	//	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	ACS_SYS.EmgStop(axis);

	return TRUE;
}

//===================================================================================
void CMotion::MotorAllStop()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;
	
//	int j;
//	for(j= 0; j< _ACS_MAX_AXIS; j++) { if(ACS_SYS.isServoOn(j)== _ON)	ACS_SYS.MoveStop(j); }
	ACS_SYS.MoveStop(_ACS_AXIS_STAGE_X);		ACS_SYS.MoveStop(_ACS_AXIS_IMP_ROLL);
	ACS_SYS.MoveStop(_ACS_AXIS_UPPER_CAM_Z);	ACS_SYS.MoveStop(_ACS_AXIS_UPPER_CAM_Y);
	ACS_SYS.MoveStop(_ACS_AXIS_LOWER_CAM_X);	ACS_SYS.MoveStop(_ACS_AXIS_LOWER_CAM_Y);	ACS_SYS.MoveStop(_ACS_AXIS_LOWER_CAM_Z);
	ACS_SYS.MoveStop(_ACS_AXIS_ALIGN_Y1);		ACS_SYS.MoveStop(_ACS_AXIS_ALIGN_Y2);
	ACS_SYS.MoveStop(_ACS_AXIS_IMP_ROLL_Z1);	ACS_SYS.MoveStop(_ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CMotion::MotorAll_EMG_Stop()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	if(!ACS_DeviceOpenOK)  return ;
	
	int j;
	for(j= 0; j< _ACS_MAX_AXIS; j++) { if(ACS_SYS.isServoOn(j)== _ON)	ACS_SYS.EmgStop(j); }
}
//===================================================================================
BOOL CMotion::JogStart(long axis, long dir, double spd)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	if(g_bZ_InterLockFlag)
	{
		if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
		{ if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; }
	}

	ACS_SYS.SetSpeed(axis, spd, ParamFile()->Motion_data.dWork_Accel[axis]*10.0, ParamFile()->Motion_data.dWork_Accel[axis]*10.0);
	ACS_SYS.JogStart(axis, dir, spd);

	return TRUE;
}
//===================================================================================
BOOL CMotion::JogStart_Dual(long axis1, long axis2, long dir, double spd)
	//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	if((axis1== _ACS_AXIS_STAGE_X || axis1== _ACS_AXIS_LOWER_CAM_X ||axis1== _ACS_AXIS_LOWER_CAM_Y ||	axis1== _ACS_AXIS_ALIGN_Y1 ||axis1== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis1, POSITION_READY))	return FALSE; }

	ACS_SYS.SetSpeed(axis1, spd, ParamFile()->Motion_data.dWork_Accel[axis1], ParamFile()->Motion_data.dWork_Decel[axis1]); 	ACS_SYS.SetSpeed(axis2, spd, ParamFile()->Motion_data.dWork_Accel[axis2], ParamFile()->Motion_data.dWork_Decel[axis2]); 
	ACS_SYS.JogStart_Dual(axis1, axis2, dir, dir, spd);
	return TRUE;
}
//===================================================================================
BOOL  CMotion::Align_JogStart(long axis, long dir, double spd)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; }

	int axis1, axis2;
	CString strText;
	if(axis== ALIGN_STAGE_X)
	{
		strText.Format(_T("축 번호 ERROR!\n[%d][X]"), axis);	MyMessageBox(strText.GetBuffer(0), _T("Aling_JogStart"), M_ICONERROR);	return FALSE;
	}
	else if(axis== ALIGN_STAGE_Y)
	{
		axis1= _ACS_AXIS_ALIGN_Y1;	axis2= _ACS_AXIS_ALIGN_Y2;
		ACS_SYS.SetSpeed(axis1, spd, ParamFile()->Motion_data.dWork_Accel[axis1], ParamFile()->Motion_data.dWork_Decel[axis1]); 	ACS_SYS.SetSpeed(axis2, spd, ParamFile()->Motion_data.dWork_Accel[axis2], ParamFile()->Motion_data.dWork_Decel[axis2]); 
		ACS_SYS.JogStart_Dual(axis1, axis2, dir, dir, spd);
	}
	else if(axis== ALIGN_STAGE_T)
	{
		axis1= _ACS_AXIS_ALIGN_Y1;	axis2= _ACS_AXIS_ALIGN_Y2;
		ACS_SYS.SetSpeed(axis1, spd, ParamFile()->Motion_data.dWork_Accel[axis1], ParamFile()->Motion_data.dWork_Decel[axis1]); 	ACS_SYS.SetSpeed(axis2, spd, ParamFile()->Motion_data.dWork_Accel[axis2], ParamFile()->Motion_data.dWork_Decel[axis2]); 
		if   (dir== _DIR_CW)
			ACS_SYS.JogStart_Dual(axis1, axis2, _DIR_CCW, _DIR_CW, spd);
		else if(dir== _DIR_CCW)
			ACS_SYS.JogStart_Dual(axis1, axis2, _DIR_CW, _DIR_CCW, spd);
	}
	else
	{
		strText.Format(_T("축 번호 ERROR!\n[%d][X]"), axis);	MyMessageBox(strText.GetBuffer(0), _T("Aling_JogStart"), M_ICONERROR);	return FALSE;
	}

	return TRUE;
}

//===================================================================================
BOOL CMotion::JogStop(long axis)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	ACS_SYS.MoveStop(axis);

	return TRUE;
}
//===================================================================================
BOOL CMotion::JogStop_Dual(long axis1, long axis2)
	//-----------------------------------------------------------------------------------
{
	//	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	ACS_SYS.JogStop_Dual(axis1, axis2);
	return TRUE;
}
//===================================================================================
BOOL CMotion::INC_Move(long axis, double dist, int flag, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
		
	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;

	//174.2
	double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
	double RollFact= 360.0 / (dRoll_Diameter * PI);

	double curX= Get_Position(_ACS_AXIS_STAGE_X);
	double curBX= Get_Position(_ACS_AXIS_LOWER_CAM_X);
	double curBZ= Get_Position(_ACS_AXIS_LOWER_CAM_Z);
	double x_hap= curX + curBX + dist;

	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	
		axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{
		if(fabs(dist) > 1.0 && (axis== _ACS_AXIS_LOWER_CAM_Z || !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z)))
		{
			if(axis== _ACS_AXIS_LOWER_CAM_Y && Get_Position(_ACS_AXIS_STAGE_X) < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				;
			else if(axis== _ACS_AXIS_LOWER_CAM_X)
			{
				if(x_hap < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				{
					if(x_hap > ParamFile()->System_data.dX_Axis_Interlock)
					{
						return FALSE;
					}
				}
				else if(x_hap > ParamFile()->System_data.X_SAFE_ZONE_L1 && x_hap < ParamFile()->System_data.X_SAFE_ZONE_L2)
				{
					;
				}
				else
				{
					if(curX> 5.0)
					{
						if(!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
							/*if(!Safe_PosCheck(axis, POSITION_READY))*/	return FALSE; 
					}
				}
			}
			else
			{
				if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; 
			}
		}
	}

	if(!isLimitPosCheck(axis, dist, false))
	{
		CString strText;
		strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	if(flag== _JOG_MODE)
	{
		spd= ParamFile()->Motion_data.dJog_MidSpeed[axis];
		acc= ParamFile()->Motion_data.dJog_MidSpeed[axis] * 10.0;
		dec= ParamFile()->Motion_data.dJog_MidSpeed[axis] * 10.0;
	}
	else//_SPD_MODE
	{
		spd= ParamFile()->Motion_data.dWork_Speed[axis];
		acc= ParamFile()->Motion_data.dWork_Accel[axis];
		dec= ParamFile()->Motion_data.dWork_Decel[axis];
	}
	
	if(AxisHomeStat[axis]!= _MT_END) 
	{
		CString strText;
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d:%s 축 ]"),axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	double sx_pos= 0.0;
	double rx_pos= 0.0;
	
	ACS_SYS.SetSpeed(axis, spd, acc, dec);
	//return FALSE;
	if(axis== _ACS_AXIS_IMP_ROLL)	ret= ACS_SYS.MoveToINC(axis, dist*RollFact);
	else							ret= ACS_SYS.MoveToINC(axis, dist);

	if(ret) 
	{
		if(EndDoneCheck)
		{
			Sleep(100);
			return EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);

/*			DWORD st= GetTickCount();
			while(1)
			{
				if(isDone(axis)) return TRUE;
				if((GetTickCount()-st) > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT) return FALSE;
				Sleep(1);
			}
*/
		}
		return TRUE;
	}
	return FALSE;
}
//===================================================================================
BOOL CMotion::INC_MoveRollAlign(long axis, double dist, int flag, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
		
	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;

	//174.2
	double dRoll_DiameterOld = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
	double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[STAGE_3_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_3_ALIGN_PROC];
	
	double RollFact= 360.0 / (dRoll_Diameter * PI);
	double curX= Get_Position(_ACS_AXIS_STAGE_X);
	double curBX= Get_Position(_ACS_AXIS_LOWER_CAM_X);
	double curBZ= Get_Position(_ACS_AXIS_LOWER_CAM_Z);
	double x_hap= curX + curBX + dist;

	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	
		axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{
		if(fabs(dist) > 1.0 && (axis== _ACS_AXIS_LOWER_CAM_Z || !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z)))
		{
			if(axis== _ACS_AXIS_LOWER_CAM_Y && Get_Position(_ACS_AXIS_STAGE_X) < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				;
			else if(axis== _ACS_AXIS_LOWER_CAM_X)
			{
				if(x_hap < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				{
					if(x_hap > ParamFile()->System_data.dX_Axis_Interlock)
					{
						return FALSE;
					}
				}
				else if(x_hap > ParamFile()->System_data.X_SAFE_ZONE_L1 && x_hap < ParamFile()->System_data.X_SAFE_ZONE_L2)
				{
					;
				}
				else
				{
					if(!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
						/*if(!Safe_PosCheck(axis, POSITION_READY))*/	return FALSE; 
				}
			}
			else
			{
				if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; 
			}
		}
	}

	if(!isLimitPosCheck(axis, dist, false))
	{
		CString strText;
		strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	if(flag== _JOG_MODE)
	{
		spd= ParamFile()->Motion_data.dJog_MaxSpeed[axis];
		acc= ParamFile()->Motion_data.dJog_MaxSpeed[axis] * 10.0;
		dec= ParamFile()->Motion_data.dJog_MaxSpeed[axis] * 10.0;
	}
	else//_SPD_MODE
	{
		spd= ParamFile()->Motion_data.dWork_Speed[axis];
		acc= ParamFile()->Motion_data.dWork_Accel[axis];
		dec= ParamFile()->Motion_data.dWork_Decel[axis];
	}
	
	if(AxisHomeStat[axis]!= _MT_END) 
	{
		CString strText;
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d:%s 축 ]"),axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	double sx_pos= 0.0;
	double rx_pos= 0.0;
	
	ACS_SYS.SetSpeed(axis, spd, acc, dec);
	//return FALSE;
	if(axis== _ACS_AXIS_IMP_ROLL)	ret= ACS_SYS.MoveToINC(axis, dist*RollFact);
	else							ret= ACS_SYS.MoveToINC(axis, dist);

	if(ret) 
	{
		if(EndDoneCheck)
		{
			Sleep(100);
			return EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);

/*			DWORD st= GetTickCount();
			while(1)
			{
				if(isDone(axis)) return TRUE;
				if((GetTickCount()-st) > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT) return FALSE;
				Sleep(1);
			}
*/
		}
		return TRUE;
	}
	return FALSE;
}
//===================================================================================
BOOL CMotion::INC_Move_SafeMode(long axis, double dist, int flag, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
		
	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;

	//174.2
	double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
	double RollFact= 360.0 / (dRoll_Diameter * PI);


	if(!isLimitPosCheck(axis, dist, false))
	{
		CString strText;
		strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	if(flag== _JOG_MODE)
	{
		spd= ParamFile()->Motion_data.dJog_MaxSpeed[axis];
		acc= ParamFile()->Motion_data.dJog_MaxSpeed[axis] * 10.0;
		dec= ParamFile()->Motion_data.dJog_MaxSpeed[axis] * 10.0;
	}
	else//_SPD_MODE
	{
		spd= ParamFile()->Motion_data.dWork_Speed[axis];
		acc= ParamFile()->Motion_data.dWork_Accel[axis];
		dec= ParamFile()->Motion_data.dWork_Decel[axis];
	}
	
	if(AxisHomeStat[axis]!= _MT_END) 
	{
		CString strText;
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d:%s 축 ]"),axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	double sx_pos= 0.0;
	double rx_pos= 0.0;
	
	ACS_SYS.SetSpeed(axis, spd, acc, dec);
	if(axis== _ACS_AXIS_IMP_ROLL)	ret= ACS_SYS.MoveToINC(axis, dist*RollFact);
	else							ret= ACS_SYS.MoveToINC(axis, dist);

	if(ret) 
	{
		if(EndDoneCheck)
		{
			Sleep(100);
			return EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);

/*			DWORD st= GetTickCount();
			while(1)
			{
				if(isDone(axis)) return TRUE;
				if((GetTickCount()-st) > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT) return FALSE;
				Sleep(1);
			}
*/
		}
		return TRUE;
	}
	return FALSE;
}
//===================================================================================
BOOL CMotion::INC_Move_Dual(long axis1, long axis2, double dist1,  double dist2, int flag, BOOL EndDoneCheck, int SyncMode)
	//-----------------------------------------------------------------------------------
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	CString strText;
	double sx_pos= 0.0;
	double rx_pos= 0.0;

	if((axis1== _ACS_AXIS_STAGE_X || axis1== _ACS_AXIS_LOWER_CAM_X ||axis1== _ACS_AXIS_LOWER_CAM_Y ||	axis1== _ACS_AXIS_ALIGN_Y1 ||axis1== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis1, POSITION_READY))	return FALSE; }

	if(flag== _JOG_MODE)
	{
		spd= ParamFile()->Motion_data.dJog_MaxSpeed[axis1];	acc= ParamFile()->Motion_data.dJog_MaxSpeed[axis1] * 5.0; dec= ParamFile()->Motion_data.dJog_MaxSpeed[axis1] * 5.0;
	}
	else//_SPD_MODE
	{
		spd= ParamFile()->Motion_data.dWork_Speed[axis1];	acc= ParamFile()->Motion_data.dWork_Accel[axis1];	dec= ParamFile()->Motion_data.dWork_Decel[axis1];
	}
	if(AxisHomeStat[axis1]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(AxisHomeStat[axis2]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis1))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis2))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}

		ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
		ret= ACS_SYS.MoveToINC_Dual(axis1, axis2, dist1, dist2);
		if(ret) 
		{
			if(EndDoneCheck)
			{
				if(!EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT))		return FALSE;
				return EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
			}
			return TRUE;
		}

	return FALSE;
}
//===================================================================================
BOOL CMotion::INC_Move_Dual_Speed(long axis1, long axis2, double dist1,  double dist2, double speed, int flag, BOOL EndDoneCheck, int SyncMode)
	//-----------------------------------------------------------------------------------
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	CString strText;
	double sx_pos= 0.0;
	double rx_pos= 0.0;

	if((axis1== _ACS_AXIS_STAGE_X || axis1== _ACS_AXIS_LOWER_CAM_X ||axis1== _ACS_AXIS_LOWER_CAM_Y ||	axis1== _ACS_AXIS_ALIGN_Y1 ||axis1== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis1, POSITION_READY))	return FALSE; }

	if(speed < 0.0) speed= 0.01;
	if(speed > 10.0) speed= 10.0;
	if(speed < 1.0)
	{
		spd= speed;	acc= speed * 100.0; dec= speed * 100.0;
	}
	else
	{
		spd= speed;	acc= speed * 10.0; dec= speed * 10.0;
	}

	if(AxisHomeStat[axis1]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(AxisHomeStat[axis2]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis1))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis2))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
	}

	ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
	ret= ACS_SYS.MoveToINC_Dual(axis1, axis2, dist1, dist2);
	if(ret) 
	{
		if(EndDoneCheck)
		{
			if(!EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT))		return FALSE;
			return EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
		}
		return TRUE;
	}

	return FALSE;
}
//===================================================================================
BOOL CMotion::Align_INC_Move(long axis, double dist, int flag, BOOL EndDoneCheck, int SyncMode)
	//-----------------------------------------------------------------------------------
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	CString strText;
	double dist1= 0.0;
	double dist2= 0.0;
	int axis1, axis2;

	double curX= Get_Position(_ACS_AXIS_STAGE_X);
	double curBX= Get_Position(_ACS_AXIS_LOWER_CAM_X);
	double x_hap= curX + curBX + dist;

	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	
		axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{
		if(fabs(dist) > 1.0)
		{
			if(axis== _ACS_AXIS_LOWER_CAM_Y && Get_Position(_ACS_AXIS_STAGE_X) < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				;
			else if(axis== _ACS_AXIS_LOWER_CAM_X)
			{
				if(Get_Position(_ACS_AXIS_STAGE_X) < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				{
					if(x_hap > ParamFile()->System_data.dX_Axis_Interlock)
					{
						return FALSE;
					}
				}
				else if(x_hap > ParamFile()->System_data.X_SAFE_ZONE_L1 && x_hap < ParamFile()->System_data.X_SAFE_ZONE_L2)
				{
					;
				}
				else
				{
					if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; 
				}
			}
			else
			{
				if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; 
			}
		}
	}

	if(axis== ALIGN_STAGE_X)
	{
		strText.Format(_T("축 번호 ERROR!\n[%d][X]"), axis);	MyMessageBox(strText.GetBuffer(0), _T("Aling_INC_Move"), M_ICONERROR);	return FALSE;
	}
	else if(axis== ALIGN_STAGE_Y)
	{
		axis1= _ACS_AXIS_ALIGN_Y1;	axis2= _ACS_AXIS_ALIGN_Y2;
		dist1= dist;				dist2= dist; 
		spd= ParamFile()->Motion_data.dWork_Speed[axis1];	acc= ParamFile()->Motion_data.dWork_Accel[axis1];	dec= ParamFile()->Motion_data.dWork_Decel[axis1];
		if(AxisHomeStat[axis1]!= _MT_END) 
		{
			strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		if(AxisHomeStat[axis2]!= _MT_END) 
		{
			strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		if(!isServoOn(axis1))
		{
			strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		if(!isServoOn(axis2))
		{
			strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
		ret= ACS_SYS.MoveToINC_Dual(axis1, axis2, dist1, dist2);
		if(ret) 
		{
			if(EndDoneCheck)
			{
				if(!EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT))		return FALSE;
				return EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
			}
			return TRUE;
		}
	}
	else if(axis== ALIGN_STAGE_T)
	{
		axis1= _ACS_AXIS_ALIGN_Y1;	axis2= _ACS_AXIS_ALIGN_Y2;
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
		double cal_fact= 3.072314416687;
#else
		double cal_fact= 3.607238;
#endif

		dist1= -dist * cal_fact;				dist2= dist * cal_fact; 

		spd= ParamFile()->Motion_data.dWork_Speed[axis1];	acc= ParamFile()->Motion_data.dWork_Accel[axis1];	dec= ParamFile()->Motion_data.dWork_Decel[axis1];
		if(AxisHomeStat[axis1]!= _MT_END) 
		{
			strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		if(AxisHomeStat[axis2]!= _MT_END) 
		{
			strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		if(!isServoOn(axis1))
		{
			strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		if(!isServoOn(axis2))
		{
			strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("INC_Move_Dual"), M_ICONERROR);	return FALSE;
		}
		ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
		ret= ACS_SYS.MoveToINC_Dual(axis1, axis2, dist1, dist2);
		if(ret) 
		{
			if(EndDoneCheck)
			{
				if(!EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT))		return FALSE;
				return EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
			}
			return TRUE;
		}
	}
	else
	{
		strText.Format(_T("축 번호 ERROR!\n[%d][?]"), axis);	MyMessageBox(strText.GetBuffer(0), _T("Aling_INC_Move"), M_ICONERROR);	return FALSE;
	}

	return FALSE;
}

//===================================================================================
BOOL CMotion::INC_Move_Speed(long axis, double dist, double speed, int flag, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
		
	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;

/*
	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{ if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; }
*/
	double curX= Get_Position(_ACS_AXIS_STAGE_X);
	double curBX= Get_Position(_ACS_AXIS_LOWER_CAM_X);
	double x_hap= curX + curBX + dist;
	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	
		axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{
		if(fabs(dist) > 1.0 && (axis== _ACS_AXIS_LOWER_CAM_Z || !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z)))
		{
			if(axis== _ACS_AXIS_LOWER_CAM_Y && Get_Position(_ACS_AXIS_STAGE_X) < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				;
			else if(axis== _ACS_AXIS_LOWER_CAM_X)
			{
				if(Get_Position(_ACS_AXIS_STAGE_X) < ParamFile()->System_data.X_SAFE_FREEZONE_H)
				{
					if(x_hap > ParamFile()->System_data.dX_Axis_Interlock)
					{
						return FALSE;
					}
				}
				else if(x_hap > ParamFile()->System_data.X_SAFE_ZONE_L1 && x_hap < ParamFile()->System_data.X_SAFE_ZONE_L2)
				{
					;
				}
				else
				{
					if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; 
				}
			}
			else
			{
				if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; 
			}
		}
	}

	if(!isLimitPosCheck(axis, dist, false))
	{
		CString strText;
		strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	if(flag== _JOG_MODE)
	{
		spd= ParamFile()->Motion_data.dJog_MaxSpeed[axis];
		acc= ParamFile()->Motion_data.dWork_Accel[axis];
		dec= ParamFile()->Motion_data.dWork_Decel[axis];
	}
	else//_SPD_MODE
	{
			spd= speed;
			acc= ParamFile()->Motion_data.dWork_Accel[axis];
			dec= ParamFile()->Motion_data.dWork_Decel[axis];
	}
	double sx_pos= 0.0;
	double rx_pos= 0.0;
	
	ACS_SYS.SetSpeed(axis, spd, acc, dec);
	ret= ACS_SYS.MoveToINC(axis, dist);

	if(ret) 
	{
		if(EndDoneCheck)
		{
			return EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);

/*			DWORD st= GetTickCount();
			while(1)
			{
				if(isDone(axis)) return TRUE;
				if((GetTickCount()-st) > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT) return FALSE;
				Sleep(1);
			}
*/
		}
		return TRUE;
	}
	return FALSE;
}
//===================================================================================
BOOL CMotion::ABS_Move(long axis, double dist, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	bool ret= false;
	double spd= 0.0; 
	double acc= 0.0;
	double dec= 0.0;
	
	//174.2
	double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
	double RollFact= 360.0 / (dRoll_Diameter * PI);

	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{ if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; }

	if(axis== _ACS_AXIS_IMP_ROLL_Z1 || axis== _ACS_AXIS_IMP_ROLL_Z2)
	{
		if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC]== RUN_ENABLE && dist > ParamFile()->System_data.dZ_Base_Position[STAGE_1_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC]== RUN_ENABLE && dist > ParamFile()->System_data.dZ_Base_Position[STAGE_2_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE && dist > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE && dist > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && dist > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
	}
	if(AxisHomeStat[axis]!= _MT_END) 
	{
		CString strText;
		strText.Format(_T("원점 동작이 안되었습니다!\n[%d:%s 축 ]"), axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}
	if(!isServoOn(axis))
	{
		CString strText;
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d:%s 축 ]"), axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	if(!isLimitPosCheck(axis, dist, true))
	{
		CString strText;
		strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis, GetMotionName(axis), dist);
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}

	spd= ParamFile()->Motion_data.dWork_Speed[axis];
	acc= ParamFile()->Motion_data.dWork_Accel[axis];
	dec= ParamFile()->Motion_data.dWork_Decel[axis];

	double z_pos= 0.0;
	double sx_pos= 0.0;
	double rx_pos= 0.0;

	ACS_SYS.SetSpeed(axis, spd, acc, dec);
// 	if(axis== _ACS_AXIS_IMP_ROLL)	ret= ACS_SYS.MoveToABS(axis, dist*RollFact);
// 	else							
		ret= ACS_SYS.MoveToABS(axis, dist);

	if(EndDoneCheck)
	{
		return EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
	}
	if(ret) return TRUE;
	return FALSE;
}
//===================================================================================
BOOL CMotion::ABS_Move_Dual(long axis1, long axis2, double dist1, double dist2, BOOL EndDoneCheck, int SyncMode)
	//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	CString strText;
	double sx_pos= 0.0;
	double rx_pos= 0.0;


	if((axis1== _ACS_AXIS_STAGE_X || axis1== _ACS_AXIS_LOWER_CAM_X ||axis1== _ACS_AXIS_LOWER_CAM_Y ||	axis1== _ACS_AXIS_ALIGN_Y1 ||axis1== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis1, POSITION_READY))	return FALSE; }

	spd= ParamFile()->Motion_data.dWork_Speed[axis1];	acc= ParamFile()->Motion_data.dWork_Accel[axis1];	dec= ParamFile()->Motion_data.dWork_Decel[axis1];

	if(AxisHomeStat[axis1]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(AxisHomeStat[axis2]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis1))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis2))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}

	if(axis1== _ACS_AXIS_IMP_ROLL_Z1 || axis1== _ACS_AXIS_IMP_ROLL_Z2)
	{
		if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC]== RUN_ENABLE && dist1 > ParamFile()->System_data.dZ_Base_Position[STAGE_1_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis1, GetMotionName(axis1), dist1);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC]== RUN_ENABLE && dist1 > ParamFile()->System_data.dZ_Base_Position[STAGE_2_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis1, GetMotionName(axis1), dist1);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE && dist1 > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis1, GetMotionName(axis1), dist1);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE && dist1 > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis1, GetMotionName(axis1), dist1);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && dist1 > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			CString strText;
			strText.Format(_T("이동거리 초과 입력...!\n[%d:%s 축 ]\n입력: %.3f [mm]"), axis1, GetMotionName(axis1), dist1);
			MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
			return FALSE;
		}
	}
	

	ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
		ret= ACS_SYS.MoveToABS_Dual(axis1, axis2, dist1, dist2);
		if(ret) 
		{
			if(EndDoneCheck)
			{
				EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
				return EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
			}
			return TRUE;
		}

	return FALSE;
}
//===================================================================================
BOOL CMotion::ABS_Move_Dual_Speed(long axis1, long axis2, double dist1, double dist2, double run_speed, BOOL EndDoneCheck, int SyncMode)
	//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	CString strText;
	double sx_pos= 0.0;
	double rx_pos= 0.0;

	if((axis1== _ACS_AXIS_STAGE_X || axis1== _ACS_AXIS_LOWER_CAM_X ||axis1== _ACS_AXIS_LOWER_CAM_Y ||	axis1== _ACS_AXIS_ALIGN_Y1 ||axis1== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis1, POSITION_READY))	return FALSE; }

	spd= run_speed;	acc= ParamFile()->Motion_data.dWork_Accel[axis1];	dec= ParamFile()->Motion_data.dWork_Decel[axis1];

	if(AxisHomeStat[axis1]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(AxisHomeStat[axis2]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis1))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis2))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual"), M_ICONERROR);	return FALSE;
	}

	ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
	ret= ACS_SYS.MoveToABS_Dual(axis1, axis2, dist1, dist2);
	if(ret) 
	{
		if(EndDoneCheck)
		{
			EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
			return EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
		}
		return TRUE;
	}

	return FALSE;
}
//===================================================================================
BOOL CMotion::ABS_Move_Dual_Offset(long axis1, long axis2, double dist1, double axis2_offset, BOOL EndDoneCheck, int SyncMode)
	//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	CString strText;
	double sx_pos= 0.0;
	double rx_pos= 0.0;

	if((axis1== _ACS_AXIS_STAGE_X || axis1== _ACS_AXIS_LOWER_CAM_X ||axis1== _ACS_AXIS_LOWER_CAM_Y ||	axis1== _ACS_AXIS_ALIGN_Y1 ||axis1== _ACS_AXIS_ALIGN_Y2)) 
	{ if(!Safe_PosCheck(axis1, POSITION_READY))	return FALSE; }

	spd= ParamFile()->Motion_data.dWork_Speed[axis1];	acc= ParamFile()->Motion_data.dWork_Accel[axis1];	dec= ParamFile()->Motion_data.dWork_Decel[axis1];

	if(AxisHomeStat[axis1]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual_Offset"), M_ICONERROR);	return FALSE;
	}
	if(AxisHomeStat[axis2]!= _MT_END) 
	{
		strText.Format(_T("원점 동작이 안되었습니다!\n[ %d-%s 축 ]"),axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual_Offset"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis1))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis1, GetMotionName(axis1));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual_Offset"), M_ICONERROR);	return FALSE;
	}
	if(!isServoOn(axis2))
	{
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d-%s 축 ]"), axis2, GetMotionName(axis2));	MyMessageBox(strText.GetBuffer(0), _T("ABS_Move_Dual_Offset"), M_ICONERROR);	return FALSE;
	}

		ACS_SYS.SetSpeed(axis1, spd, acc, dec);		ACS_SYS.SetSpeed(axis2, spd, acc, dec);
		ret= ACS_SYS.MoveToABS_Dual(axis1, axis2, dist1, dist1 + axis2_offset);
		if(ret) 
		{
			if(EndDoneCheck)
			{
				EndDone(axis2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
				return EndDone(axis1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
			}
			return TRUE;
		}
	return FALSE;
}

//===================================================================================
BOOL CMotion::ABS_Move_Speed(long axis, double dist, double speed, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	bool ret= false;
	double spd= 0.0;
	double acc= 0.0;
	double dec= 0.0;
	
	//174.2
	double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
	double RollFact= 360.0 / (dRoll_Diameter * PI);

	if((axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_X ||axis== _ACS_AXIS_LOWER_CAM_Y ||	axis== _ACS_AXIS_ALIGN_Y1 ||axis== _ACS_AXIS_ALIGN_Y2 ||axis== _ACS_AXIS_LOWER_CAM_Z)) 
	{ if(!Safe_PosCheck(axis, POSITION_READY))	return FALSE; }

	if(AxisHomeStat[axis]!= _MT_END) 
	{
		CString strText;
		strText.Format(_T("원점 동작이 안되었습니다!\n[%d:%s 축 ]"), axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}
	if(!isServoOn(axis))
	{
		CString strText;
		strText.Format(_T("Servo-ON이 안되었습니다!\n[%d:%s 축 ]"), axis, GetMotionName(axis));
		MyMessageBox(strText.GetBuffer(0), _T("INC_Move"), M_ICONERROR);
		return FALSE;
	}
		spd= speed;
		acc= ParamFile()->Motion_data.dWork_Accel[axis];
		dec= ParamFile()->Motion_data.dWork_Decel[axis];
	
	double z_pos= 0.0;
	
	ACS_SYS.SetSpeed(axis, spd, acc, dec);
// 	if(axis== _ACS_AXIS_IMP_ROLL)	ret= ACS_SYS.MoveToABS(axis, dist*RollFact);
// 	else							
		ret= ACS_SYS.MoveToABS(axis, dist);
	
	if(EndDoneCheck)
	{
		return EndDone(axis, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
	}
	if(ret) return TRUE;
	return FALSE;
}
//===================================================================================
double CMotion::GetADValue(int no)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	double rrr= 0.0;
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	AD_OffsetVal[_AD_LOADCELL_F]= 0.4;	AD_OffsetVal[_AD_LOADCELL_R]= 0.0;
	rrr= ACS_SYS.GetAD(no)-AD_OffsetVal[no];
#else
	AD_OffsetVal[_AD_LOADCELL_F]= 0.0;	AD_OffsetVal[_AD_LOADCELL_R]= 0.0;
	if(no== _AD_LOADCELL_F)			rrr= ACS_SYS.Get_Real_Val(_T("AD_FZ"), 0);
	else if(no== _AD_LOADCELL_R)	rrr= ACS_SYS.Get_Real_Val(_T("AD_RZ"), 0);
	else							rrr= ACS_SYS.GetAD(no)-AD_OffsetVal[no];
#endif
	return rrr;
}
//===================================================================================
double CMotion::GetDiamValue(int no)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if (!Motion_Initial || PROGRAM_EXIT == TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if (!ACS_DeviceOpenOK)  return FALSE;

	double rrr = ACS_SYS.GetDiam(no);
	
	return rrr;
}
//===================================================================================
BOOL CMotion::SetDancerDataValue(double data, int no)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if (!Motion_Initial || PROGRAM_EXIT == TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if (!ACS_DeviceOpenOK)  return FALSE;

	return TRUE;
}

double CMotion::radianToDegree(double rad) {
    return (180.0 / PI)* rad;
}
double CMotion::degreeToRadian(double deg) {
    return (PI / 180.0 ) * deg;
}

BOOL CMotion::Z2Home_Offset_Send()
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	ACS_SYS.Set_Real_Val(_T("g_Z2Home_Offset"), 0, ParamFile()->System_data.dZ2HomeOffsetVal);
	return TRUE;
}
BOOL CMotion::OFF_Sync_Start()
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
/*
	ACS_SYS.BufferStop(_STAGE_SYNC_BUFFER_NO);
	
	ACS_SYS.Set_Real_Val("P_Roll_l", 0, ParamFile()->Model_data[SelLayer].dPatternRollGear);
	ACS_SYS.Set_Real_Val("B_Roll_l", 0, ParamFile()->Model_data[SelLayer].dBlanketRollGear);
	ACS_SYS.Set_Real_Val("OFF_A_Speed", 0, ParamFile()->Model_data[SelLayer].dPrintSpeed);
	ACS_SYS.Set_Real_Val("OFF_A_EPOS", 0, ParamFile()->Model_data[SelLayer].dPrintLength);
	ACS_SYS.Set_Real_Val("interval", 0, 9.0);
	ACS_SYS.Set_Real_Val("OFF_FLAG", 0, 1);
	
	if(ParamFile()->Model_data[SelLayer].iPrint_OFF_DelayTime> 0) Sleep(ParamFile()->Model_data[SelLayer].iPrint_OFF_DelayTime);
	//	Sleep(500);
	ACS_SYS.BufferRun(_STAGE_DATA_BUFFER_NO);
	ACS_SYS.BufferRun(_STAGE_SYNC_BUFFER_NO);
*/
	
	return TRUE;
}
BOOL CMotion::Sync_Mode_Set(int stage_no)
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;


	if(stage_no== ROLL_ALIGN_PROC || stage_no> SYNC_PROC)
	{
		AfxMessageBox(_T("ERROR=> Stage No. Error!!!")); return FALSE;
	}
	if(ParamFile()->Model_data.dRun_Speed[stage_no]== 0.0) {
		AfxMessageBox(_T("ERROR=> Speed = 0.0!!!")); return FALSE;
	}
	if(ParamFile()->Model_data.dRun_Speed[stage_no]>= 150.0) {
		AfxMessageBox(_T("ERROR=> Speed > 150!!!")); return FALSE;
	}
	if(ParamFile()->Model_data.dRun_Length[stage_no]>= 200.0) {
		AfxMessageBox(_T("ERROR=> 길이 > 200 !!!")); return FALSE;
	}
	if(ParamFile()->Model_data.dRun_Length[stage_no]<= 0.0) {
		AfxMessageBox(_T("ERROR=> 길이 = 0   Error!!!")); return FALSE;
	}

	ACS_SYS.Set_Real_Val(_T("Sync_Speed"), 0, ParamFile()->Model_data.dRun_Speed[stage_no]);
	ACS_SYS.Set_Real_Val(_T("INC_SyncMove_Pos"), 0, ParamFile()->Model_data.dRun_Length[stage_no]);
	if(stage_no== SYNC_PROC)
	{
		ACS_SYS.Set_Real_Val(_T("ROLL_DIAM"), 0, ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]);
	}
	else
	{
		ACS_SYS.Set_Real_Val(_T("ROLL_DIAM"), 0, ParamFile()->System_data.dRoll_Diameter[stage_no] + ParamFile()->System_data.dRoll_Diameter_AddVal[stage_no]);
	}
	ACS_SYS.BufferRun(_STAGE_SYNCMODE_SET_BUFFER_NO);

	return TRUE;
}
void CMotion::Sync_Mode_Reset()
{
	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;

	ACS_SYS.BufferStop(_STAGE_SYNCMODE_SET_BUFFER_NO);
	ACS_SYS.BufferRun(_STAGE_SYNC_STOP_BUFFER_NO);
}

BOOL CMotion::Sync_Start(int stage_no)
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	
	if(stage_no== ROLL_ALIGN_PROC || stage_no> SYNC_PROC)
	{
		AfxMessageBox(_T("Stage No. Error!!!")); return FALSE;
	}
	if(ParamFile()->Model_data.dRun_Speed[stage_no]== 0.0) return FALSE;
	if(ParamFile()->Model_data.dRun_Speed[stage_no]>= 150.0) return FALSE;
	if(ParamFile()->Model_data.dRun_Length[stage_no]>= 200.0) return FALSE;
	if(ParamFile()->Model_data.dRun_Length[stage_no]<= 0.0) return FALSE;

	ACS_SYS.Set_Real_Val(_T("Sync_Speed"), 0, ParamFile()->Model_data.dRun_Speed[stage_no]);
	ACS_SYS.Set_Real_Val(_T("INC_SyncMove_Pos"), 0, ParamFile()->Model_data.dRun_Length[stage_no]);
	CString strText;
	if(stage_no== SYNC_PROC)
	{
			ACS_SYS.Set_Real_Val(_T("ROLL_DIAM"), 0, ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]);
			strText.Format(_T("[Stage=%d], Diam=%.3f+%.3f, Len=%.3f, Speed=%.3f"), 
				stage_no, 
				ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC], ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC],
				ParamFile()->Model_data.dRun_Length[stage_no], ParamFile()->Model_data.dRun_Speed[stage_no]);
			Log()->SaveLogFile(_MOTION_LOG, strText);
	}
	else
	{
		ACS_SYS.Set_Real_Val(_T("ROLL_DIAM"), 0, ParamFile()->System_data.dRoll_Diameter[stage_no] + ParamFile()->System_data.dRoll_Diameter_AddVal[stage_no]);
		strText.Format(_T("[Stage=%d], Diam=%.3f+%.3f, Len=%.3f, Speed=%.3f"), 
			stage_no,
			ParamFile()->System_data.dRoll_Diameter[stage_no], ParamFile()->System_data.dRoll_Diameter_AddVal[stage_no],
			ParamFile()->Model_data.dRun_Length[stage_no], ParamFile()->Model_data.dRun_Speed[stage_no]);
		Log()->SaveLogFile(_MOTION_LOG, strText);
	}
	ACS_SYS.Set_Real_Val(_T("OFF_END"), 0, 0);

	ACS_SYS.BufferRun(_STAGE_DATA_BUFFER_NO);
	ACS_SYS.BufferRun(_STAGE_SYNC_BUFFER_NO);

	return TRUE;
}
BOOL CMotion::Grapg_Only_Start(int stage_no)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;


	if(stage_no== ROLL_ALIGN_PROC || stage_no> SYNC_PROC)
	{
		AfxMessageBox(_T("Stage No. Error!!!")); return FALSE;
	}
	ACS_SYS.BufferRun(_STAGE_DATA_BUFFER_NO);
	return TRUE;
}
void CMotion::Sync_Stop()
{
	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;

	Stamp_Stop();
	ACS_SYS.BufferStop(_STAGE_DATA_BUFFER_NO);
	ACS_SYS.BufferStop(_STAGE_SYNC_BUFFER_NO);
	ACS_SYS.BufferRun(_STAGE_SYNC_STOP_BUFFER_NO);
	
}
BOOL CMotion::isSync_End()
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	if( ACS_SYS.Get_Int_Val(_T("OFF_END"), 0) == 1)
	{
		Stamp_Stop();
		ACS_SYS.BufferStop(_STAGE_SYNC_BUFFER_NO);
		return TRUE;
	}
	return FALSE;
}
//-----------
BOOL CMotion::RollForceControl_Set(double L_Force, double R_Force)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_VAL_F"), 0, L_Force);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_VAL_R"), 0, R_Force);
	return TRUE;
}

BOOL CMotion::Z_Control_Mode_Send(int data)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
// 	return TRUE;
	if(data== 1)
		ACS_SYS.Set_Real_Val(_T("g_iZ_Control_Z1Hap"), 0, ParamFile()->Model_data.iZ_Control_Z1Hap);
	else
		ACS_SYS.Set_Real_Val(_T("g_iZ_Control_Z1Hap"), 0, 0);

	ACS_SYS.Set_Real_Val(_T("g_Z_Control_Mode"), 0, data);
	return TRUE;
}
BOOL CMotion::RollForceControl_Start(double L_Force, double R_Force, int stage_no)
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	Z_Control_Mode_Send(ParamFile()->Model_data.iZ_ControlOption);

	if(ParamFile()->Model_data.dRun_DownSpeed[stage_no]<= 0.0)	{ AfxMessageBox(_T("Down-Speed Error!!! [L]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_DownSpeed[stage_no]>= 10.0) { AfxMessageBox(_T("Down-Speed Error!!! [H]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_UpSpeed[stage_no]<= 0.0)	{ AfxMessageBox(_T("Up-Speed Error!!! [L]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_UpSpeed[stage_no]>= 10.0)	{ AfxMessageBox(_T("Up-Speed Error!!! [H]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_Force[stage_no]< 0.0)		{ AfxMessageBox(_T("Force-val Error!!! [L]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_Force2[stage_no]< 0.0)		{ AfxMessageBox(_T("Force-val Error!!! [R]")); return FALSE; }

	ACS_SYS.BufferStop(_STAGE_DATA_BUFFER_NO);
	ACS_SYS.BufferStop(_STAGE_Z_CONTROL_BUFFER_NO);

	int dispPos_Sel;
	if     (stage_no== STAGE_1_ALIGN_PROC) dispPos_Sel= POSITION_STAGE1_ALIGN;
	else if(stage_no== STAGE_2_ALIGN_PROC) dispPos_Sel= POSITION_STAGE2_ALIGN;
	else if(stage_no== STAGE_3_ALIGN_PROC) dispPos_Sel= POSITION_STAGE3_ALIGN;
	else
	{
		AfxMessageBox(_T("Pos-val Error!!! "));
		return FALSE;
	}

	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_END"), 0, 0);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_VAL_F"), 0, L_Force);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_VAL_R"), 0, R_Force);
	ACS_SYS.Set_Real_Val(_T("Z1_CONTROL_FAST_POS"), 0, 0.0/*pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel)*/);
	ACS_SYS.Set_Real_Val(_T("Z2_CONTROL_FAST_POS"), 0, 0.0/*pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel)*/);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_UP_VEL"), 0, ParamFile()->Model_data.dRun_UpSpeed[stage_no]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_DOWN_VEL"), 0, ParamFile()->Model_data.dRun_DownSpeed[stage_no]); //ParamFile()->Motion_data.dWork_Speed[_ACS_AXIS_IMP_ROLL_Z1]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_REL_VEL"), 0, 0.1);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_UP_DELAY"), 0, ParamFile()->Model_data.iRun_Delay[stage_no]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_START_FLAG"), 0, 1);

 	ACS_SYS.BufferRun(_STAGE_Z_CONTROL_BUFFER_NO);

	return TRUE;
}
BOOL CMotion::Stamp_Start(int stage_no)
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;


	if(stage_no== ROLL_ALIGN_PROC || stage_no> SYNC_PROC)
	{
		AfxMessageBox(_T("Stage No. Error!!!")); return FALSE;
	}
	if(ParamFile()->Model_data.dRun_DownSpeed[stage_no]<= 0.0)	{ AfxMessageBox(_T("Down-Speed Error!!! [L]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_DownSpeed[stage_no]>= 10.0) { AfxMessageBox(_T("Down-Speed Error!!! [H]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_UpSpeed[stage_no]<= 0.0)	{ AfxMessageBox(_T("Up-Speed Error!!! [L]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_UpSpeed[stage_no]>= 10.0)	{ AfxMessageBox(_T("Up-Speed Error!!! [H]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_Force[stage_no]< 0.0)		{ AfxMessageBox(_T("Force-val Error!!! [L]")); return FALSE; }
	if(ParamFile()->Model_data.dRun_Force2[stage_no]< 0.0)		{ AfxMessageBox(_T("Force-val Error!!! [R]")); return FALSE; }

/*
	GLOBAL INT Z_CONTROL_FLAG, Z_CONTROL_RUN;
	GLOBAL REAL Z_CONTROL_VAL_F, Z_CONTROL_VAL_R, Z_CONTROL_GAIN, Z_HIST_VAL
	GLOBAL REAL Z1_CONTROL_FAST_POS, Z2_CONTROL_FAST_POS
	GLOBAL REAL Z_CONTROL_DOWN_VEL
	GLOBAL REAL Z_CONTROL_UP_VEL
	GLOBAL REAL Z_CONTROL_UP_DELAY
	GLOBAL INT Z_CONTROL_END; Z_CONTROL_END=0;
*/
	ACS_SYS.BufferStop(_STAGE_DATA_BUFFER_NO);
	ACS_SYS.BufferStop(_STAGE_Z_CONTROL_BUFFER_NO);
	ACS_SYS.BufferRun(_STAGE_SYNC_STOP_BUFFER_NO);

	int dispPos_Sel;
	if     (stage_no== STAGE_1_ALIGN_PROC) dispPos_Sel= POSITION_STAGE1_ALIGN;
	else if(stage_no== STAGE_2_ALIGN_PROC) dispPos_Sel= POSITION_STAGE2_ALIGN;
	else if(stage_no== STAGE_3_ALIGN_PROC) dispPos_Sel= POSITION_STAGE3_ALIGN;
	else
	{
		AfxMessageBox(_T("Pos-val Error!!! "));
		return FALSE;
	}

	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_END"), 0, 0);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_VAL_F"), 0, ParamFile()->Model_data.dRun_Force[stage_no]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_VAL_R"), 0, ParamFile()->Model_data.dRun_Force2[stage_no]);
	ACS_SYS.Set_Real_Val(_T("Z1_CONTROL_FAST_POS"), 0, pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel));
	ACS_SYS.Set_Real_Val(_T("Z2_CONTROL_FAST_POS"), 0, pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel));
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_UP_VEL"), 0, ParamFile()->Model_data.dRun_UpSpeed[stage_no]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_DOWN_VEL"), 0, ParamFile()->Model_data.dRun_DownSpeed[stage_no]); //ParamFile()->Motion_data.dWork_Speed[_ACS_AXIS_IMP_ROLL_Z1]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_REL_VEL"), 0, 0.1);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_UP_DELAY"), 0, ParamFile()->Model_data.iRun_Delay[stage_no]);
	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_START_FLAG"), 0, 1);

 //	ACS_SYS.BufferRun(_STAGE_DATA_BUFFER_NO);
 	ACS_SYS.BufferRun(_STAGE_Z_CONTROL_BUFFER_NO);

	return TRUE;
}

void CMotion::Stamp_Stop()
{
	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;

	ACS_SYS.Set_Real_Val(_T("Z_CONTROL_START_FLAG"), 0, 0);

	//ACS_SYS.BufferStop(_STAGE_DATA_BUFFER_NO);
	ACS_SYS.BufferStop(_STAGE_Z_CONTROL_BUFFER_NO);

}
void CMotion::Z_Mapping_Stop()
{
	//if(SIMULATION_MODE) { return ; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return ;
	
	ACS_SYS.BufferRun(24);

}


BOOL CMotion::isStampc_End()
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	if( ACS_SYS.Get_Int_Val(_T("Z_CONTROL_RUN"), 0) == 9)//Z_CONTROL_END
	{
		//ACS_SYS.BufferStop(_STAGE_SYNC_BUFFER_NO);
		//ACS_SYS.BufferStop(_STAGE_DATA_BUFFER_NO);
		return TRUE;
	}
	return FALSE;
}
//-----------
int CMotion::GetGetherData(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5, double *Data6, double *Data7, double *Data8)
{
	return ACS_SYS.GetGetherdata(Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8);
}
int CMotion::GetGetherSyncData(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5)
{
	return ACS_SYS.GetGetherSync_data(Data1, Data2, Data3, Data4, Data5);
}

BOOL CMotion::DownLoadMAT_Data(double *Z1_Data, double *Z2_Data)
{
	if(ACS_SYS.LoadMatData(Z1_Data, Z2_Data) > 0)
	{
		ACS_SYS.BufferRun(_STAGE_CAL_BUFFER_NO);
		return TRUE;
	}
	return FALSE;
}
BOOL CMotion::DownLoadMatData_X(double *X_Pos_Data, double *X_Cal_Data, int count)
{
	if(ACS_SYS.LoadMatData_X(X_Pos_Data, X_Cal_Data, count) > 0)
	{
		ACS_SYS.BufferRun(16);
		return TRUE;
	}
	return FALSE;
}

BOOL CMotion::isMapRun()
{
	return ACS_SYS.isMapRun();
}

//==================================================================================
BOOL CMotion::X_R_LinearMoveStart(int stage_no)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;

	return Sync_Start(stage_no);
}

//==================================================================================
BOOL CMotion::LinearMoveStop()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	if(!ACS_DeviceOpenOK)  return FALSE;
	
	Sync_Stop();
	return TRUE;
}

//==================================================================================
BOOL CMotion::isStage_Check()
//-----------------------------------------------------------------------------------
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
		return FALSE;
}
//==================================================================================
CString CMotion::GetMotionName(int axis)
{
	CString strText;
	switch(axis)
	{
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	case 0: strText.Format(_T("[ACS] 리니어 STAGE_X 축")); break;
	case 1: strText.Format(_T("[ACS] 압롤 축")); break;
	case 2: strText.Format(_T("[ACS] 동기 측정 Enc.")); break;
	case 3: strText.Format(_T("[ACS] AXIS_3")); break;
	case 4: strText.Format(_T("[ACS] 상부 Vision Z 축")); break;
	case 5: strText.Format(_T("[ACS] AXIS_5")); break;
	case 6: strText.Format(_T("[ACS] AXIS_6")); break;
	case 7: strText.Format(_T("[ACS] AXIS_7")); break;
	case 8: strText.Format(_T("[ACS] 상부 Vision Y 축")); break;
	case 9: strText.Format(_T("[ACS] 하부 Vision X 축")); break;
	case 10: strText.Format(_T("[ACS] 하부 Vision Y 축")); break;
	case 11: strText.Format(_T("[ACS] 하부 Vision Z 축")); break;
	case 12: strText.Format(_T("[ACS] Align-Stage Y1 축")); break;
	case 13: strText.Format(_T("[ACS] Align-Stage Y2 축")); break;
	case 14: strText.Format(_T("[ACS] 압롤 Z1 축")); break;
	case 15: strText.Format(_T("[ACS] 압롤 Z2 축")); break;
	default: strText.Format(_T("Unknown")); break;
#else
	case 0: strText.Format(_T("[ACS] 리니어 STAGE_X 축")); break;
	case 2: strText.Format(_T("[ACS] 압롤 축")); break;
	case 1: strText.Format(_T("[ACS] 동기 측정 Enc.")); break;
	case 3: strText.Format(_T("[ACS] AXIS_3")); break;

	case 4: strText.Format(_T("[ACS] 상부 Vision Y 축")); break;
	case 5: strText.Format(_T("[ACS] 하부 Vision X 축")); break;
	case 6: strText.Format(_T("[ACS] 하부 Vision Y 축")); break;
	case 7: strText.Format(_T("[ACS] 하부 Vision Z 축")); break;

	case 8: strText.Format(_T("[ACS] 상부 Vision Z 축")); break;
	case 9: strText.Format(_T("[ACS] AXIS_9")); break;
	case 10: strText.Format(_T("[ACS] Align-Stage Y1 축")); break;
	case 11: strText.Format(_T("[ACS] Align-Stage Y2 축")); break;

	case 12: strText.Format(_T("[ACS] 압롤 Z1 축")); break;
	case 13: strText.Format(_T("[ACS] 압롤 Z2 축")); break;
	case 14: strText.Format(_T("[ACS] AXIS_14")); break;
	case 15: strText.Format(_T("[ACS] AXIS_15")); break;
	default: strText.Format(_T("Unknown")); break;
#endif
	}
	return strText;
}
//==================================================================================
void CMotion::CalAlignPos()
{
	double CenToiMarkX= 0.0;
	double CenToiMarkY= 0.0;
	if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
	{
		if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]!= ALIGN_NONE_MODE)
		{
			CenToiMarkX = ParamFile()->Model_data.dAlignMarkToMark[STAGE_1_ALIGN_PROC] / 2.0;
			CenToiMarkY = ParamFile()->Model_data.dAlignMarkToMarkY[STAGE_1_ALIGN_PROC] / 2.0;
			ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_STAGE_X]= 
				ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_STAGE_X] + 
				ParamFile()->System_data.dQuartz_PosX + ParamFile()->System_data.dStage1ToStage2_Dist + ParamFile()->System_data.dStage2ToStage3_Dist + CenToiMarkX;
			ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
				ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y] + 
				ParamFile()->System_data.dQuartz_PosY + CenToiMarkY;
		}

		if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]!= ALIGN_NONE_MODE)
		{
			CenToiMarkX = ParamFile()->Model_data.dAlignMarkToMark[STAGE_2_ALIGN_PROC] / 2.0;
			CenToiMarkY = ParamFile()->Model_data.dAlignMarkToMarkY[STAGE_2_ALIGN_PROC] / 2.0;
			ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_STAGE_X]= 
				ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_STAGE_X] + 
				ParamFile()->System_data.dQuartz_PosX + ParamFile()->System_data.dStage2ToStage3_Dist + CenToiMarkX;
			ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
				ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y] + 
				ParamFile()->System_data.dQuartz_PosY+ CenToiMarkY;
		}
		if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]!= ALIGN_NONE_MODE)
		{
			CenToiMarkX = ParamFile()->Model_data.dAlignMarkToMark[STAGE_3_ALIGN_PROC] / 2.0;
			CenToiMarkY = ParamFile()->Model_data.dAlignMarkToMarkY[STAGE_3_ALIGN_PROC] / 2.0;
			ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_STAGE_X]= 
				ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_STAGE_X] + 
				ParamFile()->System_data.dQuartz_PosX + CenToiMarkX;
			ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
				ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y] + 
				ParamFile()->System_data.dQuartz_PosY + CenToiMarkY;
		}
	// 	if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]!= ALIGN_NONE_MODE)
	// 	{
			if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
			{
				CenToiMarkX = ParamFile()->Model_data.dStamp_Len_X / 2.0;
				CenToiMarkY = ParamFile()->Model_data.dStamp_Len_Y / 2.0;
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]= 0.0;
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_UPPER_CAM_Y]=
					ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y] + 
					ParamFile()->System_data.dQuartz_PosY;

				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]= 
					ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_X] + 
					ParamFile()->System_data.dRollPlateStamp_PosX;
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= 
					ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_Y] + 
					ParamFile()->System_data.dRollPlateStamp_PosY + CenToiMarkY;

				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->System_data.dRollAlign_ZPos;
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->System_data.dRollAlign_ZPos;
			}
	//	}
			if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE &&
				ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE )
			{
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_UPPER_CAM_Y] =		ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y];
				ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_UPPER_CAM_Y] =			ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y];

				ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y] =			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
				ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_LOWER_CAM_Y] =	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
				ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_LOWER_CAM_Y] =	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
				ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_LOWER_CAM_Y] =	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
			}
			else
			{
// 				ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y] = 0.0;;
				ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_LOWER_CAM_Y] = 0.0;
				ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_LOWER_CAM_Y] = 0.0;
				ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_LOWER_CAM_Y] = 0.0;
			}


	}
}
//==================================================================================
double CMotion::LoadPosdata(long axis, int posNo)
{
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return 0.0;

	CString strText;
	double ToPosition1= 0.0;
	switch(posNo)
	{
	case POSITION_STAGE1_ALIGN_END:
// 		ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis];
// 		break;
	case POSITION_STAGE2_ALIGN_END:
// 		ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis];
// 		break;
	case POSITION_ROLL_ALIGN_END:
// 		ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis];
// 		break;
	case POSITION_SYNC_END:
// 		ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis];
// 		break;
	case POSITION_TOP_MASK_20X:
// 		ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis]+;
// 		break;
	case POSITION_BTM_MASK_20X:
// 		ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis];
		strText.Format(_T("[READ] Position-Data 에러.- \n확인 Pos.= %d [%s:%d]"), posNo, GetMotionName(axis), axis);
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return 0.0;
		break;
	default:
		ToPosition1= ParamFile()->Model_data.dPosition[posNo][axis] + ParamFile()->Model_data.dPosition_Offset[posNo][axis];
	}
	return ToPosition1;
}
//==================================================================================
BOOL CMotion::Safe_PosCheck(int axis, int pos_no)
{
	double lx_pos= Get_Position(_ACS_AXIS_STAGE_X);
	double sx_pos= Get_Position(_ACS_AXIS_LOWER_CAM_X);
	double rz1_pos= Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	double rz2_pos= Get_Position(_ACS_AXIS_IMP_ROLL_Z2);
	double lcz_pos= Get_Position(_ACS_AXIS_LOWER_CAM_Z);
	double x_cha= lx_pos + sx_pos;

	if(AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]!= _MT_END)
	{
		CString strText;
		strText.Format(_T("상부 Roll-Z1축 초기화 에러.- \n원점 동작 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}
	if(AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]!= _MT_END)
	{
		CString strText;
		strText.Format(_T("상부 Roll-Z2축 초기화 에러.- \n원점 동작 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}
	if(AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]!= _MT_END)
	{
		CString strText;
		strText.Format(_T("하부 Cam-Z축 초기화 에러.- \n원점 동작 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}

	if(axis== _ACS_AXIS_LOWER_CAM_Z)
	{
		int pos_no_cnv= pos_no;
		if(lx_pos <= 1.0) return TRUE;
		if(pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, pos_no_cnv) < 1.0)
			return TRUE;

		if(pos_no_cnv== POSITION_BTM_MASK_20X) pos_no_cnv= POSITION_BTM_MASK;
		if(pos_no_cnv== POSITION_BTM_MASK || pos_no_cnv== POSITION_BTM_MASK_20X)
		{
			if(pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, pos_no_cnv)> 5.0 && pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, pos_no_cnv)> 5.0)
			{
				CString strText;
				strText.Format(_T("상부 Plate 와 충돌 위험이 있습니다.- \nX축 위치확인 [Pos.]"));
				MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
				return FALSE;
			}
			return TRUE;
		}
		else
		{
			if(x_cha > ParamFile()->System_data.X_SAFE_FREEZONE_L && x_cha < ParamFile()->System_data.X_SAFE_FREEZONE_H)
			{
				;
			}
			else
			{
				if(x_cha < ParamFile()->System_data.X_SAFE_ZONE_L1 || x_cha > ParamFile()->System_data.X_SAFE_ZONE_L2)
				{
					CString strText;
					strText.Format(_T("[0] 상부 Plate 와 충돌 위험이 있습니다.[%.1f]\n안전위치 확인 [Pos.]"), x_cha);
					MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
					return FALSE;
				}
			}
		}
	}

	if(axis!= _ACS_AXIS_LOWER_CAM_Z) 
	{
		if(axis== _ACS_AXIS_STAGE_X || axis== _ACS_AXIS_LOWER_CAM_Y || axis== _ACS_AXIS_LOWER_CAM_X)
		{
			if(!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
			{
				if(x_cha < ParamFile()->System_data.X_SAFE_FREEZONE_L || x_cha > ParamFile()->System_data.X_SAFE_FREEZONE_H)
				{
// 					ParamFile()->System_data.X_SAFE_ZONE_L1= 400.0;
// 					ParamFile()->System_data.X_SAFE_ZONE_L2= 460.0;
					if(x_cha > ParamFile()->System_data.X_SAFE_ZONE_L1 && x_cha < ParamFile()->System_data.X_SAFE_ZONE_L2)
					{
						return TRUE;
					}
				}
				CString strText;
				strText.Format(_T("[1] 상부 Plate 와 충돌 위험이 있습니다.[%.1f]\n안전위치 확인 [Pos.]"), x_cha);
				MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
				return FALSE;
			}
		}

		if(axis== _ACS_AXIS_STAGE_X || lx_pos < ParamFile()->System_data.X_SAFE_FREEZONE_L || lx_pos > ParamFile()->System_data.X_SAFE_FREEZONE_H)
		{
			if(axis== _ACS_AXIS_LOWER_CAM_Y || axis== _ACS_AXIS_LOWER_CAM_X)
				return TRUE;

			if(rz1_pos> LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY) + 1.0)
			{
				CString strText;
				strText.Format(_T("상부 Roll-Z1축이 충돌 위험이 있습니다.- \n안전위치 확인 [Pos.]"));
				MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
				return FALSE;
			}

			if(rz2_pos> LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY) + 1.0)
			{
				CString strText;
				strText.Format(_T("상부 Roll-Z2축이 충돌 위험이 있습니다.- \n안전위치 확인 [Pos.]"));
				MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
				return FALSE;
			}
			if( !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_IMP_ROLL_Z1) || !pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_IMP_ROLL_Z2))
			{
				CString strText;
				strText.Format(_T("상부 Roll-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [ORG]"));
				MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
				return FALSE;
			}
		}
		if(lcz_pos > 0.1)
		{
			CString strText;
			strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [Pos.]"));
			MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
			return FALSE;
		}
		if(!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
		{
			CString strText;
			strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [ORG]"));
			MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}
//==================================================================================
BOOL CMotion::Z_Up_PosCheck()
{
	double lx_pos= Get_Position(_ACS_AXIS_STAGE_X);
	double sy_pos= Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	double rz1_pos= Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	double rz2_pos= Get_Position(_ACS_AXIS_IMP_ROLL_Z2);
	double rz_pos= Get_Position(_ACS_AXIS_LOWER_CAM_Z);

	if(AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z1]!= _MT_END)
	{
		CString strText;
		strText.Format(_T("상부 Roll-Z1축 초기화 에러.- \n원점 동작 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}
	if(AxisHomeStat[_ACS_AXIS_IMP_ROLL_Z2]!= _MT_END)
	{
		CString strText;
		strText.Format(_T("상부 Roll-Z2축 초기화 에러.- \n원점 동작 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}
	if(AxisHomeStat[_ACS_AXIS_LOWER_CAM_Z]!= _MT_END)
	{
		CString strText;
		strText.Format(_T("하부 Cam-Z축 초기화 에러.- \n원점 동작 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}

/*
	if(sy_pos > LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY) + 3.0 || sy_pos < LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY) - 3.0)
	{
		CString strText;
		strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [Y_Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}
*/
	if(rz_pos> LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY) + 0.5)
	{
		CString strText;
		strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [Pos.]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}

	if(	!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
	{
		CString strText;
		strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [ORG]"));
		MyMessageBox(strText.GetBuffer(0), _T("Move-Check"), M_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

//==================================================================================
BOOL	CMotion::AlignStage_ReadyPos_Move(BOOL EndDoneCheck)
{
	double ToPosition1, ToPosition2;
	CString strText;
	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	
	//-------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);  if(ToPosition1> 0.0) ToPosition1= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	TRUE)) 
	{	MyMessageBox(_T("[BOT_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(!EndDone(_ACS_AXIS_LOWER_CAM_Z, ParamFile()->System_data._AXIS_MOVE_TIME_OUT)) { MyMessageBox(_T("[BOT_Z] 모터 이동이 실패했습니다. Time-Out"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;  }
	if(	!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
	{
		CString strText;
		strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [ORG]"));
		MyMessageBox(strText.GetBuffer(0), _T("ABS_Move"), M_ICONERROR);
		return FALSE;
	}
	//-------------

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY); if(ToPosition1> 0.0) ToPosition1= 0.0;
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY); if(ToPosition2> 0.0) ToPosition2= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) 
	{	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(EndDoneCheck)
	{
		if(!EndDone(_ACS_AXIS_ALIGN_Y1, ParamFile()->System_data._AXIS_MOVE_TIME_OUT)) { MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다. Time-Out"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;  }
		if(!EndDone(_ACS_AXIS_ALIGN_Y2, ParamFile()->System_data._AXIS_MOVE_TIME_OUT)) { MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다. Time-Out"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;  }
	}
	return TRUE;
}
//===================================================================================
BOOL	CMotion::BotVision_ReadyPos_Move(BOOL EndDoneCheck)
{
	double ToPosition1, ToPosition2;
	CString strText;

	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	//-------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);  if(ToPosition1> 0.0) ToPosition1= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	TRUE)) 
	{	MyMessageBox(_T("[BOT_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	if(!EndDone(_ACS_AXIS_LOWER_CAM_Z, ParamFile()->System_data._AXIS_MOVE_TIME_OUT)) { MyMessageBox(_T("[BOT_Z] 모터 이동이 실패했습니다. Time-Out"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;  }
	if(	!pSystem.GetMotionComponent()->isORG_SensorCheck(_ACS_AXIS_LOWER_CAM_Z))
	{
		CString strText;
		strText.Format(_T("하부 Vision-Z축이 충돌 위험이 있습니다.- \n안전위치 확인 [ORG]"));
		MyMessageBox(strText.GetBuffer(0), _T("ABS_Move"), M_ICONERROR);
		return FALSE;
	}
	//-------------

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);//  if(ToPosition1> 0.0) ToPosition1= 0.0;
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY);//  if(ToPosition2> 0.0) ToPosition2= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) 
	{	MyMessageBox(_T("[BOT_XY] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition2,	FALSE)) 
	{	MyMessageBox(_T("[BOT_XY] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(EndDoneCheck)
	{
		if(!EndDone(_ACS_AXIS_LOWER_CAM_X, ParamFile()->System_data._AXIS_MOVE_TIME_OUT)) { MyMessageBox(_T("[BOT_X] 모터 이동이 실패했습니다. Time-Out"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;  }
		if(!EndDone(_ACS_AXIS_LOWER_CAM_Y, ParamFile()->System_data._AXIS_MOVE_TIME_OUT)) { MyMessageBox(_T("[BOT_Y] 모터 이동이 실패했습니다. Time-Out"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;  }
	}
	return TRUE;
}

BOOL CMotion::Roll_Z_Down_Proc(int pos_no)
{
	return FALSE;
}

double CMotion::PosCheckSWLimit(double val, int axis)
{
	double ret_val= 0.0;
	ret_val= val;
	if(val > ParamFile()->Motion_data.dSW_Limit_High[axis])	ret_val= ParamFile()->Motion_data.dSW_Limit_High[axis];
	else if(val < ParamFile()->Motion_data.dSW_Limit_Low[axis])	ret_val= ParamFile()->Motion_data.dSW_Limit_Low[axis];
	return ret_val;
}
double CMotion::PosOffsetCheckSWLimit(double val, int axis, double Offset_val)
{
	double ret_val= 0.0;
	ret_val= Offset_val;
	if(ret_val+ val > ParamFile()->Motion_data.dSW_Limit_High[axis])	ret_val= 0.0;//ParamFile()->Motion_data.dSW_Limit_High[axis];
	else if(ret_val+ val < ParamFile()->Motion_data.dSW_Limit_Low[axis])	ret_val= 0.0;//ParamFile()->Motion_data.dSW_Limit_Low[axis];
	return ret_val;
}

BOOL CMotion::isLimitPosCheck(int axis, double dist, bool flag)
{
	double cur_pos= Get_Position(axis);
	double end_pos= 0.0;
	
	if(flag)	end_pos= dist;
	else		end_pos= cur_pos + dist;

	if(end_pos > ParamFile()->Motion_data.dSW_Limit_High[axis])		return FALSE;
	else if(end_pos < ParamFile()->Motion_data.dSW_Limit_Low[axis])	return FALSE;

	return TRUE;
}

double CMotion::Roll_GetmmToDeg(double Tomm)
{
	double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
	double RollFact= 360.0 / (dRoll_Diameter * PI);

	return RollFact * Tomm;
}
//===================================================================================
BOOL CMotion::Z_ControlEnable(int stage_no)
{
	//	double ToPosition1;//, ToPosition2;
	CString strText;

	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	Z_Control_Mode_Send(0);
	Stamp_Start(stage_no);
	//-------------
	return TRUE;
}
BOOL CMotion::Z_ControlDisable()
{
//	double ToPosition1;//, ToPosition2;
	CString strText;

	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;
	Stamp_Stop();
	ACS_SYS.BufferStop(_STAGE_Z_CONTROL_BUFFER_NO);

	Z_Control_Mode_Send(0);
	//-------------
	return TRUE;
}
BOOL CMotion::isZ_ControlEnd()
{
	//	double ToPosition1;//, ToPosition2;
	CString strText;

	if(!Motion_Initial || PROGRAM_EXIT== TRUE || !ACS_DeviceOpenOK /*|| !COMI_DeviceOpenOK*/) return FALSE;

	int rrr= ACS_SYS.Get_Int_Val(_T("Z_CONTROL_END"), 0);
	if(rrr== 9) return TRUE;

	//-------------
	return FALSE;
}
//===================================================================================

void CMotion::GetStampPos(int CameraStageUsedNo, int lens_sel, double *L_X, double *Stage_Y, double *TopCamY, double *bot_X, double *bot_Y, double *Roll)
{
	CString strText;
	double move_stamp_posx= 0.0, move_stamp_posy= 0.0;
	double MaskToPosX= 0.0, MaskToPosY= 0.0;

	double move_stamp_posbx= 0.0, move_stamp_posby= 0.0;
	double MaskToPosBX= 0.0, MaskToPosBY= 0.0;
	double Stamp_Offset_Y= 0.0, ToPosition1= 0.0, ToPosition2= 0.0;

	*Roll= 0.0;

	if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
	{
		move_stamp_posx= ParamFile()->Model_data.dStamp_Len_X / 2.0;
		move_stamp_posy= ParamFile()->Model_data.dStamp_Len_Y / 2.0;
		if(move_stamp_posx > ParamFile()->System_data.dX_Axis_Interlock-1.0)
		{
			move_stamp_posx=  ParamFile()->System_data.dX_Axis_Interlock-1.0;
		}

		if(move_stamp_posy > ParamFile()->System_data.dStamp_Y_Max)
		{
			Stamp_Offset_Y= move_stamp_posy - ParamFile()->System_data.dStamp_Y_Max;
			move_stamp_posy=  ParamFile()->System_data.dStamp_Y_Max;
		}
		if(CameraStageUsedNo== eTOP_CAM)
		{
			MaskToPosX= move_stamp_posx + ParamFile()->System_data.dQuartz_PosX + pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
			MaskToPosY= move_stamp_posy + ParamFile()->System_data.dQuartz_PosY + pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);
			*L_X= MaskToPosX; *Stage_Y= Stamp_Offset_Y; *TopCamY= MaskToPosY; *bot_X= 0.0; *bot_Y= 0.0;
			return;
		}
		else
		{
			//롤 얼라인 위치 X, 롤 중심 센터 Y는 평스템프 모드인 경우 스템프 센터에서 스템프 길이의 절반 값으로 변경, 
			move_stamp_posbx= ParamFile()->Model_data.dStamp_Len_X / 2.0;
			move_stamp_posby= ParamFile()->Model_data.dStamp_Len_Y / 2.0;

			double masktomask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) + 
				ParamFile()->System_data.dQuartz_PosX - ParamFile()->System_data.dRollPlateStamp_PosX;
			MaskToPosX= masktomask_x + move_stamp_posbx;

			double pos1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			double pos2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK)+move_stamp_posby;

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY)+Stamp_Offset_Y;
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY)+Stamp_Offset_Y;
			pos1= pos1/*+move_stamp_posbx*/+ParamFile()->System_data.dRollPlateStamp_PosX;
			//			pos1= pos1+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;

			*L_X= /*masktomask_x*/MaskToPosX; *Stage_Y= Stamp_Offset_Y; *TopCamY= 0.0; *bot_X= pos1; *bot_Y= pos2;
			return;
		}
	}
	else
	{
		move_stamp_posx= ParamFile()->Model_data.dStamp_Len_X / 2.0;
		move_stamp_posy= ParamFile()->Model_data.dStamp_Len_Y / 2.0;
		if(move_stamp_posx > ParamFile()->System_data.dX_Axis_Interlock-1.0)
		{
			move_stamp_posx=  ParamFile()->System_data.dX_Axis_Interlock-1.0;
		}

		if(move_stamp_posy > ParamFile()->System_data.dStamp_Y_Max)
		{
			Stamp_Offset_Y= move_stamp_posy - ParamFile()->System_data.dStamp_Y_Max;
			move_stamp_posy=  ParamFile()->System_data.dStamp_Y_Max;
		}
		if(CameraStageUsedNo== eTOP_CAM)
		{
			MaskToPosX= move_stamp_posx + ParamFile()->System_data.dQuartz_PosX + pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
			MaskToPosY= move_stamp_posy + ParamFile()->System_data.dQuartz_PosY + pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);
			*L_X= MaskToPosX; *Stage_Y= Stamp_Offset_Y; *TopCamY= MaskToPosY; *bot_X= 0.0; *bot_Y= 0.0;
			if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE && CameraStageUsedNo== eTOP_CAM && lens_sel== e2X_LENS)
			{
				*L_X= *L_X + ParamFile()->System_data.TB_Offset_2X_X; //0.024;
				*Stage_Y= *Stage_Y + ParamFile()->System_data.TB_Offset_2X_Y; //0.;
			}

			return;
		}
		else
		{
			//롤 얼라인 위치 X, 롤 중심 센터 Y는 평스템프 모드인 경우 스템프 센터에서 스템프 길이의 절반 값으로 변경, 
			move_stamp_posbx= ParamFile()->Model_data.dStamp_Len_X / 2.0;
			move_stamp_posby= ParamFile()->Model_data.dStamp_Len_Y / 2.0;
			if(move_stamp_posbx > ParamFile()->System_data.dX_Axis_Interlock-1.0)
			{
				move_stamp_posbx=  ParamFile()->System_data.dX_Axis_Interlock-1.0;
			}
			if(move_stamp_posby > ParamFile()->System_data.dStamp_Y_Max)
			{
				Stamp_Offset_Y= move_stamp_posby - ParamFile()->System_data.dStamp_Y_Max;
				move_stamp_posby=  ParamFile()->System_data.dStamp_Y_Max;
			}
			//	move_stamp_posbx = move_stamp_posbx + ParamFile()->System_data.dQuartz_PosX;

			MaskToPosX= move_stamp_posx + ParamFile()->System_data.dQuartz_PosX + pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
			double masktomask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
			masktomask_x= masktomask_x + MaskToPosX;
//			double masktomask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) + ParamFile()->System_data.dQuartz_PosX - ParamFile()->System_data.dRollPlateStamp_PosX;
//			MaskToPosX= masktomask_x - move_stamp_posbx;

			double pos1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			double pos2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK)+move_stamp_posby;

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY)+Stamp_Offset_Y;
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY)+Stamp_Offset_Y;
			pos1= pos1;//+move_stamp_posbx+ParamFile()->System_data.dRollPlateStamp_PosX;
			//			pos1= pos1+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;

			*L_X= masktomask_x; *Stage_Y= Stamp_Offset_Y; *TopCamY= 0.0; *bot_X= pos1; *bot_Y= pos2;
			*Roll= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY) + move_stamp_posbx;
			return;
		}
	}
}

void CMotion::GetPickPlace_Pos(int Stage_no, double cur_x, double cur_y, double *tarPosX,  double *tarPosY )
{

	*tarPosX= 0.0; *tarPosY= 0.0; 

	double AlignPos_To_CurX= 0.0;
	double AlignPos_To_CurY= 0.0; 
	double Dist_X= 0.0; 
	double Dist_Y= 0.0;
	double calX= 0.0;

	if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
	{
		double move_stamp_posx= ParamFile()->Model_data.dStamp_Len_X / 2.0;
		double move_stamp_posy= ParamFile()->Model_data.dStamp_Len_Y / 2.0;

		double topMaskPos_To_CurX= cur_x - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
		double topMaskPos_To_CurY= cur_y - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);

		double btmMaskPos_To_CurY=  pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN) - move_stamp_posy;
		double btmPosChaY= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK) - btmMaskPos_To_CurY;

		double MaskToMask_X= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
		btmPosChaY= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK) - btmMaskPos_To_CurY;
		calX= MaskToMask_X + cur_x;
		if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
		{
			calX= calX + ParamFile()->System_data.TB_Offset_2X_X; //0.024;
			btmPosChaY= btmPosChaY + ParamFile()->System_data.TB_Offset_2X_Y; //0.024;
		}
		btmPosChaY= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK) - btmMaskPos_To_CurY;
		Dist_X= calX;
		Dist_Y= btmPosChaY;

		*tarPosX= Dist_X; 
		*tarPosY= Dist_Y;
	}
	else
	{
		*tarPosX= 0.0; *tarPosY= 0.0;

		double AlignPos_To_CurX= 0.0;
		double AlignPos_To_CurY= 0.0; 
		double Dist_X= 0.0; 
		double Dist_Y= 0.0;

	/*
		AlignPos_To_CurX= LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN)-cur_x;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
		AlignPos_To_CurY= LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE1_ALIGN)-cur_y;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
		Dist_X= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X] - AlignPos_To_CurX;
		Dist_Y= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_UPPER_CAM_Y] - AlignPos_To_CurY;
	*/
//		Stage_no
// 	ROLL_ALIGN_PROC		= 0,// 	STAGE_1_ALIGN_PROC	= 1,// 	STAGE_2_ALIGN_PROC	= 2,// 	STAGE_3_ALIGN_PROC	= 3,
// 	SYNC_PROC			= 4,// 	MANUAL_STAMP_PROC	= 5,// 	MANUAL_ROLL_ALIGN_PROC	= 6,
		switch(Stage_no)
		{
		case STAGE_1_ALIGN_PROC:	
			AlignPos_To_CurX= LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN)-cur_x;
			AlignPos_To_CurY= LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE1_ALIGN)-cur_y;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			Dist_X= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X] - AlignPos_To_CurX+ParamFile()->System_data.dStage1ToStage2_Dist + ParamFile()->System_data.dStage2ToStage3_Dist;
			break;
		case STAGE_2_ALIGN_PROC:	
			AlignPos_To_CurX= LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE2_ALIGN)-cur_x;
			AlignPos_To_CurY= LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE2_ALIGN)-cur_y;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			Dist_X= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X] - AlignPos_To_CurX+ParamFile()->System_data.dStage2ToStage3_Dist;
			break;
		case STAGE_3_ALIGN_PROC:	
			AlignPos_To_CurX= LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE3_ALIGN)-cur_x;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			AlignPos_To_CurY= LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE3_ALIGN)-cur_y;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			Dist_X= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X] - AlignPos_To_CurX;
			break;
		default:
			AfxMessageBox(L"Stage-number -> Not Define!!!");
			*tarPosX= 0.0; 			*tarPosY= 0.0;
			return;
		}
//		AlignPos_To_CurX= ParamFile()->System_data.dBasePos_Stage1_X-cur_x;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
//		AlignPos_To_CurY= ParamFile()->System_data.dBasePos_Stage1_CamY-cur_y;//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
		Dist_Y= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_UPPER_CAM_Y] - AlignPos_To_CurY;

		*tarPosX= Dist_X; 
		*tarPosY= Dist_Y;
	
		double MarkToMark_TX= 0.0;

		double CurTX_ToMark= cur_x - LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);
		double CurTY_ToMark= cur_y - LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);

		double CurBX_ToMark= LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK) - LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		double CurBY_ToMark= LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN) - LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK);
	
		MarkToMark_TX= LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) - LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);

		double cha_TY= cur_y - pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
	}
	
}

void CMotion::AF_Proc_Start(int CamNo, double step, int count)
{
	double Z_CurPos= 0.0;
	double Speed= 0.0;


	double AF_Stepmm= 0.01;
	if(CamNo== eTOP_CAM_20X || CamNo== eTOP_CAM_2X)		Z_CurPos= Get_Position(_ACS_AXIS_UPPER_CAM_Z);
	else												Z_CurPos= Get_Position(_ACS_AXIS_LOWER_CAM_Z);

	AF_Stepmm= step;

	double Inc_TotalZ_Pos= 0.0;
	double Inc_StartZ_Pos= 0.0;

	Inc_TotalZ_Pos= AF_Stepmm * (double)(count);
	Inc_StartZ_Pos= Inc_TotalZ_Pos / 2.0;

	Speed= AF_Stepmm * 7.0; // 1초에 10번

	if(CamNo== eTOP_CAM_20X || CamNo== eTOP_CAM_2X)	
	{
		double Cp= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
		if(Cp < ParamFile()->Motion_data.dSW_Limit_Low[_ACS_AXIS_UPPER_CAM_Z]) Inc_StartZ_Pos= ParamFile()->Motion_data.dSW_Limit_Low[_ACS_AXIS_UPPER_CAM_Z]-Cp;
		if(Cp > ParamFile()->Motion_data.dSW_Limit_High[_ACS_AXIS_UPPER_CAM_Z]) 
			Inc_StartZ_Pos= Inc_TotalZ_Pos - (ParamFile()->Motion_data.dSW_Limit_High[_ACS_AXIS_UPPER_CAM_Z]-Cp);

		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, -Inc_StartZ_Pos, _SPD_MODE, TRUE); Sleep(200);

		pSystem.GetVisionComponent()->AF_ProcStart(CamNo) ;
		pSystem.GetMotionComponent()->INC_Move_Speed(_ACS_AXIS_UPPER_CAM_Z, Inc_TotalZ_Pos, Speed, _SPD_MODE, FALSE); Sleep(200);
	}
	else
	{
		double Cp= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
		if(Cp < ParamFile()->Motion_data.dSW_Limit_Low[_ACS_AXIS_LOWER_CAM_Z]) Inc_StartZ_Pos= ParamFile()->Motion_data.dSW_Limit_Low[_ACS_AXIS_LOWER_CAM_Z]-Cp;
		if(Cp > ParamFile()->Motion_data.dSW_Limit_High[_ACS_AXIS_LOWER_CAM_Z]) 
			Inc_StartZ_Pos= Inc_TotalZ_Pos - (ParamFile()->Motion_data.dSW_Limit_High[_ACS_AXIS_LOWER_CAM_Z]-Cp);

		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, -Inc_StartZ_Pos, _SPD_MODE, TRUE); Sleep(200);

		pSystem.GetVisionComponent()->AF_ProcStart(CamNo) ;
		pSystem.GetMotionComponent()->INC_Move_Speed(_ACS_AXIS_LOWER_CAM_Z, Inc_TotalZ_Pos, Speed, _SPD_MODE, FALSE); Sleep(200);
	}
}
