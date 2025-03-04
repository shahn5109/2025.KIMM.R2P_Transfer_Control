#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H


//////////////////////////////////////////////////////////////////////
//		End-User
//////////////////////////////////////////////////////////////////////
#include "KIMM_Transfer_Define.h"
//#include "LGE_Transfer_Define.h"
//#define _PROGRAM_ROOT_DIR	_T("D:\\Transfer_Program&Config")
//////////////////////////////////////////////////////////////////////
#include "VisionInfo.h"
//////////////////////////////////////////////////////////////////////
#define _PROGRAM_ROOT_DIR	_T("D:\\Transfer_Program&Config")

/**
 * SystemInfo.h : This file defines object type of System.
 *            
 *
 */
////////////////////////////////////////////////////////////////////// 
#define	PI			3.1415926535897932384626433832795
#define _PI_		3.1415926535897932384626433832795
#define RAD			3.1415926535897932384626433832795 / 180.0
#define DEGREE		180.0 / 3.1415926535897932384626433832795

#define _MAX_MAT_DATA		650
#define _MAX_LED_FIND_COUNT 300
#define _ERROR_RETURN	FALSE
#define _OK_RETURN		TRUE

#define _LAYER_MC4U_IP		_T("10.0.0.100")
#define _LAYER_MC4U_PORT		701


#define _LED_1CH	0
#define _LED_2CH	1
#define _LED_3CH	2
#define _LED_4CH	3
//------------------------------------------------------------------------------------
#define _MAX_MOVE_SET_POS	17
enum
{
	POSITION_READY				= 0,
	POSITION_STAGE1_ALIGN		= 1,
	POSITION_STAGE1_ALIGN_END	= 2,
	POSITION_STAGE2_ALIGN		= 3,
	POSITION_STAGE2_ALIGN_END	= 4,
	POSITION_ROLL_ALIGN			= 5,
	POSITION_ROLL_ALIGN_END		= 6,
	POSITION_SYNC				= 7,
	POSITION_SYNC_END			= 8,
	POSITION_TOP_MASK			= 9,
	POSITION_TOP_MASK_20X		= 10,
	POSITION_BTM_MASK			= 11, //ÁÂ
	POSITION_BTM_MASK_20X		= 12, //ÁÂ
	POSITION_STAGE3_ALIGN		= 13,
	POSITION_STAGE3_ALIGN_END	= 14,
	POSITION_X_AXIS_COMP_TOP_START	= 15, //X-Axis Compensation-Position
	POSITION_X_AXIS_COMP_BTM_START	= 16, //X-Axis Compensation-Position
};
enum
{
	STAGE_SEL_1 = 0,
	STAGE_SEL_2 = 1,
};
enum
{
	ALIGN_STAGE_X = 0,
	ALIGN_STAGE_Y = 1,
	ALIGN_STAGE_T = 2,
};
enum
{
	COMP_POS_TOP_START = 15,
	COMP_POS_TOP_END = 16,
	COMP_POS_BTM_START = 17,
	COMP_POS_BTM_END = 18,
};

enum
{
	SEL_STAMP_MODE= 0,
	SEL_DEF_MASK_MODE= 1,
	SEL_MANUAL_CENTER_MODE= 2,
};

#define	_MAX_RUN_PARAM			7
#define _MAX_ALIGN_RESULT_DATA	7
enum PROC_IDX
{
	ROLL_ALIGN_PROC		= 0,
	STAGE_1_ALIGN_PROC	= 1,
	STAGE_2_ALIGN_PROC	= 2,
	STAGE_3_ALIGN_PROC	= 3,
	SYNC_PROC			= 4,
	MANUAL_STAMP_PROC	= 5,  //½ºÅÆÇÁ ÀåÂø¿ë
	MANUAL_ROLL_ALIGN_PROC	= 6, //¸Þ´º¾ó ROLL ¾ó¶óÀÎ
	MANUAL_QT_ROLL_ALIGN_PROC = 7,
	X_AXIS_COMP_PROC = 8
};

enum
{
	STAGE_1_GRAPH	= 0,
	STAGE_2_GRAPH	= 1,
	STAGE_3_GRAPH	= 2,
	SYNC_GRAPH		= 3,
	GRAPH_TMP		= 4,
};
enum
{
	RUN_DISABLE		= 0,
	RUN_ENABLE		= 1,
};
enum
{
	RUN_ROLL_MODE		= 0,
	RUN_PLATE_MODE		= 1,
};
enum
{
	ALIGN_NONE_MODE		= 0,
	ALIGN_AUTO_MODE		= 1,
	ALIGN_MANU_MODE		= 2,
};
enum
{
	_POSITION_MODE = 0,
	_FORCE_MODE = 1,
};

