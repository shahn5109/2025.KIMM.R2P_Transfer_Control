// Matrox.cpp: implementation of the CMatrox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrox.h"
#include <direct.h>	// Dos Command. //
#include "stdio.h"
#include <math.h>
#include "Mainfrm.h"
#include "xImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction 
//////////////////////////////////////////////////////////////////////
extern CSystemClass pSystem;
CParameterFile* CMatrox::ModelFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

double  EdgeDrawColor = CONTOUR_DRAW_COLOR,     /* Edge draw color.          */
LabelDrawColor = CONTOUR_LABEL_COLOR;   /* Text draw color.          */
int     NumEdgeFound  = 0L,                     /* Number of edges found.    */
NumResults    = 0L;                     /* Number of results found.  */   

//===================================================================================
CMatrox::CMatrox()
//-----------------------------------------------------------------------------------
{
	int i;
	for(i= 0 ; i< 4; i++)	{		m_bExec[i] = FALSE; m_pThread[i] = NULL;	}

	strcpy(strDirRootPath,		_PROGRAM_ROOT_DIR);
	
	sprintf(strDirConfigPath,	"%s\\Config",		strDirRootPath);
	sprintf(strDirCalibrationImagePath,	"%s\\Config\\Calib_Image",		strDirRootPath);
	sprintf(strDirRecipePath,	"%s\\Recipe",		strDirRootPath);
	sprintf(strDirProgramPath,	"%s\\Bin",			strDirRootPath);
	sprintf(strDirSystemLogPath,"%s\\Log_Process",	strDirRootPath);
	sprintf(strDirVisionLogName,"%s\\Log_Vision",	strDirRootPath);
	sprintf(strDirDefName,		"%s\\Report",		strDirRootPath);
	sprintf(strDirImagePath,	"%s\\Image",		strDirDefName);//strDirMeasImageSavePath
	sprintf(strDirDebugImage,	"%s\\Debug_Image",	strDirRootPath);
	
	_mkdir(strDirRootPath); 
	_mkdir(strDirConfigPath); 
	_mkdir(strDirRecipePath);
	_mkdir(strDirSystemLogPath);
	_mkdir(strDirImagePath);
	_mkdir(strDirDefName);
	_mkdir(strDirVisionLogName);
	_mkdir(strDirDebugImage);

	for(i= 0; i< _MAX_VIEW_; i++) {
		MilDisplayControl[i]= M_NULL;
		MilOverlayImage[i]= M_NULL;
		MilDigitizer[i]= M_NULL;
		MilDisplayImage[i]= M_NULL;
		MilGrabImage[i]= M_NULL;
		Insp_ColorImage[i]= M_NULL;
	}
	for(i= 0; i< _MAX_TMPVIEW_; i++) {
		Insp_Tmp_MilImage[i]= M_NULL;
	}

	TransparentColor= NULL;        // Keying color
	MilMeasureMarker = NULL;
	MilGraphicContext= NULL;

//	ModelSetDisplay= M_NULL;
//	ModelSetDispImage= M_NULL;
//	MilSetOverlayImage= M_NULL;

//	ModelDontCareImage= M_NULL;

	PatternSetDisplay_1= M_NULL;
	PatternSetDispImage_1= M_NULL;
	PatternDontCareImage_1= M_NULL;
	PatternSetOverlayImage_1= M_NULL;
	for( i= 0; i< _MAX_FIND_MODEL; i++) {
		MilPatternModel_1[i]= M_NULL;
		MilPatternResult_1[i]= M_NULL;
	}
	PatternSetDisplay_2= M_NULL;
	PatternSetDispImage_2= M_NULL;
	PatternDontCareImage_2= M_NULL;
	PatternSetOverlayImage_2= M_NULL;
	for( i= 0; i< _MAX_FIND_MODEL; i++) {
		MilPatternModel_2[i]= M_NULL;
		MilPatternResult_2[i]= M_NULL;
	}
	
	ZoomCamDispDisplay_1= M_NULL;
	ZoomCamDispImage_1= M_NULL;
	NormalCamDispDisplay_1= M_NULL;
	NormalCamDispImage_1= M_NULL;
	ZoomCamDispDisplay_2= M_NULL;
	ZoomCamDispImage_2= M_NULL;
	NormalCamDispDisplay_2= M_NULL;
	NormalCamDispImage_2= M_NULL;

	DimBlobResult= NULL;
	DimFeatureList= NULL;
	DimBlobResult2= NULL;
	DimFeatureList2= NULL;

/*	for( i= 0; i< _MAX_FIND_MODEL; i++) {
		MilSearchContext[i]= M_NULL;
		MilModelResult[i]= M_NULL;
	}
*/
	Model_AllocSizeX= 10L;
	Model_AllocSizeY= 10L;
	Pattern_AllocSizeX= 10L;
	Pattern_AllocSizeY= 10L;

	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
	Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
	Vision_VGA_Initial_Flag= false;
	
//	strLogFileBuffer= _T("");
	iLogLevel= _LOG_DETAIL;
	VISION_SYS_MODE= _DEV_MODE;
	
	main_dcf_file.Format("%s\\%s", strDirConfigPath, _INSP_CAM_DCF);

	TestImageLoadCount_L_1= 0;
	TestImageLoadCount_R_1= 0;
	TestImageLoadCount_L_2= 0;
	TestImageLoadCount_R_2= 0;

}
//===================================================================================
CMatrox::~CMatrox()
//-----------------------------------------------------------------------------------
{

}

//============================================================================================
void CMatrox::RunThread(int nID)
//-----------------------------------------------------------------------------------------
{
/*	m_bExec[nID] = TRUE; 
	THREADPAPAMS *pThreadParams = new THREADPAPAMS;
	pThreadParams->pContinue = &m_bExec[nID];
	pThreadParams->pWnd =  ((CMatrox*)this);
	if(m_pThread[nID] == NULL) {
		switch(nID) {
		case 0: 
			m_pThread[nID] = AfxBeginThread(ThreadFuncLiveProc_0, pThreadParams);  break;
		case 1: 
			m_pThread[nID] = AfxBeginThread(ThreadFuncLiveProc_1, pThreadParams);  break;
		}
	}
	m_pThread[nID]->m_bAutoDelete = true;
*/
}
//--------------------------------------------------------------------------------------------
void CMatrox::KillThread(int nID)
//-----------------------------------------------------------------------------------------
{
/*
	if(m_pThread[nID] != NULL)	{
		HANDLE hThread = m_pThread[nID]->m_hThread;
		m_bExec[nID] = FALSE;
		::WaitForSingleObject(hThread,1000);
		TerminateThread(m_pThread[nID]->m_hThread, 0);
		m_pThread[nID] = NULL;
	}
*/
}
//============================================================================================
UINT ThreadFuncLiveProc_0(LPVOID pParam)
//-----------------------------------------------------------------------------------------
{
	// 넘어온 인자를 복사
	THREADPAPAMS *pThreadParams = (THREADPAPAMS *)pParam;
	CMatrox *pWnd = pThreadParams->pWnd;//THREADPAPAMS_Mag1;
	BOOL *pContinue = pThreadParams->pContinue;
	delete pThreadParams;
	
	CMatrox* pOwner =  (CMatrox*)pWnd ;
	
	while(*pContinue)
	{
		if(PROGRAM_EXIT) { Sleep(10); continue; }
/*		if(pOwner->Vision_Initial_Flag && pOwner->VISION_SYS_MODE== _DEV_MODE) {
			if(pOwner->Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) {
				if(pOwner->MilDigitizer[_INSP_CAM_LEFT]  && pOwner->MilGrabImage[_INSP_CAM_LEFT]) {
					pOwner->InspLiveUseGrab(_INSP_CAM_LEFT);
					;
				}
			}
		}
*/
		Sleep(100);
	}
	return 0;
}
//============================================================================================
UINT ThreadFuncLiveProc_1(LPVOID pParam)
//-----------------------------------------------------------------------------------------
{
	// 넘어온 인자를 복사
	THREADPAPAMS *pThreadParams = (THREADPAPAMS *)pParam;
	CMatrox *pWnd = pThreadParams->pWnd;//THREADPAPAMS_Mag1;
	BOOL *pContinue = pThreadParams->pContinue;
	delete pThreadParams;
	
	CMatrox* pOwner =  (CMatrox*)pWnd ;
	
	while(*pContinue)
	{
		if(PROGRAM_EXIT) { Sleep(10); continue; }
/*		if(pOwner->Vision_Initial_Flag && pOwner->VISION_SYS_MODE== _DEV_MODE) {
			if(pOwner->Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) {
				if(pOwner->MilDigitizer[_INSP_CAM_RIGHT] && pOwner->MilGrabImage[_INSP_CAM_RIGHT]) {
					pOwner->InspLiveUseGrab(_INSP_CAM_RIGHT);
					;
				}
			}
		}
*/
		Sleep(100);
	}
	return 0;
}
//===================================================================================
void CMatrox::Debug_Flag_Set(int LogLevel)
//-----------------------------------------------------------------------------------
{
	iLogLevel= LogLevel;
}
//===================================================================================
void CMatrox::Set_SysMode(int sys_mode)
//-----------------------------------------------------------------------------------
{
	CString strLogFileBuffer;
	if(sys_mode== 1) VISION_SYS_MODE= _DEV_MODE;
	else			 VISION_SYS_MODE= _VGA_MODE;
	if(!VISION_SYS_MODE) { 
		strLogFileBuffer.Format("VGA-MODE!"); SaveLogFile(strLogFileBuffer);
	}
	else {
		strLogFileBuffer.Format("DEV_MODE!"); SaveLogFile(strLogFileBuffer);
	}
}
//===================================================================================
int CMatrox::is_SysMode()
//-----------------------------------------------------------------------------------
{
	return VISION_SYS_MODE;
}

