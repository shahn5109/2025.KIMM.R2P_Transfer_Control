// Matrox.cpp: implementation of the CMatrox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrox.h"
#include <direct.h>	// Dos Command. //
#include "stdio.h"
#include <math.h>
#include "Mainfrm.h"
#include "../Common/ImageView/Include/xGraphicObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSystemClass pSystem;
CParameterFile* CMatrox::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CMatrox::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

long MFTYPE FocusHookFunction(long HookType, long position,  void MPTYPE *UserDataPtr);

//===================================================================================
CMatrox::CMatrox()
//-----------------------------------------------------------------------------------
{
	int i;
	ModelSetHwnd= NULL;
	PatternSetHwnd= NULL;
	PatternSetHwnd2= NULL;

	Model_AllocSizeX= 10L;
	Model_AllocSizeY= 10L;
	Pattern_AllocSizeX= 10L;
	Pattern_AllocSizeY= 10L;
	Pattern_AllocSizeX2= 10L;
	Pattern_AllocSizeY2= 10L;

	for(i= 0 ; i< 2; i++)	{		m_bExec[i] = FALSE; m_pThread[i] = NULL;	}

	MilSystem = M_NULL;

	for(i= 0; i< _MAX_VIEW_; i++) {
		MilDigitizer[i]= M_NULL;
		MilGrabImage[i]= M_NULL;
		Proc_Tmp_MilImage[i]= M_NULL;
	}
	for(i= 0; i< _MAX_TMPVIEW_; i++) {
		Insp_Tmp_MilImage[i]= M_NULL;
	}
	TransparentColor= NULL;        // Keying color
	for(i= 0; i< _MAX_AF_BUF_COUNT; i++)
	{
		MilFocusImage[i] = NULL;
	}
	MilAutoFocusImage = NULL;

	Insp_OriginalImage= M_NULL;
	Insp_BinarizeImage= M_NULL;

	PatternSetDisplay= M_NULL;
	PatternSetDispImage= M_NULL;
	PatternDontCareImage= M_NULL;
	PatternSetOverlayImage= M_NULL;
	PatternSetDisplay2= M_NULL;
	PatternSetDispImage2= M_NULL;
	PatternDontCareImage2= M_NULL;
	PatternSetOverlayImage2= M_NULL;
	for( i= 0; i< _MAX_FIND_MODEL; i++) {
		MilPatternModel[i]= M_NULL;
		MilPatternResult[i]= M_NULL;
	}

	MilTmp2ColorBuffer= MilTmpColorBuffer= M_NULL;

	Vision_Initial_Flag = ERR_SYSTEM_INITIAL_ERROR;
	Vision_DIg_Initial_Flag = ERR_SYSTEM_INITIAL_ERROR;
	Vision_VGA_Initial_Flag = false;

	m_strLogFileBuffer= _T("");
	m_iLogLevel= _LOG_DETAIL;
	m_VISION_SYS_MODE= _DEV_MODE;
	m_Vision_Grab_Mode= _INSP_CAM_GRAB;

	MilEdgeContext = M_NULL;
	MilEdgeResult = M_NULL;

	m_rcROI.SetRectEmpty();

	m_nSelectedCamForLive = -1;

	m_pImageCtrl = NULL;

	mSel_TopCam_No= e2X_LENS; mSel_BotCam_No= e2X_LENS;

	AF_GrabCamera= 0; AF_GrabCount= 0;
	AF_GrabStart= false; AF_ChChg_Flag= false;

}
//===================================================================================
CMatrox::~CMatrox()
//-----------------------------------------------------------------------------------
{

}
//===================================================================================
void CMatrox::InitViewer( CWnd* pParent )
{
	m_ImageCtrl[0].Create( &m_ImgCtrlMng, pParent );
	m_ImageCtrl[1].Create( &m_ImgCtrlMng, pParent );
	m_ImageCtrl[2].Create( &m_ImgCtrlMng, pParent );
	m_ImageCtrl[3].Create( &m_ImgCtrlMng, pParent );

	m_ImgObj[0].Create( _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8 );
	m_ImgObj[1].Create( _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8 );
	m_ImgObj[2].Create( _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8 );
	m_ImgObj[3].Create( _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8 );

	m_ImageCtrl[0].SetImageObject( &m_ImgObj[0] );
	m_ImageCtrl[1].SetImageObject( &m_ImgObj[1] );
	m_ImageCtrl[2].SetImageObject( &m_ImgObj[2] );
	m_ImageCtrl[3].SetImageObject( &m_ImgObj[3] );

	SetCameraResolution();
}
//===================================================================================
void CMatrox::SetCameraResolution()
{
	if( m_ImgObj[eTOP_CAM_20X].isValid() )
	{
		m_ImageCtrl[eTOP_CAM_20X].SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_20X) );
		m_ImageCtrl[eTOP_CAM_2X].SetRealSizePerPixel( (float)(ParamFile()->System_data.dTop_Cam_PixelToum_2X) );
		m_ImageCtrl[eBTM_CAM_20X].SetRealSizePerPixel( (float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
		m_ImageCtrl[eBTM_CAM_2X].SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_2X) );
	}
}
//===================================================================================
void CMatrox::GetImageViewer( CxImageViewCtrl*& imgCtrl )
{
	imgCtrl = m_ImageCtrl;
}
//===================================================================================
CxImageViewCtrl* CMatrox::GetImageViewer( int nViewIdx )
{
	return &m_ImageCtrl[nViewIdx];
}
//===================================================================================
void CMatrox::GetImageObj( CImageObj*& ImgObj )
{
	ImgObj = m_ImgObj;
}
//===================================================================================
CImageObj* CMatrox::GetImageObj( int nViewIdx )
{
	if( !m_ImgObj[nViewIdx].isValid() )
		return NULL;

	if(m_VISION_SYS_MODE == _DEV_MODE)	
			return &m_ImgObj[nViewIdx];//m_ImageCtrl[nViewIdx].GetVisibleImageObject();
	else	return m_ImageCtrl[nViewIdx].GetVisibleImageObject();
}
//===================================================================================
void CMatrox::SetTrackerMode( int nViewIdx, BOOL bTracking )
{
	m_ImageCtrl[nViewIdx].SetTrackerMode( bTracking );
}

void CMatrox::SetROIEmpty()
{
	m_rcROI.SetRectEmpty();
}

void CMatrox::SetROI( CRect& rcRect )
{
	m_rcROI = rcRect;
}
CRect CMatrox::GetROI()
{
	return m_rcROI;
}

void CMatrox::SetTempImageCtrl(  CxImageViewCtrl* pView  )
{
	m_pImageCtrl = pView;
}
void CMatrox::DrawUpdate(int DispNo)
{
	m_ImageCtrl[DispNo].ImageUpdate();
}

//===================================================================================
bool CMatrox::Open()
{
	int i= 0, j= 0;

	m_main_dcf_file.Format( _T("%s\\%s"),ParamFile()->strDirConfigPath, _INSP_CAM_DCF);

	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
	Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Allocate a MIL application. 
	m_strLogFileBuffer.Format(_T("=================================================================="));	SaveLogFile(m_strLogFileBuffer);
	m_strLogFileBuffer.Format(_T("CMATROX-Class: Vision-Open Start!!!"));		SaveLogFile(m_strLogFileBuffer);

	MappAlloc(M_DEFAULT, &MilApplication);
	//	MappControl(M_ERROR, M_PRINT_DISABLE);	m_strLogFileBuffer.Format(_T("M_ERROR-Display -> DISABLE!");		SaveLogFile(m_strLogFileBuffer);
	m_strLogFileBuffer.Format(_T("M_ERROR-Display -> ENABLE!"));		SaveLogFile(m_strLogFileBuffer);

	Vision_VGA_Initial_Flag= false;
	MilSystem = NULL; Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;

	if(m_VISION_SYS_MODE == _DEV_MODE)	
	{   
		MsysAlloc(M_SYSTEM_METEOR_II, M_DEFAULT, M_DEFAULT, &MilSystem); 
		if(MilSystem == NULL) m_VISION_SYS_MODE= _VGA_MODE;
	}
	if(m_VISION_SYS_MODE == _VGA_MODE)
		MsysAlloc(M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT/*M_NO_DDRAW*/, &MilSystem);

	if(m_VISION_SYS_MODE == _VGA_MODE) 
	{
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
		m_strLogFileBuffer.Format(_T("MsysAlloc(M_SYSTEM_VGA,,,)"));			SaveLogFile(m_strLogFileBuffer);
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
	}
	else 
	{
		m_strLogFileBuffer.Format(_T("MsysAlloc(M_SYSTEM_METEOR_II,,,)"));	SaveLogFile(m_strLogFileBuffer);
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
	}

	if(MilApplication == NULL)	
	{ 
		m_strLogFileBuffer.Format(_T("MilApplication Alloc Failed~!"));	
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
		SaveLogFile(m_strLogFileBuffer); AfxMessageBox(m_strLogFileBuffer) ;	return false ; 
	}
	if(MilSystem == NULL)		
	{ 
		m_strLogFileBuffer.Format(_T("System Alloc Failed~!"));		
		Vision_Initial_Flag= ERR_SYSTEM_INITIAL_OK;
		SaveLogFile(m_strLogFileBuffer); AfxMessageBox(m_strLogFileBuffer) ;	return false ; 
	}

	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

	if(m_VISION_SYS_MODE == _DEV_MODE ) 
	{
		if(MsysInquire(MilSystem, M_DIGITIZER_NUM, M_NULL) ) 
		{
			if(Vision_Initial_Flag== ERR_SYSTEM_INITIAL_OK)	
			{
#ifdef _KIMM_TRANSFER_VER1_SYSTEM_INFO_H
				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Roll\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eTOP_CAM_20X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eTOP_CAM_20X);
				SaveLogFile(m_strLogFileBuffer);

				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Roll\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eTOP_CAM_2X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eTOP_CAM_2X);
				SaveLogFile(m_strLogFileBuffer);

				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Roll\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eBTM_CAM_20X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eBTM_CAM_20X);
				SaveLogFile(m_strLogFileBuffer);

				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Roll\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eBTM_CAM_2X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eBTM_CAM_2X);
				SaveLogFile(m_strLogFileBuffer);
#else
				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Plate\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eTOP_CAM_20X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eTOP_CAM_20X);
				SaveLogFile(m_strLogFileBuffer);

				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Plate\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eTOP_CAM_2X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eTOP_CAM_2X);
				SaveLogFile(m_strLogFileBuffer);

				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Plate\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eBTM_CAM_20X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eBTM_CAM_20X);
				SaveLogFile(m_strLogFileBuffer);

				MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT( "D:\\Transfer_Program&Config\\Config_Plate\\VCC-g20s20_NINT_HV.dcf"), M_DEFAULT, &MilDigitizer[eBTM_CAM_2X]);
				m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Complite!"), m_main_dcf_file.GetBuffer(0), eBTM_CAM_2X);
				SaveLogFile(m_strLogFileBuffer);
#endif
				Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_OK;

				if(MilDigitizer[eTOP_CAM_20X]== NULL) 
				{
					MilDigitizer[eTOP_CAM_20X]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!"), m_main_dcf_file.GetBuffer(0), eTOP_CAM_20X);
					SaveLogFile(m_strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}
				if(MilDigitizer[eTOP_CAM_2X]== NULL) 
				{
					MilDigitizer[eTOP_CAM_2X]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!"), m_main_dcf_file.GetBuffer(0), eTOP_CAM_2X);
					SaveLogFile(m_strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}

				if(MilDigitizer[eBTM_CAM_20X]== NULL) 
				{
					MilDigitizer[eBTM_CAM_20X]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!"), m_main_dcf_file.GetBuffer(0), eBTM_CAM_20X);
					SaveLogFile(m_strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}

				if(MilDigitizer[eBTM_CAM_2X]== NULL) 
				{
					MilDigitizer[eBTM_CAM_2X]= NULL; //MdigAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &MilDigitizer);
					m_strLogFileBuffer.Format(_T("MdigAlloc(,,%s,,) ---> Digitizer[%d] Fail!"), m_main_dcf_file.GetBuffer(0), eBTM_CAM_2X);
					SaveLogFile(m_strLogFileBuffer);
					Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				}

				if(MilDigitizer[eTOP_CAM_20X]!= NULL && MilDigitizer[eTOP_CAM_2X]!= NULL && MilDigitizer[eBTM_CAM_20X]!= NULL && MilDigitizer[eBTM_CAM_2X]!= NULL) 
				{
					MdigChannel(MilDigitizer[eTOP_CAM_20X], M_CH0); //AHN);
					MdigControl(MilDigitizer[eTOP_CAM_20X], M_GRAB_TIMEOUT,500);					 //mSec

					MdigChannel(MilDigitizer[eTOP_CAM_2X], M_CH1);//);
					MdigControl(MilDigitizer[eTOP_CAM_2X], M_GRAB_TIMEOUT,500);					 //mSec

					MdigChannel(MilDigitizer[eBTM_CAM_20X], M_CH4); //AHN);
					MdigControl(MilDigitizer[eBTM_CAM_20X], M_GRAB_TIMEOUT,500);					 //mSec

					MdigChannel(MilDigitizer[eBTM_CAM_2X], M_CH5); //AHN);
					MdigControl(MilDigitizer[eBTM_CAM_2X], M_GRAB_TIMEOUT,500);					 //mSec

					if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[eTOP_CAM_20X]))
					{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}
					else		MdigHalt(MilDigitizer[eTOP_CAM_20X]);

					if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[eTOP_CAM_2X]))
					{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}
					else		MdigHalt(MilDigitizer[eTOP_CAM_20X]);

					if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[eBTM_CAM_20X]))
					{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}
					else		MdigHalt(MilDigitizer[eTOP_CAM_20X]);

					if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_PROC+M_NON_PAGED, &MilGrabImage[eBTM_CAM_2X]))
					{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}
					else		MdigHalt(MilDigitizer[eTOP_CAM_20X]);

					for(i= 0; i < _MAX_AF_BUF_COUNT; i++)
					{
						MilFocusImage[i] = NULL;
						MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilFocusImage[i]);
					}
					MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAutoFocusImage);
					MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilSubImage);
				}
			}
		}
		else
		{
			if(m_VISION_SYS_MODE== _DEV_MODE ) {
				m_strLogFileBuffer.Format(_T("Digitizer를 찾을 수 없습니다. ~!"));	SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(m_strLogFileBuffer) ;
				for(i= 0; i< _MAX_CAM_; i++) 	MilDigitizer[i]= NULL;
				m_strLogFileBuffer.Format(_T("VISION-Initialize ERROR!"));			SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(m_strLogFileBuffer) ;
				Vision_DIg_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
				return true;
			}
		}
	}
	else
	{
		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilGrabImage[eTOP_CAM_20X]))
		{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}

		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilGrabImage[eTOP_CAM_2X]))
		{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}

		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilGrabImage[eBTM_CAM_20X]))
		{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}

		if (!MbufAlloc2d(MilSystem, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilGrabImage[eBTM_CAM_2X]))
		{	m_strLogFileBuffer.Format(_T("MbufAllocColor(MilGrabImageColor) - Error!")); SaveLogFile(m_strLogFileBuffer);	return FALSE;	}

		for(i= 0; i < _MAX_AF_BUF_COUNT; i++)
		{
			MilFocusImage[i] = NULL;
			MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilFocusImage[i]);
		}
		MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilAutoFocusImage);
		MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilSubImage);
	}

	for(i= 0; i< _MAX_VIEW_; i++) 
		MbufAlloc2d(MilSystem, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &Proc_Tmp_MilImage[i]);
	for(i= 0; i< _MAX_FIND_MODEL; i++) 
	{
		MpatAllocResult(MilSystem, M_DEFAULT, &MilPatternResult[i]);         
	}

	RunThread( 0 );
