// Comizoa_Class.cpp: implementation of the CComizoa_Class class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "R2R_Control.h"
#include "Comizoa_Class.h"

#include "mmsystem.h"
#include "Cmmsdk.h"
//#include "CmmsdkDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSystemClass pSystem;
CParameterFile* CComizoa_Class::ModelFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CComizoa_Class::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
 
void WINAPI InterruptCallback (LPVOID lParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//*****************************************************************************
// GetEventIntString(): 이벤트 인터럽트 상태코드의 각 비트별로 의미하는 인터럽트
// 이름을 스트링으로 반환해주는 함수.
//*****************************************************************************
void GetEventIntString(int nBitIdx, char *szBuffer, int nBufferLen)
{
	char *szEvtState[] = {
		"Normal Stop", // 0
		"Succesive start", // 1
		"", // 2
		"", // 3
		"Start of acceleration", //4
		"End of acceleration", // 5
		"Start of deceleration", // 6
		"End of deceleration", // 7
		"", // 8
		"", // 9
		"Position error tolerance exceed", // 10
		"General Comparator", // 11
		"Compared trigger", // 12
		"CLR signal resets the counter value", // 13
		"LTC input making counter value latched", // 14
		"ORG input signal ON", // 15
		"SD input signal ON", // 16
		"+DR input changes", // 17
		"-DR input changes" // 18
	};

	int nStrLen = strlen(szEvtState[nBitIdx]);
	if(nBufferLen > nStrLen){ // 버퍼크기가 충분하면 full string을 복사해주고...
		strcpy(szBuffer, szEvtState[nBitIdx]);
	}else{ // 그렇지 않으면 버퍼크기에 맞는 문자수만 전달해준다.
		memcpy(szBuffer, szEvtState[nBitIdx], nBufferLen-1);
		szBuffer[nBufferLen-1] = '\0';
	}
}

/***********************************************************************************************
* InterrHandlerCommon(): 각 Event Handler에서 공통으로 호출하는 함수. 
************************************************************************************************/
void InterruptHandlerCommon(LPVOID lParam)
{
/*
	long dwFlag1, dwFlag2;
	BOOL bIsTheAxis = FALSE;
	long dwErrState =0, dwEvtState= 0;
	int nAxis = _AXIS_TMP_ENC;

	cmmIntReadFlag(&dwFlag1, &dwFlag2);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 본 예제에서는 지정한 축의 인터럽트만 처리하는 것이므로 지정한 축의 인터럽트가 발생했는지 체크한다. 
	// cmmIntReadFlag() 함수 대신에 cmmIntReadErrorStatus()와 cmmIntReadEventStatus()를 사용해서도
	// 인터럽트가 발생되었는지를 체크할 수 있다. 다만 여러축의 인터럽트 상태를 체크할 때에는 
	// cmmIntReadFlag() 함수를 사용하는 것이 처리속도 측면에서 상당히 유리하다. 
	if(nAxis < 32){
		bIsTheAxis = cmmGetBit(dwFlag1, nAxis);
	}else{
		bIsTheAxis = cmmGetBit(dwFlag2, (nAxis-32));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 참고: GUI 관련 작업을 이 함수내에서 수행하는 것을 피하기 위하여 List에 보여줄 아이템들을 CArray
	// 변수인 CInterruptEventDlg 클래스의 m_IntItemArray 멤버변수에 저장하고, CInterruptEventDlg 타이머
	// 이벤트에서 m_IntItemArray 멤버에 저장된 아이템들을 디스플레이하는 방식으로 구성되었다.
	
	// 해당축에 인터럽트가 발생되었으면... //
	if(bIsTheAxis){
		pSystem.GetMotionComponent()->Winder_Stop();
//		AfxMessageBox("...");
	}
*/
}

/***********************************************************************************************
*                       CALLBACK 방식 처리 
************************************************************************************************/
void WINAPI InterruptCallback (LPVOID lParam)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// 주의: 쓰레드내에서 GUI 관련작업을 수행하면 에러를 발생시키는 것과 마찬가지로 CALLBACK
	// 함수내에서 GUI 관련작업을 수행하면 예기치않은 에러를 발생시킬 수 있으므로, 가급적 
	// GUI 관련작업은 피하는 것이 좋다. 단, GUI의 속성을 참조하는 것은 상관없다.
	InterruptHandlerCommon(lParam);
}

//===================================================================================
CComizoa_Class::CComizoa_Class()
//-----------------------------------------------------------------------------------
{
	int i;
	for(i= 0; i< 16; i++) {
		AxisHomeStat[i]= _M_NONE;
	}
		lOutPulse_X= 0;		lOutPulse_T= 0;		lOutPulse_B= 0;
		dOutPulse_X= 0.0;	dOutPulse_T= 0.0;	dOutPulse_B= 0.0;
	COMI_Initial= FALSE;

	ReadyCheckFlagMsg= false;

	bSercoOnCheckFlag= true;
}
//===================================================================================
CComizoa_Class::~CComizoa_Class()
//-----------------------------------------------------------------------------------
{

}
//===================================================================================
BOOL CComizoa_Class::Open()
//-----------------------------------------------------------------------------------
{
	// Load CMMSDK(DLL) Library //
	Log()->SaveLogFile(_MOTION_LOG, "[FUNC] Open()");
	if(!cmmLoadDll()){
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] Can't load CMMSDK Library(DLL)");
//		MessageBox("Can't load CMMSDK Library(DLL)", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Load Motion & DIO devices //
	long nNumAxes;
	if(cmmGnDeviceLoad(TRUE, &nNumAxes) != cmERR_NONE){
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] COMI-Device Load");

//		cmmErrShowLast(GetSafeHwnd()); // 에러 발생하였으면 에러 디스플레이합니다.
		return FALSE;
	}else{
		// Motion 환경 초기화 //

		if(!InitMotionDevices()) {
			Log()->SaveLogFile(_MOTION_LOG, "[ERROR] COMI-Device Initial");
			return FALSE;
		}
		COMI_Initial= TRUE;

		ServoAllOn(TRUE);

	}
	Log()->SaveLogFile(_MOTION_LOG, "[FUNC] Open() - SUCCESS!");
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Close()
//-----------------------------------------------------------------------------------
{
	MotorAllStop();
	ServoAllOn(FALSE);
	Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z1_SYNC]);
	Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z1_SYNC]);
	
	cmmGnDeviceUnload(); // Unload Device
	Log()->SaveLogFile(_MOTION_LOG, "[COMI] COMI-Device Unload");
	cmmUnloadDll(); // Unload DLL 	
	Log()->SaveLogFile(_MOTION_LOG, "[COMI] Unload CMMSDK Library(DLL)");
	COMI_Initial= FALSE;

	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isMotionInitial()