//===================================================================================
void CMatrox::SaveLogFile(CString strText)
//-----------------------------------------------------------------------------------
{
	char value[512];
	CString strFile__Name;
	SYSTEMTIME st; 
	GetLocalTime(&st); 
	
	CString m_strDayLogDirName;
	m_strDayLogDirName.Format("%s\\%4d%02d%02d", strDirVisionLogName, st.wYear, st.wMonth, st.wDay); // File Name 생성
	_mkdir(m_strDayLogDirName); // Day Loge Directory 생성. //
	
	strFile__Name.Format("%s\\Vision.Log", m_strDayLogDirName);
	sprintf(value, "%s", strText);
	
	FILE* fout = fopen(strFile__Name.GetBuffer(0), "a+");   // 파일에 계속 추가시킨다. //
	if(fout == NULL) 	{		return;		}
	CString strMsg;
	strMsg.Format ("[%02ld:%02ld:%02ld]\t%s", st.wHour, st.wMinute, st.wSecond, value); fprintf(fout, "%s\n", strMsg);
	fclose(fout);
}
//===================================================================================
bool CMatrox::Open()
//-----------------------------------------------------------------------------------
{
	int i= 0, j= 0;
	CString strLogFileBuffer;
	CString strDcfFileName;
	
	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
	Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// Allocate a MIL application. 
	strLogFileBuffer.Format("==================================================================");	SaveLogFile(strLogFileBuffer);
	strLogFileBuffer.Format("CMATROX-Class: Vision-Open Start!!!");		SaveLogFile(strLogFileBuffer);
	
	MappAlloc(M_DEFAULT, &MilApplication);

//	MappControl(M_ERROR, M_PRINT_DISABLE);	strLogFileBuffer.Format("M_ERROR-Display -> DISABLE!");		SaveLogFile(strLogFileBuffer);
	strLogFileBuffer.Format("M_ERROR-Display -> ENABLE!");		SaveLogFile(strLogFileBuffer);
	
	Vision_VGA_Initial_Flag= false;
	MilSystem = NULL; Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
	
	if(VISION_SYS_MODE== _DEV_MODE)	{   
//		MsysAlloc("M_DEFAULT", M_DEFAULT, /*M_DEFAULT*/M_NO_DDRAW, &MilSystem); 
		MsysAlloc(M_SYSTEM_SOLIOS, M_DEFAULT, M_DEFAULT, &MilSystem); 
//		MsysAlloc(M_SYSTEM_SOLIOS, M_DEFAULT, /*M_DEFAULT*/M_NO_DDRAW, &MilSystem); 
		if(MilSystem == NULL) VISION_SYS_MODE= _VGA_MODE;
	}
	if(VISION_SYS_MODE== _VGA_MODE)
		MsysAlloc(M_SYSTEM_VGA, M_DEFAULT, M_DEFAULT/*M_NO_DDRAW*/, &MilSystem);

	if(VISION_SYS_MODE== _VGA_MODE) {
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
		strLogFileBuffer.Format("MsysAlloc(M_SYSTEM_VGA,,,)");			SaveLogFile(strLogFileBuffer);
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
	}
	else {
		strLogFileBuffer.Format("MsysAlloc(M_SYSTEM_METEOR_II,,,)");	SaveLogFile(strLogFileBuffer);
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
	}
	
	if(MilApplication == NULL)	{ 
		strLogFileBuffer.Format("MilApplication Alloc Failed~!");	
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
		SaveLogFile(strLogFileBuffer); AfxMessageBox(strLogFileBuffer) ;	return false ; 
	}
	if(MilSystem == NULL)		{ 
		strLogFileBuffer.Format("System Alloc Failed~!");		
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
		SaveLogFile(strLogFileBuffer); AfxMessageBox(strLogFileBuffer) ;	return false ; 
	}
	
    MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

	if(VISION_SYS_MODE== _DEV_MODE ) {
		if(MsysInquire(MilSystem, M_DIGITIZER_NUM, M_NULL) ) {
			if(Vision_Initial_Flag== ERR_SYSTEM_INITIAL_OK)	{
				//--------------------------------------
				strDcfFileName.Format("%s\\Config\\HR70a_Solios_CH1.dcf", _PROGRAM_ROOT_DIR);
				MdigAlloc(MilSystem, M_DEV0, strDcfFileName.GetBuffer(0), M_DEFAULT, &MilDigitizer[_INSP_2_F_CAM_PATTERN]);
				strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!", main_dcf_file.GetBuffer(0), _INSP_2_F_CAM_PATTERN);
				SaveLogFile(strLogFileBuffer);

				strDcfFileName.Format("%s\\Config\\HR70a_Solios_CH2.dcf", _PROGRAM_ROOT_DIR);
				MdigAlloc(MilSystem, M_DEV1, strDcfFileName.GetBuffer(0), M_DEFAULT, &MilDigitizer[_INSP_2_F_CAM_FILM]);
				strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!", main_dcf_file.GetBuffer(0), _INSP_2_F_CAM_FILM);
				SaveLogFile(strLogFileBuffer);
				//--------------------------------------
				strDcfFileName.Format("%s\\Config\\HR70a_Solios_CH3.dcf", _PROGRAM_ROOT_DIR);
				MdigAlloc(MilSystem, M_DEV2, strDcfFileName.GetBuffer(0), M_DEFAULT, &MilDigitizer[_INSP_2_R_CAM_ZOOM]);
				strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!", main_dcf_file.GetBuffer(0), _INSP_2_R_CAM_ZOOM);
				SaveLogFile(strLogFileBuffer);
				
				strDcfFileName.Format("%s\\Config\\HR70a_Solios_CH4.dcf", _PROGRAM_ROOT_DIR);
				MdigAlloc(MilSystem, M_DEV3, strDcfFileName.GetBuffer(0), M_DEFAULT, &MilDigitizer[_INSP_2_R_CAM_NORMAL]);
				strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!", main_dcf_file.GetBuffer(0), _INSP_2_R_CAM_NORMAL);
				SaveLogFile(strLogFileBuffer);
				//--------------------------------------
				Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
				//--------------------------------------
				if(MilDigitizer[_INSP_2_F_CAM_FILM]== NULL) {
					MilDigitizer[_INSP_2_F_CAM_FILM]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!", main_dcf_file.GetBuffer(0), _INSP_2_F_CAM_FILM);
					SaveLogFile(strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}
				if(MilDigitizer[_INSP_2_F_CAM_PATTERN]== NULL) {
					MilDigitizer[_INSP_2_F_CAM_PATTERN]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!", main_dcf_file.GetBuffer(0), _INSP_2_F_CAM_PATTERN);
					SaveLogFile(strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}

				if(MilDigitizer[_INSP_2_R_CAM_ZOOM]== NULL) {
					MilDigitizer[_INSP_2_R_CAM_ZOOM]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!", main_dcf_file.GetBuffer(0), _INSP_2_R_CAM_ZOOM);
					SaveLogFile(strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}
				if(MilDigitizer[_INSP_2_R_CAM_NORMAL]== NULL) {
					MilDigitizer[_INSP_2_R_CAM_NORMAL]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					strLogFileBuffer.Format("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!", main_dcf_file.GetBuffer(0), _INSP_2_R_CAM_NORMAL);
					SaveLogFile(strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}
				//--------------------------------------
				if(MilDigitizer[_INSP_2_F_CAM_FILM]!= NULL && MilDigitizer[_INSP_2_F_CAM_PATTERN]!= NULL &&
				   MilDigitizer[_INSP_2_R_CAM_ZOOM]!= NULL && MilDigitizer[_INSP_2_R_CAM_NORMAL]!= NULL) {
					MdigControl(MilDigitizer[_INSP_2_F_CAM_FILM], M_GRAB_TIMEOUT,1000);					 //mSec
					MdigControl(MilDigitizer[_INSP_2_F_CAM_PATTERN], M_GRAB_TIMEOUT,1000);					 //mSec
					MdigControl(MilDigitizer[_INSP_2_R_CAM_ZOOM], M_GRAB_TIMEOUT,1000);					 //mSec
					MdigControl(MilDigitizer[_INSP_2_R_CAM_NORMAL], M_GRAB_TIMEOUT,1000);					 //mSec
				}
				//--------------------------------------
			}
		}
		else
		{
			if(VISION_SYS_MODE== _DEV_MODE ) {
				strLogFileBuffer.Format("Digitizer를 찾을 수 없습니다. ~!");	SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
				for(i= 0; i< _MAX_CAM_; i++) 	MilDigitizer[i]= NULL;
				strLogFileBuffer.Format("VISION-Initialize ERROR!");			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
				Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				return true;
			}
		}
	}
/*
	for(i= 0; i< _MAX_FIND_MODEL; i++) {
//		MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilSearchContext[i]);	
		MmodAllocResult(MilSystem, M_DEFAULT, &MilModelResult[i]);         
	}
*/
	for(i= 0; i< _MAX_FIND_MODEL; i++) {
		MpatAllocResult(MilSystem, M_DEFAULT, &MilPatternResult_1[i]);         
		MpatAllocResult(MilSystem, M_DEFAULT, &MilPatternResult_2[i]);         
	}
	MblobAllocFeatureList(MilSystem, &DimFeatureList);
	MblobAllocFeatureList(MilSystem, &DimFeatureList2);
	MblobAllocResult(MilSystem, &DimBlobResult); 
	MblobAllocResult(MilSystem, &DimBlobResult2); 
	return true;
}
//===================================================================================
void CMatrox::Close()
//-----------------------------------------------------------------------------------
{
	int i;	
	CString strLogFileBuffer;
	
	if(Vision_Initial_Flag && VISION_SYS_MODE== _DEV_MODE)
	{
//		KillThread(0);
//		KillThread(1);
//		KillThread(2);
//		KillThread(3);
		MdigControl(MilDigitizer[0], M_GRAB_ABORT, M_DEFAULT);
		MdigControl(MilDigitizer[1], M_GRAB_ABORT, M_DEFAULT);
		MdigControl(MilDigitizer[2], M_GRAB_ABORT, M_DEFAULT);
		MdigControl(MilDigitizer[3], M_GRAB_ABORT, M_DEFAULT);
	}
	
	if(DimBlobResult)	{ MblobFree(DimBlobResult);		DimBlobResult= NULL;	}
	if(DimFeatureList)	{ MblobFree(DimFeatureList);	DimFeatureList= NULL;	}
	if(DimBlobResult2)	{ MblobFree(DimBlobResult2);		DimBlobResult2= NULL;	}
	if(DimFeatureList2)	{ MblobFree(DimFeatureList2);	DimFeatureList2= NULL;	}

	if(MilMeasureMarker)
		MmeasFree(MilMeasureMarker); MilMeasureMarker = NULL;

	m_pDC.Detach();		
	////////////////////////////////////////////////////////////////
//		for( i= 0; i< _MAX_FIND_MODEL; i++) {
//			if(MilSearchContext[i])		{ MmodFree(MilSearchContext[i]);						MilSearchContext[i]= 0; }
//			if(MilModelResult[i])		{ MmodFree(MilModelResult[i]);							MilModelResult[i]= 0; }
//		}
	////////////////////////////////////////////////////////////////
//	if(ModelSetDisplay)					MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_DISABLE);
//	if(ModelSetDisplay)					{ MdispDeselect(ModelSetDisplay,ModelSetDispImage); }
//	if(ModelSetDispImage)				{ MbufFree(ModelSetDispImage) ; ModelSetDispImage= 0; }
//	if(ModelSetDisplay)					{ MdispFree(ModelSetDisplay);	ModelSetDisplay= M_NULL; }
//	if(ModelDontCareImage)				{ MbufFree(ModelDontCareImage) ; ModelDontCareImage= 0; }
	////////////////////////////////////////////////////////////////

		for( i= 0; i< _MAX_FIND_MODEL; i++) {
			if(MilPatternModel_1[i])		{ MpatFree(MilPatternModel_1[i]);					MilPatternModel_1[i]= 0; }
			if(MilPatternResult_1[i])		{ MpatFree(MilPatternResult_1[i]);					MilPatternResult_1[i]= 0; }
		}
	if(PatternSetDisplay_1)					MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(PatternSetDisplay_1)					{ MdispDeselect(PatternSetDisplay_1,	PatternSetDispImage_1); }
	if(PatternSetDispImage_1)				{ MbufFree(PatternSetDispImage_1) ;		PatternSetDispImage_1= 0; }
	if(PatternDontCareImage_1)				{ MbufFree(PatternDontCareImage_1) ;	PatternDontCareImage_1= 0; }
	if(PatternSetDisplay_1)					{ MdispFree(PatternSetDisplay_1);		PatternSetDisplay_1= M_NULL; }

		for( i= 0; i< _MAX_FIND_MODEL; i++) {
			if(MilPatternModel_2[i])		{ MpatFree(MilPatternModel_2[i]);					MilPatternModel_2[i]= 0; }
			if(MilPatternResult_2[i])		{ MpatFree(MilPatternResult_2[i]);					MilPatternResult_2[i]= 0; }
		}
	if(PatternSetDisplay_2)					MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(PatternSetDisplay_2)					{ MdispDeselect(PatternSetDisplay_2,	PatternSetDispImage_2); }
	if(PatternSetDispImage_2)				{ MbufFree(PatternSetDispImage_2) ;		PatternSetDispImage_2= 0; }
	if(PatternDontCareImage_2)				{ MbufFree(PatternDontCareImage_2) ;	PatternDontCareImage_2= 0; }
	if(PatternSetDisplay_2)					{ MdispFree(PatternSetDisplay_2);		PatternSetDisplay_2= M_NULL; }
	////////////////////////////////////////////////////////////////

	if(ZoomCamDispDisplay_1)					MdispControl(ZoomCamDispDisplay_1, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(ZoomCamDispDisplay_1)					{ MdispDeselect(ZoomCamDispDisplay_1,ZoomCamDispImage_1); }
	if(ZoomCamDispImage_1)					{ MbufFree(ZoomCamDispImage_1) ; ZoomCamDispImage_1= 0; }
	if(ZoomCamDispDisplay_1)					{ MdispFree(ZoomCamDispDisplay_1);	ZoomCamDispDisplay_1= M_NULL; }

	if(NormalCamDispDisplay_1)				MdispControl(NormalCamDispDisplay_1, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(NormalCamDispDisplay_1)				{ MdispDeselect(NormalCamDispDisplay_1,NormalCamDispImage_1); }
	if(NormalCamDispImage_1)					{ MbufFree(NormalCamDispImage_1) ; NormalCamDispImage_1= 0; }
	if(NormalCamDispDisplay_1)				{ MdispFree(NormalCamDispDisplay_1);	NormalCamDispDisplay_1= M_NULL; }
	////////////////////////////////////////////////////////////////
	if(ZoomCamDispDisplay_2)					MdispControl(ZoomCamDispDisplay_2, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(ZoomCamDispDisplay_2)					{ MdispDeselect(ZoomCamDispDisplay_2,ZoomCamDispImage_2); }
	if(ZoomCamDispImage_2)					{ MbufFree(ZoomCamDispImage_2) ; ZoomCamDispImage_2= 0; }
	if(ZoomCamDispDisplay_2)					{ MdispFree(ZoomCamDispDisplay_2);	ZoomCamDispDisplay_2= M_NULL; }

	if(NormalCamDispDisplay_2)				MdispControl(NormalCamDispDisplay_2, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(NormalCamDispDisplay_2)				{ MdispDeselect(NormalCamDispDisplay_2,NormalCamDispImage_2); }
	if(NormalCamDispImage_2)					{ MbufFree(NormalCamDispImage_2) ; NormalCamDispImage_2= 0; }
	if(NormalCamDispDisplay_2)				{ MdispFree(NormalCamDispDisplay_2);	NormalCamDispDisplay_2= M_NULL; }
	////////////////////////////////////////////////////////////////


	for(i= 0; i< _MAX_TMPVIEW_; i++) {
		if(Insp_Tmp_MilImage[i]) {			MbufFree(Insp_Tmp_MilImage[i]);		Insp_Tmp_MilImage[i]= M_NULL; }
	}

	for(i= 0; i< _MAX_VIEW_; i++) 	{
		if(MilDisplayControl[i]!= M_NULL)	MdispControl(MilDisplayControl[i], M_WINDOW_OVR_WRITE, M_DISABLE);
		if(MilOverlayImage[i])	{ 
			if(m_pDC1[i])					MbufControl(MilOverlayImage[i], M_WINDOW_DC_FREE, M_DEFAULT);
			MilOverlayImage[i]= 0;
		}
	}
	
	for(i= 0; i< _MAX_VIEW_; i++)			m_pDC1[i].Detach();
	
	for(i= 0; i< _MAX_VIEW_; i++)	{
		if(MilDisplayControl[i])	{		MdispDeselect(MilDisplayControl[i],	MilDisplayImage[i]);	}
	}

	for(i= 0; i< _MAX_VIEW_; i++) 	{ 
		if(MilDisplayControl[i]) {			MdispFree(MilDisplayControl[i]);	MilDisplayControl[i]= M_NULL; }
		if(MilDisplayImage[i])	{			MbufFree(MilDisplayImage[i]);		MilDisplayImage[i]= M_NULL; }
		if(MilGrabImage[i])	{				MbufFree(MilGrabImage[i]);		MilGrabImage[i]= M_NULL; }
		if(Insp_ColorImage[i])	{			MbufFree(Insp_ColorImage[i]);		Insp_ColorImage[i]= M_NULL; }
	}

	for(i= 0; i< _MAX_CAM_; i++) {
		if(MilDigitizer[i])			{
			MdigControl(MilDigitizer[i], M_CAMERA_LOCK, M_DISABLE);		Sleep(50);
			MdigFree(MilDigitizer[i]);		MilDigitizer[i]= 0;
		}
	}

	if(MilSystem)			{				MsysFree(MilSystem);				MilSystem= M_NULL; }
	if(MilApplication)		{				MappFree(MilApplication);			MilApplication= M_NULL; }

	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
	strLogFileBuffer.Format("CMATROX-Class: VISION Close!"); SaveLogFile(strLogFileBuffer);
}
//===================================================================================
bool CMatrox::CreateMainWindow(HWND UserWindow1, HWND UserWindow2, HWND UserWindow3, HWND UserWindow4)
//-----------------------------------------------------------------------------------
{
	CString strLogFileBuffer;

	strLogFileBuffer.Format("CreateMainWindow(HWND UserWindow1)!"); SaveLogFile(strLogFileBuffer);
	int i;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate a MIL display Channel 0. 
	if (!MdispAlloc(MilSystem, M_DEFAULT, "M_DEFAULT", M_DEFAULT , &MilDisplayControl[_INSP_2_F_CAM_FILM]))
	{	strLogFileBuffer.Format("MdispAlloc_CH%d - Error!", _INSP_2_F_CAM_FILM); SaveLogFile(strLogFileBuffer);		return FALSE;	}
	if (!MdispAlloc(MilSystem, M_DEFAULT, "M_DEFAULT", M_DEFAULT , &MilDisplayControl[_INSP_2_F_CAM_PATTERN]))
	{	strLogFileBuffer.Format("MdispAlloc_CH%d - Error!", _INSP_2_F_CAM_PATTERN); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	//----------------
	if (!MdispAlloc(MilSystem, M_DEFAULT, "M_DEFAULT", M_DEFAULT , &MilDisplayControl[_INSP_2_R_CAM_ZOOM]))
	{	strLogFileBuffer.Format("MdispAlloc_CH%d - Error!", _INSP_2_R_CAM_ZOOM); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	if (!MdispAlloc(MilSystem, M_DEFAULT, "M_DEFAULT", M_DEFAULT , &MilDisplayControl[_INSP_2_R_CAM_NORMAL]))
	{	strLogFileBuffer.Format("MdispAlloc_CH%d - Error!", _INSP_2_R_CAM_NORMAL); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	//--------------------------------------------------------------
	if(MilDigitizer[_INSP_2_F_CAM_FILM]) {
		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[_INSP_2_F_CAM_FILM]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
		else		MdigHalt(MilDigitizer[_INSP_2_F_CAM_FILM]);
	}
	else {
		if(!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilGrabImage[_INSP_2_F_CAM_FILM]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	}
	if(MilDigitizer[_INSP_2_F_CAM_PATTERN]) {
		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[_INSP_2_F_CAM_PATTERN]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
		else		MdigHalt(MilDigitizer[_INSP_2_F_CAM_PATTERN]);
	}
	else {
		if(!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilGrabImage[_INSP_2_F_CAM_PATTERN]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	}
	//----------------
	if(MilDigitizer[_INSP_2_R_CAM_ZOOM]) {
		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[_INSP_2_R_CAM_ZOOM]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
		else		MdigHalt(MilDigitizer[_INSP_2_R_CAM_ZOOM]);
	}
	else {
		if(!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilGrabImage[_INSP_2_R_CAM_ZOOM]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	}
	if(MilDigitizer[_INSP_2_R_CAM_NORMAL]) {
		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[_INSP_2_R_CAM_NORMAL]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
		else		MdigHalt(MilDigitizer[_INSP_2_R_CAM_NORMAL]);
	}
	else {
		if(!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilGrabImage[_INSP_2_R_CAM_NORMAL]))
		{	strLogFileBuffer.Format("MbufAllocColor(MilGrabImageColor) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	}

	//--------------------------------------------------------------
	if (!MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilDisplayImage[_INSP_2_F_CAM_FILM]))
	{		strLogFileBuffer.Format("MbufAlloc2d(MilDisplayImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(MilDisplayImage[_INSP_2_F_CAM_FILM], 0);
	if (!MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilDisplayImage[_INSP_2_F_CAM_PATTERN]))
	{		strLogFileBuffer.Format("MbufAlloc2d(MilDisplayImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(MilDisplayImage[_INSP_2_F_CAM_PATTERN], 0);
	//----------------
	if (!MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilDisplayImage[_INSP_2_R_CAM_ZOOM]))
	{		strLogFileBuffer.Format("MbufAlloc2d(MilDisplayImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(MilDisplayImage[_INSP_2_R_CAM_ZOOM], 0);
	if (!MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilDisplayImage[_INSP_2_R_CAM_NORMAL]))
	{		strLogFileBuffer.Format("MbufAlloc2d(MilDisplayImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(MilDisplayImage[_INSP_2_R_CAM_NORMAL], 0);

	if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &Insp_ColorImage[_INSP_2_F_CAM_FILM]))
	{		strLogFileBuffer.Format("MbufAllocColor(Insp_ColorImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(Insp_ColorImage[_INSP_2_F_CAM_FILM], 0);
	if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &Insp_ColorImage[_INSP_2_F_CAM_PATTERN]))
	{		strLogFileBuffer.Format("MbufAllocColor(Insp_ColorImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(Insp_ColorImage[_INSP_2_F_CAM_PATTERN], 0);
	if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &Insp_ColorImage[_INSP_2_R_CAM_ZOOM]))
	{		strLogFileBuffer.Format("MbufAllocColor(Insp_ColorImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(Insp_ColorImage[_INSP_2_R_CAM_ZOOM], 0);
	if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &Insp_ColorImage[_INSP_2_R_CAM_NORMAL]))
	{		strLogFileBuffer.Format("MbufAllocColor(Insp_ColorImage) - Error!"); SaveLogFile(strLogFileBuffer);	return FALSE;	}
	else	MbufClear(Insp_ColorImage[_INSP_2_R_CAM_NORMAL], 0);

	//--------------------------------------------------------------
	MdispSelectWindow(MilDisplayControl[_INSP_2_F_CAM_FILM], MilDisplayImage[_INSP_2_F_CAM_FILM], UserWindow1);
	MdispSelectWindow(MilDisplayControl[_INSP_2_F_CAM_PATTERN], MilDisplayImage[_INSP_2_F_CAM_PATTERN], UserWindow2);
	//----------------
	MdispSelectWindow(MilDisplayControl[_INSP_2_R_CAM_ZOOM], MilDisplayImage[_INSP_2_R_CAM_ZOOM], UserWindow3);
	MdispSelectWindow(MilDisplayControl[_INSP_2_R_CAM_NORMAL], MilDisplayImage[_INSP_2_R_CAM_NORMAL], UserWindow4);
	//--------------------------------------------------------------
	MdispControl(MilDisplayControl[_INSP_2_F_CAM_FILM], M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_F_CAM_FILM], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[_INSP_2_F_CAM_FILM]);
	MdispControl(MilDisplayControl[_INSP_2_F_CAM_FILM], M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_F_CAM_FILM], M_KEY_COLOR, &TransparentColor); 
	MdispControl(MilDisplayControl[_INSP_2_F_CAM_PATTERN], M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_F_CAM_PATTERN], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[_INSP_2_F_CAM_PATTERN]);
	MdispControl(MilDisplayControl[_INSP_2_F_CAM_PATTERN], M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_F_CAM_PATTERN], M_KEY_COLOR, &TransparentColor); 
	//----------------
	MdispControl(MilDisplayControl[_INSP_2_R_CAM_ZOOM], M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_R_CAM_ZOOM], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[_INSP_2_R_CAM_ZOOM]);
	MdispControl(MilDisplayControl[_INSP_2_R_CAM_ZOOM], M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_R_CAM_ZOOM], M_KEY_COLOR, &TransparentColor); 
	MdispControl(MilDisplayControl[_INSP_2_R_CAM_NORMAL], M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_R_CAM_NORMAL], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[_INSP_2_R_CAM_NORMAL]);
	MdispControl(MilDisplayControl[_INSP_2_R_CAM_NORMAL], M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(MilDisplayControl[_INSP_2_R_CAM_NORMAL], M_KEY_COLOR, &TransparentColor); 
	//--------------------------------------------------------------
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 255));
	MdispControl(MilDisplayControl[_INSP_2_F_CAM_FILM], M_WINDOW_OVR_SHOW, M_ENABLE);
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 255));
	MdispControl(MilDisplayControl[_INSP_2_F_CAM_PATTERN], M_WINDOW_OVR_SHOW, M_ENABLE);
	//----------------
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 255));
	MdispControl(MilDisplayControl[_INSP_2_R_CAM_ZOOM], M_WINDOW_OVR_SHOW, M_ENABLE);
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 255));
	MdispControl(MilDisplayControl[_INSP_2_R_CAM_NORMAL], M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------------------------------------------------------------
	if(Vision_Initial_Flag)	{
		for(i= 0; i<_MAX_TMPVIEW_; i++) {
			MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &Insp_Tmp_MilImage[i]);
			MbufClear(Insp_Tmp_MilImage[i], 0L);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MmeasAllocMarker(MilSystem, M_STRIPE, M_DEFAULT, &MilMeasureMarker);

	MdispZoom(MilDisplayControl[_INSP_2_F_CAM_FILM] , _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);
	MdispZoom(MilDisplayControl[_INSP_2_F_CAM_PATTERN], _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);
	MdispZoom(MilDisplayControl[_INSP_2_R_CAM_ZOOM] , _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);
	MdispZoom(MilDisplayControl[_INSP_2_R_CAM_NORMAL], _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);

	strLogFileBuffer.Format("CreateMainWindow(HWND UserWindow1) - Complite!"); SaveLogFile(strLogFileBuffer);

	if(Vision_Initial_Flag && VISION_SYS_MODE== _DEV_MODE)	{
		MdigControl(MilDigitizer[0], M_GRAB_TIMEOUT, M_INFINITE  );
		MdigControl(MilDigitizer[1], M_GRAB_TIMEOUT, M_INFINITE  );
		MdigControl(MilDigitizer[2], M_GRAB_TIMEOUT, M_INFINITE  );
		MdigControl(MilDigitizer[3], M_GRAB_TIMEOUT, M_INFINITE  );
//		RunThread(0); 
//		RunThread(1); 
//		RunThread(2); 
//		RunThread(3); 
		InspGrabContinue_1();
		Cam_ReferenceChange(_INSP_2_F_CAM_FILM);		Cam_ReferenceChange(_INSP_2_F_CAM_PATTERN);
		Cam_ExposureChange(_INSP_2_F_CAM_FILM);		Cam_ExposureChange(_INSP_2_F_CAM_PATTERN);
		Cam_InputGainChange(_INSP_2_F_CAM_FILM);		Cam_InputGainChange(_INSP_2_F_CAM_PATTERN);

		InspGrabContinue_2();
		Cam_ReferenceChange(_INSP_2_R_CAM_ZOOM);		Cam_ReferenceChange(_INSP_2_R_CAM_NORMAL);
		Cam_ExposureChange(_INSP_2_R_CAM_ZOOM);		Cam_ExposureChange(_INSP_2_R_CAM_NORMAL);
		Cam_InputGainChange(_INSP_2_R_CAM_ZOOM);		Cam_InputGainChange(_INSP_2_R_CAM_NORMAL);

		Cam_ExpTimeDelayChange();
	}

	return true;
}
//===================================================================================
bool CMatrox::InspGrab_1(int ch) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

//	if(Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) InspGrabContinueStop();

	Vision_Grab_Mode_1= _INSP_CAM_GRAB;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		if(ch== _INSP_CAM_LEFT)
//			MdigChannel(MilDigitizer[_INSP_CAM_LEFT], M_CH1);//+M_SYNC);
//		else if(ch== _INSP_CAM_RIGHT)
//			MdigChannel(MilDigitizer[_INSP_CAM_RIGHT], M_CH0); //AHN //+M_SYNC);MilGrabImage[_INSP_CAM_RIGHT]
		MdigGrab(MilDigitizer[0], MilGrabImage[0]);		MbufCopy(MilGrabImage[0], MilDisplayImage[0]);
		MdigGrab(MilDigitizer[1], MilGrabImage[1]);		MbufCopy(MilGrabImage[1], MilDisplayImage[1]);
	}
	return true;
}
bool CMatrox::InspGrab_2(int ch) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

//	if(Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) InspGrabContinueStop();

	Vision_Grab_Mode_2= _INSP_CAM_GRAB;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		if(ch== _INSP_CAM_LEFT)
//			MdigChannel(MilDigitizer[_INSP_CAM_LEFT], M_CH1);//+M_SYNC);
//		else if(ch== _INSP_CAM_RIGHT)
//			MdigChannel(MilDigitizer[_INSP_CAM_RIGHT], M_CH0); //AHN //+M_SYNC);MilGrabImage[_INSP_CAM_RIGHT]
		MdigGrab(MilDigitizer[2], MilGrabImage[2]);		MbufCopy(MilGrabImage[2], MilDisplayImage[2]);
		MdigGrab(MilDigitizer[3], MilGrabImage[3]);		MbufCopy(MilGrabImage[3], MilDisplayImage[3]);
	}
	return true;
}

//===================================================================================
bool CMatrox::InspLiveUseGrab_1(int ch) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false; 

	if(VISION_SYS_MODE== _DEV_MODE ) {
//		if(ch== _INSP_CAM_LEFT)
//			MdigChannel(MilDigitizer[_INSP_CAM_LEFT], M_CH1);//+M_SYNC);
//		else if(ch== _INSP_CAM_RIGHT)
//			MdigChannel(MilDigitizer[_INSP_CAM_RIGHT], M_CH0); //AHN //+M_SYNC);
		MdigGrab(MilDigitizer[ch], MilGrabImage[ch]);
		MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
		if(ch== 0) {
			if(ZoomCamDispImage_1)
				MbufCopy(MilDisplayImage[ch], ZoomCamDispImage_1);
		}
		else if(ch== 1) {
				if(NormalCamDispImage_1)
					MbufCopy(MilDisplayImage[ch], NormalCamDispImage_1);
		}
	}
	return true;
}
bool CMatrox::InspLiveUseGrab_2(int ch) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false; 

	if(VISION_SYS_MODE== _DEV_MODE ) {
//		if(ch== _INSP_CAM_LEFT)
//			MdigChannel(MilDigitizer[_INSP_CAM_LEFT], M_CH1);//+M_SYNC);
//		else if(ch== _INSP_CAM_RIGHT)
//			MdigChannel(MilDigitizer[_INSP_CAM_RIGHT], M_CH0); //AHN //+M_SYNC);
		MdigGrab(MilDigitizer[ch], MilGrabImage[ch]);
		MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
		if(ch== 2) {
			if(ZoomCamDispImage_2)
				MbufCopy(MilDisplayImage[ch], ZoomCamDispImage_2);
		}
		else if(ch== 3) {
				if(NormalCamDispImage_2)
					MbufCopy(MilDisplayImage[ch], NormalCamDispImage_2);
		}
	}
	return true;
} 
//===================================================================================
bool CMatrox::InspUseGrab_INSP_2_F_CAM_FILM() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	CString strText;

	if(VISION_SYS_MODE== _DEV_MODE ) {
		if(TestImageLoadCount_L_1>= 10000) TestImageLoadCount_L_1= 0;
		MdigGrab(MilDigitizer[_INSP_2_F_CAM_FILM], MilGrabImage[_INSP_2_F_CAM_FILM]);
		MbufCopy(MilGrabImage[_INSP_2_F_CAM_FILM], MilDisplayImage[_INSP_2_F_CAM_FILM]);
//		strText.Format("%s\\Test_Image\\%05d_F.tif", strDirRootPath, TestImageLoadCount_L_1);
//		if(ModelFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN)
//			MbufSave(strText.GetBuffer(0), MilDisplayImage[_INSP_2_F_CAM_FILM]);
//		TestImageLoadCount_L_1++;
	}
	else {
		if(TestImageLoadCount_L_1>= 10) TestImageLoadCount_L_1= 0;

		
		strText.Format("%s\\Test_Image\\F_1_%02d.tif", strDirRootPath, TestImageLoadCount_L_1);

//		MIL_ID aaa;
//		if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &aaa));
//		MbufLoad(strText.GetBuffer(0), aaa);
//		MbufCopyColor(aaa, MilDisplayImage[_INSP_2_F_CAM_FILM], M_RED);
		MbufLoad(strText.GetBuffer(0), MilDisplayImage[_INSP_2_F_CAM_FILM]);

//		strText.Format("%s\\Test_Image\\F_1_%02d.tif", strDirRootPath, TestImageLoadCount_L_1);
//		MbufSave(strText.GetBuffer(0), MilDisplayImage[_INSP_2_F_CAM_FILM]);
		TestImageLoadCount_L_1++;
//		MbufFree(aaa);
	}
	if(ZoomCamDispImage_1)
	{
		MbufCopy(MilDisplayImage[_INSP_2_F_CAM_FILM], ZoomCamDispImage_1);
	}
	return true;
}
bool CMatrox::InspUseGrab_INSP_2_F_CAM_PATTERN() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	if(VISION_SYS_MODE== _DEV_MODE ) {
		CString strText;
		if(TestImageLoadCount_L_2>= 10000) TestImageLoadCount_L_2= 0;
		MdigGrab(MilDigitizer[_INSP_2_F_CAM_PATTERN], MilGrabImage[_INSP_2_F_CAM_PATTERN]);
		MbufCopy(MilGrabImage[_INSP_2_F_CAM_PATTERN], MilDisplayImage[_INSP_2_F_CAM_PATTERN]);
//		strText.Format("%s\\Test_Image\\%05d_P.tif", strDirRootPath, TestImageLoadCount_L_2);
//		if(ModelFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN)
//			MbufSave(strText.GetBuffer(0), MilDisplayImage[_INSP_2_F_CAM_PATTERN]);
//		TestImageLoadCount_L_2++;
	}
	else {
		if(TestImageLoadCount_L_2>= 10) TestImageLoadCount_L_2= 0;

		CString strText;

		strText.Format("%s\\Test_Image\\R_2_%02d.tif", strDirRootPath, TestImageLoadCount_L_2);
		
//		MIL_ID aaa;
//		if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &aaa));
//		MbufLoad(strText.GetBuffer(0), aaa);
//		MbufCopyColor(aaa, MilDisplayImage[_INSP_2_F_CAM_PATTERN], M_RED);
		MbufLoad(strText.GetBuffer(0), MilDisplayImage[_INSP_2_F_CAM_PATTERN]);
//		strText.Format("%s\\Test_Image\\R_2_%02d.tif", strDirRootPath, TestImageLoadCount_L_2);
//		MbufSave(strText.GetBuffer(0), MilDisplayImage[_INSP_2_F_CAM_PATTERN]);
		TestImageLoadCount_L_2++;
	}
	if(ZoomCamDispImage_2)
	{
			MbufCopy(MilDisplayImage[_INSP_2_F_CAM_PATTERN], ZoomCamDispImage_2);
	}
	return true;
}
//===================================================================================
bool CMatrox::InspUseGrab_INSP_2_R_CAM_ZOOM() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	if(VISION_SYS_MODE== _DEV_MODE ) {
		MdigGrab(MilDigitizer[_INSP_2_R_CAM_ZOOM], MilGrabImage[_INSP_2_R_CAM_ZOOM]);
		MbufCopy(MilGrabImage[_INSP_2_R_CAM_ZOOM], MilDisplayImage[_INSP_2_R_CAM_ZOOM]);
//		MbufLoad("d:\\z.tif", MilDisplayImage[_INSP_2_R_CAM_ZOOM]);
	}
	else {
		if(TestImageLoadCount_R_2>= 10) TestImageLoadCount_R_2= 0;

		CString strText;

		strText.Format("%s\\Test_Image\\R_Z_%02d.tif", strDirRootPath, TestImageLoadCount_R_2);
//		MIL_ID aaa;
//		if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &aaa));
//		MbufLoad(strText.GetBuffer(0), aaa);
//		MbufCopyColor(aaa, MilDisplayImage[_INSP_2_R_CAM_ZOOM], M_RED);
		MbufLoad(strText.GetBuffer(0), MilDisplayImage[_INSP_2_R_CAM_ZOOM]);
//		strText.Format("%s\\Test_Image\\R_Z_%02d.tif1", strDirRootPath, TestImageLoadCount_R_2);
//		MbufSave(strText.GetBuffer(0), MilDisplayImage[_INSP_2_R_CAM_ZOOM]);
		TestImageLoadCount_R_2++;
	}
	if(NormalCamDispImage_2)
		MbufCopy(MilDisplayImage[_INSP_2_R_CAM_ZOOM], NormalCamDispImage_2);
	return true;
}
bool CMatrox::InspUseGrab_INSP_2_R_CAM_NORMAL() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	if(VISION_SYS_MODE== _DEV_MODE ) {
		MdigGrab(MilDigitizer[_INSP_2_R_CAM_NORMAL], MilGrabImage[_INSP_2_R_CAM_NORMAL]);
		MbufCopy(MilGrabImage[_INSP_2_R_CAM_NORMAL], MilDisplayImage[_INSP_2_R_CAM_NORMAL]);
//		MbufLoad("d:\\n.tif", MilDisplayImage[_INSP_2_R_CAM_NORMAL]);
	}
	else {
		if(TestImageLoadCount_R_1>= 10) TestImageLoadCount_R_1= 0;

		CString strText;

		strText.Format("%s\\Test_Image\\R_N_%02d.tif", strDirRootPath, TestImageLoadCount_R_1);
//		MIL_ID aaa;
//		if (!MbufAllocColor(MilSystem, 3, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &aaa));
//		MbufLoad(strText.GetBuffer(0), aaa);
//		MbufCopyColor(aaa, MilDisplayImage[_INSP_2_R_CAM_NORMAL], M_RED);
		MbufLoad(strText.GetBuffer(0), MilDisplayImage[_INSP_2_R_CAM_NORMAL]);
//		strText.Format("%s\\Test_Image\\R_N_%02d.tif1", strDirRootPath, TestImageLoadCount_R_1);
//		MbufSave(strText.GetBuffer(0), MilDisplayImage[_INSP_2_R_CAM_NORMAL]);
		TestImageLoadCount_R_1++;
	}
	if(NormalCamDispImage_1)
		MbufCopy(MilDisplayImage[_INSP_2_R_CAM_NORMAL], NormalCamDispImage_1);
	return true;
}
//===================================================================================
bool CMatrox::InspGrabContinue_1() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
	Vision_Grab_Mode_1= _INSP_CAM_GRAB_CONTINUE;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigControl(MilDigitizer[0], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
//		MdigControl(MilDigitizer[1], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
	}
	else {
		;
	}
	return true;
}
bool CMatrox::InspGrabContinue_2() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
	Vision_Grab_Mode_2= _INSP_CAM_GRAB_CONTINUE;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigControl(MilDigitizer[2], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
//		MdigControl(MilDigitizer[3], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
	}
	else {
		;
	}
	return true;
}
//===================================================================================
bool CMatrox::InspGrabContinueStop_1() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
	Vision_Grab_Mode_1= _INSP_CAM_GRAB;
	Sleep(100);
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigGrabWait(MilDigitizer[_INSP_WINDOW_], M_GRAB_END);
//		MdigControl(MilDigitizer[0], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
//		MdigControl(MilDigitizer[1], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
	}
	return true;
}
bool CMatrox::InspGrabContinueStop_2() 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
	Vision_Grab_Mode_2= _INSP_CAM_GRAB;
	Sleep(100);
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigGrabWait(MilDigitizer[_INSP_WINDOW_], M_GRAB_END);
//		MdigControl(MilDigitizer[2], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
//		MdigControl(MilDigitizer[3], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
	}
	return true;
}
//===================================================================================
bool CMatrox::InspGrabTriggerModeChange_1(bool flag) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
//	Vision_Grab_Mode= _INSP_CAM_GRAB_CONTINUE;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigGrabWait(MilDigitizer[_INSP_WINDOW_], M_GRAB_END);
		if(flag) {
			MdigControl(MilDigitizer[0], M_GRAB_TIMEOUT, M_INFINITE  );
			MdigControl(MilDigitizer[1], M_GRAB_TIMEOUT, M_INFINITE  );
			MdigControl(MilDigitizer[0], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
			MdigControl(MilDigitizer[1], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
		}
		else {
			MdigControl(MilDigitizer[0], M_GRAB_ABORT, M_DEFAULT);
			MdigControl(MilDigitizer[1], M_GRAB_ABORT, M_DEFAULT);

			MdigControl(MilDigitizer[0], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
			MdigControl(MilDigitizer[1], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
		} 
	}
	return true;
}
bool CMatrox::InspGrabTriggerModeChange_2(bool flag) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
//	Vision_Grab_Mode= _INSP_CAM_GRAB_CONTINUE;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigGrabWait(MilDigitizer[_INSP_WINDOW_], M_GRAB_END);
		if(flag) {
			MdigControl(MilDigitizer[2], M_GRAB_TIMEOUT, M_INFINITE  );
			MdigControl(MilDigitizer[3], M_GRAB_TIMEOUT, M_INFINITE  );
			MdigControl(MilDigitizer[2], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
			MdigControl(MilDigitizer[3], M_GRAB_EXPOSURE_SOURCE  , M_HARDWARE_PORT0 );
		}
		else {
			MdigControl(MilDigitizer[2], M_GRAB_ABORT, M_DEFAULT);
			MdigControl(MilDigitizer[3], M_GRAB_ABORT, M_DEFAULT);

			MdigControl(MilDigitizer[2], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
			MdigControl(MilDigitizer[3], M_GRAB_EXPOSURE_SOURCE  , M_TIMER2 );
		} 
	}
	return true;
}
//===================================================================================
bool CMatrox::Insp_isGrabContinue_1() 
//-----------------------------------------------------------------------------------
{
	if(Vision_Grab_Mode_1== _INSP_CAM_GRAB_CONTINUE) return true;
	return false;
}
bool CMatrox::Insp_isGrabContinue_2() 
//-----------------------------------------------------------------------------------
{
	if(Vision_Grab_Mode_2== _INSP_CAM_GRAB_CONTINUE) return true;
	return false;
}
//===================================================================================
bool CMatrox::InspImageLoad_1(int ch, CString strName) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
//	Vision_Grab_Mode= _INSP_CAM_GRAB;
	
	MbufLoad(strName.GetBuffer(0),  MilDisplayImage[ch]);
//	MbufCopy(Insp_OriginalImage,  MilGrabImage[ch]);
//	MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
	return true;
}
//===================================================================================
bool CMatrox::InspImageSave_1(int ch, CString strName) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
//	Vision_Grab_Mode= _INSP_CAM_GRAB;
	
	MbufSave(strName.GetBuffer(0),  MilDisplayImage[ch]);
//	MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
	return true;
}
//===================================================================================
bool CMatrox::InspImageLoad_2(int ch, CString strName) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
//	Vision_Grab_Mode= _INSP_CAM_GRAB;
	
	MbufLoad(strName.GetBuffer(0),  MilDisplayImage[ch]);