//	m_vMilFocusImg.clear();

	//////////////////////////////////////////////////////////////////////////
	// 모델 이미지가 있다면 Loading.
//	ModelImageLoad();
//	PatternImageLoad();

	return true;
}
//===================================================================================
void CMatrox::Close()
{
	int i;	

	MappControl(M_ERROR, M_PRINT_DISABLE);	m_strLogFileBuffer.Format(_T("M_ERROR-Display -> DISABLE!"));		SaveLogFile(m_strLogFileBuffer);
	if(Vision_Initial_Flag && m_VISION_SYS_MODE== _DEV_MODE)
		KillThread(0);

	InspGrab(eTOP_CAM_20X); 
	InspGrab(eTOP_CAM_2X); 	

	//////////////////////////////////////////////////////////////////////////
	// For Model Finder
	for( i= 0; i< _MAX_FIND_MODEL; i++) {
		if(MilSearchContext[i])			{ MmodFree(MilSearchContext[i]);				MilSearchContext[i]= M_NULL; }
		if(MilModelResult[i])			{ MmodFree(MilModelResult[i]);					MilModelResult[i]= M_NULL; }
		if( MilSearchContext_Mask[i] )	{ MmodFree(MilSearchContext_Mask[i]);		MilSearchContext_Mask[i] = M_NULL; }
	}
	//
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	for( i= 0; i< _MAX_FIND_MODEL; i++) {
		if(MilPatternModel[i])		{ MpatFree(MilPatternModel[i]);						MilPatternModel[i]= M_NULL; }
		if(MilPatternResult[i])		{ MpatFree(MilPatternResult[i]);					MilPatternResult[i]= M_NULL; }
	}
	if(PatternSetDisplay)					MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(PatternSetDisplay)					{ MdispDeselect(PatternSetDisplay,PatternSetDispImage); }
	if(PatternSetDispImage)					{ MbufFree(PatternSetDispImage) ; PatternSetDispImage= M_NULL; }
	if(PatternDontCareImage)				{ MbufFree(PatternDontCareImage) ; PatternDontCareImage= M_NULL; }
	if(PatternSetDisplay)					{ MdispFree(PatternSetDisplay);	PatternSetDisplay= M_NULL; }

	if(PatternSetDisplay2)					MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_DISABLE);
	if(PatternSetDisplay2)					{ MdispDeselect(PatternSetDisplay2,PatternSetDispImage2); }
	if(PatternSetDispImage2)				{ MbufFree(PatternSetDispImage2) ; PatternSetDispImage2= M_NULL; }
	if(PatternDontCareImage2)				{ MbufFree(PatternDontCareImage2) ; PatternDontCareImage2= M_NULL; }
	if(PatternSetDisplay2)					{ MdispFree(PatternSetDisplay2);	PatternSetDisplay2= M_NULL; }
	////////////////////////////////////////////////////////////////
	if(MilTmpColorBuffer)	{				MbufFree(MilTmpColorBuffer);		MilTmpColorBuffer= M_NULL; }
	if(MilTmp2ColorBuffer)	{				MbufFree(MilTmp2ColorBuffer);		MilTmp2ColorBuffer= M_NULL; }


	for(i= 0; i< _MAX_TMPVIEW_; i++) {
		if(Insp_Tmp_MilImage[i]) {			MbufFree(Insp_Tmp_MilImage[i]);		Insp_Tmp_MilImage[i]= M_NULL; }
	}
	if(Insp_BinarizeImage)	{				MbufFree(Insp_BinarizeImage);		Insp_BinarizeImage= M_NULL; }
	if(Insp_OriginalImage)	{				MbufFree(Insp_OriginalImage);		Insp_OriginalImage= M_NULL; }

	for(i= 0; i< _MAX_AF_BUF_COUNT; i++) {
		if(MilFocusImage[i]) {				MbufFree(MilFocusImage[i]);		MilFocusImage[i]= M_NULL; }
	}
	if(MilAutoFocusImage)	{		MbufFree(MilAutoFocusImage);	MilAutoFocusImage = NULL;	}
	if(MilSubImage)			{		MbufFree(MilSubImage);			MilSubImage = NULL;	}

	//	MgraFree(MilGraphicContext);

	for(i= 0; i< _MAX_VIEW_; i++) 	{ 
		if(MilGrabImage[i])	{				MbufFree(MilGrabImage[i]);		MilGrabImage[i]= M_NULL; }
		if(Proc_Tmp_MilImage[i])	{				MbufFree(Proc_Tmp_MilImage[i]);		Proc_Tmp_MilImage[i]= M_NULL; }
}

	for(i= 0; i< _MAX_CAM_; i++) {
		if(MilDigitizer[i])			{
			MdigControl(MilDigitizer[i], M_CAMERA_LOCK, M_DISABLE);		Sleep(50);
			MdigFree(MilDigitizer[i]);		MilDigitizer[i]= 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Clear Auto Focus Image buffer
	ClearAutoFocusImage();

	if( MilEdgeContext )	MedgeFree( MilEdgeContext );
	if( MilEdgeResult )		MedgeFree( MilEdgeResult );

	if(MilSystem)			{				MsysFree(MilSystem);				MilSystem= M_NULL; }
	if(MilApplication)		{				MappFree(MilApplication);			MilApplication= M_NULL; }
	//MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);
	/* Free defaults. */
	//  MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);

	Vision_Initial_Flag= ERR_SYSTEM_INITIAL_ERROR;
	m_strLogFileBuffer.Format(_T("CMATROX-Class: VISION Close!")); SaveLogFile(m_strLogFileBuffer);

	m_ImgObj[0].Destroy();
	m_ImgObj[1].Destroy();
	m_ImgObj[2].Destroy();
	m_ImgObj[3].Destroy();
}
//===================================================================================
void CMatrox::SaveLogFile(CString strText)
{
	TCHAR value[512];
	CString strFile__Name;
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString m_strDayLogDirName;
	m_strDayLogDirName.Format(_T("%s\\%4d%02d%02d"), ParamFile()->strDirVisionLogPath, st.wYear, st.wMonth, st.wDay); // File Name 생성

	USES_CONVERSION;
	CreateFolder( m_strDayLogDirName ); // Day Loge Directory 생성. //

	strFile__Name.Format(_T("%s\\Vision.Log"), m_strDayLogDirName);
	swprintf_s(value, _T("%s"), strText);

	errno_t err;
	FILE* fout;
	err = _tfopen_s(&fout, strFile__Name, _T("a+") );   // 파일에 계속 추가시킨다. //
	if(err != 0) 	{		return;		}

	CString strMsg;
	strMsg.Format (_T("[%02ld:%02ld:%02ld]\t%s"), st.wHour, st.wMinute, st.wSecond, value); 
	fwprintf(fout, _T("%s\n"), strMsg);
	fclose(fout);
}

//===================================================================================
void CMatrox::InitModelSet_1(HWND hwnd)
//-----------------------------------------------------------------------------------
{
	// 	if(PatternSetDisplay)		{ MdispDeselect(PatternSetDisplay,PatternSetDispImage); }
	// 	if(PatternSetDisplay)		{ MdispFree(PatternSetDisplay);PatternSetDisplay= 0; }
	// 	
	// 	if(PatternDontCareImage)	{ MbufFree(PatternDontCareImage) ; PatternDontCareImage= 0; }
	// 	if(PatternSetDispImage)	{ MbufFree(PatternSetDispImage) ; PatternSetDispImage= 0; }

	if(PatternSetDisplay== M_NULL) MdispAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &PatternSetDisplay) ;
	if(PatternDontCareImage== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX,	Pattern_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage);
	if(PatternSetDispImage== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX,	Pattern_AllocSizeY,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage);

	MdispSelectWindow(PatternSetDisplay, PatternSetDispImage, hwnd) ;

	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternSetOverlayImage, TransparentColor); 
	MbufClear(PatternSetDispImage, 0);

//	MdispZoom(PatternSetDisplay , _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);
//	MdispZoom(PatternSetDisplay , _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	PatternSetHwnd= hwnd;
}
//===================================================================================
void CMatrox::InitModelSet_2(HWND hwnd)
	//-----------------------------------------------------------------------------------
{
	// 	if(PatternSetDisplay)		{ MdispDeselect(PatternSetDisplay,PatternSetDispImage); }
	// 	if(PatternSetDisplay)		{ MdispFree(PatternSetDisplay);PatternSetDisplay= 0; }
	// 	
	// 	if(PatternDontCareImage)	{ MbufFree(PatternDontCareImage) ; PatternDontCareImage= 0; }
	// 	if(PatternSetDispImage)	{ MbufFree(PatternSetDispImage) ; PatternSetDispImage= 0; }

	if(PatternSetDisplay2== M_NULL) MdispAlloc(MilSystem, M_DEFAULT, M_DISPLAY_SETUP, M_DEFAULT, &PatternSetDisplay2) ;
	if(PatternDontCareImage2== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX2,	Pattern_AllocSizeY2,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage2);
	if(PatternSetDispImage2== M_NULL) MbufAlloc2d(MilSystem, Pattern_AllocSizeX2,	Pattern_AllocSizeY2,   8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage2);

	MdispSelectWindow(PatternSetDisplay2, PatternSetDispImage2, hwnd) ;

	//--------
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage2);
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage2);
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternSetOverlayImage2, TransparentColor); 
	MbufClear(PatternSetDispImage2, 0);

	//	MdispZoom(PatternSetDisplay , _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);
	//	MdispZoom(PatternSetDisplay , _DISP_ZOOM_FACTOR, _DISP_ZOOM_FACTOR);

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	PatternSetHwnd2= hwnd;
}

