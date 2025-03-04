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
// GetEventIntString(): �̺�Ʈ ���ͷ�Ʈ �����ڵ��� �� ��Ʈ���� �ǹ��ϴ� ���ͷ�Ʈ
// �̸��� ��Ʈ������ ��ȯ���ִ� �Լ�.
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
	if(nBufferLen > nStrLen){ // ����ũ�Ⱑ ����ϸ� full string�� �������ְ�...
		strcpy(szBuffer, szEvtState[nBitIdx]);
	}else{ // �׷��� ������ ����ũ�⿡ �´� ���ڼ��� �������ش�.
		memcpy(szBuffer, szEvtState[nBitIdx], nBufferLen-1);
		szBuffer[nBufferLen-1] = '\0';
	}
}

/***********************************************************************************************
* InterrHandlerCommon(): �� Event Handler���� �������� ȣ���ϴ� �Լ�. 
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
	// �� ���������� ������ ���� ���ͷ�Ʈ�� ó���ϴ� ���̹Ƿ� ������ ���� ���ͷ�Ʈ�� �߻��ߴ��� üũ�Ѵ�. 
	// cmmIntReadFlag() �Լ� ��ſ� cmmIntReadErrorStatus()�� cmmIntReadEventStatus()�� ����ؼ���
	// ���ͷ�Ʈ�� �߻��Ǿ������� üũ�� �� �ִ�. �ٸ� �������� ���ͷ�Ʈ ���¸� üũ�� ������ 
	// cmmIntReadFlag() �Լ��� ����ϴ� ���� ó���ӵ� ���鿡�� ����� �����ϴ�. 
	if(nAxis < 32){
		bIsTheAxis = cmmGetBit(dwFlag1, nAxis);
	}else{
		bIsTheAxis = cmmGetBit(dwFlag2, (nAxis-32));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����: GUI ���� �۾��� �� �Լ������� �����ϴ� ���� ���ϱ� ���Ͽ� List�� ������ �����۵��� CArray
	// ������ CInterruptEventDlg Ŭ������ m_IntItemArray ��������� �����ϰ�, CInterruptEventDlg Ÿ�̸�
	// �̺�Ʈ���� m_IntItemArray ����� ����� �����۵��� ���÷����ϴ� ������� �����Ǿ���.
	
	// �ش��࿡ ���ͷ�Ʈ�� �߻��Ǿ�����... //
	if(bIsTheAxis){
		pSystem.GetMotionComponent()->Winder_Stop();
//		AfxMessageBox("...");
	}
*/
}

/***********************************************************************************************
*                       CALLBACK ��� ó�� 
************************************************************************************************/
void WINAPI InterruptCallback (LPVOID lParam)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// ����: �����峻���� GUI �����۾��� �����ϸ� ������ �߻���Ű�� �Ͱ� ���������� CALLBACK
	// �Լ������� GUI �����۾��� �����ϸ� ����ġ���� ������ �߻���ų �� �����Ƿ�, ������ 
	// GUI �����۾��� ���ϴ� ���� ����. ��, GUI�� �Ӽ��� �����ϴ� ���� �������.
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