//	MbufCopy(Insp_OriginalImage,  MilGrabImage[ch]);
//	MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
	return true;
}
//===================================================================================
bool CMatrox::InspImageSave_2(int ch, CString strName) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;
	
//	Vision_Grab_Mode= _INSP_CAM_GRAB;
	
	MbufSave(strName.GetBuffer(0),  MilDisplayImage[ch]);
//	MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
	return true;
}
//===================================================================================
void CMatrox::OverlayClear_1(int ID_No)
//-----------------------------------------------------------------------------------
{
	MbufClear(MilOverlayImage[ID_No], TransparentColor); 
}
//===================================================================================
void CMatrox::OverlayClear_2(int ID_No)
//-----------------------------------------------------------------------------------
{
	MbufClear(MilOverlayImage[ID_No], TransparentColor); 
}
//===================================================================================
void CMatrox::Insp_ROI_Display(int ch, CRect *rt, int ROI_Count, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString strText;
	int i;

	for(i= 0; i< ROI_Count; i++) {
		// Clear the overlay buffer with the keying color.           
		if(MilOverlayImage[ch]) {
				if(rt[i].left<0)	rt[i].left = 0;		if(rt[i].left  >_INSP_IMAGE_SIZE_X)	rt[i].left =	_INSP_IMAGE_SIZE_X;
				if(rt[i].right<0)	rt[i].right = 0;	if(rt[i].right >_INSP_IMAGE_SIZE_X)	rt[i].right =	_INSP_IMAGE_SIZE_X;
				if(rt[i].top<0)		rt[i].top = 0;		if(rt[i].top   >_INSP_IMAGE_SIZE_Y)	rt[i].top =	_INSP_IMAGE_SIZE_Y;
				if(rt[i].bottom<0)	rt[i].bottom = 0;	if(rt[i].bottom>_INSP_IMAGE_SIZE_Y)	rt[i].bottom =	_INSP_IMAGE_SIZE_Y;
			
			MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
			MgraRect(M_DEFAULT, MilOverlayImage[ch], rt[i].left, rt[i].top, rt[i].right, rt[i].bottom);
			strText.Format("%d", i);
			MgraText(M_DEFAULT, MilOverlayImage[ch], rt[i].left, rt[i].top- 6,   strText.GetBuffer(0));
		}
	}
}
//===================================================================================
void CMatrox::DrawRect_1(int ID_No, bool overlayclear, CRect rt, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString str;
	// Clear the overlay buffer with the keying color.           
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_1(ID_No);

		if(rt.left<0)	rt.left = 0;	if(rt.left  >_INSP_IMAGE_SIZE_X)	rt.left =	_INSP_IMAGE_SIZE_X;
		if(rt.right<0)	rt.right = 0;	if(rt.right >_INSP_IMAGE_SIZE_X)	rt.right =	_INSP_IMAGE_SIZE_X;
		if(rt.top<0)	rt.top = 0;		if(rt.top   >_INSP_IMAGE_SIZE_Y)	rt.top =	_INSP_IMAGE_SIZE_Y;
		if(rt.bottom<0) rt.bottom = 0;	if(rt.bottom>_INSP_IMAGE_SIZE_Y)	rt.bottom =	_INSP_IMAGE_SIZE_Y;
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraRect(M_DEFAULT, MilOverlayImage[ID_No], rt.left, rt.top, rt.right, rt.bottom);
	}
}
void CMatrox::DrawRect_2(int ID_No, bool overlayclear, CRect rt, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString str;
	// Clear the overlay buffer with the keying color.           
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_2(ID_No);

		if(rt.left<0)	rt.left = 0;	if(rt.left  >_INSP_IMAGE_SIZE_X)	rt.left =	_INSP_IMAGE_SIZE_X;
		if(rt.right<0)	rt.right = 0;	if(rt.right >_INSP_IMAGE_SIZE_X)	rt.right =	_INSP_IMAGE_SIZE_X;
		if(rt.top<0)	rt.top = 0;		if(rt.top   >_INSP_IMAGE_SIZE_Y)	rt.top =	_INSP_IMAGE_SIZE_Y;
		if(rt.bottom<0) rt.bottom = 0;	if(rt.bottom>_INSP_IMAGE_SIZE_Y)	rt.bottom =	_INSP_IMAGE_SIZE_Y;
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraRect(M_DEFAULT, MilOverlayImage[ID_No], rt.left, rt.top, rt.right, rt.bottom);
	}
}
//===================================================================================
void CMatrox::DrawLine_1(int ID_No, bool overlayclear, CPoint ptStart, CPoint ptEnd, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString str;
	// Clear the overlay buffer with the keying color.   
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_1(ID_No);
		MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		// Set drawing color to the green index in the pseudo-color LUT.
		
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraLine(M_DEFAULT, MilOverlayImage[ID_No], (long)(ptStart.x), (long)(ptStart.y), (long)(ptEnd.x), (long)(ptEnd.y));
		
	}
}
void CMatrox::DrawLine_2(int ID_No, bool overlayclear, CPoint ptStart, CPoint ptEnd, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString str;
	// Clear the overlay buffer with the keying color.   
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_2(ID_No);
		MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		// Set drawing color to the green index in the pseudo-color LUT.
		
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraLine(M_DEFAULT, MilOverlayImage[ID_No], (long)(ptStart.x), (long)(ptStart.y), (long)(ptEnd.x), (long)(ptEnd.y));
		
	}
}
//===================================================================================
void CMatrox::DrawDot_1(int ID_No, bool overlayclear, CPoint ptPoint, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString str;
	
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_1(ID_No);
		
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraDot(M_DEFAULT, MilOverlayImage[ID_No], (long)ptPoint.x, (long)ptPoint.y);

	}
}
void CMatrox::DrawDot_2(int ID_No, bool overlayclear, CPoint ptPoint, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString str;
	
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_2(ID_No);
		
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraDot(M_DEFAULT, MilOverlayImage[ID_No], (long)ptPoint.x, (long)ptPoint.y);

	}
}
//===================================================================================
void CMatrox::DrawText_1(int ID_No, bool overlayclear, CPoint ptPoint, CString strText, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString text;
	
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_1(ID_No);
		MgraFontScale(M_DEFAULT, 2.0, 2.0); 
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraText(M_DEFAULT, MilOverlayImage[ID_No], ptPoint.x, ptPoint.y,   strText.GetBuffer(0));
	}
}
void CMatrox::DrawText_2(int ID_No, bool overlayclear, CPoint ptPoint, CString strText, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString text;
	
	if(MilOverlayImage[ID_No]) {
		if(overlayclear) OverlayClear_2(ID_No);
		MgraFontScale(M_DEFAULT, 2.0, 2.0); 
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraText(M_DEFAULT, MilOverlayImage[ID_No], ptPoint.x, ptPoint.y,   strText.GetBuffer(0));
	}
}
//===================================================================================
void CMatrox::PatternDrawFillPoint_1(CPoint point, long factor, long size)
//-----------------------------------------------------------------------------------
{
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MgraArcFill(M_DEFAULT, PatternSetOverlayImage_1	 , (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
	MgraColor(M_DEFAULT, 255) ;
	MgraArcFill(M_DEFAULT, PatternDontCareImage_1, (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
}
//===================================================================================
void CMatrox::PatternDrawFillPoint_2(CPoint point, long factor, long size)
//-----------------------------------------------------------------------------------
{
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MgraArcFill(M_DEFAULT, PatternSetOverlayImage_2	 , (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
	MgraColor(M_DEFAULT, 255) ;
	MgraArcFill(M_DEFAULT, PatternDontCareImage_2, (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
}
//===================================================================================
void CMatrox::DrawFillPoint(CPoint point, long factor, long size)
//-----------------------------------------------------------------------------------
{
//	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
//	MgraArcFill(M_DEFAULT, MilSetOverlayImage	 , (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
//	MgraColor(M_DEFAULT, 255) ;
//	MgraArcFill(M_DEFAULT, ModelDontCareImage, (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
}
//===================================================================================
void CMatrox::MaskApply(int ModelIndex, double zoom)
//-----------------------------------------------------------------------------------
{
/*		MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MmodMask(MilSearchContext[ModelIndex], 0, ModelDontCareImage , M_DONT_CARES, M_DEFAULT);
						
		MmodPreprocess(MilSearchContext[ModelIndex], M_DEFAULT);
		MbufClear(MilSetOverlayImage, TransparentColor); 

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MmodDraw(M_DEFAULT,  MilSearchContext[ModelIndex], ModelSetDispImage, M_DRAW_IMAGE+M_DRAW_EDGES, 0, M_DEFAULT);
		MgraColor(M_DEFAULT, 255) ;
		MbufClear(ModelDontCareImage, 0);
		MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
		MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], MilSetOverlayImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
*/
}
//===================================================================================
void CMatrox::MaskEditClear(int ModelIndex, double zoom)
//-----------------------------------------------------------------------------------
{
/*	MbufClear(ModelDontCareImage, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MmodMask(MilSearchContext[ModelIndex], 0, ModelDontCareImage , M_DONT_CARES, M_DEFAULT);
					
		MmodPreprocess(MilSearchContext[ModelIndex], M_DEFAULT);
		MbufClear(MilSetOverlayImage, TransparentColor);

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MmodDraw(M_DEFAULT,  MilSearchContext[ModelIndex], ModelSetDispImage, M_DRAW_IMAGE+M_DRAW_EDGES, 0, M_DEFAULT);
		MgraColor(M_DEFAULT, 255) ;
		MmodDraw(M_DEFAULT,  MilSearchContext[ModelIndex], ModelDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT);
*/
}
//===================================================================================
void CMatrox::PatternMaskApply_1(int ModelIndex, double zoom)
//-----------------------------------------------------------------------------------
{
	CString strText; 

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatPreprocModel(M_NULL, MilPatternModel_1[ModelIndex], M_RESET);
	MpatCopy(MilPatternModel_1[ModelIndex], PatternDontCareImage_1 , M_DONT_CARES);

	strText.Format("%s\\PatternMaskApply_1_DC_%d.tif", strDirDebugImage, ModelIndex);
	MbufSave(strText.GetBuffer(0), PatternDontCareImage_1);
	MpatSetDontCare(MilPatternModel_1[ModelIndex], PatternDontCareImage_1, 0, 0, 255);

	MpatPreprocModel(M_NULL, MilPatternModel_1[ModelIndex], M_DEFAULT);
	MbufClear(PatternSetOverlayImage_1, TransparentColor); 

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetDispImage_1,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;
	MbufClear(PatternDontCareImage_1, 0);
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternDontCareImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;

	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetOverlayImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
}
//===================================================================================
void CMatrox::PatternMaskEditClear_1(int ModelIndex, double zoom)
//-----------------------------------------------------------------------------------
{
	MbufClear(PatternDontCareImage_1, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatCopy(MilPatternModel_1[ModelIndex], PatternDontCareImage_1 , M_DONT_CARES);
	MpatSetDontCare(MilPatternModel_1[ModelIndex], PatternDontCareImage_1, 0, 0, 255);
				
	MpatPreprocModel(M_NULL, MilPatternModel_1[ModelIndex], M_DEFAULT);
	MbufClear(PatternSetOverlayImage_1, TransparentColor); 

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetDispImage_1,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternDontCareImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetOverlayImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
}
//===================================================================================
void CMatrox::PatternMaskApply_2(int ModelIndex, double zoom)
//-----------------------------------------------------------------------------------
{
	CString strText; 

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MpatPreprocModel(M_NULL, MilPatternModel_2[ModelIndex], M_RESET);
		MpatCopy(MilPatternModel_2[ModelIndex], PatternDontCareImage_2 , M_DONT_CARES);

		strText.Format("%s\\PatternMaskApply_2_DC_%d.tif", strDirDebugImage, ModelIndex);
		MbufSave(strText.GetBuffer(0), PatternDontCareImage_2);
		MpatSetDontCare(MilPatternModel_2[ModelIndex], PatternDontCareImage_2, 0, 0, 255);

		MpatPreprocModel(M_NULL, MilPatternModel_2[ModelIndex], M_DEFAULT);
		MbufClear(PatternSetOverlayImage_2, TransparentColor); 

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetDispImage_2,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, 255) ;
		MbufClear(PatternDontCareImage_2, 0);
		MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternDontCareImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;

		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetOverlayImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
}
//===================================================================================
void CMatrox::PatternMaskEditClear_2(int ModelIndex, double zoom)
//-----------------------------------------------------------------------------------
{
	MbufClear(PatternDontCareImage_2, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MpatCopy(MilPatternModel_2[ModelIndex], PatternDontCareImage_2 , M_DONT_CARES);
		MpatSetDontCare(MilPatternModel_2[ModelIndex], PatternDontCareImage_2, 0, 0, 255);
					
		MpatPreprocModel(M_NULL, MilPatternModel_2[ModelIndex], M_DEFAULT);
		MbufClear(PatternSetOverlayImage_2, TransparentColor); 

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetDispImage_2,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternDontCareImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetOverlayImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
}
//===================================================================================
void CMatrox::DrawDefaultCross_1(bool flag)
//-----------------------------------------------------------------------------------
{
	CString text;
	
	if(flag)
	{
		OverlayClear_1(_INSP_2_F_CAM_FILM);
		OverlayClear_1(_INSP_2_F_CAM_PATTERN);
	}
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(100,100,255));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y)+1,_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y)+1);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), 0,(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), _INSP_CAM_SIZE_Y);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X)+1, 0,(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X)+1, _INSP_CAM_SIZE_Y);

	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y)+1,_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y)+1);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), _INSP_CAM_SIZE_Y);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X)+1, 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X)+1, _INSP_CAM_SIZE_Y);
}
//===================================================================================
void CMatrox::DrawDefaultCross_2()
//-----------------------------------------------------------------------------------
{
	CString text;
	
	OverlayClear_2(_INSP_2_R_CAM_ZOOM);
	OverlayClear_2(_INSP_2_R_CAM_NORMAL);
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_R_CAM_ZOOM],
		0L, (long)(ModelFile()->System_data.i2_CEN_R_ZOOM_Y),
		_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_R_ZOOM_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_R_CAM_ZOOM],
		(long)(ModelFile()->System_data.i2_CEN_R_ZOOM_X), 0,
		(long)(ModelFile()->System_data.i2_CEN_R_ZOOM_X), _INSP_CAM_SIZE_Y);

	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_R_CAM_NORMAL],
		0L, (long)(ModelFile()->System_data.i2_CEN_R_NORMAL_Y),
		_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_R_NORMAL_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_R_CAM_NORMAL],
		(long)(ModelFile()->System_data.i2_CEN_R_NORMAL_X), 0,
		(long)(ModelFile()->System_data.i2_CEN_R_NORMAL_X), _INSP_CAM_SIZE_Y);
}


