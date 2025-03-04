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
 

	bool bSercoOnCheckFlag;
	#define IX_MAP_IDX	0

	bool ReadyCheckFlagMsg;
	int AxisHomeStat[16];
	long	lOutPulse_X;
	long	lOutPulse_T;
	long	lOutPulse_B;

	double	dOutPulse_X;
	double	dOutPulse_T;
	double	dOutPulse_B;

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
	void	MotorZStop(int z_axis);
	void	MotorZ_EMG_Stop();
	void	MotorZ1Stop(int z_axis);
	void	MotorZ2Stop(int z_axis);

	BOOL	JogStart(long axis, long dir, int spd);
	BOOL	JogStart_Z(long axis, long dir, int spd, int SyncMode);
	BOOL	JogStop(long axis);
	BOOL	MoveStop(long axis);
	BOOL	MoveEmgStop(long axis);
	BOOL	Z_Axis_SYNC(int z_axis, long mode, double speed);

	BOOL	INC_Move(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL	ABS_Move(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL	ABS_Z_Offset_Move(int z_axis, double dist);

	BOOL	isMotionInitial();
	BOOL	isIO_ReadyCheck();
	BOOL	isMotion_ReadyCheck(long nAxis);
	BOOL	isMotion_AlarmCheck();
	BOOL	isMotion_AlarmCheck(long nAxis);

	void	ResetPos(long nAxis);
	BOOL	isDone(long axis) ;
	BOOL	isHomeBusy(long axis);
	void	HomeSearchErrorProc();
	BOOL	HomeEndProc(long axis) ;
	BOOL	MotionHome(long axis);
	BOOL	isHomeReturnEheck(long axis, bool flag= false);
	BOOL	isAllMotorHomeEnd();
	
	double	Get_Position(long axis);
	BOOL	Z_Axis_Check(int z_axis);
	BOOL	Z_Axis_Check(int z_axis, int *Z1Dir);

	void EMG_Proc();
	BOOL isAIR_ON_Check();

	void Power_OnOff(int flag);
	BOOL isPower_On();


//===================================================================================
	BOOL isEMO_Check();

	BOOL Vac_On();
	BOOL Vac_Off();
	BOOL Blow_On();
	BOOL Blow_Off();
//	BOOL ABS_Z_Wait_Pos_Move(long axis, double dist, BOOL EndDoneCheck);

	BOOL isMotionReadyCheck();

	BOOL Z_Offset_Pos_Move(long axis, double dist, BOOL EndDoneCheck= TRUE);

	CComizoa_Class();
	virtual ~CComizoa_Class();

	BOOL	InitMotionDevices();
	BOOL	isPOWER_ON_Check();
	//===================================================================================
	//===================================================================================
	void L1_Motor_Power_OnOff(int flag);
	void L2_Motor_Power_OnOff(int flag);
	BOOL isL1_Motor_Power_On();
	BOOL isL2_Motor_Power_On();
	//===================================================================================
	void InFeed_Cylinder_For(int flag);
	void InFeed_Cylinder_Bak(int flag);
	BOOL isInFeed_Cylinder_For();
	BOOL isInFeed_Cylinder_Bak();

	void OutFeed_Cylinder_For(int flag);
	void OutFeed_Cylinder_Bak(int flag);
	BOOL isOutFeed_Cylinder_For();
	BOOL isOutFeed_Cylinder_Bak();

	void L1_InkBox_Cylinder_Up(int flag);
	void L1_InkBox_Cylinder_Down(int flag);
	BOOL isL1_InkBox_Cylinder_Up();
	BOOL isL1_InkBox_Cylinder_Down();

	void L2_InkBox_Cylinder_Up(int flag);
	void L2_InkBox_Cylinder_Down(int flag);
	BOOL isL2_InkBox_Cylinder_Up();
	BOOL isL2_InkBox_Cylinder_Down();

	void L1_BladeMove_Cylinder_For(int flag);
	void L1_BladeMove_Cylinder_Bak(int flag);
	BOOL isL1_BladeMove_Cylinder_For();
	BOOL isL1_BladeMove_Cylinder_Bak();

	void L2_BladeMove_Cylinder_For(int flag);
	void L2_BladeMove_Cylinder_Bak(int flag);
	BOOL isL2_BladeMove_Cylinder_For();
	BOOL isL2_BladeMove_Cylinder_Bak();

	void L1_Blade_Cylinder_Up(int flag);
	void L1_Blade_Cylinder_Down(int flag);
	BOOL isL1_Blade_Cylinder_Up();
	BOOL isL1_Blade_Cylinder_Down();

	void L2_Blade_Cylinder_Up(int flag);
	void L2_Blade_Cylinder_Down(int flag);
	BOOL isL2_Blade_Cylinder_Up();
	BOOL isL2_Blade_Cylinder_Down();
	//===================================================================================
	//===================================================================================
	
private:
	CParameterFile* ModelFile();
	CParameterFile* Log();

	BOOL	COMI_Initial;
};

#endif // !defined(AFX_COMIZOA_CLASS_H__7986E517_8E98_40BC_BAC9_840A2B84F11C__INCLUDED_)
