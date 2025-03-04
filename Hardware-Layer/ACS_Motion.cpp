// ACS_Motion.cpp: implementation of the CACS_Motion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Transfer_Control.h"
#include "ACS_Motion.h"
#include "ACSC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSystemClass pSystem;
CParameterFile* CACS_Motion::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CACS_Motion::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

CACS_Motion::CACS_Motion()
{
	ACS_Motion_Initial= false;
}

CACS_Motion::~CACS_Motion()
{

}
//===================================================================================
bool CACS_Motion::Init(CString strIP, int port)
//-----------------------------------------------------------------------------------
{
	wcscpy(chrIP, strIP.GetBuffer(0));
	PortNo= port;

USES_CONVERSION;

	HANDLE hComm=(HANDLE)-1;
//	int Port = 703;
//	Handle = acsc_OpenCommEthernetTCP(chrIP, PortNo);
	char* pchIP = "10.0.0.100";
	//Handle = acsc_OpenCommEthernetTCP(T2A(chrIP), PortNo);
	Handle = acsc_OpenCommEthernetTCP( pchIP, PortNo);
	if (Handle == ACSC_INVALID)
	{
		CString strLog;
		strLog.Format(_T("[CACS_Motion] Error while opening communication: %d [IP:%s, Port:%d]"), acsc_GetLastError(), chrIP, PortNo);
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return false;
	}
	char* cmd = "?SN\r";
	char buf[100];
	int Received;

	if( acsc_Transaction( Handle, // communication handle
		cmd,  // pointer to the buffer that contains 
		// executedcontroller’s command
		strlen(cmd), // size of this buffer
		buf,  // input buffer that receives controller
		// response
		100,  // size of this buffer
		&Received,  // number of characters that were actually
		// received
		NULL // waiting call
		))
	{
		CString strLog;
		strLog.Format(_T("[CACS_Motion] Error Transaction Withh ACS [IP:%s, Port:%d]"), acsc_GetLastError(), chrIP, PortNo);
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	ACS_Motion_Initial= true;
	return true;
}
//===================================================================================
bool CACS_Motion::Close()
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial)	return false;

	ACS_Motion_Initial= false;

	acsc_CloseHistoryBuffer(Handle);
	if (!acsc_CloseComm(Handle))
	{
		CString strLog;
		strLog.Format(_T("[CACS_Motion] error closing communication: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return false;
	}
	return true;
}
//===================================================================================
bool CACS_Motion::ProcCancel()
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	if (!acsc_CancelOperation(Handle, NULL)){
		CString strLog;
		strLog.Format(_T("[CACS_Motion] Cancel operation error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return false;
	}
	return true;
}
//===================================================================================
bool CACS_Motion::SetSpeed(int axis, double spd, double accspd, double decspd)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	if (!acsc_SetVelocity( Handle, axis, spd, NULL	))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetVelocity transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	if (!acsc_SetAcceleration(Handle, axis, accspd, NULL	))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetAcceleration transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}

	if (!acsc_SetDeceleration( Handle, axis, decspd, NULL))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetDeceleration transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	if (!acsc_SetKillDeceleration( Handle, axis, decspd*10.0, NULL))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetDeceleration transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	if (!acsc_SetJerk( Handle, axis, accspd * 10.0, NULL))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetJerk transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::SetFPosition(int axis, double setval)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	if (!acsc_SetFPosition( Handle, // communication handle
		axis,// axis 0
		setval, // required feedback position
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetFPosition transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
double CACS_Motion::GetFPosition(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	double FPosition= 0.0;

	if (!acsc_GetFPosition(Handle, // communication handle
		axis,// axis 0
		&FPosition, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetFPosition transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return FPosition;
}
//===================================================================================
double CACS_Motion::GetFVelocity(int axis)
//-----------------------------------------------------------------------------------
{
	if (!ACS_Motion_Initial || PROGRAM_EXIT == TRUE)	return false;
	CString strLog;
	double FVelocity = 0.0;

	if (!acsc_GetFVelocity(Handle, // communication handle
		axis,// axis 0
		&FVelocity, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetFVelocity transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return FVelocity;
}

//===================================================================================
bool CACS_Motion::SetRPosition(int axis, double setval)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	if (!acsc_SetRPosition( Handle, // communication handle
		axis, // axis 0
		setval, // required feedback position
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_SetRPosition transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
double CACS_Motion::GetRPosition(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	double FPosition= 0.0;
	
	if (!acsc_GetRPosition(Handle, // communication handle
		axis,// axis 0
		&FPosition, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetRPosition transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return FPosition;
}
//===================================================================================
double CACS_Motion::GetAPosition(int axis)
	//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	double FPosition= 0.0;

	if (!acsc_GetRPosition(Handle, // communication handle
		axis,// axis 0
		&FPosition, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetRPosition transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return FPosition;
}
//===================================================================================
bool CACS_Motion::ServoOn(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	if (!acsc_Enable(Handle, // communication handle
		axis,// enable of axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_Enable transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::ServoOnAll()
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int Axes[] = { ACSC_AXIS_1, ACSC_AXIS_2, ACSC_AXIS_3, ACSC_AXIS_4, ACSC_AXIS_6, ACSC_AXIS_7, -1 };
	if (!acsc_EnableM(Handle, // communication handle
		Axes, // enable of axes 0, 1, 4, and 5
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_EnableM transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::ServoOff(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	if (!acsc_Disable(Handle, // communication handle
		axis,// enable of axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_Disable transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::ServoOffAll()
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	/*// example of the waiting call of acsc_DisableM
	int Axes[] = { ACSC_AXIS_0, ACSC_AXIS_1, ACSC_AXIS_4, ACSC_AXIS_5, -1 };
	if (!acsc_DisableM(Handle, // communication handle
	Axes, // disable axes 0, 1, 4 and 5
	NULL// waiting call
	))
	{
	printf("transaction error: %d\n"), acsc_GetLastError());
	}
	*/
	if (!acsc_DisableAll(Handle, NULL))
	{
		strLog.Format(_T("[CACS_Motion] acsc_DisableAll transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::isServoOn(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	/*// example of the waiting call of acsc_DisableM
	int Axes[] = { ACSC_AXIS_0, ACSC_AXIS_1, ACSC_AXIS_4, ACSC_AXIS_5, -1 };
	if (!acsc_DisableM(Handle, // communication handle
	Axes, // disable axes 0, 1, 4 and 5
	NULL// waiting call
	))
	{
	printf("transaction error: %d\n"), acsc_GetLastError());
	}
	*/
	int State= 0;

	if (!acsc_GetMotorState(Handle, 
		axis,// axis 0
		&State, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetMotorState transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	if((State & ACSC_MST_ENABLE) == 0)
		return false;
	return true;
}
//===================================================================================
bool CACS_Motion::isAlarm(int axis) //알람시 TRUE return
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	/*// example of the waiting call of acsc_DisableM
	int Axes[] = { ACSC_AXIS_0, ACSC_AXIS_1, ACSC_AXIS_4, ACSC_AXIS_5, -1 };
	if (!acsc_DisableM(Handle, // communication handle
	Axes, // disable axes 0, 1, 4 and 5
	NULL// waiting call
	))
	{
	printf("transaction error: %d\n"), acsc_GetLastError());
	}
	*/
	int State= 0;

/*
	if(axis== _ACS_AXIS_ROLL_Z1 || axis== _ACS_AXIS_ROLL_Z2)
	{
		acsc_GetSafetyInputPort
			(Handle, 
			axis,// axis 0
			&State, // received value
			NULL// waiting call
			);
		if(State!= 0)
		{
			if ((State & ACSC_SAFETY_DRIVE)== 0)		return false;
		}
	}
	else
	{
*/
/*
		if (!acsc_GetFault(Handle, 
			axis,// axis 0
			&State, // received value
			NULL// waiting call
			))
		{
			strLog.Format(_T("[CACS_Motion] acsc_GetMotorState transaction error: %d\n"), acsc_GetLastError());
			Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		if(State!= 0)
		{
			CString stra;
			stra.Format(_T("%d=[0x%X]       "), axis, State);
			TRACE(stra.GetBuffer(0));

			if (State & ACSC_SAFETY_DRIVE)		return true;
			return false;
		}
*/
	acsc_GetSafetyInputPort
		(Handle, 
		axis,// axis 0
		&State, // received value
		NULL// waiting call
		);
	if(State!= 0)
	{
		//#define ACSC_SAFETY_DRIVE					0x00000200
		if ((State & ACSC_SAFETY_DRIVE)!= 0)		return true;
	}

/*
	}
*/
//	if((State & ACSC_MST_ENABLE) == 0)
//	return isServoOn(axis);

	return false;
}
//===================================================================================
bool CACS_Motion::isELN_SensorCheck(int axis) //알람시 TRUE return
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int State= 0;

	if (!acsc_GetFault(Handle, 
		axis,// axis 0
		&State, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetMotorState transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	if(State!= 0)
	{
		if (State & ACSC_SAFETY_LL)		return true;
		return false;
	}
	return false;
}
//===================================================================================
bool CACS_Motion::isELP_SensorCheck(int axis) //알람시 TRUE return
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int State= 0;
	
	if (!acsc_GetFault(Handle, 
		axis,// axis 0
		&State, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetMotorState transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	if(State!= 0)
	{
		if (State & ACSC_SAFETY_RL)		return true;
		return false;
	}
	return false;
}

//===================================================================================
bool CACS_Motion::JogStart(int axis, long dir, double spd)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	double MoveDir= 1.0;

	if(dir== _DIR_CCW) MoveDir= -1.0;
	
	if (!acsc_Jog(Handle, // communication handle
		//0, //
		ACSC_AMF_VELOCITY, // start up immediately the jog
		axis, // axis 0
		spd*MoveDir, // to the negative direction
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_Jog transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::JogStart_Dual(int axis1, int axis2, long dir1, long dir2, double spd)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	

	int MoveDir1, MoveDir2;
	if(dir1== _DIR_CCW)	MoveDir1= ACSC_NEGATIVE_DIRECTION;
	else				MoveDir1= ACSC_POSITIVE_DIRECTION;
	if(dir2== _DIR_CCW)	MoveDir2= ACSC_NEGATIVE_DIRECTION;
	else				MoveDir2= ACSC_POSITIVE_DIRECTION;

	int Axes[] = { axis1, axis2, -1 };
	int Directions[] = { MoveDir1, MoveDir2};

	if (!acsc_JogM(Handle, // communication handle
		ACSC_AMF_VELOCITY,// start up immediately the jog
		Axes, // axis 0
		Directions,
		spd, // to the negative direction
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_JogM transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
/*
//===================================================================================
bool CACS_Motion::AlignINC_Move(int axisT, int axis, double dist, double spd)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;


	return true;
}
*/

//===================================================================================
bool CACS_Motion::JogStop_Dual(int axis1, int axis2)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	

	int Axes[] = { axis1, axis2, -1 };
	if (!acsc_HaltM(Handle, // communication handle
		Axes, // of the axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_HaltM[ACSC_AMF_RELATIVE] transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::MoveToABS(int axis, double Tomm)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	if (!acsc_ToPoint(Handle, // communication handle
		0, // start up immediately the motion
		axis, // of the axis 0
		Tomm, // to the absolute target point 10000
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ToPoint transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	Sleep(1);
	return true;
}
//===================================================================================
bool CACS_Motion::MoveToINC(int axis, double Tomm)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	if (!acsc_ToPoint(Handle, // communication handle
		ACSC_AMF_RELATIVE, // start up immediately the motion
		axis, // of the axis 0
		Tomm, // to the absolute target point 10000
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ToPoint[ACSC_AMF_RELATIVE] transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return false;
	}
	Sleep(1);
	return true;
}

//===================================================================================
bool CACS_Motion::MoveToABS_Dual(int axis1, int axis2, double Tomm1, double Tomm2)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int Axes[] = { axis1, axis2, -1 };
	double Points[] = {Tomm1, Tomm2};		

	if (!acsc_ToPointM(Handle, // communication handle
		0, // start up immediately the motion
		Axes, // of the axis 0
		Points, // to the absolute target point 10000
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ToPoint transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	Sleep(1);
	return true;
}
//===================================================================================
bool CACS_Motion::MoveToINC_Dual(int axis1, int axis2, double Tomm1, double Tomm2)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int Axes[] = { axis1, axis2, -1 };
	double Points[] = {Tomm1, Tomm2};		
	
	if (!acsc_ToPointM(Handle, // communication handle
		ACSC_AMF_RELATIVE, // start up immediately the motion
		Axes, // of the axis 0
		Points, // to the absolute target point 10000
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ToPoint transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	Sleep(1);
	return true;
}

//===================================================================================
bool CACS_Motion::MoveStop(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	if (!acsc_Halt(Handle, // communication handle
		axis, // of the axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_Halt[ACSC_AMF_RELATIVE] transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::MoveAllStop()
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int Axes[] = { ACSC_AXIS_0, ACSC_AXIS_1, ACSC_AXIS_2, ACSC_AXIS_3, ACSC_AXIS_4, ACSC_AXIS_6, ACSC_AXIS_7, -1 };
	if (!acsc_HaltM(Handle, // communication handle
		Axes, // of the axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_HaltM[ACSC_AMF_RELATIVE] transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::EmgStop(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	if (!acsc_Kill(Handle, // communication handle
		axis, // of the axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_Kill[ACSC_AMF_RELATIVE] transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}
//===================================================================================
bool CACS_Motion::EmgAllStop()
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int Axes[] = { ACSC_AXIS_0, ACSC_AXIS_1, ACSC_AXIS_2, ACSC_AXIS_3, ACSC_AXIS_4, ACSC_AXIS_6, ACSC_AXIS_7, -1 };
	if (!acsc_KillM(Handle, // communication handle
		Axes, // of the axis 0
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_KillM[ACSC_AMF_RELATIVE] transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
	return true;
}

//===================================================================================
bool CACS_Motion::InputBit(long no)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	long in_no = no - _ACS_INOUT_START_NO;
	int State;

	if     (no<= _ACS_INOUT_MC4U_END_NO)	//_ACS_INOUT_MC4U_IO_PORT_ADDR #define _ACS_INOUT_MC4U_START_NO		0 : #define _ACS_INOUT_MC4U_END_NO			7
	{
		if (!acsc_GetInput(Handle, 	_ACS_INOUT_MC4U_IO_PORT_ADDR, no - _ACS_INOUT_MC4U_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] InputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
	else if(no<= _ACS_INOUT_UDilt1_END_NO)	//_ACS_INOUT_UDilt2_IO_PORT_ADDR #define _ACS_INOUT_UDilt1_START_NO		8 : #define _ACS_INOUT_UDilt1_END_NO		15
	{
		if (!acsc_GetInput(Handle, 	_ACS_INOUT_UDilt1_IO_PORT_ADDR, no - _ACS_INOUT_UDilt1_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] InputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
	else if(no<= _ACS_INOUT_UDilt2_END_NO)	//_ACS_INOUT_UDilt2_IO_PORT_ADDR #define _ACS_INOUT_UDilt2_START_NO		16 : #define _ACS_INOUT_UDilt2_END_NO		23
	{
		if (!acsc_GetInput(Handle, 	_ACS_INOUT_UDilt2_IO_PORT_ADDR, no - _ACS_INOUT_UDilt2_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] InputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	else if(no<= _ACS_INOUT_UDilt3_END_NO)	//_ACS_INOUT_UDilt3_IO_PORT_ADDR #define _ACS_INOUT_UDilt3_START_NO		24 : #define _ACS_INOUT_UDilt3_END_NO		31
	{
		if (!acsc_GetInput(Handle, 	_ACS_INOUT_UDilt3_IO_PORT_ADDR, no - _ACS_INOUT_UDilt3_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] InputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
#endif
	else
	{
		int val = 1;
		int retval = 0;
		if(no > _ACS_INOUT_DIO1_END_NO)	
		{
				retval= Get_Int_Val(_T("EIN2"), 0);
				val = val << (no - _ACS_INOUT_DIO2_START_NO);
		}
		else
		{
			retval= Get_Int_Val(_T("EIN1"), 0);
			val = val << (no - _ACS_INOUT_DIO1_START_NO);
		}
		if ((retval & val) == 0)						return FALSE;
		else										return TRUE;
	}
	return false;
}
//===================================================================================
bool CACS_Motion::isOutputBit(long no)
//-----------------------------------------------------------------------------------
{
	if (!ACS_Motion_Initial)	return false;
	CString strLog;
	long out_no = no - _ACS_INOUT_START_NO;
	int State;
	if     (no<= _ACS_INOUT_MC4U_END_NO)	//_ACS_INOUT_MC4U_IO_PORT_ADDR #define _ACS_INOUT_MC4U_START_NO		0 : #define _ACS_INOUT_MC4U_END_NO			7
	{
		if (!acsc_GetOutput(Handle, 	_ACS_INOUT_MC4U_IO_PORT_ADDR, no - _ACS_INOUT_MC4U_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
	else if(no<= _ACS_INOUT_UDilt1_END_NO)	//_ACS_INOUT_UDilt2_IO_PORT_ADDR #define _ACS_INOUT_UDilt1_START_NO		8 : #define _ACS_INOUT_UDilt1_END_NO		15
	{
		if (!acsc_GetOutput(Handle, 	_ACS_INOUT_UDilt1_IO_PORT_ADDR, no - _ACS_INOUT_UDilt1_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
	else if(no<= _ACS_INOUT_UDilt2_END_NO)	//_ACS_INOUT_UDilt2_IO_PORT_ADDR #define _ACS_INOUT_UDilt2_START_NO		16 : #define _ACS_INOUT_UDilt2_END_NO		23
	{
		if (!acsc_GetOutput(Handle, 	_ACS_INOUT_UDilt2_IO_PORT_ADDR, no - _ACS_INOUT_UDilt2_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	else if(no<= _ACS_INOUT_UDilt3_END_NO)	//_ACS_INOUT_UDilt3_IO_PORT_ADDR #define _ACS_INOUT_UDilt3_START_NO		24 : #define _ACS_INOUT_UDilt3_END_NO		31
	{
		if (!acsc_GetOutput(Handle, 	_ACS_INOUT_UDilt3_IO_PORT_ADDR, no - _ACS_INOUT_UDilt3_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
	}
#endif
	else 	//_ACS_INOUT_DIO1_IO_PORT_ADDR #define _ACS_INOUT_DIO1_START_NO		48 : #define _ACS_INOUT_DIO1_END_NO			63
	{
		int ret= 0;
		int bino= 1;
		if(no > _ACS_INOUT_DIO1_END_NO)
		{
				ret= Get_Int_Val(_T("EOUT2"), 0);
				bino= bino << (no - _ACS_INOUT_DIO2_START_NO);//36
		}
		else
		{
				ret= Get_Int_Val(_T("EOUT1"), 0);
				bino= bino << (no - _ACS_INOUT_DIO1_START_NO);
	}
	if((ret & bino) > 0) return true;
	else return false;
/*

		if (!acsc_GetExtOutput(Handle, 	_ACS_INOUT_DIO1_IO_PORT_ADDR, no - _ACS_INOUT_DIO1_START_NO, &State, NULL ))
			//		if (!acsc_GetExtOutput(Handle, 	_ACS_INOUT_DIO1_IO_PORT_ADDR, no - _ACS_INOUT_DIO1_START_NO, &State, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		if(State) return true;		}
*/
	}
	return false;
}
//===================================================================================
bool CACS_Motion::OutputBit(long no, int flag)
//-----------------------------------------------------------------------------------
{
	if (!ACS_Motion_Initial)	return false;
	CString strLog;

	long out_no = no - _ACS_INOUT_START_NO;

	if     (no<= _ACS_INOUT_MC4U_END_NO)	//_ACS_INOUT_MC4U_IO_PORT_ADDR #define _ACS_INOUT_MC4U_START_NO		0 : #define _ACS_INOUT_MC4U_END_NO			7
	{
		if (!acsc_SetOutput(Handle, 	_ACS_INOUT_MC4U_IO_PORT_ADDR, no - _ACS_INOUT_MC4U_START_NO, flag, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		return true;		}
	}
	else if(no<= _ACS_INOUT_UDilt1_END_NO)	//_ACS_INOUT_UDilt2_IO_PORT_ADDR #define _ACS_INOUT_UDilt1_START_NO		8 : #define _ACS_INOUT_UDilt1_END_NO		15
	{
		if (!acsc_SetOutput(Handle, 	_ACS_INOUT_UDilt2_IO_PORT_ADDR, no - _ACS_INOUT_UDilt1_START_NO, flag, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		return true;		}
	}
	else if(no<= _ACS_INOUT_UDilt2_END_NO)	//_ACS_INOUT_UDilt2_IO_PORT_ADDR #define _ACS_INOUT_UDilt2_START_NO		16 : #define _ACS_INOUT_UDilt2_END_NO		23
	{
		if (!acsc_SetOutput(Handle, 	_ACS_INOUT_UDilt2_IO_PORT_ADDR, no - _ACS_INOUT_UDilt2_START_NO, flag, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		return true;		}
	}
#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	else if(no<= _ACS_INOUT_UDilt3_END_NO)	//_ACS_INOUT_UDilt3_IO_PORT_ADDR #define _ACS_INOUT_UDilt3_START_NO		24 : #define _ACS_INOUT_UDilt3_END_NO		31
	{
		if (!acsc_SetOutput(Handle, 	_ACS_INOUT_UDilt3_IO_PORT_ADDR, no - _ACS_INOUT_UDilt3_START_NO, flag, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		return true;		}
	}
#endif
	else //if(no < _BECHHOFF_INOUT_START_NO)
	{
		int ret= 0;
		int bino= 1;

		if(no > _ACS_INOUT_DIO1_END_NO)
		{
				ret= Get_Int_Val(_T("EOUT2"), 0);
				if(flag) 	{ bino= bino << (no - _ACS_INOUT_DIO2_START_NO); Set_Int_Val(_T("EOUT2"), 0, ret | bino); }
				else		{ bino= bino << (no - _ACS_INOUT_DIO2_START_NO); Set_Int_Val(_T("EOUT2"), 0, ret & (~bino)); }
		}
		else
		{
				ret= Get_Int_Val(_T("EOUT1"), 0);
				if(flag) 	{ bino= bino << (no - _ACS_INOUT_DIO1_START_NO); Set_Int_Val(_T("EOUT1"), 0, ret | bino); }
				else		{ bino= bino << (no - _ACS_INOUT_DIO1_START_NO); Set_Int_Val(_T("EOUT1"), 0, ret & (~bino)); }
		}
		//		bino= bino << (no - _ACS_INOUT_DIO1_START_NO);

		if((ret & bino) > 0) return true;
		else return false;
		return true;

		if (!acsc_SetExtOutput(Handle, 	_ACS_INOUT_DIO1_IO_PORT_ADDR, no - _ACS_INOUT_DIO1_START_NO, flag, NULL ))
		{
			strLog.Format(_T("[CACS_Motion] OutputBit transaction error: %d\n"), acsc_GetLastError());	Log()->SaveLogFile(_MOTION_LOG, strLog);
		}
		else	{		return true;		}
	}
	return true;
}

//===================================================================================
bool CACS_Motion::isDone(int axis)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	/*// example of the waiting call of acsc_DisableM
	int Axes[] = { ACSC_AXIS_0, ACSC_AXIS_1, ACSC_AXIS_4, ACSC_AXIS_5, -1 };
	if (!acsc_DisableM(Handle, // communication handle
	Axes, // disable axes 0, 1, 4 and 5
	NULL// waiting call
	))
	{
	printf("transaction error: %d\n"), acsc_GetLastError());
	}
	*/
	int MotorState= 0;
	int AxisState= 0;
	if (!acsc_GetMotorState(Handle, 
		axis,// axis 0
		&MotorState, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetMotorState transaction error: %d\n"), acsc_GetLastError());
	}
	if (!acsc_GetAxisState(Handle, 
		axis,// axis 0
		&AxisState, // received value
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_GetMotorState transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
	}
//	TRACE("MotorState=0x%X, AxisState=0x%X\n", MotorState, AxisState);

	if((MotorState & ACSC_MST_INPOS) && (MotorState & ACSC_AST_MOVE)== 0 && (AxisState & ACSC_AST_MOVE)== 0)
		return true;
	return false;
}
//===================================================================================
bool CACS_Motion::EndDone(int axis, int msTime)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	if (!acsc_WaitMotionEnd(Handle, // communication handle
		axis,// wait for the end of motion of axis 0
		msTime// during 30 sec
		))
	{
		strLog.Format(_T("[CACS_Motion] EndDone transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return false;
	}
	return true;
}
//===================================================================================
double  CACS_Motion::GetAD(long no)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	int State= 0;
// 	if (!acsc_GetAnalogInput(Handle, // communication handle
// 		no, // port 0
// 		&State,// received value
// 		NULL// waiting call
// 		))
// 	{
// 		strLog.Format(_T("[CACS_Motion] acsc_GetAnalogInput transaction error: %d\n"), acsc_GetLastError());
// 		Log()->SaveLogFile(_MOTION_LOG, strLog);
// 	}

 	double ret= 0.0;
 	ret= Get_Real_Val(_T("AIN"), no);
 	return ret;
/*
	double aaa= 0.0;
	CString strVal;
	strVal.Format("AD_RES_%d", no);
	if (!acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		strVal.GetBuffer(0), // variable name
		0, 0, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&aaa, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ReadReal transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	State= (long)(aaa);
*/

/*
	int aaa= 9;

	if (!acsc_ReadInteger(Handle, // communication handle
		ACSC_NONE, // standard variable
		"A_HomeState", // variable name
		0, 0, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&aaa, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		));
*/	
	int val = 0; 
	double  retval = Get_Real_Val(_T("rAD_DATA"), no);

	return retval;
}

double CACS_Motion::GetDiam(int no)
{
	double  retval = 0.0;
	
	retval= Get_Real_Val(_T("g_rDiameter"), no); 

	return retval;
}

BOOL CACS_Motion::isMapRun()
{
	int  retval = 0;

	retval= Get_Int_Val(_T("MAP_OK"), 0); 

	if(retval== 1)	return TRUE;
	return FALSE;
}
//===================================================================================
BOOL  CACS_Motion::BufferStop(long no)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	if (!acsc_StopBuffer( Handle, // communication handle
		no, // ACSPL+ program buffer number
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] BufferStop transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL  CACS_Motion::BufferRun(long no)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	BufferStop(no);

	if (!acsc_RunBuffer( Handle, // communication handle
		no, // ACSPL+ program buffer number
		NULL,
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] BufferRun transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return FALSE;
	}
	return TRUE;
}
//===================================================================================
int  CACS_Motion::Get_Int_Val(CString strVal, int index)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	USES_CONVERSION;
	int aaa= 0;
	if (!acsc_ReadInteger(Handle, // communication handle
		ACSC_NONE, // standard variable
		T2A(strVal.GetBuffer(0)), // variable name
		index, index, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&aaa, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ReadInteger transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	return aaa;
}
//===================================================================================
double  CACS_Motion::Get_Real_Val(CString strVal, int index)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	USES_CONVERSION;
	double aaa= 0.0;
	if (!acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		T2A(strVal.GetBuffer(0)), // variable name
		index, index, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&aaa, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ReadReal transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	return aaa;
}
//===================================================================================
int  CACS_Motion::Set_Int_Val(CString strVal, int index, int val)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	USES_CONVERSION;
	if (!acsc_WriteInteger(Handle, // communication handle
		ACSC_NONE, // standard variable
		T2A(strVal.GetBuffer(0)), // variable name
		index, index, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&val, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_WriteInteger transaction error: %d [%s]\n"), acsc_GetLastError(), strVal.GetBuffer(0));
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	return 1;
}
//===================================================================================
double  CACS_Motion::Set_Real_Val(CString strVal, int index, double val)
//-----------------------------------------------------------------------------------
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	USES_CONVERSION;
	double aaa= 0.0;
	if (!acsc_WriteReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		T2A(strVal.GetBuffer(0)), // variable name
		index, index, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&val, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_WriteReal transaction error: %d [%s]\n"), acsc_GetLastError(), strVal.GetBuffer(0));
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	return 1.0;
}

int	CACS_Motion::GetGetherdata(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5, double *Data6, double *Data7, double *Data8)
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	USES_CONVERSION;
	int aaa= 0;
	if (!acsc_ReadInteger(Handle, // communication handle
		ACSC_NONE, // standard variable
		"G_COUNT", // variable name
		0, 0, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&aaa, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ReadInteger transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"X_POS_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data1, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"X_VEL_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data2, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"Z1_INPUT_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data3, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"Z2_INPUT_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data4, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"ENC_POS_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data5, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"TIME_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data6, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"Z_POS", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data7, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"Z2_POS", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data8, // pointer to the buffer contained values
		NULL// waiting call
		);
	
	return aaa;
}

int	CACS_Motion::GetGetherSync_data(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5)
{
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	USES_CONVERSION;
	int aaa= 0;
	if (!acsc_ReadInteger(Handle, // communication handle
		ACSC_NONE, // standard variable
		"G_COUNT", // variable name
		0, 0, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		&aaa, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_ReadInteger transaction error: %d\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"X_POS_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data1, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"X_VEL_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data2, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"Z1_INPUT_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data3, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"Z2_INPUT_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data4, // pointer to the buffer contained values
		NULL// waiting call
		);
	acsc_ReadReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"ENC_POS_1", // variable name
		0, aaa, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Data5, // pointer to the buffer contained values
		NULL// waiting call
		);
	return aaa;
}

int	CACS_Motion::LoadMatData(double *Z1_Data, double *Z2_Data)
{
//	ERRORT(2048), ERRORY(2048)
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;
	
	int aaa= 0;

	USES_CONVERSION;
	Set_Int_Val(_T("MAP_NO"), 0, 1);

	if (!acsc_WriteReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"ERRORZ1", // variable name
		0, 600, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Z1_Data, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_WriteReal transaction error: %d [Mat-Z1]\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	if (!acsc_WriteReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"ERRORZ2", // variable name
		0, 600, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		Z2_Data, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_WriteReal transaction error: %d [Mat-Z2]\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	return 1;
}

int	CACS_Motion::LoadMatData_X(double *X_Pos_Data, double *X_Cal_Data, int count)
{
	//	ERRORT(2048), ERRORY(2048)
	if(!ACS_Motion_Initial || PROGRAM_EXIT== TRUE)	return false;
	CString strLog;

	int aaa= 0;

	USES_CONVERSION;
// 	Set_Int_Val(_T("MAP_NO"), 0, 1);

	if (!acsc_WriteReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"AXIS_X_COMMAND", // variable name
		0, 199, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		X_Pos_Data, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_WriteReal transaction error: %d [AXIS_X_COMMAND]\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	if (!acsc_WriteReal(Handle, // communication handle
		ACSC_NONE, // standard variable
		"X_CORRECTION_MAP", // variable name
		0, 199, // first dimension indexes
		ACSC_NONE, ACSC_NONE,// no second dimension
		X_Cal_Data, // pointer to the buffer contained values
		// that must be written
		NULL// waiting call
		))
	{
		strLog.Format(_T("[CACS_Motion] acsc_WriteReal transaction error: %d [X_CORRECTION_MAP]\n"), acsc_GetLastError());
		Log()->SaveLogFile(_MOTION_LOG, strLog);
		return -1;
	}
	return 1;
}