//===================================================================================
void CMatrox::ClearAutoFocusImage()
{
/*
	for( UINT i = 0; i < m_vMilFocusImg.size(); ++i )
	{
		if( m_vMilFocusImg[i] )
			MbufFree( m_vMilFocusImg[i] );
	}

	m_vMilFocusImg.clear();
*/
}
//============================================================================================
void CMatrox::RunThread(int nID)
{
	m_bExec[nID] = TRUE; 
	THREADPAPAMS *pThreadParams = new THREADPAPAMS;
	pThreadParams->pContinue = &m_bExec[nID];
	pThreadParams->pWnd =  ((CMatrox*)this);

	if(m_pThread[nID] == NULL) {
		switch(nID) {
		case 0: 
			m_pThread[nID] = AfxBeginThread(ThreadFuncLiveProc, pThreadParams);  break;
		}
	}
	m_pThread[nID]->m_bAutoDelete = true;
}
//============================================================================================
void CMatrox::KillThread(int nID)
{
	if(m_pThread[nID] != NULL)	{
		HANDLE hThread = m_pThread[nID]->m_hThread;
		m_bExec[nID] = FALSE;
		::WaitForSingleObject(hThread,1000);
		TerminateThread(m_pThread[nID]->m_hThread, 0);
		m_pThread[nID] = NULL;
	}
}
//============================================================================================
UINT CMatrox::ThreadFuncLiveProc(LPVOID pParam)
{
	//넘어온 인자를 복사
	THREADPAPAMS *pThreadParams = (THREADPAPAMS *)pParam;
	CMatrox *pWnd = pThreadParams->pWnd;//THREADPAPAMS_Mag1;
	BOOL *pContinue = pThreadParams->pContinue;
	delete pThreadParams;

	CMatrox* pOwner =  (CMatrox*)pWnd ;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	int ch_Count= 0;
	DWORD st1;
	while(*pContinue)
	{
		if(!pFrm->isView_Init()) { Sleep(100); continue; }

		if(pOwner->Vision_Initial_Flag && pOwner->m_VISION_SYS_MODE== _DEV_MODE) 
		{
			if(pOwner->AF_GrabStart)
			{
				pOwner->AF_End_Flag= true;
				pOwner->InspLiveUseGrabChChange(pOwner->AF_GrabCamera);
				pOwner->AF_BufferToProcBuffer(pOwner->AF_GrabCamera, pOwner->AF_GrabCount);
				if(pOwner->AF_GrabCount< pOwner->ParamFile()->System_data.iAF_TotalCount) 	pOwner->AF_GrabCount++;
				else
				{
					pOwner->AF_GrabStart= false; ch_Count= 0; pOwner->AF_ChChg_Flag= false; pOwner->AF_End_Flag= false;
				}
				TRACE(_T("[%d] A/F => %d/%d  [ %d ms]\n"), pOwner->AF_GrabCamera, pOwner->AF_GrabCount, _MAX_AF_BUF_COUNT, GetTickCount()- st1);
			}
			else
			{
				 pOwner->AF_ChChg_Flag= false; pOwner->AF_End_Flag= false;
				if(pOwner->m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) 
				{
					if(pOwner->MilDigitizer[eTOP_CAM_2X]  && pOwner->MilGrabImage[eTOP_CAM_20X] &&
						pOwner->MilDigitizer[eBTM_CAM_2X]  && pOwner->MilGrabImage[eBTM_CAM_20X])
					{
						if(pOwner->AF_ChChg_Flag)
						{
							pOwner->InspLiveUseGrabChChange(pOwner->AF_GrabCamera);
						}
						else 
						{
							st1= GetTickCount();
							switch(ch_Count)
							{
							case 0:						
								{
									if(pOwner->mSel_TopCam_No== e2X_LENS)
									{
										if(pOwner->m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) 
										{ 
											pOwner->InspLiveUseGrabChChange(eTOP_CAM_2X);
											Sleep(0); 
											pOwner->InspLiveUseGrab(eTOP_CAM_2X);
										} 
									}
									else
									{
										if(pOwner->m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) 
										{ 
											pOwner->InspLiveUseGrabChChange(eTOP_CAM_20X);
											Sleep(0); 
											pOwner->InspLiveUseGrab(eTOP_CAM_20X);
										} 
									}

									if(pOwner->mSel_BotCam_No== e2X_LENS)
									{			pOwner->InspLiveUseGrabChChange(eBTM_CAM_2X);			}
									else
									{			pOwner->InspLiveUseGrabChChange(eBTM_CAM_20X);			}
									ch_Count= 1; 
								}
								break;
							case 1:	
								{

									if(pOwner->mSel_BotCam_No== e2X_LENS)
									{
										if(pOwner->m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) 
										{ 
											pOwner->InspLiveUseGrabChChange(eBTM_CAM_2X);
											Sleep(0); 
											pOwner->InspLiveUseGrab(eBTM_CAM_2X);
										} 
									}
									else
									{
										if(pOwner->m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) 
										{ 
											pOwner->InspLiveUseGrabChChange(eBTM_CAM_20X);
											Sleep(0); 
											pOwner->InspLiveUseGrab(eBTM_CAM_20X);
										} 
									}

									if(pOwner->mSel_TopCam_No== e2X_LENS)
									{			pOwner->InspLiveUseGrabChChange(eTOP_CAM_2X);			}
									else
									{			pOwner->InspLiveUseGrabChChange(eTOP_CAM_20X);			}
									ch_Count= 0; 
									pOwner->AF_End_Flag= false;
								}
								break;
							default:					ch_Count= 0; break;
							}

						}
					}
				}
			}
		}
		Sleep(10);
	}
	return 0;
}
//===================================================================================
bool CMatrox::AF_ProcStart(int ch) 
	//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	if(m_VISION_SYS_MODE== _DEV_MODE )
	{
		switch(ch)
		{
		case eTOP_CAM_20X:	MdigChannel(MilDigitizer[ch], M_CH0);			break;
		case eTOP_CAM_2X:	MdigChannel(MilDigitizer[ch], M_CH1);			break;
		case eBTM_CAM_20X:	MdigChannel(MilDigitizer[ch], M_CH4);			break;
		case eBTM_CAM_2X:	MdigChannel(MilDigitizer[ch], M_CH5);			break;
		}
//		m_Vision_Grab_Mode= _INSP_CAM_GRAB_CONTINUE;
		AF_GrabCamera= ch; AF_GrabCount= 0;
		AF_ChChg_Flag= true;

		Sleep(500);

		AF_GrabStart= true;
	}
	return true;
}
//===================================================================================
bool CMatrox::AF_ProcStop() 
	//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	if(m_VISION_SYS_MODE== _DEV_MODE )
	{
		AF_GrabCamera= 0; AF_GrabCount= 0;
		AF_GrabStart= false; AF_ChChg_Flag= false;
		m_Vision_Grab_Mode= _INSP_CAM_GRAB_CONTINUE;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Autofocus hook function that is responsible to move the lens */
/****************************************************************/
long MFTYPE FocusHookFunction(long HookType, long position,  void MPTYPE *UserDataPtr)
{
	/* Focus position must be changed     */
	if(HookType == M_CHANGE || HookType == M_ON_FOCUS)
	{
		TRACE("s-FocusHookFunction=%d      :%d", position, (((long*)UserDataPtr)[2]));
		(((long*)UserDataPtr)[2])++;

		MbufCopy(pSystem.GetVisionComponent()->MilFocusImage[position], pSystem.GetVisionComponent()->MilAutoFocusImage);
		MbufCopy(pSystem.GetVisionComponent()->MilFocusImage[position], pSystem.GetVisionComponent()->MilSubImage);
		TRACE("e-FocusHookFunction=%d      :%d\n", position, (((long*)UserDataPtr)[2]));

	}
	Sleep(0);

	return 0;
}

void CMatrox::AF_BufferToProcBuffer(int Camno, int cnt)
{
/*
	switch(ch)
	{
	case eTOP_CAM_20X:	MdigChannel(MilDigitizer[ch], M_CH0);			break;
	case eTOP_CAM_2X:	MdigChannel(MilDigitizer[ch], M_CH1);			break;
	case eBTM_CAM_20X:	MdigChannel(MilDigitizer[ch], M_CH4);			break;
	case eBTM_CAM_2X:	MdigChannel(MilDigitizer[ch], M_CH5);			break;
	}
*/
	MdigGrab(MilDigitizer[Camno], MilGrabImage[Camno]);
	if(Camno== eTOP_CAM_20X || Camno== eTOP_CAM_2X)		Z_AF_pos[cnt]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
	else												Z_AF_pos[cnt]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
	MbufCopy(MilGrabImage[Camno], MilFocusImage[cnt]);
	BYTE* pBuf = (BYTE*)m_ImgObj[Camno].GetImageBuffer();
	MbufGet2d( MilFocusImage[cnt], 0, 0, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, pBuf );
	m_ImageCtrl[Camno].ImageUpdate();
	TRACE(_T("Camno[%d] [%d]\n"), Camno, cnt);
}
void CMatrox::AF_BufferSave(int cnt)
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString strText, strPath;
	strPath.Format(_T("D:\\AF_IMG\\%04d%02d%02d_%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	_wmkdir(_T("D:\\AF_IMG"));
	_wmkdir(strPath.GetBuffer(0));
	for(int i= 0; i< cnt; i++)
	{
		strText.Format(_T("%s\\%03d_%.3f.tif"), strPath, i, Z_AF_pos[i]);
		CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
		MbufSave(strMB.GetBuffer(0), MilFocusImage[i]);
#else
		MbufSave(strText.GetBuffer(0), MilFocusImage[i]);
#endif
	}
}
void CMatrox::AF_BufferLoad(int cnt, CString strPath)
{
	CString strText;
	for(int i= 0; i< cnt; i++)
	{
		strText.Format(_T("%s\\%03d.tif"), strPath, i);
		CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
		MbufLoad(strMB.GetBuffer(0), MilFocusImage[i]);
#else
		MbufLoad(strText.GetBuffer(0), MilFocusImage[i]);
#endif
	}
}
BOOL CMatrox::AutoFocusValue(DWORD MaxNBPos, DWORD MinPos, DWORD MaxPos,
	DWORD BestPos, DWORD StartPos, DWORD MaxPosVar)
{
	FocusMaxNBPosition = MaxNBPos;//33 ModelFile()->System_data.iAF_TotalCount+1
	FocusMinPosition = MinPos;//1
	FocusMaxPosition = MaxPos;//32 ModelFile()->System_data.iAF_TotalCount
	FocusBestPosition = BestPos;//1
	FocusStartPosition = StartPos;//1
	FocusMaxPositionVariation = MaxPosVar;//1

	return TRUE;
}
int CMatrox::AutoFocus()
{
	CString str;
	DWORD st= GetTickCount();

	MbufCopy(MilFocusImage[0], MilAutoFocusImage);
	MbufCopy(MilFocusImage[0], MilSubImage);

	UserData[0] = (long)MilAutoFocusImage;
	UserData[1] = (long)MilSubImage;
	UserData[2] = 0L;

	FocusPos= 0;

	MdigFocus(M_NULL,
		MilAutoFocusImage,
		M_DEFAULT,
		FocusHookFunction,
		UserData,
		1, //FocusMinPosition,
		1, //FocusStartPosition ,
		ParamFile()->System_data.iAF_TotalCount,
		1, //FocusMaxPositionVariation,
		M_SCAN_ALL,
		&FocusPos); 

	str.Format(_T("[Auto Focus Mono ] The best focus position is %.2d"), FocusPos);
	TRACE(_T("[%d] --- %s ---\n"), GetTickCount()-st, str);

	//	InspGrabContinueStop() ;
	//	Vision_Grab_Mode= _INSP_CAM_GRAB;
	//	MbufCopy(MilFocusImage[FocusPos],  MilGrabImage[_MEASURE_CAM]);
	//	MbufCopy(MilGrabImage[_MEASURE_CAM], MilDisplayImage[_MEASURE_CAM]);

	return FocusPos;
}

//===================================================================================
bool CMatrox::InspLiveUseGrabChChange(int ch) 
	//-----------------------------------------------------------------------------------
{
	if(!Vision_Initial_Flag) return false;

	if(m_VISION_SYS_MODE== _DEV_MODE ) {
		switch(ch)
		{
		case eTOP_CAM_20X:	MdigChannel(MilDigitizer[ch], M_CH0);			break;
		case eTOP_CAM_2X:	MdigChannel(MilDigitizer[ch], M_CH1);			break;
		case eBTM_CAM_20X:	MdigChannel(MilDigitizer[ch], M_CH4);			break;
		case eBTM_CAM_2X:	MdigChannel(MilDigitizer[ch], M_CH5);			break;
		}
	}
	return true;
}
//============================================================================================
bool CMatrox::InspGrab(int ch) 
{
	if(!Vision_Initial_Flag) return false;

	if(m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) InspGrabContinueStop();

	if(m_VISION_SYS_MODE== _DEV_MODE ) {
		Sleep(50);
		switch( ch )
		{
		case eTOP_CAM_20X : MdigChannel(MilDigitizer[eTOP_CAM_20X], M_CH0); break;//+M_SYNC);
		case eTOP_CAM_2X :	MdigChannel(MilDigitizer[eTOP_CAM_2X],	M_CH1); break;//+M_SYNC);
		case eBTM_CAM_20X : MdigChannel(MilDigitizer[eTOP_CAM_20X], M_CH4); break;//+M_SYNC);
		case eBTM_CAM_2X :	MdigChannel(MilDigitizer[eTOP_CAM_2X],	M_CH5); break;//+M_SYNC);
		}
		Sleep(100);
		MdigGrab(MilDigitizer[ch], MilGrabImage[ch]);
		//MbufCopy(MilGrabImage[ch], MilDisplayImage[ch]);
		BYTE* pBuf = (BYTE*)m_ImgObj[ch].GetImageBuffer();
		MbufGet2d( MilGrabImage[ch], 0, 0, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, pBuf );
		m_ImageCtrl[ch].ImageUpdate();
	}
	m_Vision_Grab_Mode= _INSP_CAM_GRAB;
	return true;
}
//===================================================================================
bool CMatrox::InspLiveUseGrab(int ch) 
{
	if(!Vision_Initial_Flag) return false;

	if(m_VISION_SYS_MODE== _DEV_MODE ) {
		//MdigGrabWait(MilDigitizer[ch], M_GRAB_END );
		if(m_Vision_Grab_Mode!= _INSP_CAM_GRAB_CONTINUE) return true;

		MdigGrab(MilDigitizer[ch], MilGrabImage[ch]);
		BYTE* pBuf = (BYTE*)m_ImgObj[ch].GetImageBuffer();
		MbufGet2d( MilGrabImage[ch], 0, 0, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, pBuf );
		m_ImageCtrl[ch].ImageUpdate();

/*
		if(ch== eTOP_CAM_20X && g_ImageSaveFlag_Test)
		{
			CString strText;
			strText.Format(_T("d:\\%d_%03d_Img.tif"), ch, g_ImageSaveCount++);//
			MbufSave(strText.GetBuffer(0), MilGrabImage[ch]);
		}
*/
		if( m_pImageCtrl )
			m_pImageCtrl->ImageUpdate();
	}
	return true;
}
//===================================================================================
bool CMatrox::InspGrabContinue( int ch ) 
{
	if(!Vision_Initial_Flag) return false;

/*
	if(m_VISION_SYS_MODE== _DEV_MODE ) {
		if( m_nSelectedCamForLive != ch)
		{
			Sleep(50);
			switch( ch )
			{
			case eTOP_CAM_20X : MdigChannel(MilDigitizer[eTOP_CAM_20X], M_CH0); break;//+M_SYNC);
			case eTOP_CAM_2X :	MdigChannel(MilDigitizer[eTOP_CAM_2X],	M_CH1); break;//+M_SYNC);
			case eBTM_CAM_20X : MdigChannel(MilDigitizer[eTOP_CAM_20X], M_CH4); break;//+M_SYNC);
			case eBTM_CAM_2X :	MdigChannel(MilDigitizer[eTOP_CAM_2X],	M_CH5); break;//+M_SYNC);
			}
			Sleep(50);
		}
	}
	else {

	}
	m_nSelectedCamForLive = ch;
*/
	m_Vision_Grab_Mode= _INSP_CAM_GRAB_CONTINUE;
	return true;
}

void CMatrox::ImageLiveCall(int ch)
{
	m_ImageCtrl[ch].SetImageObject( pSystem.GetVisionComponent()->GetImageObj(ch) );
}
//===================================================================================
bool CMatrox::InspGrabContinueStop() 
{
	if(!Vision_Initial_Flag) return false;

	if(m_VISION_SYS_MODE== _DEV_MODE ) {
		//		MdigGrabWait(MilDigitizer[_INSP_WINDOW_], M_GRAB_END);
	}
	m_Vision_Grab_Mode= _INSP_CAM_GRAB;
	Sleep(100);
	return true;
}
//===================================================================================
bool CMatrox::Insp_isGrabContinue() 
{
	if(m_Vision_Grab_Mode== _INSP_CAM_GRAB_CONTINUE) return true;
	return false;
}
//===================================================================================
int CMatrox::PatternLoad(int index)
	//-----------------------------------------------------------------------------------
{ 
	///////////// Cap
	CString strModelNamePath;
	double aaa= 0.;
	long bbb= 0;

	strModelNamePath.Format(_T("%s\\P_Model_%d_%s"), ParamFile()->strDirConfigPath, index, ParamFile()->System_data.strCurrentModelName);
	DWORD dwAttr = GetFileAttributes(strModelNamePath.GetBuffer(0));

	MappControl(M_ERROR, M_PRINT_DISABLE);
	if(dwAttr!= -1 ) {

		if(MilPatternModel[index])	MpatPreprocModel(M_NULL, MilPatternModel[index], M_RESET);

		if(MilPatternModel[index])	{ MpatFree(MilPatternModel[index]);	MilPatternModel[index]= 0; }

		CStringA strMB( strModelNamePath ) ;
#ifdef _MIL_VER_8
		if(MpatRestore(MilSystem, strMB.GetBuffer(0), &MilPatternModel[index])== M_NULL ) 
#else
		if(MpatRestore(MilSystem, strModelNamePath.GetBuffer(0), &MilPatternModel[index])== M_NULL ) 
#endif
			aaa= 0.;
		else {
			long modelno= 0;

			MpatSetSpeed(MilPatternModel[index], ParamFile()->Pattern_data[index].iSpeed);
			MpatSetAccuracy(MilPatternModel[index], ParamFile()->Pattern_data[index].iAccuracy);
			MpatSetAcceptance(MilPatternModel[index], ParamFile()->Pattern_data[index].dScoreAcceptance);
			MpatSetCertainty(MilPatternModel[index], ParamFile()->Pattern_data[index].dScoreCertanty);
			MpatSetAngle(MilPatternModel[index], M_SEARCH_ANGLE_MODE, M_DISABLE);
			MpatSetAngle(MilPatternModel[index], M_SEARCH_ANGLE_DELTA_NEG, 2);	
			MpatSetAngle(MilPatternModel[index], M_SEARCH_ANGLE_DELTA_POS, 2);
			MpatSetNumber(MilPatternModel[index], M_ALL) ; 
			//	MpatSetAngle(MilPatternModel[ModelNo], M_SEARCH_ANGLE_ACCURACY, 1);
			//		MpatSetCenter(MilPatternModel[index], ParamFile()->Pattern_data[index].dOffsetX, ParamFile()->Pattern_data[index].dOffsetY);
			long XX= 0, YY= 0;
			MpatInquire(MilPatternModel[index], M_ALLOC_SIZE_X+ M_TYPE_LONG, &XX);
			MpatInquire(MilPatternModel[index], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &YY);

			MpatSetCenter(MilPatternModel[index],	((XX+1)/2) + ParamFile()->Pattern_data[index].dOffsetX, 
				((YY+1)/2) + ParamFile()->Pattern_data[index].dOffsetY);

			MpatPreprocModel(M_NULL, MilPatternModel[index], M_DEFAULT);

			MgraColor(M_DEFAULT, M_RGB888(255,0,0));

			m_strLogFileBuffer.Format(_T("[LOAD] No %d=P_Model-Data [%s]"), index, strModelNamePath); SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			MappControl(M_ERROR, M_PRINT_ENABLE);
			return 1;
		}
	}
	else
	{
		if(MilPatternModel[index])	MpatPreprocModel(M_NULL, MilPatternModel[index], M_RESET);

		if(MilPatternModel[index])	{ MpatFree(MilPatternModel[index]);	MilPatternModel[index]= 0; }

		CStringA strMB( strModelNamePath ) ;
#ifdef _MIL_VER_8
		if(MpatRestore(MilSystem, strMB.GetBuffer(0), &MilPatternModel[index])== M_NULL ) 
#else
		if(MpatRestore(MilSystem, strModelNamePath.GetBuffer(0), &MilPatternModel[index])== M_NULL ) 
#endif
			aaa= 0.;
	}
	MappControl(M_ERROR, M_PRINT_ENABLE);
	return 0;
}
//===================================================================================
int CMatrox::PatternCopy(int index, int toIndex)
	//-----------------------------------------------------------------------------------
{ 
	///////////// Cap
	CString strModelNamePath;
	double aaa= 0.;
	long bbb= 0;

	strModelNamePath.Format(_T("%s\\P_Model_%d_%s"), ParamFile()->strDirConfigPath, index, ParamFile()->System_data.strCurrentModelName);
	DWORD dwAttr = GetFileAttributes(strModelNamePath.GetBuffer(0));

	if(dwAttr!= -1 ) {

		if(MilPatternModel[toIndex])	MpatPreprocModel(M_NULL, MilPatternModel[toIndex], M_RESET);

		if(MilPatternModel[toIndex])	{ MpatFree(MilPatternModel[toIndex]);	MilPatternModel[toIndex]= 0; }

		CStringA strMB( strModelNamePath ) ;
#ifdef _MIL_VER_8
		if(MpatRestore(MilSystem, strMB.GetBuffer(0), &MilPatternModel[toIndex])== M_NULL ) 
#else
		if(MpatRestore(MilSystem, strModelNamePath.GetBuffer(0), &MilPatternModel[toIndex])== M_NULL ) 
#endif
			aaa= 0.;
		else {
			long modelno= 0;

			MpatSetSpeed(MilPatternModel[toIndex], ParamFile()->Pattern_data[index].iSpeed);
			MpatSetAccuracy(MilPatternModel[toIndex], ParamFile()->Pattern_data[index].iAccuracy);
			MpatSetAcceptance(MilPatternModel[toIndex], ParamFile()->Pattern_data[index].dScoreAcceptance);
			MpatSetCertainty(MilPatternModel[toIndex], ParamFile()->Pattern_data[index].dScoreCertanty);
			MpatSetAngle(MilPatternModel[toIndex], M_SEARCH_ANGLE_MODE, M_DISABLE);
			MpatSetAngle(MilPatternModel[toIndex], M_SEARCH_ANGLE_DELTA_NEG, 2);	
			MpatSetAngle(MilPatternModel[toIndex], M_SEARCH_ANGLE_DELTA_POS, 2);
			//	MpatSetAngle(MilPatternModel[ModelNo], M_SEARCH_ANGLE_ACCURACY, 1);
			//		MpatSetCenter(MilPatternModel[index], ParamFile()->Pattern_data[index].dOffsetX, ParamFile()->Pattern_data[index].dOffsetY);
			long XX= 0, YY= 0;
			MpatInquire(MilPatternModel[toIndex], M_ALLOC_SIZE_X+ M_TYPE_LONG, &XX);
			MpatInquire(MilPatternModel[toIndex], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &YY);

			MpatSetCenter(MilPatternModel[toIndex],	((XX+1)/2) + ParamFile()->Pattern_data[toIndex].dOffsetX, 
				((YY+1)/2) + ParamFile()->Pattern_data[toIndex].dOffsetY);

			MpatPreprocModel(M_NULL, MilPatternModel[toIndex], M_DEFAULT);

			MgraColor(M_DEFAULT, M_RGB888(255,0,0));

			m_strLogFileBuffer.Format(_T("[LOAD] No %d=P_Model-Copy [%s]"), index, strModelNamePath); SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
			return 1;
		}
	}
	return 0;
}
//===================================================================================
void CMatrox::PatternSave(int index)
	//-----------------------------------------------------------------------------------
{
	CString strModelNamePath;
	strModelNamePath.Format(_T("%s\\P_Model_%d_%s"), ParamFile()->strDirConfigPath, index, ParamFile()->System_data.strCurrentModelName);

	CStringA strMB( strModelNamePath ) ;
#ifdef _MIL_VER_8
	MpatSave(strMB.GetBuffer(0), MilPatternModel[index]);
#else
	MpatSave(strModelNamePath.GetBuffer(0), MilPatternModel[index]);
#endif
	m_strLogFileBuffer.Format(_T("[SAVE] No %d=P_Model-Data [%s]"), index, strModelNamePath); SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
}
//===================================================================================
void CMatrox::PatternImageLoadNdisp(int ModelIndex)
{
	CString strText;

	MbufClear(PatternSetOverlayImage, TransparentColor); 
	if(MilPatternModel[ModelIndex]) {
		MpatInquire(MilPatternModel[ModelIndex], M_ALLOC_SIZE_X+ M_TYPE_LONG, &Pattern_AllocSizeX);
		MpatInquire(MilPatternModel[ModelIndex], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &Pattern_AllocSizeY);
	}
	else {
		Pattern_AllocSizeX= 10;
		Pattern_AllocSizeY= 10;
		//return;
	}

	long width = Pattern_AllocSizeX;
	long height = Pattern_AllocSizeY;

	if(width<= 0 || height<= 0) {
		AfxMessageBox(_T("PatternModel-Define Error!"));		return;
	}

	if(PatternSetDisplay)					{ MdispDeselect(PatternSetDisplay,PatternSetDispImage); }
	if(PatternSetDispImage)				{ MbufFree(PatternSetDispImage) ; PatternSetDispImage= 0; }
	if(PatternDontCareImage)				{ MbufFree(PatternDontCareImage) ; PatternDontCareImage= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage);
	MdispSelectWindow(PatternSetDisplay, PatternSetDispImage, PatternSetHwnd) ;
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE); 
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MbufClear(PatternDontCareImage, 0);
	MbufClear(PatternSetDispImage, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage);
#endif

	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	strText.Format(_T("%s\\PatternImageLoadNdisp_DC_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternDontCareImage);
#else
	MbufSave(strText.GetBuffer(0), PatternDontCareImage);
#endif

	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;

	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetOverlayImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;

}
void CMatrox::PatternImageLoadNdisp2(int ModelIndex)
{
	CString strText;

	MbufClear(PatternSetOverlayImage2, TransparentColor); 
	if(MilPatternModel[ModelIndex]) {
		MpatInquire(MilPatternModel[ModelIndex], M_ALLOC_SIZE_X+ M_TYPE_LONG, &Pattern_AllocSizeX2);
		MpatInquire(MilPatternModel[ModelIndex], M_ALLOC_SIZE_Y+ M_TYPE_LONG, &Pattern_AllocSizeY2);
	}
	else {
		Pattern_AllocSizeX2= 10;
		Pattern_AllocSizeY2= 10;
		//return;
	}

	long width = Pattern_AllocSizeX2;
	long height = Pattern_AllocSizeY2;

	if(width<= 0 || height<= 0) {
		AfxMessageBox(_T("PatternModel-Define Error!"));		return;
	}

	if(PatternSetDisplay2)					{ MdispDeselect(PatternSetDisplay2,PatternSetDispImage2); }
	if(PatternSetDispImage2)				{ MbufFree(PatternSetDispImage2) ; PatternSetDispImage2= 0; }
	if(PatternDontCareImage2)				{ MbufFree(PatternDontCareImage2) ; PatternDontCareImage2= 0; }

	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage2);
	MbufAlloc2d	(MilSystem, width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage2);
	MdispSelectWindow(PatternSetDisplay2, PatternSetDispImage2, PatternSetHwnd2) ;
	//--------
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_ENABLE); 
	MdispInquire(PatternSetDisplay2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage2);
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage2);
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MbufClear(PatternDontCareImage2, 0);
	MbufClear(PatternSetDispImage2, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage2 ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage2);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage2);