//-----------------------------------------------------------------------------------
{
	return COMI_Initial;
}
//===================================================================================
BOOL CComizoa_Class::InitMotionDevices()
//-----------------------------------------------------------------------------------
{
	CString strText;

	////////////////////////////////////////////////////////////////////////////////////////
	// [CAUTION]: 아래와 다른 CME파일을 사용하는 경우에는 아래 코드를 수정하여 올바른 CME 파일의
	// 경로를 지정하십시오.
	#define CME_FILE_NAME	"Motion_Config.cme2" 

	char szCmeFilePath[MAX_PATH];//, szSystemDir[MAX_PATH];
//	GetSystemDirectory(szSystemDir, MAX_PATH); // 윈도우즈 시스템 디렉토리명 얻어오기 (일반적으로는 C:\Windows\System32)
	sprintf(szCmeFilePath, "%s\\%s", ModelFile()->strDirConfigPath, CME_FILE_NAME);

	if(cmmGnInitFromFile(szCmeFilePath) != cmERR_NONE){
		strText.Format("[ERROR] InitMotionDevices()-> CME-File Load Error! [%s]", szCmeFilePath);
		Log()->SaveLogFile(_MOTION_LOG, strText);
		cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
		return FALSE;
	}
	else{
		////////////////////////////////////////////////////////////////////////////////////////
		// CME 파일에서 정의한 축수와 실제 장착되어 있는 모션 축수를 비교확인 하고자할 때는 
		// 아래와 같이 하면 됩니다.
		long nNumAxes_installed, nNumAxes_defined;
		cmmAdvGetNumAvailAxes(&nNumAxes_installed); // 현재 실제로 PC에 장착되어 있는 모션축 수 (cmmGnDeviceLoad() 함수에서 반환하는 축수와 동일)
		cmmAdvGetNumDefinedAxes(&nNumAxes_defined); // CME 파일에서 정의한 축 수(CME 파일이 지정되지 않는 경우에는 현재 장착된 축 수와 일치합니다.)
		if(nNumAxes_installed != nNumAxes_defined){
			Log()->SaveLogFile(_MOTION_LOG, "[ERROR] 현재 장착되어 있는 모션 축 수가 정의된 축 수와 일치하지 않습니다!");
			MessageBox(NULL, "현재 장착되어 있는 모션 축 수가 정의된 축 수와 일치하지 않습니다", "Warning", MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}

	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isIO_ReadyCheck()
//-----------------------------------------------------------------------------------
{
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isMotion_ReadyCheck(long nAxis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	long dwTemp;
	//---- MIO Display ----------------------------------------------------//
	cmmStReadMioStatuses(nAxis, &dwTemp); // Read all MIO states
	if( (dwTemp>>cmIOST_RDY) & 0x1)	return TRUE; // ALM 
	else							{
		CString strText;
		strText.Format("[ERROR] isMotion_ReadyCheck [%d]", nAxis);
		Log()->SaveLogFile(_MOTION_LOG, strText);
		return FALSE;
	}
/*	if( (dwTemp>>cmIOST_ALM) & 0x1)) return TRUE; // ALM 
	SetLedState(LED_ELN, (dwTemp>>cmIOST_ELN) & 0x1); // +EL 
	SetLedState(LED_ELP, (dwTemp>>cmIOST_ELP) & 0x1); // -EL
	SetLedState(LED_ORG, (dwTemp>>cmIOST_ORG) & 0x1); // ORG
	SetLedState(LED_INP, (dwTemp>>cmIOST_INP) & 0x1); // INP
	SetLedState(LED_RDY, (dwTemp>>cmIOST_RDY) & 0x1); // RDY 
	SetLedState(LED_DRN, (dwTemp>>cmIOST_DRN) & 0x1); // -DR
	SetLedState(LED_DRP, (dwTemp>>cmIOST_DRP) & 0x1); // +DR
	SetLedState(LED_CLR, (dwTemp>>cmIOST_CLR) & 0x1); // CLR
	SetLedState(LED_LTC, (dwTemp>>cmIOST_LTC) & 0x1); // LTC 
	SetLedState(LED_SD, (dwTemp>>cmIOST_SD) & 0x1); // SD 
	SetLedState(LED_STA, (dwTemp>>cmIOST_STA) & 0x1); // STA
	SetLedState(LED_STP, (dwTemp>>cmIOST_STP) & 0x1); // STP
	SetLedState(LED_ERC, (dwTemp>>cmIOST_ERC) & 0x1); // ERC 
*/
}
//===================================================================================
BOOL CComizoa_Class::isMotion_AlarmCheck()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	long dwTemp;
	cmmStReadMioStatuses(_AXIS_1L_STAGE_X, &dwTemp); // Read all MIO states
	if( (dwTemp>>cmIOST_ALM) & 0x1)	{
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] isMotion_ReadyCheck [_AXIS_1L_STAGE_X]");
		return FALSE; // ALM 
	}
	else			{
		return TRUE;
	}
}
//===================================================================================
BOOL CComizoa_Class::isEMO_Check()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) return FALSE;
	if(!COMI_Initial) return FALSE;

	if(InputBit(_INPUT_EMO_SWITCH) )
		return FALSE;
	else					
		return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isPOWER_ON_Check()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) return TRUE;
	if(!COMI_Initial) return FALSE;

	if(InputBit(_INPUT_POWER_ON) )
		return TRUE;
	else 
		return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isAIR_ON_Check()
//-----------------------------------------------------------------------------------
{
	if(!COMI_Initial) return FALSE;

#ifdef _INPUT_AIR_ON
	if(InputBit(_INPUT_AIR_ON) )
		return TRUE;
	else 
		return FALSE;
#endif
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::isMotion_AlarmCheck(long nAxis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	long dwTemp;
	//---- MIO Display ----------------------------------------------------//
	cmmStReadMioStatuses(nAxis, &dwTemp); // Read all MIO states
	if( (dwTemp>>cmIOST_ALM) & 0x1)	return FALSE; // ALM 
	else			{
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] isMotion_AlarmCheck");
		return TRUE;
	}
/*	if( (dwTemp>>cmIOST_ALM) & 0x1)) return TRUE; // ALM 
	SetLedState(LED_ELN, (dwTemp>>cmIOST_ELN) & 0x1); // +EL 
	SetLedState(LED_ELP, (dwTemp>>cmIOST_ELP) & 0x1); // -EL
	SetLedState(LED_ORG, (dwTemp>>cmIOST_ORG) & 0x1); // ORG
	SetLedState(LED_INP, (dwTemp>>cmIOST_INP) & 0x1); // INP
	SetLedState(LED_RDY, (dwTemp>>cmIOST_RDY) & 0x1); // RDY 
	SetLedState(LED_DRN, (dwTemp>>cmIOST_DRN) & 0x1); // -DR
	SetLedState(LED_DRP, (dwTemp>>cmIOST_DRP) & 0x1); // +DR
	SetLedState(LED_CLR, (dwTemp>>cmIOST_CLR) & 0x1); // CLR
	SetLedState(LED_LTC, (dwTemp>>cmIOST_LTC) & 0x1); // LTC 
	SetLedState(LED_SD, (dwTemp>>cmIOST_SD) & 0x1); // SD 
	SetLedState(LED_STA, (dwTemp>>cmIOST_STA) & 0x1); // STA
	SetLedState(LED_STP, (dwTemp>>cmIOST_STP) & 0x1); // STP
	SetLedState(LED_ERC, (dwTemp>>cmIOST_ERC) & 0x1); // ERC 
*/
}
//===================================================================================
BOOL CComizoa_Class::isDone(long axis) //완료 시 TRUE리턴
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	BOOL flag= TRUE;
	long ret= 0;
	if(axis== _M_ALL) {
		for(int i= 0; i< _MAX_AXIS; i++) {
			cmmSxIsDone(i, &ret);
			if(ret!= cmTRUE)		flag= FALSE;
		}
	}
	else {
		cmmSxIsDone(axis, &ret);
		if(ret!= cmTRUE)		flag= FALSE;
	}
	return flag;
}

