// Comizoa_Class.cpp: implementation of the CComizoa_Class class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintSystem_1.h"
#include "Comizoa_Class.h"

#include "mmsystem.h"
#include "Cmmsdk.h"
#include "CmmsdkDef.h"

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
/*	long dwFlag1, dwFlag2;
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
	for(i= 0; i< 5; i++) {
		lOutPulse_X[i]= 0;		lOutPulse_T[i]= 0;		lOutPulse_B[i]= 0;
		dOutPulse_X[i]= 0.0;	dOutPulse_T[i]= 0.0;	dOutPulse_B[i]= 0.0;
	}
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
	if(cmmGnDeviceLoad(TRUE, &nNumAxes) != cmERR_NONE){  //�ý��ۿ� ��ġ�� �ϵ���� ��ġ�� �ε��ϰ� ��ġ��  �ʱ�ȭ �մϴ�. (CMMSDK�� �ٸ� �Լ��� ȣ��Ǳ����� �ݵ�� �ѹ��� ����Ǿ�� �մϴ�. 
		//cmmGnDeviceLoad(IsResetDevice(0�Ǵ� cmFalse �ʱ�ȭ���� �ʴ´�. 1�Ǵ� cmTrue �⺻������ �ʱ�ȭ�Ѵ�. , 
										//&nNumAxes -> ������ �ε�� ��� ���� ���� ������ ��ȯ�մϴ�. (NULL)�� �����ϸ� �� ���� ��ȯ���� �ʴ´�.)
										//cmERR_NONE ���༺�� 
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] COMI-Device Load");

//		cmmErrShowLast(GetSafeHwnd()); // ���� �߻��Ͽ����� ���� ���÷����մϴ�.
		return FALSE;
	}else{
		// Motion ȯ�� �ʱ�ȭ //

		if(!InitMotionDevices()) { // �ʱ�ȭ?
			Log()->SaveLogFile(_MOTION_LOG, "[ERROR] COMI-Device Initial");
			return FALSE;
		}
 		ServoOn(_AXIS_ROLLER, _ON); //cmmGnSetServoOn(ä��, 0->���� 1->�ѱ�)
 		ServoOn(_AXIS_ROLL_Z1, _ON); ServoOn(_AXIS_ROLL_Z2, _ON);
 		ServoOn(_AXIS_STAGE_X, _ON);
		COMI_Initial= TRUE;
//		OutputBit(_OUTPUT_POWER_ON, _ON); //��� �߻� 1->on 
	//	ServoAllOn(TRUE); // ��� �ѱ� 
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
	sprintf(szCmeFilePath, "%s\\%s", ModelFile()->strDirConfigPath, CME_FILE_NAME); //������ ������ 

	if(cmmGnInitFromFile(szCmeFilePath) != cmERR_NONE){ //cmmGnInitFromFile-> Ŀ������ ȯ�� �������� CME2�� ���� ������ ������ �������� �� ������ ����� ��ġ�� ���� ȯ���� �ڵ����� ����
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

//	for(int i= 0; i<3; i++) MotionSpeedCheck(i, ModelFile()->Model_data[i].dPrint_Speed);

	long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
	long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
	cmmCfgSetSoftLimit (_AXIS_STAGE_X, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit ����Ʈ���� ���� ����� Ȱ�� �Ǵ� ��Ȱ���ϰ� ���� ������ ���� 
	//	cmmCfgSetSoftLimit(���ȣ, ��� Ȱ��ȭ ���� ���� , -���� ���� �� ���� , +���� ���� �� ���� )
	lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z1]);
	lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z1]);
	cmmCfgSetSoftLimit (_AXIS_ROLL_Z1, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z2]);
	lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z2]);
	cmmCfgSetSoftLimit (_AXIS_ROLL_Z2, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit

// 	lMinPulse= (long)(_AXIS_ROLLER * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
// 	lMaxPulse= (long)(_AXIS_ROLLER * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
// 	cmmCfgSetSoftLimit (_AXIS_ROLLER, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit


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
//	//if(SIMULATION_MODE) { return TRUE; }

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
/*	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	long dwTemp;
	cmmStReadMioStatuses(_AXIS_STAGE_X, &dwTemp); // Read all MIO states
	if( (dwTemp>>cmIOST_ALM) & 0x1)	{
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] isMotion_ReadyCheck [_AXIS_STAGE_X");
		return FALSE; // ALM 
	}
	else			{
		return TRUE;
	}

  */