#endif

	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	strText.Format(_T("%s\\PatternImageLoadNdisp_DC_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternDontCareImage2);
#else
	MbufSave(strText.GetBuffer(0), PatternDontCareImage2);
#endif

	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage2,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;

	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetOverlayImage2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;

}
//===================================================================================
void CMatrox::PatternReDefine( int nIdx, CImageObj* pImgObj, int ModelIndex)
{
	if( ModelIndex >= _MAX_FIND_MODEL )
		return;

	if( m_rcROI.IsRectEmpty() )
	{
		MyMessageBox( _T("등록할 Image ROI 설정이 안되었습니다. 확인해주세요."), _T("ROI 에러"), M_ICONERROR );
		return;
	}

	m_rcROI.NormalizeRect();

	CString strText;

	long width = m_rcROI.Width();
	long height = m_rcROI.Height();

	if(Insp_Tmp_MilImage[nIdx])				{ MbufFree(Insp_Tmp_MilImage[nIdx]) ; Insp_Tmp_MilImage[nIdx]= 0; }
	Insp_Tmp_MilImage[nIdx] = MbufAlloc2d( MilSystem,  pImgObj->GetWidthBytes(), pImgObj->GetHeight(), 8+M_UNSIGNED , M_IMAGE+M_PROC, NULL );


	if(pImgObj->GetWidthBytes()<= 0 || pImgObj->GetHeight()<= 0) 
	{
		AfxMessageBox(_T("PatternModel-Define Error!"));		return;
	}

	if(PatternSetDisplay)					{ MdispDeselect(PatternSetDisplay, PatternSetDispImage); }
	if(PatternSetDispImage)					{ MbufFree(PatternSetDispImage) ; PatternSetDispImage= 0; }
	if(PatternDontCareImage)				{ MbufFree(PatternDontCareImage) ; PatternDontCareImage= 0; }

	MbufAlloc2d	(MilSystem,  width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage);
	MbufAlloc2d	(MilSystem,  width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage);
	MdispSelectWindow(PatternSetDisplay, PatternSetDispImage, PatternSetHwnd) ;
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternDontCareImage, 0);
	MbufClear(PatternSetDispImage, 0);

	MbufPut2d( Insp_Tmp_MilImage[nIdx], 0, 0, pImgObj->GetWidthBytes(), pImgObj->GetHeight(), pImgObj->GetImageBuffer() );
	MbufCopyClip(Insp_Tmp_MilImage[nIdx], PatternSetDispImage, -m_rcROI.left , -m_rcROI.top) ;//MilGrabImage[_INSP_CAM_ROLL]
	strText.Format(_T("%s\\ReDefImage_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);	
	CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), Insp_Tmp_MilImage[nIdx]);
#else
	MbufSave(strText.GetBuffer(0), Insp_Tmp_MilImage[nIdx]);
#endif
	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage);
#endif

//	MbufCopyClip(MilDisplayImage[Cam_No], PatternSetDispImage, -Start.x , -Start.y) ;//MilGrabImage[_INSP_CAM_ROLL]

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatAllocModel(MilSystem, PatternSetDispImage, 0, 0, 
		width, height, M_NORMALIZED, &MilPatternModel[ModelIndex]);
	MpatPreprocModel(PatternSetDispImage, MilPatternModel[ModelIndex], M_DEFAULT);

	MbufClear(PatternDontCareImage, 0);
	MbufClear(PatternSetDispImage, 0);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage);
#endif

	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MpatCopy(MilPatternModel[ModelIndex], PatternDontCareImage , M_DONT_CARES);
	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;


}
//===================================================================================
void CMatrox::PatternReDefine2( int nIdx, CImageObj* pImgObj, int ModelIndex)
{
	if( ModelIndex >= _MAX_FIND_MODEL )
		return;

	if( m_rcROI.IsRectEmpty() )
	{
		MyMessageBox( _T("등록할 Image ROI 설정이 안되었습니다. 확인해주세요."), _T("ROI 에러"), M_ICONERROR );
		return;
	}

	m_rcROI.NormalizeRect();

	CString strText;

	long width = m_rcROI.Width();
	long height = m_rcROI.Height();

	if(Insp_Tmp_MilImage[nIdx])				{ MbufFree(Insp_Tmp_MilImage[nIdx]) ; Insp_Tmp_MilImage[nIdx]= 0; }
	Insp_Tmp_MilImage[nIdx] = MbufAlloc2d( MilSystem,  pImgObj->GetWidthBytes(), pImgObj->GetHeight(), 8+M_UNSIGNED , M_IMAGE+M_PROC, NULL );


	if(pImgObj->GetWidthBytes()<= 0 || pImgObj->GetHeight()<= 0) 
	{
		AfxMessageBox(_T("PatternModel-Define Error!"));		return;
	}

	if(PatternSetDisplay2)					{ MdispDeselect(PatternSetDisplay2, PatternSetDispImage2); }
	if(PatternSetDispImage2)				{ MbufFree(PatternSetDispImage2) ; PatternSetDispImage2= 0; }
	if(PatternDontCareImage2)				{ MbufFree(PatternDontCareImage2) ; PatternDontCareImage2= 0; }

	MbufAlloc2d	(MilSystem,  width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage2);
	MbufAlloc2d	(MilSystem,  width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage2);
	MdispSelectWindow(PatternSetDisplay2, PatternSetDispImage2, PatternSetHwnd2) ;
	//--------
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage2);
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage2);
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay2, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay2, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternDontCareImage2, 0);
	MbufClear(PatternSetDispImage2, 0);

	MbufPut2d( Insp_Tmp_MilImage[nIdx], 0, 0, pImgObj->GetWidthBytes(), pImgObj->GetHeight(), pImgObj->GetImageBuffer() );
	MbufCopyClip(Insp_Tmp_MilImage[nIdx], PatternSetDispImage2, -m_rcROI.left , -m_rcROI.top) ;//MilGrabImage[_INSP_CAM_ROLL]
	strText.Format(_T("%s\\ReDefImage_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);	
	CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), Insp_Tmp_MilImage[nIdx]);