//===================================================================================
double CMatrox::FindStripe(int DispID, int Insp_No, CRect rtRange, double dAngle, CPoint cpCenter, int sel, bool ThEnable, bool ThAuto, long ThVal, bool initFlag)
//-----------------------------------------------------------------------------------
{
	return 0.0;
}
//===================================================================================
double CMatrox::FindStripeLineToLine(int DispID, int Insp_No, CRect rtRange, double dAngle, CPoint cpCenter, int sel, bool ThEnable, bool ThAuto, long ThVal, bool initFlag)
//-----------------------------------------------------------------------------------
{
	return 0.0;
}


//===================================================================================
void CMatrox::MeasureResultImageSave(CString strText, int ch)
//-----------------------------------------------------------------------------------
{  
/*	if(ModelFile()->System_data.iJPGSave== 1)
		MbufExport(strText.GetBuffer(0),  M_JPEG_LOSSY, Insp_ColorImage[ch]);
	else
		MbufSave(strText.GetBuffer(0),  Insp_ColorImage[ch]);
*/
}
//===================================================================================
void CMatrox::InitLeftCamSet_1(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
	MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &ZoomCamDispDisplay_1) ;
	MdispZoom(ZoomCamDispDisplay_1, -1 , -1);
	MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X,	_INSP_CAM_SIZE_X,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ZoomCamDispImage_1);

	MdispSelectWindow(ZoomCamDispDisplay_1, ZoomCamDispImage_1, hwnd) ;
	
	MbufClear(ZoomCamDispImage_1, 0);

	ZoomCamSetHwnd_1= hwnd;
}
void CMatrox::InitLeftCamSet_2(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
	MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &ZoomCamDispDisplay_2) ;
	MdispZoom(ZoomCamDispDisplay_2, -1 , -1);
	MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X,	_INSP_CAM_SIZE_X,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ZoomCamDispImage_2);

	MdispSelectWindow(ZoomCamDispDisplay_2, ZoomCamDispImage_2, hwnd) ;
	
	MbufClear(ZoomCamDispImage_2, 0);

	ZoomCamSetHwnd_2= hwnd;
}
//===================================================================================
void CMatrox::InitRightCamSet_1(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
	MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &NormalCamDispDisplay_1) ;
	MdispZoom(NormalCamDispDisplay_1, -1 , -1);
	MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X,	_INSP_CAM_SIZE_X,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &NormalCamDispImage_1);

	MdispSelectWindow(NormalCamDispDisplay_1, NormalCamDispImage_1, hwnd) ;
	
	MbufClear(NormalCamDispImage_1, 0);
/*
	//--------
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);*/
	//--------
	NormalCamSetHwnd_1= hwnd;
}
void CMatrox::InitRightCamSet_2(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
	MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &NormalCamDispDisplay_2) ;
	MdispZoom(NormalCamDispDisplay_2, -1 , -1);
	MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X,	_INSP_CAM_SIZE_X,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &NormalCamDispImage_2);

	MdispSelectWindow(NormalCamDispDisplay_2, NormalCamDispImage_2, hwnd) ;
	
	MbufClear(NormalCamDispImage_2, 0);
/*
	//--------
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(NormalCamDispDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(NormalCamDispDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);*/
	//--------
	NormalCamSetHwnd_2= hwnd;
}

//===================================================================================
void CMatrox::InitModelSet(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
/*	if(ModelSetDisplay)		{ MdispDeselect(ModelSetDisplay,ModelSetDispImage); }
	if(ModelSetDisplay)		{ MdispFree(ModelSetDisplay);ModelSetDisplay= 0; }

	if(ModelDontCareImage)	{ MbufFree(ModelDontCareImage) ; ModelDontCareImage= 0; }
	if(ModelSetDispImage)	{ MbufFree(ModelSetDispImage) ; ModelSetDispImage= 0; }

	MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &ModelSetDisplay) ;
	MdispZoom(ModelSetDisplay, -1 , -1);
	MbufAlloc2d(MilSystem, Model_AllocSizeX,	Model_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelDontCareImage);
//	MbufAlloc2d(MilSystem, Model_AllocSizeX,	Model_AllocSizeY,	8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MaskZoomImage);
	MbufAlloc2d(MilSystem, Model_AllocSizeX,	Model_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelSetDispImage);
//	MbufAlloc2d(MilSystem, Model_AllocSizeX,	Model_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelEditImage);

	MdispSelectWindow(ModelSetDisplay, ModelSetDispImage, hwnd) ;
	
	//--------
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(MilSetOverlayImage, TransparentColor); 
	MbufClear(ModelSetDispImage, 0);

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	ModelSetHwnd= hwnd;
*/
}
//===================================================================================
void CMatrox::InitModelSet_1(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
//	if(PatternSetDisplay_1)		{ MdispDeselect(PatternSetDisplay_1,	PatternSetDispImage_1); }
//	if(PatternSetDisplay_1)		{ MdispFree(PatternSetDisplay_1);		PatternSetDisplay_1= 0; }

//	if(PatternSetDisplay_1)		MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_DISABLE);
//	if(PatternSetDisplay_1)		{ MdispDeselect(PatternSetDisplay_1,	PatternSetDispImage_1); }

//	if(PatternDontCareImage_1)	{ MbufFree(PatternDontCareImage_1) ; PatternDontCareImage_1= 0; }
//	if(PatternSetDispImage_1)	{ MbufFree(PatternSetDispImage_1) ; PatternSetDispImage_1= 0; }

	if(PatternSetDisplay_1== M_NULL) MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &PatternSetDisplay_1) ;
	if(PatternDontCareImage_1== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX,	Pattern_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage_1);
	if(PatternSetDispImage_1== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX,	Pattern_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage_1);

	MdispSelectWindow(PatternSetDisplay_1, PatternSetDispImage_1, hwnd) ;
	MdispZoom(PatternSetDisplay_1, -1 , -1);
	
	//--------
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_1);
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_1);
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternSetOverlayImage_1, TransparentColor); 
	MbufClear(PatternSetDispImage_1, 0);

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	PatternSetHwnd_1= hwnd;
}
//===================================================================================
void CMatrox::InitModelSet_2(HWND hwnd)
//-----------------------------------------------------------------------------------
{ 
//	if(PatternSetDisplay_2)		{ MdispDeselect(PatternSetDisplay_2,	PatternSetDispImage_2); }
//	if(PatternSetDisplay_2)		{ MdispFree(PatternSetDisplay_2);		PatternSetDisplay_2= 0; }

//	if(PatternDontCareImage_2)	{ MbufFree(PatternDontCareImage_2) ; PatternDontCareImage_2= 0; }
//	if(PatternSetDispImage_2)	{ MbufFree(PatternSetDispImage_2) ; PatternSetDispImage_2= 0; }

	if(PatternSetDisplay_2== M_NULL) MdispAlloc	  (MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &PatternSetDisplay_2) ;
	MdispZoom(PatternSetDisplay_2, -1 , -1);
	if(PatternDontCareImage_2== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX,	Pattern_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage_2);
	if(PatternSetDispImage_2== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX,	Pattern_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage_2);

	MdispSelectWindow(PatternSetDisplay_2, PatternSetDispImage_2, hwnd) ;
	
	//--------
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_2);
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_2);
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternSetOverlayImage_2, TransparentColor); 
	MbufClear(PatternSetDispImage_2, 0);

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	PatternSetHwnd_2= hwnd;

}

