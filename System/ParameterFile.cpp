// ParameterFile.cpp: implementation of the CParameterFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <iostream> 
#include <direct.h>	// Dos Command. //
#include "stdio.h"
#include "ParameterFile.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//===================================================================================
CParameterFile::CParameterFile()
//-----------------------------------------------------------------------------------
{

	_tcscpy(strDirRootPath,		_PROGRAM_ROOT_DIR);

#ifdef _KIMM_TRANSFER_VER1_SYSTEM_INFO_H
	_stprintf(strDirConfigPath,		_T("%s\\Config_Roll"),		strDirRootPath);
#else
	_stprintf(strDirConfigPath,		_T("%s\\Config_Plate"),		strDirRootPath);
#endif	
	_stprintf(strDirProgramPath,	_T("%s\\Bin"),			strDirRootPath);
	_stprintf(strDirSystemLogPath,	_T("%s\\Log_Process"),	strDirRootPath);

	_stprintf(strDirRecipePath,		_T("%s\\Recipe"),		strDirRootPath);
	_stprintf(strDirVisionLogPath,	_T("%s\\Log_Vision"),	strDirRootPath);
	_stprintf(strDirDefName,		_T("%s\\Report"),		strDirRootPath);
	_stprintf(strDirImagePath,		_T("%s\\Image"),		strDirDefName);
	_stprintf(strDirDebugImage,		_T("%s\\Debug_Image"),	strDirRootPath);

	_stprintf(strDirReportSavePath,	_T("%s\\Image"),		strDirDefName);

	_wmkdir(strDirRootPath); 
	_wmkdir(strDirConfigPath); 
	_wmkdir(strDirSystemLogPath);

	_wmkdir(strDirRecipePath);
	_wmkdir(strDirImagePath);
	_wmkdir(strDirDefName);
	_wmkdir(strDirVisionLogPath);
	_wmkdir(strDirDebugImage);

	//------------------------------------------

	SaveLogFile(_SYSTEM_LOG, _T("************************************************************************"));
	SaveLogFile(_SYSTEM_LOG, _T("********************       PROGRAM START !!!      **********************"));
	SaveLogFile(_SYSTEM_LOG, _T("************************************************************************"));

	System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_STOP;
	System_data.bEND_STOP_FLAG= _CURRENT_ENDSTOP_DIS;
	System_data.iPROCESS_CYCLE_COUNT= 1;
	System_data.iWORK_PROCESS_MODE= _PROCESS_AUTO_RUN;

	System_data.iPROCESS_RUNNING_COUNT= 0;


	AlignDataClear();

	_stprintf(User_data.strUserName, _T("%s"), _T(" "));
	_stprintf(User_data.strUseInk, _T("%s"), _T(" "));
	_stprintf(User_data.strUseBlanket, _T("%s"), _T(" "));
	_stprintf(User_data.strMemo, _T("%s"), _T(" "));
	User_data.lUseHour= 0;		
	User_data.lUseMinute= 0;
	User_data.lUseDay= 0;		
	User_data.lUseMon= 0;		
	User_data.lUseYear= 0;		
	User_data.dUseTemp= 0.0;		
	User_data.dUseHumidity= 0.0;

	ReadFile();
}

//===================================================================================
CParameterFile::~CParameterFile()
//-----------------------------------------------------------------------------------
{
}

void CParameterFile::AlignDataClear()
{
	int i, j;
	for(i= 0; i < _MAX_ALIGN_RESULT_DATA; i++)
	{
		Align_Result_data[i].dTheta= -999.999;

		Align_Result_data[i].dptPoint_1st.x= 0.0;	Align_Result_data[i].dptPoint_1st.y= 0.0;
		Align_Result_data[i].dptPoint_2nd.x= 0.0;	Align_Result_data[i].dptPoint_2nd.y= 0.0;
		Align_Result_data[i].dptRealPoint_1st.x= 0.0;		Align_Result_data[i].dptRealPoint_1st.y= 0.0;
		Align_Result_data[i].dptRealPoint_2nd.x= 0.0;		Align_Result_data[i].dptRealPoint_2nd.y= 0.0;

		Align_Result_data[i].dTheta= 0.0;
		Align_Result_data[i].dDistance= 0.0;

		Align_Result_data[i].iAlignEndFlag= 0;

		for(j= 0; j< _MAX_AXIS; j++) Align_Result_data[i].dPosData[j]= 0.0;
	}
}
void CParameterFile::AlignDataClear(PROC_IDX no)
{
	Align_Result_data[no].iAlignEndFlag= 0;
	Align_Result_data[no].dTheta= -999.999;

	Align_Result_data[no].dptPoint_1st.x= 0.0;		Align_Result_data[no].dptPoint_1st.y= 0.0;
	Align_Result_data[no].dptPoint_2nd.x= 0.0;		Align_Result_data[no].dptPoint_2nd.y= 0.0;
	Align_Result_data[no].dptRealPoint_1st.x= 0.0;	Align_Result_data[no].dptRealPoint_1st.y= 0.0;
	Align_Result_data[no].dptRealPoint_2nd.x= 0.0;	Align_Result_data[no].dptRealPoint_2nd.y= 0.0;

	Align_Result_data[no].dTheta= 0.0;
	Align_Result_data[no].dDistance= 0.0;
}
//===================================================================================
void CParameterFile::ReadFile() // 프로그램 최초에 1번 읽는것, 이전 모델명 읽기 위해
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500];
	TCHAR value[500];
//	int i, j, k;
	TCHAR strTmpName[256];
	TCHAR strSlotName[256], strProcessName[256];
	CString strText;
	TCHAR szParamFile[255];

	_stprintf(szParamFile, _T("%s\\Default.INI"), strDirConfigPath );
	_stprintf(buf, _T("Model_Name"));			GetPrivateProfileString(_T("Def_Name"), buf, _T("Model1"), strTmpName, sizeof(buf), szParamFile);
	_stprintf(buf, _T("SlotDieRecipe_Name"));	GetPrivateProfileString(_T("Recipe_1"), buf, _T("Default"), strSlotName, sizeof(buf), szParamFile);
	_stprintf(buf, _T("CurrentRecipeName"));	GetPrivateProfileString(_T("Recipe_1"), buf, _T("Default"), strProcessName, sizeof(buf), szParamFile);
	
	_stprintf(buf, _T("PassWord")); 	GetPrivateProfileString(_T("MANAGER MODE"), buf, _T("1111"), _MANAGER_PASSWORD, sizeof(buf), szParamFile);
	_stprintf(buf, _T("PassWord")); 	GetPrivateProfileString(_T("OPERATOR MODE"), buf, _T("1111"), _OPERATOR_PASSWORD, sizeof(buf), szParamFile);

	_stprintf(buf, _T("TestMode")); 	GetPrivateProfileString(_T("TEST MODE"), buf,  _T("0"), value, sizeof(value), szParamFile);
	int aaa= _wtoi(value);
	if(aaa) SIMULATION_MODE= TRUE;
	else	SIMULATION_MODE= FALSE;

	//-------------------------------
	_tcscpy(System_data.strCurrentModelName, strTmpName);
	_tcscpy(System_data.strCurrentSlotDieName, strSlotName);
	_tcscpy(System_data.strCurrentRecipeName, strProcessName);
	strText.Format(_T("[SYSTEM-DATA-READ] ReadFile() -> Complite! [MODEL-NAME: %s]"), strTmpName);	SaveLogFile(_SYSTEM_LOG, strText);
	//-------------------------------
	OpenSytemConfigFile();//System_data.strCurrentModelName);
	OpenModelConfigFile(System_data.strCurrentModelName);
	ReadMotionParamFile();//System_data.strCurrentModelName);
//	ReadUserParamFile();
//	ReadIOTableParamFile();
	ReadResultDataFile();
	//---------------------------------------
	FILE* fcheck = _wfopen(szParamFile, _T("r"));
	if(fcheck == NULL) 	{
		strText.Format(_T("[SYSTEM-DATA-INITIAL] ReadFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
		SaveFile(System_data.strCurrentModelName);	
	}
	else				fclose(fcheck);
}
//===================================================================================
void CParameterFile::ModelNameChangeFile(CString strName) //  // System_data.strCurrentModelName 현재 사용중인 모델명 저장
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500];
	TCHAR szParamFile[255];
	
	CString strText;
	
	_stprintf(szParamFile, _T("%s\\Default.INI"), strDirConfigPath );
	_tcscpy(System_data.strCurrentModelName, strName);
	
	_stprintf(value, _T("%s"), strName);								_stprintf(buf, _T("Model_Name"));			WritePrivateProfileString(_T("Def_Name"), buf, value, szParamFile);

	_stprintf(value, _T("%s"), _MANAGER_PASSWORD);	_stprintf(buf, _T("PassWord")); 	WritePrivateProfileString(_T("MANAGER MODE"), buf, value, szParamFile);
	_stprintf(value, _T("%s"), _OPERATOR_PASSWORD);	_stprintf(buf, _T("PassWord")); 	WritePrivateProfileString(_T("OPERATOR MODE"), buf, value, szParamFile);
	
	//-------------------------------
	strText.Format(_T("[SYSTEM-MODEL-CHANGE] ModelNameChangeFile() -> Complite! [MODEL-NAME: %s]"), System_data.strCurrentModelName);	SaveLogFile(_SYSTEM_LOG, strText);
	//-------------------------------
	//	SaveSytemConfigFile(System_data.strCurrentModelName);
//	ReadFile();
	//-------------------------------
}

//===================================================================================
void CParameterFile::SaveFile(CString strName) //  // System_data.strCurrentModelName 현재 사용중인 모델명 저장
//-----------------------------------------------------------------------------------
{
	CString strText;

	SaveSytemConfigFile();//System_data.strCurrentModelName);
	SaveModelConfigFile(System_data.strCurrentModelName);
	SaveMotionParamFile();//System_data.strCurrentModelName);

	strText.Format(_T("[DATA-SAVE] SaveFile() -> Complite! [MODEL-NAME: %s]"), System_data.strCurrentModelName);	SaveLogFile(_SYSTEM_LOG, strText);
	ReadFile();
}
//===================================================================================
void CParameterFile::OpenSytemConfigFile() // System_data.strCurrentModelName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[50];
	int i= 0, j= 0;

	CString strText;
	TCHAR szParamFile[255];

	_stprintf(szParamFile, _T("%s\\Default.sys_"), strDirConfigPath);