#else
	MbufSave(strText.GetBuffer(0), Insp_Tmp_MilImage[nIdx]);
#endif
	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage2);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage2);
#endif

	//	MbufCopyClip(MilDisplayImage[Cam_No], PatternSetDispImage2, -Start.x , -Start.y) ;//MilGrabImage[_INSP_CAM_ROLL]

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatAllocModel(MilSystem, PatternSetDispImage2, 0, 0, 
		width, height, M_NORMALIZED, &MilPatternModel[ModelIndex]);
	MpatPreprocModel(PatternSetDispImage2, MilPatternModel[ModelIndex], M_DEFAULT);

	MbufClear(PatternDontCareImage2, 0);
	MbufClear(PatternSetDispImage2, 0);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage2 ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage2);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage2);
#endif

	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage2, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MpatCopy(MilPatternModel[ModelIndex], PatternDontCareImage2 , M_DONT_CARES);
	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage2,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;


}
void CMatrox::PatternTmpImageDefine( int nIdx, CImageObj* pImgObj, int ModelIndex, CPoint ptPos)
{
	if( ModelIndex >= _MAX_FIND_MODEL )
		return;

	if( ptPos.x<= 0 || ptPos.y<= 0 )
	{
		MyMessageBox( _T("등록할 Image ROI 설정이 안되었습니다. 확인해주세요."), _T("ROI 에러"), M_ICONERROR );
		return;
	}

	CString strText;

	long width = 300;
	long height = 300;
/*
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.5 && ParamFile()->Model_data.dLED_Pitch_Y > 0.5)
	{
		if(Cam_Mode== eTOP_CAM_2X) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0)) / 1.5; //2.0);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0)) / 1.5; //2.0);
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0)) / 1.5; //2.0);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0)) / 1.5; //2.0);
		}
	}
*/
		
	CRect m_rcROI;
	m_rcROI.left= ptPos.x - (width/2);			if(m_rcROI.left<= 0) m_rcROI.left= 0;
	m_rcROI.right= ptPos.x + (width/2);			if(m_rcROI.right> _INSP_IMAGE_SIZE_X) m_rcROI.right= _INSP_IMAGE_SIZE_X;
	m_rcROI.top= ptPos.y - (height/2);			if(m_rcROI.top< 0) m_rcROI.top= 0;
	m_rcROI.bottom= ptPos.y + (height/2);		if(m_rcROI.bottom> _INSP_IMAGE_SIZE_Y) m_rcROI.bottom= _INSP_IMAGE_SIZE_Y;

	if(Insp_Tmp_MilImage[nIdx])				{ MbufFree(Insp_Tmp_MilImage[nIdx]) ; Insp_Tmp_MilImage[nIdx]= 0; }
	Insp_Tmp_MilImage[nIdx] = MbufAlloc2d( MilSystem,  pImgObj->GetWidthBytes(), pImgObj->GetHeight(), 8+M_UNSIGNED , M_IMAGE+M_PROC, NULL );


	if(pImgObj->GetWidthBytes()<= 0 || pImgObj->GetHeight()<= 0) 
	{
		AfxMessageBox(_T("PatternModel-Define Error!"));		return;
	}

	if(PatternSetDisplay)					{ MdispDeselect(PatternSetDisplay, PatternSetDispImage); }
	if(PatternSetDispImage)					{ MbufFree(PatternSetDispImage) ; PatternSetDispImage= 0; }
	if(PatternDontCareImage)				{ MbufFree(PatternDontCareImage) ; PatternDontCareImage= 0; }

	MbufAlloc2d	(MilSystem,  width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternDontCareImage);
	MbufAlloc2d	(MilSystem,  width, height, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &PatternSetDispImage);
	MdispSelectWindow(PatternSetDisplay, PatternSetDispImage, PatternSetHwnd) ;
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_WINDOW_OVR_BUF_ID, &PatternSetOverlayImage);
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	MdispInquire(PatternSetDisplay, M_KEY_COLOR, &TransparentColor); 
	//--------
	MdispControl(PatternSetDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);
	//--------

	MbufClear(PatternDontCareImage, 0);
	MbufClear(PatternSetDispImage, 0);

	MbufPut2d( Insp_Tmp_MilImage[nIdx], 0, 0, pImgObj->GetWidthBytes(), pImgObj->GetHeight(), pImgObj->GetImageBuffer() );
	MbufCopyClip(Insp_Tmp_MilImage[nIdx], PatternSetDispImage, -m_rcROI.left , -m_rcROI.top) ;//MilGrabImage[_INSP_CAM_ROLL]
	strText.Format(_T("%s\\ReDefImage_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);	
	CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), Insp_Tmp_MilImage[nIdx]);
#else
	MbufSave(strText.GetBuffer(0), Insp_Tmp_MilImage[nIdx]);
#endif
	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage);
#endif

	//	MbufCopyClip(MilDisplayImage[Cam_No], PatternSetDispImage, -Start.x , -Start.y) ;//MilGrabImage[_INSP_CAM_ROLL]

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0)) ;

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));

	MpatAllocModel(MilSystem, PatternSetDispImage, 0, 0, 
		width, height, M_NORMALIZED, &MilPatternModel[ModelIndex]);
	MpatPreprocModel(PatternSetDispImage, MilPatternModel[ModelIndex], M_DEFAULT);

	MbufClear(PatternDontCareImage, 0);
	MbufClear(PatternSetDispImage, 0);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage ,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;

	strText.Format(_T("%s\\ReDefPattern_%d.tif"), ParamFile()->strDirDebugImage, ModelIndex);
	strMB= strText;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternSetDispImage);
#else
	MbufSave(strText.GetBuffer(0), PatternSetDispImage);
#endif

	MgraColor(M_DEFAULT, 255) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MpatCopy(MilPatternModel[ModelIndex], PatternDontCareImage , M_DONT_CARES);
	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;


}

//===================================================================================
void CMatrox::PatternDrawFillPoint(CPoint point, long factor, long size)
	//-----------------------------------------------------------------------------------
{
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MgraArcFill(M_DEFAULT, PatternSetOverlayImage	 , (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
	MgraColor(M_DEFAULT, 255) ;
	MgraArcFill(M_DEFAULT, PatternDontCareImage, (long)((point.x)/factor) , (long)((point.y)/factor), size, size, 0, 360) ;
}

//===================================================================================
void CMatrox::PatternMaskApply(int ModelIndex, double zoom)
	//-----------------------------------------------------------------------------------
{
	CString strText; 

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_RESET);
	MpatCopy(MilPatternModel[ModelIndex], PatternDontCareImage , M_DONT_CARES);

	strText.Format(_T("d:\\PatternMaskApply_DC_%d.tif"), ModelIndex);
	CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer(0), PatternDontCareImage);
#else
	MbufSave(strText.GetBuffer(0), PatternDontCareImage);
#endif
	MpatSetDontCare(MilPatternModel[ModelIndex], PatternDontCareImage, 0, 0, 255);

	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MbufClear(PatternSetOverlayImage, TransparentColor); 

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, 255) ;
	MbufClear(PatternDontCareImage, 0);
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;

	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetOverlayImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
}
//===================================================================================
void CMatrox::PatternMaskEditClear(int ModelIndex, double zoom)
	//-----------------------------------------------------------------------------------
{
	MbufClear(PatternDontCareImage, 0);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatCopy(MilPatternModel[ModelIndex], PatternDontCareImage , M_DONT_CARES);
	MpatSetDontCare(MilPatternModel[ModelIndex], PatternDontCareImage, 0, 0, 255);

	MpatPreprocModel(M_NULL, MilPatternModel[ModelIndex], M_DEFAULT);
	MbufClear(PatternSetOverlayImage, TransparentColor); 

	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetDispImage,	  M_DRAW_IMAGE, 0, M_DEFAULT) ;
	MgraColor(M_DEFAULT, M_RGB888(0, 255, 0)) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternDontCareImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
	MpatDraw(M_DEFAULT, MilPatternModel[ModelIndex], PatternSetOverlayImage, M_DRAW_DONT_CARES, 0, M_DEFAULT) ;
}

void CMatrox::PatDispZoom(double val)
{
	MdispZoom(PatternSetDisplay, val , val);
}
void CMatrox::OverlayClear(int no)
{
	CxGraphicObject& pGO = m_ImageCtrl[no].GetGraphicObject();
	pGO.Reset();
	m_ImageCtrl[no].ImageUpdate();
}
bool CMatrox::Save_Image(int cam_no, CString strName)
{
	CImageObj* pImg = pSystem.GetVisionComponent()->GetImageObj( cam_no );

	if(pImg->GetWidthBytes()<= 0 || pImg->GetHeight()<= 0) 
	{		AfxMessageBox(_T("Pattern-Image Error!"));		return false;	}
	Insp_OriginalImage = MbufAlloc2d( MilSystem,  pImg->GetWidthBytes(), pImg->GetHeight(), 8+M_UNSIGNED , M_IMAGE+M_PROC, NULL );
	MbufClear(Insp_OriginalImage, 0);
	MbufPut2d( Insp_OriginalImage, 0, 0, m_ImageCtrl[cam_no].GetVisibleImageObject()->GetWidth(), 
		m_ImageCtrl[cam_no].GetVisibleImageObject()->GetHeight(), m_ImageCtrl[cam_no].GetVisibleImageObject()->GetImageBuffer() );
	//--------------------------------
	CStringA strMB( strName ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer (0), Insp_OriginalImage);
#else
	MbufSave(strName.GetBuffer (0), Insp_OriginalImage);
#endif

	if(Insp_OriginalImage)	{				MbufFree(Insp_OriginalImage);		Insp_OriginalImage= M_NULL; }
	
	return true;
}
bool CMatrox::Load_Image(int cam_no, CString strName)
{
	CStringA strMB( strName ) ;
#ifdef _MIL_VER_8
	MbufLoad(strMB.GetBuffer (0), MilGrabImage[cam_no]);
#else
	MbufLoad(strName.GetBuffer (0), MilGrabImage[cam_no]);
#endif
	BYTE* pBuf = (BYTE*)m_ImgObj[cam_no].GetImageBuffer();
	MbufGet2d( MilGrabImage[cam_no], 0, 0, _INSP_IMAGE_SIZE_X, _INSP_IMAGE_SIZE_Y, pBuf );
	m_ImageCtrl[cam_no].GetVisibleImageObject()->LoadFromFile(strName.GetBuffer (0));
	m_ImageCtrl[cam_no].ImageUpdate();
	//	GetImageViewer(cam_no)->ImageUpdate();
	return true;
}