enum{
	_OPEN_POS_MODE = 0,
	_QT_POS_MODE= 1,
};
//------------------------------------------------------------------------------------
#define _MAX_STAGE_NO		3
#define _STAGE_1			0
#define _STAGE_2			1
#define _STAGE_3			2
#define _STAGE_4			3
#define _STAGE_NONE			9
//------------------------------------------------------------------------------------


#define _VEL_MOVE_STARTPOS	TRUE
#define _VEL_MOVE_APPLYPOS	FALSE
//------------------------------------------------------------------------------------

#define _MAIN_DISP_VIEW			0
#define _MENU_DISP_VIEW			1
#define _TEACH_DISP_VIEW		2
#define _SETUP_DISP_VIEW		3
#define _ALIGN_DISP_VIEW		4
#define _CALIB_DISP_VIEW		5
#define _STAMP_DISP_VIEW		6
#define _COMP_DISP_VIEW			7

//------------------------------------------------------------------------------------
//	int iWORK_MODE_SELECT
//	int iWORK_PROCESS_MODE;
#define _PROCESS_AUTO_RUN		0
#define _PROCESS_CYCLE_RUN		1

//	int iCURRENT_RUN_STATUS;
#define _CURRENT_RUN_STOP		0
#define _CURRENT_RUN_READY		1
#define _CURRENT_RUN_RUN		2
#define _CURRENT_RUN_PAUSE		3
#define _CURRENT_RUN_ERROR		9
#define _CURRENT_RUN_ERROR1		10
#define _CURRENT_RUN_ERROR2		11
#define _CURRENT_RUN_ERROR3		12

//	BOOL bEND_STOP_FLAG;
#define _CURRENT_ENDSTOP_DIS		FALSE
#define _CURRENT_ENDSTOP_ENA		TRUE
//------------------------------------------------------------------------------------
#define _MAX_MEASURE_NO		20 

#define _MAX_RESULT_DATA	100 
#define _MAX_DATA_OUT_NO	10
#define _MAX_GRAPH_DATA		100

//------------------------------------------------------------------------------------
#define	WM_MSGBOX_MSG				(WM_USER + 100)		// MyMessageBox ¶ç¿ì±â
//------------------------------------------------------------------------------------
/** System Error Define */
const	int	ERR_SYSTEM_SUCCESS = 1;		// ¼º°ø Ã³¸® Return Value
const	int	ERR_SYSTEM_CANNOT_FIND_OBJECT = 0;
const	int	ERR_SYSTEM_INITIAL_OK = 1;		// ¼º°ø Ã³¸® Return Value
const	int	ERR_SYSTEM_INITIAL_ERROR = 0;
//------------------------------------------------------------------------------------

#define _Z2_OFFSET_READY					0
#define _Z2_OFFSET_OFF_PRINT				1
#define _Z2_OFFSET_SET_PRINT				2

typedef enum _TCmBool
{
	cmFALSE, cmTRUE
}TCmBool;

typedef enum _MoveDir
{
	_DIR_CCW /*(-)Dir*/, _DIR_CW /*(+)Dir*/
}MoveDir;

typedef enum _JogSpeed
{
	_MAX_SPEED, _MID_SPEED, _MIN_SPEED, _PTP_MOVE
}JogSpeed;
enum { _SYNC_MOVE = 0, _NON_SYNC_MOVE = 1 };

#define _JOG_MODE	0
#define _SPD_MODE	1
//====================================================================================
#define _SYSTEM_LOG		0
#define _ERROR_LOG		1
#define _MOTION_LOG		2
#define _THREAD_LOG		3
#define _OPC_LOG		4
//------------------------------------------------------------------------------------
#define _ON				1
#define _OFF			0
#define _ERROR_RETURN	FALSE
#define _OK_RETURN		TRUE

#define _STEP_NONE		0
#define _STEP_RUN		1
#define _STEP_ERROR		2
#define _STEP_SUCCESS	9

