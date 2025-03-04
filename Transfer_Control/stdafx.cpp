// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// 모든 권리 보유.

// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// Transfer_Control.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

#include "SystemClass.h"
//////////////////////////////////////////////////////////////////////////////////
CSystemClass pSystem;

//Tracker_info m_poly, m_oldpoly, m_tmppoly;

BOOL	SIMULATION_MODE = FALSE;

//--------------------------------------------------------------------------------
int g_iCurrViewNo = 0;
//--------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
// View-Init Flag
//-------------------------------------------------------------------------------
BOOL g_bCfrmTitleViewInit = FALSE;
BOOL g_bCfrmSubMenuViewInit = FALSE;
BOOL g_bCfrmMainViewInit = FALSE;
BOOL g_bCfrmManualViewInit = FALSE;
BOOL g_bCfrmCameraViewInit = FALSE;
BOOL g_bCfrmTeachViewInit = FALSE;
BOOL g_bCfrmAlignViewInit = FALSE;
BOOL g_bCfrmCalibViewInit = FALSE;
BOOL g_bCfrmSetupViewInit = FALSE;
BOOL g_bCfrmDebugViewInit = FALSE;
BOOL g_bCfrmDataViewInit = FALSE;
BOOL g_bCfrmStampViewInit= FALSE;
BOOL g_bCfrmCompViewInit= FALSE;
//-------------------------------------------------------------------------------

BOOL	g_ServoCheckFlag = FALSE;

BOOL PROGRAM_EXIT = FALSE;

int	g_AxisHomeStat[_MAX_AXIS];

BOOL g_bZ_InterLockFlag= TRUE;

int g_FootSW_Dir= _DIR_CW;

BOOL g_AutoRunAlignFlag= TRUE;
//-----------------------------------------------
double *PlotData_Time_1; 
double *PlotData_L1_1; 
double *PlotData_L2_1; 
double *PlotData_XP_1; 
double *PlotData_ZP_1; 
double *PlotData_Z2P_1; 
double *PlotData_XS_1; 
double *PlotData_Enc_1; 

double *PlotData_Time_2; 
double *PlotData_L1_2; 
double *PlotData_L2_2; 
double *PlotData_XP_2; 
double *PlotData_ZP_2; 
double *PlotData_Z2P_2; 
double *PlotData_XS_2; 
double *PlotData_Enc_2; 

double *PlotData_Time_3; 
double *PlotData_L1_3; 
double *PlotData_L2_3; 
double *PlotData_XP_3; 
double *PlotData_ZP_3; 
double *PlotData_Z2P_3; 
double *PlotData_XS_3; 
double *PlotData_Enc_3; 

double *PlotData_Time_4; 
double *PlotData_L1_S; 
double *PlotData_L2_S; 
double *PlotData_XP_S; 
double *PlotData_ZP_S; 
double *PlotData_Z2P_S; 
double *PlotData_XS_S; 
double *PlotData_Enc_S; 

double MAT_Org_Data_Z1[_MAX_MAT_DATA];
double MAT_Org_Data_Z2[_MAX_MAT_DATA];
double MAT_Down_Data_Z1[_MAX_MAT_DATA];
double MAT_Down_Data_Z2[_MAX_MAT_DATA];
double dMAT_COR_Data_Z1[_MAX_MAT_DATA];
double dMAT_COR_Data_Z2[_MAX_MAT_DATA];

double g_dBtm_1st_MaskMark_LX= 0.0, g_dBtm_1st_MaskMark_BX= 0.0, g_dBtm_1st_MaskMark_BY= 0.0, g_dBtm_2nd_MaskMark_LX= 0.0, g_dBtm_2nd_MaskMark_BX= 0.0, g_dBtm_2nd_MaskMark_BY= 0.0;

//-----------------------------------------------

int g_ImageSaveCount= 0;
bool g_ImageSaveFlag_Test= false;
bool g_Wait_Flag= false;

int g_Led_Sel_Num= ePLATE_3;

double RollAlign_X_Pix[_MAX_ROLL_ALING_DATA], RollAlign_Y_Pix[_MAX_ROLL_ALING_DATA], RollAlign_Angle[_MAX_ROLL_ALING_DATA];
double RollAlign_Pos[_MAX_ROLL_ALING_DATA];

double LED_Find_X_PixPos[_MAX_LED_FIND_COUNT], LED_Find_Y_PixPos[_MAX_LED_FIND_COUNT];