return TRUE; //�ӽ� 
}
//===================================================================================
BOOL CComizoa_Class::isEMO_Check()
//-----------------------------------------------------------------------------------
{
	if(!COMI_Initial) return FALSE;

	if(!InputBit(_INPUT_EMO_SWITCH) )
		return FALSE;
	else					
		return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isMotion_AlarmCheck(long nAxis)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	long dwTemp;
	//---- MIO Display ----------------------------------------------------//
	cmmStReadMioStatuses(nAxis, &dwTemp); // Read all MIO states (��ǰ��� ���� ��ȯ)
	if( (dwTemp>>cmIOST_ALM) & 0x1)	
		{
		Log()->SaveLogFile(_MOTION_LOG, "[ERROR] isMotion_AlarmCheck");
		return TRUE;
	}
	else			{
				return FALSE; // ALM 
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
//	//if(SIMULATION_MODE) { return TRUE; }

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
//	//if(SIMULATION_MODE) { return FALSE; }

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
//	//if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ;
	
	CString strText;
	strText.Format("[MOTION] ResetPos - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
	if(axis== _M_ALL) 
	{
		for(int i= 0; i< _MAX_AXIS; i++) 
		{
			cmmStSetCount(i, cmCNT_COMM, 0); // Reset COMMAND Position
			cmmStSetCount(i, cmCNT_FEED, 0); // Reset FEEDBACK Position
			cmmStSetPosition(i, cmCNT_COMM, 0.0); // Reset COMMAND Position
			cmmStSetPosition(i, cmCNT_FEED, 0.0); // Reset FEEDBACK Position
		}
	}
	else 
	{
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
	//if(SIMULATION_MODE) { return ; }

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
	if(axis== _AXIS_STAGE_X) {
		long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
		long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
		cmmCfgSetSoftLimit (_AXIS_STAGE_X, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_ROLL_Z1 || axis== _AXIS_ROLL_Z2) {
		long lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z1]);
		long lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z1]);
		cmmCfgSetSoftLimit (_AXIS_ROLL_Z1, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
		cmmCfgSetSoftLimit (_AXIS_ROLL_Z2, TRUE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
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
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE;
	cmmDoPutOne(no, status); //cmmDoPutOne �Լ��� ������ ������ ��� ä���� ���� ������ ����� �߻� 
	//cmmDoPutOne(ä�� , 0->off 1->on
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isHomeReturnEheck(long axis, bool flag)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

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
// 	//if(SIMULATION_MODE) { return TRUE; }
// 
// 	if(	isHomeReturnEheck(_AXIS_STAGE_X, false) && 
// 		isHomeReturnEheck(_AXIS_ROLLER, false) &&
// 		isHomeReturnEheck(_AXIS_ROLL_Z, false) ) return TRUE;
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::MotionHome(long axis)
//-----------------------------------------------------------------------------------
{
	if(SIMULATION_MODE) { return TRUE; }

	CString strText;
	if(!COMI_Initial) return FALSE;

	if(axis== _AXIS_STAGE_X) {
		long lMinPulse= (long)(_X_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
		long lMaxPulse= (long)(_X_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
		cmmCfgSetSoftLimit (_AXIS_STAGE_X, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}
	if(axis== _AXIS_Z_SYNC) {
		long lMinPulse= (long)(_Z_AXIS_MIN_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z1]);
		long lMaxPulse= (long)(_Z_AXIS_MAX_ * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z1]);
		cmmCfgSetSoftLimit (_AXIS_ROLL_Z1, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
		cmmCfgSetSoftLimit (_AXIS_ROLL_Z2, FALSE, lMinPulse, lMaxPulse); // [SL] Enable, (-)SW Limit, (+)SW Limit
	}

// 	long MioStatus= 0;
// 
// 	if(axis== _AXIS_ROLLER) {
// 		cmmStReadMioStatuses(_AXIS_ROLLER, &MioStatus);
// 		if((MioStatus >> cmIOST_ORG) & 0x01) {
// 			strText.Format("[MotionHome] _AXIS_ROLL_UPPER-> ���� ����-ON");	Log()->SaveLogFile(_MOTION_LOG, strText);
// 			//			AfxMessageBox("ORG-ON");
// // 			double ddd= (ModelFile()->Motion_data.dROLL_Diameter-10.)*_PI_;
// // 			strText.Format("[MotionHome] _AXIS_ROLL_UPPER-> ���� ���� Ż�� [%f]", ddd);	Log()->SaveLogFile(_MOTION_LOG, strText);
// // 			if(INC_Move(_AXIS_ROLLER,  ddd, TRUE)== FALSE) {
// // 				strText.Format("[ERROR] _AXIS_ROLL_UPPER-> ���� ���� Ż�� ����");	Log()->SaveLogFile(_MOTION_LOG, strText);
// // 				return FALSE;
// // 			}
// // 			strText.Format("[MotionHome] _AXIS_ROLL_UPPER-> ���� ���� Ż�� �Ϸ�");	Log()->SaveLogFile(_MOTION_LOG, strText);
// 		}
// 	}

	// �������� ���� //
//	ResetPos(axis) ;
//	cmmCfgSetMioProperty (axis, cmORG_LOGIC, cmLOGIC_B); // ORG Input Logic
//	cmmCfgSetMioProperty (axis, cmEZ_LOGIC, cmLOGIC_A); // EZ Input Logic
//	cmmCfgSetMioProperty (axis, cmERC_OUT, cmLOGIC_A); // Output ERC signal at completion

	//if(axis== _AXIS_ROLLER) cmmSxOptSetIniSpeed(_AXIS_ROLLER, ModelFile()->Motion_data.dStart_Speed[_AXIS_ROLLER]);

	cmmHomeSetSpeedPattern(axis, cmSMODE_S,ModelFile()->Motion_data.dOrigin_Speed[axis],
		ModelFile()->Motion_data.dOrigin_Accel[axis], 
		ModelFile()->Motion_data.dOrigin_Decel[axis], 
		ModelFile()->Motion_data.dOrigin_Speed[axis]/3.); 

	Sleep(100);
	if(axis== _AXIS_ROLLER) 
	{
	//	MoveStop(axis);
		Sleep(500);
		if(cmmHomeMoveStart(axis, cmDIR_N) != cmERR_NONE){
			strText.Format("[ERROR] �������� ���� ����! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	else if (axis == _AXIS_Z_SYNC)
	{
		Sleep(500);

		cmmHomeSetSpeedPattern(_AXIS_ROLL_Z1, cmSMODE_S,ModelFile()->Motion_data.dOrigin_Speed[_AXIS_ROLL_Z1],
			ModelFile()->Motion_data.dOrigin_Accel[_AXIS_ROLL_Z1], 
			ModelFile()->Motion_data.dOrigin_Decel[_AXIS_ROLL_Z1], 
			5000.0); 

		cmmHomeSetSpeedPattern(_AXIS_ROLL_Z2, cmSMODE_S,ModelFile()->Motion_data.dOrigin_Speed[_AXIS_ROLL_Z2],
			ModelFile()->Motion_data.dOrigin_Accel[_AXIS_ROLL_Z2], 
			ModelFile()->Motion_data.dOrigin_Decel[_AXIS_ROLL_Z2], 
			5000.0); 

		if(cmmHomeMoveStart(_AXIS_ROLL_Z1, cmDIR_N) != cmERR_NONE){
			strText.Format("[ERROR] �������� ���� ����! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
		if(cmmHomeMoveStart(_AXIS_ROLL_Z2, cmDIR_N) != cmERR_NONE){
			strText.Format("[ERROR] �������� ���� ����! - %d", axis);	Log()->SaveLogFile(_MOTION_LOG, strText);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	else if(cmmHomeMoveStart(axis, cmDIR_N) != cmERR_NONE)
	{
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
	//if(SIMULATION_MODE) { return TRUE; }

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
	//if(SIMULATION_MODE) { return TRUE; }

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
	//if(SIMULATION_MODE) { return 0; }

	long nStatus = 0x0;
	if(!COMI_Initial) return FALSE;
	if ( cmmDiGetOne(no, &nStatus) == cmERR_NONE)
	{
		return nStatus;
	}
	return cmFALSE;
}
//===================================================================================
void CComizoa_Class::ServoOn(long no, long status)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!COMI_Initial) return ; 
//	OutputBit(_OUTPUT_POWER_ON, _ON);

	cmmGnSetServoOn(no, status); // cmmGnSetServoOn �Լ��� ������ ä��(��)�� servo-on ��ȣ ����� ���� 1(���� ��)
}
//===================================================================================
long CComizoa_Class::isServoOn(long no)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return 1; }
	long nStatus = 0x0;
	if(!COMI_Initial) return FALSE; 
	cmmGnGetServoOn(no, &nStatus);
	return nStatus;
}
//===================================================================================
void CComizoa_Class::ServoAllOn(long status)
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }
	if(!COMI_Initial) return ; 
 	cmmGnSetServoOn(_AXIS_STAGE_X,	status);
 	cmmGnSetServoOn(_AXIS_ROLLER,	status);
 	cmmGnSetServoOn(_AXIS_ROLL_Z1,	status);
	cmmGnSetServoOn(_AXIS_ROLL_Z2,	status);
}
//===================================================================================
void CComizoa_Class::MotorAllStop()
//-----------------------------------------------------------------------------------
{
//	//if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ; 
	long nAxes_1[4]={0, 1, 2, 3};
	cmmMxStop(4, nAxes_1, FALSE, FALSE);
	CString strLog; strLog.Format("[MOTOR] MotorAllStop");	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
}
//===================================================================================
void CComizoa_Class::MotorAll_EMG_Stop()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return ; }

	if(!COMI_Initial) return ; 

	long nAxes_1[4]={0, 1, 2, 3};

	cmmMxStopEmg(4, nAxes_1);

}
//===================================================================================
BOOL CComizoa_Class::JogStart(long axis, long dir, int spd)
//-----------------------------------------------------------------------------------
{
//	int Z1Dir;
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	double SetSpeed= 0.0;

	//cmmSxOptSetIniSpeed(axis, ModelFile()->Motion_data.dStart_Speed[axis]);
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

	cmmCfgSetSpeedPattern(axis, cmSMODE_S, SetSpeed,
		SetSpeed * 10, SetSpeed * 10);
	cmmSxSetSpeedRatio(axis, cmSMODE_KEEP, 100.0, 100.0, 100.0);

	if(cmmSxVMoveStart(axis, dir) != cmERR_NONE){ 
		CString strTTT; strTTT.Format("%d : JogStart() cmmSxVMoveStart", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
		return FALSE;
	}
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::JogStop(long axis)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	MoveStop(axis);

	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::MoveStop(long axis)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	cmmSxStop(axis, FALSE, FALSE);
	CString strLog; strLog.Format("[MOTOR] MoveStop= %d", axis);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::MoveEmgStop(long axis)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	cmmSxStopEmg(axis);

	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::INC_Move(long axis, double dist, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { Sleep(10); return TRUE; }

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

	if(axis== _AXIS_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			AfxMessageBox("[_AXIS_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); 
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_STAGE_X (INC_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!   �̵� ���� �Ÿ� �ʰ� �Է�!");
			return FALSE;
		}
	}
// 	if(axis== _AXIS_ROLL_Z1 || axis == _AXIS_Z_SYNC || axis== _AXIS_ROLL_Z2) {
// 		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
// 			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_ROLL_Z (INC_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!  �̵� ���� �Ÿ� �ʰ� �Է�!");
// 			AfxMessageBox("[_AXIS_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!"); return FALSE;
// 		}
// 	}
// 	if(axis== _AXIS_ROLLER) {
// 		if(dist < 0.0) {
// 			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_ROLLER (INC_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!  -���� �Է� �� ����!");
// 			AfxMessageBox("[_AXIS_ROLLER (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n-���� �Է� �� ����!"); return FALSE;
// 		}
// 	}
	CString strLog; strLog.Format("[MOTOR] INC_Move(%d, %f, %d, %d)", axis, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	double dOutPulse= 0.0;
	long lOutPulse= 0;
	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis]);
	dOutPulse= (double)(lOutPulse);


	//�ӵ� ����


	if (axis == _AXIS_ROLLER)
	{
		if (SyncMode == _SYNC_MOVE)
		{
			double dOutPulse1= 0.0;
			long lOutPulse1= 0;
			lOutPulse1= (long)(ModelFile()->Model_data.dRoll_Vel * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
			dOutPulse1= (double)(lOutPulse1);
			cmmCfgSetSpeedPattern(axis, cmSMODE_S, dOutPulse1,	dOutPulse1 * 5 , dOutPulse1 * 5);  //�� �� �ӵ� ����.... 
		}

	}
	
	if (axis == _AXIS_STAGE_X  || axis == _AXIS_ROLL_Z1 || axis == _AXIS_ROLL_Z2)
	{
		cmmCfgSetSpeedPattern(axis, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[axis],	ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
	}
	
	cmmSxSetSpeedRatio(axis, cmSMODE_S, 100.0, 100.0, 100.0);

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
	//if(SIMULATION_MODE) { Sleep(10); return TRUE; }

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
		return FALSE;}


	if(!isMotion_AlarmCheck()) return FALSE; 

	if(axis== _AXIS_STAGE_X) {	
		if(dist >= _X_AXIS_MAX_ || dist< _X_AXIS_MIN_) { 
			CString strAAA;
			strAAA.Format("[_AXIS_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!\n[ %f ]", dist);
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_STAGE_X (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!! �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox(strAAA);
			return FALSE;
		}
	}
	if(axis== _AXIS_ROLL_Z1 || axis == _AXIS_Z_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			CString strAAA;
			strAAA.Format("[_AXIS_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!\n[ %f ]", dist);
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_ROLL_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!! �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox(strAAA);
		}
	}
// 	if(axis== _AXIS_ROLLER) {
// 		if(dist < Get_Position(axis)) {
// 			CString strLog; 
// 			strLog.Format("[MOTOR] _AXIS_ROLLER-ABS_Move -> MotionHome(%d) [dist=%f < curr=%f])", axis, dist, Get_Position(axis));
// 			Log()->SaveLogFile(_SYSTEM_LOG, strLog);
// 			Log()->SaveLogFile(_MOTION_LOG, strLog);
// 		/*	if(axis== _AXIS_ROLLER && ModelFile()->System_data._LOWER_ROLLER_ORG_SKIP_)
// 			{
// 				strLog.Format("[MOTOR] ABS_Move -> _AXIS_ROLLER �� ���� SKIP!");
// 				Log()->SaveLogFile(_SYSTEM_LOG, strLog);
// 				Log()->SaveLogFile(_MOTION_LOG, strLog);
// 			}*/
// 		//	else 
// 		//	{
// // 				if(MotionHome(axis)) { 
// // 					DWORD st= GetTickCount();
// // 					while(1) {
// // 						if(!isHomeBusy(axis) && isDone(axis)) {
// // 							if(!HomeEndProc(axis)) {
// // 								HomeSearchErrorProc(); return FALSE;
// // 							}
// // 							Log()->SaveLogFile(_SYSTEM_LOG, "[MOTOR] _AXIS_ROLLER-ABS_Move -> MotionHome-END");
// // 							Sleep(500); 
// // 							break;
// // 						}
// // 						if(GetTickCount()-st > (DWORD)ModelFile()->Motion_data.iDelayTime[_HOME_AXIS_TIME_OUT] * 2)
// // 						{
// // 							Log()->SaveLogFile(_SYSTEM_LOG, "[ERROR] _AXIS_ROLLER-ABS_Move -> MotionHome-TimeOut");
// // 							HomeSearchErrorProc(); return FALSE;
// // 						}
// // 						Sleep(10); 
// // 					}
// // 				}
// // 				else {
// // 					Log()->SaveLogFile(_SYSTEM_LOG, "[ERROR] _AXIS_ROLLER-ABS_Move -> MotionHome");
// // 					return FALSE;
// // 				}
// // 		//	}
// 		}
// 	}

	if(!isMotion_AlarmCheck()) return FALSE; 
	CString strLog; strLog.Format("[MOTOR] ABS_Move(%d, %f, %d, %d)", axis, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	double dOutPulse= 0.0;
	long lOutPulse= 0;
	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis]);
	dOutPulse= (double)(lOutPulse);

	if(axis== _AXIS_ROLLER) {
		cmmCfgSetSpeedPattern(axis, cmSMODE_S, 
						ModelFile()->Motion_data.dWork_Speed[axis],
						ModelFile()->Motion_data.dWork_Speed[axis] * 5, 
						ModelFile()->Motion_data.dWork_Speed[axis] * 5);

	}
	else {
		cmmCfgSetSpeedPattern(axis, cmSMODE_S, ModelFile()->Motion_data.dWork_Speed[axis],
						ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
	}
	cmmSxSetSpeedRatio(axis, cmSMODE_S, 100.0, 100.0, 100.0);

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
double CComizoa_Class::Get_Position(long axis)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return 0.0; }

	if(!COMI_Initial) return 0.0; 

	double fCurPos=0.0, Posmm= 0.0;
	// Command position //

// 	if(axis== _AXIS_STAGE_X) {
// 		cmmStGetPosition(axis, cmCNT_COMM, &fCurPos);
// 		Posmm= fCurPos / ModelFile()->Motion_data.dPulsePerMM[axis];
// 	}
// 	else {
		cmmStGetPosition(axis, cmCNT_COMM, &fCurPos);
 		Posmm= fCurPos / ModelFile()->Motion_data.dPulsePerMM[axis];
// 	}
	return Posmm;
}
//===================================================================================
void CComizoa_Class::MotionSpeedCheck(int mode, double speed_set) //ModelFile()->Model_data[mode].dPrint_Speed
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return ; }

// 	lOutPulse_X[mode]= (long) (speed_set * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
// 	lOutPulse_T[mode]= (long)((speed_set * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER])+
// 		ModelFile()->Model_data[mode].iRoll_Work_Length_Offset_Pulse);
// 	lOutPulse_B[mode]= (long)((speed_set * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER])+
// 		ModelFile()->Model_data[mode].iRoll_Work_Length_Offset_Pulse);
// 
// 	dOutPulse_X[mode]= (double)(lOutPulse_X[mode] / ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
// 	dOutPulse_T[mode]= (double)(lOutPulse_T[mode] / ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
// 	dOutPulse_B[mode]= (double)(lOutPulse_B[mode] / ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
}

//===================================================================================
//===================================================================================
BOOL CComizoa_Class::Vac_On()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE; 

// #ifdef _OUTPUT_VAC
// 	return OutputBit(_OUTPUT_VAC, _ON);
// #endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::Vac_Off()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 
// 
// #ifdef _OUTPUT_VAC
// 	return OutputBit(_OUTPUT_VAC, _OFF);
// #endif
	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::TopRollerFreeRun()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { Sleep(10); return TRUE; }

	if(!COMI_Initial) return FALSE; 
// 	MotionSpeedCheck(ModelFile()->System_data.iWORK_MODE_SELECT, ModelFile()->Model_data[ModelFile()->System_data.iWORK_MODE_SELECT].dPrint_Speed) ;
// 	cmmSxOptSetIniSpeed(_AXIS_ROLLER, 0);
// 
// 	cmmCfgSetSpeedPattern(_AXIS_ROLLER, cmSMODE_C, 
// 						dOutPulse_B[ModelFile()->System_data.iWORK_MODE_SELECT]* ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER],
// 						ModelFile()->Motion_data.dWork_Accel[_AXIS_ROLLER], 
// 						ModelFile()->Motion_data.dWork_Decel[_AXIS_ROLLER]);
// 
// 	cmmSxSetSpeedRatio(_AXIS_ROLLER, cmSMODE_KEEP, 100.0, 100.0, 100.0);
// 	if(cmmSxVMoveStart(_AXIS_ROLLER, _DIR_CW) != cmERR_NONE){ 
// 		CString strTTT; strTTT.Format("_AXIS_ROLL_UPPER : TopRollerFreeRun() cmmSxVMoveStart-1");	Log()->SaveLogFile(_MOTION_LOG, strTTT);
// 		cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
// 		return FALSE;
// 	}
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::isMotionReadyCheck()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	int axis_no= 0;
	BOOL flag= TRUE;

	if(!isMotion_AlarmCheck()) return FALSE; 
	if(isServoOn(_AXIS_STAGE_X)== FALSE) {
		if(!ReadyCheckFlagMsg) {
			Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
			ReadyCheckFlagMsg= true; AfxMessageBox("X�� ���� ON�� �ȵǾ� �ֽ��ϴ�.");
		}
		
		return FALSE;
	}

	if(isServoOn(_AXIS_ROLLER)== FALSE) {
		if(!ReadyCheckFlagMsg) {
			Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
			ReadyCheckFlagMsg= true; AfxMessageBox("Roll ���� ON�� �ȵǾ� �ֽ��ϴ�.");
		}
		return FALSE;
	}

	if(isServoOn(_AXIS_ROLL_Z1)== FALSE) {
		if(!ReadyCheckFlagMsg) {
			Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
			ReadyCheckFlagMsg= true; AfxMessageBox("Z1 ���� ON�� �ȵǾ� �ֽ��ϴ�.");
		}
		return FALSE;
	}

	if(isServoOn(_AXIS_ROLL_Z2)== FALSE) {
		if(!ReadyCheckFlagMsg) {
			Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
			ReadyCheckFlagMsg= true; AfxMessageBox("Z2 ���� ON�� �ȵǾ� �ֽ��ϴ�.");
		}
		return FALSE;
	}

// 	if(isServoOn(_AXIS_ROLLER)== FALSE && bSercoOnCheckFlag) {
// 		if(  ModelFile()->System_data.iCURRENT_RUN_STATUS != _CURRENT_RUN_RUN || 
// 			(ModelFile()->System_data.iCURRENT_RUN_STATUS == _CURRENT_RUN_RUN &&
// 			(pSystem.GetProcessThread_MainComponent()->GetStep() <= STEP_MAIN_STEP_START_X_WAIT_POS  ||
// 			pSystem.GetProcessThread_MainComponent()->GetStep() >= STEP_MAIN_STEP_PRINT_END_Z_WAIT_POS_END)) ) 
// 		{
// 
// 			if(!ReadyCheckFlagMsg) {
// 				Log()->SaveLogFile(_MOTION_LOG, "���� ON�� �ȵǾ� �ֽ��ϴ�.");
// 				ReadyCheckFlagMsg= true; AfxMessageBox("���� ON�� �ȵǾ� �ֽ��ϴ�.");
// 			}
// 			return FALSE;
// 		}
// 	}
	if(AxisHomeStat[_AXIS_STAGE_X]!= _M_END)	flag= FALSE;
	if(AxisHomeStat[_AXIS_ROLLER]!= _M_END)		flag= FALSE;
	if(AxisHomeStat[_AXIS_ROLL_Z2]!= _M_END)		flag= FALSE;
	if(AxisHomeStat[_AXIS_ROLL_Z1]!= _M_END)		flag= FALSE;
//f(AxisHomeStat[_AXIS_DISPENSER]!= _M_END)	flag= FALSE;
	ReadyCheckFlagMsg= false;
	return flag;
}

//===================================================================================
BOOL CComizoa_Class::Z_ReadyPosCheck()
//-----------------------------------------------------------------------------------
{
	double Zpos= 0.0;
// 	
// 	Zpos= pSystem.GetMotionComponent()->Get_Position(_AXIS_ROLL_Z);
// 	if((ModelFile()->Model_data[ModelFile()->System_data.iWORK_MODE_SELECT].dPosition_Wait[_AXIS_ROLL_Z]+ 1.0) < (Zpos))
// 		return FALSE;
// 	
	return TRUE;
}

//===================================================================================
void CComizoa_Class::EMG_Proc()
//-----------------------------------------------------------------------------------
{
	int i;
	for(i= 0; i< 16; i++) {
		AxisHomeStat[i]= _M_NONE;
	}
}
//===================================================================================
BOOL CComizoa_Class::isPOWER_ON_Check()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE;
	
#ifdef _INPUT_POWER_ON
	if(InputBit(_INPUT_POWER_ON) )
		return FALSE;
	else 
		return TRUE;
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isAIR_ON_Check()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE;
// 	
// #ifdef _INPUT_VAC_ON
// 	if(InputBit(_INPUT_AIR_ON) )
// 		return TRUE;
// 	else 
// 		return FALSE;
// #endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isF_CYL_DOWN_ON_Check()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE;
	
#ifdef _INPUT_F_DOWN
	if(InputBit(_INPUT_F_DOWN) )
		return TRUE;
	else 
		return FALSE;
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isR_CYL_DOWN_ON_Check()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE;
	
#ifdef _INPUT_R_DOWN
	if(InputBit(_INPUT_R_DOWN) )
		return TRUE;
	else 
		return FALSE;
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isUL_CYL_FOR_ON_Check()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE;
	
#ifdef _INPUT_UL_FOR
	if(InputBit(_INPUT_UL_FOR) )
		return TRUE;
	else 
		return FALSE;
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::isUL_CYL_BAK_ON_Check()
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE;
	
#ifdef _INPUT_UL_BAK
	if(InputBit(_INPUT_UL_BAK) )
		return TRUE;
	else 
		return FALSE;
#endif
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::UL_CYL_FOR_OnOff(int flag)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE; 
	
#ifdef _OUTPUT_UL_FOR
	if(flag) OutputBit(_OUTPUT_UL_BAK, _OFF);
	return OutputBit(_OUTPUT_UL_FOR, flag);
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::UL_CYL_BAK_OnOff(int flag)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE; 
	
#ifdef _OUTPUT_UL_BAK
	if(flag) OutputBit(_OUTPUT_UL_FOR, _OFF);
	return OutputBit(_OUTPUT_UL_BAK, flag);
#endif
	return TRUE;
}
//===================================================================================
BOOL CComizoa_Class::BLOW_OnOff(int flag)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }
	if(!COMI_Initial) return FALSE; 
	
#ifdef _OUTPUT_BLOW
	return OutputBit(_OUTPUT_BLOW, flag);
#endif
	return TRUE;
}


//===================================================================================
BOOL CComizoa_Class::RollerMotionHomeCheck() // TRUE: ���� ����
//-----------------------------------------------------------------------------------
{
// 	double pos= pSystem.GetMotionComponent()->Get_Position(_AXIS_ROLLER);
// 
// 	if(pos <= ModelFile()->Model_data[_RUN_MODE1].dPosition_Start[_AXIS_ROLLER]) return FALSE;
	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::PrintMoveStart()
//-----------------------------------------------------------------------------------
{
	double len= 0.0;

//	len= ModelFile()->Model_data[_RUN_MODE1].dWork_Length + ModelFile()->Model_data[_RUN_MODE1].dPosition_Start[_AXIS_ROLLER];

//	ABS_Move_VelControl(_RUN_MODE1, _AXIS_ROLLER, len, FALSE, FALSE);

	return FALSE;
}
//===================================================================================
BOOL CComizoa_Class::ABS_Move_VelControl(int mode, long axis, double dist, BOOL EndDoneCheck, int SyncMode)
//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { Sleep(10); return TRUE; }

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
		return FALSE;
	}
// 	if(!isMotion_AlarmCheck()) return FALSE; 
// 
// 
// 	CString strLog; strLog.Format("[MOTOR] ABS_Move_VelControl(%d, %f, %d, %d)", axis, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
// 	if(!isMotion_AlarmCheck()) return FALSE; 
// 
// 	MotionSpeedCheck(mode, ModelFile()->Model_data[mode].dPrint_Speed) ;
// 
// 	if(axis== _AXIS_STAGE_X)
// 		cmmCfgSetSpeedPattern(axis, cmSMODE_S, dOutPulse_X[mode]*ModelFile()->Motion_data.dPulsePerMM[axis],
// 		ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
// 	if(axis== _AXIS_ROLLER)
// 		cmmCfgSetSpeedPattern(axis, cmSMODE_S, dOutPulse_T[mode]*ModelFile()->Motion_data.dPulsePerMM[axis],
// 		ModelFile()->Motion_data.dWork_Accel[axis], ModelFile()->Motion_data.dWork_Decel[axis]);
// 
// 	cmmSxSetSpeedRatio(axis, cmSMODE_S, 100.0, 100.0, 100.0);
// 
// 	double dOutPulse= 0.0;
// 	long lOutPulse= 0;
// 	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis]);
// 	dOutPulse= (double)(lOutPulse);
// 
// 	if(EndDoneCheck) {
// 		if(cmmSxMoveTo(axis, dOutPulse, TRUE) != cmERR_NONE){  
// 			CString strTTT; strTTT.Format("%d : ABS_Move_VelControl() cmmSxMoveTo-2", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
// 			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
// 			return FALSE;
// 		}
// 	}
// 	else {
// 		if(cmmSxMoveToStart(axis, dOutPulse) != cmERR_NONE){  
// 			CString strTTT; strTTT.Format("%d : ABS_Move_VelControl() cmmSxMoveToStart-3", axis);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
// 			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
// 			return FALSE;
// 		}
// 	}
	return TRUE;
}


//===================================================================================
BOOL CComizoa_Class::JogStart_Z(long axis1, long axis2, long dir, int spd)
	//-----------------------------------------------------------------------------------
{
	if(!COMI_Initial) return FALSE; 

	double SetSpeed= 0.0;
	double SetSpeed1= 0.0;

	long nAxisListp[2] = {axis1, axis2};

	cmmSxOptSetIniSpeed(axis1, 0);
	cmmSxOptSetIniSpeed(axis2, 0);

	long nDirList[2] = {0 , 0};

	if (dir == _DIR_CW)
	{
		nDirList[0] = cmDIR_P;
		nDirList[1] = cmDIR_P;

	} 
	else
	{
		nDirList[0] = cmDIR_N;
		nDirList[1] = cmDIR_N;
	}

	//	cmmSxOptSetIniSpeed(axis, ModelFile()->Motion_data.dStart_Speed[axis]);
	switch(spd) {
	case _MAX_SPEED:
		SetSpeed= ModelFile()->Motion_data.dJog_MaxSpeed[axis1];
		SetSpeed1= ModelFile()->Motion_data.dJog_MaxSpeed[axis2];
		break;
	case _MID_SPEED: 
		SetSpeed= ModelFile()->Motion_data.dJog_MidSpeed[axis1];
		SetSpeed1= ModelFile()->Motion_data.dJog_MidSpeed[axis2];
		break;
	case _MIN_SPEED: 
		SetSpeed= ModelFile()->Motion_data.dJog_MinSpeed[axis1];
		SetSpeed1= ModelFile()->Motion_data.dJog_MinSpeed[axis2];
		break;
	default:
		return FALSE;
	}

	cmmCfgSetSpeedPattern(_AXIS_ROLL_Z1, cmSMODE_C, SetSpeed,
		SetSpeed * 10, SetSpeed *10);
	cmmCfgSetSpeedPattern(_AXIS_ROLL_Z2, cmSMODE_C, SetSpeed1,
		SetSpeed1 *10, SetSpeed1 *10);
	cmmSxSetSpeedRatio(axis1, cmSMODE_KEEP, 100.0, 100.0, 100.0);
	cmmSxSetSpeedRatio(axis2, cmSMODE_KEEP, 100.0, 100.0, 100.0);


	if(cmmMxVMoveStart(2, nAxisListp, nDirList) != cmERR_NONE ){ 
		CString strTTT; strTTT.Format("%d %d : JogStart() cmmMxVMoveStart", axis1, axis2);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
		cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
		return FALSE;
	}
	return TRUE;

}

//===================================================================================
BOOL CComizoa_Class::Jog_Z_Stop(long axis1, long axis2)
	//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 

	Move_Z1_Z2_Stop(axis1 , axis2);

	return TRUE;
}

