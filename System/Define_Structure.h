#if !defined(_FILE_STRUCTURE_DEFINE_H_)
#define _FILE_STRUCTURE_DEFINE_H_

#include "SystemInfo.h"
#include "../Common/ImageView/Include/xDataTypes.h"
#include <tchar.h>

#define _MAX_COMP_CAL_DATA_COUNT	200
//=================================================
typedef struct _COMP_DATA
{
	int X_Count, Y_Count;

	typedef struct _TData
	{
		double	MovePos;					
		double	GetPos;
		double  Cal_um; //
	}Data;
	Data Top_data_X[_MAX_COMP_CAL_DATA_COUNT];
	Data Btm_data_X[_MAX_COMP_CAL_DATA_COUNT];
	Data Top_data_Y[_MAX_COMP_CAL_DATA_COUNT];
}COMP_DATA;
//#pragma pack(pop, 1)
//-------------------------------------------------
extern COMP_DATA	CompCal_data;

//=================================================
//#pragma pack(push, 1)
typedef struct _RESULT_DATA
{
	int		iAlignEndFlag;
	DPOINT	dptRealPoint_1st;
	DPOINT	dptRealPoint_2nd;
	DPOINT	dptPoint_1st;
	DPOINT	dptPoint_2nd;
	double	dTheta;					
	double	dDistance;
	//2X기준으로 데이터 저장
	double  dPosData[_MAX_AXIS]; //
}RESULT_DATA;
//#pragma pack(pop, 1)
//-------------------------------------------------
extern RESULT_DATA	Align_Result_data[_MAX_ALIGN_RESULT_DATA];
//=================================================
//#pragma pack(push, 1)
typedef struct _MODEL_DATA{

	int SearchROI_left[_MAX_CAM_];
	int SearchROI_right[_MAX_AXIS];
	int SearchROI_top[_MAX_AXIS];
	int SearchROI_bottom[_MAX_AXIS];

	double	dPosition[_MAX_MOVE_SET_POS][_MAX_AXIS];
	double	dPosition_Offset[_MAX_MOVE_SET_POS][_MAX_AXIS];

	//----------------------------
	int		iRunEnable[_MAX_RUN_PARAM];
	int		iWaitFlag[_MAX_RUN_PARAM];
	int		iAlignMode[_MAX_RUN_PARAM];
	int		iRun_RollMode[_MAX_RUN_PARAM];

	double	dRun_Speed[_MAX_RUN_PARAM];
	double	dRun_Length[_MAX_RUN_PARAM];
	double	dRun_Force[_MAX_RUN_PARAM];
	double	dRun_Force2[_MAX_RUN_PARAM];
	double	dRun_DownSpeed[_MAX_RUN_PARAM];
	double	dRun_UpSpeed[_MAX_RUN_PARAM];
	int		iRun_Delay[_MAX_RUN_PARAM];
	double	dRun_Offset_X[_MAX_RUN_PARAM];
	double	dRun_Offset_Y[_MAX_RUN_PARAM];
	//----------------------------
	double  dPlateSheetTmm[_MAX_RUN_PARAM];

	//----------------------------
	double  dAlignMarkToMark[_MAX_RUN_PARAM];
	double  dAlignMarkToMarkY[_MAX_RUN_PARAM];
	int		iMarkSearchCount[_MAX_RUN_PARAM];
	int		iRollAlignModeFlag[_MAX_RUN_PARAM];
	int		iMarkMaxCount;
	double  dAlignAccuracy_umXY;
	double  dAlignAccuracy_umT;
	double  dAlignOffset_X[_MAX_RUN_PARAM];
	double  dAlignOffset_Y[_MAX_RUN_PARAM];
	double  dAlignOffset_T[_MAX_RUN_PARAM];
	//----------------------------
	int iForceControlEnableFlag[_MAX_RUN_PARAM];
	double dZ_Control_Fast_Length[_MAX_RUN_PARAM];
	double dZ_Control_Slow_Length[_MAX_RUN_PARAM];
	int iZ_Control_Mode[_MAX_RUN_PARAM];

	double dStamp_Len_X, dStamp_Len_Y;
	double dStamp_2X_X, dStamp_2X_Y;
	double dStamp_20X_X, dStamp_20X_Y;
	double dStamp_2X_2nd_X, dStamp_2X_2nd_Y;
	double dStamp_20X_2nd_X, dStamp_20X_2nd_Y;

	double dStampToBaseAlignDist_X;
	double dStampToBaseAlignDist_Y;
	double dStamp_Btm_2X_1st_Xpix, dStamp_Btm_2X_1st_Ypix;	//Roll또는 기타 얼라인 하부 픽셀 좌표
	double dStamp_Btm_20X_1st_Xpix, dStamp_Btm_20X_1st_Ypix;//Roll또는 기타 얼라인 하부 픽셀 좌표
	double dStamp_Btm_2X_2nd_Xpix, dStamp_Btm_2X_2nd_Ypix;//Roll또는 기타 얼라인 하부 픽셀 좌표
	double dStamp_Btm_20X_2nd_Xpix, dStamp_Btm_20X_2nd_Ypix;//Roll또는 기타 얼라인 하부 픽셀 좌표

	double dStamp_Top_20X_1st_Xpix, dStamp_Top_20X_1st_Ypix;//Roll대비 상부 얼라인 픽셀 좌표
	double dStamp_Top_20X_2nd_Xpix, dStamp_Top_20X_2nd_Ypix;//Roll대비 상부 얼라인 픽셀 좌표
	double dStamp_Top_2X_1st_Xpix, dStamp_Top_2X_1st_Ypix;//Roll대비 상부 얼라인 픽셀 좌표
	double dStamp_Top_2X_2nd_Xpix, dStamp_Top_2X_2nd_Ypix;//Roll대비 상부 얼라인 픽셀 좌표

	double dZ_SlowDownLen;
	int iDownEnd_WaitFlag;

	int iRoll_AngleAlignMode;
	double dRoll_AngleAlign_Pitch;
	int iRoll_AngleAlign_Count;
	double dLED_Pitch_X, dLED_Pitch_Y;

	double dRollTest_Z1, dRollTest_Z2;
	int iZ_ControlOption;
	int iZ_Control_Z1Hap;
}MODEL_DATA;
//#pragma pack(pop, 1)
//-------------------------------------------------
extern MODEL_DATA	Model_data;
//=================================================
//#pragma pack(push, 1)
typedef struct _MOTION_DATA{
	double	dOrigin_Speed[_MAX_AXIS];
	double	dOrigin_Accel[_MAX_AXIS];
	double	dOrigin_Decel[_MAX_AXIS];

	double	dStart_Speed[_MAX_AXIS];
	double	dWork_Speed[_MAX_AXIS];
	double	dWork_Accel[_MAX_AXIS];
	double	dWork_Decel[_MAX_AXIS];

	double	dJog_MaxSpeed[_MAX_AXIS];
	double	dJog_MidSpeed[_MAX_AXIS];
	double	dJog_MinSpeed[_MAX_AXIS];
	
	double dPulsePerMM[_MAX_AXIS];

	double dSW_Limit_High[_MAX_AXIS];
	double dSW_Limit_Low[_MAX_AXIS];

	double dZ_Axis_Interlock;

	double dZ1_ORG_Offset;
	double dZ2_ORG_Offset;
	double dRollRun_BZ_Pos;
}MOTION_DATA;
//#pragma pack(pop, 1)
//-------------------------------------------------
extern MOTION_DATA	Motion_data;
//=================================================
// #pragma pack(push, 1)
typedef struct _SYSTEM_DATA{ //고정
	int VirtualOS_Enable;

	wchar_t strCurrentModelName[255];	// 현재 사용중인 모델명 기억
	wchar_t strCurrentSlotDieName[255];
	wchar_t strCurrentRecipeName[255];
	int iWORK_MODE_SELECT;

	int iWORK_PROCESS_MODE;
	int iCURRENT_RUN_STATUS;
	bool bEND_STOP_FLAG;

	int	 iPROCESS_CYCLE_COUNT;
	int	 iPROCESS_RUNNING_COUNT;

	int	_HOME_TIME_OUT;
	int	_AXIS_MOVE_TIME_OUT;

	int	_STEP_DELAY_TIME;
	int	_CYL_AFTER_DELAY_TIME;
	int iVac_OFF_Delay;
	
	double _ROLL_Z_AXIS_MIN_;
	double _ROLL_Z_AXIS_MAX_;
	
	double dDancer_1_ZeroVal;

	double dTop_Cam_PixelToum_2X;
	double dTop_Cam_PixelToum_20X;
	double dBot_Cam_PixelToum_2X;
	double dBot_Cam_PixelToum_20X;

	double dTopCam20To2_X;
	double dTopCam20To2_Y;
	double dTopCam20To2_Z;
	double dBotCam20To2_X;
	double dBotCam20To2_Y;
	double dBotCam20To2_Z;

	double dBot_Cam_20X_Offset_X;
	double dBot_Cam_20X_Offset_Y;


	double dStage1ToStage2_Dist, dStage2ToStage3_Dist;
	double dRotCen_PosStageX_20X, dRotCen_PosCamY_20X;

	int iImageGrab_Delay;

	//----------------------------
	double dRoll_Diameter[_MAX_RUN_PARAM];
	double dRoll_Diameter_AddVal[_MAX_RUN_PARAM];
	double dZ_Base_Position[_MAX_RUN_PARAM];
	//----------------------------

	double dmmToDeg;
	// :-----------------:                      :
	//   X_SAFE_ZONE_L1  : : X_SAFE_FREEZONE_L  :
	//   X_SAFE_ZONE_H1  : : X_SAFE_FREEZONE_H  :
	// :-----------------:                    :

	double X_SAFE_FREEZONE_L, X_SAFE_FREEZONE_H;  //간섭이 전혀 없는 부분
	double X_SAFE_ZONE_L1, X_SAFE_ZONE_H1;
	double X_SAFE_ZONE_L2, X_SAFE_ZONE_H2;
	double X_MASK_VIEWZONE_L, X_MASK_VIEWZONE_H;  // 마스크 보는 위치
	double dRollAlignPos_H, dRollAlignPos_L;  // Roll Align 하는 위치

	double dX_Axis_Interlock, dQuartz_PosX, dQuartz_PosY, dRollPlateStamp_PosY, dRollPlateStamp_PosX, dStamp_Y_Max;
	double dShift_CamY;
	double AF_Stepmm_20X;
	double AF_Stepmm_2X;
	int iAF_TotalCount;
	double dStampMoveOffset_X, dStampMoveOffset_Y;
	double dBasePos_Stage1_X, dBasePos_Stage1_CamY, dBasePos_Stage1_AY1, dBasePos_Stage1_AY2;
	double dBasePos_Stage2_X, dBasePos_Stage2_CamY, dBasePos_Stage2_AY1, dBasePos_Stage2_AY2;
	double dBasePos_Stage3_X, dBasePos_Stage3_CamY, dBasePos_Stage3_AY1, dBasePos_Stage3_AY2;

	int iCAM_20X_USED_FLAG;
	double dHome_Offset_BX;
	double TB_Offset_2X_X;//, Roll_Offset_2X;
// 	double Q_Offset_20X, Roll_Offset_20X;
 	double TB_Offset_2X_Y;//, Roll_Offset_2X_Y;
// 	double Q_Offset_20X_Y, Roll_Offset_20X_Y;

	double dZ2HomeOffsetVal;
	int RollAlignPosMode, Led_Remote;

	int iLED_Top_Val_2X[_MAX_RUN_PARAM], iLED_Btm_Val_2X[_MAX_RUN_PARAM];
	int iLED_Top_Val_20X[_MAX_RUN_PARAM], iLED_Btm_Val_20X[_MAX_RUN_PARAM];

	double dKgPerum,	dExcept_XPos,	dExcept_XPos2;

	double dRollAlign_ZPos;
	int iRollTeachingDataUsed;
	double dRectSizeX, dRectSizeY;

	double dCompPitch, dCompRectSize;
	int iCompCountX;
	int iCompCountY;
	int bRollAF_Enable;
}SYSTEM_DATA;
// #pragma pack(pop, 1)
//-------------------------------------------------
extern SYSTEM_DATA	System_data;
//=================================================
//=================================================
// #pragma pack(push, 1)
typedef struct tagIOTABLE
{
	wchar_t Name[100];
	int		Address;
	int		Index;
	int		Type;
}SIOTABLE;
// #pragma pack(pop, 1)
//-------------------------------------------------
extern SIOTABLE	IO_Table;
//=================================================
//-------------------------------------------------
// #pragma pack(push, 1)
//=================================================
// #pragma pack(push, 1)
typedef struct _PATTERN_DATA{
	int		iSpeed;
	int		iAccuracy;
	double	dScoreCertanty;
	double	dScoreAcceptance; 

	int		iTimeOut; 

	double	dOffsetX;
	double	dOffsetY;

}PATTERN_DATA;
// #pragma pack(pop, 1)
//-------------------------------------------------
extern PATTERN_DATA	Pattern_data[_MAX_FIND_MODEL];
//=================================================

//-------------------------------------------------
typedef struct _USER_DATA{
	wchar_t Memo[255];
	wchar_t strUserName[255];
	wchar_t strUseInk[255];
	wchar_t strUseBlanket[255];
	long lUseHour;
	long lUseMinute;
	long lUseDay;
	long lUseMon;
	long lUseYear;
	double dUseTemp;
	double dUseHumidity;
	wchar_t strMemo[1024];

	int iImageLogDisplay;
}USER_DATA;
extern USER_DATA	User_data;

//=================================================
typedef struct _RECIPE_DATA{
	int		iCommand;	//  
	double	dOP_Val1;	//  
	double	dOP_Val2;	//  
	double	dOP_Val3;	//  
	double	dOP_Val4;	//  
	double	dOP_Val5;	//  
	wchar_t	chrOP_Val[255];	//  
}RECIPE_DATA;
extern RECIPE_DATA	Recipe_data[_MAX_RECIPE]; //pSystsem에서 사용

//-------------------------------------------------


#endif