//===================================================================================
bool CMatrox::PatternFinderPos(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	int i;
	*angle= 0.;	*posx= 0.; *posy= 0.;

	//--------------------------------
	CImageObj* pImg = pSystem.GetVisionComponent()->GetImageObj( Cam_No );

	if( ModelNo >= _MAX_FIND_MODEL )		return false;
	if(pImg->GetWidthBytes()<= 0 || pImg->GetHeight()<= 0) 
	{		AfxMessageBox(_T("Pattern-Image Error!"));		return false;	}
	Insp_OriginalImage = MbufAlloc2d( MilSystem,  pImg->GetWidthBytes(), pImg->GetHeight(), 8+M_UNSIGNED , M_IMAGE+M_PROC, NULL );
	MbufClear(Insp_OriginalImage, 0);
	MbufPut2d( Insp_OriginalImage, 0, 0, m_ImageCtrl[Cam_No].GetVisibleImageObject()->GetWidth(), 
		m_ImageCtrl[Cam_No].GetVisibleImageObject()->GetHeight(), m_ImageCtrl[Cam_No].GetVisibleImageObject()->GetImageBuffer() );
	//--------------------------------
	fnSave.Format (_T("%s\\%d년%d월%d일"), ParamFile()->strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_wmkdir(fnSave);
	SaveFileName.Format (_T("%s\\Pat_FindImage_%02d%02d%02d_C%02dM%02d.tif"), fnSave, st.wHour, st.wMinute, st.wSecond, Cam_No, ModelNo);
	CStringA strMB( SaveFileName ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer (0), Insp_OriginalImage);
#else
	MbufSave(SaveFileName.GetBuffer (0), Insp_OriginalImage);
#endif

	////////////////////////////////////////////////

	long bNum=0 ;
	double *modPosx, *modPosy, *Score;
	long   Model[50]  = {0L, };                     /* Number of results found.*/   
	double Time = 0.0;                           /* Bench variable.         */  

	long number =0 ;

	CString aaa1, aaa2;
	DWORD dst;
	dst= GetTickCount();
	double dTime = 0.0;

	CRect setROI;
	MpatSetNumber(MilPatternModel[ModelNo], M_ALL) ; 
	if(ParamFile()->System_data.dShift_CamY!= 0 && 
		ModelNo != mdTOP_20X_STAEG1_START	&& 	ModelNo != mdTOP_20X_STAEG1_END	&&
		ModelNo != mdTOP_20X_STAEG2_START	&&	ModelNo != mdTOP_20X_STAEG2_END	&&
		ModelNo != mdBTM_20X_ROLL_START		&&	ModelNo != mdBTM_20X_ROLL_END	&& 
		ModelNo != mdBTM_2X_ROLL_START		&&	ModelNo != mdBTM_2X_ROLL_END	&&
		ModelNo != mdTOP_20X_MASK			&&	ModelNo != mdTOP_2X_MASK		&& 
		ModelNo != mdBTM_20X_MASK			&&	ModelNo != mdBTM_2X_MASK		&&
		ModelNo != mdTOP_20X_DEF_START		&& 	ModelNo != mdTOP_20X_DEF_END	&& 
		ModelNo != mdTOP_2X_DEF_START		&&	ModelNo != mdTOP_2X_DEF_END		&&
		ModelNo != mdTMP_MASK				&&
		ModelNo != mdTOP_20X_STAEG3_START	&&	ModelNo != mdTOP_20X_STAEG3_END	)
	{
		double tor_x_Val= 0.0, tor_y_Val= 0.0;
		tor_y_Val= (ParamFile()->System_data.dShift_CamY / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0);
		setROI.left= 0;
		setROI.top= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val);
		setROI.right= (LONG)(_INSP_IMAGE_SIZE_X);
		setROI.bottom= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val);
		
		MpatSetPosition(MilPatternModel[ModelNo], setROI.left, setROI.top, setROI.Width(), setROI.Height());
	}
	else
		MpatSetPosition(MilPatternModel[ModelNo], M_ALL, M_ALL, M_ALL, M_ALL);
	/* R
	{eset the timer. */

	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(Insp_OriginalImage, MilPatternModel[ModelNo], MilPatternResult[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
	TRACE("The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult[ModelNo], M_NULL);

	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

	CxGraphicObject& pGO = m_ImageCtrl[Cam_No].GetGraphicObject();

	MgraColor(M_DEFAULT, M_RGB888(255,50,50));
	MgraFontScale(M_DEFAULT, 1.5, 1.5);

	if(number >= 1 && number< 90)
	{
		modPosx = new double[number] ;
		modPosy = new double[number] ;
		Score = new double[number] ;

		/* Get the results for each model. */
		MpatGetResult(MilPatternResult[ModelNo],  M_SCORE, Score);
		MpatGetResult(MilPatternResult[ModelNo], M_POSITION_X, modPosx);
		MpatGetResult(MilPatternResult[ModelNo], M_POSITION_Y, modPosy);

		int  findno= 0;
		double minscore= 0.;
		if(number> 1) 
		{
//			COLORALIGNMARK clrMark;
			for(i= 0; i< number; i++) 
			{
				if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
// 				clrMark.CreateObject( PDC_BLUE, modPosx[i], modPosy[i], /*50*/10, 5/*10*/, PS_SOLID, 1);
// 				pGO.AddDrawAlignMark( clrMark ); 
			}
		}
		*posx= modPosx[findno];	*posy= modPosy[findno];	*angle= Score[findno];

		double blobPosX= 0.;	double blobPosY= 0.;
		long cenx= 0, ceny= 0;
		POINT pt;
		cenx= (long)(*posx); ceny= (long)(*posy);

		int tmpLimit= 0;

		if(overlayflag) {
			CString str1 ;
			if(ModelNo % 2) 
			{
/*
				MgraColor(M_DEFAULT, M_RGB888(255,50,50));
				//			MgraFontScale(M_DEFAULT, 1.5, 1.5); 
				str1.Format(_T("%3.1f"), Score[findno]);
				MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
				str1.Format(_T("X:%3.1f,Y:%3.1f"), *posx, *posy);
				MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 20L,   str1.GetBuffer(0));
				//			MgraFontScale(M_DEFAULT, 1.0, 1.0);
*/
// 				COLORDCROSS clrCross;
// 				COLORLINE	clrLine;
// 				clrCross.CreateObject( PDC_BLUE, DPOINT((double)cenx, (double)ceny), 30 );
// 				clrLine.CreateObject( PDC_BLUE, cenx, ceny, cenx, ceny, PS_SOLID, 1);
//				pGO.AddDrawDCross( clrCross );
//				pGO.AddDrawLine( clrLine );
	
				COLORBOX clrBox;
				COLORTEXT clrText;
				COLORALIGNMARK clrMark;
				int sssss= (int)(ParamFile()->System_data.dRectSizeX / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0*2.0));
				int sssss1= (int)(ParamFile()->System_data.dRectSizeY / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0*2.0));
				if(sssss< 5) sssss= 5;
				if(sssss1< 10) sssss1= 10;
				clrMark.CreateObject( PDC_BLUE, cenx, ceny, /*50*/sssss1, sssss/*10*/, PS_SOLID, 1);
				pGO.AddDrawAlignMark( clrMark ); 

				pt.x = 850;		pt.y = 15;
				str1.Format(_T("X:%3.1f,Y:%3.1f:%3.1f"), *posx, *posy, Score[findno]);
				clrText.CreateObject( PDC_BLUE, pt, 12 );		clrText.SetText( str1 );

				if(ParamFile()->System_data.dShift_CamY!= 0)
				{
					clrBox.CreateObject( RGB(255,250,200), setROI, PS_DOT, 1);
					pGO.AddDrawBox( clrBox );
				}

				pGO.AddDrawText( clrText );
			}
			else	
			{
/*
				MgraColor(M_DEFAULT, M_RGB888(50,255,50));
				//			MgraFontScale(M_DEFAULT, 1.5, 1.5); 
				str1.Format(_T("%3.1f"), Score[findno]);
				MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 5L,   str1.GetBuffer(0));
				str1.Format(_T("X:%3.1f,Y:%3.1f : %3.1f"), *posx, *posy, Score[findno]);
				MgraText(M_DEFAULT, MilOverlayImage[Cam_No], 10L, 20L,   str1.GetBuffer(0));
				//		MgraFontScale(M_DEFAULT, 1.0, 1.0);
*/
//				COLORBOX clrBox;
				COLORBOX clrBox;
				COLORALIGNMARK clrMark;
				COLORTEXT clrText;
//				COLORLINE	clrLine;
			//	clrCross.CreateObject( PDC_GREEN, DPOINT((double)cenx, (double)ceny), 30 );
//				clrLine.CreateObject( PDC_GREEN, cenx-50, ceny, cenx+50, ceny, PS_SOLID, 1);
	//			pGO.AddDrawLine( clrLine );
		//		clrLine.CreateObject( PDC_GREEN, cenx, ceny-50, cenx, ceny+50, PS_SOLID, 1);
			//	pGO.AddDrawLine( clrLine );
//				clrBox.CreateObject( PDC_GREEN, cenx-10, ceny-10, cenx+10, ceny+10, PS_SOLID, 1);
	//			pGO.AddDrawBox( clrBox );
				int sssss= (int)(ParamFile()->System_data.dRectSizeX / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0*2.0));
				int sssss1= (int)(ParamFile()->System_data.dRectSizeY / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0*2.0));
				if(sssss< 5) sssss= 5;
				if(sssss1< 10) sssss1= 10;
				clrMark.CreateObject( PDC_GREEN, cenx, ceny, sssss1/*50*/, /*10*/sssss, PS_SOLID, 1);
				pGO.AddDrawAlignMark( clrMark ); 

				pt.x = 10;		pt.y = 15;
				str1.Format(_T("X:%3.1f,Y:%3.1f:%3.1f"), *posx, *posy, Score[findno]);
				clrText.CreateObject( PDC_GREEN, pt, 12 );		clrText.SetText( str1 );

				if(ParamFile()->System_data.dShift_CamY!= 0)
				{
					clrBox.CreateObject( RGB(255,250,200), setROI, PS_DOT, 1);
					pGO.AddDrawBox( clrBox );
				}
				pGO.AddDrawText( clrText );
				//-----------------------------------------
			}

			//	MgraFontScale(M_DEFAULT, 1.0, 1.0);
			long dist= 100;

			//	MpatDraw(M_DEFAULT, MilPatternResult[ModelNo], MilOverlayImage[Cam_No], M_DRAW_POSITION+M_DRAW_BOX , 0, M_DEFAULT) ;
/*
			MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny), (long)(cenx+dist), (long)(ceny));
			MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx), ceny-dist,	(long)(cenx), (long)(ceny+dist));
			MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	cenx-dist, (long)(ceny)+1, (long)(cenx+dist), (long)(ceny)+1);
			MgraLine(M_DEFAULT, MilOverlayImage[Cam_No],	(long)(cenx)+1, ceny-dist,	(long)(cenx)+1, (long)(ceny+dist));
			MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
*/

		}

/*
		if(ModelNo % 2) MgraColor(M_DEFAULT, M_RGB888(255,50,50));
		else			MgraColor(M_DEFAULT, M_RGB888(50,2550,50));
*/

		if(modPosx) delete [] modPosx ;
		if(modPosy) delete [] modPosy ;
		if(Score) delete [] Score ;

		m_strLogFileBuffer.Format(_T("[PAT_FIND] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]"), 
			ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
		SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
		if(*angle<= 0.) *angle= 1.;

		if(Insp_OriginalImage)	{				MbufFree(Insp_OriginalImage);		Insp_OriginalImage= M_NULL; }
		m_ImageCtrl[Cam_No].ImageUpdate();

		return true;
	}
	else 
	{
		CString str1;
		CPoint pt;

		*angle= 0.;			*posx= 0.; 			*posy= 0.; 
		if(errsaveflag) {
			SaveFileName.Format (_T("%s\\ERROR_Pat_FindImage_%02d%02d%02d"), fnSave, st.wHour, st.wMinute, st.wSecond);
			aaa2.Format(_T("%s[%d].tif"), SaveFileName, ModelNo); 
			CStringA strMB( aaa2 ) ;
#ifdef _MIL_VER_8
			MbufSave(strMB.GetBuffer (0), Insp_OriginalImage);
#else
			MbufSave(aaa2.GetBuffer (0), Insp_OriginalImage);
#endif
		}
		if(overlayflag) {
/*
			CString str1 ;
			MgraColor(M_DEFAULT, 200);
			MgraColor(M_DEFAULT, M_RGB888(255, 255, 255)) ;
*/
			COLORDCROSS clrCross;
			COLORTEXT clrText;
			pt.x = 500;		pt.y =100;
			str1.Format(_T("Find-Error!!!"));
			clrText.CreateObject( PDC_RED, pt, 20 );		clrText.SetText( str1 );

			pGO.AddDrawText( clrText );
		}
		m_strLogFileBuffer.Format(_T("[PAT_FIND] ERROR! No=%d"), ModelNo); 
		SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;

		if(Insp_OriginalImage)	{				MbufFree(Insp_OriginalImage);		Insp_OriginalImage= M_NULL; }

		m_ImageCtrl[Cam_No].ImageUpdate();
		return false;
	}
	m_ImageCtrl[Cam_No].ImageUpdate();
	return false;
}
//==================================================================================================================================================================
bool CMatrox::PatternFinderPosAll(int find_flag, double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag, bool errsaveflag, bool allsaveflag)
{
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	CString bbb;
	CString aaa;
	CString SaveFileName;
	CString fnSave;
	int Sort_Mode= 0;
	int i;
	*angle= 0.;	*posx= 0.; *posy= 0.;
	//Cam_No= eTOP_CAM_2X
	CRect setROI;
	double tor_x_Val= 0.0, tor_y_Val= 0.0;
	setROI.left= 0;	setROI.top= 0;	setROI.right= _INSP_IMAGE_SIZE_X;	setROI.bottom= _INSP_IMAGE_SIZE_Y;
	if(Cam_No== eBTM_CAM_2X || Cam_No== eBTM_CAM_20X)
	{
		if(ModelNo== mdBTM_2X_ROLL_START)			Sort_Mode= 1;
		else										Sort_Mode= 0;
		if(ParamFile()->Model_data.iRollAlignModeFlag[ROLL_ALIGN_PROC]== _NORMAL_SELECT)
			find_flag= ParamFile()->Model_data.iRollAlignModeFlag[ROLL_ALIGN_PROC];
		else
		{
			if(find_flag== _CENTER_SELECT || ParamFile()->Model_data.iRollAlignModeFlag[ROLL_ALIGN_PROC] == _CENTER_SELECT)
			{	find_flag= _CENTER_SELECT;		Sort_Mode= 0;	}
			else
			{
				if(ModelNo== mdBTM_2X_ROLL_START)
				{	find_flag= _LAST_SELECT;	Sort_Mode= 0;	}
				else
				{	find_flag= _FIRST_SELECT;	Sort_Mode= 0;	}
			}
		}

		find_flag= _NORMAL_SELECT;  Sort_Mode= (ModelNo) % 2;
	}
	else 
	{
		find_flag= _NORMAL_SELECT;  Sort_Mode= ModelNo % 2;
	}
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.0 && ParamFile()->Model_data.dLED_Pitch_Y> 0.0 && find_flag != _NORMAL_SELECT)
	{
		if(Cam_No== eBTM_CAM_2X) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0) * 1.2;
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0) * 1.2;
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0) * 1.5;
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0) * 1.5;
		}
		setROI.left= 0;
		setROI.top= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val);
		setROI.right= (LONG)(_INSP_IMAGE_SIZE_X);
		setROI.bottom= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val);
	}

	if((Cam_No== eBTM_CAM_2X || Cam_No== eTOP_CAM_2X))
	{
		setROI.left= ParamFile()->Model_data.SearchROI_left[Cam_No];
		setROI.top= ParamFile()->Model_data.SearchROI_top[Cam_No];
		setROI.right= ParamFile()->Model_data.SearchROI_right[Cam_No];
		setROI.bottom= ParamFile()->Model_data.SearchROI_bottom[Cam_No];
	}
	else
	{
		setROI.left= 0;
		setROI.top= 0;
		setROI.right= (LONG)(_INSP_IMAGE_SIZE_X);
		setROI.bottom= (LONG)(_INSP_IMAGE_SIZE_Y);
	}
	//--------------------------------
	CImageObj* pImg = pSystem.GetVisionComponent()->GetImageObj( Cam_No );

	if( ModelNo >= _MAX_FIND_MODEL )		return false;
	if(pImg->GetWidthBytes()<= 0 || pImg->GetHeight()<= 0) 
	{		AfxMessageBox(_T("Pattern-Image Error!"));		return false;	}
	Insp_OriginalImage = MbufAlloc2d( MilSystem,  pImg->GetWidthBytes(), pImg->GetHeight(), 8+M_UNSIGNED , M_IMAGE+M_PROC, NULL );
	MbufClear(Insp_OriginalImage, 0);
	MbufPut2d( Insp_OriginalImage, 0, 0, m_ImageCtrl[Cam_No].GetVisibleImageObject()->GetWidth(), 
		m_ImageCtrl[Cam_No].GetVisibleImageObject()->GetHeight(), m_ImageCtrl[Cam_No].GetVisibleImageObject()->GetImageBuffer() );
	//--------------------------------
	fnSave.Format (_T("%s\\%d년%d월%d일"), ParamFile()->strDirImagePath, st.wYear, st.wMonth, st.wDay); 	_wmkdir(fnSave);
	SaveFileName.Format (_T("%s\\Pat_FindImage_%02d%02d%02d_C%02dM%02d.tif"), fnSave, st.wHour, st.wMinute, st.wSecond, Cam_No, ModelNo);
	CStringA strMB( SaveFileName ) ;
#ifdef _MIL_VER_8
	MbufSave(strMB.GetBuffer (0), Insp_OriginalImage);
#else
	MbufSave(SaveFileName.GetBuffer (0), Insp_OriginalImage);
#endif

	////////////////////////////////////////////////

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
	MpatSetNumber(MilPatternModel[ModelNo], M_ALL) ; 


//	if(find_flag != _NORMAL_SELECT)
	if((Cam_No== eBTM_CAM_2X || Cam_No== eTOP_CAM_2X))
		MpatSetPosition(MilPatternModel[ModelNo], setROI.left, setROI.top, setROI.Width(), setROI.Height());
	else							MpatSetPosition(MilPatternModel[ModelNo], M_ALL, M_ALL, M_ALL, M_ALL);