#define _STEP1			0
#define _STEP2			1
#define _STEP3			2
#define _STEP4			3
#define _STEP5			4
//------------------------------------------------------------------------------------
#define _SYSTEM_LOG		0
#define _ERROR_LOG		1
#define _MOTION_LOG		2
#define _THREAD_LOG		3
#define _ACS_MOTION_LOG 4
//------------------------------------------------------------------------------------
#define _MT_NONE			0
#define _MT_START		1
#define _MT_RUN			2
#define _MT_STOP			3
#define _MT_ERR			4
#define _MT_END			9
#define _MT_WAIT			8
#define _MT_ALL			99
//------------------------------------------------------------------------------------
enum { _LED_CH1 = 1, _LED_CH2 = 2, _LED_CH3 = 3, _LED_CH4 = 4 };
enum { _NORMAL_SELECT = 0, _FIRST_SELECT = 1, _CENTER_SELECT = 2, _LAST_SELECT = 3 };
//-----------------------------------------------------------------------------------
#define _MAX_RECIPE				30
#define _MAX_RECIPE_COMMAND		14
#define _OP_NONE				0
#define _OP_MOVE_WAIT_ALL		1
#define _OP_MOVE_WAIT_X			2
#define _OP_MOVE_WAIT_Y			3
#define _OP_MOVE_WAIT_RZ		4
#define _OP_PROC_STAGE1			5
#define _OP_PROC_STAGE2			6
#define _OP_PROC_STAGE3			7
#define _OP_PROC_PLATE_ALIGN	8
#define _OP_PROC_VACUUM			10
#define _OP_PROC_DELAY			11
#define _OP_PROC_MESSAGE		12
#define _OP_PROC_END			13
//-----------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
enum { ORIGIN_TIMER_ID = 100, ORIGIN_TIMER_INTERVAL = 200 };
enum { frmTitleView_TIMER_ID = 101, frmTitleView_TIMER_INTERVAL = 1000 };
enum { frmTeachView_TIMER_ID = 102, frmTeachView_TIMER_INTERVAL = 200 };
enum { frmDebugView_TIMER_ID = 103, frmDebugView_TIMER_INTERVAL = 500 };
enum { frmMainView_TIMER_ID = 104, frmMainView_TIMER_INTERVAL = 200 };
enum { frmSubMenuView_TIMER_ID = 105, frmSubMenuView_TIMER_INTERVAL = 1000 };
enum { frmStampView_TIMER_ID = 106, frmStampView_TIMER_INTERVAL = 250 };
enum { frmCompView_TIMER_ID = 107, frmCompView_TIMER_INTERVAL = 500 };

enum { dlgManual_IO_Monitor_TIMER_ID = 201, dlgManual_IO_MonitorSync = 200 };
enum { dlgData_MotionStatus_TIMER_ID = 202, dlgData_MotionStatus_TIMER_INTERVAL = 200 };
enum { dlgManuAlign_TIMER_ID = 203, dlgManuAlign_TIMER_INTERVAL = 250 };
enum { dlgManuStampCalib_TIMER_ID = 204, dlgManuStampCalib_TIMER_INTERVAL = 250 };
enum { dlgRollForce_Test_TIMER_ID1 = 205, dlgRollForce_Test_TIMER_INTERVAL1 = 200 };
enum { dlgRollForce_Test_TIMER_ID2 = 206, dlgRollForce_Test_TIMER_INTERVAL2 = 200 };
enum { dlgRollForce_Test_TIMER_ID3 = 207, dlgRollForce_Test_TIMER_INTERVAL3 = 200 };

enum { dlgStartWait_TIMER_ID = 121, dlgStartWaitSync = 500 };
enum { ORIGIN_TOTAL_TIMER_ID = 113, ORIGIN_TOTAL_TIMER_INTERVAL = 500 };
enum { TEN_DEBUG_TIMER_ID = 115, TEN_DEBUG_TIMER_INTERVAL = 250 };

#define UPDATE_ITEMPANE_EVENT	114

//------------------------------------------------------------------------------------

#define _CYL_TIME_OUT	3000

#define WMU_MotInterrupt				(WM_APP + 1)

#define WM_MSG_DATA_UPDATE 				(WM_USER + 104)

#define WM_MSG_PAT_ROI_UPDATE				(WM_USER + 200)
#define WM_MSG_PAT_ROI_END_SET_L_1		(WM_USER + 201)	

#define WM_MSG_PAT_ROI_START_SET_L_2	(WM_USER + 202)
#define WM_MSG_PAT_ROI_END_SET_L_2		(WM_USER + 203)	

#define WM_MSG_GRAPH_STOP				(WM_USER + 210)	
#define WM_MSG_RESULT_UPDATE_1			(WM_USER + 211)	

#define WM_MSG_CMD_PAT_FIND_1			(WM_USER + 220)	
#define WM_MSG_CMD_PAT_FIND_2			(WM_USER + 221)	

#define WM_MSG_OFF_GRAPH_DATA_UPDATE1	(WM_USER + 301)	
#define WM_MSG_OFF_GRAPH_DATA_UPDATE2	(WM_USER + 302)	
#define WM_MSG_OFF_GRAPH_DATA_UPDATE3	(WM_USER + 303)	
#define WM_MSG_OFF_GRAPH_DATA_UPDATE4	(WM_USER + 304)	
#define WM_MSG_OFF_GRAPH_DATA_UPDATE5	(WM_USER + 305)	

#define WM_MSG_GRAB_START				(WM_USER + 500)	
#define WM_MSG_LED_UPDATE				(WM_USER + 600)	

#define WM_MSG_TOP_LOADIMAGE			(WM_USER + 601)
#define WM_MSG_BTM_LOADIMAGE			(WM_USER + 602)
#define WM_MSG_GRID_COMPUPDATE			(WM_USER + 603)

//------------------------------------------------------------------------------------

#endif
