// Matrox.h: interface for the CMatrox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATROX_H__8EBEC8FC_9BA0_491F_A6C6_95FF2B12B2B0__INCLUDED_)
#define AFX_MATROX_H__8EBEC8FC_9BA0_491F_A6C6_95FF2B12B2B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _MIL_VER_8
#define _MIL_VER_9

#include "mil.h"
#include "VisionInfo.h"
#include "../Common/ImageView/Include/ImageObj.h"
#include "../Common/ImageView/Include/xImageViewCtrl.h"

typedef enum _LOG_LEVEL{
	_LOG_NONE,  _LOG_LOW,  _LOG_MID,  _LOG_HIGH,  _LOG_DETAIL
}LOG_LEVEL;

#define _MAX_AF_BUF_COUNT	65
#define _MAX_AF_MAX_COUNT	64

class CMatrox  
{
public:
	CMatrox();
	virtual ~CMatrox();


	//protected:
	//--------------------------------------------------------
	MIL_ID  MilApplication,	// MIL Application identifier.
		MilDigitizer[_MAX_VIEW_],
		Insp_Tmp_MilImage[_MAX_TMPVIEW_],
		Insp_OriginalImage,
		Insp_BinarizeImage,
		MilTmpColorBuffer,
		MilTmp2ColorBuffer,
		MilGrabImage[_MAX_VIEW_],
		Proc_Tmp_MilImage[_MAX_VIEW_],

		MilFocusImage[_MAX_AF_BUF_COUNT],
		MilAutoFocusImage,
		MilSubImage,

		TransparentColor,
		//////////// Model /////////////////]////////////////////
		MilSearchContext[_MAX_FIND_MODEL],    
		MilSearchContext_Mask[_MAX_FIND_MODEL],
		MilModelResult[_MAX_FIND_MODEL],                      
		/////////////////////////////]////////////////////

		/////////////////////////////]////////////////////
		MilPatternModel[_MAX_FIND_MODEL],
		MilPatternResult[_MAX_FIND_MODEL],
		PatternSetDispImage,
		PatternSetDisplay,
		PatternSetOverlayImage,
		PatternDontCareImage,
		PatternSetDispImage2,
		PatternSetDisplay2,
		PatternSetOverlayImage2,
		PatternDontCareImage2,
	//--------------------------------------------------------
		MilEdgeContext,
		MilEdgeResult,

		MilSystem;				// MIL System identifier. 
	//--------------------------------------------------------

protected:
	//////////////////////////////////////////////////////////////////////////
	// Viewer
	CxImageViewManager	m_ImgCtrlMng;
	CxImageViewCtrl		m_ImageCtrl[_MAX_VIEW_];
	CImageObj			m_ImgObj[_MAX_VIEW_];
	CxImageViewCtrl*	m_pImageCtrl;

public:
	double Z_AF_pos[_MAX_AF_BUF_COUNT];
	int AF_GrabCamera, AF_GrabCount;
	bool AF_GrabStart ,AF_ChChg_Flag;
	bool AF_ProcStart(int ch) ;
	bool AF_ProcStop() ;
	long UserData[3];	/* User data passed to the hook  */

	bool AF_End_Flag;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Autofocus hook function that is responsible to move the lens */
	/****************************************************************/

	void AF_BufferToProcBuffer(int Camno, int cnt);
	void AF_BufferSave(int cnt);
	void AF_BufferLoad(int cnt, CString strPath);

	long FocusPos;		/* Best focus position           */
	DWORD FocusMaxNBPosition;
	DWORD FocusMinPosition;
	DWORD FocusMaxPosition;
	DWORD FocusBestPosition;
	DWORD FocusStartPosition;
	DWORD FocusMaxPositionVariation;
	BOOL  AutoFocusValue(DWORD MaxNBPos, DWORD MinPos, DWORD MaxPos,
		DWORD BestPos, DWORD StartPos, DWORD MaxPosVar);
	int AutoFocus();

	CString m_main_dcf_file;

	int m_iLogLevel;
	CString m_strLogFileBuffer;

	bool m_VISION_SYS_MODE;
	int	m_Vision_Grab_Mode;
	int	m_nSelectedCamForLive;

	bool Vision_Initial_Flag;
	bool Vision_VGA_Initial_Flag;
	bool Vision_DIg_Initial_Flag;

	//--------------------------------------------------------
	void		InitViewer( CWnd* pParent );
	void		GetImageViewer( CxImageViewCtrl*& imgCtrl );
	CxImageViewCtrl* GetImageViewer( int nViewIdx );
	void		GetImageObj( CImageObj*& ImgObj );
	CImageObj*	GetImageObj( int nViewIdx );
	void		SetCameraResolution();

	void SetTrackerMode( int nViewIdx, BOOL bTracking ); //Vision Set ROI Mode
	void SetROIEmpty();
	void SetROI( CRect& rcRect );
	CRect GetROI();