//===================================================================================
int CMatrox::ModelLoad(int index)
//-----------------------------------------------------------------------------------
{ 
/*
///////////// Cap
	CString strModelNamePath;
	double aaa= 0.;
	long bbb= 0;

//	if(!Vision_Initial_Flag) return false;

	strModelNamePath.Format("%s\\FindModel_%d.mmf", strDirConfigPath, index);
	
	if(MilSearchContext[index])	MmodPreprocess(MilSearchContext[index], M_RESET);

//	if(MilSearchContext[index])		{ MmodFree(MilSearchContext[index]);	MilSearchContext[index]= 0; }

	if(MilSearchContext[index])	{ MmodFree(MilSearchContext[index]);	MilSearchContext[index]= 0; }

	if(MmodRestore(strModelNamePath.GetBuffer(0), MilSystem, M_DEFAULT, &MilSearchContext[index])== M_NULL ) 
		aaa= 0.;
	else {
		long modelno= 0;

		MmodInquire(MilSearchContext[index], M_DEFAULT, M_OBJECT_TYPE + M_TYPE_LONG, &ModelFile()->Vision_data[index].iModel_Type);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_NUMBER + M_TYPE_LONG, &modelno);

		MmodInquire(MilSearchContext[index], M_CONTEXT, M_SPEED+ M_TYPE_LONG, &ModelFile()->Vision_data[index].iSpeed);
		MmodInquire(MilSearchContext[index], M_CONTEXT, M_ACCURACY+ M_TYPE_LONG, &ModelFile()->Vision_data[index].iAccuracy);
		MmodInquire(MilSearchContext[index], M_CONTEXT, M_SMOOTHNESS, &ModelFile()->Vision_data[index].dSmoothness);

		//	MmodInquire(MilSearchContext[index], M_DEFAULT, M_SPEED+ M_TYPE_LONG, &Model_Speed);
		//	MmodInquire(MilSearchContext[index], M_DEFAULT, M_ACCURACY+ M_TYPE_LONG, &Model_Accuracy);
		//	MmodInquire(MilSearchContext[index], M_DEFAULT, M_SMOOTHNESS, &Model_Smoothness);

		MmodInquire(MilSearchContext[index], M_CONTEXT, M_TIMEOUT+M_TYPE_LONG, &ModelFile()->Vision_data[index].iTimeOut);

		MmodInquire(MilSearchContext[index], M_DEFAULT, M_CERTAINTY+M_TYPE_DOUBLE, &ModelFile()->Vision_data[index].dScoreCertanty);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_ACCEPTANCE, &ModelFile()->Vision_data[index].dScoreAcceptance);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_CERTAINTY_TARGET, &ModelFile()->Vision_data[index].dScoreTargetCertanty);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_ACCEPTANCE_TARGET, &ModelFile()->Vision_data[index].dScoreTargetAcceptance);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_ANGLE_DELTA_NEG, &ModelFile()->Vision_data[index].dRotationDeltaNeg);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_ANGLE_DELTA_POS, &ModelFile()->Vision_data[index].dRotationDeltaPos);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_SCALE_MAX_FACTOR, &ModelFile()->Vision_data[index].dScaleMaxFactor);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_SCALE_MIN_FACTOR, &ModelFile()->Vision_data[index].dScaleMinFactor);

		MmodInquire(MilSearchContext[index], M_DEFAULT, M_ALLOC_SIZE_X+ M_TYPE_LONG, &Model_AllocSizeX);
		MmodInquire(MilSearchContext[index], M_DEFAULT, M_ALLOC_SIZE_Y+ M_TYPE_LONG, &Model_AllocSizeY);

		MmodControl(MilSearchContext[index], M_CONTEXT, M_DETAIL_LEVEL , M_HIGH);//M_MEDIUM, M_HIGH, M_VERY_HIGH, M_DEFAULT
		MmodControl(MilSearchContext[index], M_CONTEXT, M_SEARCH_SCALE_RANGE, M_ENABLE);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_SEARCH_ANGLE_RANGE, M_DISABLE);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_SPEED, ModelFile()->Vision_data[index].iSpeed);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_ACCURACY, ModelFile()->Vision_data[index].iAccuracy);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_SMOOTHNESS, ModelFile()->Vision_data[index].dSmoothness);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_TIMEOUT, ModelFile()->Vision_data[index].iTimeOut);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_DETAIL_LEVEL , M_MEDIUM); //, M_HIGH, M_VERY_HIGH, M_DEFAULT
		
		MmodPreprocess(MilSearchContext[index], M_DEFAULT);

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));

		strLogFileBuffer.Format("[LOAD] No %d=Model-Data [%s]", index, strModelNamePath); SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
		return 1;
	}*/

	return 0;
}
//===================================================================================
int CMatrox::PatternLoad_1(int index)
//-----------------------------------------------------------------------------------
{ 
///////////// Cap
	CString strModelNamePath;
	double aaa= 0.;
	long bbb= 0;
	CString strLogFileBuffer;

	strModelNamePath.Format("%s\\PatternModel_1_%d", strDirConfigPath, index);
	DWORD dwAttr = GetFileAttributes(strModelNamePath.GetBuffer(0));

	if(dwAttr!= -1 ) {
		
		if(MilPatternModel_1[index])	MpatPreprocModel(M_NULL, MilPatternModel_1[index], M_RESET);

		if(MilPatternModel_1[index])	{ MpatFree(MilPatternModel_1[index]);	MilPatternModel_1[index]= 0; }

		if(MpatRestore(MilSystem, strModelNamePath.GetBuffer(0), &MilPatternModel_1[index])== M_NULL ) 
			aaa= 0.;
		else {
			long modelno= 0;

			MpatSetSpeed(MilPatternModel_1[index], ModelFile()->Pattern_data[index].iSpeed);
			MpatSetAccuracy(MilPatternModel_1[index], ModelFile()->Pattern_data[index].iAccuracy);
			MpatSetAcceptance(MilPatternModel_1[index], ModelFile()->Pattern_data[index].dScoreAcceptance);
			MpatSetCertainty(MilPatternModel_1[index], ModelFile()->Pattern_data[index].dScoreCertanty);
			MpatSetAngle(MilPatternModel_1[index], M_SEARCH_ANGLE_MODE, M_DISABLE);
			MpatSetAngle(MilPatternModel_1[index], M_SEARCH_ANGLE_DELTA_NEG, 2);	
			MpatSetAngle(MilPatternModel_1[index], M_SEARCH_ANGLE_DELTA_POS, 2);
			//	MpatSetAngle(MilPatternModel[ModelNo], M_SEARCH_ANGLE_ACCURACY, 1);
	//		MpatSetCenter(MilPatternModel[index], ModelFile()->Pattern_data[index].dOffsetX, ModelFile()->Pattern_data[index].dOffsetY);
			long XX= 0, YY= 0;
			MpatInquire(MilPatternModel_1[index], M_ALLOC_SIZE_X+ M_TYPE_LONG, &XX);
			MpatInquire(MilPatternModel_1[index], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &YY);
			
			MpatSetCenter(MilPatternModel_1[index],	((XX+1)/2) + ModelFile()->Pattern_data[index].dOffsetX, 
													((YY+1)/2) + ModelFile()->Pattern_data[index].dOffsetY);

			MpatPreprocModel(M_NULL, MilPatternModel_1[index], M_DEFAULT);

			MgraColor(M_DEFAULT, M_RGB888(255,0,0));

			strLogFileBuffer.Format("[LOAD] No %d=PattrnModel_1-Data [%s]", index, strModelNamePath); SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return 1;
		}
	}
	return 0;
}
//===================================================================================
int CMatrox::PatternLoad_2(int index)
//-----------------------------------------------------------------------------------
{ 
///////////// Cap
	CString strModelNamePath;
	double aaa= 0.;
	long bbb= 0;
	CString strLogFileBuffer;

	strModelNamePath.Format("%s\\PatternModel_2_%d", strDirConfigPath, index);
	DWORD dwAttr = GetFileAttributes(strModelNamePath.GetBuffer(0));

	if(dwAttr!= -1 ) {
		
		if(MilPatternModel_2[index])	MpatPreprocModel(M_NULL, MilPatternModel_2[index], M_RESET);

		if(MilPatternModel_2[index])	{ MpatFree(MilPatternModel_2[index]);	MilPatternModel_2[index]= 0; }

		if(MpatRestore(MilSystem, strModelNamePath.GetBuffer(0), &MilPatternModel_2[index])== M_NULL ) 
			aaa= 0.;
		else {
			long modelno= 0;

			MpatSetSpeed(MilPatternModel_2[index], ModelFile()->Pattern_data[index].iSpeed);
			MpatSetAccuracy(MilPatternModel_2[index], ModelFile()->Pattern_data[index].iAccuracy);
			MpatSetAcceptance(MilPatternModel_2[index], ModelFile()->Pattern_data[index].dScoreAcceptance);
			MpatSetCertainty(MilPatternModel_2[index], ModelFile()->Pattern_data[index].dScoreCertanty);
			MpatSetAngle(MilPatternModel_2[index], M_SEARCH_ANGLE_MODE, M_DISABLE);
			MpatSetAngle(MilPatternModel_2[index], M_SEARCH_ANGLE_DELTA_NEG, 2);	
			MpatSetAngle(MilPatternModel_2[index], M_SEARCH_ANGLE_DELTA_POS, 2);
			//	MpatSetAngle(MilPatternModel[ModelNo], M_SEARCH_ANGLE_ACCURACY, 1);
	//		MpatSetCenter(MilPatternModel[index], ModelFile()->Pattern_data[index].dOffsetX, ModelFile()->Pattern_data[index].dOffsetY);
			long XX= 0, YY= 0;
			MpatInquire(MilPatternModel_2[index], M_ALLOC_SIZE_X+ M_TYPE_LONG, &XX);
			MpatInquire(MilPatternModel_2[index], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &YY);
			
			MpatSetCenter(MilPatternModel_2[index],	((XX+1)/2) + ModelFile()->Pattern_data[index].dOffsetX, 
													((YY+1)/2) + ModelFile()->Pattern_data[index].dOffsetY);

			MpatPreprocModel(M_NULL, MilPatternModel_2[index], M_DEFAULT);

			MgraColor(M_DEFAULT, M_RGB888(255,0,0));

			strLogFileBuffer.Format("[LOAD] No %d=PattrnModel_2-Data [%s]", index, strModelNamePath); SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return 1;
		}
	}
	return 0;
}

//===================================================================================
void CMatrox::ModelSave(int index)
//-----------------------------------------------------------------------------------
{
/*	CString strModelNamePath;
	strModelNamePath.Format("%s\\FindModel_%d.mmf", strDirConfigPath, index);

//	if(!Vision_Initial_Flag) return false;

		MmodControl(MilSearchContext[index], M_DEFAULT, M_NUMBER, 1);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_OBJECT_TYPE, ModelFile()->Vision_data[index].iModel_Type);

		MmodControl(MilSearchContext[index], M_CONTEXT, M_SEARCH_SCALE_RANGE, M_DISABLE); //M_ENABLE);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_SEARCH_ANGLE_RANGE, M_DISABLE);

		MmodControl(MilSearchContext[index], M_CONTEXT, M_SPEED, ModelFile()->Vision_data[index].iSpeed);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_ACCURACY, ModelFile()->Vision_data[index].iAccuracy);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_SMOOTHNESS, ModelFile()->Vision_data[index].dSmoothness);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_TIMEOUT, ModelFile()->Vision_data[index].iTimeOut);
		MmodControl(MilSearchContext[index], M_CONTEXT, M_DETAIL_LEVEL , M_MEDIUM); //, M_HIGH, M_VERY_HIGH, M_DEFAULT

		MmodControl(MilSearchContext[index], M_DEFAULT, M_CERTAINTY, ModelFile()->Vision_data[index].dScoreCertanty);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_ACCEPTANCE, ModelFile()->Vision_data[index].dScoreAcceptance);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_CERTAINTY_TARGET, ModelFile()->Vision_data[index].dScoreTargetCertanty);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_ACCEPTANCE_TARGET, ModelFile()->Vision_data[index].dScoreTargetAcceptance);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_ANGLE_DELTA_NEG, ModelFile()->Vision_data[index].dRotationDeltaNeg);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_ANGLE_DELTA_POS, ModelFile()->Vision_data[index].dRotationDeltaPos);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_SCALE_MAX_FACTOR, ModelFile()->Vision_data[index].dScaleMaxFactor);
		MmodControl(MilSearchContext[index], M_DEFAULT, M_SCALE_MIN_FACTOR, ModelFile()->Vision_data[index].dScaleMinFactor);

		MmodSave(strModelNamePath.GetBuffer(0), MilSearchContext[index], M_DEFAULT);
		strLogFileBuffer.Format("[SAVE] No %d=Model-Data [%s]", index, strModelNamePath); SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
*/
}
//===================================================================================
void CMatrox::PatternSave_1(int index)
//-----------------------------------------------------------------------------------
{
	CString strModelNamePath;
	CString strLogFileBuffer;

	strModelNamePath.Format("%s\\PatternModel_1_%d", strDirConfigPath, index);

	MpatSave(strModelNamePath.GetBuffer(0), MilPatternModel_1[index]);
	strLogFileBuffer.Format("[SAVE] No %d=PatternModel_1-Data [%s]", index, strModelNamePath); SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
}
//===================================================================================
void CMatrox::PatternSave_2(int index)
//-----------------------------------------------------------------------------------
{
	CString strModelNamePath;
	CString strLogFileBuffer;

	strModelNamePath.Format("%s\\PatternModel_2_%d", strDirConfigPath, index);

	MpatSave(strModelNamePath.GetBuffer(0), MilPatternModel_2[index]);
	strLogFileBuffer.Format("[SAVE] No %d=PatternModel_2-Data [%s]", index, strModelNamePath); SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
}
//===================================================================================
void CMatrox::ModelReDefine(int ModelIndex, int Cam_No, POINT pOldPoint, POINT pNewPoint)
//-----------------------------------------------------------------------------------
{
/*	CString strText;
	CPoint Start = 	pOldPoint;
	CPoint End =	pNewPoint ;

	long width = abs(Start.x - End.x) ;
	long height = abs(Start.y - End.y) ;

	if(width<= 0 || height<= 0) {
			AfxMessageBox("Model-Define Error!");		return;
	}

	if(ModelSetDisplay)					{ MdispDeselect(ModelSetDisplay,ModelSetDispImage); }
	if(ModelSetDispImage)				{ MbufFree(ModelSetDispImage) ; ModelSetDispImage= 0; }
	if(ModelDontCareImage)				{ MbufFree(ModelDontCareImage) ; ModelDontCareImage= 0; }

//	if(MilSearchContext[ModelIndex])	{ MmodFree(MilSearchContext[ModelIndex]);	MilSearchContext[ModelIndex]= 0; }
//	if(MilModelResult[ModelIndex])		{ MmodFree(MilModelResult[ModelIndex]);		MilModelResult[ModelIndex]= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelDontCareImage);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelSetDispImage);
	MdispSelectWindow(ModelSetDisplay, ModelSetDispImage, ModelSetHwnd) ;
	//--------
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------
	
	MbufCopyClip(MilDisplayImage[Cam_No], ModelSetDispImage, -Start.x , -Start.y) ;//MilGrabImage[_INSP_CAM_RIGHT]

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;


//	MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilSearchContext[ModelIndex]);
//	MmodAllocResult(MilSystem, M_DEFAULT, &MilModelResult[ModelIndex]);         

		MmodDefine(MilSearchContext[ModelIndex], M_DELETE, M_ALL, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

		MmodDefine(MilSearchContext[ModelIndex], M_IMAGE, ModelSetDispImage, 0, 0, width, height );

		MmodPreprocess(MilSearchContext[ModelIndex], M_DEFAULT);

		MbufClear(ModelDontCareImage, 0);
		MbufClear(ModelSetDispImage, 0);
		MgraColor(M_DEFAULT, M_RGB888(255,0,0));

		MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelSetDispImage ,	  M_DRAW_IMAGE+M_DRAW_EDGES, 0, M_DEFAULT) ;

		strText.Format("%s\\ReDefModel_%d.tif", strDirDebugImage, ModelIndex);
		MbufSave(strText.GetBuffer(0), ModelSetDispImage);
		
		MgraColor(M_DEFAULT, 255) ;
		MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
		MmodMask(MilSearchContext[ModelIndex], 0, ModelDontCareImage , M_DONT_CARES, M_DEFAULT);
		MmodPreprocess(MilSearchContext[ModelIndex], M_DEFAULT);
		MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelSetDispImage,	  M_DRAW_IMAGE+M_DRAW_EDGES, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
		MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], MilSetOverlayImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, 255) ;
*/
}
//===================================================================================
void CMatrox::ModelImageLoadNdisp(int ModelIndex)
//-----------------------------------------------------------------------------------
{
/*
	CString strText;

	if(MilSearchContext[ModelIndex]) {
		MmodInquire(MilSearchContext[ModelIndex], M_DEFAULT, M_ALLOC_SIZE_X+ M_TYPE_LONG, &Model_AllocSizeX);
		MmodInquire(MilSearchContext[ModelIndex], M_DEFAULT, M_ALLOC_SIZE_Y+ M_TYPE_LONG, &Model_AllocSizeY);
	}
	else {
		Model_AllocSizeX= 10;
		Model_AllocSizeY= 10;
		//return;
	}
	
	long width = Model_AllocSizeX;
	long height = Model_AllocSizeY;

	if(width<= 0 || height<= 0) {
			AfxMessageBox("Model-Define Error!");		return;
	}

	if(ModelSetDisplay)					{ MdispDeselect(ModelSetDisplay,ModelSetDispImage); }
	if(ModelSetDispImage)				{ MbufFree(ModelSetDispImage) ; ModelSetDispImage= 0; }
	if(ModelDontCareImage)				{ MbufFree(ModelDontCareImage) ; ModelDontCareImage= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelDontCareImage);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &ModelSetDispImage);
	MdispSelectWindow(ModelSetDisplay, ModelSetDispImage, ModelSetHwnd) ;
	//--------
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE); 
	MdispInquire(ModelSetDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_WINDOW_OVR_BUF_ID, &MilSetOverlayImage);
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(ModelSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(ModelSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------
	
	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MbufClear(ModelDontCareImage, 0);
	MbufClear(ModelSetDispImage, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelSetDispImage ,	  M_DRAW_IMAGE+M_DRAW_EDGES, 0, M_DEFAULT) ;

	strText.Format("%s\\ReDefModel_%d.tif", strDirDebugImage, ModelIndex);
	MbufSave(strText.GetBuffer(0), ModelSetDispImage);
	
	MgraColor(M_DEFAULT, 255) ;
	MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MmodMask(MilSearchContext[ModelIndex], 0, ModelDontCareImage , M_DONT_CARES, M_DEFAULT);
	MmodPreprocess(MilSearchContext[ModelIndex], M_DEFAULT);
	MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], ModelSetDispImage,	  M_DRAW_IMAGE+M_DRAW_EDGES, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0,255,0));
	MmodDraw(M_DEFAULT, MilSearchContext[ModelIndex], MilSetOverlayImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;
*/
}