//===================================================================================
BOOL CComizoa_Class::Move_Z1_Z2_Stop(long axis1, long axis2)
	//-----------------------------------------------------------------------------------
{
	//if(SIMULATION_MODE) { return TRUE; }

	if(!COMI_Initial) return FALSE; 
	
	cmmSxStop(axis1, FALSE, FALSE);
	cmmSxStop(axis2, FALSE, FALSE);

// 	long nAxisListp[2] = {axis1, axis2};
// 	
// 	cmmMxStop(2 , nAxisListp, FALSE, FALSE);
	CString strLog; strLog.Format("[MOTOR] MoveStop= %d, %d", axis1, axis2);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	return TRUE;
}


BOOL CComizoa_Class::Ix_Move_X_ROLL(long axis1, long axis2, double dist, BOOL EndDoneCheck, int SyncMode)
{
	if(!COMI_Initial) return FALSE;

	if(!isMotion_AlarmCheck()) return FALSE; 
	CString strLog; strLog.Format("[MOTOR] ABS_Move(%d, %f, %d, %d)", axis1, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);

	//���� �� ����
	cmmIxMapAxes(_Ix_ComiMAP_, cmX1_MASK | cmY1_MASK, 0);

	if (SyncMode == VSTAGE_MODE) // ù��° �������� 
	{
		ModelFile()->Motion_data.dSync_Speed[axis1] = (ModelFile()->Model_data.dStage_X_Vel * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
		ModelFile()->Motion_data.dSync_Speed[axis2] = (ModelFile()->Model_data.dStage_X_Vel * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
	}
	else
	{
		ModelFile()->Motion_data.dSync_Speed[axis1] = (ModelFile()->Model_data.dH_Stage_Vel * ModelFile()->Motion_data.dPulsePerMM[_AXIS_STAGE_X]);
		ModelFile()->Motion_data.dSync_Speed[axis2] = (ModelFile()->Model_data.dH_Stage_Vel * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLLER]);
	}

	//���� �̵� �ӵ� ����
	cmmCfgSetSpeedPattern(_AXIS_STAGE_X, cmSMODE_C, ModelFile()->Motion_data.dSync_Speed[axis1],
		ModelFile()->Motion_data.dSync_Speed[axis1]*10, ModelFile()->Motion_data.dSync_Speed[axis1]*10);
	cmmCfgSetSpeedPattern(_AXIS_ROLLER, cmSMODE_C, ModelFile()->Motion_data.dSync_Speed[axis2],
		ModelFile()->Motion_data.dSync_Speed[axis2]*10, ModelFile()->Motion_data.dSync_Speed[axis2]*10);

	
	double dOutPulse= 0.0;
	double dOutPulse1= 0.0;

	long lOutPulse= 0;
	long lOutPulse1= 0;
	
	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis1]);
	lOutPulse1= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis2]);

	dOutPulse= (double)(lOutPulse);
	dOutPulse1= (double)(lOutPulse1);


	double fPos[2] = {dOutPulse, dOutPulse1};

	cmmIxSetSpeedPattern(_Ix_ComiMAP_, cmFALSE, cmSMODE_T, 100, 70, 70);


	if(EndDoneCheck) {
		if(cmmIxLine(_Ix_ComiMAP_, fPos, cmFALSE) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d %d: IxLine_Move() cmmIxLine-2", axis1, axis2);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	else {
		if(cmmIxLineStart(_Ix_ComiMAP_, fPos) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d %d: IxLine_Move() cmmIxLineStart-3", axis1, axis2);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	return TRUE;

}




//===================================================================================
BOOL CComizoa_Class::ABS_Move_Z(long axis1, long axis2, double dist, BOOL EndDoneCheck, int SyncMode)
	//===================================================================================
{

	if(!COMI_Initial) return FALSE;
	if(axis1== _AXIS_ROLL_Z1 || axis2== _AXIS_ROLL_Z2 || axis1== _AXIS_Z_SYNC) {
		if(dist >= _Z_AXIS_MAX_ || dist< _Z_AXIS_MIN_) {
			CString strAAA;
			strAAA.Format("[_AXIS_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n�̵� ���� �Ÿ� �ʰ� �Է�!\n[ %f ]", dist);
			Log()->SaveLogFile(_MOTION_LOG, "[_AXIS_Z (ABS_Move)] �̵� �Ÿ� �Է��� ������ �մϴ�!!! �̵� ���� �Ÿ� �ʰ� �Է�!");
			AfxMessageBox(strAAA);
		}
	}

	if(!isMotion_AlarmCheck()) return FALSE; 
	CString strLog; strLog.Format("[MOTOR] ABS_Move(%d, %f, %d, %d)", axis1, dist, EndDoneCheck, SyncMode);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);
	

	double dOutPulse= 0.0;
	long lOutPulse= 0;
	lOutPulse= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis1]);
	dOutPulse= (double)(lOutPulse);

	double dOutPulse1= 0.0;
	long lOutPulse1= 0;
	lOutPulse1= (long)(dist * ModelFile()->Motion_data.dPulsePerMM[axis2]);
	dOutPulse1= (double)(lOutPulse1);

	long nAxisList[2]={axis1, axis2};
	double fPos[2] = {dOutPulse, dOutPulse1};

	double SetSpeed= 0.0;
	double SetSpeed1= 0.0;

	SetSpeed = ModelFile()->Motion_data.dWork_Speed[_AXIS_ROLL_Z1];
	SetSpeed1 = ModelFile()->Motion_data.dWork_Speed[_AXIS_ROLL_Z2];


	cmmCfgSetSpeedPattern(_AXIS_ROLL_Z1, cmSMODE_C, SetSpeed,
		SetSpeed * 10, SetSpeed *10);
	cmmCfgSetSpeedPattern(_AXIS_ROLL_Z2, cmSMODE_C, SetSpeed1,
		SetSpeed1 *10, SetSpeed1 *10);
	cmmSxSetSpeedRatio(axis1, cmSMODE_S, 100.0, 100.0, 100.0);
	cmmSxSetSpeedRatio(axis2, cmSMODE_S, 100.0, 100.0, 100.0);

	
	if(EndDoneCheck) {
		if(cmmMxMoveTo(2, nAxisList, fPos, TRUE) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d %d: ABS_Move() cmmMxMoveTo-2", axis1, axis2);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	else {
		if(cmmMxMoveToStart(2, nAxisList, fPos) != cmERR_NONE){  
			CString strTTT; strTTT.Format("%d %d: ABS_Move() cmmMxMoveToStart-3", axis1, axis2);	Log()->SaveLogFile(_MOTION_LOG, strTTT);
			cmmErrShowLast(NULL); // ���� �߻��Ͽ����� ���� ���÷���.
			return FALSE;
		}
	}
	return TRUE;


}