//	_stprintf(szParamFile, "%s\\%s.Model_", strDirConfigPath, strName );

	_stprintf(ReadModel, _T("SYSTEM_DATA"));

	_stprintf(buf, _T("_HOME_TIME_OUT"));			GetPrivateProfileString(ReadModel, buf, _T("100000"), value, sizeof(value), szParamFile);	System_data._HOME_TIME_OUT= _wtoi(value);
	_stprintf(buf, _T("_STEP_DELAY_TIME"));			GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);	System_data._STEP_DELAY_TIME= _wtoi(value);
	_stprintf(buf, _T("_CYL_AFTER_DELAY_TIME"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);	System_data._CYL_AFTER_DELAY_TIME= _wtoi(value);
	_stprintf(buf, _T("_AXIS_MOVE_TIME_OUT"));		GetPrivateProfileString(ReadModel, buf, _T("100000"), value, sizeof(value), szParamFile);	System_data._AXIS_MOVE_TIME_OUT= _wtoi(value);
	_stprintf(buf, _T("iVac_OFF_Delay"));			GetPrivateProfileString(ReadModel, buf, _T("100"), value, sizeof(value), szParamFile);	System_data.iVac_OFF_Delay= _wtoi(value);
	
	_stprintf(buf, _T("_ROLL_Z_AXIS_MIN_"));		GetPrivateProfileString(ReadModel, buf, _T("-3.0"), value, sizeof(value), szParamFile);		System_data._ROLL_Z_AXIS_MIN_= _wtof(value);
	_stprintf(buf, _T("_ROLL_Z_AXIS_MAX_"));		GetPrivateProfileString(ReadModel, buf, _T("8.0"), value, sizeof(value), szParamFile);		System_data._ROLL_Z_AXIS_MAX_ = _wtof(value);

	_stprintf(buf, _T("dDancer_1_ZeroVal"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dDancer_1_ZeroVal = _wtof(value);
	
	_stprintf(buf, _T("dTop_Cam_PixelToum_2X"));	GetPrivateProfileString(ReadModel, buf, _T("1.0"), value, sizeof(value), szParamFile);	System_data.dTop_Cam_PixelToum_2X = _wtof(value);
	_stprintf(buf, _T("dTop_Cam_PixelToum_20X"));	GetPrivateProfileString(ReadModel, buf, _T("1.0"), value, sizeof(value), szParamFile);	System_data.dTop_Cam_PixelToum_20X = _wtof(value);
	_stprintf(buf, _T("dBot_Cam_PixelToum_2X"));	GetPrivateProfileString(ReadModel, buf, _T("1.0"), value, sizeof(value), szParamFile);	System_data.dBot_Cam_PixelToum_2X = _wtof(value);
	_stprintf(buf, _T("dBot_Cam_PixelToum_20X"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dBot_Cam_PixelToum_20X = _wtof(value);
	_stprintf(buf, _T("iImageGrab_Delay"));			GetPrivateProfileString(ReadModel, buf, _T("100"), value, sizeof(value), szParamFile);	System_data.iImageGrab_Delay = _wtoi(value);

	_stprintf(buf, _T("dTopCam20To2_X"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dTopCam20To2_X = _wtof(value);
	_stprintf(buf, _T("dTopCam20To2_Y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dTopCam20To2_Y = _wtof(value);
	_stprintf(buf, _T("dTopCam20To2_Z"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dTopCam20To2_Z = _wtof(value);
	_stprintf(buf, _T("dBotCam20To2_X"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dBotCam20To2_X = _wtof(value);
	_stprintf(buf, _T("dBotCam20To2_Y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dBotCam20To2_Y = _wtof(value);
	_stprintf(buf, _T("dBotCam20To2_Z"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dBotCam20To2_Z = _wtof(value);
	_stprintf(buf, _T("dBot_Cam_20X_Offset_X"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dBot_Cam_20X_Offset_X = _wtof(value);
	_stprintf(buf, _T("dBot_Cam_20X_Offset_Y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dBot_Cam_20X_Offset_Y = _wtof(value);

	_stprintf(buf, _T("dRotCen_PosStageX_20X"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dRotCen_PosStageX_20X = _wtof(value);
	_stprintf(buf, _T("dRotCen_PosCamY_20X"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	System_data.dRotCen_PosCamY_20X = _wtof(value);

	_stprintf(buf, _T("dmmToDeg"));		GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.dmmToDeg = _wtof(value);

	_stprintf(buf, _T("X_SAFE_FREEZONE_L"));	GetPrivateProfileString(ReadModel, buf, _T("-999.0"), value, sizeof(value), szParamFile);	System_data.X_SAFE_FREEZONE_L = _wtof(value);
	_stprintf(buf, _T("X_SAFE_FREEZONE_H"));	GetPrivateProfileString(ReadModel, buf, _T("100.0"), value, sizeof(value), szParamFile);	System_data.X_SAFE_FREEZONE_H = _wtof(value);
	_stprintf(buf, _T("X_SAFE_ZONE_L1"));		GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.X_SAFE_ZONE_L1 = _wtof(value);
	_stprintf(buf, _T("X_SAFE_ZONE_H1"));		GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.X_SAFE_ZONE_H1 = _wtof(value);
	_stprintf(buf, _T("X_SAFE_ZONE_L2"));		GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.X_SAFE_ZONE_L2 = _wtof(value);
	_stprintf(buf, _T("X_SAFE_ZONE_H2"));		GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.X_SAFE_ZONE_H2 = _wtof(value);
	_stprintf(buf, _T("X_MASK_VIEWZONE_L"));	GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.X_MASK_VIEWZONE_L = _wtof(value);
	_stprintf(buf, _T("X_MASK_VIEWZONE_H"));	GetPrivateProfileString(ReadModel, buf, _T("3.5597876"), value, sizeof(value), szParamFile);	System_data.X_MASK_VIEWZONE_H = _wtof(value);
	_stprintf(buf, _T("dRollAlignPos_H"));		GetPrivateProfileString(ReadModel, buf, _T("350.0"), value, sizeof(value), szParamFile);	System_data.dRollAlignPos_H = _wtof(value);
	_stprintf(buf, _T("dRollAlignPos_L"));		GetPrivateProfileString(ReadModel, buf, _T("340.0"), value, sizeof(value), szParamFile);	System_data.dRollAlignPos_L = _wtof(value);
	
	_stprintf(buf, _T("dX_Axis_Interlock"));	GetPrivateProfileString(ReadModel, buf, _T("20.0"), value, sizeof(value), szParamFile);		System_data.dX_Axis_Interlock = _wtof(value);
	_stprintf(buf, _T("dQuartz_PosX"));			GetPrivateProfileString(ReadModel, buf, _T("300.0"), value, sizeof(value), szParamFile);	System_data.dQuartz_PosX = _wtof(value);
	_stprintf(buf, _T("dQuartz_PosY"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dQuartz_PosY = _wtof(value);
	_stprintf(buf, _T("dRollPlateStamp_PosX"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dRollPlateStamp_PosX = _wtof(value);
	_stprintf(buf, _T("dRollPlateStamp_PosY"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dRollPlateStamp_PosY = _wtof(value);
	_stprintf(buf, _T("dStamp_Y_Max"));	GetPrivateProfileString(ReadModel, buf, _T("22.5"), value, sizeof(value), szParamFile);		System_data.dStamp_Y_Max = _wtof(value);
	_stprintf(buf, _T("dShift_CamY"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dShift_CamY = _wtof(value);
	_stprintf(buf, _T("dStampMoveOffset_X"));	GetPrivateProfileString(ReadModel, buf, _T("0.041"), value, sizeof(value), szParamFile);		System_data.dStampMoveOffset_X = _wtof(value);
	_stprintf(buf, _T("dStampMoveOffset_Y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dStampMoveOffset_Y = _wtof(value);

	_stprintf(buf, _T("AF_Stepmm_2X"));			GetPrivateProfileString(ReadModel, buf, _T("0.03"), value, sizeof(value), szParamFile);		System_data.AF_Stepmm_2X = _wtof(value);
	_stprintf(buf, _T("AF_Stepmm_20X"));		GetPrivateProfileString(ReadModel, buf, _T("0.002"), value, sizeof(value), szParamFile);	System_data.AF_Stepmm_20X = _wtof(value);
	_stprintf(buf, _T("iAF_TotalCount"));		GetPrivateProfileString(ReadModel, buf, _T("48"), value, sizeof(value), szParamFile);	System_data.iAF_TotalCount = _wtoi(value);

	_stprintf(buf, _T("dStage1ToStage2_Dist"));			GetPrivateProfileString(ReadModel, buf, _T("100.0"), value, sizeof(value), szParamFile);		System_data.dStage1ToStage2_Dist = _wtof(value);
	_stprintf(buf, _T("dStage2ToStage3_Dist"));			GetPrivateProfileString(ReadModel, buf, _T("100.0"), value, sizeof(value), szParamFile);		System_data.dStage2ToStage3_Dist = _wtof(value);

	_stprintf(buf, _T("dBasePos_Stage1_X"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dBasePos_Stage1_X = _wtof(value);
	_stprintf(buf, _T("dBasePos_Stage1_CamY"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dBasePos_Stage1_CamY = _wtof(value);
	_stprintf(buf, _T("dBasePos_Stage2_X"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dBasePos_Stage2_X = _wtof(value);
	_stprintf(buf, _T("dBasePos_Stage2_CamY"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dBasePos_Stage2_CamY = _wtof(value);
	_stprintf(buf, _T("dBasePos_Stage3_X"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dBasePos_Stage3_X = _wtof(value);
	_stprintf(buf, _T("dBasePos_Stage3_CamY"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dBasePos_Stage3_CamY = _wtof(value);

	_stprintf(buf, _T("iCAM_20X_USED_FLAG"));			GetPrivateProfileString(ReadModel, buf, _T("1"), value, sizeof(value), szParamFile);		System_data.iCAM_20X_USED_FLAG = _wtoi(value);
	_stprintf(buf, _T("dHome_Offset_BX"));				GetPrivateProfileString(ReadModel, buf, _T("5.7765"), value, sizeof(value), szParamFile);			System_data.dHome_Offset_BX = _wtof(value);

	_stprintf(buf, _T("TB_Offset_2X_X"));				GetPrivateProfileString(ReadModel, buf, _T("0.000"), value, sizeof(value), szParamFile);		System_data.TB_Offset_2X_X = _wtof(value);
// 	_stprintf(buf, _T("Roll_Offset_2X"));			GetPrivateProfileString(ReadModel, buf, _T("0.024"), value, sizeof(value), szParamFile);		System_data.Roll_Offset_2X = _wtof(value);
// 	_stprintf(buf, _T("Q_Offset_20X"));				GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);			System_data.Q_Offset_20X = _wtof(value);
// 	_stprintf(buf, _T("Roll_Offset_20X"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);			System_data.Roll_Offset_20X = _wtof(value);
	_stprintf(buf, _T("TB_Offset_2X_Y"));				GetPrivateProfileString(ReadModel, buf, _T("0.000"), value, sizeof(value), szParamFile);		System_data.TB_Offset_2X_Y = _wtof(value);
// 	_stprintf(buf, _T("Roll_Offset_2X_Y"));			GetPrivateProfileString(ReadModel, buf, _T("0.024"), value, sizeof(value), szParamFile);		System_data.Roll_Offset_2X_Y = _wtof(value);
// 	_stprintf(buf, _T("Q_Offset_20X_Y"));				GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);			System_data.Q_Offset_20X_Y = _wtof(value);
// 	_stprintf(buf, _T("Roll_Offset_20X_Y"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);			System_data.Roll_Offset_20X_Y = _wtof(value);

	_stprintf(buf, _T("RollAlignPosMode"));			GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);		System_data.RollAlignPosMode = _wtoi(value);
	_stprintf(buf, _T("Led_Remote"));				GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);		System_data.Led_Remote = _wtoi(value);

	_stprintf(buf, _T("dKgPerum"));					GetPrivateProfileString(ReadModel, buf, _T("10.000"), value, sizeof(value), szParamFile);		System_data.dKgPerum = _wtof(value);
	_stprintf(buf, _T("dExcept_XPos"));				GetPrivateProfileString(ReadModel, buf, _T("0.000"), value, sizeof(value), szParamFile);		System_data.dExcept_XPos = _wtof(value);
	_stprintf(buf, _T("dExcept_XPos2"));			GetPrivateProfileString(ReadModel, buf, _T("0.000"), value, sizeof(value), szParamFile);		System_data.dExcept_XPos2 = _wtof(value);
	_stprintf(buf, _T("dRollAlign_ZPos"));			GetPrivateProfileString(ReadModel, buf, _T("9.000"), value, sizeof(value), szParamFile);		System_data.dRollAlign_ZPos = _wtof(value);
	_stprintf(buf, _T("iRollTeachingDataUsed"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);		System_data.iRollTeachingDataUsed = _wtoi(value);
	_stprintf(buf, _T("dRectSizeX"));	GetPrivateProfileString(ReadModel, buf, _T("0.1"), value, sizeof(value), szParamFile);		System_data.dRectSizeX = _wtof(value);
	_stprintf(buf, _T("dRectSizeY"));	GetPrivateProfileString(ReadModel, buf, _T("0.1"), value, sizeof(value), szParamFile);		System_data.dRectSizeY = _wtof(value);
	
	_stprintf(buf, _T("dZ2HomeOffsetVal"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);		System_data.dZ2HomeOffsetVal = _wtof(value);

	_stprintf(buf, _T("dCompRectSize"));	GetPrivateProfileString(ReadModel, buf, _T("5.0"), value, sizeof(value), szParamFile);		System_data.dCompRectSize = _wtof(value);
	_stprintf(buf, _T("dCompPitch"));	GetPrivateProfileString(ReadModel, buf, _T("5.0"), value, sizeof(value), szParamFile);		System_data.dCompPitch = _wtof(value);
	_stprintf(buf, _T("iCompCountX"));	GetPrivateProfileString(ReadModel, buf, _T("16"), value, sizeof(value), szParamFile);		System_data.iCompCountX = _wtoi(value);
	_stprintf(buf, _T("iCompCountY"));	GetPrivateProfileString(ReadModel, buf, _T("12"), value, sizeof(value), szParamFile);		System_data.iCompCountY = _wtoi(value);
	_stprintf(buf, _T("bRollAF_Enable"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);		System_data.bRollAF_Enable = _wtoi(value);
	
	for(i= 0; i< _MAX_RUN_PARAM; i++) 
	{
		_stprintf(buf, _T("dRoll_Diameter_%d"), i);			GetPrivateProfileString(ReadModel, buf, _T("200.0"),  value, sizeof(value), szParamFile);	System_data.dRoll_Diameter[i]= _wtof(value);
		_stprintf(buf, _T("dRoll_Diameter_AddVal_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile);		System_data.dRoll_Diameter_AddVal[i]= _wtof(value);
		_stprintf(buf, _T("dZ_Base_Position_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("6.0"),  value, sizeof(value), szParamFile);		System_data.dZ_Base_Position[i]= _wtof(value);
		_stprintf(buf, _T("iLED_Top_Val_2X_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("10"), value, sizeof(value), szParamFile);		System_data.iLED_Top_Val_2X[i] = _wtoi(value);
		_stprintf(buf, _T("iLED_Btm_Val_2X_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("10"), value, sizeof(value), szParamFile);		System_data.iLED_Btm_Val_2X[i] = _wtoi(value);
		_stprintf(buf, _T("iLED_Top_Val_20X_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("10"), value, sizeof(value), szParamFile);		System_data.iLED_Top_Val_20X[i] = _wtoi(value);
		_stprintf(buf, _T("iLED_Btm_Val_20X_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("10"), value, sizeof(value), szParamFile);		System_data.iLED_Btm_Val_20X[i] = _wtoi(value);
	}

	//-------------------------------
	strText.Format(_T("[SYSTEM-DATA-READ] OpenSytemConfigFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
}
//===================================================================================
void CParameterFile::SaveSytemConfigFile() // System_data.strCurrentModelName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	CString strText;
	TCHAR szParamFile[255];
	int i= 0, j= 0;

	_stprintf(szParamFile, _T("%s\\Default.sys_"), strDirConfigPath);
	
	_stprintf(ReadModel, _T("SYSTEM_DATA"));
//	_stprintf(value, _T("%d"), System_data.iRun_Mode);	_stprintf(buf, _T("Run_Mode"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data._ROLL_Z_AXIS_MIN_);		_stprintf(buf, _T("_ROLL_Z_AXIS_MIN_"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data._ROLL_Z_AXIS_MAX_);		_stprintf(buf, _T("_ROLL_Z_AXIS_MAX_"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%d"), System_data.iVac_OFF_Delay);			_stprintf(buf, _T("iVac_OFF_Delay"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
//	_stprintf(value, _T("%d"), System_data._HOME_AXIS_TIME_OUT);	_stprintf(buf, _T("_HOME_AXIS_TIME_OUT"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data._HOME_TIME_OUT);			_stprintf(buf, _T("_HOME_TIME_OUT"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data._STEP_DELAY_TIME);		_stprintf(buf, _T("_STEP_DELAY_TIME"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data._CYL_AFTER_DELAY_TIME);	_stprintf(buf, _T("_CYL_AFTER_DELAY_TIME"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data._AXIS_MOVE_TIME_OUT);	_stprintf(buf, _T("_AXIS_MOVE_TIME_OUT"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dDancer_1_ZeroVal);		_stprintf(buf, _T("dDancer_1_ZeroVal"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	
	_stprintf(value, _T("%f"), System_data.dTop_Cam_PixelToum_2X);	_stprintf(buf, _T("dTop_Cam_PixelToum_2X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dTop_Cam_PixelToum_20X);	_stprintf(buf, _T("dTop_Cam_PixelToum_20X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBot_Cam_PixelToum_2X);	_stprintf(buf, _T("dBot_Cam_PixelToum_2X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBot_Cam_PixelToum_20X);	_stprintf(buf, _T("dBot_Cam_PixelToum_20X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data.iImageGrab_Delay);		_stprintf(buf, _T("iImageGrab_Delay"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dTopCam20To2_X);	_stprintf(buf, _T("dTopCam20To2_X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dTopCam20To2_Y);	_stprintf(buf, _T("dTopCam20To2_Y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dTopCam20To2_Z);	_stprintf(buf, _T("dTopCam20To2_Z"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBotCam20To2_X);	_stprintf(buf, _T("dBotCam20To2_X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBotCam20To2_Y);	_stprintf(buf, _T("dBotCam20To2_Y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBotCam20To2_Z);	_stprintf(buf, _T("dBotCam20To2_Z"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dBot_Cam_20X_Offset_X);	_stprintf(buf, _T("dBot_Cam_20X_Offset_X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBot_Cam_20X_Offset_Y);	_stprintf(buf, _T("dBot_Cam_20X_Offset_Y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dRotCen_PosStageX_20X);	_stprintf(buf, _T("dRotCen_PosStageX_20X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRotCen_PosCamY_20X);	_stprintf(buf, _T("dRotCen_PosCamY_20X"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dmmToDeg);	_stprintf(buf, _T("dmmToDeg"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	
	_stprintf(value, _T("%f"), System_data.X_SAFE_FREEZONE_L);	_stprintf(buf, _T("X_SAFE_FREEZONE_L"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_SAFE_FREEZONE_H);	_stprintf(buf, _T("X_SAFE_FREEZONE_H"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_SAFE_ZONE_L1);		_stprintf(buf, _T("X_SAFE_ZONE_L1"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_SAFE_ZONE_H1);		_stprintf(buf, _T("X_SAFE_ZONE_H1"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_SAFE_ZONE_L2);		_stprintf(buf, _T("X_SAFE_ZONE_L2"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_SAFE_ZONE_H2);		_stprintf(buf, _T("X_SAFE_ZONE_H2"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_MASK_VIEWZONE_L);	_stprintf(buf, _T("X_MASK_VIEWZONE_L"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.X_MASK_VIEWZONE_H);	_stprintf(buf, _T("X_MASK_VIEWZONE_H"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRollAlignPos_H);	_stprintf(buf, _T("dRollAlignPos_H"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRollAlignPos_L);	_stprintf(buf, _T("dRollAlignPos_L"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dX_Axis_Interlock);	_stprintf(buf, _T("dX_Axis_Interlock"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dQuartz_PosX);		_stprintf(buf, _T("dQuartz_PosX"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dQuartz_PosY);		_stprintf(buf, _T("dQuartz_PosY"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRollPlateStamp_PosX);	_stprintf(buf, _T("dRollPlateStamp_PosX"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRollPlateStamp_PosY);	_stprintf(buf, _T("dRollPlateStamp_PosY"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dStamp_Y_Max);	_stprintf(buf, _T("dStamp_Y_Max"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dShift_CamY);	_stprintf(buf, _T("dShift_CamY"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dStampMoveOffset_X);	_stprintf(buf, _T("dStampMoveOffset_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dStampMoveOffset_Y);	_stprintf(buf, _T("dStampMoveOffset_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.AF_Stepmm_2X);		_stprintf(buf, _T("AF_Stepmm_2X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.AF_Stepmm_20X);		_stprintf(buf, _T("AF_Stepmm_20X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data.iAF_TotalCount);		_stprintf(buf, _T("iAF_TotalCount"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dStage1ToStage2_Dist);		_stprintf(buf, _T("dStage1ToStage2_Dist"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dStage2ToStage3_Dist);		_stprintf(buf, _T("dStage2ToStage3_Dist"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dBasePos_Stage1_X);			_stprintf(buf, _T("dBasePos_Stage1_X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBasePos_Stage1_CamY);		_stprintf(buf, _T("dBasePos_Stage1_CamY"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBasePos_Stage2_X);			_stprintf(buf, _T("dBasePos_Stage2_X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBasePos_Stage2_CamY);		_stprintf(buf, _T("dBasePos_Stage2_CamY"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBasePos_Stage3_X);			_stprintf(buf, _T("dBasePos_Stage3_X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dBasePos_Stage3_CamY);		_stprintf(buf, _T("dBasePos_Stage3_CamY"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%d"), System_data.iCAM_20X_USED_FLAG);			_stprintf(buf, _T("iCAM_20X_USED_FLAG"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dHome_Offset_BX);			_stprintf(buf, _T("dHome_Offset_BX"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.TB_Offset_2X_X);			_stprintf(buf, _T("TB_Offset_2X_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
// 	_stprintf(value, _T("%f"), System_data.Roll_Offset_2X);			_stprintf(buf, _T("Roll_Offset_2X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
// 	_stprintf(value, _T("%f"), System_data.Q_Offset_20X);			_stprintf(buf, _T("Q_Offset_20X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
// 	_stprintf(value, _T("%f"), System_data.Roll_Offset_20X);		_stprintf(buf, _T("Roll_Offset_20X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.TB_Offset_2X_Y);			_stprintf(buf, _T("TB_Offset_2X_Y"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
// 	_stprintf(value, _T("%f"), System_data.Roll_Offset_2X_Y);		_stprintf(buf, _T("Roll_Offset_2X_Y"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
// 	_stprintf(value, _T("%f"), System_data.Q_Offset_20X_Y);			_stprintf(buf, _T("Q_Offset_20X_Y"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
// 	_stprintf(value, _T("%f"), System_data.Roll_Offset_20X_Y);		_stprintf(buf, _T("Roll_Offset_20X_Y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%d"), System_data.RollAlignPosMode);		_stprintf(buf, _T("RollAlignPosMode"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data.Led_Remote);				_stprintf(buf, _T("Led_Remote"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dKgPerum);				_stprintf(buf, _T("dKgPerum"));				WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dExcept_XPos);			_stprintf(buf, _T("dExcept_XPos"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dExcept_XPos2);			_stprintf(buf, _T("dExcept_XPos2"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRollAlign_ZPos);		_stprintf(buf, _T("dRollAlign_ZPos"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data.iRollTeachingDataUsed);	_stprintf(buf, _T("iRollTeachingDataUsed"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	
	_stprintf(value, _T("%f"), System_data.dRectSizeX);	_stprintf(buf, _T("dRectSizeX"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dRectSizeY);	_stprintf(buf, _T("dRectSizeY"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	
	_stprintf(value, _T("%f"), System_data.dZ2HomeOffsetVal);	_stprintf(buf, _T("dZ2HomeOffsetVal"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), System_data.dCompPitch);		_stprintf(buf, _T("dCompPitch"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data.iCompCountX);	_stprintf(buf, _T("iCompCountX"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), System_data.iCompCountY);	_stprintf(buf, _T("iCompCountY"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), System_data.dCompRectSize);		_stprintf(buf, _T("dCompRectSize"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	
	_stprintf(value, _T("%d"), System_data.bRollAF_Enable);	_stprintf(buf, _T("bRollAF_Enable"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	for(i= 0; i< _MAX_RUN_PARAM; i++) 
	{
		_stprintf(value, _T("%f"), System_data.dRoll_Diameter[i]	);		_stprintf(buf, _T("dRoll_Diameter_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), System_data.dRoll_Diameter_AddVal[i]);	_stprintf(buf, _T("dRoll_Diameter_AddVal_%d"), i);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), System_data.dZ_Base_Position[i]);		_stprintf(buf, _T("dZ_Base_Position_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), System_data.iLED_Top_Val_2X[i]);			_stprintf(buf, _T("iLED_Top_Val_2X_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), System_data.iLED_Btm_Val_2X[i]);			_stprintf(buf, _T("iLED_Btm_Val_2X_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), System_data.iLED_Top_Val_20X[i]);		_stprintf(buf, _T("iLED_Top_Val_20X_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), System_data.iLED_Btm_Val_20X[i]);		_stprintf(buf, _T("iLED_Btm_Val_20X_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}
	//-------------------------------
	strText.Format(_T("[SYSTEM-DATA-SAVE] SaveSytemConfigFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
}
//===================================================================================
void CParameterFile::OpenModelConfigFile(CString strName) // System_data.strCurrentModelName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	int i= 0;

	CString strText;
	TCHAR szParamFile[255];

	_stprintf(szParamFile, _T("%s\\%s.Model_"), strDirConfigPath, strName );
	_stprintf(ReadModel, _T("SYSTEM_DATA"));


	int pos;
	for(pos= 0; pos< _MAX_MOVE_SET_POS; pos++) 
	{
		_stprintf(ReadModel, _T("PositionNo_%d"), pos);
		for(i= 0; i< _MAX_AXIS; i++) 
		{
			_stprintf(buf, _T("AxisNo_%d_dPosition"), i);			GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dPosition[pos][i]= _wtof(value);
			_stprintf(buf, _T("AxisNo_%d_dPosition_Offset"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dPosition_Offset[pos][i]= _wtof(value);
		}
	}
	_stprintf(ReadModel, _T("MODEL_DATA"));
	
	for(i= 0; i< _MAX_CAM_; i++)
	{
		_stprintf(buf, _T("SearchROI_left_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.SearchROI_left[i]= _wtoi(value);
		_stprintf(buf, _T("SearchROI_right_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("1352"),  value, sizeof(value), szParamFile); Model_data.SearchROI_right[i]= _wtoi(value);
		_stprintf(buf, _T("SearchROI_top_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.SearchROI_top[i]= _wtoi(value);
		_stprintf(buf, _T("SearchROI_bottom_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("1030"),  value, sizeof(value), szParamFile); Model_data.SearchROI_bottom[i]= _wtoi(value);
	}
	for(i= 0; i< _MAX_RUN_PARAM; i++) 
	{
		_stprintf(buf, _T("iWaitFlag_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.iWaitFlag[i]= _wtoi(value);
		_stprintf(buf, _T("iRunEnable_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.iRunEnable[i]= _wtoi(value);
//		_stprintf(buf, _T("iAlignEnable_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.iAlignEnable[i]= _wtoi(value);
		_stprintf(buf, _T("iAlignMode_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.iAlignMode[i]= _wtoi(value);
		_stprintf(buf, _T("iRun_RollMode_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.iRun_RollMode[i]= _wtoi(value);
		_stprintf(buf, _T("iRun_Delay_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile); Model_data.iRun_Delay[i]= _wtoi(value);

		_stprintf(buf, _T("dRun_Speed_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_Speed[i]= _wtof(value);
		_stprintf(buf, _T("dRun_Length_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_Length[i]= _wtof(value);
		_stprintf(buf, _T("dRun_Force_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_Force[i]= _wtof(value);
		_stprintf(buf, _T("dRun_Force2_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_Force2[i]= _wtof(value);
		_stprintf(buf, _T("dRun_DownSpeed_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_DownSpeed[i]= _wtof(value);
		_stprintf(buf, _T("dRun_UpSpeed_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_UpSpeed[i]= _wtof(value);
		_stprintf(buf, _T("dRun_Offset_X_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_Offset_X[i]= _wtof(value);
		_stprintf(buf, _T("dRun_Offset_Y_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dRun_Offset_Y[i]= _wtof(value);

		_stprintf(buf, _T("iForceControlEnableFlag_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile);	Model_data.iForceControlEnableFlag[i]= _wtoi(value);
		_stprintf(buf, _T("iZ_Control_Mode_%d"), i);			GetPrivateProfileString(ReadModel, buf, _T("0"),  value, sizeof(value), szParamFile);	Model_data.iZ_Control_Mode[i]= _wtoi(value);

		_stprintf(buf, _T("dZ_Control_Fast_Length_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dZ_Control_Fast_Length[i]= _wtof(value);
		_stprintf(buf, _T("dZ_Control_Slow_Length_%d"), i);		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Model_data.dZ_Control_Slow_Length[i]= _wtof(value);

		//----------------------------
		_stprintf(buf, _T("iMarkSearchCount_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);			Model_data.iMarkSearchCount[i]= _wtoi(value);
		_stprintf(buf, _T("iRollAlignModeFlag_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);			Model_data.iRollAlignModeFlag[i]= _wtoi(value);
		
		_stprintf(buf, _T("dAlignMarkToMark_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);			Model_data.dAlignMarkToMark[i]= _wtof(value);
		_stprintf(buf, _T("dAlignMarkToMark_Y_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);			Model_data.dAlignMarkToMarkY[i]= _wtof(value);
		_stprintf(buf, _T("dAlignOffset_X_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);				Model_data.dAlignOffset_X[i]= _wtof(value);
		_stprintf(buf, _T("dAlignOffset_Y_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);				Model_data.dAlignOffset_Y[i]= _wtof(value);
		_stprintf(buf, _T("dAlignOffset_T_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);				Model_data.dAlignOffset_T[i]= _wtof(value);
	}

	//----------------------------
	_stprintf(buf, _T("iMarkMaxCount"));			GetPrivateProfileString(ReadModel, buf, _T("10"), value, sizeof(value), szParamFile); 		Model_data.iMarkMaxCount= _wtoi(value);
	_stprintf(buf, _T("dAlignAccuracy_umXY"));		GetPrivateProfileString(ReadModel, buf, _T("10.0"), value, sizeof(value), szParamFile); 	Model_data.dAlignAccuracy_umXY= _wtof(value);
	_stprintf(buf, _T("dAlignAccuracy_umT"));		GetPrivateProfileString(ReadModel, buf, _T("0.1"), value, sizeof(value), szParamFile); 		Model_data.dAlignAccuracy_umT= _wtof(value);

	_stprintf(buf, _T("dStamp_Len_X"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Len_X= _wtof(value);
	_stprintf(buf, _T("dStamp_Len_Y"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Len_Y= _wtof(value);

	_stprintf(buf, _T("dStamp_2X_Y"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_2X_Y= _wtof(value);
	_stprintf(buf, _T("dStamp_2X_X"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_2X_X= _wtof(value);
	_stprintf(buf, _T("dStamp_20X_X"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_20X_X= _wtof(value);
	_stprintf(buf, _T("dStamp_20X_Y"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_20X_Y= _wtof(value);

	_stprintf(buf, _T("dStamp_2X_2nd_Y"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_2X_2nd_Y= _wtof(value);
	_stprintf(buf, _T("dStamp_2X_2nd_X"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_2X_2nd_X= _wtof(value);
	_stprintf(buf, _T("dStamp_20X_2nd_X"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_20X_2nd_X= _wtof(value);
	_stprintf(buf, _T("dStamp_20X_2nd_Y"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_20X_2nd_Y= _wtof(value);

	_stprintf(buf, _T("dStampToBaseAlignDist_X"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStampToBaseAlignDist_X= _wtof(value);
	_stprintf(buf, _T("dStampToBaseAlignDist_Y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStampToBaseAlignDist_Y= _wtof(value);

	_stprintf(buf, _T("dStamp_Btm_2X_1st_Xpix"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_2X_1st_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_2X_1st_Ypix"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_2X_1st_Ypix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_20X_1st_Xpix"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_20X_1st_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_20X_1st_Ypix"));			GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_20X_1st_Ypix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_2X_2nd_Xpix"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_2X_2nd_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_2X_2nd_Ypix"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_2X_2nd_Ypix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_20X_2nd_Xpix"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_20X_2nd_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Btm_20X_2nd_Ypix"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Btm_20X_2nd_Ypix= _wtof(value);

	_stprintf(buf, _T("dStamp_Top_20X_1st_Xpix"));		GetPrivateProfileString(ReadModel, buf, _T("676.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_20X_1st_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_20X_1st_Ypix"));		GetPrivateProfileString(ReadModel, buf, _T("515.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_20X_1st_Ypix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_20X_2nd_Xpix"));	GetPrivateProfileString(ReadModel, buf, _T("676.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_20X_2nd_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_20X_2nd_Ypix"));	GetPrivateProfileString(ReadModel, buf, _T("515.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_20X_2nd_Ypix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_2X_1st_Xpix"));		GetPrivateProfileString(ReadModel, buf, _T("676.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_2X_1st_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_2X_1st_Ypix"));		GetPrivateProfileString(ReadModel, buf, _T("515.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_2X_1st_Ypix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_2X_2nd_Xpix"));	GetPrivateProfileString(ReadModel, buf, _T("676.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_2X_2nd_Xpix= _wtof(value);
	_stprintf(buf, _T("dStamp_Top_2X_2nd_Ypix"));	GetPrivateProfileString(ReadModel, buf, _T("515.0"), value, sizeof(value), szParamFile); 		Model_data.dStamp_Top_2X_2nd_Ypix= _wtof(value);

	_stprintf(buf, _T("dZ_SlowDownLen"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 		Model_data.dZ_SlowDownLen= _wtof(value);
	_stprintf(buf, _T("iDownEnd_WaitFlag"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile); 	Model_data.iDownEnd_WaitFlag= _wtoi(value);

	_stprintf(buf, _T("dLED_Pitch_X"));	GetPrivateProfileString(ReadModel, buf, _T("0.6"), value, sizeof(value), szParamFile); 	Model_data.dLED_Pitch_X= _wtof(value);
	_stprintf(buf, _T("dLED_Pitch_Y"));	GetPrivateProfileString(ReadModel, buf, _T("0.6"), value, sizeof(value), szParamFile); 	Model_data.dLED_Pitch_Y= _wtof(value);

	_stprintf(buf, _T("dRollTest_Z1"));	GetPrivateProfileString(ReadModel, buf, _T("5.0"), value, sizeof(value), szParamFile); 	Model_data.dRollTest_Z1= _wtof(value);
	_stprintf(buf, _T("dRollTest_Z2"));	GetPrivateProfileString(ReadModel, buf, _T("5.0"), value, sizeof(value), szParamFile); 	Model_data.dRollTest_Z2= _wtof(value);
	_stprintf(buf, _T("iZ_ControlOption"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile); 	Model_data.iZ_ControlOption= _wtoi(value);
	Model_data.iZ_Control_Z1Hap= 0;

	_stprintf(buf, _T("iRoll_AngleAlignMode"));		GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile); 	Model_data.iRoll_AngleAlignMode= _wtoi(value);
	_stprintf(buf, _T("dRoll_AngleAlign_Pitch"));	GetPrivateProfileString(ReadModel, buf, _T("5.0"), value, sizeof(value), szParamFile); 	Model_data.dRoll_AngleAlign_Pitch= _wtof(value);
	_stprintf(buf, _T("iRoll_AngleAlign_Count"));	GetPrivateProfileString(ReadModel, buf, _T("5"), value, sizeof(value), szParamFile); 	Model_data.iRoll_AngleAlign_Count= _wtoi(value);

	for(i= 0; i< _MAX_FIND_MODEL; i++) 
	{
		_stprintf(ReadModel, _T("PATTERN_MODEL_DATA_%d"), i);

		_stprintf(buf, _T("iSpeed"));	GetPrivateProfileString(ReadModel, buf, _T("4"), value, sizeof(value), szParamFile);				Pattern_data[i].iSpeed= _wtoi(value);

		_stprintf(buf, _T("iAccuracy"));	GetPrivateProfileString(ReadModel, buf, _T("2"), value, sizeof(value), szParamFile);			Pattern_data[i].iAccuracy= _wtoi(value);

		_stprintf(buf, _T("dScoreCertanty"));	GetPrivateProfileString(ReadModel, buf, _T("60.0"), value, sizeof(value), szParamFile);		Pattern_data[i].dScoreCertanty= _wtof(value);

		_stprintf(buf, _T("dScoreAcceptance"));	GetPrivateProfileString(ReadModel, buf, _T("50.0"), value, sizeof(value), szParamFile);		Pattern_data[i].dScoreAcceptance= _wtof(value);

		_stprintf(buf, _T("iTimeOut"));	GetPrivateProfileString(ReadModel, buf, _T("1000"), value, sizeof(value), szParamFile);				Pattern_data[i].iTimeOut= _wtoi(value);

		_stprintf(buf, _T("dOffsetX"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);				Pattern_data[i].dOffsetX= _wtof(value);

		_stprintf(buf, _T("dOffsetY"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);				Pattern_data[i].dOffsetY= _wtof(value);

	}

	//-------------------------------
	_stprintf(buf, _T("UserName"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("USER"), value, sizeof(value), szParamFile);
	_stprintf(User_data.strUserName, value);

	_stprintf(buf, _T("strUseInk"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("USER"), value, sizeof(value), szParamFile);
	_stprintf(User_data.strUseInk, value);
	_stprintf(buf, _T("strUseBlanket"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("USER"), value, sizeof(value), szParamFile);
	_stprintf(User_data.strUseBlanket, value);
	_stprintf(buf, _T("Memo"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("USER"), value, sizeof(value), szParamFile);
	_stprintf(User_data.Memo, value);

	_stprintf(buf, _T("lUseHour"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1"), value, sizeof(value), szParamFile); 
	User_data.lUseHour= _wtoi(value);
	_stprintf(buf, _T("lUseMinute"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1"), value, sizeof(value), szParamFile); 
	User_data.lUseMinute= _wtoi(value);
	_stprintf(buf, _T("lUseDay"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1"), value, sizeof(value), szParamFile); 
	User_data.lUseDay= _wtoi(value);
	_stprintf(buf, _T("lUseMon"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1"), value, sizeof(value), szParamFile); 
	User_data.lUseMon= _wtoi(value);
	_stprintf(buf, _T("lUseYear"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1"), value, sizeof(value), szParamFile); 
	User_data.lUseYear= _wtoi(value);
	_stprintf(buf, _T("dUseTemp"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1.0"), value, sizeof(value), szParamFile); 
	User_data.dUseTemp= _wtof(value);
	_stprintf(buf, _T("dUseHumidity"));	GetPrivateProfileString(_T("USER_DATA"), buf, _T("1.0"), value, sizeof(value), szParamFile); 
	User_data.dUseHumidity= _wtof(value);

	//-------------------------------
	strText.Format(_T("[MODEL-DATA-READ] OpenSytemConfigFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
	//-------------------------------
	FILE* fcheck = _wfopen(szParamFile, _T("r"));
	if(fcheck == NULL) 	{	
		strText.Format(_T("[MODEL-DATA-INITIAL] OpenSytemConfigFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
		SaveModelConfigFile(System_data.strCurrentModelName);
	}
	else				fclose(fcheck);

	OpenRecipeConfigFile(System_data.strCurrentRecipeName);

//	ReadSlotDieParamFile(System_data.strCurrentModelName);
}
//===================================================================================
void CParameterFile::SaveModelConfigFile(CString strName) // System_data.strCurrentModelName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	CString strText;
	TCHAR szParamFile[255];
	int i= 0;

	_stprintf(szParamFile, _T("%s\\%s.Model_"), strDirConfigPath, strName );
	
	_stprintf(ReadModel, _T("SYSTEM_DATA"));
	//---------------------------------------

//	double	dPosition[_MAX_MOVE_SET_POS][_MAX_AXIS];
//	double	dPosition_Offset[_MAX_MOVE_SET_POS][_MAX_AXIS];
	
	int pos;
	for(pos= 0; pos< _MAX_MOVE_SET_POS; pos++) 
	{
		_stprintf(ReadModel, _T("PositionNo_%d"), pos);
		for(i= 0; i< _MAX_AXIS; i++) 
		{
			_stprintf(value, _T("%f"), Model_data.dPosition[pos][i]	);			_stprintf(buf, _T("AxisNo_%d_dPosition"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
			_stprintf(value, _T("%f"), Model_data.dPosition_Offset[pos][i]	);	_stprintf(buf, _T("AxisNo_%d_dPosition_Offset"), i);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		}
	}

	_stprintf(ReadModel, _T("MODEL_DATA"));

	for(i= 0; i< _MAX_CAM_; i++)
	{
		_stprintf(value, _T("%d"), Model_data.SearchROI_left[i]	);		_stprintf(buf, _T("SearchROI_left_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.SearchROI_right[i]	);	_stprintf(buf, _T("SearchROI_right_%d"), i);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.SearchROI_top[i]	);		_stprintf(buf, _T("SearchROI_top_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.SearchROI_bottom[i]	);	_stprintf(buf, _T("SearchROI_bottom_%d"), i);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
}
	for(i= 0; i< _MAX_RUN_PARAM; i++) 
	{

		_stprintf(value, _T("%d"), Model_data.iWaitFlag[i]	);			_stprintf(buf, _T("iWaitFlag_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.iRunEnable[i]	);			_stprintf(buf, _T("iRunEnable_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
//		_stprintf(value, _T("%d"), Model_data.iAlignEnable[i]	);		_stprintf(buf, _T("iAlignEnable_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.iAlignMode[i]	);			_stprintf(buf, _T("iAlignMode_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.iRun_RollMode[i]	);		_stprintf(buf, _T("iRun_RollMode_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.iRun_Delay[i]	);			_stprintf(buf, _T("iRun_Delay_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Model_data.dRun_Speed[i]	);			_stprintf(buf, _T("dRun_Speed_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_Length[i]	);		_stprintf(buf, _T("dRun_Length_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_Force[i]	);			_stprintf(buf, _T("dRun_Force_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_Force2[i]	);			_stprintf(buf, _T("dRun_Force2_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_DownSpeed[i]	);		_stprintf(buf, _T("dRun_DownSpeed_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_UpSpeed[i]	);		_stprintf(buf, _T("dRun_UpSpeed_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_Offset_X[i]	);		_stprintf(buf, _T("dRun_Offset_X_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dRun_Offset_Y[i]	);		_stprintf(buf, _T("dRun_Offset_Y_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%d"), Model_data.iForceControlEnableFlag[i]	);	_stprintf(buf, _T("iForceControlEnableFlag_%d"), i);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.iZ_Control_Mode[i]	);			_stprintf(buf, _T("iZ_Control_Mode_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Model_data.dZ_Control_Fast_Length[i]	);		_stprintf(buf, _T("dZ_Control_Fast_Length_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dZ_Control_Slow_Length[i]	);		_stprintf(buf, _T("dZ_Control_Slow_Length_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		//----------------------------

		_stprintf(value, _T("%d"), Model_data.iMarkSearchCount[i]);		_stprintf(buf, _T("iMarkSearchCount_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%d"), Model_data.iRollAlignModeFlag[i]);	_stprintf(buf, _T("iRollAlignModeFlag_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		
		_stprintf(value, _T("%f"), Model_data.dAlignMarkToMark[i]);		_stprintf(buf, _T("dAlignMarkToMark_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dAlignMarkToMarkY[i]);	_stprintf(buf, _T("dAlignMarkToMark_Y_%d"), i);		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dAlignOffset_X[i]);		_stprintf(buf, _T("dAlignOffset_X_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dAlignOffset_Y[i]);		_stprintf(buf, _T("dAlignOffset_Y_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Model_data.dAlignOffset_T[i]);		_stprintf(buf, _T("dAlignOffset_T_%d"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}
	System_data.dRollPlateStamp_PosY= 0.0;	System_data.dQuartz_PosY= 0.0;
	//----------------------------
	_stprintf(value, _T("%d"), Model_data.iMarkMaxCount);			_stprintf(buf, _T("iMarkMaxCount"));				WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dAlignAccuracy_umXY);		_stprintf(buf, _T("dAlignAccuracy_umXY"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dAlignAccuracy_umT);		_stprintf(buf, _T("dAlignAccuracy_umT"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	//----------------------------
	_stprintf(value, _T("%f"), Model_data.dStamp_Len_X);		_stprintf(buf, _T("dStamp_Len_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Len_Y);		_stprintf(buf, _T("dStamp_Len_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dStamp_2X_X);		_stprintf(buf, _T("dStamp_2X_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_2X_Y);		_stprintf(buf, _T("dStamp_2X_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_20X_X);		_stprintf(buf, _T("dStamp_20X_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_20X_Y);		_stprintf(buf, _T("dStamp_20X_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dStamp_2X_2nd_X);		_stprintf(buf, _T("dStamp_2X_2nd_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_2X_2nd_Y);		_stprintf(buf, _T("dStamp_2X_2nd_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_20X_2nd_X);		_stprintf(buf, _T("dStamp_20X_2nd_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_20X_2nd_Y);		_stprintf(buf, _T("dStamp_20X_2nd_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dStampToBaseAlignDist_X);		_stprintf(buf, _T("dStampToBaseAlignDist_X"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStampToBaseAlignDist_Y);		_stprintf(buf, _T("dStampToBaseAlignDist_Y"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_2X_1st_Xpix);		_stprintf(buf, _T("dStamp_Btm_2X_1st_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_2X_1st_Ypix);		_stprintf(buf, _T("dStamp_Btm_2X_1st_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_20X_1st_Xpix);		_stprintf(buf, _T("dStamp_Btm_20X_1st_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_20X_1st_Ypix);		_stprintf(buf, _T("dStamp_Btm_20X_1st_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_2X_2nd_Xpix);		_stprintf(buf, _T("dStamp_Btm_2X_2nd_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_2X_2nd_Ypix);		_stprintf(buf, _T("dStamp_Btm_2X_2nd_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_20X_2nd_Xpix);		_stprintf(buf, _T("dStamp_Btm_20X_2nd_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Btm_20X_2nd_Ypix);		_stprintf(buf, _T("dStamp_Btm_20X_2nd_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dStamp_Top_20X_1st_Xpix);		_stprintf(buf, _T("dStamp_Top_20X_1st_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_20X_1st_Ypix);		_stprintf(buf, _T("dStamp_Top_20X_1st_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_20X_2nd_Xpix);	_stprintf(buf, _T("dStamp_Top_20X_2nd_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_20X_2nd_Ypix);	_stprintf(buf, _T("dStamp_Top_20X_2nd_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_2X_1st_Xpix);		_stprintf(buf, _T("dStamp_Top_2X_1st_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_2X_1st_Ypix);		_stprintf(buf, _T("dStamp_Top_2X_1st_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_2X_2nd_Xpix);	_stprintf(buf, _T("dStamp_Top_2X_2nd_Xpix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dStamp_Top_2X_2nd_Ypix);	_stprintf(buf, _T("dStamp_Top_2X_2nd_Ypix"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dZ_SlowDownLen);			_stprintf(buf, _T("dZ_SlowDownLen"));				WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), Model_data.iDownEnd_WaitFlag);		_stprintf(buf, _T("iDownEnd_WaitFlag"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dLED_Pitch_X);			_stprintf(buf, _T("dLED_Pitch_X"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dLED_Pitch_Y);			_stprintf(buf, _T("dLED_Pitch_Y"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Model_data.dRollTest_Z1);			_stprintf(buf, _T("dRollTest_Z1"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dRollTest_Z2);			_stprintf(buf, _T("dRollTest_Z2"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
//	_stprintf(value, _T("%d"), Model_data.iZ_ControlOption);		_stprintf(buf, _T("iZ_ControlOption"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%d"), Model_data.iRoll_AngleAlignMode);			_stprintf(buf, _T("iRoll_AngleAlignMode"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Model_data.dRoll_AngleAlign_Pitch);			_stprintf(buf, _T("dRoll_AngleAlign_Pitch"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%d"), Model_data.iRoll_AngleAlign_Count);			_stprintf(buf, _T("iRoll_AngleAlign_Count"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	for(i= 0; i< _MAX_FIND_MODEL; i++) {
		_stprintf(ReadModel, _T("PATTERN_MODEL_DATA_%d"), i);

		_stprintf(value, _T("%d"), Pattern_data[i].iSpeed);				_stprintf(buf, _T("iSpeed"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%d"), Pattern_data[i].iAccuracy);			_stprintf(buf, _T("iAccuracy"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Pattern_data[i].dScoreCertanty);		_stprintf(buf, _T("dScoreCertanty"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Pattern_data[i].dScoreAcceptance);	_stprintf(buf, _T("dScoreAcceptance"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%d"), Pattern_data[i].iTimeOut);			_stprintf(buf, _T("iTimeOut"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Pattern_data[i].dOffsetX);			_stprintf(buf, _T("dOffsetX"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Pattern_data[i].dOffsetY);			_stprintf(buf, _T("dOffsetY"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}

	//-------------------------------
	_stprintf(value, _T("%s"), User_data.strUserName);		_stprintf(buf, _T("UserName"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%s"), User_data.strUseInk);		_stprintf(buf, _T("strUseInk"));		WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%s"), User_data.strUseBlanket);	_stprintf(buf, _T("strUseBlanket"));	WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%s"), User_data.strMemo);			_stprintf(buf, _T("strMemo"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);

	_stprintf(value, _T("%ld"), User_data.lUseHour);		_stprintf(buf, _T("lUseHour"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%ld"), User_data.lUseMinute);		_stprintf(buf, _T("lUseMinute"));		WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%ld"), User_data.lUseDay);			_stprintf(buf, _T("lUseDay"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%ld"), User_data.lUseMon);			_stprintf(buf, _T("lUseMon"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%ld"), User_data.lUseYear);		_stprintf(buf, _T("lUseYear"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%f"), User_data.dUseTemp);			_stprintf(buf, _T("dUseTemp"));			WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	_stprintf(value, _T("%f"), User_data.dUseHumidity);		_stprintf(buf, _T("dUseHumidity"));		WritePrivateProfileString(_T("USER_DATA"), buf, value, szParamFile);
	//---------------------------------------
	strText.Format(_T("[MODEL-DATA-SAVE] SaveSytemConfigFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
}
//===================================================================================
void CParameterFile::SaveModelConfigFileSemi(CString strName) // System_data.strCurrentModelName
	//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	CString strText;
	TCHAR szParamFile[255];
	int i= 0;

	_stprintf(szParamFile, _T("%s\\%s.Model_"), strDirConfigPath, strName );

	_stprintf(ReadModel, _T("SYSTEM_DATA"));
	//---------------------------------------

	//	double	dPosition[_MAX_MOVE_SET_POS][_MAX_AXIS];
	//	double	dPosition_Offset[_MAX_MOVE_SET_POS][_MAX_AXIS];

	int pos;
	for(pos= 0; pos< _MAX_MOVE_SET_POS; pos++) 
	{
		_stprintf(ReadModel, _T("PositionNo_%d"), pos);
		for(i= 0; i< _MAX_AXIS; i++) 
		{
			_stprintf(value, _T("%f"), Model_data.dPosition[pos][i]	);			_stprintf(buf, _T("AxisNo_%d_dPosition"), i);			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
			_stprintf(value, _T("%f"), Model_data.dPosition_Offset[pos][i]	);	_stprintf(buf, _T("AxisNo_%d_dPosition_Offset"), i);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		}
	}
}

//===================================================================================
void CParameterFile::ReadMotionParamFile() //Model_data.strCurrentMotionName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	TCHAR szParamFile[255];
	CString strText;
	int i= 0;

	_stprintf(szParamFile, _T("%s\\default.Mot_"), strDirConfigPath);//, strName );
	//---------------------------------------
	for(i= 0; i< _MAX_AXIS; i++) 
	{
		_stprintf(ReadModel, _T("MOTION_AXIS_%d"), i);

		_stprintf(buf, _T("Origin_Speed"));	GetPrivateProfileString(ReadModel, buf, _T("1000.0"),  value, sizeof(value), szParamFile); Motion_data.dOrigin_Speed[i]= _wtof(value);
		_stprintf(buf, _T("Origin_Accel"));	GetPrivateProfileString(ReadModel, buf, _T("10000.0"), value, sizeof(value), szParamFile); Motion_data.dOrigin_Accel[i]= _wtof(value);
		_stprintf(buf, _T("Origin_Decel"));	GetPrivateProfileString(ReadModel, buf, _T("10000.0"), value, sizeof(value), szParamFile); Motion_data.dOrigin_Decel[i]= _wtof(value);

		_stprintf(buf, _T("Start_Speed"));	GetPrivateProfileString(ReadModel, buf, _T("1000.0"),  value, sizeof(value), szParamFile); Motion_data.dStart_Speed[i]= _wtof(value);
		_stprintf(buf, _T("Work_Speed"));	GetPrivateProfileString(ReadModel, buf, _T("1000.0"),  value, sizeof(value), szParamFile); Motion_data.dWork_Speed[i]= _wtof(value);
		_stprintf(buf, _T("Work_Accel"));	GetPrivateProfileString(ReadModel, buf, _T("10000.0"), value, sizeof(value), szParamFile); Motion_data.dWork_Accel[i]= _wtof(value);
		_stprintf(buf, _T("Work_Decel"));	GetPrivateProfileString(ReadModel, buf, _T("10000.0"), value, sizeof(value), szParamFile); Motion_data.dWork_Decel[i]= _wtof(value);

		_stprintf(buf, _T("Jog_MaxSpeed"));	GetPrivateProfileString(ReadModel, buf, _T("1000.0"), value, sizeof(value), szParamFile); Motion_data.dJog_MaxSpeed[i]= _wtof(value);
		_stprintf(buf, _T("Jog_MidSpeed"));	GetPrivateProfileString(ReadModel, buf, _T("500.0"),  value, sizeof(value), szParamFile); Motion_data.dJog_MidSpeed[i]= _wtof(value);
		_stprintf(buf, _T("Jog_MinSpeed"));	GetPrivateProfileString(ReadModel, buf, _T("100.0"),  value, sizeof(value), szParamFile); Motion_data.dJog_MinSpeed[i]= _wtof(value);

		_stprintf(buf, _T("PulsePerMM"));	GetPrivateProfileString(ReadModel, buf, _T("1000.0"),		value, sizeof(value), szParamFile);	Motion_data.dPulsePerMM[i]	= _wtof(value);

		_stprintf(buf, _T("dSW_Limit_Low"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),		value, sizeof(value), szParamFile);	Motion_data.dSW_Limit_Low[i]	= _wtof(value);
		_stprintf(buf, _T("dSW_Limit_High"));	GetPrivateProfileString(ReadModel, buf, _T("10.0"),		value, sizeof(value), szParamFile);	Motion_data.dSW_Limit_High[i]	= _wtof(value);
	}

	_stprintf(ReadModel, _T("GENERAL_PARAM"));
	_stprintf(buf, _T("Z_Axis_Interlock"));	GetPrivateProfileString(ReadModel, buf, _T("15.0"),	value, sizeof(value), szParamFile);	Motion_data.dZ_Axis_Interlock	= _wtof(value);

	_stprintf(buf, _T("Z1_ORG_Offset"));		GetPrivateProfileString(ReadModel, buf, _T("100.0"), value, sizeof(value), szParamFile); 		Motion_data.dZ1_ORG_Offset= _wtof(value);
	_stprintf(buf, _T("Z2_ORG_Offset"));		GetPrivateProfileString(ReadModel, buf, _T("100.0"), value, sizeof(value), szParamFile); 		Motion_data.dZ2_ORG_Offset= _wtof(value);
	_stprintf(buf, _T("dRollRun_BZ_Pos"));		GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile); 			Motion_data.dRollRun_BZ_Pos= _wtof(value);
	
	//---------------------------------------
	strText.Format(_T("[MOTION-DATA-READ] ReadMotionParamFile() -> Complite! [MODEL-NAME: %s]"), szParamFile); 	SaveLogFile(_SYSTEM_LOG, strText);
	//---------------------------------------
	FILE* fcheck = _wfopen(szParamFile, _T("r"));
	if(fcheck == NULL) 	{	
		strText.Format(_T("[MOTION-DATA-INITIAL] ReadMotionParamFile() -> Complite! [MODEL-NAME: %s]"), szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);
		SaveMotionParamFile();//System_data.strCurrentModelName);
	}
	else				fclose(fcheck);
}

//===================================================================================
void CParameterFile::SaveMotionParamFile() //Model_data.strCurrentMotionName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	TCHAR szParamFile[255];
	CString strText;
	int i= 0;

	_stprintf(szParamFile, _T("%s\\default.Mot_"), strDirConfigPath);//, strName );
	//---------------------------------------
	for(i= 0; i< _MAX_AXIS; i++) 
	{
		_stprintf(ReadModel, _T("MOTION_AXIS_%d"), i);
		_stprintf(value, _T("%f"), Motion_data.dOrigin_Speed[i]);	_stprintf(buf, _T("Origin_Speed"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dOrigin_Accel[i]);	_stprintf(buf, _T("Origin_Accel"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dOrigin_Decel[i]);	_stprintf(buf, _T("Origin_Decel"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dStart_Speed[i]);	_stprintf(buf, _T("Start_Speed"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dWork_Speed[i]);		_stprintf(buf, _T("Work_Speed"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dWork_Accel[i]);		_stprintf(buf, _T("Work_Accel"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dWork_Decel[i]);		_stprintf(buf, _T("Work_Decel"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dJog_MaxSpeed[i]);	_stprintf(buf, _T("Jog_MaxSpeed"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dJog_MidSpeed[i]);	_stprintf(buf, _T("Jog_MidSpeed"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dJog_MinSpeed[i]);	_stprintf(buf, _T("Jog_MinSpeed"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Motion_data.dPulsePerMM[i]);	_stprintf(buf, _T("PulsePerMM"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), Motion_data.dSW_Limit_Low[i]);	_stprintf(buf, _T("dSW_Limit_Low"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Motion_data.dSW_Limit_High[i]);	_stprintf(buf, _T("dSW_Limit_High"));		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}
	_stprintf(ReadModel, _T("GENERAL_PARAM"));

	_stprintf(value, _T("%f"), Motion_data.dZ_Axis_Interlock);	_stprintf(buf, _T("Z_Axis_Interlock"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	_stprintf(value, _T("%f"), Motion_data.dZ1_ORG_Offset);		_stprintf(buf, _T("Z1_ORG_Offset"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Motion_data.dZ2_ORG_Offset);		_stprintf(buf, _T("Z2_ORG_Offset"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	_stprintf(value, _T("%f"), Motion_data.dRollRun_BZ_Pos);		_stprintf(buf, _T("dRollRun_BZ_Pos"));			WritePrivateProfileString(ReadModel, buf, value, szParamFile);

	//---------------------------------------
	strText.Format(_T("[MOTION-DATA-SAVE] SaveMotionParamFile() -> Complite! [MODEL-NAME: %s]"), szParamFile); 
	SaveLogFile(_SYSTEM_LOG, strText);
}

//===================================================================================
void CParameterFile::OpenRecipeConfigFile(CString strName) // System_data.strCurrentRecipeName
	//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	int i= 0, j= 0;

	CString strText;
	TCHAR szParamFile[255];

	_stprintf(szParamFile, L"%s\\%s.Recipe_", strDirConfigPath, strName );

	_stprintf(ReadModel, L"RECIPE_DATA");

	for(i= 0; i< _MAX_RECIPE; i++) {
		_stprintf(ReadModel, L"RECIPE_DATA_%d", i);

		_stprintf(buf, L"iCommand");	GetPrivateProfileString(ReadModel, buf, L"0",	value, sizeof(value), szParamFile);	Recipe_data[i].iCommand	= _wtoi(value);
		_stprintf(buf, L"dOP_Val1");	GetPrivateProfileString(ReadModel, buf, L"0.0",	value, sizeof(value), szParamFile);	Recipe_data[i].dOP_Val1	= _wtof(value);
		_stprintf(buf, L"dOP_Val2");	GetPrivateProfileString(ReadModel, buf, L"0.0",	value, sizeof(value), szParamFile);	Recipe_data[i].dOP_Val2	= _wtof(value);
		_stprintf(buf, L"dOP_Val3");	GetPrivateProfileString(ReadModel, buf, L"0.0",	value, sizeof(value), szParamFile);	Recipe_data[i].dOP_Val3	= _wtof(value);
		_stprintf(buf, L"dOP_Val4");	GetPrivateProfileString(ReadModel, buf, L"0.0",	value, sizeof(value), szParamFile);	Recipe_data[i].dOP_Val4	= _wtof(value);
		_stprintf(buf, L"dOP_Val5");	GetPrivateProfileString(ReadModel, buf, L"0.0",	value, sizeof(value), szParamFile);	Recipe_data[i].dOP_Val5	= _wtof(value);
		_stprintf(buf, L"chrOP_Val"); 	GetPrivateProfileString(ReadModel, buf, L" ", Recipe_data[i].chrOP_Val,	sizeof(buf), szParamFile);
	}

	//-------------------------------
	strText.Format(L"[MODEL-DATA-READ] OpenRecipeConfigFile() -> Complite! [MODEL-NAME: %s]", szParamFile);	
	SaveLogFile(_SYSTEM_LOG, strText);
	//-------------------------------
}
//===================================================================================
void CParameterFile::SaveRecipeConfigFile(CString strName) // System_data.strCurrentRecipeName
	//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	CString strText;
	TCHAR szParamFile[255];
	int i= 0, j= 0;

	_stprintf(szParamFile, L"%s\\%s.Recipe_", strDirConfigPath, strName );

	//---------------------------------------
	_stprintf(ReadModel, L"RECIPE_DATA");

	for(i= 0; i< _MAX_RECIPE; i++) {
		_stprintf(ReadModel, L"RECIPE_DATA_%d", i);
		_stprintf(value, L"%d", Recipe_data[i].iCommand);		_stprintf(buf, L"iCommand");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, L"%f", Recipe_data[i].dOP_Val1);		_stprintf(buf, L"dOP_Val1");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, L"%f", Recipe_data[i].dOP_Val2);		_stprintf(buf, L"dOP_Val2");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, L"%f", Recipe_data[i].dOP_Val3);		_stprintf(buf, L"dOP_Val3");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, L"%f", Recipe_data[i].dOP_Val4);		_stprintf(buf, L"dOP_Val4");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, L"%f", Recipe_data[i].dOP_Val5);		_stprintf(buf, L"dOP_Val5");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, L"%s", Recipe_data[i].chrOP_Val);		_stprintf(buf, L"chrOP_Val");		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}

	//---------------------------------------
	strText.Format(L"[MODEL-DATA-SAVE] SaveRecipeConfigFile() -> Complite! [MODEL-NAME: %s]", szParamFile);	SaveLogFile(_SYSTEM_LOG, strText);

	_stprintf(szParamFile, L"%s\\Default.INI", strDirConfigPath );
	_stprintf(value, L"%s", System_data.strCurrentRecipeName);	_stprintf(buf, L"Recipe_Name"); 	WritePrivateProfileString(L"Def_Name", buf, value, szParamFile);
}

//===================================================================================
void CParameterFile::ReadResultDataFile() //Model_data.strCurrentMotionName
	//-----------------------------------------------------------------------------------
{
	TCHAR buf[500], value[500], ReadModel[500];
	TCHAR szParamFile[255];
	CString strText;
	int i= 0, j= 0;

	_stprintf(szParamFile, _T("%s\\Result.Dat_"), strDirConfigPath);//, strName );
	//---------------------------------------
	for(i= 0; i< _MAX_ALIGN_RESULT_DATA; i++) 
	{
		_stprintf(ReadModel, _T("Result_%d"), i);

		_stprintf(buf, _T("dTheta"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dTheta= _wtof(value);
		_stprintf(buf, _T("dptRealPoint_1st.x"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptRealPoint_1st.x= _wtof(value);
		_stprintf(buf, _T("dptRealPoint_1st.y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptRealPoint_1st.y= _wtof(value);
		_stprintf(buf, _T("dptRealPoint_2nd.x"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptRealPoint_2nd.x= _wtof(value);
		_stprintf(buf, _T("dptRealPoint_2nd.y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptRealPoint_2nd.y= _wtof(value);
		_stprintf(buf, _T("dptPoint_1st.x"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptPoint_1st.x= _wtof(value);
		_stprintf(buf, _T("dptPoint_2nd.x"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptPoint_2nd.x= _wtof(value);
		_stprintf(buf, _T("dptPoint_1st.y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptPoint_1st.y= _wtof(value);
		_stprintf(buf, _T("dptPoint_2nd.y"));	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dptPoint_2nd.y= _wtof(value);
		for(j= 0; j< _MAX_AXIS; j++) 
		{
			_stprintf(ReadModel, _T("MOTION_AXIS_%d"), i);
			_stprintf(buf, _T("dPosData_%d"), j);	GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile); Align_Result_data[i].dPosData[j]= _wtof(value);
		}
	}
	//---------------------------------------
}

//===================================================================================
void CParameterFile::SaveResultDataFile() //Model_data.strCurrentMotionName
	//-----------------------------------------------------------------------------------
{
	TCHAR buf[50], value[50], ReadModel[50];
	TCHAR szParamFile[255];
	CString strText;
	int i= 0, j= 0;

	_stprintf(szParamFile, _T("%s\\Result.Dat_"), strDirConfigPath);//, strName );
	//---------------------------------------
	for(i= 0; i< _MAX_ALIGN_RESULT_DATA; i++) 
	{
		_stprintf(ReadModel, _T("Result_%d"), i);

		_stprintf(value, _T("%f"), Align_Result_data[i].dTheta);	_stprintf(buf, _T("dTheta"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptRealPoint_1st.x);	_stprintf(buf, _T("dptRealPoint_1st.x"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptRealPoint_1st.y);	_stprintf(buf, _T("dptRealPoint_1st.y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptRealPoint_2nd.x);	_stprintf(buf, _T("dptRealPoint_2nd.x"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptRealPoint_2nd.y);	_stprintf(buf, _T("dptRealPoint_2nd.y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptPoint_1st.x);	_stprintf(buf, _T("dptPoint_1st.x"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptPoint_2nd.x);	_stprintf(buf, _T("dptPoint_2nd.x"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptPoint_1st.y);	_stprintf(buf, _T("dptPoint_1st.y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		_stprintf(value, _T("%f"), Align_Result_data[i].dptPoint_2nd.y);	_stprintf(buf, _T("dptPoint_2nd.y"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		for(j= 0; j< _MAX_AXIS; j++) 
		{
			_stprintf(ReadModel, _T("MOTION_AXIS_%d"), i);
			_stprintf(value, _T("%f"), Align_Result_data[i].dPosData[j]);	_stprintf(buf, _T("dPosData_%d"), j);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
		}
	}
}

//===================================================================================
void CParameterFile::ReadIOTableParamFile() //Model_data.strCurrentMotionName
	//-----------------------------------------------------------------------------------
{
	TCHAR buf[50], ReadModel[50];
	TCHAR szParamFile[255];
	CString strText;
	int i= 0;

#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	_stprintf(szParamFile, _T("%s\\LGE_IO_Name_Table.INI"), strDirConfigPath);
#else
	_stprintf(szParamFile, _T("%s\\KIMM_IO_Name_Table.INI"), strDirConfigPath);
#endif
	//---------------------------------------
	_stprintf(ReadModel, _T("INPUT_Def_Name"));
	for(i= 0; i< DEF_IOMONITOR_MAX_IO_INPUT; i++) {
		_stprintf(buf, _T("INPUT_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("i--"),  Input_Table[i].Name, sizeof(buf), szParamFile);
	}
	_stprintf(ReadModel, _T("OUTPUT_Def_Name"));
	for(i= 0; i< DEF_IOMONITOR_MAX_IO_OUTPUT; i++) {
		_stprintf(buf, _T("OUTPUT_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("o--"),  Output_Table[i].Name, sizeof(buf), szParamFile);
	}
	// 	_stprintf(ReadModel, "MOTOR_Def_Name"));
	// 	for(i= 0; i< _MAX_AXIS; i++) {
	// 		_stprintf(buf, _T("AXIS_%d", i);	GetPrivateProfileString(ReadModel, buf, "a--",  Axis_Table[i].Name, sizeof(buf), szParamFile);
	// 	}
	// 	_stprintf(ReadModel, "MOTOR_Used_Def_Name"));
	// 	for(i= 0; i< _MAX_AXIS; i++) {
	// 		Axis_Table[i].Index= i;
	// 		_stprintf(buf, _T("AXIS_%d", i);	GetPrivateProfileString(ReadModel, buf, "a--",  value, sizeof(buf), szParamFile);
	// 		Axis_Table[i].Used= _wtoi(value);
	// 	}
	//---------------------------------------
	strText.Format(_T("[MOTION-DATA-READ] ReadIOTableParamFile() -> Complite! [MODEL-NAME: %s]"), szParamFile); 	SaveLogFile(_SYSTEM_LOG, strText);
	//---------------------------------------
}

//===================================================================================
void CParameterFile::SaveLogFile(int LogType, CString strText)
//-----------------------------------------------------------------------------------
{
	_wsetlocale( LC_ALL, L"korean" );

	TCHAR value[512];
	CString strFile__Name;

	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString m_strDayLogDirName;
	m_strDayLogDirName.Format(_T("%s\\%4d%02d%02d"), strDirSystemLogPath, st.wYear, st.wMonth, st.wDay); // File Name 생성
	_wmkdir(m_strDayLogDirName); // Day Loge Directory 생성. //

	switch(LogType) {
		case _SYSTEM_LOG:
			strFile__Name.Format(_T("%s\\Process.Log_%02d"), m_strDayLogDirName, st.wHour);		break;
		case _ERROR_LOG:
			strFile__Name.Format(_T("%s\\Error.Log"), m_strDayLogDirName);			break;
		case _MOTION_LOG:
			strFile__Name.Format(_T("%s\\Motion.Log"), m_strDayLogDirName);			break;
		case _ACS_MOTION_LOG:
			strFile__Name.Format(_T("%s\\ACS_Motion.Log"), m_strDayLogDirName);	break;
		case _THREAD_LOG:
			strFile__Name.Format(_T("%s\\ThreadFlow_%02d.Log"), m_strDayLogDirName, st.wHour);		break;
		default:
			strFile__Name.Format(_T("%s\\Other.Log"), m_strDayLogDirName);			break;
	}
	_stprintf(value, _T("%s"), strText);
	
	//pFile = _wfopen( L"d:\\Text.txt", L"w"  );
	FILE* fout = _wfopen(strFile__Name.GetBuffer(0), _T("a+"));   // 파일에 계속 추가시킨다. //
	if(fout == NULL) 	{		return;		}

	CString strMsg;
	if(SIMULATION_MODE) 
		strMsg.Format (_T("[%02ld:%02ld:%02ld]\t[SIMULATION_MODE]%s"), st.wHour, st.wMinute, st.wSecond, value); 
	else
		strMsg.Format (_T("[%02ld:%02ld:%02ld]\t%s"), st.wHour, st.wMinute, st.wSecond, value); 
	fwprintf(fout, _T("%s\n"), strMsg);


	fclose(fout);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(pFrm->isView_Init()) pFrm->GetWindowHandler_DebugView()->ListPrint(strMsg);
}
//-----------------------------------------------------------------------------------
//===================================================================================
void CParameterFile::CreratReportFolder()
//-----------------------------------------------------------------------------------
{
	CString strFile__Name;
	CString strData;

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString m_strReportPath;
	_stprintf(strDirReportSavePath, _T("%s\\Report\\%04d%02d%02d_%02d%02d%02d"),
		strDirRootPath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);//strDirMeasImageSavePath
	_tcscpy(strDirMeasImageSavePath, strDirReportSavePath);

	m_strReportPath.Format(_T("%s"), strDirReportSavePath);
	_wmkdir(strDirReportSavePath);

	strFile__Name.Format(_T("%s\\Report_.txt"), strDirReportSavePath); //strDirMeasImageSavePath);
	FILE* fout = _wfopen(strFile__Name.GetBuffer(0), _T("a+"));   // 파일에 계속 추가시킨다. //
	if (fout == NULL) 	{ return; }

	//fwprintf( pFile, L"%d %f %s \n", i, 0.5+i, L"한글" );
	fwprintf(fout, _T(" ---------------------------------------------------------------------------------------------\n"));
	fwprintf(fout, _T(" - 시        간  : %04d년 %02d월 %02d일 %02d시 %02d분\n"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	fwprintf(fout, _T(" ---------------------------------------------------------------------------------------------\n"));
	strData.Format(_T("No.\t시간\tPos\tVel\tTension#1\tTension#2\tA/D#1-1\tA/D#1-2\tA/D#2-1\tA/D#2-2"));
	fwprintf(fout, _T("%s\n"), strData);
	fclose(fout);
}
//===================================================================================
void CParameterFile::Report_Save(int no, DWORD th, double pos, double vel, double ad1, double ad2, double ad11, double ad12, double ad21, double ad22)
//-----------------------------------------------------------------------------------
{
	CString strFile__Name;
	CString strData;

	SYSTEMTIME st;
	GetLocalTime(&st);

	strFile__Name.Format(_T("%s\\Report_.txt"), strDirReportSavePath); //strDirMeasImageSavePath);
	FILE* fout = _wfopen(strFile__Name.GetBuffer(0), _T("a+"));   // 파일에 계속 추가시킨다. //
	if (fout == NULL) 	{ return; }

	strData.Format(_T("%d\t%.1f\t%.3f\t%.3f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t"), 
		no, double(th)/1000.0, pos, vel, ad1, ad2, ad11, ad12, ad21, ad22);
	fwprintf(fout, _T("%s\n"), strData);
	fclose(fout);
}

//===================================================================================
void CParameterFile::StartProcess_Report()
//-----------------------------------------------------------------------------------
{
/*
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	_mkdir(strDirProcessReportSavePath);

	CString strFile__Name;
	CString strData;
	CString strResult;

	strFile__Name.Format("%s\\Report_.txt", strDirProcessReportSavePath); //strDirMeasImageSavePath);

	FILE* fout = _wfopen(strFile__Name.GetBuffer(0), "a+"));   // 파일에 계속 추가시킨다. //
	if(fout == NULL) 	{		return;		}

	fwprintf(fout, " ---------------------------------------------------------------------------------------------\n"));
	fwprintf(fout, " - 작 업 모 델   : %s\n", System_data.strCurrentModelName);
	fwprintf(fout, " ---------------------------------------------------------------------------------------------\n"));
	fwprintf(fout, " - 시        간  : %04d년 %02d월 %02d일 %02d시 %02d분\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	fwprintf(fout, " - 사   용   자  : %s\n", User_data.strUserName );
	fwprintf(fout, " - 사 용   INK   : %s\n", User_data.strUseInk );
	fwprintf(fout, " - 사 용 BLANKET : %s\n", User_data.strUseBlanket );
	fwprintf(fout, " - 온        도  : %3.1f 도                      - 습        도  : %3.1f %%\n", User_data.dUseTemp, User_data.dUseHumidity );
	fwprintf(fout, " -  Pressure     : %3.1f kgf\n", User_data.dPressure);

	fwprintf(fout, " ---------------------------------------------------------------------------------------------\n"));
	CString strA;
	
	fwprintf(fout, " ---------------------------------------------------------------------------------------------\n"));

	fwprintf(fout, " - 메        모  : %s\n", User_data.strMemo );
	fwprintf(fout, " ---------------------------------------------------------------------------------------------\n"));

	strData.Format("No.\t시간\t"));

	strResult.Format("블레이드 동작 수\t블레이드 속도[ms]\t인쇄 속도[ms]\t제판 길이\tRoll-Angle\tRoll-길이\t인쇄 시작 위치-X\tOffset-X\tOffset-Y\tOffset-각도"));
		strData= strData + strResult;
	fwprintf(fout, "%s\n", strData);
	fclose(fout);
*/
}
//===================================================================================
void CParameterFile::Process_Report()
//-----------------------------------------------------------------------------------
{
/*
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString strFile__Name;
	CString strData;
	CString strResult;

	strFile__Name.Format("%s\\Report_.txt", strDirProcessReportSavePath); //strDirMeasImageSavePath);

	FILE* fout = _wfopen(strFile__Name.GetBuffer(0), "a+"));   // 파일에 계속 추가시킨다. //
	if(fout == NULL) 	{		return;		}

	strData.Format("%d\t%02d시 %02d분 %02d초\t",System_data.iPROCESS_RUNNING_COUNT,  st.wHour, st.wMinute, st.wSecond);

	fwprintf(fout, "%s\n", strData);
	
	fclose(fout);
*/
}
