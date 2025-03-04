// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://msdn.microsoft.com/officeui�� �����Ͻʽÿ�.
//
// Copyright (C) Microsoft Corporation
// ��� �Ǹ� ����.

// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// Transfer_Control.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

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