	void SetTempImageCtrl( CxImageViewCtrl* pView );

	void DrawUpdate(int DispNo);
	//--------------------------------------------------------
	void ClearAutoFocusImage();
	//--------------------------------------------------------

	HWND	ModelSetHwnd;
	HWND	PatternSetHwnd, PatternSetHwnd2; 
	long	Pattern_AllocSizeX,		Pattern_AllocSizeY;
	long	Pattern_AllocSizeX2,		Pattern_AllocSizeY2;
	long	Model_AllocSizeX,		Model_AllocSizeY;

public:
	//==========================================================================
	int mSel_TopCam_No, mSel_BotCam_No;
	void SaveLogFile(CString strText);

	bool Open();
	void Close();

	bool Save_Image(int cam_no, CString strName);
	bool Load_Image(int cam_no, CString strName);

	void ImageLiveCall(int ch);

	void InitModelSet_1(HWND hwnd);
	void InitModelSet_2(HWND hwnd);
	void PatternImageLoadNdisp(int ModelIndex);
	void PatternImageLoadNdisp2(int ModelIndex);
	void PatternReDefine( int nIdx, CImageObj* pImgObj , int ModelIndex);
	void PatternReDefine2( int nIdx, CImageObj* pImgObj , int ModelIndex);
	void PatternTmpImageDefine( int nIdx, CImageObj* pImgObj , int ModelIndex, CPoint ptPos);
	void PatternMaskEditClear(int ModelIndex, double zoom);
	void PatternMaskApply(int ModelIndex, double zoom);
	void PatternDrawFillPoint(CPoint point, long factor, long size);

	bool InspLiveUseGrabChChange(int ch) ;
	bool InspGrab(int ch) ;
	bool InspGrabContinueStop() ;
	bool Insp_isGrabContinue() ;
	bool InspLiveUseGrab(int ch= 0) ;
	bool InspGrabContinue( int ch= 0 );

	int  PatternLoad(int index);
	int  PatternCopy(int index, int toIndex);
	void PatternSave(int index);

	void PatDispZoom(double val);
	bool PatternFinderPos(double *angle, double *posx, double *posy, int ModelNo, int Cam_No,
		bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	bool PatternFinderPosAll(int find_flag, double *angle, double *posx, double *posy, int ModelNo, int Cam_No, bool overlayflag= true, bool errsaveflag= true, bool allsaveflag= true);
	void OverlayClear(int camno);

	void GetCam_ROICood_Pos(int lensno, double x, double y, double *retx, double *rety, double cenx, double ceny, bool h_rev= false, bool v_rev= false);
	void Trans_RealCoord(int CamLens, double x, double y, double *retx, double *rety);
	void Trans_RealCoord_FixPos(int CamLens,double c_x, double c_y, double *retx, double *rety, double curPosX, double curPosY, double curPosAY);
	void GetCam_ROICood(int CamPos, int CamLens, double x, double y, double *retx, double *rety, bool h_rev= false, bool v_rev= false);
	void GetCam_ROICood_pixPoint(int lensno, double x, double y, double *retx, double *rety, bool h_rev= false, bool v_rev= false);

	double GetAngle(double x1, double y1, double x2, double y2);
	double radianToDegree(double rad);
	double degreeToRadian(double deg);
	void GetRotationCoord(double m_xGlass,double m_yGlass,double theta, double *o_x, double *o_y);
	void Calc_OrgPosition(double x1, double y1, double x2, double y2, double angle, double *retx, double *rety);

	int SearchLED_Intersection(int find_flag, int Cam_Mode, int mode, int count, double *x_pos, double *y_pos, double *ret_x, double *ret_y);
	//==========================================================================
	void Test1();
public:
	CParameterFile*			ParamFile();
	CParameterFile*			Log();

private:
	//----------------------------------------
	// Multi-Thread
	CWinThread *m_pThread[2];
	BOOL	m_bExec[2];
	void RunThread(int nID);
	void KillThread(int nID);
	static UINT ThreadFuncLiveProc(LPVOID pParam);;
	//----------------------------------------

	CRect			m_rcROI;
};
//===================================================================================
#pragma comment(lib, "mil.lib")
//#pragma comment(lib, "milmet2.lib")
#pragma comment(lib, "milim.lib")
#pragma comment(lib, "milmeas.lib")
#pragma comment(lib, "milblob.lib")
#pragma comment(lib, "milpat.lib")
#pragma comment(lib, "miledge.lib")
#pragma comment(lib, "milmod.lib")
//===================================================================================

#pragma pack(push, 1)
typedef struct tagTREADPARAMS 
{
	BOOL *pContinue;
	CMatrox *pWnd;
} THREADPAPAMS;
#pragma pack(pop, 1)

#endif // !defined(AFX_MATROX_H__8EBEC8FC_9BA0_491F_A6C6_95FF2B12B2B0__INCLUDED_)


