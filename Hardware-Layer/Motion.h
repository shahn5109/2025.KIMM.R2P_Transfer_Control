// Motion.h: interface for the CMotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTION_H__1B717053_7226_4721_86B2_D9F949B030EA__INCLUDED_)
#define AFX_MOTION_H__1B717053_7226_4721_86B2_D9F949B030EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ACS_Motion.h"
//#include "Comizoa_Class.h"

class CMotion  
{
public:
	CMotion();
	virtual ~CMotion();

	CACS_Motion		ACS_SYS;
//	CComizoa_Class	COMI_SYS;

	int AxisHomeStat[_MAX_AXIS];
	double CurPosition_Data[_MAX_AXIS];
	//int AxisHomeStat[16];
	double AD_OffsetVal[7];
	
	//===================================================================================

	BOOL	Motion_Initial;
	bool	ACS_DeviceOpenOK;
//	bool	COMI_DeviceOpenOK;
	//===================================================================================
	void AF_Proc_Start(int CamNo, double step, int count);

	CString GetMotionName(int axis);

	BOOL	Open();
	BOOL	Close();

	BOOL Syringe_Sync_Set();

	void CalAlignPos();

	BOOL isMapRun();

//	void Z_ControlEnable(int no);
//void Z_ControlDisable();
	BOOL isMotionServoOnCheck();
	BOOL isForceDoneCheck();
	BOOL X_R_LinearMoveStart(int stage_no);
	BOOL LinearMoveStop();

	BOOL DownLoadMAT_Data(double *Z1_Data, double *Z2_Data);
	BOOL DownLoadMatData_X(double *X_Pos_Data, double *X_Cal_Data, int count);

	BOOL  isMotionInitial();
	void  EMG_Proc();
	BOOL  isMotionReadyCheck();
	BOOL  isMotion_ReadyCheck(long nAxis, bool flag= true);
	BOOL  isMotion_AlarmCheck(long axis);
	BOOL  isMotion_AlarmCheck();
	BOOL  isDone(long axis); //완료 시 TRUE리턴
	BOOL  isHomeBusy(long axis); //동작중이면 TRUE리턴
	void  HomeSearchErrorProc(); 
	BOOL  HomeEndProc(long axis); 
	BOOL  isHomeReturnCheck(long axis, bool flag= false);
	BOOL  isAllMotorHomeEnd();
	BOOL  MotionHome(long axis);
	BOOL  OutputBit(long no, long status);
	BOOL  isOutputBit(long no);
	BOOL  OutputToggle(long no);
	long  InputBit(long no);
	void  ResetPos(long axis); 
	double  Get_Position(long axis);
	double  Get_APosition(long axis);
	void	Position_RealAll();
	double Get_Velocity(long axis);
	void  ServoOn(long axis);
	void  ServoOff(long axis);
	long  isServoOn(long axis);
	void  ServoAllOn();
	void  ServoAllOff();
	BOOL  MoveStop(long axis);
	BOOL  Move_EMG_Stop(long axis);
	void  MotorAllStop();
	void  MotorAll_EMG_Stop();

	BOOL  Z2Home_Offset_Send();
	BOOL  Z_Control_Mode_Send(int data); //0=Z1,Z2   1=Z1(Z2-Pass) 

	BOOL  JogStart(long axis, long dir, double spd);
	BOOL  JogStart_Dual(long axis1, long axis2, long dir, double spd);
	BOOL  Align_JogStart(long axis, long dir, double spd);

	BOOL  JogStart_SZ(long axis, long dir, double spd, int SyncMode= _SYNC_MOVE);

	BOOL  JogStop(long axis);
	BOOL  JogStop_Dual(long axis1, long axis2);
	BOOL  INC_Move(long axis, double dist, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  INC_MoveRollAlign(long axis, double dist, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);

	BOOL  INC_Move_SafeMode(long axis, double dist, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  INC_Move_Dual(long axis1, long axis2, double dist1,  double dist2, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  INC_Move_Dual_Speed(long axis1, long axis2, double dist1,  double dist2, double speed, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  Align_INC_Move(long axis, double dist, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  INC_Move_Speed(long axis, double dist, double speed, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  BRoll_INC_Move(long axis, double dist, int flag= _SPD_MODE, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  ABS_Move(long axis, double dist, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  ABS_Move_Speed(long axis, double dist, double speed, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  ABS_Move_Dual(long axis1, long axis2, double dist1, double dist2, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  ABS_Move_Dual_Speed(long axis1, long axis2, double dist1, double dist2, double run_speed, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);
	BOOL  ABS_Move_Dual_Offset(long axis1, long axis2, double dist1, double axis2_offset, BOOL EndDoneCheck= FALSE, int SyncMode= _SYNC_MOVE);

	BOOL SetDancerDataValue(double data, int no);
	double GetADValue(int no);
	BOOL OFF_Sync_Start();

	bool EndDone(long axis, int msTime);

	void HomeFlag_Clear();

	double radianToDegree(double rad);
	double degreeToRadian(double deg);

	BOOL Vac_OnOff(int flag);
	BOOL isVac_On();

	BOOL Sync_Start(int stage_no);
	void Sync_Stop();
	BOOL isSync_End();
	BOOL Z_ControlDisable();
	BOOL Grapg_Only_Start(int stage_no);

	int GetGetherData(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5, double *Data6, double *Data7, double *Data8);
	int GetGetherSyncData(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5);

	double  GetDiamValue(int no);

	void	Z_Mapping_Stop();
	BOOL	isORG_SensorCheck(long paAxis);
	BOOL	isELN_SensorCheck(long paAxis);
	BOOL	isELP_SensorCheck(long paAxis);
	BOOL	isALM_SensorCheck(long paAxis);

	BOOL	isStage_Check();
	double	LoadPosdata(long axis, int posNo);
	BOOL	Safe_PosCheck(int axis, int pos_no);
	BOOL	AlignStage_ReadyPos_Move(BOOL EndDoneCheck= FALSE);
	BOOL	BotVision_ReadyPos_Move(BOOL EndDoneCheck= FALSE);

	BOOL	Roll_Z_Down_Proc(int pos_no);
	double  PosCheckSWLimit(double val, int axis);
	double  PosOffsetCheckSWLimit(double val, int axis, double offset_val);
	BOOL	Z_Up_PosCheck();

	BOOL	isLimitPosCheck(int axis, double dist, bool flag);

	double Roll_GetmmToDeg(double Tomm);

	void GetStampPos(int CameraStageUsedNo, int lens_sel, double *L_X, double *Stage_Y, double *TopCamY, double *bot_X, double *bot_Y, double *Roll);
	void GetPickPlace_Pos(int Stage_no, double cur_x, double cur_y, double *tarPosX,  double *tarPosY );

	BOOL Z_ControlEnable(int stage_no);
	BOOL isZ_ControlEnd();
	BOOL Stamp_Start(int stage_no);
	BOOL RollForceControl_Start(double L_Force, double R_Force, int stage_no= 3);
	BOOL RollForceControl_Set(double L_Force, double R_Force);

	void Stamp_Stop();
	BOOL isStampc_End();

	BOOL Sync_Mode_Set(int stage_no);
	void Sync_Mode_Reset();

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

};

#endif // !defined(AFX_MOTION_H__1B717053_7226_4721_86B2_D9F949B030EA__INCLUDED_)
