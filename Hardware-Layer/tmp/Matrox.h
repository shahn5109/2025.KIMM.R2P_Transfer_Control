// Matrox.h: interface for the CMatrox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATROX_H__8EBEC8FC_9BA0_491F_A6C6_95FF2B12B2B0__INCLUDED_)
#define AFX_MATROX_H__8EBEC8FC_9BA0_491F_A6C6_95FF2B12B2B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mil.h"
#include "VisionInfo.h"

typedef enum _LOG_LEVEL{
	_LOG_NONE,  _LOG_LOW,  _LOG_MID,  _LOG_HIGH,  _LOG_DETAIL
}LOG_LEVEL;

#define CONTOUR_MAX_RESULTS         100
#define CONTOUR_MAXIMUM_ELONGATION  0.8
#define CONTOUR_DRAW_COLOR          M_COLOR_GREEN
#define CONTOUR_LABEL_COLOR         M_COLOR_RED

class CMatrox  
{ 
public:
	CMatrox(); 
	virtual ~CMatrox(); 

public:
	/* MIL variables */
	MIL_ID  MilApplication,	// MIL Application identifier.
		MilDisplayControl[_MAX_VIEW_],
		MilGrabImage[_MAX_VIEW_],
		MilDisplayImage[_MAX_VIEW_],
		MilDigitizer[_MAX_VIEW_], 
		MilOverlayImage[_MAX_VIEW_],
		Insp_Tmp_MilImage[_MAX_TMPVIEW_],
		Insp_ColorImage[_MAX_VIEW_],
		TransparentColor,
		MilMeasureMarker,
		MilEdgeContext,                         /* Edge context.             */ 
		MilEdgeResult,                          /* Edge result identifier.   */   
		MilGraphicContext,
		DimFeatureList, DimBlobResult,
		DimFeatureList2, DimBlobResult2,
		//////////// Model /////////////////]////////////////////
//			MilSearchContext[_MAX_FIND_MODEL],                            
//			MilModelResult[_MAX_FIND_MODEL],                      
//			ModelSetDispImage,
//			ModelSetDisplay,
//			MilSetOverlayImage,
//			ModelDontCareImage,
		/////////////////////////////]////////////////////
			MilPatternModel_1[_MAX_FIND_MODEL],
			MilPatternResult_1[_MAX_FIND_MODEL],
			PatternSetDispImage_1,
			PatternSetDisplay_1,
			PatternSetOverlayImage_1,
			PatternDontCareImage_1,
			//-------------------------------------------
			MilPatternModel_2[_MAX_FIND_MODEL],
			MilPatternResult_2[_MAX_FIND_MODEL],
			PatternSetDispImage_2,
			PatternSetDisplay_2,
			PatternSetOverlayImage_2,
			PatternDontCareImage_2,
		/////////////////////////////////////////////////
		ZoomCamDispDisplay_1,
		ZoomCamDispImage_1,
		NormalCamDispDisplay_1,
		NormalCamDispImage_1,

		ZoomCamDispDisplay_2,
		ZoomCamDispImage_2,
		NormalCamDispDisplay_2,
		NormalCamDispImage_2,
		MilSystem;				// MIL System identifier.  

	double MeanFeretDiameter[CONTOUR_MAX_RESULTS]; /* Edge mean Feret diameter. */
	double Fit_Error[CONTOUR_MAX_RESULTS]; /* Edge mean Feret diameter. */
	double Fit_Cen_X[CONTOUR_MAX_RESULTS]; /* Edge mean Feret diameter. */
	double Fit_Cen_Y[CONTOUR_MAX_RESULTS]; /* Edge mean Feret diameter. */
	
	CDC m_pDC1[_MAX_VIEW_];	
	int	Vision_Grab_Mode_1;
	int	Vision_Grab_Mode_2;
	CDC m_pDC ;

	int iLogLevel;
//	HWND	ModelSetHwnd; 
	HWND	PatternSetHwnd_1; 
	HWND	PatternSetHwnd_2; 
	
	HWND	ZoomCamSetHwnd_1;
	HWND	NormalCamSetHwnd_1;
	HWND	ZoomCamSetHwnd_2;
	HWND	NormalCamSetHwnd_2;

	void ImageDataSave(unsigned char *buf, CString csFileName);

//	CString strDebufSaveFile;
	/////////////////////////////////////////////////////////////////////////////
public:
	//===================================================================================
	char strCurrentModelName[255]; // 현재 사용중인 모델명 기억
	char szParamFile[255], szInspSettingParamFile[255], szLogFile[255];
	
