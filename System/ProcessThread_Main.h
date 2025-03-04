// ProcessThread_Main.h: interface for the CProcessThread_Main class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSTHREAD_MAIN_H__DD6BEA67_675B_43CB_9DB8_5ACDF3BCE115__INCLUDED_)
#define AFX_PROCESSTHREAD_MAIN_H__DD6BEA67_675B_43CB_9DB8_5ACDF3BCE115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum enumStepMain
{
	/** 0. Init Resource Manager */
		STEP_MAIN_DUMMY											= -1,					// 0.
		STEP_MAIN_INIT											= 0,
		STEP_MAIN_START											= 10,
		STEP_MAIN_Z_SAFE_POS_START								= 20,
		STEP_MAIN_Z_SAFE_POS_END								= 30,
		STEP_MAIN_READY_POS_START								= 40,
		STEP_MAIN_READY_POS_END									= 50,

		//=====================================================================================
		//Stage Sync. -------------------------			 
		STEP_MAIN_SYNC_PROCESS_CHECK							= 5000,
		STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_START				= 5040,
		STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_END 				= 5050,
		STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_CAL_START			= 5060,
		STEP_MAIN_SYNC_X_ROLL_AXIS_START_POS_CAL_END 			= 5070,
		STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_START 				= 5080,
		STEP_MAIN_SYNC_Z_AXIS_START_POS_DOWN_END 				= 5090,
		STEP_MAIN_SYNC_Z_AXIS_START_POS_DELAY_END 				= 5100,
		STEP_MAIN_SYNC_PICKING_PROC_Z_CONTROL_START				= 5102,
		STEP_MAIN_SYNC_PICKING_PROC_Z_CONTROL_END				= 5104,
		STEP_MAIN_SYNC_PICKING_PROC_RUN_START 					= 5110,
		STEP_MAIN_SYNC_PICKING_PROC_Z_RUN_END					= 5120,
		STEP_MAIN_SYNC_PICKING_PROC_FORCE_RUN_END				= 5130,
		STEP_MAIN_SYNC_PICKING_PROC_RUN_END 					= 5140,
		STEP_MAIN_SYNC_Z_AXIS_START_PROC_END_UP_START 			= 5150,
		STEP_MAIN_SYNC_Z_AXIS_START_PROC_END_UP_END 			= 5160,
		// Stage Sync PROC_END & Z_UP_END ==================		   
		//=====================================================================================
		//-----------------------------------------------
		STEP_MAIN_STEP_RECIPE_START								= 6000,
		STEP_MAIN_STEP_RECIPE_COMMAND_START						= 6010,
		STEP_MAIN_STEP_RECIPE_Z_READY_POS_MOVE_PROC_START		= 6020,
		STEP_MAIN_STEP_RECIPE_Z_READY_POS_MOVE_PROC_END			= 6030,
		STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_START		= 6040,
		STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_Z1_END	= 6050,
		STEP_MAIN_STEP_RECIPE_ALL_READY_POS_MOVE_PROC_ALL_END	= 6060,
		//-----------------------------------------------
		// Roll-ALIGN
		//-----------------------------------------------
		STEP_MAIN_ROLL_ALIGN_PROCESS_CHECK						= 1000,
		STEP_MAIN_ROLL_ALIGN_PROCESS_START						= 1010,
		STEP_MAIN_ROLL_ALIGN_PROCESS_END						= 1020,
		//------------------------------------------------
		STEP_MAIN_STAGE1_ALIGN_PROCESS_CHECK					= 1100,
		STEP_MAIN_STAGE1_ALIGN_PROCESS_START					= 1110,
		STEP_MAIN_STAGE1_ALIGN_PROCESS_END						= 1120,
		//------------------------------------------------		  
		STEP_MAIN_STAGE2_ALIGN_PROCESS_CHECK					= 1200,
		STEP_MAIN_STAGE2_ALIGN_PROCESS_START					= 1210,
		STEP_MAIN_STAGE2_ALIGN_PROCESS_END						= 1220,
		//------------------------------------------------		  
		STEP_MAIN_STAGE3_ALIGN_PROCESS_CHECK					= 1310,
		STEP_MAIN_STAGE3_ALIGN_PROCESS_START					= 1320,
		STEP_MAIN_STAGE3_ALIGN_PROCESS_END						= 1330,
		STEP_MAIN_STAGE_ALIGN_PROCESS_START						= 1500,

		//-----------------------------------------------
		STEP_MAIN_STAGE_PROCESS_CHECK							= 7000,
		STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_START				= 7050,
		STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_START			= 7100,
		STEP_MAIN_STAGE_X_ROLL_AXIS_START_POS_CAL_END			= 7150,
		STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START				= 7200,
		STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END				= 7250,
		STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_START_SLOW		= 7260,
		STEP_MAIN_STAGE_Z_AXIS_START_POS_DOWN_END_SLOW			= 7270,
		STEP_MAIN_STAGE_Z_AXIS_START_POS_DELAY_END				= 7300,
		STEP_MAIN_STAGE_PICKING_PROC_RUN_START					= 7350,
		STEP_MAIN_STAGE_PICKING_PROC_RUN_END					= 7400,
		STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_WAIT		= 7450,
		STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_SLOW		= 7500,
		STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START_SLOW_END	= 7550,
		STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_START			= 7600,
		STEP_MAIN_STAGE_Z_AXIS_START_PROC_END_UP_END			= 7650,
		//-----------------------------------------------
		STEP_MAIN_STEP_RECIPE_COMMAND_END						= 8000,
		STEP_MAIN_STEP_RECIPE_END_CHECK							= 8100,
	//=====================================================================================

		STEP_MAIN_END_READY_POS_START1							= 9000,
		STEP_MAIN_END_READY_POS_START2							= 9100,
		STEP_MAIN_END_READY_POS_END								= 9200,

		STEP_MAIN_END											= 9900,					// 0.
		STEP_MAIN_ERROR											= 9999,					// 0.
} EStepMain;