//===================================================================================
BOOL CComizoa_Class::isHomeBusy(long axis) //동작중이면 TRUE리턴
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return FALSE; }

	if(!COMI_Initial) return FALSE;
	BOOL flag= TRUE;
	long ret= 0;
	if(axis== _M_ALL) {
		for(int i= 0; i< _MAX_AXIS; i++) {
			cmmHomeIsBusy(i, &ret); //TRUE이면 진행 중
			if(ret!= cmTRUE)		flag= FALSE;
		}
	}
	else {
		cmmHomeIsBusy(axis, &ret);
		if(ret== cmTRUE)		flag= TRUE;
		else					return FALSE;
	}
	return flag;
}
//===================================================================================
void CComizoa_Class::ResetPos(long axis) 
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ;
	
	CString strText;
	strText.Format("[MOTION] ResetPos - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
	if(axis== _M_ALL) {
		for(int i= 0; i< _MAX_AXIS; i++) {
			cmmStSetCount(i, cmCNT_COMM, 0); // Reset COMMAND Position
			cmmStSetCount(i, cmCNT_FEED, 0); // Reset FEEDBACK Position
			cmmStSetPosition(i, cmCNT_COMM, 0.0); // Reset COMMAND Position
			cmmStSetPosition(i, cmCNT_FEED, 0.0); // Reset FEEDBACK Position
		}
	}
	else {
		cmmStSetCount(axis, cmCNT_COMM, 0); // Reset COMMAND Position
		cmmStSetCount(axis, cmCNT_FEED, 0); // Reset FEEDBACK Position
		cmmStSetPosition(axis, cmCNT_COMM, 0.0); // Reset COMMAND Position
		cmmStSetPosition(axis, cmCNT_FEED, 0.0); // Reset FEEDBACK Position
	}
}
//===================================================================================
void CComizoa_Class::HomeSearchErrorProc() 
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ;
	CString strText;
	strText.Format("[ERROR] HomeSearchErrorProc");	Log()->SaveLogFile(_MOTION_LOG, strText);
	MotorAllStop();
	for(int i= 0; i< 16; i++) {
		AxisHomeStat[i]= _M_NONE;
	}
}
//===================================================================================
BOOL CComizoa_Class::HomeEndProc(long axis) 
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	long dwIsSuccess;
	if(!COMI_Initial) return FALSE;
	cmmSxStop(axis, FALSE, FALSE);
	cmmHomeWaitDone(axis, cmFALSE); // 해당축이 Home Return을 완료했는지 체크합니다.
	cmmHomeGetSuccess(axis, &dwIsSuccess); // 원점복귀가 성공되었는지 체크 
	CString strText;
	if(axis== _AXIS_1L_STAGE_X) {
		long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_STAGE_X]);
		long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_STAGE_X]);
		cmmCfgSetSoftLimit (_AXIS_1L_STAGE_X, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_2L_STAGE_X) {
		long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_STAGE_X]);
		long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_STAGE_X]);
		cmmCfgSetSoftLimit (_AXIS_2L_STAGE_X, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_1L_ROLL_Z) {
		long lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_ROLL_Z]);
		long lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_ROLL_Z]);
		cmmCfgSetSoftLimit (_AXIS_1L_ROLL_Z1_SYNC, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
		cmmCfgSetSoftLimit (_AXIS_1L_ROLL_Z2_SYNC, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_2L_ROLL_Z) {
		long lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_ROLL_Z]);
		long lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_ROLL_Z]);
		cmmCfgSetSoftLimit (_AXIS_2L_ROLL_Z1_SYNC, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
		cmmCfgSetSoftLimit (_AXIS_2L_ROLL_Z2_SYNC, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(dwIsSuccess) { // 원점복귀 성공
		strText.Format("[HOME] 원점복귀 성공 - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
		return TRUE;
	}
	long dwErrCode;
	char szErrReason[CMM_MAX_STR_LEN_ERR];

	cmmErrGetLastCode(&dwErrCode);
	cmmErrGetString(dwErrCode, szErrReason, CMM_MAX_STR_LEN_ERR);

	strText.Format("[ERROR] 원점복귀 실패 - %d", axis);		Log()->SaveLogFile(_MOTION_LOG, strText);
	strText.Format("[ERROR] 에러 원인: %s", szErrReason);	Log()->SaveLogFile(_MOTION_LOG, strText);
//	cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
	return FALSE;
}

//===================================================================================
BOOL CComizoa_Class::OutputBit(long no, long status)
//-----------------------------------------------------------------------------------
{
#ifdef _PEMS_R2P_VER1
	no= no + _IO_OFFSET;
#endif
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	cmmDoPutOne(no, status);
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isHomeReturnEheck(long axis, bool flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	//////////////////////////////////////////////////////////////////////////
	// 이전에 원점복귀를 성공적으로 수행했는지 체크하고자할 때에는 cmmHomeGetSuccess()
	// 함수를 사용한다. cmmHomeGetSuccess()에서 반환하는 플래그는 프로그램이 
	// 종료되었다가 다시 실행되어도 계속 유지된다(단, PC가 재부팅되면 리셋된다)
	long dwIsSuccess;
	cmmHomeGetSuccess(axis, &dwIsSuccess); // 원점복귀가 성공되었는지 체크 
	CString strText;
	if(dwIsSuccess ){ 
		if(flag) {
			// 이전에 원점복귀가 이미 수행되었으면...(이것은 프로그램이 종료되었가 다시 실행되었을 때도 유효하다)
			strText.Format("[HOME] 이전에 원점복귀가 이미 수행되었슴! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
			MotorAllStop();
		}
		return TRUE;
	}
	if(flag) {
		strText.Format("[HOME] 원점복귀 수행해야함! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
	}
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isAllMotorHomeEnd()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(	isHomeReturnEheck(_AXIS_1L_STAGE_X, false) && 
		isHomeReturnEheck(_AXIS_1L_ROLL_Z1_SYNC, false) &&
		isHomeReturnEheck(_AXIS_1L_ROLL_Z2_SYNC, false) &&
		isHomeReturnEheck(_AXIS_2L_STAGE_X, false) && 
		isHomeReturnEheck(_AXIS_2L_ROLL_Z1_SYNC, false) &&
		isHomeReturnEheck(_AXIS_2L_ROLL_Z2_SYNC, false) ) return TRUE;
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::MotionHome(long axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	CString strText;
	if(!COMI_Initial) return FALSE;

	if(axis== _AXIS_1L_STAGE_X) {
		long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_STAGE_X]);
		long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_STAGE_X]);
		cmmCfgSetSoftLimit (_AXIS_1L_STAGE_X, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_2L_STAGE_X) {
		long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_STAGE_X]);
		long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_STAGE_X]);
		cmmCfgSetSoftLimit (_AXIS_2L_STAGE_X, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_1L_ROLL_Z) {
		long lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_ROLL_Z]);
		long lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_ROLL_Z]);
		cmmCfgSetSoftLimit (_AXIS_1L_ROLL_Z1_SYNC, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
		cmmCfgSetSoftLimit (_AXIS_1L_ROLL_Z2_SYNC, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_2L_ROLL_Z) {
		long lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_ROLL_Z]);
		long lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_ROLL_Z]);
		cmmCfgSetSoftLimit (_AXIS_2L_ROLL_Z1_SYNC, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
		cmmCfgSetSoftLimit (_AXIS_2L_ROLL_Z2_SYNC, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}

	long MioStatus= 0;

	// 원점복귀 수행 //
//	ResetPos(axis) ;
//	cmmCfgSetMioProperty (axis, cmORG_LOGIC, cmLOGIC_B); // ORG Input Logic
	cmmCfgSetMioProperty (axis, cmEZ_LOGIC, cmLOGIC_A); // EZ Input Logic
	cmmCfgSetMioProperty (axis, cmERC_OUT, cmLOGIC_A); // Output ERC signal at completion
	if(axis== _AXIS_1L_STAGE_X) {
		cmmSxOptSetIniSpeed(_AXIS_1L_STAGE_X, ModelFile()->Motion_data.dStart_Speed[_AXIS_1L_STAGE_X]);
	}
	if(axis== _AXIS_2L_STAGE_X) {
		cmmSxOptSetIniSpeed(_AXIS_2L_STAGE_X, ModelFile()->Motion_data.dStart_Speed[_AXIS_2L_STAGE_X]);
	}
	if(axis!= _AXIS_1L_ROLL_Z1_SYNC && axis!= _AXIS_1L_ROLL_Z2_SYNC && axis!= _AXIS_2L_ROLL_Z1_SYNC && axis!= _AXIS_2L_ROLL_Z2_SYNC) 
		cmmSxOptSetIniSpeed(axis, ModelFile()->Motion_data.dStart_Speed[axis]);

	cmmHomeSetSpeedPattern(axis, cmSMODE_S,ModelFile()->Motion_data.dOrigin_Speed[axis],
		ModelFile()->Motion_data.dOrigin_Accel[axis], 
		ModelFile()->Motion_data.dOrigin_Decel[axis], 
		ModelFile()->Motion_data.dOrigin_Speed[axis]/3.); 
	Sleep(100);
	if(cmmHomeMoveStart(axis, cmDIR_N) != cmERR_NONE){
		cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
		strText.Format("[ERROR] 원점복귀 수행 실패! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
		return FALSE;
	}
	strText.Format("[HOME] 원점복귀 수행! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::OutputToggle(long no)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;

	long nStatus = 0x0;
	if ( cmmDoGetOne(no, &nStatus) == cmERR_NONE)
	{
		nStatus = nStatus ? cmFALSE : cmTRUE;
		cmmDoPutOne(no, nStatus);
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isOutputBit(long no)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	long nStatus = 0x0;
	if ( cmmDoGetOne(no, &nStatus) == cmERR_NONE)
	{
		if(nStatus== cmTRUE) return TRUE;
		return FALSE;
	}
	return FALSE;
}

//===================================================================================
long CComizoa_Class::InputBit(long no)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return 0; }
	long nStatus = 0x0;
	if(!COMI_Initial) return FALSE;
	if ( cmmDiGetOne(no, &nStatus) == cmERR_NONE)
	{
		return nStatus;
	}
//			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
	return cmFALSE;
}
//===================================================================================
void CComizoa_Class::ServoOn(long no, long status)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }
	if(!COMI_Initial) return ; 
	cmmGnSetServoOn(no, status);
}
//===================================================================================
long CComizoa_Class::isServoOn(long no)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return 1; }
	long nStatus = 0x0;
	if(!COMI_Initial) return FALSE; 
	cmmGnGetServoOn(no, &nStatus);
	return nStatus;
}
//===================================================================================
void CComizoa_Class::ServoAllOn(long status)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }
	if(!COMI_Initial) return ; 
	cmmGnSetServoOn(_AXIS_1L_STAGE_X,		status);
	cmmGnSetServoOn(_AXIS_1L_ROLL_Z1_SYNC,	status);
	cmmGnSetServoOn(_AXIS_1L_ROLL_Z2_SYNC,	status);
	cmmGnSetServoOn(_AXIS_2L_STAGE_X,		status);
	cmmGnSetServoOn(_AXIS_2L_ROLL_Z1_SYNC,	status);
	cmmGnSetServoOn(_AXIS_2L_ROLL_Z2_SYNC,	status);
}
//===================================================================================
void CComizoa_Class::MotorAllStop()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ; 
	long nAxes_1[4]={0, 1, 2, 3};
	cmmMxStop(4, nAxes_1, FALSE, FALSE);

	CString strLog; strLog.Format("[MOTOR] MotorAllStop");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z]);
	Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z]);
}
//===================================================================================
void CComizoa_Class::MotorAll_EMG_Stop()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ; 

	long nAxes_1[4]={0, 1, 2, 3};

	MotorZStop(_AXIS_1L_ROLL_Z);
	MotorZStop(_AXIS_2L_ROLL_Z);
	cmmMxStopEmg(4, nAxes_1);

	Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z]);
	Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z]);
}
//===================================================================================
void CComizoa_Class::MotorZStop(int z_axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ;
	if(z_axis== _AXIS_1L_ROLL_Z)
	{
		long nAxes_1[2]={_AXIS_1L_ROLL_Z1_SYNC, _AXIS_1L_ROLL_Z2_SYNC};
		cmmMxStop(2, nAxes_1, TRUE, FALSE);
		Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z]);
		CString strLog; strLog.Format("[MOTOR] L1_MotorZStop-_AXIS_1L_ROLL_Z");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	}
	else
	{
		long nAxes_2[2]={_AXIS_2L_ROLL_Z1_SYNC, _AXIS_2L_ROLL_Z2_SYNC};
		cmmMxStop(2, nAxes_2, TRUE, FALSE);
		Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z]);
		CString strLog; strLog.Format("[MOTOR] L2_MotorZStop-_AXIS_2L_ROLL_Z");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	}
}
//===================================================================================
void CComizoa_Class::MotorZ1Stop(int z_axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }
	if(!COMI_Initial) return ; 

	if(z_axis== _AXIS_1L_ROLL_Z)
	{
		cmmSxStop(_AXIS_1L_ROLL_Z1_SYNC,  TRUE, FALSE);
		CString strLog; strLog.Format("[MOTOR] L1_MotorZ1Stop");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
		if(Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z1_SYNC])== FALSE) return ;
	}
	else
	{
		cmmSxStop(_AXIS_2L_ROLL_Z1_SYNC,  TRUE, FALSE);
		CString strLog; strLog.Format("[MOTOR] L2_MotorZ1Stop");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
		if(Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z1_SYNC])== FALSE) return ;
	}
}
//===================================================================================
void CComizoa_Class::MotorZ2Stop(int z_axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }
	if(!COMI_Initial) return ; 
	
	if(z_axis== _AXIS_1L_ROLL_Z)
	{
		cmmSxStop(_AXIS_1L_ROLL_Z2_SYNC,  TRUE, FALSE);
		CString strLog; strLog.Format("[MOTOR] L1_MotorZ2Stop");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
		if(Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC])== FALSE) return ;
	}
	else
	{
		cmmSxStop(_AXIS_2L_ROLL_Z2_SYNC,  TRUE, FALSE);
		CString strLog; strLog.Format("[MOTOR] L2_MotorZ2Stop");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
		if(Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC])== FALSE) return ;
	}
}
//===================================================================================
void CComizoa_Class::MotorZ_EMG_Stop()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ; 

	long nAxes_1[2]={_AXIS_1L_ROLL_Z1_SYNC, _AXIS_1L_ROLL_Z2_SYNC};
	cmmMxStopEmg(2, nAxes_1);
	long nAxes_2[2]={_AXIS_2L_ROLL_Z1_SYNC, _AXIS_2L_ROLL_Z2_SYNC};
	cmmMxStopEmg(2, nAxes_2);

	Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z]);
	Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z]);
}
//===================================================================================
BOOL CComizoa_Class::JogStart(long axis, long dir, int spd)
//-----------------------------------------------------------------------------------
{
//	int Z1Dir;
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 
	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) {
		return FALSE;
	}
	if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) {
		return FALSE;
	}

	double SetSpeed= 0.0;

	//	cmmSxOptSetIniSpeed(axis, ModelFile()->Motion_data.dStart_Speed[axis]);
	cmmSxOptSetIniSpeed(axis, 0);
	switch(spd) {
		case _MAX_SPEED:
			SetSpeed= ModelFile()->Motion_data.dJog_MaxSpeed[axis];
			break;
		case _MID_SPEED: 
			SetSpeed= ModelFile()->Motion_data.dJog_MidSpeed[axis];
			break;
		case _MIN_SPEED: 
			SetSpeed= ModelFile()->Motion_data.dJog_MinSpeed[axis];
			break;
		default:
			return FALSE;
	}

	cmmCfgSetSpeedPattern(axis, cmSMODE_C, SetSpeed,
		ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
	cmmSxSetSpeedRatio(axis, cmSMODE_KEEP, 100.0, 100.0, 100.0);

	if(cmmSxVMoveStart(axis, dir) != cmERR_NONE){ 
		CString strTTT; strTTT.Format("%d : JogStart() cmmSxVMoveStart", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
		return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::JogStart_Z(long axis, long dir, int spd, int SyncMode)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	int Z1Dir;
	if(!COMI_Initial) return FALSE; 
	CString strLog; strLog.Format("[MOTOR] JogStart_Z(%d, %d, ,%d)", axis, dir, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) 
	{
		if(!Z_Axis_Check(_AXIS_1L_ROLL_Z, &Z1Dir)) 
		{
			if(Z1Dir< 0) {
				if(axis== _AXIS_1L_ROLL_Z1_SYNC && dir== _DIR_CCW)		return FALSE;
				else if(axis== _AXIS_1L_ROLL_Z2_SYNC && dir== _DIR_CW)	return FALSE;
			}
			else {
				if(axis== _AXIS_1L_ROLL_Z1_SYNC && dir== _DIR_CW)		return FALSE;
				else if(axis== _AXIS_1L_ROLL_Z2_SYNC && dir== _DIR_CCW)	return FALSE;
			}
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) 
	{
		if(!Z_Axis_Check(_AXIS_2L_ROLL_Z, &Z1Dir)) {
			if(Z1Dir< 0) {
				if(axis== _AXIS_2L_ROLL_Z1_SYNC && dir== _DIR_CCW)		return FALSE;
				else if(axis== _AXIS_2L_ROLL_Z2_SYNC && dir== _DIR_CW)	return FALSE;
			}
			else {
				if(axis== _AXIS_2L_ROLL_Z1_SYNC && dir== _DIR_CW)		return FALSE;
				else if(axis== _AXIS_2L_ROLL_Z2_SYNC && dir== _DIR_CCW)	return FALSE;
			}
		}
	}
	else return FALSE;

	double SetSpeed= 0.0;

	//	cmmSxOptSetIniSpeed(axis, ModelFile()->Motion_data.dStart_Speed[axis]);
	cmmSxOptSetIniSpeed(axis, 0);
	switch(spd) {
		case _MAX_SPEED:
			SetSpeed= ModelFile()->Motion_data.dJog_MaxSpeed[axis];
			break;
		case _MID_SPEED: 
			SetSpeed= ModelFile()->Motion_data.dJog_MidSpeed[axis];
			break;
		case _MIN_SPEED: 
			SetSpeed= ModelFile()->Motion_data.dJog_MinSpeed[axis];
			break;
		default:
			return FALSE;
	}
	if(SyncMode== _SYNC_MOVE) {
		if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) 
		{
			if(Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, SetSpeed)== FALSE) return FALSE;
		}
		else
		{
			if(Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, SetSpeed)== FALSE) return FALSE;
		}
	}
	else {
		if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) 
		{
			if(Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, SetSpeed)== FALSE) return FALSE;
		}
		else
		{
			if(Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, SetSpeed)== FALSE) return FALSE;
		}
	}

	cmmCfgSetSpeedPattern(axis, cmSMODE_C, SetSpeed,
		ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
	cmmSxSetSpeedRatio(axis, cmSMODE_KEEP, 100.0, 100.0, 100.0);
//	cmmSxSetSpeedRatio(axis, cmSMODE_C/*Keep prev. setting*/, 100, 100, 100); // Speed ratio 설정: 가감속은 편의상 100%로 고정함
	if(cmmSxVMoveStart(axis, dir) != cmERR_NONE){ 
			CString strTTT; strTTT.Format("%d : JogStart_Z() cmmSxVMoveStart", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
		return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::JogStop(long axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) 
	{
		cmmSxStop(axis, TRUE, FALSE);
		if(Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis])== FALSE) return FALSE;
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) 
	{
		cmmSxStop(axis, TRUE, FALSE);
		if(Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis])== FALSE) return FALSE;
	}
	else 	MoveStop(axis);

	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::MoveStop(long axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	cmmSxStop(axis, FALSE, FALSE);
	CString strLog; strLog.Format("[MOTOR] MoveStop= %d", axis);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) 
	{
		if(Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis])== FALSE) return FALSE;
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) 
	{
		if(Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis])== FALSE) return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::MoveEmgStop(long axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	cmmSxStopEmg(axis);
	Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z1_SYNC]);
	Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z1_SYNC]);
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Z_Axis_SYNC(int z_axis, long mode, double speed)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }
//#define _AXIS_ROLL_Z1_SYNC	6
//#define _AXIS_ROLL_Z2_SYNC	7
	if(!COMI_Initial) return FALSE; 
	CString strLog; strLog.Format("[MOTOR] Z_Axis_SYNC(%d, %f)", mode, speed);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	if(mode== _SYNC_MOVE) 
	{
		if(z_axis== _AXIS_1L_ROLL_Z1_SYNC || z_axis== _AXIS_1L_ROLL_Z2_SYNC)
		{
			cmmMsUnregisterSlave(_AXIS_1L_ROLL_Z2_SYNC); Sleep(100);
			if(cmmMsRegisterSlave(_AXIS_1L_ROLL_Z2_SYNC, speed, cmFALSE) != cmERR_NONE)
			{ 
				;
			}
		}
		else
		{
			cmmMsUnregisterSlave(_AXIS_2L_ROLL_Z2_SYNC); Sleep(100);
			if(cmmMsRegisterSlave(_AXIS_2L_ROLL_Z2_SYNC, speed, cmFALSE) != cmERR_NONE)
			{ 
				;
			}
		}
	}
	else 
	{
		if(z_axis== _AXIS_1L_ROLL_Z1_SYNC || z_axis== _AXIS_1L_ROLL_Z2_SYNC)
		{
			if(cmmMsUnregisterSlave(_AXIS_1L_ROLL_Z2_SYNC) != cmERR_NONE){ 
				;
			}
		}
		else
		{
			if(cmmMsUnregisterSlave(_AXIS_2L_ROLL_Z2_SYNC) != cmERR_NONE){ 
				;
			}
		}
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::INC_Move(long axis, double dist, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { Sleep(10); return TRUE; }

	///////////////////////////////////////////////////////////////////////////////////
	// [CAUTION] cmmSxMove() 함수의 IsBlocking 파라미터에 대하여:
	// 본예제는 편의상 이송중에도 타이머 이벤트나 Stop 버튼 이벤트 처리를 할 수 있도록 
	// IsBlocking 파라미터를 FALSE로 하였다. 하지만 쓰레드를 사용할 때는 IsBlocking 
	// 파라미터를 반드시 TRUE로 해야한다. 
	// cmmSxMoveTo  : 절대치 이동 & EndDone, cmmSxMoveToStart	: 절대치 이동 & return
	// cmmSxMove	: 상대치 이동 & EndDone, cmmSxMoveStart		: 상대치 이동 & return
	if(!COMI_Initial) return FALSE; 

	if(isServoOn(axis)== FALSE) {
		Log()->SaveLogFile(_MOTION_LOG, "서보 ON이 안되어 있습니다.");
		AfxMessageBox("서보 ON이 안되어 있습니다."); return FALSE;
	}

	if(axis== _AXIS_1L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_1L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!   이동 가능 거리 초과 입력!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_2L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_2L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!   이동 가능 거리 초과 입력!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!  이동 가능 거리 초과 입력!");
			AfxMessageBox("[_AXIS_1L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); return FALSE;
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!  이동 가능 거리 초과 입력!");
			AfxMessageBox("[_AXIS_2L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); return FALSE;
		}
	}

	if(axis== _AXIS_1L_STAGE_X) {	
		double CurPos= pSystem.GetMotionComponent()->Get_Position(_AXIS_1L_STAGE_X);
		if(CurPos >= _X_AXIS_MAX_ || CurPos<= _X_AXIS_MIN_)
		{
				Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_STAGE_X (ABS_Move)] 이동 거리 초과!");
				return TRUE;
		}
	}
	else if(axis== _AXIS_2L_STAGE_X) {	
		double CurPos= pSystem.GetMotionComponent()->Get_Position(_AXIS_2L_STAGE_X);
		if(CurPos >= _X_AXIS_MAX_ || CurPos<= _X_AXIS_MIN_)
		{
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_STAGE_X (ABS_Move)] 이동 거리 초과!");
			return TRUE;
		}
	}
	
	CString strLog; strLog.Format("[MOTOR] INC_Move(%d, %f, %d, %d)", axis, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	double dOutPulse= 0.0;
	long lOutPulse= 0;
	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis]);
	dOutPulse= (double)(lOutPulse);

	cmmCfgSetSpeedPattern(axis, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[axis],
					ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
	cmmSxSetSpeedRatio(axis, cmSMODE_S, 100.0, 100.0, 100.0);

	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) { 
		if(SyncMode== _NON_SYNC_MOVE) {
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			if(cmmSxMove(axis, dOutPulse, TRUE) != cmERR_NONE){  
				CString strTTT; strTTT.Format("%d : INC_Move() cmmSxMove-1", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
				cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
				Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
				return FALSE;
			}
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			return TRUE;
		}
		else {
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) { 
		if(SyncMode== _NON_SYNC_MOVE) {
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			if(cmmSxMove(axis, dOutPulse, TRUE) != cmERR_NONE){  
				CString strTTT; strTTT.Format("%d : INC_Move() cmmSxMove-1", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
				cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
				Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
				return FALSE;
			}
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			return TRUE;
		}
		else {
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
		}
	}


	if(EndDoneCheck) {
		if(cmmSxMove(axis, dOutPulse, TRUE) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d : INC_Move() cmmSxMove-2", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
			return FALSE;
		}
	}
	else {
		if(cmmSxMoveStart(axis, dOutPulse) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d : INC_Move() cmmSxMoveStart-3", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
			return FALSE;
		}
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::ABS_Move(long axis, double dist, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { Sleep(10); return TRUE; }

	///////////////////////////////////////////////////////////////////////////////////
	// [CAUTION] cmmSxMove() 함수의 IsBlocking 파라미터에 대하여:
	// 본예제는 편의상 이송중에도 타이머 이벤트나 Stop 버튼 이벤트 처리를 할 수 있도록 
	// IsBlocking 파라미터를 FALSE로 하였다. 하지만 쓰레드를 사용할 때는 IsBlocking 
	// 파라미터를 반드시 TRUE로 해야한다. 
	// cmmSxMoveTo  : 절대치 이동 & EndDone, cmmSxMoveToStart	: 절대치 이동 & return
	// cmmSxMove	: 상대치 이동 & EndDone, cmmSxMoveStart		: 상대치 이동 & return
	if(!COMI_Initial) return FALSE; 
	if(isServoOn(axis)== FALSE) {
		Log()->SaveLogFile(_MOTION_LOG, "서보 ON이 안되어 있습니다.");
		AfxMessageBox("서보 ON이 안되어 있습니다."); return FALSE;
		return FALSE;	}

	if(!isMotion_AlarmCheck()) return FALSE; 

	if(axis== _AXIS_1L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_1L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!   이동 가능 거리 초과 입력!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_2L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_2L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_STAGE_X (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!   이동 가능 거리 초과 입력!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!  이동 가능 거리 초과 입력!");
			AfxMessageBox("[_AXIS_1L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); return FALSE;
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!  이동 가능 거리 초과 입력!");
			AfxMessageBox("[_AXIS_2L_ROLL_Z (ABS_Move)] 이동 거리 입력이 부적절 합니다!!!\n이동 가능 거리 초과 입력!"); return FALSE;
		}
	}

	if(!isMotion_AlarmCheck()) return FALSE; 
	CString strLog; strLog.Format("[MOTOR] ABS_Move(%d, %f, %d, %d)", axis, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	double dOutPulse= 0.0;
	long lOutPulse= 0;
	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis]);
	dOutPulse= (double)(lOutPulse);

		cmmCfgSetSpeedPattern(axis, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[axis],
						ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);

		cmmSxSetSpeedRatio(axis, cmSMODE_S, 100.0, 100.0, 100.0);

	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) { 
		if(SyncMode== _NON_SYNC_MOVE) {
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			if(cmmSxMoveTo(axis, dOutPulse, TRUE) != cmERR_NONE){  
				CString strTTT; strTTT.Format("%d : ABS_Move() cmmSxMoveTo-1", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
				cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
				Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
				return FALSE;
			}
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			return TRUE;
		}
		else  {
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) { 
		if(SyncMode== _NON_SYNC_MOVE) {
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			if(cmmSxMoveTo(axis, dOutPulse, TRUE) != cmERR_NONE){  
				CString strTTT; strTTT.Format("%d : ABS_Move() cmmSxMoveTo-1", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
				cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
				Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
				return FALSE;
			}
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
			return TRUE;
		}
		else  {
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[axis]);
		}
	}

	if(EndDoneCheck) {
		if(cmmSxMoveTo(axis, dOutPulse, TRUE) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d : ABS_Move() cmmSxMoveTo-2", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
			return FALSE;
		}
	}
	else {
		if(cmmSxMoveToStart(axis, dOutPulse) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d : ABS_Move() cmmSxMoveToStart-3", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
			return FALSE;
		}
	}
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::ABS_Z_Offset_Move(int z_axis, double dist)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { Sleep(10); return TRUE; }

	///////////////////////////////////////////////////////////////////////////////////
	// [CAUTION] cmmSxMove() 함수의 IsBlocking 파라미터에 대하여:
	// 본예제는 편의상 이송중에도 타이머 이벤트나 Stop 버튼 이벤트 처리를 할 수 있도록 
	// IsBlocking 파라미터를 FALSE로 하였다. 하지만 쓰레드를 사용할 때는 IsBlocking 
	// 파라미터를 반드시 TRUE로 해야한다. 
	// cmmSxMoveTo  : 절대치 이동 & EndDone, cmmSxMoveToStart	: 절대치 이동 & return
	// cmmSxMove	: 상대치 이동 & EndDone, cmmSxMoveStart		: 상대치 이동 & return

	if(!COMI_Initial) return FALSE; 
	if(!isMotion_AlarmCheck()) return FALSE; 
	
	double Z1_pos= 0.0;
	if(z_axis== _AXIS_1L_ROLL_Z1_SYNC || z_axis== _AXIS_1L_ROLL_Z2_SYNC)
	{
		Z1_pos= Get_Position(_AXIS_1L_ROLL_Z1_SYNC);
	}
	else if(z_axis== _AXIS_2L_ROLL_Z1_SYNC || z_axis== _AXIS_2L_ROLL_Z2_SYNC)
	{
		Z1_pos= Get_Position(_AXIS_2L_ROLL_Z1_SYNC);
	}

	if(!Z_Axis_Check(z_axis)) return FALSE;

	if(dist >= ModelFile()->Motion_data.dZ_Axis_Interlock) {
		Log()->SaveLogFile(_MOTION_LOG, "[ABS_Z_Offset_Move] 이동 거리 입력이 부적절 합니다!!! 인터록 설정치를 초과!!!");
		AfxMessageBox("[ABS_Z_Offset_Move] 이동 거리 입력이 부적절 합니다!!!\n인터록 설정치를 초과!!!"); return FALSE;
	}

	CString strLog; strLog.Format("[MOTOR] ABS_Z_Offset_Move(_AXIS_Z2, %f [Z1=%f:Offset=%f])", 
		dist+Z1_pos, Z1_pos, dist);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	double dOutPulse= 0.0;
	long lOutPulse= 0;

	if(z_axis== _AXIS_1L_ROLL_Z1_SYNC || z_axis== _AXIS_1L_ROLL_Z2_SYNC)
	{
		lOutPulse= (long)((dist+Z1_pos) * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_ROLL_Z2_SYNC]);
		dOutPulse= (double)(lOutPulse);
	}
	else if(z_axis== _AXIS_2L_ROLL_Z1_SYNC || z_axis== _AXIS_2L_ROLL_Z2_SYNC)
	{
		lOutPulse= (long)((dist+Z1_pos) * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_ROLL_Z2_SYNC]);
		dOutPulse= (double)(lOutPulse);
	}

	if(z_axis== _AXIS_1L_ROLL_Z)
	{
		cmmCfgSetSpeedPattern(_AXIS_1L_ROLL_Z2_SYNC, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC],
						ModelFile()->Motion_data.dWork_Accel[_AXIS_1L_ROLL_Z2_SYNC], ModelFile()->Motion_data.dWork_Decel[_AXIS_1L_ROLL_Z2_SYNC]);
		cmmSxSetSpeedRatio(_AXIS_1L_ROLL_Z2_SYNC, cmSMODE_S, 100.0, 100.0, 100.0);
		Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC]);

		if(cmmSxMoveTo(_AXIS_1L_ROLL_Z2_SYNC, dOutPulse, TRUE) != cmERR_NONE){  
			CString strTTT; strTTT.Format("_AXIS_1L_ROLL_Z2_SYNC : ABS_Z_Offset_Move() cmmSxMoveTo-1");	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC]);
			return FALSE;
		}
		Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC]);
	}
	else
	{
		cmmCfgSetSpeedPattern(_AXIS_2L_ROLL_Z2_SYNC, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC],
			ModelFile()->Motion_data.dWork_Accel[_AXIS_2L_ROLL_Z2_SYNC], ModelFile()->Motion_data.dWork_Decel[_AXIS_2L_ROLL_Z2_SYNC]);
		cmmSxSetSpeedRatio(_AXIS_2L_ROLL_Z2_SYNC, cmSMODE_S, 100.0, 100.0, 100.0);
		Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC]);

		if(cmmSxMoveTo(_AXIS_2L_ROLL_Z2_SYNC, dOutPulse, TRUE) != cmERR_NONE){  
			CString strTTT; strTTT.Format("_AXIS_2L_ROLL_Z2_SYNC : ABS_Z_Offset_Move() cmmSxMoveTo-1");	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // 에러 발생하였으면 에러 디스플레이.
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC]);
			return FALSE;
		}
		Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC]);
	}

	return TRUE;
}

