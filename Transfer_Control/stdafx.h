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

// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��



//-------------------------------------------------------------------------------

#pragma warning (disable : 4996)

//-------------------------------------------------------------------------------
#include "SystemClass.h"
#include "Common_Tool.h"
#include "UserDefine.h"

extern BOOL	SIMULATION_MODE;

extern BOOL PROGRAM_EXIT;

extern BOOL	g_ServoCheckFlag, g_bZ_InterLockFlag;
extern int	g_AxisHomeStat[_MAX_AXIS];

extern int g_FootSW_Dir;

extern BOOL g_AutoRunAlignFlag;
//--------------------------------------------------------------------------------
extern int g_iCurrViewNo ;
//--------------------------------------------------------------------------------
extern BOOL g_bCfrmTitleViewInit;
extern BOOL g_bCfrmSubMenuViewInit;
extern BOOL g_bCfrmMainViewInit;
extern BOOL g_bCfrmManualViewInit;
extern BOOL g_bCfrmCameraViewInit;
extern BOOL g_bCfrmTeachViewInit;
extern BOOL g_bCfrmAlignViewInit;
extern BOOL g_bCfrmCalibViewInit;
extern BOOL g_bCfrmSetupViewInit;
extern BOOL g_bCfrmDebugViewInit;
extern BOOL g_bCfrmDataViewInit;
extern BOOL g_bCfrmStampViewInit, g_bCfrmCompViewInit;
//-----------------------------------------------

extern double *PlotData_Time_1; 
extern double *PlotData_L1_1; 
extern double *PlotData_L2_1; 
extern double *PlotData_XP_1; 
extern double *PlotData_ZP_1; 
extern double *PlotData_Z2P_1; 
extern double *PlotData_XS_1; 
extern double *PlotData_Enc_1; 

extern double *PlotData_Time_2; 
extern double *PlotData_L1_2; 
extern double *PlotData_L2_2; 
extern double *PlotData_XP_2; 
extern double *PlotData_ZP_2; 
extern double *PlotData_Z2P_2; 
extern double *PlotData_XS_2; 
extern double *PlotData_Enc_2; 

extern double *PlotData_Time_3; 
extern double *PlotData_L1_3; 
extern double *PlotData_L2_3; 
extern double *PlotData_XP_3; 
extern double *PlotData_ZP_3; 
extern double *PlotData_Z2P_3; 
extern double *PlotData_XS_3; 
extern double *PlotData_Enc_3; 

extern double *PlotData_Time_4; 
extern double *PlotData_L1_S; 
extern double *PlotData_L2_S; 
extern double *PlotData_XP_S; 
extern double *PlotData_ZP_S; 
extern double *PlotData_Z2P_S; 
extern double *PlotData_XS_S; 
extern double *PlotData_Enc_S; 

extern double MAT_Org_Data_Z1[_MAX_MAT_DATA];
extern double MAT_Org_Data_Z2[_MAX_MAT_DATA];
extern double MAT_Down_Data_Z1[_MAX_MAT_DATA];
extern double MAT_Down_Data_Z2[_MAX_MAT_DATA];
extern double dMAT_COR_Data_Z1[_MAX_MAT_DATA];
extern double dMAT_COR_Data_Z2[_MAX_MAT_DATA];

extern double LED_Find_X_PixPos[_MAX_LED_FIND_COUNT], LED_Find_Y_PixPos[_MAX_LED_FIND_COUNT];
//-----------------------------------------------
extern int g_ImageSaveCount;
extern bool g_ImageSaveFlag_Test, g_Wait_Flag;

extern double g_dBtm_1st_MaskMark_LX, g_dBtm_1st_MaskMark_BX, g_dBtm_1st_MaskMark_BY, g_dBtm_2nd_MaskMark_LX, g_dBtm_2nd_MaskMark_BX, g_dBtm_2nd_MaskMark_BY;
extern int g_Led_Sel_Num;

#define _MAX_ROLL_ALING_DATA	300
extern double RollAlign_X_Pix[_MAX_ROLL_ALING_DATA], RollAlign_Y_Pix[_MAX_ROLL_ALING_DATA], RollAlign_Angle[_MAX_ROLL_ALING_DATA];
extern double RollAlign_Pos[_MAX_ROLL_ALING_DATA];

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