	char strDirRootPath[255];			// 프로그램 루트 디렉토리
	char strDirConfigPath[255];			// 시스템 Config 디렉토리
	char strDirRecipePath[255];			// Model 별 Recipe 디렉토리
	char strDirProgramPath[255];		// 실행 파일 디렉토리
	char strDirSystemLogPath[255];		// 로그 디렉토리
	char strDirVisionLogName[255];
	char strDirDefName[255];			// 각종 기타 Define된 내용 디렉토리
	char strDirImagePath[255];			// 이미지 저장 디렉토리
	char strDirDebugImage[255];			// 디버그용 이미지 저장 디렉토리
	char strDirCalibrationImagePath[255];
	char strDirReportName[255];
	CString main_dcf_file;
	//===================================================================================
	// Vision_Initial_Flag	Vision_VGA_Initial_Flag   Vision_DIg_Initial_Flag
	//	false					false						false				=> 비젼부 전체 사용 못함
	//	true					true						false				=> VGA-Mode로 사용
	//	true					false						true				=> Grab 가능 상태
	//	true					false						false				=> 비젼부 전체 사용 못함
	//----------------------------------------------------------------------------
	bool Vision_Initial_Flag;
	bool Vision_VGA_Initial_Flag;
	bool Vision_DIg_Initial_Flag;
//	CString strLogFileBuffer;
	int m_CurrentDisplay;
	bool VISION_SYS_MODE;
	//===================================================================================
	bool GetEdgeLine(int proc_count, CRect rt, double *pos_x);

	int is_SysMode();

	void DrawOffsetCross_1();
	void DrawOffsetCross_2();
	void Cam_ExpTimeDelayChange();
 
	void Cam_InputGainChange(int ch);
	void Cam_ReferenceChange(int ch);
	void Cam_ExposureChange(int ch);

	void InitLeftCamSet_1(HWND hwnd);
	void InitRightCamSet_1(HWND hwnd);
	void InitLeftCamSet_2(HWND hwnd);
	void InitRightCamSet_2(HWND hwnd);
	//==========================================================================
	void Debug_Flag_Set(int LogLevel);
	//==========================================================================
	void Set_SysMode(int sys_mode);
	//==========================================================================
	void SaveLogFile(CString strText);
	//==========================================================================
	bool Open();
	void Close();
	//==========================================================================
	bool CreateMainWindow(HWND UserWindow1, HWND UserWindow2, HWND UserWindow3, HWND UserWindow4);
	//==========================================================================
	bool InspUseGrab_INSP_2_F_CAM_FILM() ;
	bool InspUseGrab_INSP_2_F_CAM_PATTERN() ;
	bool InspGrab_1(int ch) ;
	bool InspGrabContinueStop_1() ;
	bool Insp_isGrabContinue_1() ;
	bool InspLiveUseGrab_1(int ch) ;
	bool InspGrabContinue_1();
	bool InspGrabTriggerModeChange_1(bool flag) ;

	bool InspUseGrab_INSP_2_R_CAM_ZOOM() ;
	bool InspUseGrab_INSP_2_R_CAM_NORMAL() ;
	bool InspGrab_2(int ch) ;
	bool InspGrabContinueStop_2() ;
	bool Insp_isGrabContinue_2() ;
	bool InspLiveUseGrab_2(int ch) ;
	bool InspGrabContinue_2();
	bool InspGrabTriggerModeChange_2(bool flag) ;
	//==========================================================================
	bool InspImageLoad_1(int ch, CString strName) ;
	bool InspImageSave_1(int ch, CString strName) ;
	bool InspImageLoad_2(int ch, CString strName) ;
	bool InspImageSave_2(int ch, CString strName) ;
	//==========================================================================
	void OverlayClear_1(int ID_No);
	void OverlayClear_2(int ID_No);

	void Insp_ROI_Display(int ch, CRect *rt, int ROI_Count, int dR, int dG, int dB);

	void DrawRect_1(int ID_No, bool overlayclear, CRect rt, int dR, int dG, int dB);
	void DrawLine_1(int ID_No, bool overlayclear, CPoint ptStart, CPoint ptEnd, int dR, int dG, int dB);
	void DrawDot_1(int ID_No, bool overlayclear, CPoint ptPoint, int dR, int dG, int dB);
	void DrawText_1(int ID_No, bool overlayclear, CPoint ptPoint, CString strText, int dR, int dG, int dB);
	void DrawText_Test(int ID_No, bool overlayclear, CPoint ptPoint, CString strText, int dR, int dG, int dB);