//===================================================================================
void CMatrox::PatternReDefine_1(int ModelIndex, int Cam_No, POINT pOldPoint, POINT pNewPoint)
//-----------------------------------------------------------------------------------
{
	CString strText;
	CPoint Start = 	pOldPoint;
	CPoint End =	pNewPoint ;

	long width = abs(Start.x - End.x) ;
	long height = abs(Start.y - End.y) ;

	if(width<= 0 || height<= 0) {
			AfxMessageBox("PatternModel_1-Define Error!");		return;
	}

	if(PatternSetDisplay_1)					{ MdispDeselect(PatternSetDisplay_1, PatternSetDispImage_1); }
	if(PatternSetDispImage_1)				{ MbufFree(PatternSetDispImage_1) ; PatternSetDispImage_1= 0; }
	if(PatternDontCareImage_1)				{ MbufFree(PatternDontCareImage_1) ; PatternDontCareImage_1= 0; }

		if(MilPatternModel_1[ModelIndex])	MpatPreprocModel(M_NULL, MilPatternModel_1[ModelIndex], M_RESET);

		if(MilPatternModel_1[ModelIndex])	{ MpatFree(MilPatternModel_1[ModelIndex]);	MilPatternModel_1[ModelIndex]= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage_1);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage_1);
	MdispSelectWindow(PatternSetDisplay_1, PatternSetDispImage_1, PatternSetHwnd_1) ;
	//--------
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_1);
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_1);
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------
	
		MbufClear(PatternDontCareImage_1, 0);
		MbufClear(PatternSetDispImage_1, 0);

	MbufCopyClip(MilDisplayImage[Cam_No], PatternSetDispImage_1, -Start.x , -Start.y) ;//MilGrabImage[_INSP_CAM_RIGHT]

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));

		MpatAllocModel(MilSystem, PatternSetDispImage_1, 0, 0, 
			width, height, M_NORMALIZED, &MilPatternModel_1[ModelIndex]);
		MpatPreprocModel(PatternSetDispImage_1, MilPatternModel_1[ModelIndex], M_DEFAULT);

		MbufClear(PatternDontCareImage_1, 0);
		MbufClear(PatternSetDispImage_1, 0);
		MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetDispImage_1 ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

		strText.Format("%s\\ReDefPattern_1_%d.tif", strDirDebugImage, ModelIndex);
		MbufSave(strText.GetBuffer(0), PatternSetDispImage_1);

		MgraColor(M_DEFAULT, 255) ;
		MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternDontCareImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
		MpatCopy(MilPatternModel_1[ModelIndex], PatternDontCareImage_1 , M_DONT_CARES);
		MpatPreprocModel(M_NULL, MilPatternModel_1[ModelIndex], M_DEFAULT);
		MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetDispImage_1,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, 255) ;
}
//===================================================================================
void CMatrox::PatternReDefine_2(int ModelIndex, int Cam_No, POINT pOldPoint, POINT pNewPoint)
//-----------------------------------------------------------------------------------
{
	CString strText;
	CPoint Start = 	pOldPoint;
	CPoint End =	pNewPoint ;

	long width = abs(Start.x - End.x) ;
	long height = abs(Start.y - End.y) ;

	if(width<= 0 || height<= 0) {
			AfxMessageBox("PatternModel_2-Define Error!");		return;
	}

	if(PatternSetDisplay_2)					{ MdispDeselect(PatternSetDisplay_2, PatternSetDispImage_2); }
	if(PatternSetDispImage_2)				{ MbufFree(PatternSetDispImage_2) ; PatternSetDispImage_2= 0; }
	if(PatternDontCareImage_2)				{ MbufFree(PatternDontCareImage_2) ; PatternDontCareImage_2= 0; }

		if(MilPatternModel_2[ModelIndex])	MpatPreprocModel(M_NULL, MilPatternModel_2[ModelIndex], M_RESET);

		if(MilPatternModel_2[ModelIndex])	{ MpatFree(MilPatternModel_2[ModelIndex]);	MilPatternModel_2[ModelIndex]= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage_2);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage_2);
	MdispSelectWindow(PatternSetDisplay_2, PatternSetDispImage_2, PatternSetHwnd_2) ;
	//--------
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_2);
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_2);
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------
	
		MbufClear(PatternDontCareImage_2, 0);
		MbufClear(PatternSetDispImage_2, 0);

	MbufCopyClip(MilDisplayImage[Cam_No], PatternSetDispImage_2, -Start.x , -Start.y) ;//MilGrabImage[_INSP_CAM_RIGHT]

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));

		MpatAllocModel(MilSystem, PatternSetDispImage_2, 0, 0, 
			width, height, M_NORMALIZED, &MilPatternModel_2[ModelIndex]);
		MpatPreprocModel(PatternSetDispImage_2, MilPatternModel_2[ModelIndex], M_DEFAULT);

		MbufClear(PatternDontCareImage_2, 0);
		MbufClear(PatternSetDispImage_2, 0);
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetDispImage_2 ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

		strText.Format("%s\\ReDefPattern_2_%d.tif", strDirDebugImage, ModelIndex);
		MbufSave(strText.GetBuffer(0), PatternSetDispImage_2);

		MgraColor(M_DEFAULT, 255) ;
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternDontCareImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
		MpatCopy(MilPatternModel_2[ModelIndex], PatternDontCareImage_2 , M_DONT_CARES);
		MpatPreprocModel(M_NULL, MilPatternModel_2[ModelIndex], M_DEFAULT);
		MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetDispImage_2,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
		MgraColor(M_DEFAULT, 255) ;
}