//===================================================================================
double CComizoa_Class::Get_Position(long axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return 0.0; }

	if(!COMI_Initial) return 0.0; 

	double fCurPos, Posmm= 0.0;
	// Command position //

	cmmStGetPosition(axis, cmCNT_COMM, &fCurPos);
	Posmm= fCurPos / ModelFile()->Motion_data.dPulsePerMM[axis];

	return Posmm;
}
//===================================================================================
BOOL CComizoa_Class::Z_Axis_Check(int z_axis, int *Z1Dir)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	*Z1Dir= 0; 

	double Z1_Pos= 0.0;
	double Z2_Pos= 0.0;

	if(z_axis== _AXIS_1L_ROLL_Z1_SYNC || z_axis== _AXIS_1L_ROLL_Z2_SYNC)
	{
		Z1_Pos= Get_Position(_AXIS_1L_ROLL_Z1_SYNC);
		Z2_Pos= Get_Position(_AXIS_1L_ROLL_Z2_SYNC);
	}
	else if(z_axis== _AXIS_2L_ROLL_Z1_SYNC || z_axis== _AXIS_2L_ROLL_Z2_SYNC)
	{
		Z1_Pos= Get_Position(_AXIS_2L_ROLL_Z1_SYNC);
		Z2_Pos= Get_Position(_AXIS_2L_ROLL_Z2_SYNC);
	}

	double Z_Sum= fabs(Z1_Pos - Z2_Pos);

	CString strLog; strLog.Format("[MOTOR] Z_Axis_Check 1[%f] - 2[%f] = [%f]", Z1_Pos, Z2_Pos, Z_Sum);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	
	if(Z_Sum>= ModelFile()->Motion_data.dZ_Axis_Interlock) {
		double aaa= Z1_Pos - Z2_Pos;
		if(aaa< 0) { 
			*Z1Dir= -1; 
		}
		else 
			*Z1Dir= 1; 
		return FALSE;
	}
	return TRUE;
}
BOOL CComizoa_Class::Z_Axis_Check(int z_axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	double Z1_Pos= 0.0;
	double Z2_Pos= 0.0;

	if(z_axis== _AXIS_1L_ROLL_Z1_SYNC || z_axis== _AXIS_1L_ROLL_Z2_SYNC)
	{
		Z1_Pos= Get_Position(_AXIS_1L_ROLL_Z1_SYNC);
		Z2_Pos= Get_Position(_AXIS_1L_ROLL_Z2_SYNC);
	}
	else if(z_axis== _AXIS_2L_ROLL_Z1_SYNC || z_axis== _AXIS_2L_ROLL_Z2_SYNC)
	{
		Z1_Pos= Get_Position(_AXIS_2L_ROLL_Z1_SYNC);
		Z2_Pos= Get_Position(_AXIS_2L_ROLL_Z2_SYNC);
	}

	double Z_Sum= fabs(Z1_Pos - Z2_Pos);
//	CString strLog; strLog.Format("[MOTOR] Z_Axis_Check 1[%f] - 2[%f] = [%f]", Z1_Pos, Z2_Pos, Z_Sum);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	if(Z_Sum>= ModelFile()->Motion_data.dZ_Axis_Interlock) {
		return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Vac_On()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 
#ifdef _OUTPUT_VAC
	return OutputBit(_OUTPUT_VAC, _ON);
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Vac_Off()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

#ifdef _OUTPUT_VAC
	return OutputBit(_OUTPUT_VAC, _OFF);
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Blow_On()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }
	
	if(!COMI_Initial) return FALSE; 
