// SystemClass.cpp: implementation of the CSystemClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemClass.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//===================================================================================
CSystemClass::CSystemClass()
//-----------------------------------------------------------------------------------
{
	m_pParameterFile=	NULL;
	m_pMotion=			NULL;
	m_pIO=				NULL;
	m_pProcessThread_Main= NULL;
	m_pVision=			NULL;
/*
	m_pProcessThread_RollWinder= NULL;
*/

}
//===================================================================================
CSystemClass::~CSystemClass()
//-----------------------------------------------------------------------------------
{
	if(m_pParameterFile)		{	delete m_pParameterFile;	m_pParameterFile = NULL;	}
	if(m_pIO)					{	delete m_pIO;				m_pIO = NULL;	}
	if(m_pMotion)				{	delete m_pMotion;			m_pMotion = NULL;	}
	if(m_pVision)				{	delete m_pVision;			m_pVision = NULL;	}
}
//===================================================================================
int CSystemClass::Initialize()
//-----------------------------------------------------------------------------------
{
	bool flag= true;
//	PROCESS_INFORMATION pi; 
	STARTUPINFO sui; 
	
	ZeroMemory(&sui, sizeof(STARTUPINFO));
//	CreateProcess("D:\\FileDeleteLog\\TrayFileDelete.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sui, &pi); 

	//--------------------------------------------------------
	if(createParameterFileComponent()!= ERR_SYSTEM_SUCCESS)	{
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[System-Initialize] ..."));
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createParameterFileComponent => ERR_SYSTEM_CANNOT_FIND_OBJECT"));
		flag= false;
	}
	else 	{
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[System-Initialize] ..."));
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createParameterFileComponent => SYSTEM_SUCCESS"));
	}
	//--------------------------------------------------------
	if(createMotionComponent()!= ERR_SYSTEM_SUCCESS)	{
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createMotionComponent => ERR_SYSTEM_CANNOT_FIND_OBJECT"));
		flag= false;
	}
	else 	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createMotionComponent => SYSTEM_SUCCESS"));
	//--------------------------------------------------------
	if(createIOComponent()!= ERR_SYSTEM_SUCCESS)	{
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createIOComponent => ERR_SYSTEM_CANNOT_FIND_OBJECT"));
		flag= false;
	}
	else 	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createIOComponent => SYSTEM_SUCCESS"));
	//--------------------------------------------------------
	if(createVisionComponent()!= ERR_SYSTEM_SUCCESS)	{
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createVisionComponent => ERR_SYSTEM_CANNOT_FIND_OBJECT"));
		flag= false;
	}
	else 	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createVisionComponent => SYSTEM_SUCCESS"));
	//--------------------------------------------------------
/*
	if(!createProcessThread_RollWinderComponent()) {
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createProcessThread_RollWinderComponent => ERR_SYSTEM_CANNOT_FIND"));
		flag= false;
	}
	else 	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createProcessThread_RollWinderComponent => SYSTEM_SUCCESS"));
*/
	//--------------------------------------------------------
	if(!createProcessThread_MainComponent()) {
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createProcessThread_MainComponent => ERR_SYSTEM_CANNOT_FIND"));
		flag= false;
	}
	else 	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createProcessThread_MainComponent => SYSTEM_SUCCESS"));

	//--------------------------------------------------------
	if(flag) {
		GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createComponent_ALL => SYSTEM_SUCCESS"));
		return ERR_SYSTEM_SUCCESS;
	}
	else 	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[Initialize] createComponent_ALL => ERR_SYSTEM_CANNOT_FIND_OBJECT"));
	//--------------------------------------------------------

	return ERR_SYSTEM_CANNOT_FIND_OBJECT;
}
//===================================================================================
BOOL CSystemClass::IO_SystemInitial()
//-----------------------------------------------------------------------------------
{	// 일반 입출력 기기 오픈.  Control과 연결되는 것은 하면 안됨. [폼 생성전에 실행]
	BOOL flag= true;
	BOOL flag1= true;
	flag= MotionSystemInitial();
	flag1= IOSystemInitial();
	
	GetProcessThread_MainComponent()->ProcessStart();

	if(flag && flag1) return TRUE;
	return FALSE;
}
//===================================================================================
void CSystemClass::SystemClose() // 모든 시스템 유닛 Close
//-----------------------------------------------------------------------------------
{
	GetProcessThread_MainComponent()->ProcessStop();

	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[System-Close] ..."));
	VisionSystemClose();
	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[IO_SystemClose] VisionSystemClose!"));
	MotionSystemClose();
	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[IO_SystemClose] MotionSystemClose!"));
	IOSystemClose();
	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, _T("[IO_SystemClose] IOSystemClose!"));
}
//===================================================================================
int	CSystemClass::createParameterFileComponent()
//-----------------------------------------------------------------------------------
{
	m_pParameterFile = new CParameterFile;
	m_pParameterFile->SaveLogFile(_SYSTEM_LOG, _T("========================================================================"));
	return ERR_SYSTEM_SUCCESS;
}
//===================================================================================
CParameterFile* CSystemClass::GetParameterFileComponent()
//-----------------------------------------------------------------------------------
{
	return m_pParameterFile;
}