//===================================================================================
void CMatrox::PatternImageLoadNdisp_1(int ModelIndex)
//-----------------------------------------------------------------------------------
{

	CString strText;

	if(MilPatternModel_1[ModelIndex]) {
		MpatInquire(MilPatternModel_1[ModelIndex], M_ALLOC_SIZE_X+ M_TYPE_LONG, &Pattern_AllocSizeX);
		MpatInquire(MilPatternModel_1[ModelIndex], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &Pattern_AllocSizeY);
	}
	else {
		Pattern_AllocSizeX= 10;
		Pattern_AllocSizeY= 10;
		//return;
	}
	
	long width = Pattern_AllocSizeX;
	long height = Pattern_AllocSizeY;

	if(width<= 0 || height<= 0) {
			AfxMessageBox("PatternModel_1-Define Error!");		return;
	}

	if(PatternSetDisplay_1)					{ MdispDeselect(PatternSetDisplay_1,PatternSetDispImage_1); }
	if(PatternSetDispImage_1)				{ MbufFree(PatternSetDispImage_1) ; PatternSetDispImage_1= 0; }
	if(PatternDontCareImage_1)				{ MbufFree(PatternDontCareImage_1) ; PatternDontCareImage_1= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage_1);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage_1);
	MdispSelectWindow(PatternSetDisplay_1, PatternSetDispImage_1, PatternSetHwnd_1) ;
	//--------
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_ENABLE); 
	MdispInquire(PatternSetDisplay_1, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_1);
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_1);
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_1, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay_1, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------
	
	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MbufClear(PatternDontCareImage_1, 0);
	MbufClear(PatternSetDispImage_1, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetDispImage_1 ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format("%s\\ReDefPattern_1_%d.tif", strDirDebugImage, ModelIndex);
	MbufSave(strText.GetBuffer(0), PatternSetDispImage_1);
	
	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternDontCareImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	strText.Format("%s\\PatternImageLoadNdisp_1_DC_%d.tif", strDirDebugImage, ModelIndex);
	MbufSave(strText.GetBuffer(0), PatternDontCareImage_1);

	MpatPreprocModel(M_NULL, MilPatternModel_1[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetDispImage_1,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;

	MpatDraw(M_DEFAULT, MilPatternModel_1[ModelIndex], PatternSetOverlayImage_1, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;
}
//===================================================================================
void CMatrox::PatternImageLoadNdisp_2(int ModelIndex)
//-----------------------------------------------------------------------------------
{

	CString strText;
	
	if(MilPatternModel_2[ModelIndex]) {
		MpatInquire(MilPatternModel_2[ModelIndex], M_ALLOC_SIZE_X+ M_TYPE_LONG, &Pattern_AllocSizeX);
		MpatInquire(MilPatternModel_2[ModelIndex], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &Pattern_AllocSizeY);
	}
	else {
		Pattern_AllocSizeX= 10;
		Pattern_AllocSizeY= 10;
		//return;
	}
	
	long width = Pattern_AllocSizeX;
	long height = Pattern_AllocSizeY;

	if(width<= 0 || height<= 0) {
			AfxMessageBox("PatternModel_2-Define Error!");		return;
	}

	if(PatternSetDisplay_2)					{ MdispDeselect(PatternSetDisplay_2,PatternSetDispImage_2); }
	if(PatternSetDispImage_2)				{ MbufFree(PatternSetDispImage_2) ; PatternSetDispImage_2= 0; }
	if(PatternDontCareImage_2)				{ MbufFree(PatternDontCareImage_2) ; PatternDontCareImage_2= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage_2);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage_2);
	MdispSelectWindow(PatternSetDisplay_2, PatternSetDispImage_2, PatternSetHwnd_2) ;
	//--------
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_ENABLE); 
	MdispInquire(PatternSetDisplay_2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_2);
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage_2);
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay_2, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay_2, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------
	
	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MbufClear(PatternDontCareImage_2, 0);
	MbufClear(PatternSetDispImage_2, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetDispImage_2 ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format("%s\\ReDefPattern_2_%d.tif", strDirDebugImage, ModelIndex);
	MbufSave(strText.GetBuffer(0), PatternSetDispImage_2);
	
	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternDontCareImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	strText.Format("%s\\PatternImageLoadNdisp_2_DC_%d.tif", strDirDebugImage, ModelIndex);
	MbufSave(strText.GetBuffer(0), PatternDontCareImage_2);

	MpatPreprocModel(M_NULL, MilPatternModel_2[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetDispImage_2,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;

	MpatDraw(M_DEFAULT, MilPatternModel_2[ModelIndex], PatternSetOverlayImage_2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;
}

//===================================================================================
bool CMatrox::ModelFinderPos(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
/*	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_mkdir(fnSave);
	SaveFileName.Format ("%s\\FindImage_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modAngle,*modPosx, *modPosy;
	long   Model[50]  = {0L, };                     
	double Score[50],         
           Time = 0.0;                           

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MmodFind(MilSearchContext[ModelNo], MilDisplayImage[Cam_No], MilModelResult[ModelNo]);

	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
	TRACE("The search time is %.1f ms\n", dTime*1000.0);


	MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_NUMBER+M_TYPE_LONG, &number);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modAngle = new double[number] ;
			modPosx = new double[number] ;
			modPosy = new double[number] ;

			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_SCORE, Score);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_ANGLE, modAngle) ;			*angle= modAngle[findno];
			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_POSITION_X, modPosx) ;		*posx= modPosx[findno];
			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_POSITION_Y, modPosy) ;		*posy= modPosy[findno];

			*posx= *posx + (long)ModelFile()->Vision_data[ModelNo].dOffsetX;
			*posy= *posy + (long)ModelFile()->Vision_data[ModelNo].dOffsetY;

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			*angle= (double)(GetTickCount()- dst);
			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
				else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

				if(ModelFile()->System_data.iOverlaySaveEnable) {
					MmodDraw(M_DEFAULT, MilModelResult[ModelNo], MilDisplayImage[Cam_No], M_DRAW_BOX, 0, M_DEFAULT) ;
				}
				else MmodDraw(M_DEFAULT, MilModelResult[ModelNo], MilOverlayImage[Cam_No], M_DRAW_BOX, 0, M_DEFAULT) ;//M_DRAW_EDGES+
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

//				str1.Format("PosX:%3.3f, PosY:%3.3f",*posx, *posy);
//				MgraText(M_DEFAULT, MilOverlayImage[ModelNo], 10L, 50L,   str1.GetBuffer(0));

			}

			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(ModelFile()->System_data.iOverlaySaveEnable) {
				MgraLine(M_DEFAULT, MilDisplayImage[Cam_No], (long)(*posx-50), (long)(*posy), (long)(*posx+50), (long)(*posy));
				MgraLine(M_DEFAULT, MilDisplayImage[Cam_No], (long)(*posx), (long)(*posy-50), (long)(*posx), (long)(*posy+50));
			}
			else {
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No], (long)(*posx-50), (long)(*posy), (long)(*posx+50), (long)(*posy));
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No], (long)(*posx), (long)(*posy-50), (long)(*posx), (long)(*posy+50));
			}

			if(modAngle) delete [] modAngle ;
			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
	
			strLogFileBuffer.Format("[FIND] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]", ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_FindImage_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
//				str1.Format("[%d]PosX:???, PosY:???", number);
//				MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 50L, 50L,   str1.GetBuffer(0));
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[FIND] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
*/
		return false;
}
//===================================================================================
bool CMatrox::ModelFinderPos_R(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
/*	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_mkdir(fnSave);
	SaveFileName.Format ("%s\\FindImage_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modAngle,*modPosx, *modPosy;
	long   Model[50]  = {0L, };                    
	double Score[50],       
           Time = 0.0;                          

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MmodFind(MilSearchContext[ModelNo], MilDisplayImage[Cam_No], MilModelResult[ModelNo]);

	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
	TRACE("The Pat_search time is %.1f ms\n", dTime*1000.0);

	MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_NUMBER+M_TYPE_LONG, &number);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modAngle = new double[number] ;
			modPosx = new double[number] ;
			modPosy = new double[number] ;

			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_SCORE, Score);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_ANGLE, modAngle) ;			*angle= modAngle[findno];
			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_POSITION_X, modPosx) ;		*posx= modPosx[findno];
			MmodGetResult(MilModelResult[ModelNo], M_DEFAULT, M_POSITION_Y, modPosy) ;		*posy= modPosy[findno];

			*posx= *posx + (long)ModelFile()->Vision_data[ModelNo].dOffsetX;
			*posy= *posy + (long)ModelFile()->Vision_data[ModelNo].dOffsetY;

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			*angle= (double)(GetTickCount()- dst);
			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
				else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

				if(ModelFile()->System_data.iOverlaySaveEnable) {
					MmodDraw(M_DEFAULT, MilModelResult[ModelNo], MilDisplayImage[Cam_No], M_DRAW_BOX, 0, M_DEFAULT) ;//M_DRAW_EDGES+
				}
				else MmodDraw(M_DEFAULT, MilModelResult[ModelNo], MilOverlayImage[Cam_No], M_DRAW_BOX, 0, M_DEFAULT) ;
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

//				str1.Format("PosX:%3.3f, PosY:%3.3f",*posx, *posy);
//				MgraText(M_DEFAULT, MilOverlayImage[ModelNo], 10L, 50L,   str1.GetBuffer(0));

			}

			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(ModelFile()->System_data.iOverlaySaveEnable) {
				MgraLine(M_DEFAULT, MilDisplayImage[Cam_No], (long)(*posx-50), (long)(*posy), (long)(*posx+50), (long)(*posy));
				MgraLine(M_DEFAULT, MilDisplayImage[Cam_No], (long)(*posx), (long)(*posy-50), (long)(*posx), (long)(*posy+50));
			}
			else {
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No], (long)(*posx-50), (long)(*posy), (long)(*posx+50), (long)(*posy));
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No], (long)(*posx), (long)(*posy-50), (long)(*posx), (long)(*posy+50));
			}

			if(modAngle) delete [] modAngle ;
			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
	
			strLogFileBuffer.Format("[FIND] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]", ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_FindImage_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
//				str1.Format("[%d]PosX:???, PosY:???", number);
//				MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 50L, 50L,   str1.GetBuffer(0));
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[FIND] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
*/
		return false;
}




//===================================================================================
bool CMatrox::PatternFinderPos_1(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	CString strLogFileBuffer;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay);
	_mkdir(fnSave);
	if(Cam_No== _INSP_2_F_CAM_FILM)
		SaveFileName.Format ("%s\\F_Image_%02d%02d%02d_%d.tif", fnSave, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	else if(Cam_No== _INSP_2_F_CAM_PATTERN)
		SaveFileName.Format ("%s\\P_Image_%02d%02d%02d_%d.tif", fnSave, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	else
		SaveFileName.Format ("%s\\Image_%02d%02d%02d_%d.tif", fnSave, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	/* Reset the timer. */
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(MilDisplayImage[Cam_No], MilPatternModel_1[ModelNo], MilPatternResult_1[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
//Debug	TRACE("_1 The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult_1[ModelNo], M_NULL);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modPosx = new double[number] ;
			modPosy = new double[number] ;
			Score = new double[number] ;

      /* Get the results for each model. */
			MpatGetResult(MilPatternResult_1[ModelNo],  M_SCORE, Score);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_X, modPosx);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_Y, modPosy);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			*posx= modPosx[findno];
			*posy= modPosy[findno];
			*angle= Score[findno];

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) {
					MgraColor(M_DEFAULT, M_RGB888(255,50,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
				}
				else	{
					MgraColor(M_DEFAULT, M_RGB888(100,100,255));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
				}

				MgraFontScale(M_DEFAULT, 1.0, 1.0);

				long dist= 50;
				if(ModelFile()->System_data.iOverlaySaveEnable) {
//					MpatDraw(M_DEFAULT, MilDisplayImage[ModelNo], MilDisplayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX  , 0, M_DEFAULT) ;
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				}
				else 
				{
				//	MpatDraw(M_DEFAULT, MilPatternResult_1[ModelNo], MilOverlayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX , 0, M_DEFAULT) ;
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				}
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

			}

//			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
//			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
			if(Score) delete [] Score ;
	
			strLogFileBuffer.Format("[PAT_FIND_1] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]", 
				ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			if(Cam_No== _INSP_2_F_CAM_FILM && ModelFile()->System_data.iFilmCameraSave)
			{
				MbufCopy(MilDisplayImage[_INSP_2_F_CAM_FILM], Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM]);
				MgraColor(M_DEFAULT, 100);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], 0L,	_INSP_CAM_SIZE_Y / 2, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y / 2);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], _INSP_CAM_SIZE_X/2,	0, _INSP_CAM_SIZE_X / 2, _INSP_CAM_SIZE_Y);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], cenx-10,	ceny, cenx+10, ceny);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], cenx,	ceny-10, cenx, ceny+10);
				CString str0;
				str0.Format("X= %d : %.3fmm", cenx - (_INSP_CAM_SIZE_X/2), (double)(cenx- (_INSP_CAM_SIZE_X/2)) * ModelFile()->System_data.d2_Pixelmm_FILM);
				MgraText(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], 10L, 30L, str0.GetBuffer(0));
				MbufSave(SaveFileName.GetBuffer(0), Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM]);
			}
			else if(Cam_No== _INSP_2_F_CAM_PATTERN && ModelFile()->System_data.iRollCameraSave)
			{
				MbufCopy(MilDisplayImage[_INSP_2_F_CAM_PATTERN], Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN]);
				MgraColor(M_DEFAULT, 100);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], 0L,	_INSP_CAM_SIZE_Y / 2, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y / 2);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], _INSP_CAM_SIZE_X/2,	0, _INSP_CAM_SIZE_X / 2, _INSP_CAM_SIZE_Y);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], cenx-10,	ceny, cenx+10, ceny);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], cenx,	ceny-10, cenx, ceny+10);
				CString str0;
				str0.Format("X= %d : %.3fmm", cenx - (_INSP_CAM_SIZE_X/2) , (double)(cenx- (_INSP_CAM_SIZE_X/2) ) * ModelFile()->System_data.d2_Pixelmm_PATTERN);
				MgraText(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], 10L, 30L, str0.GetBuffer(0));
				MbufSave(SaveFileName.GetBuffer(0), Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN]);
			}
			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_Pat_FindImage_1_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[PAT_FIND_1] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
		return false;
}
bool CMatrox::PatternFinderPos_1_Test(int no, double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	CString strLogFileBuffer;
	int i;

	fnSave.Format ("D:\\%d년%d월%d일", st.wYear, st.wMonth, st.wDay);
	_mkdir(fnSave);
	if(Cam_No== _INSP_2_F_CAM_FILM)
		SaveFileName.Format ("%s\\F_Image_%02d%02d%02d_%d.tif", fnSave, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	else if(Cam_No== _INSP_2_F_CAM_PATTERN)
		SaveFileName.Format ("%s\\P_Image_%02d%02d%02d_%d.tif", fnSave, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	else
		SaveFileName.Format ("%s\\Image_%02d%02d%02d_%d.tif", fnSave, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	/* Reset the timer. */
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(MilDisplayImage[Cam_No], MilPatternModel_1[ModelNo], MilPatternResult_1[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
//Debug	TRACE("_1 The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult_1[ModelNo], M_NULL);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modPosx = new double[number] ;
			modPosy = new double[number] ;
			Score = new double[number] ;

      /* Get the results for each model. */
			MpatGetResult(MilPatternResult_1[ModelNo],  M_SCORE, Score);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_X, modPosx);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_Y, modPosy);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			*posx= modPosx[findno];
			*posy= modPosy[findno];
			*angle= Score[findno];

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) {
					MgraColor(M_DEFAULT, M_RGB888(255,50,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					str1.Format("%02d= X:%3.1f,Y:%3.1f", no, *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					if(allsaveflag)
					{
						MgraColor(M_DEFAULT, M_RGB888(255,50,50));
						MgraFontScale(M_DEFAULT, 1.5, 1.5); 
						str1.Format("%3.1f", Score[findno]);
						MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
						str1.Format("%02d= X:%3.1f,Y:%3.1f", no, *posx, *posy);
						MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
						MgraFontScale(M_DEFAULT, 1.0, 1.0);
					}
				}
				else	{
					MgraColor(M_DEFAULT, M_RGB888(100,100,255));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					str1.Format("%02d= X:%3.1f,Y:%3.1f", no, *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
					if(allsaveflag)
					{
						MgraColor(M_DEFAULT, M_RGB888(100,100,255));
						MgraFontScale(M_DEFAULT, 1.5, 1.5); 
						str1.Format("%3.1f", Score[findno]);
						MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
						str1.Format("%02d= X:%3.1f,Y:%3.1f", no, *posx, *posy);
						MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
						MgraFontScale(M_DEFAULT, 1.0, 1.0);
						MgraColor(M_DEFAULT, M_RGB888(50,255,50));
					}
				}

				MgraFontScale(M_DEFAULT, 1.0, 1.0);

				long dist= 50;
				if(ModelFile()->System_data.iOverlaySaveEnable) {
//					MpatDraw(M_DEFAULT, MilDisplayImage[ModelNo], MilDisplayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX  , 0, M_DEFAULT) ;
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				}
				else 
				{
				//	MpatDraw(M_DEFAULT, MilPatternResult_1[ModelNo], MilOverlayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX , 0, M_DEFAULT) ;
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				}
				if(allsaveflag)
				{
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				}
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

			}

//			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
//			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
			if(Score) delete [] Score ;
	
			strLogFileBuffer.Format("[PAT_FIND_1] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]", 
				ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			if(Cam_No== _INSP_2_F_CAM_FILM && ModelFile()->System_data.iFilmCameraSave)
			{
				MbufCopy(MilDisplayImage[_INSP_2_F_CAM_FILM], Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM]);
				MgraColor(M_DEFAULT, 100);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], 0L,	_INSP_CAM_SIZE_Y / 2, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y / 2);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], _INSP_CAM_SIZE_X/2,	0, _INSP_CAM_SIZE_X / 2, _INSP_CAM_SIZE_Y);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], cenx-10,	ceny, cenx+10, ceny);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], cenx,	ceny-10, cenx, ceny+10);
				CString str0;
				str0.Format("X= %d : %.3fmm", cenx - (_INSP_CAM_SIZE_X/2), (double)(cenx- (_INSP_CAM_SIZE_X/2)) * ModelFile()->System_data.d2_Pixelmm_FILM);
				MgraText(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM], 10L, 30L, str0.GetBuffer(0));
				MbufSave(SaveFileName.GetBuffer(0), Insp_Tmp_MilImage[_INSP_2_F_CAM_FILM]);
			}
			else if(Cam_No== _INSP_2_F_CAM_PATTERN && ModelFile()->System_data.iRollCameraSave)
			{
				MbufCopy(MilDisplayImage[_INSP_2_F_CAM_PATTERN], Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN]);
				MgraColor(M_DEFAULT, 100);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], 0L,	_INSP_CAM_SIZE_Y / 2, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y / 2);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], _INSP_CAM_SIZE_X/2,	0, _INSP_CAM_SIZE_X / 2, _INSP_CAM_SIZE_Y);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], cenx-10,	ceny, cenx+10, ceny);
				MgraLine(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], cenx,	ceny-10, cenx, ceny+10);
				CString str0;
				str0.Format("X= %d : %.3fmm", cenx - (_INSP_CAM_SIZE_X/2) , (double)(cenx- (_INSP_CAM_SIZE_X/2) ) * ModelFile()->System_data.d2_Pixelmm_PATTERN);
				MgraText(M_DEFAULT, Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN], 10L, 30L, str0.GetBuffer(0));
				MbufSave(SaveFileName.GetBuffer(0), Insp_Tmp_MilImage[_INSP_2_F_CAM_PATTERN]);
			}
			if(allsaveflag)
			{
				MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y));
				MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), _INSP_CAM_SIZE_Y);
				SaveFileName.Format ("%s\\%02d.tif", fnSave, no);
				MbufSave(SaveFileName.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_Pat_FindImage_1_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			if(allsaveflag)
			{
				SaveFileName.Format ("%s\\%02d.tif", fnSave, st.wHour, st.wMinute, st.wSecond);
				MbufSave(SaveFileName.GetBuffer (0), MilDisplayImage[Cam_No]);
			}

			strLogFileBuffer.Format("[PAT_FIND_1] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
		return false;
}

//===================================================================================
bool CMatrox::PatternFinderPos_2(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	CString strLogFileBuffer;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_mkdir(fnSave);
	SaveFileName.Format ("%s\\Pat_FindImage_2_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	/* Reset the timer. */
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(MilDisplayImage[Cam_No], MilPatternModel_2[ModelNo], MilPatternResult_2[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
//Debug	TRACE("_2 The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult_2[ModelNo], M_NULL);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modPosx = new double[number] ;
			modPosy = new double[number] ;
			Score = new double[number] ;

      /* Get the results for each model. */
			MpatGetResult(MilPatternResult_2[ModelNo],  M_SCORE, Score);
			MpatGetResult(MilPatternResult_2[ModelNo], M_POSITION_X, modPosx);
			MpatGetResult(MilPatternResult_2[ModelNo], M_POSITION_Y, modPosy);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			*posx= modPosx[findno];
			*posy= modPosy[findno];
			*angle= Score[findno];

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) {
					MgraColor(M_DEFAULT, M_RGB888(255,50,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
				}
				else	{ //
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
				}

				MgraFontScale(M_DEFAULT, 1.0, 1.0);

				long dist= 50;
				if(ModelFile()->System_data.iOverlaySaveEnable) {
				//	MpatDraw(M_DEFAULT, MilPatternResult_2[ModelNo], MilDisplayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX  , 0, M_DEFAULT) ;
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				}
				else //
				{
					//MpatDraw(M_DEFAULT, MilPatternResult_2[ModelNo], MilOverlayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX , 0, M_DEFAULT) ;
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));

// 					m_i2_CAM_FILM_ROLL_OFFSET_X= ModelFile()->System_data.i2_CAM_FILM_ROLL_OFFSET_X;
// 					m_i2_CAM_FILM_ROLL_OFFSET_Y= ModelFile()->System_data.i2_CAM_FILM_ROLL_OFFSET_Y;
// 					m_i2_CAM_NOR_ZOOM_OFFSET_X= ModelFile()->System_data.i2_CAM_NOR_ZOOM_OFFSET_X;
// 					m_i2_CAM_NOR_ZOOM_OFFSET_Y= ModelFile()->System_data.i2_CAM_NOR_ZOOM_OFFSET_Y;
					if(ModelNo % 2 == 0) {
						MgraColor(M_DEFAULT, M_RGB888(255,255,0));
						if(Cam_No== _INSP_2_R_CAM_NORMAL)
							MgraArcFill(M_DEFAULT, MilOverlayImage[Cam_No], 
								(long)(cenx)+(long)(-ModelFile()->System_data.i2_CAM_NOR_ZOOM_OFFSET_X/ModelFile()->System_data.d2_Pixelmm_NORMAL), 
								(long)(ceny)+(long)(ModelFile()->System_data.i2_CAM_NOR_ZOOM_OFFSET_Y/ModelFile()->System_data.d2_Pixelmm_NORMAL), 
								6, 6, 0, 360) ;
						else
							MgraArcFill(M_DEFAULT, MilOverlayImage[Cam_No], 
								(long)(cenx)+(long)(-ModelFile()->System_data.i2_CAM_NOR_ZOOM_OFFSET_X/ModelFile()->System_data.d2_Pixelmm_ZOOM), 
								(long)(ceny)+(long)(ModelFile()->System_data.i2_CAM_NOR_ZOOM_OFFSET_Y/ModelFile()->System_data.d2_Pixelmm_ZOOM), 
								6, 6, 0, 360) ;
					}
				}
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

			}

//			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
//			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
			if(Score) delete [] Score ;
	
			strLogFileBuffer.Format("[PAT_FIND_2] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]", 
				ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_Pat_FindImage_2_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[PAT_FIND_2] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
		return false;
}


//===================================================================================
bool CMatrox::PatternFinderPos_R_1(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	CString strLogFileBuffer;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_mkdir(fnSave);
	SaveFileName.Format ("%s\\Pat_FindImage_1_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	/* Reset the timer. */
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(MilDisplayImage[Cam_No], MilPatternModel_1[ModelNo], MilPatternResult_1[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
//Debug	TRACE("_1 The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult_1[ModelNo], M_NULL);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modPosx = new double[number] ;
			modPosy = new double[number] ;
			Score = new double[number] ;

      /* Get the results for each model. */
			MpatGetResult(MilPatternResult_1[ModelNo],  M_SCORE, Score);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_X, modPosx);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_Y, modPosy);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			*posx= modPosx[findno];
			*posy= modPosy[findno];
			*angle= Score[findno];

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) {
					MgraColor(M_DEFAULT, M_RGB888(255,50,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
				}
				else	{
					MgraColor(M_DEFAULT, M_RGB888(100,100,255));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
				}

				MgraFontScale(M_DEFAULT, 1.0, 1.0);

				if(ModelFile()->System_data.iOverlaySaveEnable) {
					MpatDraw(M_DEFAULT, MilPatternResult_1[ModelNo], MilDisplayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX  , 0, M_DEFAULT) ;
				}
				else MpatDraw(M_DEFAULT, MilPatternResult_1[ModelNo], MilOverlayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX , 0, M_DEFAULT) ;
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

			}

//			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
//			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
			if(Score) delete [] Score ;
	
			strLogFileBuffer.Format("[PAT_FIND_1] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]",
				ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_Pat_FindImage_1_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[PAT_FIND_1] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
		return false;
}
//===================================================================================
bool CMatrox::PatternFinderPos_R_2(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	CString strLogFileBuffer;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_mkdir(fnSave);
	SaveFileName.Format ("%s\\Pat_FindImage_2_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	/* Reset the timer. */
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(MilDisplayImage[Cam_No], MilPatternModel_2[ModelNo], MilPatternResult_2[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
//Debug	TRACE("_2 The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult_2[ModelNo], M_NULL);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modPosx = new double[number] ;
			modPosy = new double[number] ;
			Score = new double[number] ;

      /* Get the results for each model. */
			MpatGetResult(MilPatternResult_2[ModelNo],  M_SCORE, Score);
			MpatGetResult(MilPatternResult_2[ModelNo], M_POSITION_X, modPosx);
			MpatGetResult(MilPatternResult_2[ModelNo], M_POSITION_Y, modPosy);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			*posx= modPosx[findno];
			*posy= modPosy[findno];
			*angle= Score[findno];

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) {
					MgraColor(M_DEFAULT, M_RGB888(255,50,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
				}
				else	{
					MgraColor(M_DEFAULT, M_RGB888(100,100,255));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
				}

				MgraFontScale(M_DEFAULT, 1.0, 1.0);

				if(ModelFile()->System_data.iOverlaySaveEnable) {
					MpatDraw(M_DEFAULT, MilPatternResult_2[ModelNo], MilDisplayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX  , 0, M_DEFAULT) ;
				}
				else MpatDraw(M_DEFAULT, MilPatternResult_2[ModelNo], MilOverlayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX , 0, M_DEFAULT) ;
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

			}

//			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
//			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
			if(Score) delete [] Score ;
	
			strLogFileBuffer.Format("[PAT_FIND_2] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]",
				ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_Pat_FindImage_2_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[PAT_FIND_2] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
		return false;
}




//===================================================================================
void CMatrox::ThresholdRectArea(int ID_No, long value, CRect rt)
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return ;
	MIL_ID tmpMilImage= M_NULL;
	CRect Rt;
	long xx= 0, yy= 0;
	
	if(rt.right< rt.left) {	xx= rt.left - rt.right; Rt.left= rt.right;	Rt.right= rt.left; }
	else				  {	xx= rt.right - rt.left; Rt.left= rt.left;	Rt.right= rt.right; }
	if(rt.bottom< rt.top) {	yy= rt.top - rt.bottom; Rt.top= rt.bottom;  Rt.bottom= rt.top;  }
	else				  {	yy= rt.bottom - rt.top; Rt.top= rt.top;		Rt.bottom= rt.bottom;  }
	
	MbufAlloc2d(MilSystem, xx, yy, 8+M_UNSIGNED, M_IMAGE+M_PROC, &tmpMilImage);
	
	MbufClear(MilOverlayImage[ID_No], TransparentColor); 
	
	MbufCopyClip(MilGrabImage[ID_No], tmpMilImage, -Rt.left, -Rt.top) ;
	
	MimBinarize(tmpMilImage, tmpMilImage, M_GREATER, value, NULL) ;
	
	MbufCopyClip(tmpMilImage, MilDisplayImage[ID_No], Rt.left, Rt.top) ;
	
	if(tmpMilImage)	{ MbufFree(tmpMilImage);	tmpMilImage= M_NULL; }
}

//===================================================================================
void CMatrox::MeasureProcess(CString strDirMeasImageSavePath, int Meas_no)
//-----------------------------------------------------------------------------------
{
}


//===================================================================================
void CMatrox::MeasureStartProcess()
//-----------------------------------------------------------------------------------
{
}

//===================================================================================
void CMatrox::Cam_ReferenceChange(int ch)
//-----------------------------------------------------------------------------------
{
/*	if(VISION_SYS_MODE== _DEV_MODE ) {
		MdigReference(MilDigitizer[ch], M_BLACK_REF , ModelFile()->System_data.BlackRef[ch] );
		MdigReference(MilDigitizer[ch], M_WHITE_REF , ModelFile()->System_data.WhiteRef[ch]  );
	}
*/
}

//===================================================================================
void CMatrox::Cam_ExposureChange(int ch)
//-----------------------------------------------------------------------------------
{
/*	double aaa= 0.0;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigInquire(MilDigitizer[0], M_GRAB_EXPOSURE_TIME, &aaa);//1016949/0.000 000 000
//		MdigInquire(MilDigitizer[1], M_GRAB_EXPOSURE_TIME, &aaa);

		if(ModelFile()->System_data.ExpTime[ch]!= 0.0)
			MdigControl(MilDigitizer[ch], M_GRAB_EXPOSURE_TIME, ModelFile()->System_data.ExpTime[ch]); //nsec

	}
*/
}
 
//===================================================================================
void CMatrox::Cam_InputGainChange(int ch)
//-----------------------------------------------------------------------------------
{
	double aaa= 0.0;
	if(VISION_SYS_MODE== _DEV_MODE ) {
//		MdigInquire(MilDigitizer[0], M_GRAB_EXPOSURE_TIME, &aaa);//1016949/0.000 000 000
//		MdigInquire(MilDigitizer[1], M_GRAB_EXPOSURE_TIME, &aaa);
//		MdigInquire(MilDigitizer[2], M_GRAB_EXPOSURE_TIME, &aaa);
//		MdigInquire(MilDigitizer[3], M_GRAB_EXPOSURE_TIME, &aaa);

//		MdigControl(MilDigitizer[ch], M_GRAB_INPUT_GAIN, ModelFile()->System_data.InputGain[ch]); //nsec
		MdigControl(MilDigitizer[_INSP_2_F_CAM_FILM], M_GRAB_INPUT_GAIN, 200.0); //ModelFile()->System_data.InputGain[ch]); //nsec

	}

}
 
//===================================================================================
void CMatrox::Cam_ExpTimeDelayChange()
//-----------------------------------------------------------------------------------
{
/*
	double aaa= 0.0;
	long bbb= 0;
	if(VISION_SYS_MODE== _DEV_MODE ) {
		MdigInquire(MilDigitizer[0], M_GRAB_EXPOSURE_TIME_DELAY + M_TIMER1, &aaa);//1016949/0.000 000 000
		MdigInquire(MilDigitizer[1], M_GRAB_EXPOSURE_TIME_DELAY + M_TIMER1, &bbb);
//MdigControl(MilDigitizer, M_GRAB_EXPOSURE_TIME_DELAY + M_TIMER1,  1 * 1000 * 1000); // 1 ms. time in nsec.
		if(g_Mark_Mode== _MARK_2_FIND) { //34
			MdigControl(MilDigitizer[0], M_GRAB_EXPOSURE_TIME_DELAY, 34.0); //nsec
			MdigControl(MilDigitizer[1], M_GRAB_EXPOSURE_TIME_DELAY, 34.0); //nsec
		}
		else {
			MdigControl(MilDigitizer[0], M_GRAB_EXPOSURE_TIME_DELAY,
				(double)(ModelFile()->System_data.iExpTime * 1000 * 1000)); //nsec
			MdigControl(MilDigitizer[1], M_GRAB_EXPOSURE_TIME_DELAY, 
				(double)(ModelFile()->System_data.iExpTime * 1000 * 1000)); //nsec
		}
	}
*/
}
//===================================================================================
void CMatrox::DrawOffsetCross_1()
//-----------------------------------------------------------------------------------
{
	CString text;

	int XX_L= 0;
	int YY_L= 0;
	int XX_R= 0;
	int YY_R= 0;
	double XX_L_gap= 0.0;
	double YY_L_gap= 0.0;
	double XX_R_gap= 0.0;
	double YY_R_gap= 0.0;

	MgraColor(M_DEFAULT, M_RGB888(255,100,100));

	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],
		0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y), _INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],
		(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), 0,	(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), _INSP_CAM_SIZE_Y);
	
	//-----------------------------------------------------
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],
		0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y), _INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],
		(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), _INSP_CAM_SIZE_Y);


}
//===================================================================================
void CMatrox::DrawOffsetCross_2()
//-----------------------------------------------------------------------------------
{
	CString text;

	int XX= 0;
	int YY= 0;

	XX= (ModelFile()->System_data.i2_CEN_R_ZOOM_X);// -
	YY= (ModelFile()->System_data.i2_CEN_R_ZOOM_Y);// -

	MgraColor(M_DEFAULT, M_RGB888(255,100,100));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_R_CAM_NORMAL],
		0L, (long)(YY), _INSP_CAM_SIZE_X, (long)(YY));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_R_CAM_NORMAL],
		(long)(XX), 0,	(long)(XX), _INSP_CAM_SIZE_Y);
}

void CMatrox::NORMAL_To_ZOOM_CAL(int B_X, int B_Y)
{
	int XX_L= 0;
	int YY_L= 0;
	int XX_R= 0;
	int YY_R= 0;
	double XX_L_gap= 0.0;
	double YY_L_gap= 0.0;
	double XX_R_gap= 0.0;
	double YY_R_gap= 0.0;

	double fact= ModelFile()->System_data.d2_Pixelmm_NORMAL / ModelFile()->System_data.d2_Pixelmm_ZOOM; 

	XX_R= B_X;// -
	YY_R= B_Y;// -

	XX_R_gap= XX_R - (_INSP_CAM_SIZE_X / 2.0);// - ModelFile()->System_data.i2_CAM_OFFSET_X;
	YY_R_gap= YY_R - (_INSP_CAM_SIZE_Y / 2.0);// - ModelFile()->System_data.i2_CAM_OFFSET_Y;
	
	XX_L= (int)( (_INSP_CAM_SIZE_X / 2.0) + (XX_R_gap * fact) );
	YY_L= (int)( (_INSP_CAM_SIZE_Y / 2.0) + (YY_R_gap * fact) );
	

	ModelFile()->System_data.i2_CEN_R_NORMAL_X= XX_R;
	ModelFile()->System_data.i2_CEN_F_PATTERN_Y= YY_R;
	ModelFile()->System_data.i2_CEN_R_ZOOM_X= XX_L ;
	ModelFile()->System_data.i2_CEN_R_NORMAL_Y= YY_L ;

	ModelFile()->SaveSytemConfigFileSemi(ModelFile()->System_data.strCurrentModelName);
}