//		cmmErrShowLast(GetSafeHwnd()); // ���� �߻��Ͽ����� ���� ���÷����մϴ�.
		return FALSE;
	}else{
		// Motion ȯ�� �ʱ�ȭ //

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
	// [CAUTION]: �Ʒ��� �ٸ� CME������ ����ϴ� ��쿡�� �Ʒ� �ڵ带 �����Ͽ� �ùٸ� CME ������
	// ��θ� �����Ͻʽÿ�.
	#define CME_FILE_NAME	"Motion_Config.cme2" 

	char szCmeFilePath[MAX_PATH];//, szSystemDir[MAX_PATH];
//	GetSystemDirectory(szSystemDir, MAX_PATH); // �������� �ý��� ���丮�� ������ (�Ϲ������δ� C:\Windows\System32)
	sprintf(szCmeFilePath, "%s\\%s", ModelFile()->strDirConfigPath, CME_FILE_NAME);

	if(cmmGnInitFromFile(szCmeFilePath) != cmERR_NONE){
		strText.Format("[ERROR] InitMotionDevices()-> CME-File Load Error! [%s]", szCmeFilePath);
		Log()->SaveLogFile(_MOTION_LOG, strText);
		cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
		return FALSE;
	}
	else{
		////////////////////////////////////////////////////////////////////////////////////////
		// CME ���Ͽ��� ������ ����� ���� �����Ǿ� �ִ� ��� ����� ��Ȯ�� �ϰ����� ���� 
		// �Ʒ��� ���� �ϸ� �˴ϴ�.
		long nNumAxes_installed, nNumAxes_defined;
		cmmAdvGetNumAvailAxes(&nNumAxes_installed); // ���� ������ PC�� �����Ǿ� �ִ� ����� �� (cmmGnDeviceLoad() �Լ����� ��ȯ�ϴ� ����� ����)
		cmmAdvGetNumDefinedAxes(&nNumAxes_defined); // CME ���Ͽ��� ������ �� ��(CME ������ �������� �ʴ� ��쿡�� ���� ������ �� ���� ��ġ�մϴ�.)
		if(nNumAxes_installed != nNumAxes_defined){
			Log()->SaveLogFile(_MOTION_LOG, "[ERROR] ���� �����Ǿ� �ִ� ��� �� ���� ���ǵ� �� ���� ��ġ���� �ʽ��ϴ�!");
			MessageBox(NULL, "���� �����Ǿ� �ִ� ��� �� ���� ���ǵ� �� ���� ��ġ���� �ʽ��ϴ�", "Warning", MB_OK | MB_ICONWARNING);
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
BOOL CComizoa_Class::isDone(long axis) //�Ϸ� �� TRUE����
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
BOOL CComizoa_Class::isHomeBusy(long axis) //�������̸� TRUE����
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return FALSE; }

	if(!COMI_Initial) return FALSE;
	BOOL flag= TRUE;
	long ret= 0;
	if(axis== _M_ALL) {
		for(int i= 0; i< _MAX_AXIS; i++) {
			cmmHomeIsBusy(i, &ret); //TRUE�̸� ���� ��
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
	cmmHomeWaitDone(axis, cmFALSE); // �ش����� Home Return�� �Ϸ��ߴ��� üũ�մϴ�.
	cmmHomeGetSuccess(axis, &dwIsSuccess); // �������Ͱ� �����Ǿ����� üũ 
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
	if(dwIsSuccess) { // �������� ����
		strText.Format("[HOME] �������� ���� - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
		return TRUE;
	}
	long dwErrCode;
	char szErrReason[CMM_MAX_STR_LEN_ERR];

	cmmErrGetLastCode(&dwErrCode);
	cmmErrGetString(dwErrCode, szErrReason, CMM_MAX_STR_LEN_ERR);

	strText.Format("[ERROR] �������� ���� - %d", axis);		Log()->SaveLogFile(_MOTION_LOG, strText);
	strText.Format("[ERROR] ���� ����: %s", szErrReason);	Log()->SaveLogFile(_MOTION_LOG, strText);
//	cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
	// ������ �������͸� ���������� �����ߴ��� üũ�ϰ����� ������ cmmHomeGetSuccess()
	// �Լ��� ����Ѵ�. cmmHomeGetSuccess()���� ��ȯ�ϴ� �÷��״� ���α׷��� 
	// ����Ǿ��ٰ� �ٽ� ����Ǿ ��� �����ȴ�(��, PC�� ����õǸ� ���µȴ�)
	long dwIsSuccess;
	cmmHomeGetSuccess(axis, &dwIsSuccess); // �������Ͱ� �����Ǿ����� üũ 
	CString strText;
	if(dwIsSuccess ){ 
		if(flag) {
			// ������ �������Ͱ� �̹� ����Ǿ�����...(�̰��� ���α׷��� ����Ǿ��� �ٽ� ����Ǿ��� ���� ��ȿ�ϴ�)
			strText.Format("[HOME] ������ �������Ͱ� �̹� ����Ǿ���! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
			MotorAllStop();
		}
		return TRUE;
	}
	if(flag) {
		strText.Format("[HOME] �������� �����ؾ���! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
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

	// �������� ���� //
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
		cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
		strText.Format("[ERROR] �������� ���� ����! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
		return FALSE;
	}
	strText.Format("[HOME] �������� ����! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
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
//			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
//	cmmSxSetSpeedRatio(axis, cmSMODE_C/*Keep prev. setting*/, 100, 100, 100); // Speed ratio ����: �������� ���ǻ� 100%�� ������
	if(cmmSxVMoveStart(axis, dir) != cmERR_NONE){ 
			CString strTTT; strTTT.Format("%d : JogStart_Z() cmmSxVMoveStart", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
	// [CAUTION] cmmSxMove() �Լ��� IsBlocking �Ķ���Ϳ� ���Ͽ�:
	// �������� ���ǻ� �̼��߿��� Ÿ�̸� �̺�Ʈ�� Stop ��ư �̺�Ʈ ó���� �� �� �ֵ��� 
	// IsBlocking �Ķ���͸� FALSE�� �Ͽ���. ������ �����带 ����� ���� IsBlocking 
	// �Ķ���͸� �ݵ�� TRUE�� �ؾ��Ѵ�. 
	// cmmSxMoveTo  : ����ġ �̵� & EndDone, cmmSxMoveToStart	: ����ġ �̵� & return
	// cmmSxMove	: ���ġ �̵� & EndDone, cmmSxMoveStart		: ���ġ �̵� & return
	if(!COMI_Initial) return FALSE; 

	if(isServoOn(axis)== FALSE) {
		Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
		AfxMessageBox("���� ON�� �ȵǾ� �ֽ��ϴ�."); return FALSE;
	}

	if(axis== _AXIS_1L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_1L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!   �̵� ���� �Ÿ� �ʰ� �Է�!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_2L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_2L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!   �̵� ���� �Ÿ� �ʰ� �Է�!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!  �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox("[_AXIS_1L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); return FALSE;
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!  �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox("[_AXIS_2L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); return FALSE;
		}
	}

	if(axis== _AXIS_1L_STAGE_X) {	
		double CurPos= pSystem.GetMotionComponent()->Get_Position(_AXIS_1L_STAGE_X);
		if(CurPos >= _X_AXIS_MAX_ || CurPos<= _X_AXIS_MIN_)
		{
				Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_STAGE_X (ABS_Move)] �̵� �Ÿ� �ʰ�!");
				return TRUE;
		}
	}
	else if(axis== _AXIS_2L_STAGE_X) {	
		double CurPos= pSystem.GetMotionComponent()->Get_Position(_AXIS_2L_STAGE_X);
		if(CurPos >= _X_AXIS_MAX_ || CurPos<= _X_AXIS_MIN_)
		{
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_STAGE_X (ABS_Move)] �̵� �Ÿ� �ʰ�!");
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
				cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
				cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	else {
		if(cmmSxMoveStart(axis, dOutPulse) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d : INC_Move() cmmSxMoveStart-3", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
	// [CAUTION] cmmSxMove() �Լ��� IsBlocking �Ķ���Ϳ� ���Ͽ�:
	// �������� ���ǻ� �̼��߿��� Ÿ�̸� �̺�Ʈ�� Stop ��ư �̺�Ʈ ó���� �� �� �ֵ��� 
	// IsBlocking �Ķ���͸� FALSE�� �Ͽ���. ������ �����带 ����� ���� IsBlocking 
	// �Ķ���͸� �ݵ�� TRUE�� �ؾ��Ѵ�. 
	// cmmSxMoveTo  : ����ġ �̵� & EndDone, cmmSxMoveToStart	: ����ġ �̵� & return
	// cmmSxMove	: ���ġ �̵� & EndDone, cmmSxMoveStart		: ���ġ �̵� & return
	if(!COMI_Initial) return FALSE; 
	if(isServoOn(axis)== FALSE) {
		Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
		AfxMessageBox("���� ON�� �ȵǾ� �ֽ��ϴ�."); return FALSE;
		return FALSE;	}

	if(!isMotion_AlarmCheck()) return FALSE; 

	if(axis== _AXIS_1L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_1L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!   �̵� ���� �Ÿ� �ʰ� �Է�!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_2L_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_2L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!   �̵� ���� �Ÿ� �ʰ� �Է�!");
			return FALSE;
		}
	}
	else if(axis== _AXIS_1L_ROLL_Z1_SYNC || axis== _AXIS_1L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_1L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!  �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox("[_AXIS_1L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); return FALSE;
		}
	}
	else if(axis== _AXIS_2L_ROLL_Z1_SYNC || axis== _AXIS_2L_ROLL_Z2_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_2L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!  �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox("[_AXIS_2L_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); return FALSE;
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
				cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
				cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	else {
		if(cmmSxMoveToStart(axis, dOutPulse) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d : ABS_Move() cmmSxMoveToStart-3", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
	// [CAUTION] cmmSxMove() �Լ��� IsBlocking �Ķ���Ϳ� ���Ͽ�:
	// �������� ���ǻ� �̼��߿��� Ÿ�̸� �̺�Ʈ�� Stop ��ư �̺�Ʈ ó���� �� �� �ֵ��� 
	// IsBlocking �Ķ���͸� FALSE�� �Ͽ���. ������ �����带 ����� ���� IsBlocking 
	// �Ķ���͸� �ݵ�� TRUE�� �ؾ��Ѵ�. 
	// cmmSxMoveTo  : ����ġ �̵� & EndDone, cmmSxMoveToStart	: ����ġ �̵� & return
	// cmmSxMove	: ���ġ �̵� & EndDone, cmmSxMoveStart		: ���ġ �̵� & return

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
		Log()->SaveLogFile(_MOTION_LOG, "[ABS_Z_Offset_Move] �̵� �Ÿ� �Է��� ������ �մϴ�!!! ���ͷ� ����ġ�� �ʰ�!!!");
		AfxMessageBox("[ABS_Z_Offset_Move] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n���ͷ� ����ġ�� �ʰ�!!!"); return FALSE;
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
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
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
		Log()->SaveLogFile(_MOTION_LOG, "���� �۾��� ���� �����Ͻʽÿ�!");
		MyMessageBox("���� �۾��� ���� �����Ͻʽÿ�!. [_AXIS_1L_STAGE_X]", "Origin Failed", M_ICONERROR);
	}
	else if(AxisHomeStat[_AXIS_2L_STAGE_X]!= _M_END) {
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "���� �۾��� ���� �����Ͻʽÿ�!");
		MyMessageBox("���� �۾��� ���� �����Ͻʽÿ�!. [_AXIS_2L_STAGE_X]", "Origin Failed", M_ICONERROR);
	}
	else if(AxisHomeStat[_AXIS_1L_ROLL_Z]!= _M_END)	{
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "���� �۾��� ���� �����Ͻʽÿ�!");
		MyMessageBox("���� �۾��� ���� �����Ͻʽÿ�!. [_AXIS_1L_ROLL_Z]", "Origin Failed", M_ICONERROR);	
	}
	else if(AxisHomeStat[_AXIS_2L_ROLL_Z]!= _M_END)	{
		flag= FALSE;
		Log()->SaveLogFile(_MOTION_LOG, "���� �۾��� ���� �����Ͻʽÿ�!");
		MyMessageBox("���� �۾��� ���� �����Ͻʽÿ�!. [_AXIS_2L_ROLL_Z]", "Origin Failed", M_ICONERROR);	
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
		Log()->SaveLogFile(_MOTION_LOG, "[Z_Offset_Pos_Move] �̵� �Ÿ� �Է��� ������ �մϴ�!!! ���ͷ� ����ġ�� �ʰ�!!!");
		AfxMessageBox("[Z_Offset_Pos_Move] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n���ͷ� ����ġ�� �ʰ�!!!"); return FALSE;
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