typedef enum enumStepDisp
{
	STEP_DISP_DUMMY= -1,					// 0.
	STEP_DISP_INIT= 0,				
	STEP_DISP_STEP_START, 

	STEP_DISP_STEP_START_POS,
	STEP_DISP_STEP_START_POS_END,
	STEP_DISP_STEP_END_POS,	
	STEP_DISP_STEP_END_POS_END,
	STEP_DISP_STEP_END=   90,					// 0.
	STEP_DISP_STEP_ERROR= 99,					// 0.
} EStepDisp;

class CProcessThread_Main  
{
public:
	CProcessThread_Main();
	virtual ~CProcessThread_Main();

	int RecipeProc_Step;
	int CuringCount;
	double realTime;

	double Start_Enc_Val, End_Enc_Val,	Cal_Enc_Val, Cal_Diam, Cal_Tmp;

	int OK_Count;
	bool bServo_CheckFlag;
	
	DWORD ServoCheckTime;
	bool Z_Offset_Calib(int  no);

	int  iGetStep();
	void ProcessStart();
	void ProcessStop();
	BOOL IsAutoRun();

	EStepMain GetStep();

	void StepInitial();

	BOOL TimeOutCheck();

	void StepProcessStart();
	void StepProcessStop();

	void StepDispenserStart();
	void StepDispenserStop();

	void AlignDataClear();
	void isDoneError();
	BOOL isAll_Done();

	bool bNoz_CheckFlag, bTank_CheckFlag;

	DWORD dwCheckTimeStart;
	DWORD dwCheckTimeStart_Axis;
	DWORD dwCheckTimeStart_Total;

	DWORD dwCheckTimeStart_X;
	DWORD dwCheckTimeStart_Z;
	DWORD dwCheckTimeStart_Roll;
	DWORD dwCheckTimeStart_Wait;
	DWORD dwCheckTimeStart_PROC;
	DWORD dwCheckTimeStart_Z_DOWN;
	DWORD dwCheckTimeStart_Cyl;
	DWORD dwCheckProcessStartTime;
//	DWORD dwDryCheckTime;

	DWORD dwCheckDryTime;

	double Auto_Roll_Deg;
	double Auto_Roll_X;
	double Auto_Roll_Y;
	double Auto_MV_To_X, Auto_MV_To_Y, Auto_MV_To_T;

	DWORD dwCheckTimePrintDelay;
	DWORD dwCheckTimeDoctoringDelay, dwOffsetTime;

	bool ext_jog_flag;

	BOOL  bEndStopFlag;
	bool	bZAlm_CheckFlag;

	int iInk_Apply_Count;

	int Z_Offset_Count;
	double  Z_1st_Offset_AD1;
	double  Z_1st_Offset_AD2;

	double L_Move, BR_Move, PR_Move;

	bool bFirstStartFlag;

	int Stage_Proc_No, Stage_No ,Stage_Pos_No;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

private:
	bool bEMO_CheckFlag, bAIR_CheckFlag;

	/**Ready 상태*/
	BOOL m_bFlagIsReleaseReady;
		
	EStepMain m_eAutoStep;
	EStepMain m_eOldAutoStep;
	EStepDisp m_eAutoDispStep;
	EStepDisp m_eOldAutoDispStep;
	/**
	 * CInspConveyorProcess의 Run Flag	
	 */
	BOOL m_bAutoRun;
	
	/**
	 * CInspConveyorProcess의 Initialize Flag	
	 */
	BOOL m_bAutoInitialized;

	/** Thread 구동 Flag*/
	BOOL m_bThreadLife;
	/** Thread의 Terminate Flag */
	BOOL m_bThreadTerminate;
	/** Thread Handle */
	CWinThread* m_pThread;	
	
	/** 
	 * Thread 함수
	 */
	static UINT threadFunction(LPVOID pParam);

	/**
     * CInspConveyorProcess Thread의 Job을 실행 하는 함수	
	 * AutoManager State 감시 및 doRunStep 호출
     */
	void threadJob();

	/**
	 * CInspConveyorProcess Step을 실행 하는 함수
	 */
//	void MODE1_doRunStep();
//	void MODE2_doRunStep();
//	void MODE34_doRunStep();
	void Recipe_doRunStep();

	void 	doRunStep();

	/** 
	 * Set CInspConveyorProcess Thread Step 
	 */
	void setStep(EStepMain estepNext);

protected:
	bool IsIO_Init_Flag;


};

#endif // !defined(AFX_PROCESSTHREAD_MAIN_H__DD6BEA67_675B_43CB_9DB8_5ACDF3BCE115__INCLUDED_)