	void DrawRect_2(int ID_No, bool overlayclear, CRect rt, int dR, int dG, int dB);
	void DrawLine_2(int ID_No, bool overlayclear, CPoint ptStart, CPoint ptEnd, int dR, int dG, int dB);
	void DrawDot_2(int ID_No, bool overlayclear, CPoint ptPoint, int dR, int dG, int dB);
	void DrawText_2(int ID_No, bool overlayclear, CPoint ptPoint, CString strText, int dR, int dG, int dB);
	//===================================================================================
	void DrawDefaultCross_1(bool flag=true);
	void DrawDefaultCross_2();

	double FindStripe(int DispID, int Insp_No, CRect rtRange, double dAngle, CPoint cpCenter, int sel, bool ThEnable= false, bool ThAuto= false, long ThVal= 0, bool initFlag= true);
	double FindStripeLineToLine(int DispID, int Insp_No, CRect rtRange, double dAngle, CPoint cpCenter, int sel, bool ThEnable= false, bool ThAuto= false, long ThVal= 0, bool initFlag= true);
	bool PatternFinderPos_1_Test(int no, double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);


	void InitModelSet(HWND hwnd);
	int	 ModelLoad(int index);
	void ModelSave(int index);
	void ModelReDefine(int ModelIndex, int Cam_No, POINT pOldPoint, POINT pNewPoint);
	void MaskEditClear(int ModelIndex, double zoom);
	void MaskApply(int ModelIndex, double zoom);
	void DrawFillPoint(CPoint point, long factor, long size);
	void ModelImageLoadNdisp(int ModelIndex);
	bool ModelFinderPos(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	bool ModelFinderPos_R(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	void ThresholdRectArea(int ID_No, long value, CRect rt);
	void MeasureResultImageSave(CString strText, int ch);
	void MeasureProcess(CString strDirMeasImageSavePath, int Meas_no);
	void MeasureStartProcess();


	void InitModelSet_1(HWND hwnd);
	void InitModelSet_2(HWND hwnd);
	int	 PatternLoad_1(int index);
	void PatternSave_1(int index);
	int	 PatternLoad_2(int index);
	void PatternSave_2(int index);

	void PatternReDefine_1(int ModelIndex, int Cam_No, POINT pOldPoint, POINT pNewPoint);
	void PatternReDefine_2(int ModelIndex, int Cam_No, POINT pOldPoint, POINT pNewPoint);

	void PatternMaskEditClear_1(int ModelIndex, double zoom);
	void PatternMaskEditClear_2(int ModelIndex, double zoom);
	void PatternMaskApply_1(int ModelIndex, double zoom);
	void PatternMaskApply_2(int ModelIndex, double zoom);
	void PatternDrawFillPoint_1(CPoint point, long factor, long size);
	void PatternDrawFillPoint_2(CPoint point, long factor, long size);
	void PatternImageLoadNdisp_1(int ModelIndex);
	void PatternImageLoadNdisp_2(int ModelIndex);

	bool PatternFinderPos_Test(int i_no, double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	bool PatternFinderPos_1(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	bool PatternFinderPos_R_1(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);

	bool PatternFinderPos_2(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	bool PatternFinderPos_R_2(double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);

	void ZOOM_To_NORMAL_CAL(int B_X, int B_Y);
	void NORMAL_To_ZOOM_CAL(int B_X, int B_Y);

	void DrawDefaultCross_Test();
	bool InspImageSave_Test(CString strName) ;


	long		Model_AllocSizeX,
				Model_AllocSizeY;
	long		Pattern_AllocSizeX,
				Pattern_AllocSizeY;

private:
	CParameterFile*			ModelFile();
	//----------------------------------------
	// Multi-Thread
	CWinThread *m_pThread[4];
	BOOL	m_bExec[4];
	void RunThread(int nID);
	void KillThread(int nID);
	//----------------------------------------
};
//===================================================================================
#pragma comment(lib, "mil.lib")
//#pragma comment(lib, "milmet2.lib")
#pragma comment(lib, "milim.lib")
#pragma comment(lib, "milmeas.lib")
#pragma comment(lib, "milblob.lib") 
#pragma comment(lib, "milmod.lib")
#pragma comment(lib, "milpat.lib")
#pragma comment(lib, "miledge.lib")
//===================================================================================
typedef struct tagTREADPARAMS {
	BOOL *pContinue;
	CMatrox *pWnd;
} THREADPAPAMS;
UINT ThreadFuncLiveProc_0(LPVOID pParam);
UINT ThreadFuncLiveProc_1(LPVOID pParam);
UINT ThreadFuncLiveProc_2(LPVOID pParam);
UINT ThreadFuncLiveProc_3(LPVOID pParam);

#endif // !defined(AFX_MATROX_H__8EBEC8FC_9BA0_491F_A6C6_95FF2B12B2B0__INCLUDED_)