/*
	if(Cam_No== eTOP_CAM_2X)
	{
		MgraColor(M_DEFAULT, M_RGB888(255,255,255));
		MgraRectFill(M_DEFAULT, Insp_OriginalImage, _INSP_CAM_SIZE_X-70, 0, _INSP_CAM_SIZE_X, _INSP_CAM_SIZE_Y);
		MgraRectFill(M_DEFAULT, Insp_OriginalImage, 0, 0, 170, _INSP_CAM_SIZE_Y);
	}
*/

	MimOpen(Insp_OriginalImage, Insp_OriginalImage, 1, M_GRAYSCALE);		
	MimClose(Insp_OriginalImage, Insp_OriginalImage, 1, M_GRAYSCALE);
	
	MappTimer(M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);
	MpatFindModel(Insp_OriginalImage, MilPatternModel[ModelNo], MilPatternResult[ModelNo]);
	/* Read the find time. */
	MappTimer(M_TIMER_READ+M_SYNCHRONOUS, &dTime);
	TRACE("The Pat_search time is %.1f ms\n", dTime*1000.0);

	number= MpatGetNumber(MilPatternResult[ModelNo], M_NULL);

	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

	CxGraphicObject& pGO = m_ImageCtrl[Cam_No].GetGraphicObject();

	MgraColor(M_DEFAULT, M_RGB888(255,50,50));
	MgraFontScale(M_DEFAULT, 1.5, 1.5);

	if(number >= 1 && number< (_MAX_LED_FIND_COUNT-10) )
	{
		modPosx = new double[number] ;
		modPosy = new double[number] ;
		Score = new double[number] ;

		/* Get the results for each model. */
		MpatGetResult(MilPatternResult[ModelNo],  M_SCORE, Score);
		MpatGetResult(MilPatternResult[ModelNo], M_POSITION_X, modPosx);
		MpatGetResult(MilPatternResult[ModelNo], M_POSITION_Y, modPosy);

		int  findno= 0;
		double minscore= 0.;
		CString str1 ;
		COLORTEXT clrText;
		COLORALIGNMARK clrMark;
		COLORELLIPSE clrEllipse;
		COLORBOX clrBox;
		double blobPosX= 0.;	double blobPosY= 0.;
		long cenx= 0, ceny= 0;
		POINT pt;

		int sssss= (int)(ParamFile()->System_data.dRectSizeX / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0*2.0));
		int sssss1= (int)(ParamFile()->System_data.dRectSizeY / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0*2.0));
		if(sssss< 5) sssss= 5;
		if(sssss1< 10) sssss1= 10;

		if(find_flag != _NORMAL_SELECT)
		{
			clrBox.CreateObject( RGB(255,250,200), setROI, PS_DOT, 1);
			pGO.AddDrawBox( clrBox );
		}
		if(number> 1) 
		{
			for(i= 0; i< number; i++) 
			{
				if(Score[i]< minscore) { minscore= Score[i]; findno= i; }
				// CreateObject( COLORREF dwFgColor, int _x1, int _y1, int _x2, int _y2, int nStyle = PS_SOLID, int nThickness=1, COLORREF dwBgColor=-1 );
				clrEllipse.CreateObject( RGB(150,255,150), modPosx[i]-2, modPosy[i]-2, modPosx[i]+2, modPosy[i]+2, PS_SOLID, 1);
				pGO.AddDrawEllipse( clrEllipse ); 
//				clrMark.CreateObject( PDC_BLUE, modPosx[i], modPosy[i], /*50*/sssss1, sssss/*10*/, PS_SOLID, 1);
	//			pGO.AddDrawAlignMark( clrMark ); 
				str1.Format(_T("%d"), i);
				pt.x =  (LONG)(modPosx[i]+20.0);		pt.y =  (LONG)(modPosy[i]+20.0);
				clrText.CreateObject( PDC_GREEN, pt, 6 );		clrText.SetText( str1 );
				pGO.AddDrawText( clrText );
			}
			double ret_x= 0.0, ret_y= 0.0;
			if(Cam_No== eBTM_CAM_20X || Cam_No== eBTM_CAM_20X)
			{

				if(Sort_Mode== 0)
					findno= SearchLED_Intersection(find_flag, Cam_No, Sort_Mode/*ModelNo % 2*/, number, modPosx, modPosy, &ret_x, &ret_y);
				else
					findno= SearchLED_Intersection(find_flag, Cam_No, Sort_Mode/*ModelNo % 2*/, number, modPosx, modPosy, &ret_x, &ret_y);
			}
			else
			{
				if(Sort_Mode== 0)
					findno= SearchLED_Intersection(find_flag, Cam_No, 0/*ModelNo % 2*/, number, modPosx, modPosy, &ret_x, &ret_y);
				else
					findno= SearchLED_Intersection(find_flag, Cam_No, 1/*ModelNo % 2*/, number, modPosx, modPosy, &ret_x, &ret_y);
			}
			if(findno > 0)
			{
				*posx= ret_x;	*posy= ret_y;	*angle= Score[0];	
				if(findno==2)
				{
					str1.Format(_T("#"));
					pt.x =  (LONG)(ret_x+20.0);		pt.y =  (LONG)(ret_y+20);
					clrText.CreateObject( PDC_GREEN, pt, 10 );		clrText.SetText( str1 );
					pGO.AddDrawText( clrText );
				}
			}
			else
			{	*posx= modPosx[0];	*posy= modPosy[0];	*angle= Score[0];	}
		}
		else
		{	*posx= modPosx[0];	*posy= modPosy[0];	*angle= Score[0];	}

		cenx= (long)(*posx); ceny= (long)(*posy);

		int tmpLimit= 0;

		if(overlayflag) 
		{
			if(ModelNo % 2) 
			{
				clrMark.CreateObject( PDC_BLUE, cenx, ceny, /*50*/sssss1, sssss/*10*/, PS_SOLID, 1);

				pt.x = 850;		pt.y = 15;
				str1.Format(_T("X:%3.1f,Y:%3.1f:%3.1f"), *posx, *posy, Score[findno]);
				clrText.CreateObject( PDC_BLUE, pt, 12 );		clrText.SetText( str1 );
			}
			else	
			{
				clrMark.CreateObject( PDC_GREEN, cenx, ceny, sssss1/*50*/, /*10*/sssss, PS_SOLID, 1);

				pt.x = 10;		pt.y = 15;
				str1.Format(_T("X:%3.1f,Y:%3.1f:%3.1f"), *posx, *posy, Score[findno]);
				clrText.CreateObject( PDC_GREEN, pt, 12 );		clrText.SetText( str1 );
				//-----------------------------------------
			}
			pGO.AddDrawAlignMark( clrMark ); 
			pGO.AddDrawText( clrText );

			//	MgraFontScale(M_DEFAULT, 1.0, 1.0);
			long dist= 100;
		}
		if(modPosx) delete [] modPosx ;
		if(modPosy) delete [] modPosy ;
		if(Score) delete [] Score ;

		m_strLogFileBuffer.Format(_T("[PAT_FIND] No=%d [%d/%d] X-%3.1f,Y-%3.1f [X-%3.1f,Y-%3.1f] [%.1f]"), 
			ModelNo, number, findno, *posx, *posy, blobPosX, blobPosY, dTime*1000.0); 
		SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;
		if(*angle<= 0.) *angle= 1.;

		if(Insp_OriginalImage)	{				MbufFree(Insp_OriginalImage);		Insp_OriginalImage= M_NULL; }

		return true;
	}
	else 
	{
		CString str1;
		CPoint pt;

		*angle= 0.;			*posx= 0.; 			*posy= 0.; 
		if(errsaveflag)
		{
			SaveFileName.Format (_T("%s\\ERROR_Pat_FindImage_%02d%02d%02d"), fnSave, st.wHour, st.wMinute, st.wSecond);
			aaa2.Format(_T("%s[%d].tif"), SaveFileName, ModelNo); 
			CStringA strMB( aaa2 ) ;
#ifdef _MIL_VER_8
			MbufSave(strMB.GetBuffer (0), Insp_OriginalImage);
#else
			MbufSave(aaa2.GetBuffer (0), Insp_OriginalImage);
#endif
		}
		if(overlayflag) 
		{
			COLORDCROSS clrCross;
			COLORTEXT clrText;
			pt.x = 500;		pt.y =100;
			str1.Format(_T("Find-Error!!!"));
			clrText.CreateObject( PDC_RED, pt, 20 );		clrText.SetText( str1 );

			pGO.AddDrawText( clrText );
		}
		m_strLogFileBuffer.Format(_T("[PAT_FIND] ERROR! No=%d"), ModelNo); 
		SaveLogFile(m_strLogFileBuffer); //AfxMessageBox(strLogFileBuffer) ;

		if(Insp_OriginalImage)	{				MbufFree(Insp_OriginalImage);		Insp_OriginalImage= M_NULL; }

		return false;
	}
	return false;
}
//==================================================================================================================================================================
void CMatrox::GetCam_ROICood(int CamPos, int lensno, double x, double y, double *retx, double *rety, bool h_rev, bool v_rev)
{
	double CamRes2X= 0.0, CamRes20X= 0.0;
	if(CamPos== eTOP_CAM)	{	CamRes2X= ParamFile()->System_data.dTop_Cam_PixelToum_2X; CamRes20X= ParamFile()->System_data.dTop_Cam_PixelToum_20X; }
	else					{	CamRes2X= ParamFile()->System_data.dBot_Cam_PixelToum_2X; CamRes20X= ParamFile()->System_data.dBot_Cam_PixelToum_20X; }
	if(lensno== e2X_LENS)
	{
		
		if(h_rev)			*retx= (((_INSP_IMAGE_SIZE_X/2) - x) * CamRes2X) / 1000.0; 
		else				*retx= ((x - (_INSP_IMAGE_SIZE_X/2 )) * CamRes2X) / 1000.0; 
		if(v_rev)			*rety= (((_INSP_IMAGE_SIZE_Y/2) - y) * CamRes2X) / 1000.0; 
		else				*rety= ((y - (_INSP_IMAGE_SIZE_Y/2 )) * CamRes2X) / 1000.0; 
	}
	else
	{
		if(h_rev)			*retx= (((_INSP_IMAGE_SIZE_X/2) - x) * CamRes20X) / 1000.0; 
		else				*retx= ((x - (_INSP_IMAGE_SIZE_X/2 )) * CamRes20X) / 1000.0; 
		if(v_rev)			*rety= (((_INSP_IMAGE_SIZE_Y/2) - y) * CamRes20X) / 1000.0; 
		else				*rety= ((y - (_INSP_IMAGE_SIZE_Y/2 )) * CamRes20X) / 1000.0; 
	}
}
void CMatrox::GetCam_ROICood_pixPoint(int lensno, double x, double y, double *retx, double *rety, bool h_rev, bool v_rev)
{
	if(lensno== e2X_LENS)
	{
		*retx= (_INSP_IMAGE_SIZE_X/2 ) + (x / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0)) ;
		*rety= (_INSP_IMAGE_SIZE_Y/2 ) + (y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0)) ;
	}
	else
	{
		*retx= (_INSP_IMAGE_SIZE_X/2 ) + (x / (ParamFile()->System_data.dTop_Cam_PixelToum_20X / 1000.0)) ;
		*rety= (_INSP_IMAGE_SIZE_Y/2 ) + (y / (ParamFile()->System_data.dTop_Cam_PixelToum_20X / 1000.0)) ;
	}
}

void CMatrox::GetCam_ROICood_Pos(int lensno, double x, double y, double *retx, double *rety, double cenx, double ceny, bool h_rev, bool v_rev)
{
	if(lensno== e2X_LENS)
	{

		if(h_rev)			*retx= ((cenx - x) * ParamFile()->System_data.dTop_Cam_PixelToum_2X) / 1000.0; 
		else				*retx= ((x - cenx) * ParamFile()->System_data.dTop_Cam_PixelToum_2X) / 1000.0; 
		if(v_rev)			*rety= ((ceny - y) * ParamFile()->System_data.dTop_Cam_PixelToum_2X) / 1000.0; 
		else				*rety= ((y - ceny) * ParamFile()->System_data.dTop_Cam_PixelToum_2X) / 1000.0; 
	}
	else
	{
		if(h_rev)			*retx= ((cenx - x) * ParamFile()->System_data.dTop_Cam_PixelToum_20X) / 1000.0; 
		else				*retx= ((x - cenx) * ParamFile()->System_data.dTop_Cam_PixelToum_20X) / 1000.0; 
		if(v_rev)			*rety= ((cenx - y) * ParamFile()->System_data.dTop_Cam_PixelToum_20X) / 1000.0; 
		else				*rety= ((y - ceny) * ParamFile()->System_data.dTop_Cam_PixelToum_20X) / 1000.0; 
	}
}
//입력된 좌표를 실세계 좌표로 바꾸어 준다.
void CMatrox::Trans_RealCoord(int CamLens,double c_x, double c_y, double *retx, double *rety)
{
	//ParamFile()->System_data.dRotCen_PosStageX_20X : 20배 기준 위치
	//ParamFile()->System_data.dTopCam20To2_X : 2배서 20배 렌즈 떨어진 거리
	double Cal_X, Cal_Y;
	double Cur_Pos_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	double Cur_Pos_CY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
	double Cur_Pos_SY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
	if(CamLens== e2X_LENS)
	{
		Cal_X= (ParamFile()->System_data.dRotCen_PosStageX_20X- (Cur_Pos_X  + ParamFile()->System_data.dTopCam20To2_X)) + c_x;
		Cal_Y= (ParamFile()->System_data.dRotCen_PosCamY_20X-   (Cur_Pos_CY + ParamFile()->System_data.dTopCam20To2_Y)) - c_y;
		*retx =Cal_X;
		*rety = Cal_Y;
	}
	else
	{
		Cal_X= (ParamFile()->System_data.dRotCen_PosStageX_20X- (Cur_Pos_X  )) + c_x;
		Cal_Y= (ParamFile()->System_data.dRotCen_PosCamY_20X-   (Cur_Pos_CY )) - c_y;
		*retx =Cal_X;
		*rety = Cal_Y;
	}
}
void CMatrox::Trans_RealCoord_FixPos(int CamLens,double c_x, double c_y, double *retx, double *rety, double curPosX, double curPosY, double curPosAY)
{
	//ParamFile()->System_data.dRotCen_PosStageX_20X : 20배 기준 위치
	//ParamFile()->System_data.dTopCam20To2_X : 2배서 20배 렌즈 떨어진 거리
	double Cal_X, Cal_Y;
	double Cur_Pos_X= curPosX;
	double Cur_Pos_CY= curPosY;
	double Cur_Pos_SY= curPosAY;
	if(CamLens== e2X_LENS)
	{
		Cal_X= (ParamFile()->System_data.dRotCen_PosStageX_20X- (Cur_Pos_X  + ParamFile()->System_data.dTopCam20To2_X)) + c_x;
		Cal_Y= (ParamFile()->System_data.dRotCen_PosCamY_20X-   (Cur_Pos_CY + ParamFile()->System_data.dTopCam20To2_Y)) - c_y;
		*retx =Cal_X;
		*rety = Cal_Y;
	}
	else
	{
		Cal_X= (ParamFile()->System_data.dRotCen_PosStageX_20X- (Cur_Pos_X  )) + c_x;
		Cal_Y= (ParamFile()->System_data.dRotCen_PosCamY_20X-   (Cur_Pos_CY )) - c_y;
		*retx =Cal_X;
		*rety = Cal_Y;
	}
}
double CMatrox::radianToDegree(double rad) {
	return (180.0 / PI)* rad;
}
double CMatrox::degreeToRadian(double deg) {
	return (PI / 180.0 ) * deg;
}
double CMatrox::GetAngle(double x1, double y1, double x2, double y2)
{
	double tempx,tempy;
	tempx = fabs(x1 - x2);	
	tempy = y1 - y2;
	if(fabs(tempx) < 0.00001 || fabs(tempy) < 0.00001)
	{
		//		AfxMessageBox("Can't Calibrate !! Moving Distance is too small !!");
		return 0.;
	}
	double val= atan((tempy)/tempx);
	double va2l= radianToDegree(val);
	
	val= atan2(tempx, tempy);
	va2l= radianToDegree(val);

	return va2l-90.0;
}
//////////////////////////////////////////////////////////////////////////////////
////////    회전후의좌표(이전좌표xy(중심부터의실세계),회전각도,이후xy(중심부터의실세계))
//////////////////////////////////////////////////////////////////////////////////
void CMatrox::GetRotationCoord(double m_xGlass,double m_yGlass,double theta, double *o_x, double *o_y)
{
	*o_x = cos(theta*RAD)*m_xGlass - sin(theta*RAD)*m_yGlass;
	*o_y = sin(theta*RAD)*m_xGlass + cos(theta*RAD)*m_yGlass;
}