#ifdef _OUTPUT_BLOW
	return OutputBit(_OUTPUT_BLOW, _ON);
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Blow_Off()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }
	
	if(!COMI_Initial) return FALSE; 
	
#ifdef _OUTPUT_BLOW
	return OutputBit(_OUTPUT_BLOW, _OFF);
#endif
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::isMotionReadyCheck()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	int axis_no= 0;
	BOOL flag= TRUE;

	if(!isMotion_AlarmCheck()) return FALSE; 

	if(AxisHomeStat[_AXIS_1L_STAGE_X]!= _M_END) {
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "원점 작업을 먼저 수행하십시요!");
		MyMessageBox("원점 작업을 먼저 수행하십시요!. [_AXIS_1L_STAGE_X]", "Origin Failed", M_ICONERROR);
	}
	else if(AxisHomeStat[_AXIS_2L_STAGE_X]!= _M_END) {
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "원점 작업을 먼저 수행하십시요!");
		MyMessageBox("원점 작업을 먼저 수행하십시요!. [_AXIS_2L_STAGE_X]", "Origin Failed", M_ICONERROR);
	}
	else if(AxisHomeStat[_AXIS_1L_ROLL_Z]!= _M_END)	{
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "원점 작업을 먼저 수행하십시요!");
		MyMessageBox("원점 작업을 먼저 수행하십시요!. [_AXIS_1L_ROLL_Z]", "Origin Failed", M_ICONERROR);	
	}
	else if(AxisHomeStat[_AXIS_2L_ROLL_Z]!= _M_END)	{
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "원점 작업을 먼저 수행하십시요!");
		MyMessageBox("원점 작업을 먼저 수행하십시요!. [_AXIS_2L_ROLL_Z]", "Origin Failed", M_ICONERROR);	
	}

	ReadyCheckFlagMsg= false;
	return flag;
}

