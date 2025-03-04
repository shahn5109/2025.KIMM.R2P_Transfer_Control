// ACS_Motion.h: interface for the CACS_Motion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACS_MOTION_H__C1FF7CE7_F460_4AB6_92CB_681B842D816F__INCLUDED_)
#define AFX_ACS_MOTION_H__C1FF7CE7_F460_4AB6_92CB_681B842D816F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CACS_Motion  
{
public:
	CACS_Motion();
	virtual ~CACS_Motion();

	
	//===================================================================================
	TCHAR chrIP[255];
	int  PortNo;

	HANDLE	Handle;
	BOOL	ACS_Motion_Initial;
	//===================================================================================

	bool Init(CString strIP, int port);
	bool Close();

	BOOL isMapRun();

	bool ProcCancel();
	bool SetSpeed(int axis, double spd, double accspd, double decspd);
	bool SetFPosition(int axis, double setval);
	double GetFPosition(int axis);
	double GetFVelocity(int axis);
	bool SetRPosition(int axis, double setval);
	double GetRPosition(int axis);
	double GetAPosition(int axis);
	bool ServoOn(int axis);
	bool ServoOnAll();
	bool ServoOff(int axis);
	bool ServoOffAll();
	bool MoveToABS(int axis, double Tomm);
	bool MoveToINC(int axis, double Tomm);
	bool InputBit(long no);
	bool isOutputBit(long no);
	bool OutputBit(long no, int flag);
	bool isELN_SensorCheck(int axis); //알람시 TRUE return
	bool isELP_SensorCheck(int axis); //알람시 TRUE return

	double  GetAD(long no);

	bool MoveStop(int axis);
	bool MoveAllStop();
	bool EmgStop(int axis);
	bool EmgAllStop();

	bool isServoOn(int axis);
	bool isDone(int axis);
	//===================================================================================
	bool JogStart(int axis, long dir, double spd);
	bool JogStart_Dual(int axis1, int axis2, long dir1, long dir2, double spd);
	bool JogStop_Dual(int axis1, int axis2);
	//===================================================================================

	bool MoveToABS_Dual(int axis1, int axis2, double Tomm1, double Tomm2);
	bool MoveToINC_Dual(int axis1, int axis2, double Tomm1, double Tomm2);

	BOOL  BufferStop(long no);
	BOOL  BufferRun(long no);
	int  Get_Int_Val(CString strVal, int index);
	double  Get_Real_Val(CString strVal, int index);
	int  Set_Int_Val(CString strVal, int index, int val);
	double  Set_Real_Val(CString strVal, int index, double val);
	bool EndDone(int axis, int msTime);
//	int	GetGetherdata(double *PosX, double *VelX, double *LoadAD1, double *LoadAD2);
	int	GetGetherdata(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5, double *Data6, double *Data7, double *Data8);
	int	GetGetherSync_data(double *Data1, double *Data2, double *Data3, double *Data4, double *Data5);
	int	LoadMatData(double *Z1_Data, double *Z2_Data);
	int	LoadMatData_X(double *X_Pos_Data, double *X_Cal_Data, int count);

	bool isAlarm(int axis); //알람시 TRUE return
	double GetDiam(int no);

	int nNumSamples;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();
};

#endif // !defined(AFX_ACS_MOTION_H__C1FF7CE7_F460_4AB6_92CB_681B842D816F__INCLUDED_)
