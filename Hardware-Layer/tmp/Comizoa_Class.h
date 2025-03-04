// Comizoa_Class.h: interface for the CComizoa_Class class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMIZOA_CLASS_H__7986E517_8E98_40BC_BAC9_840A2B84F11C__INCLUDED_)
#define AFX_COMIZOA_CLASS_H__7986E517_8E98_40BC_BAC9_840A2B84F11C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParameterFile.h"

class CComizoa_Class  
{
public:


	bool	bSercoOnCheckFlag;
	#define IX_MAP_IDX	0

	bool	ReadyCheckFlagMsg;
	int		AxisHomeStat[16];
	long	lOutPulse_X[5];
	long	lOutPulse_T[5];
	long	lOutPulse_B[5];

	double	dOutPulse_X[5];
	double	dOutPulse_T[5];
	double	dOutPulse_B[5];

	BOOL	Z_ReadyPosCheck();

	BOOL	isOutputBit(long no);
	BOOL	OutputToggle(long no);
	BOOL	OutputBit(long no, long status);
	long	InputBit(long no);

	BOOL	Close();
	BOOL	Open();
	void	ServoOn(long no, long status);
	long	isServoOn(long no);
	void	ServoAllOn(long status);
	void	MotorAllStop();
	void	MotorAll_EMG_Stop();

	BOOL	JogStart(long axis, long dir, int spd);
	BOOL	JogStart_Z(long axis1, long axis2, long dir, int spd);
	BOOL	JogStop(long axis);
	BOOL	Jog_Z_Stop(long axis1, long axis2);
	BOOL	MoveStop(long axis);
	BOOL	Move_Z1_Z2_Stop(long axis1, long axis2);
	BOOL	MoveEmgStop(long axis);

	BOOL	INC_Move(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _NON_SYNC_MOVE);
	BOOL	ABS_Move(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL	ABS_MoveApply(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL	ABS_MoveRollApply(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL	ABS_Move_VelControl(int mode, long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL	ABS_Z_Offset_Move(double dist);
	BOOL	ABS_Move_Z(long axis1, long axis2,double dist, BOOL EndDoneCheck, int SyncMode = _SYNC_MOVE);
	BOOL	Ix_Move_X_ROLL(long axis1, long axis2,double dist, BOOL EndDoneCheck=FALSE, int SyncMode = _SYNC_MOVE);
	
	BOOL	isMotionInitial();
	BOOL	isIO_ReadyCheck();
	BOOL	isMotion_ReadyCheck(long nAxis);
	BOOL	isMotion_AlarmCheck();
	BOOL	isMotion_AlarmCheck(long nAxis);

	void	ResetPos(long nAxis);
	BOOL	isDone(long axis) ;
	BOOL	isHomeBusy(long axis);
	void	HomeSearchErrorProc();
	BOOL	HomeEndProc(long axis);
	BOOL	MotionHome(long axis);
	BOOL	isHomeReturnEheck(long axis, bool flag= false);
	BOOL	isAllMotorHomeEnd();
	
	double	Get_Position(long axis);

	void	MotionSpeedCheck(int mode, double speed_set) ;
	
	BOOL TopRollerFreeRun();
	BOOL RollerMotionHomeCheck(); // TRUE: 원점 실행

	BOOL PrintMoveStart();

//===================================================================================
	void EMG_Proc();
	BOOL isAIR_ON_Check();

	BOOL isEMO_Check();

	BOOL isPOWER_ON_Check();

	BOOL Vac_On();
	BOOL Vac_Off();

	BOOL isMotionReadyCheck();

	BOOL Z_Offset_Pos_Move(long axis, double dist, BOOL EndDoneCheck= TRUE);

	//===================================================================================
	BOOL isF_CYL_DOWN_ON_Check();
	BOOL isR_CYL_DOWN_ON_Check();
	BOOL isUL_CYL_FOR_ON_Check();
	BOOL isUL_CYL_BAK_ON_Check();
	BOOL UL_CYL_FOR_OnOff(int flag);
	BOOL UL_CYL_BAK_OnOff(int flag);
	BOOL BLOW_OnOff(int flag);


	CComizoa_Class();
	virtual ~CComizoa_Class();

	BOOL	InitMotionDevices();
	
private:
	CParameterFile* ModelFile();
	CParameterFile* Log();

	BOOL	COMI_Initial;
};

#endif // !defined(AFX_COMIZOA_CLASS_H__7986E517_8E98_40BC_BAC9_840A2B84F11C__INCLUDED_)