//===================================================================================
BOOL CSystemClass::IOSystemInitial()
//-----------------------------------------------------------------------------------
{
	if(m_pIO) {
		if(m_bMotionIsInitial) {	m_bIOIsInitial= TRUE; }
		else						m_bIOIsInitial= FALSE;
	}
	else	m_bIOIsInitial= FALSE;
	
	return	m_bIOIsInitial;
}
//===================================================================================
void CSystemClass::IOSystemClose()
//-----------------------------------------------------------------------------------
{
	m_bIOIsInitial= FALSE;
}
//===================================================================================
int	CSystemClass::createIOComponent()
//-----------------------------------------------------------------------------------
{
	m_pIO= NULL;
	m_pIO = new CIO;
	m_bIOIsInitial= FALSE;
	return ERR_SYSTEM_SUCCESS;
}
//===================================================================================
CIO* CSystemClass::GetIO()
//-----------------------------------------------------------------------------------
{
	return m_pIO;
}
//===================================================================================
BOOL CSystemClass::IsIOSystemInitial()
//-----------------------------------------------------------------------------------
{
	return m_bIOIsInitial;
}

//===================================================================================
BOOL CSystemClass::MotionSystemInitial()
//-----------------------------------------------------------------------------------
{
	if(m_pMotion) {
		if(m_pMotion->Open()) {	m_bMotionIsInitial= TRUE; }
		else							m_bMotionIsInitial= FALSE;
	}
	else	m_bMotionIsInitial= FALSE;
	
	return	m_bMotionIsInitial;
}
//===================================================================================
void CSystemClass::MotionSystemClose()
//-----------------------------------------------------------------------------------
{
	if(m_pMotion) { 
		m_pMotion->Close();	m_bMotionIsInitial= FALSE;
	}
}
//===================================================================================
int	CSystemClass::createMotionComponent()
//-----------------------------------------------------------------------------------
{
	m_pMotion= NULL;
	m_pMotion = new CMotion;
	m_bMotionIsInitial= FALSE;
	return ERR_SYSTEM_SUCCESS;
}
//===================================================================================
CMotion* CSystemClass::GetMotionComponent()
//-----------------------------------------------------------------------------------
{
	return m_pMotion;
}
//===================================================================================
BOOL CSystemClass::IsMotionSystemInitial()
//-----------------------------------------------------------------------------------
{
	return m_bMotionIsInitial;
}

//=================================================================================
int	CSystemClass::createVisionComponent()
	//-----------------------------------------------------------------------------------
{
	m_pVision = new CMatrox;
	m_bVisionIsInitial= FALSE;
	return ERR_SYSTEM_SUCCESS;
}
//=================================================================================
CMatrox* CSystemClass::GetVisionComponent()
	//-----------------------------------------------------------------------------------
{ 
	return m_pVision;
}
//=================================================================================
BOOL CSystemClass::VisionSystemInitial()
	//-----------------------------------------------------------------------------------
{
	if(m_pVision) {
		if(m_pVision->Open()) {	m_bVisionIsInitial= TRUE; }
		else							m_bVisionIsInitial= FALSE;
	}
	else	m_bVisionIsInitial= FALSE;
	return m_bVisionIsInitial;
}
//=================================================================================
void CSystemClass::VisionSystemClose()
	//-----------------------------------------------------------------------------------
{
	if(m_bVisionIsInitial) {
		m_pVision->Close();	m_bVisionIsInitial= FALSE;
	}
}
//=================================================================================
BOOL CSystemClass::IsVisionSystemInitial()
	//-----------------------------------------------------------------------------------
{
	return m_bVisionIsInitial;
}

//===================================================================================
int	CSystemClass::createProcessThread_MainComponent()
//-----------------------------------------------------------------------------------
{
	m_pProcessThread_Main= new CProcessThread_Main;
	return ERR_SYSTEM_SUCCESS;
}
//===================================================================================
CProcessThread_Main*	CSystemClass::GetProcessThread_MainComponent()
//-----------------------------------------------------------------------------------
{
	return m_pProcessThread_Main;
}
//===================================================================================
void CSystemClass::RunModeChange()
//-----------------------------------------------------------------------------------
{
}
//===================================================================================
void CSystemClass::AutoRunErrorProc(CString strRet, int step)
//-----------------------------------------------------------------------------------
{
	CString strText;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 


	strText.Format(_T("[AutoRunErrorProc] %s : [STEP: %d]"), strRet, step);

//	pFrm->GetWindowHandler_DebugView()->ListPrint(strRet);	pFrm->GetWindowHandler_DebugView()->ListPrint(strText);

	GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, strText);

	GetParameterFileComponent()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_ERROR;

	pFrm->GetWindowHandler_MainView()->StopProcess();

	GetMotionComponent()->Z_ControlDisable();
	GetMotionComponent()->MotorAll_EMG_Stop();
	GetMotionComponent()->MotorAllStop();
	GetMotionComponent()->Sync_Stop();
	
	strText.Format(_T("[AutoRunErrorProc] %s \n [STEP: %d]"), strRet, step);
	MyMessageBox(strText.GetBuffer(0), _T("자동 동작 에러"), M_ICONERROR);
}
//===================================================================================
void CSystemClass::AutoRunEndProc()
//-----------------------------------------------------------------------------------
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
//	pFrm->GetWindowHandler_DebugView()->ListPrint("Auto-End.....");
//	int iCURRENT_RUN_STATUS;
//#define _CURRENT_RUN_STOP		0
//#define _CURRENT_RUN_RUN		1
//#define _CURRENT_RUN_PAUSE		2
//#define _CURRENT_RUN_ERROR		9


	GetMotionComponent()->Z_ControlDisable();

	GetParameterFileComponent()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_STOP;
	
	pFrm->GetWindowHandler_MainView()->StopProcess();
	GetMotionComponent()->Sync_Stop();

}