//===================================================================================
void CMatrox::Calc_OrgPosition(double x1, double y1, double x2, double y2, double angle, double *retx, double *rety)
{
	double vec1x, vec1y, temp, tempx, tempy, vec2x, vec2y, dist;

	vec1x= (x1 + x2) / 2.0;
	vec1y= (y1 + y2) / 2.0;

	dist= sqrt( (vec1x - x1 ) * (vec1x-x1) + (vec1y-y1)*(vec1y-y1) ) * tan(RAD * (90.0 - angle / 2.0 ));

	tempx= x2-x1;
	tempy= y2-y1;

	temp = sqrt(tempx*tempx + tempy*tempy);
	tempx= tempx / temp;
	tempy= tempy / temp;
	GetRotationCoord(tempx,tempy,-90.0, &vec2x, &vec2y);

	*retx= vec1x - vec2x*dist;
	*rety= vec1y - vec2y*dist;
}
//===================================================================================
int CMatrox::SearchLED_Intersection(int find_flag, int Cam_Mode, int mode, int count, double *x_pos, double *y_pos, double *ret_x, double *ret_y)
{

	if(count > 1)
	{
		double dirX_min[2]= {0.0, 0.0 }, dirY_min[2]= {0.0, 0.0 };
		double dirX_max[2]= {0.0, 0.0 }, dirY_max[2]= {0.0, 0.0 };
		double *rPosx, *rPosy;
		double *r1stX_Posx, *r1stX_Posy;
		double *r1stY_Posx, *r1stY_Posy;

		int dirX_min_No= 0, dirY_min_No= 0;
		int dirX_max_No= 0, dirY_max_No= 0;
		int r1stX_Count= 0, r1stY_Count= 0;
		int i;

		double tor_x_Val= 0.0;
		double tor_y_Val= 0.0;
		if(Cam_Mode== eTOP_CAM_2X) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) / 2.0) * 1.5;
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) / 2.0) * 1.5;
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) / 2.0) * 1.5;
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) / 2.0) * 1.5;
		}

		rPosx = new double[count] ;				rPosy = new double[count] ;
		r1stX_Posx = new double[count] ;		r1stX_Posy = new double[count] ;
		r1stY_Posx = new double[count] ;		r1stY_Posy = new double[count] ;

		for(i= 0; i< count; i++)
		{
			if(i== 0) 
			{
				dirX_min_No= 0; dirY_min_No= 0; dirX_min[0]= x_pos[i]; dirX_min[1]= y_pos[i]; dirY_min[0]= x_pos[i]; dirY_min[1]= y_pos[i]; 
				dirX_max_No= 0; dirY_max_No= 0; dirX_max[0]= x_pos[i]; dirX_max[1]= y_pos[i]; dirY_max[0]= x_pos[i]; dirY_max[1]= y_pos[i]; 
			}
			if(dirX_min[0] > x_pos[i]) { dirX_min[0]= x_pos[i]; dirX_min[1]= y_pos[i]; dirX_min_No= i; }
			if(dirY_min[1] > y_pos[i]) { dirY_min[0]= x_pos[i]; dirY_min[1]= y_pos[i]; dirY_min_No= i; }
			if(dirX_max[0] < x_pos[i]) { dirX_max[0]= x_pos[i]; dirX_max[1]= y_pos[i]; dirX_max_No= i; }
			if(dirY_max[1] < y_pos[i]) { dirY_max[0]= x_pos[i]; dirY_max[1]= y_pos[i]; dirY_max_No= i; }
		}
		if(mode== 1) //x(세로방향)=min / y(가로방향)=max
		{
			for(i= 0; i< count; i++)
			{
				if(i== 0) 
				{
					r1stX_Count= 0; r1stX_Posx[0]= dirX_min[0]; r1stX_Posy[0]= dirX_min[1];
					r1stY_Count= 0; r1stY_Posx[0]= dirY_max[0]; r1stY_Posy[0]= dirY_max[1];
				}
				if((r1stX_Posx[0] + tor_x_Val) >  x_pos[i] && (r1stX_Posx[0] - tor_x_Val) < x_pos[i])	{	r1stX_Posx[r1stX_Count]= x_pos[i]; r1stX_Posy[r1stX_Count]= y_pos[i];	r1stX_Count++; }
				if((r1stY_Posy[0] + tor_y_Val) >  y_pos[i] && (r1stY_Posy[0] - tor_y_Val) < y_pos[i])	{	r1stY_Posx[r1stY_Count]= x_pos[i]; r1stY_Posy[r1stY_Count]= y_pos[i];	r1stY_Count++; }
			}
			int si, sj;
				for ( si = 0; si < r1stX_Count - 1; si++ )
				{
					for ( sj = 1; sj < r1stX_Count - si; sj++ )
					{
						double xx= 0.0, yy= 0.0;
						if(r1stX_Posy[sj-1] < r1stX_Posy[sj])
						{
							yy= r1stX_Posy[sj-1]; xx= r1stX_Posx[sj-1]; 
							r1stX_Posy[sj-1]= r1stX_Posy[sj]; r1stX_Posx[sj-1]= r1stX_Posx[sj]; 
							r1stX_Posy[sj]= yy; r1stX_Posx[sj]= xx; 
						}
					}
				}
				for ( si = 0; si < r1stY_Count - 1; si++ )
				{
					for ( sj = 1; sj < r1stY_Count - si; sj++ )
					{
						double xx= 0.0, yy= 0.0;
						if(r1stY_Posx[sj-1] > r1stY_Posx[sj])
						{
							yy= r1stY_Posy[sj-1]; xx= r1stY_Posx[sj-1]; 
							r1stY_Posy[sj-1]= r1stY_Posy[sj]; r1stY_Posx[sj-1]= r1stY_Posx[sj]; 
							r1stY_Posy[sj]= yy; r1stY_Posx[sj]= xx; 
						}
					}
				}
				if(r1stX_Count == 1 && r1stY_Count == 1)
				{
					*ret_x= r1stX_Posx[0]; *ret_y= r1stY_Posy[0]; 
					dirX_min_No= 1;
				}
				else if(r1stX_Count >= 1 && r1stY_Count >= 1)
				{
					if(find_flag == _NORMAL_SELECT)
					{
						if(r1stX_Posx[0] == r1stY_Posx[0] && r1stX_Posy[0] == r1stY_Posy[0])
						{
							*ret_x= r1stX_Posx[0]; *ret_y= r1stX_Posy[0]; 
							dirX_min_No= 1;
						}
						else
						{
							//		AfxMessageBox(_T("[SORT]"));
							*ret_x= r1stX_Posx[0]; *ret_y= r1stY_Posy[0]; 
							dirX_min_No= 2;
						}
					}
					else if(find_flag == _FIRST_SELECT)
					{
						*ret_x= r1stX_Posx[0]; *ret_y= r1stY_Posy[0]; 
						dirX_min_No= 1;
					}
					else if(find_flag == _CENTER_SELECT)
					{
						double cen_X= 0.0, cen_Y= 0.0;
						double cenRef= _INSP_IMAGE_SIZE_X / 2.0;
						cen_X= r1stY_Posx[0];						cen_Y= r1stY_Posy[0];
						for(int k= 0; k< r1stY_Count; k++)
						{
							if(fabs(cen_X-cenRef) > fabs(r1stY_Posx[k]-cenRef)) {	cen_X= r1stY_Posx[k];	cen_Y= r1stY_Posy[k]; }
						}
						*ret_x= cen_X; *ret_y= cen_Y; 
						dirX_min_No= 1;
					}
					else
					{
						if(r1stX_Count== 1)		{	*ret_x= r1stY_Posx[r1stY_Count-1]; *ret_y= r1stY_Posy[r1stY_Count-1]; }
						else					{	*ret_x= r1stY_Posx[r1stY_Count-1]; *ret_y= r1stY_Posy[r1stY_Count-1]; }
						dirX_min_No= 1;
					}
				}
				else
				{
				//	AfxMessageBox(_T("Find-Error!!! [SORT]"));
					*ret_x= dirY_min[0]; *ret_y= dirY_min[1]; 
					dirX_min_No= -1;
				}

			if(rPosx) delete [] rPosx ;
			if(rPosy) delete [] rPosy ;
			if(r1stX_Posx) delete [] r1stX_Posx ;
			if(r1stX_Posy) delete [] r1stX_Posy ;
			if(r1stY_Posx) delete [] r1stY_Posx ;
			if(r1stY_Posy) delete [] r1stY_Posy ;
			return dirX_min_No;
			//	return dirY_min_No;
		}
		else  //x(세로방향)= max / y(가로방향)= max
		{
			for(i= 0; i< count; i++)
			{
				if(i== 0) 
				{
					r1stX_Count= 0; r1stX_Posx[0]= dirX_max[0]; r1stX_Posy[0]= dirX_max[1];
					r1stY_Count= 0; r1stY_Posx[0]= dirY_max[0]; r1stY_Posy[0]= dirY_max[1];
				}
				if((r1stX_Posx[0] + tor_x_Val) >  x_pos[i] && (r1stX_Posx[0] - tor_x_Val) < x_pos[i])	{	r1stX_Posx[r1stX_Count]= x_pos[i]; r1stX_Posy[r1stX_Count]= y_pos[i];	r1stX_Count++; }
				if((r1stY_Posy[0] + tor_y_Val) >  y_pos[i] && (r1stY_Posy[0] - tor_y_Val) < y_pos[i])	{	r1stY_Posx[r1stY_Count]= x_pos[i]; r1stY_Posy[r1stY_Count]= y_pos[i];	r1stY_Count++; }
			}
			int si, sj;
			for ( si = 0; si < r1stX_Count - 1; si++ ) //X sorting
			{
				for ( sj = 1; sj < r1stX_Count - si; sj++ )
				{
					double xx= 0.0, yy= 0.0;
					if(r1stX_Posy[sj-1] < r1stX_Posy[sj])
					{
						yy= r1stX_Posy[sj-1]; xx= r1stX_Posx[sj-1]; 
						r1stX_Posy[sj-1]= r1stX_Posy[sj]; r1stX_Posx[sj-1]= r1stX_Posx[sj]; 
						r1stX_Posy[sj]= yy; r1stX_Posx[sj]= xx; 
					}
				}
			}
			for ( si = 0; si < r1stY_Count - 1; si++ ) //Y sorting
			{
				for ( sj = 1; sj < r1stY_Count - si; sj++ )
				{
					double xx= 0.0, yy= 0.0;
					if(r1stY_Posx[sj-1] < r1stY_Posx[sj])
					{
						yy= r1stY_Posy[sj-1]; xx= r1stY_Posx[sj-1]; 
						r1stY_Posy[sj-1]= r1stY_Posy[sj]; r1stY_Posx[sj-1]= r1stY_Posx[sj]; 
						r1stY_Posy[sj]= yy; r1stY_Posx[sj]= xx; 
					}
				}
			}
			if(r1stX_Count >= 1 && r1stY_Count >= 1) //체크 : 0:0 LED있는지 체크
			{
				if(r1stX_Posx[0] == r1stY_Posx[0] && r1stX_Posy[0] == r1stY_Posy[0])
				{
					*ret_x= r1stX_Posx[0]; *ret_y= r1stX_Posy[0]; 
					dirX_min_No= 1;
				}
				else
				{
					//		AfxMessageBox(_T("[SORT]"));
					*ret_x= r1stX_Posx[0]; *ret_y= r1stY_Posy[0]; 
					dirX_min_No= 2;
				}
			}
			else
			{
				//	AfxMessageBox(_T("Find-Error!!! [SORT]"));
				*ret_x= dirY_min[0]; *ret_y= dirY_min[1]; 
				dirX_min_No= -1;
			}

			if(rPosx) delete [] rPosx ;
			if(rPosy) delete [] rPosy ;
			if(r1stX_Posx) delete [] r1stX_Posx ;
			if(r1stX_Posy) delete [] r1stX_Posy ;
			if(r1stY_Posx) delete [] r1stY_Posx ;
			if(r1stY_Posy) delete [] r1stY_Posy ;
			return dirX_min_No;
		}
	}

	return -1;
}

//===================================================================================
void CMatrox::Test1()
{
	CString strText;
	for(int i= 0; i< ParamFile()->System_data.iCompCountX+1; i++)
	{
		MbufCopyClip(MilGrabImage[eTOP_CAM_20X], Proc_Tmp_MilImage[eTOP_CAM_20X], (i*10), 0L) ;

		strText.Format(_T("d:\\work\\top\\%03d.tif"), i);
		CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
		MbufSave(strMB.GetBuffer(0), Proc_Tmp_MilImage[eTOP_CAM_20X]);
#else
		MbufSave(strText.GetBuffer(0), Proc_Tmp_MilImage[eTOP_CAM_20X]);
#endif
	}
	for(int i= 0; i< ParamFile()->System_data.iCompCountX+1; i++)
	{
		MbufCopyClip(MilGrabImage[eBTM_CAM_20X], Proc_Tmp_MilImage[eBTM_CAM_20X], (i*20), 0L) ;

		strText.Format(_T("d:\\work\\Btm\\%03d.tif"), i);
		CStringA strMB( strText ) ;
#ifdef _MIL_VER_8
		MbufSave(strMB.GetBuffer(0), Proc_Tmp_MilImage[eBTM_CAM_20X]);
#else
		MbufSave(strText.GetBuffer(0), Proc_Tmp_MilImage[eBTM_CAM_20X]);
#endif
	}

}