BOOL CComizoa_Class::Z_Offset_Pos_Move(long axis, double dist, BOOL EndDoneCheck)
{
	if(!COMI_Initial) return FALSE; 
	BOOL ret= TRUE;
	if(axis== _AXIS_ROLL_Z1 || axis== _AXIS_ROLL_Z2 || axis== _AXIS_Z_SYNC)
	{

		double Z1_pos= Get_Position(_AXIS_ROLL_Z1);
		double Z2_pos= Get_Position(_AXIS_ROLL_Z2);

		if(dist >= ModelFile()->System_data.dZAxisLimit) {
			Log()->SaveLogFile(_MOTION_LOG, "[Z_Offset_Pos_Move] _AXIS_Z_SYNC �̵� �Ÿ� �Է��� ������ �մϴ�!!! ���ͷ� ����ġ�� �ʰ�!!!");
			AfxMessageBox("[Z_Offset_Pos_Move] �̵� �Ÿ� �Է��� ������ �մϴ�!!!\n���ͷ� ����ġ�� �ʰ�!!!"); return FALSE;
		}
		CString strLog; strLog.Format("[MOTOR] Z_Offset_Pos_Move(Z1= %f Z2= %f [Offset=%f])", 
			Z1_pos, Z2_pos, dist);	Log()->SaveLogFile(_SYSTEM_LOG, strLog);


		double dOutPulse= 0.0;
		long lOutPulse= 0;
		lOutPulse= (long)((dist + Z1_pos) * ModelFile()->Motion_data.dPulsePerMM[_AXIS_ROLL_Z2]);
		dOutPulse= (double)(dist + Z1_pos);

		double SetSpeed= 0.0;

		SetSpeed = ModelFile()->Motion_data.dWork_Speed[_AXIS_ROLL_Z2];


		cmmCfgSetSpeedPattern(_AXIS_ROLL_Z1, cmSMODE_C, SetSpeed,
			SetSpeed * 10, SetSpeed *10);

		if(EndDoneCheck) ret= ABS_Move(_AXIS_ROLL_Z2, dOutPulse, true);
		else			 ret= ABS_Move(_AXIS_ROLL_Z2, dOutPulse, false);

	}

	return ret;
}

