// ParameterFile.h: interface for the CParameterFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETERFILE_H__757CC59E_6BF6_4732_BB32_5075502425F1__INCLUDED_)
#define AFX_PARAMETERFILE_H__757CC59E_6BF6_4732_BB32_5075502425F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define_structure.h"

class CParameterFile  
{
public:
	CParameterFile();
	virtual ~CParameterFile();
public:
//-----------------------------------------------------------------------------------------
//	char strCurrentModelName[255]; // ���� ������� �𵨸� ���
	TCHAR szLogFile[255];
	TCHAR strDirRootPath[255];				// ���α׷� ��Ʈ ���丮
	TCHAR strDirConfigPath[255];			// �ý��� Config ���丮
	TCHAR strDirProgramPath[255];			// ���� ���� ���丮
	TCHAR strDirSystemLogPath[255];			// �α� ���丮

	TCHAR strDirReportSavePath[255];		// �α� ���丮
	TCHAR strDirProcessReportSavePath[255];
	TCHAR strDirMeasImageSavePath[255];
	
	TCHAR strDirRecipePath[255];			// Model �� Recipe ���丮
	TCHAR strDirVisionLogPath[255];
	TCHAR strDirDefName[255];				// ���� ��Ÿ Define�� ���� ���丮
	TCHAR strDirImagePath[255];				// �̹��� ���� ���丮
	TCHAR strDirDebugImage[255];			// ����׿� �̹��� ���� ���丮

	double AD_0_Val;
	double AD_1_Val;
//-----------------------------------------------------------------------------------------
	TCHAR _MANAGER_PASSWORD[255];
	TCHAR _OPERATOR_PASSWORD[255];
//-----------------------------------------------------------------------------------------

	void SaveLogFile(int LogType, CString strText);
//	void TmpDataSave();
//	void TmpDataRead();

/*
	double ModelFind_LEFT_Angle;
	double ModelFind_LEFT_PosX;
	double ModelFind_LEFT_PosY;
	double ModelFind_RIGHT_Angle;
	double ModelFind_RIGHT_PosX;
	double ModelFind_RIGHT_PosY;
*/

//----------------------------------------------------------------------------------------- 
	USER_DATA		User_data;
	MOTION_DATA		Motion_data;
	MODEL_DATA		Model_data;
	SYSTEM_DATA		System_data;
	PATTERN_DATA	Pattern_data[_MAX_FIND_MODEL]; 

	SIOTABLE		Input_Table[DEF_IOMONITOR_MAX_IO_INPUT];
	SIOTABLE		Output_Table[DEF_IOMONITOR_MAX_IO_OUTPUT];
	RESULT_DATA		Align_Result_data[_MAX_ALIGN_RESULT_DATA];

	RECIPE_DATA		Recipe_data[_MAX_RECIPE];

	int			Measure_data_No;

	void AlignDataClear();
	void AlignDataClear(PROC_IDX no);
//-----------------------------------------------------------------------------------------
//	ConfigFile	
	void ReadFile(); // ReadInspParamFromFile(CString "c:\\Work\Setting\\aaa.vfd");)
	
	void ReadUserParamFile();
	void SaveUserParamFile();

	void ModelNameChangeFile(CString strName);
/*
	void SlotDieModelNameChangeFile(CString strName);
*/
	void SaveFile(CString strName);
	void OpenSytemConfigFile(); // �⺻���� �ý��� ���� �Ķ���� OpenSytemConfigFile(_T("Model1"));)
	void SaveSytemConfigFile(); // �⺻���� �ý��� ���� �Ķ���� SaveSytemConfigFile(_T("Model1"));)
	void OpenModelConfigFile(CString strName); // �⺻���� �ý��� ���� �Ķ���� OpenSytemConfigFile(_T("Model1"));)
	void SaveModelConfigFile(CString strName); // System_data.strCurrentModelName
	void SaveModelConfigFileSemi(CString strName);
	void ReadIOTableParamFile();

	void OpenRecipeConfigFile(CString strName); // System_data.strCurrentRecipeName
	void SaveRecipeConfigFile(CString strName); // System_data.strCurrentRecipeName

	void ReadResultDataFile();
	void SaveResultDataFile();
//-----------------------------------------------------------------------------------------
	void ReadMotionParamFile();
	void SaveMotionParamFile();
//-----------------------------------------------------------------------------------------
	void StartProcess_Report();
	void Process_Report();

	void CreratReportFolder();
	void Report_Save(int no, DWORD th, double pos, double vel, double ad1, double ad2, double ad11, double ad12, double ad21, double ad22);

//-----------------------------------------------------------------------------------------
};

#endif // !defined(AFX_PARAMETERFILE_H__757CC59E_6BF6_4732_BB32_5075502425F1__INCLUDED_)