//===================================================================================
BOOL CComizoa_Class::Z_Offset_Pos_Move(long axis, double dist, BOOL EndDoneCheck)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { Sleep(10); return TRUE; }
	
	if(!COMI_Initial) return FALSE; 
	if(!isMotion_AlarmCheck()) return FALSE; 

	
	double Z1_pos= 0.0;
	double Z2_pos= 0.0;

	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC)
	{
		if(!Z_Axis_Check(_AXIS_1L_ROLL_Z)) return FALSE;
		Z1_pos= Get_Position(_AXIS_1L_ROLL_Z1_SYNC);
		Z2_pos= Get_Position(_AXIS_1L_ROLL_Z2_SYNC);
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC)
	{
		if(!Z_Axis_Check(_AXIS_2L_ROLL_Z)) return FALSE;
		Z1_pos= Get_Position(_AXIS_2L_ROLL_Z1_SYNC);
		Z2_pos= Get_Position(_AXIS_2L_ROLL_Z2_SYNC);
	}
	
	if(dist >= ModelFile()->Motion_data.dZ_Axis_Interlock) {
		Log()->SaveLogFile(_MOTION_LOG, "[Z_Offset_Pos_Move] 이동 거리 입력이 부적절 합니다!!! 인터록 설정치를 초과!!!");
		AfxMessageBox("[Z_Offset_Pos_Move] 이동 거리 입력이 부적절 합니다!!!\n인터록 설정치를 초과!!!"); return FALSE;
	}
	CString strLog; strLog.Format("[MOTOR] Z_Offset_Pos_Move(Z1= %f Z2= %f [Offset=%f])", 
		Z1_pos, Z2_pos, dist);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	
	if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC)
	{
		cmmCfgSetSpeedPattern(_AXIS_1L_ROLL_Z2_SYNC, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC],
			ModelFile()->Motion_data.dWork_Accel[_AXIS_1L_ROLL_Z2_SYNC], ModelFile()->Motion_data.dWork_Decel[_AXIS_1L_ROLL_Z2_SYNC]);
		cmmSxSetSpeedRatio(_AXIS_1L_ROLL_Z2_SYNC, cmSMODE_S, 100.0, 100.0, 100.0);
		
		double dOutPulse= 0.0;
		long lOutPulse= 0;
		lOutPulse= (long)((dist + Z1_pos) * ModelFile()->Motion_data.dPulsePerMM[_AXIS_1L_ROLL_Z2_SYNC]);
		dOutPulse= (double)(lOutPulse);
		
		Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC]);
		if(cmmSxMoveTo(_AXIS_1L_ROLL_Z2_SYNC, dOutPulse, TRUE) != cmERR_NONE){  
			Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC]);
			return FALSE;
		}
		Z_Axis_SYNC(_AXIS_1L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_1L_ROLL_Z2_SYNC]);
	}
	else
	{
		cmmCfgSetSpeedPattern(_AXIS_2L_ROLL_Z2_SYNC, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC],
			ModelFile()->Motion_data.dWork_Accel[_AXIS_2L_ROLL_Z2_SYNC], ModelFile()->Motion_data.dWork_Decel[_AXIS_2L_ROLL_Z2_SYNC]);
		cmmSxSetSpeedRatio(_AXIS_2L_ROLL_Z2_SYNC, cmSMODE_S, 100.0, 100.0, 100.0);
		
		double dOutPulse= 0.0;
		long lOutPulse= 0;
		lOutPulse= (long)((dist + Z1_pos) * ModelFile()->Motion_data.dPulsePerMM[_AXIS_2L_ROLL_Z2_SYNC]);
		dOutPulse= (double)(lOutPulse);
		
		Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _NON_SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC]);
		if(cmmSxMoveTo(_AXIS_2L_ROLL_Z2_SYNC, dOutPulse, TRUE) != cmERR_NONE){  
			Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC]);
			return FALSE;
		}
		Z_Axis_SYNC(_AXIS_2L_ROLL_Z, _SYNC_MOVE, ModelFile()->Motion_data.dWork_Speed[_AXIS_2L_ROLL_Z2_SYNC]);
	}
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::Z_ReadyPosCheck()
//-----------------------------------------------------------------------------------
{
	double Zpos= 0.0;

	Zpos= Get_Position(_AXIS_1L_ROLL_Z1_SYNC);
	if((ModelFile()->Model_data.dPosition_Wait[_AXIS_1L_ROLL_Z]+ 1.0) < (Zpos))
		return FALSE;
	Zpos= Get_Position(_AXIS_2L_ROLL_Z1_SYNC);
	if((ModelFile()->Model_data.dPosition_Wait[_AXIS_2L_ROLL_Z]+ 1.0) < (Zpos))
		return FALSE;

	return TRUE;
}
//===================================================================================
void CComizoa_Class::EMG_Proc()
//-----------------------------------------------------------------------------------
{
	int i;
	
	Power_OnOff(_OFF);

	for(i= 0; i< 16; i++) {
		AxisHomeStat[i]= _M_NONE;
	}
}
//===================================================================================
void CComizoa_Class::Power_OnOff(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ; 

	if(flag== _OFF)	
	{
//		OutputBit(_OUTPUT_POWER_ON, _ON);
		OutputBit(_OUTPUT_L1_3PH_POWER, _OFF);
		OutputBit(_OUTPUT_L2_3PH_POWER, _OFF);
		OutputBit(_OUTPUT_ETC_3PH_POWER, _OFF);
	}
	else
	{
//		OutputBit(_OUTPUT_POWER_ON, _OFF);
		OutputBit(_OUTPUT_L1_3PH_POWER, _ON);
		OutputBit(_OUTPUT_L2_3PH_POWER, _ON);
		OutputBit(_OUTPUT_ETC_3PH_POWER, _ON);
	}
}
//===================================================================================
BOOL CComizoa_Class::isPower_On()
//-----------------------------------------------------------------------------------
{
	if(!COMI_Initial) return FALSE;
	
	if(InputBit(_INPUT_POWER_ON) )
		return TRUE;
	else 
		return FALSE;
}
//===================================================================================
void CComizoa_Class::L1_Motor_Power_OnOff(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }
	
	if(!COMI_Initial) return ; 
	
	if(flag== _ON)	
	{
		OutputBit(_OUTPUT_L1_3PH_POWER, _ON);
		OutputBit(_OUTPUT_ETC_3PH_POWER, _ON);
	}
	else
	{
		OutputBit(_OUTPUT_L1_3PH_POWER, _OFF);
		OutputBit(_OUTPUT_ETC_3PH_POWER, _OFF);
	}
}
//===================================================================================
void CComizoa_Class::L2_Motor_Power_OnOff(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return ; }
	
	if(!COMI_Initial) return ; 
	
	if(flag== _ON)	
	{
		OutputBit(_OUTPUT_L2_3PH_POWER, _ON);
		OutputBit(_OUTPUT_ETC_3PH_POWER, _ON);
	}
	else
	{
		OutputBit(_OUTPUT_L2_3PH_POWER, _OFF);
		OutputBit(_OUTPUT_ETC_3PH_POWER, _OFF);
	}
}
//===================================================================================
BOOL CComizoa_Class::isL1_Motor_Power_On()
//-----------------------------------------------------------------------------------
{
	if(!COMI_Initial) return FALSE;
	
	if(isOutputBit(_OUTPUT_L1_3PH_POWER))// && isOutputBit(_OUTPUT_ETC_3PH_POWER))
		return TRUE;
	else 
		return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL2_Motor_Power_On()
//-----------------------------------------------------------------------------------
{
	if(!COMI_Initial) return FALSE;
	
	if(isOutputBit(_OUTPUT_L2_3PH_POWER))// && isOutputBit(_OUTPUT_ETC_3PH_POWER))
		return TRUE;
	else 
		return FALSE;
}
//===================================================================================
//===================================================================================
void CComizoa_Class::InFeed_Cylinder_For(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_INFEEDER_BAK, _OFF);		OutputBit(_OUTPUT_INFEEDER_FOR, _ON);	}	
	else		{ OutputBit(_OUTPUT_INFEEDER_FOR, _OFF);		OutputBit(_OUTPUT_INFEEDER_BAK, _OFF);	}
}
//===================================================================================
void CComizoa_Class::InFeed_Cylinder_Bak(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_INFEEDER_FOR, _OFF);		OutputBit(_OUTPUT_INFEEDER_BAK, _ON);	}	
	else		{ OutputBit(_OUTPUT_INFEEDER_BAK, _OFF);		OutputBit(_OUTPUT_INFEEDER_FOR, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isInFeed_Cylinder_For()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_INFEEDER_CYL_SEN_BAK_1) || !InputBit(_INPUT_INFEEDER_CYL_SEN_BAK_2)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isInFeed_Cylinder_Bak()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_INFEEDER_CYL_SEN_BAK_1) && InputBit(_INPUT_INFEEDER_CYL_SEN_BAK_2)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::OutFeed_Cylinder_For(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_OUTFEEDER_BAK, _OFF);		OutputBit(_OUTPUT_OUTFEEDER_FOR, _ON);	}	
	else		{ OutputBit(_OUTPUT_OUTFEEDER_FOR, _OFF);		OutputBit(_OUTPUT_OUTFEEDER_BAK, _OFF);	}
}
//===================================================================================
void CComizoa_Class::OutFeed_Cylinder_Bak(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_OUTFEEDER_FOR, _OFF);		OutputBit(_OUTPUT_OUTFEEDER_BAK, _ON);	}	
	else		{ OutputBit(_OUTPUT_OUTFEEDER_BAK, _OFF);		OutputBit(_OUTPUT_OUTFEEDER_FOR, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isOutFeed_Cylinder_For()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_OUTFEEDER_CYL_SEN_BAK_1) || !InputBit(_INPUT_OUTFEEDER_CYL_SEN_BAK_2)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isOutFeed_Cylinder_Bak()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_OUTFEEDER_CYL_SEN_BAK_1) && InputBit(_INPUT_OUTFEEDER_CYL_SEN_BAK_2)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::L1_InkBox_Cylinder_Up(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L1_INKBOX_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L1_INKBOX_CYL_UP, _ON);	}	
	else		{ OutputBit(_OUTPUT_L1_INKBOX_CYL_UP, _OFF);		OutputBit(_OUTPUT_L1_INKBOX_CYL_DOWN, _OFF);	}
}
//===================================================================================
void CComizoa_Class::L1_InkBox_Cylinder_Down(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L1_INKBOX_CYL_UP, _OFF);		OutputBit(_OUTPUT_L1_INKBOX_CYL_DOWN, _ON);	}	
	else		{ OutputBit(_OUTPUT_L1_INKBOX_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L1_INKBOX_CYL_UP, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isL1_InkBox_Cylinder_Up()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_L1_INKBOX_CYL_SEN_DOWN_1) || !InputBit(_INPUT_L1_INKBOX_CYL_SEN_DOWN_2)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL1_InkBox_Cylinder_Down()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_L1_INKBOX_CYL_SEN_DOWN_1) && InputBit(_INPUT_L1_INKBOX_CYL_SEN_DOWN_2)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::L2_InkBox_Cylinder_Up(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L2_INKBOX_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L2_INKBOX_CYL_UP, _ON);	}	
	else		{ OutputBit(_OUTPUT_L2_INKBOX_CYL_UP, _OFF);		OutputBit(_OUTPUT_L2_INKBOX_CYL_DOWN, _OFF);	}
}
//===================================================================================
void CComizoa_Class::L2_InkBox_Cylinder_Down(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L2_INKBOX_CYL_UP, _OFF);		OutputBit(_OUTPUT_L2_INKBOX_CYL_DOWN, _ON);	}	
	else		{ OutputBit(_OUTPUT_L2_INKBOX_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L2_INKBOX_CYL_UP, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isL2_InkBox_Cylinder_Up()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_L2_INKBOX_CYL_SEN_DOWN_1) || !InputBit(_INPUT_L2_INKBOX_CYL_SEN_DOWN_2)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL2_InkBox_Cylinder_Down()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_L2_INKBOX_CYL_SEN_DOWN_1) && InputBit(_INPUT_L2_INKBOX_CYL_SEN_DOWN_2)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::L1_BladeMove_Cylinder_For(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_BAK, _OFF);		OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_FOR, _ON);	}	
	else		{ OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_FOR, _OFF);		OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_BAK, _OFF);	}
}
//===================================================================================
void CComizoa_Class::L1_BladeMove_Cylinder_Bak(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_FOR, _OFF);		OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_BAK, _ON);	}	
	else		{ OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_BAK, _OFF);		OutputBit(_OUTPUT_L1_BLADEMOVE_CYL_FOR, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isL1_BladeMove_Cylinder_For()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_L1_BLADE_MOVE_CYL_SEN_FOR) && !InputBit(_INPUT_L1_BLADE_MOVE_CYL_SEN_BAK)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL1_BladeMove_Cylinder_Bak()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_L1_BLADE_MOVE_CYL_SEN_FOR) && InputBit(_INPUT_L1_BLADE_MOVE_CYL_SEN_BAK)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::L2_BladeMove_Cylinder_For(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_BAK, _OFF);		OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_FOR, _ON);	}	
	else		{ OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_FOR, _OFF);		OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_BAK, _OFF);	}
}
//===================================================================================
void CComizoa_Class::L2_BladeMove_Cylinder_Bak(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_FOR, _OFF);		OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_BAK, _ON);	}	
	else		{ OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_BAK, _OFF);		OutputBit(_OUTPUT_L2_BLADEMOVE_CYL_FOR, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isL2_BladeMove_Cylinder_For()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_L2_BLADE_MOVE_CYL_SEN_FOR) && !InputBit(_INPUT_L2_BLADE_MOVE_CYL_SEN_BAK)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL2_BladeMove_Cylinder_Bak()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_L2_BLADE_MOVE_CYL_SEN_FOR) && InputBit(_INPUT_L2_BLADE_MOVE_CYL_SEN_BAK)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::L1_Blade_Cylinder_Up(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L1_BLADE_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L1_BLADE_CYL_UP, _ON);	}	
	else		{ OutputBit(_OUTPUT_L1_BLADE_CYL_UP, _OFF);			OutputBit(_OUTPUT_L1_BLADE_CYL_DOWN, _OFF);	}
}
//===================================================================================
void CComizoa_Class::L1_Blade_Cylinder_Down(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L1_BLADE_CYL_UP, _OFF);			OutputBit(_OUTPUT_L1_BLADE_CYL_DOWN, _ON);	}	
	else		{ OutputBit(_OUTPUT_L1_BLADE_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L1_BLADE_CYL_UP, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isL1_Blade_Cylinder_Up()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_L1_BLADE_CYL_SEN_UP) && !InputBit(_INPUT_L1_BLADE_CYL_SEN_DOWN)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL1_Blade_Cylinder_Down()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_L1_BLADE_CYL_SEN_UP) && InputBit(_INPUT_L1_BLADE_CYL_SEN_DOWN)) return TRUE;	
	return FALSE;
}

