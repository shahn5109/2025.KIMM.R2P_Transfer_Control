// IO.h: interface for the CIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IO_H__124D09DD_9DB1_498F_9650_7BB37FC0A145__INCLUDED_)
#define AFX_IO_H__124D09DD_9DB1_498F_9650_7BB37FC0A145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIO  
{
public:
	CIO();
	virtual ~CIO();


public:
	BOOL LoadCell_Reset();
	BOOL isCleaning_V_Open();
	void Cleaning_V_Open();
	void Cleaning_V_Close();

	BOOL isEMO_Check();
	BOOL isAir_Check();
	BOOL isMotorPower_Check();
	BOOL isPower_Check();
	BOOL MotorPower_On();
	BOOL MotorPower_Off();

	BOOL InputValveSel();
	BOOL OutputValveSel();
	BOOL VentValveOpen();
	BOOL VentValveClose();
	int  GetValvePos();
	BOOL Vac_On(int ch);
	BOOL Vac_Off(int ch);
	BOOL Vac_On();
	BOOL Vac_Off();
	BOOL isAIR_ON_Check();
	BOOL LVDT1_Cylinder_UP();
	BOOL LVDT1_Cylinder_DOWN();
	BOOL LVDT2_Cylinder_UP();
	BOOL LVDT2_Cylinder_DOWN();
	BOOL BLK_Roll_ElecCruch_ON();
	BOOL BLK_Roll_ElecCruch_OFF();
	BOOL RollWinderEndDone();

	void Vac_Switch_Check();
	bool On_Off_Check_Flag;

	void SetBuzzer(int flag);

	void BuzzerStop();
	void Buzzer_Error();
	void Buzzer_Start();
	BOOL isStage1_VacOn();
	BOOL isStage2_VacOn();
	BOOL isStage3_VacOn();

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

};

#endif // !defined(AFX_IO_H__124D09DD_9DB1_498F_9650_7BB37FC0A145__INCLUDED_)