void CMatrox::ZOOM_To_NORMAL_CAL(int B_X, int B_Y)
{
	int XX_L= 0;
	int YY_L= 0;
	int XX_R= 0;
	int YY_R= 0;
	double XX_L_gap= 0.0;
	double YY_L_gap= 0.0;
	double XX_R_gap= 0.0;
	double YY_R_gap= 0.0;

	double fact= ModelFile()->System_data.d2_Pixelmm_NORMAL / ModelFile()->System_data.d2_Pixelmm_ZOOM; 

	XX_R= (B_X);// -
	YY_R= (B_Y);// -

	XX_R_gap= XX_R - (_INSP_CAM_SIZE_X / 2.0);
	YY_R_gap= YY_R - (_INSP_CAM_SIZE_Y / 2.0);
	
//	XX_L= (int)( ((_INSP_CAM_SIZE_X / 2.0) + ModelFile()->System_data.i2_CAM_OFFSET_X) + (XX_R_gap / fact) );
//	YY_L= (int)( ((_INSP_CAM_SIZE_Y / 2.0) + ModelFile()->System_data.i2_CAM_OFFSET_Y) + (YY_R_gap / fact) );
	XX_L= (int)( ((_INSP_CAM_SIZE_X / 2.0) ) + (XX_R_gap / fact) );
	YY_L= (int)( ((_INSP_CAM_SIZE_Y / 2.0) ) + (YY_R_gap / fact) );
	

	ModelFile()->System_data.i2_CEN_R_ZOOM_X= XX_R;
	ModelFile()->System_data.i2_CEN_R_ZOOM_Y= YY_R;
	ModelFile()->System_data.i2_CEN_R_NORMAL_X= XX_L;
	ModelFile()->System_data.i2_CEN_R_NORMAL_Y= YY_L;

	ModelFile()->SaveSytemConfigFileSemi(ModelFile()->System_data.strCurrentModelName);
}


//===================================================================================
bool CMatrox::PatternFinderPos_Test(int i_no, double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
//-----------------------------------------------------------------------------------
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	CString strLogFileBuffer;
	int i;

	fnSave.Format ("%s\\%d년%d월%d일", strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_mkdir(fnSave);
	SaveFileName.Format ("%s\\Pat_FindImage_1_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
////////////////////////////////////////////////
	*angle= 0.;	*posx= 0.; *posy= 0.;

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	/* Reset the timer. */
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(MilDisplayImage[Cam_No], MilPatternModel_1[ModelNo], MilPatternResult_1[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
//Debug	TRACE("_1 The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult_1[ModelNo], M_NULL);
	
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
		 
	MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		if(number >= 1 && number< 5)
		{
			modPosx = new double[number] ;
			modPosy = new double[number] ;
			Score = new double[number] ;

      /* Get the results for each model. */
			MpatGetResult(MilPatternResult_1[ModelNo],  M_SCORE, Score);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_X, modPosx);
			MpatGetResult(MilPatternResult_1[ModelNo], M_POSITION_Y, modPosy);

			int  findno= 0;
			double minscore= 0.;
			if(number> 1) {
				for(i= 0; i< number; i++) {
					if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				}
			}
			*posx= modPosx[findno];
			*posy= modPosy[findno];
			*angle= Score[findno];

			double blobPosX= 0.;	double blobPosY= 0.;
			long cenx= 0, ceny= 0;
			cenx= (long)(*posx); ceny= (long)(*posy);

			int tmpLimit= 0;

			if(overlayflag) {
				CString str1 ;
				if(ModelNo % 2) {
					MgraColor(M_DEFAULT, M_RGB888(255,50,50));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 950L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 835L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
				}
				else	{
					MgraColor(M_DEFAULT, M_RGB888(100,100,255));
					MgraFontScale(M_DEFAULT, 1.5, 1.5); 
					str1.Format("%3.1f", Score[findno]);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
					str1.Format("X:%3.1f,Y:%3.1f", *posx, *posy);
					MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraText(M_DEFAULT, MilDisplayImage[Cam_No], 10L, 30L,   str1.GetBuffer(0));
					MgraFontScale(M_DEFAULT, 1.0, 1.0);
					MgraColor(M_DEFAULT, M_RGB888(50,255,50));
				}

				MgraFontScale(M_DEFAULT, 1.0, 1.0);

				long dist= 50;
//					MpatDraw(M_DEFAULT, MilDisplayImage[ModelNo], MilDisplayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX  , 0, M_DEFAULT) ;
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
				MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
				
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
					MgraLine(M_DEFAULT, MilDisplayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));

				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;

			}

//			if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
//			else			MgraColor(M_DEFAULT, M_RGB888(50,255,50));

			if(modPosx) delete [] modPosx ;
			if(modPosy) delete [] modPosy ;
			if(Score) delete [] Score ;
	
			strLogFileBuffer.Format("[PAT_FIND_1] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]", 
				ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			if(*angle<= 0.) *angle= 1.;

			return true;
		}
		else {
			*angle= 0.;			*posx= 0.; 			*posy= 0.; 
			if(errsaveflag) {
				SaveFileName.Format ("%s\\ERROR_Pat_FindImage_1_%02d%02d%02d", fnSave, st.wHour, st.wMinute, st.wSecond);
				aaa2.Format("%s[%d].tif", SaveFileName, ModelNo); 
				MbufSave(aaa2.GetBuffer (0), MilDisplayImage[Cam_No]);
			}
			if(overlayflag) {
				CString str1 ;
				MgraColor(M_DEFAULT, 200);
				MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
			}
			strLogFileBuffer.Format("[PAT_FIND_1] ERROR! No=%d", ModelNo); 
			SaveLogFile(strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return false;
		}
		return false;
}
//===================================================================================
void CMatrox::DrawText_Test(int ID_No, bool overlayclear, CPoint ptPoint, CString strText, int dR, int dG, int dB)
//-----------------------------------------------------------------------------------
{
	CString text;
	
	MgraFontScale(M_DEFAULT, 2.0, 2.0); 
	if(MilOverlayImage[ID_No]) {
		MgraColor(M_DEFAULT, M_RGB888(dR,dG,dB));
		MgraText(M_DEFAULT, MilOverlayImage[ID_No], ptPoint.x, ptPoint.y,   strText.GetBuffer(0));
		MgraText(M_DEFAULT, MilDisplayImage[ID_No], ptPoint.x, ptPoint.y,   strText.GetBuffer(0));
	}
	MgraFontScale(M_DEFAULT, 1.0, 1.0); 
}
//===================================================================================
void CMatrox::DrawDefaultCross_Test()
//-----------------------------------------------------------------------------------
{
	CString text;
	
	OverlayClear_1(_INSP_2_F_CAM_FILM);
	OverlayClear_1(_INSP_2_F_CAM_PATTERN);
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
	
	MgraColor(M_DEFAULT, M_RGB888(100,100,255));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y)+1,_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y)+1);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), 0,(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), _INSP_CAM_SIZE_Y);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_FILM],(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X)+1, 0,(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X)+1, _INSP_CAM_SIZE_Y);
	
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y)+1,_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y)+1);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), _INSP_CAM_SIZE_Y);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X)+1, 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X)+1, _INSP_CAM_SIZE_Y);

	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_FILM],0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y));
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_FILM],0L, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y)+1,_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_ROLL_Y)+1);
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_FILM],(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), 0,(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X), _INSP_CAM_SIZE_Y);
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_FILM],(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X)+1, 0,(long)(ModelFile()->System_data.i2_CEN_F_ROLL_X)+1, _INSP_CAM_SIZE_Y);
	
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y),_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y));
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_PATTERN],	0L, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y)+1,_INSP_CAM_SIZE_X, (long)(ModelFile()->System_data.i2_CEN_F_PATTERN_Y)+1);
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X), _INSP_CAM_SIZE_Y);
	MgraLine(M_DEFAULT, MilDisplayImage[_INSP_2_F_CAM_PATTERN],	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X)+1, 0,	(long)(ModelFile()->System_data.i2_CEN_F_PATTERN_X)+1, _INSP_CAM_SIZE_Y);
}
//===================================================================================
bool CMatrox::InspImageSave_Test(CString strName) 
//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	MIL_ID MilTestImageBuf;
	MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X*2, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilTestImageBuf);
	MbufClear(MilTestImageBuf, 0);

	MbufCopyClip(MilDisplayImage[_INSP_2_F_CAM_FILM], MilTestImageBuf, 0 , 0) ;
	MbufCopyClip(MilDisplayImage[_INSP_2_F_CAM_PATTERN], MilTestImageBuf, _INSP_CAM_SIZE_X , 0) ;

	MbufSave(strName.GetBuffer(0),  MilTestImageBuf);
	if(MilTestImageBuf) {			MbufFree(MilTestImageBuf);		MilTestImageBuf= M_NULL; }
	return true;
}

bool CMatrox::GetEdgeLine(int proc_count, CRect rect, double *pos_x)
{
	if(!Vision_Initial_Flag) return false;
	*pos_x= 0.0;
	//Blob----------------------------------------	
	long Max_Number= 0;
	long Max_Number2= 0;
	long area[100];
	long box_minx[100];
	long box_miny[100];
	long box_maxx[100];
	long box_maxy[100];
	long min_x[100];
	long max_x[100];
	CString text;
	
	MIL_ID tmpMilImage= M_NULL;
	MIL_ID tmpMilImage2= M_NULL;

	MbufCopyColor(MilDisplayImage[_INSP_2_F_CAM_PATTERN],  Insp_ColorImage[_INSP_2_F_CAM_PATTERN], M_ALL_BAND);

	MgraColor(M_DEFAULT, M_RGB888(255,100,100));
	MgraRect(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN], rect.left, rect.top, rect.right, rect.bottom);
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(_INSP_CAM_SIZE_X/2), (long)(0), _INSP_CAM_SIZE_X/2, (long)(_INSP_CAM_SIZE_Y));
	MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	0, (long)(_INSP_CAM_SIZE_Y/2), _INSP_CAM_SIZE_X, (long)(_INSP_CAM_SIZE_Y/2));

	MbufAlloc2d(MilSystem, rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &tmpMilImage);
	MbufAlloc2d(MilSystem, rect.Width(), rect.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &tmpMilImage2);
	MbufClear(tmpMilImage, 0);

	MbufCopyClip(MilDisplayImage[_INSP_2_F_CAM_PATTERN], tmpMilImage, -rect.left, -rect.top) ;
	MbufSave("D:\\bot_1.tif", tmpMilImage);

//	if(ThEnable== 1) {
//		if(ThAuto== 1) 
				MimBinarize(tmpMilImage, tmpMilImage, M_GREATER, M_DEFAULT, NULL) ;	
///		else	MimBinarize(tmpMilImage, tmpMilImage, M_GREATER, ThVal, NULL) ;	
//	}
//	else
//		MimBinarize(tmpMilImage, tmpMilImage, M_GREATER, M_DEFAULT, NULL) ;	


//	MimOpen(tmpMilImage, tmpMilImage, 2, M_BINARY); 

//	MblobAllocFeatureList(MilSystem, &DimFeatureList);
//	MblobAllocFeatureList(MilSystem, &DimFeatureList2);

	MbufCopy(tmpMilImage, tmpMilImage2);
	MimArith(tmpMilImage, M_NULL, tmpMilImage2, M_NOT );

	MblobSelectFeature(DimFeatureList, M_AREA); 
	MblobSelectFeature(DimFeatureList, M_BOX_X_MIN); 
// 	MblobSelectFeature(DimFeatureList, M_BOX_Y_MIN); 
	MblobSelectFeature(DimFeatureList, M_BOX_X_MAX);
//	MblobSelectFeature(DimFeatureList, M_BOX_Y_MAX); 

	MblobCalculate(tmpMilImage, M_NULL, DimFeatureList, DimBlobResult); 
	MblobCalculate(tmpMilImage2, M_NULL, DimFeatureList, DimBlobResult2); 

//	MblobSelect(DimBlobResult, M_DELETE, M_AREA, M_GREATER_OR_EQUAL, ModelFile()->System_data.iBlobDeleteVal, M_NULL);
	MblobSelect(DimBlobResult, M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, 
                                                 1500, M_NULL); 
	MblobSelect(DimBlobResult2, M_EXCLUDE, M_AREA, M_LESS_OR_EQUAL, 
		1500, M_NULL); 

//	MblobFill(DimBlobResult, Insp_BinarizeImage, M_EXCLUDED_BLOBS , 0); //M_ALL_BLOBS, 0);

	MblobGetNumber(DimBlobResult, &Max_Number);
	MblobGetNumber(DimBlobResult2, &Max_Number2);

	double Max_Y= 0.0;
	double Max_X= 0.0;

//	MbufSave("D:\\bot_2.tif", tmpMilImage);

	if(tmpMilImage)	{ MbufFree(tmpMilImage);	tmpMilImage= M_NULL; }

	if(Max_Number>= 1 && Max_Number2>= 1)
	{
		MblobGetResult(DimBlobResult, M_AREA+M_TYPE_LONG, area);
		MblobGetResult(DimBlobResult2, M_BOX_X_MIN+M_TYPE_LONG, box_minx);
//		MblobGetResult(DimBlobResult, M_BOX_Y_MIN+M_TYPE_LONG, box_miny);
		MblobGetResult(DimBlobResult, M_BOX_X_MAX+M_TYPE_LONG, box_maxx);
//		MblobGetResult(DimBlobResult, M_BOX_Y_MAX+M_TYPE_LONG, box_maxy);

		MgraColor(M_DEFAULT, M_RGB888(255,50,50));

		*pos_x= 0.0;

		double minx= 0.0;
		double maxx= 0.0;
		if(box_minx[0] > box_maxx[0])	
		{
			minx= box_maxx[0]; maxx= box_minx[0]; 
		}
		else							
		{
			maxx= box_maxx[0]; minx= box_minx[0]; 
		}

//		int aaa= rect.Height();
		MgraColor(M_DEFAULT, M_RGB888(100,100,100));
		MgraRect(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN], box_minx[0]+rect.left, box_miny[0]+rect.top, box_maxx[0]+rect.left, box_maxy[0]+rect.top);
		MgraRect(M_DEFAULT, Insp_ColorImage[_INSP_2_F_CAM_PATTERN], box_minx[0]+rect.left, box_miny[0]+rect.top, box_maxx[0]+rect.left, box_maxy[0]+rect.top);
		
		MgraColor(M_DEFAULT, M_RGB888(100,100,255));
		
//		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(Max_X), (long)(Max_Y), _INSP_CAM_SIZE_X, (long)(Max_Y));
//		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(Max_X), 0,	(long)(Max_X), (long)(Max_Y));
//		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(Max_X), (long)(Max_Y)+1, _INSP_CAM_SIZE_X, (long)(Max_Y)+1);
//		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(Max_X)+1, 0,	(long)(Max_X)+1, (long)(Max_Y));
		MgraArc(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN], (long)(minx)+rect.left , rect.top, 10, 10, 0, 360) ;
		MgraArc(M_DEFAULT, Insp_ColorImage[_INSP_2_F_CAM_PATTERN], (long)(minx)+rect.left , rect.top, 10, 10, 0, 360) ;
		MgraColor(M_DEFAULT, M_RGB888(100,255,100));
		MgraArc(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN], (long)(maxx)+rect.left , rect.bottom, 10, 10, 0, 360) ;
		MgraArc(M_DEFAULT, Insp_ColorImage[_INSP_2_F_CAM_PATTERN], (long)(maxx)+rect.left , rect.bottom, 10, 10, 0, 360) ;
		double xxxxx= 0.0;
		xxxxx= (fabs(maxx - minx)/2.0) + minx + rect.left;
		*pos_x= xxxxx;
		double x_cha= xxxxx - _INSP_CAM_SIZE_X/2;
		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(xxxxx), (long)(0), xxxxx, (long)(_INSP_CAM_SIZE_Y));
		MgraLine(M_DEFAULT, Insp_ColorImage[_INSP_2_F_CAM_PATTERN],	(long)(xxxxx), (long)(0), xxxxx, (long)(_INSP_CAM_SIZE_Y));
//		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(xxxxx-1)+rect.left, (long)(0), xxxxx-1+rect.left, (long)(_INSP_CAM_SIZE_Y));
//		MgraLine(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN],	(long)(xxxxx+1)+rect.left, (long)(0), xxxxx+1+rect.left, (long)(_INSP_CAM_SIZE_Y));
		text.Format("X:%.3f[%.3f] : %.3f[%.3f]", 
			*pos_x, *pos_x* ModelFile()->System_data.d2_Pixelmm_PATTERN,  
			x_cha, x_cha* ModelFile()->System_data.d2_Pixelmm_PATTERN);
		MgraFontScale(M_DEFAULT, 2.0, 2.0); 
		MgraText(M_DEFAULT, MilOverlayImage[_INSP_2_F_CAM_PATTERN], 100L, 30L,   text.GetBuffer(0));
		MgraText(M_DEFAULT, Insp_ColorImage[_INSP_2_F_CAM_PATTERN], 100L, 30L,   text.GetBuffer(0));
		*pos_x= x_cha * ModelFile()->System_data.d2_Pixelmm_PATTERN;

		CString strSaveText;
		strSaveText.Format("%s\\Image_%06d.jpg", g_strDirEdgeImageSavePath, proc_count);
		MbufSave(strSaveText.GetBuffer(0),  Insp_ColorImage[_INSP_2_F_CAM_PATTERN]);

		unsigned char *buf;
		buf = new unsigned char[1024*768*3] ;
//		MbufGet(Insp_ColorImage[_INSP_2_F_CAM_PATTERN], buf);
		MbufGetColor(Insp_ColorImage[_INSP_2_F_CAM_PATTERN], M_PACKED+M_BGR24 , M_ALL_BANDS, buf);
		ImageDataSave(buf, strSaveText);
		if(buf) delete [] buf ;


		return true;
	}
	//Blob-End----------------------------------------	
	*pos_x= -9999.999;
	return false;

}

void CMatrox::ImageDataSave(unsigned char *buf, CString csFileName)
{
	CxImage image; //image.Load("C:\\a.bmp", CXIMAGE_FORMAT_BMP);
				   /*CreateFromArray(BYTE *pArray, DWORD, dwWidth, DWORD dwHeight, DWORD dwBitsperpixel, DWORD dwBytesperline, bool bFlipImage)
				   pArray : 이미지 데이터
				   dwWidth : 가로
				   dwHeight : 세로
				   실제 테스트 해보니 가로, 세로 순서를 반대로 해야 정상적으로 출력됨
				   dwBitsperpixel : 한픽셀당 비트수
				   dwBytesperline : 정확한 의미는 잘 모르겠지만' 가로 길이 * 채널 수'
				   pFlipImage : true, false에 따라 이미지를 회전시킴
*/	
	if(image.CreateFromArray(buf, 
		1024, 768,
		24,
		1024 * 3,
		true))
		//		false))
	{
		image.SetJpegQuality(100);
		image.Save(csFileName.GetBuffer(0),CXIMAGE_FORMAT_JPG);
	}
}