//===================================================================================
void CComizoa_Class::L2_Blade_Cylinder_Up(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L2_BLADE_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L2_BLADE_CYL_UP, _ON);	}	
	else		{ OutputBit(_OUTPUT_L2_BLADE_CYL_UP, _OFF);			OutputBit(_OUTPUT_L2_BLADE_CYL_DOWN, _OFF);	}
}
//===================================================================================
void CComizoa_Class::L2_Blade_Cylinder_Down(int flag)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return ; }
	if(flag)	{ OutputBit(_OUTPUT_L2_BLADE_CYL_UP, _OFF);			OutputBit(_OUTPUT_L2_BLADE_CYL_DOWN, _ON);	}	
	else		{ OutputBit(_OUTPUT_L2_BLADE_CYL_DOWN, _OFF);		OutputBit(_OUTPUT_L2_BLADE_CYL_UP, _OFF);	}
}
//===================================================================================
BOOL CComizoa_Class::isL2_Blade_Cylinder_Up()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(InputBit(_INPUT_L2_BLADE_CYL_SEN_UP) && !InputBit(_INPUT_L2_BLADE_CYL_SEN_DOWN)) return TRUE;	
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::isL2_Blade_Cylinder_Down()
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE || !COMI_Initial) { return FALSE; }
	if(!InputBit(_INPUT_L2_BLADE_CYL_SEN_UP) && InputBit(_INPUT_L2_BLADE_CYL_SEN_DOWN)) return TRUE;	
	return FALSE;
}


