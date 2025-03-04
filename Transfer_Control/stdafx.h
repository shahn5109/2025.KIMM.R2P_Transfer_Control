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

// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <afxsock.h>            // MFC 소켓 확장



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


