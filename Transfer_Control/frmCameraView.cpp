IndexCamera
// frmCameraView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmCameraView.h"
#include "MainFrm.h"


// CfrmCameraView
extern CSystemClass pSystem;
CParameterFile* CfrmCameraView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmCameraView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmCameraView, CFormView)

	CfrmCameraView::CfrmCameraView()
	: CFormView(CfrmCameraView::IDD)
{

}

CfrmCameraView::~CfrmCameraView()
{
}

void CfrmCameraView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_JOG_SELECT_0, m_btnTopCam_2X);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnTopCam_20X);
	DDX_Control(pDX, IDB_JOG_SELECT_2, m_btnBotCam_2X);
	DDX_Control(pDX, IDB_JOG_SELECT_3, m_btnBotCam_20X);
	DDX_Control(pDX, IDB_JOG_SELECT_4, m_btnTopCam_Live);
	DDX_Control(pDX, IDB_JOG_SELECT_5, m_btnBotCam_Live);
}

BEGIN_MESSAGE_MAP(CfrmCameraView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CfrmCameraView::OnBnClickedButton1)
END_MESSAGE_MAP()


// CfrmCameraView 진단입니다.

#ifdef _DEBUG
void CfrmCameraView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmCameraView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmCameraView 메시지 처리기입니다.

//===================================================================================
void CfrmCameraView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_CameraView(this);

	mTopCameraNo= e2X_LENS;
	mBotCameraNo= e2X_LENS;

	bBotCam_Live= _INSP_CAM_GRAB_CONTINUE;	bTopCam_Live= _INSP_CAM_GRAB_CONTINUE;
	m_btnTopCam_Live.SetValue(1);	m_btnTopCam_Live.SetCaption(_T("LIVE"));
	m_btnBotCam_Live.SetValue(1);	m_btnBotCam_Live.SetCaption(_T("LIVE"));

	m_pVision->mSel_TopCam_No= e2X_LENS; m_pVision->mSel_BotCam_No= e2X_LENS;
	CameraSeletUpdate();
	m_pVision->InspGrabContinue(eTOP_CAM_2X) ;
	m_pVision->SetCameraResolution();

//	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (_INSP_IMAGE_SIZE_X/2.0);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (_INSP_IMAGE_SIZE_Y/2.0);
//	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (_INSP_IMAGE_SIZE_X/2.0);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (_INSP_IMAGE_SIZE_Y/2.0);

	g_bCfrmCameraViewInit= TRUE;
}
//===================================================================================
int CfrmCameraView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	pSystem.VisionSystemInitial();
	if(pSystem.IsVisionSystemInitial()) {
		// 		bool ret= m_pVision->CreateMainWindow(m_Display_LEFT.m_hWnd, m_Display_RIGHT.m_hWnd, m_Display_LEFT_2.m_hWnd, m_Display_RIGHT_2.m_hWnd);
		// 		if(ret)
		// 		{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[Vision] Insp_CreateWindow!"));
		// 		}
		// 		else	Log()->SaveLogFile(_SYSTEM_LOG, _T("[Vision] Insp_CreateWindow Fail!"));
	}
	//	pFrm->CamPopupInit();

	// 	for(int i= 0; i< _MAX_FIND_MODEL; i++) {
	// 		pSystem.GetVisionComponent()->PatternLoad(i);
	// 	}

	m_pVision= pSystem.GetVisionComponent();

	pSystem.GetVisionComponent()->InitViewer( this );
	pSystem.GetVisionComponent()->GetImageViewer( m_pImageCtrl );
	pSystem.GetVisionComponent()->GetImageObj( m_pImgObj );

	m_pImageCtrl[eTOP_CAM_20X].ShowWindow( SW_HIDE );
	m_pImageCtrl[eTOP_CAM_2X].ShowWindow( SW_SHOW );
	m_pImageCtrl[eBTM_CAM_20X].ShowWindow( SW_HIDE );
	m_pImageCtrl[eBTM_CAM_2X].ShowWindow( SW_SHOW );

	REGISTER_CALLBACK regCB;
	memset( &regCB, 0, sizeof(REGISTER_CALLBACK) );
	regCB.lpUsrData[0] = regCB.lpUsrData[1] = regCB.lpUsrData[2] =
		regCB.lpUsrData[3] = regCB.lpUsrData[4] = this;

	regCB.fnOnConfirmTracker = OnConfirmTracker;
	regCB.fnOnFireMouseEvent = OnFireMouseEvent;
	regCB.fnOnDrawExt = OnDrawExt;

	m_pImageCtrl[eTOP_CAM_20X].SetRegisterCallBack( 0, &regCB );
	m_pImageCtrl[eTOP_CAM_2X].SetRegisterCallBack( 1, &regCB );
	m_pImageCtrl[eBTM_CAM_20X].SetRegisterCallBack( 2, &regCB );
	m_pImageCtrl[eBTM_CAM_2X].SetRegisterCallBack( 3, &regCB );

	return 0;
}

//===================================================================================
void CfrmCameraView::OnDestroy()
{
	CFormView::OnDestroy();

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmCameraView::OnSize(UINT nType, int cx, int cy)
{
///	CFormView::OnSize(nType, cx, cy);

	CRect rcStaticSize;
	WINDOWPLACEMENT wndPlacement;
	CWnd*	pWnd = NULL;

	//////////////////////////////////////////////////////////////////////////
	// Image View Control Check
	if( m_pImageCtrl[eTOP_CAM_20X].GetSafeHwnd() )
	{
		pWnd = GetDlgItem( IDC_STATIC_TOP );
		if( pWnd != NULL )
		{
			pWnd->GetWindowPlacement( &wndPlacement );

			pWnd->GetClientRect( &rcStaticSize );

			m_pImageCtrl[eTOP_CAM_20X].SetParent( this );
			m_pImageCtrl[eTOP_CAM_20X].MoveWindow( wndPlacement.rcNormalPosition.left, wndPlacement.rcNormalPosition.top, rcStaticSize.Width(), rcStaticSize.Height() );
			m_pImageCtrl[eTOP_CAM_20X].SetTitleText( _T("TOP_10X") );
		}
	}

	if( m_pImageCtrl[eTOP_CAM_2X].GetSafeHwnd() )
	{
		pWnd = GetDlgItem( IDC_STATIC_TOP );
		if( pWnd != NULL )
		{
			pWnd->GetWindowPlacement( &wndPlacement );

			pWnd->GetClientRect( &rcStaticSize );

			m_pImageCtrl[eTOP_CAM_2X].SetParent( this );
			m_pImageCtrl[eTOP_CAM_2X].MoveWindow( wndPlacement.rcNormalPosition.left, wndPlacement.rcNormalPosition.top, rcStaticSize.Width(), rcStaticSize.Height() );
			m_pImageCtrl[eTOP_CAM_2X].SetTitleText( _T("TOP_2X") );
		}
	}

	if( m_pImageCtrl[eBTM_CAM_20X].GetSafeHwnd() )
	{
		pWnd = GetDlgItem( IDC_STATIC_BOT );
		if( pWnd != NULL )
		{
			pWnd->GetWindowPlacement( &wndPlacement );

			pWnd->GetClientRect( &rcStaticSize );

			m_pImageCtrl[eBTM_CAM_20X].SetParent( this );
			m_pImageCtrl[eBTM_CAM_20X].MoveWindow( wndPlacement.rcNormalPosition.left, wndPlacement.rcNormalPosition.top, rcStaticSize.Width(), rcStaticSize.Height() );
			m_pImageCtrl[eBTM_CAM_20X].SetTitleText( _T("BTM_10X") );
		}
	}

	if( m_pImageCtrl[eBTM_CAM_2X].GetSafeHwnd() )
	{
		pWnd = GetDlgItem( IDC_STATIC_BOT );
		if( pWnd != NULL )
		{
			pWnd->GetWindowPlacement( &wndPlacement );

			pWnd->GetClientRect( &rcStaticSize );

			m_pImageCtrl[eBTM_CAM_2X].SetParent( this );
			m_pImageCtrl[eBTM_CAM_2X].MoveWindow( wndPlacement.rcNormalPosition.left, wndPlacement.rcNormalPosition.top, rcStaticSize.Width(), rcStaticSize.Height() );
			m_pImageCtrl[eBTM_CAM_2X].SetTitleText( _T("BTM_2X") );
		}
	}
}

//===================================================================================
HBRUSH CfrmCameraView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmCameraView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
void APIENTRY CfrmCameraView::OnConfirmTracker( CRect& rcTrackRegion, UINT nIndexData, LPVOID lpUsrData )
{
 	pSystem.GetVisionComponent()->SetROI( rcTrackRegion );
  	pSystem.GetVisionComponent()->SetTrackerMode( nIndexData, FALSE );

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(pFrm->GetCurrViewNo()== _ALIGN_DISP_VIEW)
		pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_PAT_ROI_UPDATE, 0, 0);
	else if(pFrm->GetCurrViewNo()== _COMP_DISP_VIEW)
		pFrm->GetWindowHandler_CompView()->SendMessage(WM_MSG_PAT_ROI_UPDATE, 0, 0);
}
//===================================================================================
BOOL APIENTRY CfrmCameraView::OnFireMouseEvent(  DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData )
{
	return FALSE;
}
//===================================================================================
void APIENTRY CfrmCameraView::OnDrawExt( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData )
{

	CfrmCameraView* pView = (CfrmCameraView*)lpUsrData;
	CImageObj* pImgObj = pView->m_pImageCtrl[nIndexData].GetVisibleImageObject();

	if ( TRUE )
	{
		CPen DotPen;
		DotPen.CreatePen( PS_DASHDOT, 1, RGB(0xff, 0xff, 0) );
		CPen DotPen1;
		DotPen1.CreatePen( PS_SOLID, 1, RGB(255, 155, 155) );
		CPen SolidPenR;
		SolidPenR.CreatePen( PS_SOLID, 1, RGB(255, 50, 50) );
		CPen SolidPenB;
		SolidPenB.CreatePen( PS_SOLID, 1, RGB(50, 50, 255) );
		CPen SolidPenGray;
		SolidPenGray.CreatePen( PS_DOT, 1, RGB(200, 200, 255) );
		if ( pImgObj )
		{
			int nOldBkMode = pDC->SetBkMode( TRANSPARENT );
			CPoint ptCenter;
			ptCenter = pIDC->ImagePosToScreenPos( (int)pImgObj->GetWidth()/2, (int)pImgObj->GetHeight()/2 );
			CPen* pOldPen = pDC->SelectObject( &DotPen );
			pDC->MoveTo( ptCenter.x, 0 );
			pDC->LineTo( ptCenter.x, ptCenter.y*2 );
			pDC->MoveTo( 0, ptCenter.y );
			pDC->LineTo( ptCenter.x*2, ptCenter.y );

			if(nIndexData== eBTM_CAM_2X || nIndexData== eTOP_CAM_2X)
			{
				if(pView->ParamFile()->Model_data.iMarkSearchCount[STAGE_1_ALIGN_PROC] || 
					pView->ParamFile()->Model_data.iMarkSearchCount[STAGE_2_ALIGN_PROC] || 
					pView->ParamFile()->Model_data.iMarkSearchCount[STAGE_3_ALIGN_PROC] || 
					pView->ParamFile()->Model_data.iMarkSearchCount[ROLL_ALIGN_PROC] )
				{
					pDC->SelectObject( &(SolidPenGray) );
					CRect SearchRoiRect;

					SearchRoiRect = pIDC->ImagePosToScreenPos(
						pView->ParamFile()->Model_data.SearchROI_left[nIndexData], pView->ParamFile()->Model_data.SearchROI_top[nIndexData],
						pView->ParamFile()->Model_data.SearchROI_right[nIndexData], pView->ParamFile()->Model_data.SearchROI_bottom[nIndexData]);
					pDC->MoveTo(SearchRoiRect.left, SearchRoiRect.top);
					pDC->LineTo(SearchRoiRect.right, SearchRoiRect.top);		pDC->LineTo(SearchRoiRect.right, SearchRoiRect.bottom);
					pDC->LineTo(SearchRoiRect.left, SearchRoiRect.bottom);	pDC->LineTo(SearchRoiRect.left, SearchRoiRect.top);
				}
			}


/*
			if(nIndexData== eBTM_CAM_20X || nIndexData== eBTM_CAM_2X)
			{
				int mTopCameraNo;
				double cnv_Stamp_Xpix, cnv_Stamp_Ypix;//mTopCameraNo; //e2X_LENS 
				double cnv_Stamp_Xpix2, cnv_Stamp_Ypix2;//mTopCameraNo; //e2X_LENS 
				if(nIndexData== eTOP_CAM_20X || nIndexData== eBTM_CAM_20X)	mTopCameraNo= e20X_LENS;
				else														mTopCameraNo= e2X_LENS;
				pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 
					pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X, pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y, &cnv_Stamp_Xpix, &cnv_Stamp_Ypix);
				pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 
					pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X, pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y, &cnv_Stamp_Xpix2, &cnv_Stamp_Ypix2);

				ptCenter = pIDC->ImagePosToScreenPos(	(int)(cnv_Stamp_Xpix), 	(int)(cnv_Stamp_Ypix) );

				CPen* pOldPen1 = pDC->SelectObject( &SolidPenR );
				pDC->MoveTo( ptCenter.x-3, ptCenter.y-3 );				pDC->FillSolidRect(ptCenter.x-3, ptCenter.y-3, 6, 6, RGB(255, 155, 155));
				pDC->MoveTo( ptCenter.x-30, ptCenter.y );				pDC->LineTo( ptCenter.x+30, ptCenter.y);
				pDC->MoveTo( ptCenter.x, ptCenter.y-30 );				pDC->LineTo( ptCenter.x, ptCenter.y+30 );

				pDC->MoveTo( ptCenter.x, ptCenter.y );

				ptCenter = pIDC->ImagePosToScreenPos(	(int)(cnv_Stamp_Xpix2), (int)(cnv_Stamp_Ypix2) );
				CPen* pOldPen2 = pDC->SelectObject( &SolidPenB );
				pDC->MoveTo( ptCenter.x-3, ptCenter.y-3 );				pDC->FillSolidRect(ptCenter.x-3, ptCenter.y-3, 6, 6, RGB(255, 155, 155));
				pDC->MoveTo( ptCenter.x-30, ptCenter.y );				pDC->LineTo( ptCenter.x+30, ptCenter.y);
				pDC->MoveTo( ptCenter.x, ptCenter.y-30 );				pDC->LineTo( ptCenter.x, ptCenter.y+30 );
			}
*/
			pDC->SelectObject( &DotPen1 );
			double cnv_x, cnv_y;
			if(nIndexData== eTOP_CAM_20X || nIndexData== eBTM_CAM_20X)
			{
// 				cnv_x= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
// 				cnv_y= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
// 				cnv_x= (_INSP_IMAGE_SIZE_X/2.0)-cnv_x;			cnv_y= (_INSP_IMAGE_SIZE_Y/2.0)-cnv_y;
				if(nIndexData== eTOP_CAM_20X)
				{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix;			cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix;	}
				else
				{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix;			cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix;	}

				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x-10, ptCenter.y );
				pDC->LineTo( ptCenter.x+10, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-10 );
				pDC->LineTo( ptCenter.x, ptCenter.y+10 );

				if(nIndexData== eTOP_CAM_20X)
				{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Xpix;			cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Ypix; }
				else
				{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Xpix;			cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Ypix; }
				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x, ptCenter.y );
				pDC->AngleArc(ptCenter.x, ptCenter.y, 5, 0, 360);
				pDC->SelectObject( pOldPen );
				pDC->SetBkMode( nOldBkMode );
			}
			else
			{
				double cal_fac= pSystem.GetParameterFileComponent()->System_data.dBot_Cam_PixelToum_20X / pSystem.GetParameterFileComponent()->System_data.dBot_Cam_PixelToum_2X;
				if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG)
				{
					if(nIndexData== eTOP_CAM_2X) { cnv_x= (_INSP_IMAGE_SIZE_X/2.0);			cnv_y= (_INSP_IMAGE_SIZE_Y/2.0); }
					else
					{
						cnv_x= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix-(_INSP_IMAGE_SIZE_X/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_X/2.0);
						cnv_y= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix-(_INSP_IMAGE_SIZE_Y/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_Y/2.0);
					
						cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix;
						cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix;
					}
				}
				else
				{
					if(nIndexData== eTOP_CAM_2X)
					{	
						cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix;	cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix;
					}
					else
					{
						cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix;	cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix;
					}
				}
				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x-10, ptCenter.y );
				pDC->LineTo( ptCenter.x+10, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-10 );
				pDC->LineTo( ptCenter.x, ptCenter.y+10 );

				if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG)
				{
					if(nIndexData== eTOP_CAM_2X) { cnv_x= (_INSP_IMAGE_SIZE_X/2.0);			cnv_y= (_INSP_IMAGE_SIZE_Y/2.0); }
					else
					{
						cnv_x= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Xpix-(_INSP_IMAGE_SIZE_X/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_X/2.0);
						cnv_y= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Ypix-(_INSP_IMAGE_SIZE_Y/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_Y/2.0);
					}
				}
				else
				{
					if(nIndexData== eTOP_CAM_2X)
					{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Xpix;	cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Ypix; }
					else
					{	cnv_x= /*_INSP_IMAGE_SIZE_X-*/pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_2nd_Xpix;		cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_2nd_Ypix; }
				}
				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x, ptCenter.y );
				pDC->AngleArc(ptCenter.x, ptCenter.y, 5, 0, 360);
				pDC->SelectObject( pOldPen );
				pDC->SetBkMode( nOldBkMode );
			}
/*
			if(nIndexData== eTOP_CAM_20X || nIndexData== eBTM_CAM_20X)
			{
				cnv_x= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
				cnv_y= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
				cnv_x=  (_INSP_IMAGE_SIZE_X/2.0)-cnv_x;			cnv_y=  (_INSP_IMAGE_SIZE_Y/2.0)-cnv_y;
				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x, ptCenter.y );
				pDC->AngleArc(ptCenter.x, ptCenter.y, 5, 0, 360);
				pDC->SelectObject( pOldPen );
				pDC->SetBkMode( nOldBkMode );
			}
			else
			{
				cnv_x= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_2X / 1000.0);
				cnv_y= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_2X / 1000.0);
			}
*/		
		}

	}

}
//===================================================================================

BEGIN_EVENTSINK_MAP(CfrmCameraView, CFormView)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_0, DISPID_CLICK, CfrmCameraView::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_1, DISPID_CLICK, CfrmCameraView::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_2, DISPID_CLICK, CfrmCameraView::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_3, DISPID_CLICK, CfrmCameraView::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_4, DISPID_CLICK, CfrmCameraView::ClickJogSelect4, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_5, DISPID_CLICK, CfrmCameraView::ClickJogSelect5, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_0, DISPID_DBLCLICK, CfrmCameraView::DblClickJogSelect0, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_1, DISPID_DBLCLICK, CfrmCameraView::DblClickJogSelect1, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_2, DISPID_DBLCLICK, CfrmCameraView::DblClickJogSelect2, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_JOG_SELECT_3, DISPID_DBLCLICK, CfrmCameraView::DblClickJogSelect3, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_CM_MARK_LOAD, DISPID_DBLCLICK, CfrmCameraView::DblClickCmMarkLoad, VTS_NONE)
	ON_EVENT(CfrmCameraView, IDB_CM_MARK_LOAD2, DISPID_DBLCLICK, CfrmCameraView::DblClickCmMarkLoad2, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CfrmCameraView::ClickJogSelect0() //t-2x
{
	mTopCameraNo= e2X_LENS; CameraSeletUpdate();
	m_pVision->mSel_TopCam_No= e2X_LENS;
	pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
}


void CfrmCameraView::ClickJogSelect1() //t-20x
{
	mTopCameraNo= e20X_LENS; CameraSeletUpdate();
	m_pVision->mSel_TopCam_No= e20X_LENS;
}


void CfrmCameraView::ClickJogSelect2() //b-2x
{
	mBotCameraNo= e2X_LENS; CameraSeletUpdate();
	m_pVision->mSel_BotCam_No= e2X_LENS;
	pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_2X);
}


void CfrmCameraView::ClickJogSelect3() //b-20x
{
	mBotCameraNo= e20X_LENS; CameraSeletUpdate();
	m_pVision->mSel_BotCam_No= e20X_LENS;
}
//===================================================================================
void CfrmCameraView::CameraSeletUpdate()
{
	if(mTopCameraNo== e2X_LENS) 
	{
		m_btnTopCam_2X.SetValue(1); m_btnTopCam_20X.SetValue(0); 
		pSystem.GetVisionComponent()->GetImageViewer( 1 )->ShowWindow( SW_SHOW );
		pSystem.GetVisionComponent()->GetImageViewer( 0 )->ShowWindow( SW_HIDE );
		m_pVision->mSel_TopCam_No= e2X_LENS;
	}
	else
	{
		m_btnTopCam_2X.SetValue(0); m_btnTopCam_20X.SetValue(1); 
		pSystem.GetVisionComponent()->GetImageViewer( 1 )->ShowWindow( SW_HIDE );
		pSystem.GetVisionComponent()->GetImageViewer( 0 )->ShowWindow( SW_SHOW );
		m_pVision->mSel_TopCam_No= e20X_LENS;
	}
	if(mBotCameraNo== e2X_LENS) 
	{
		m_btnBotCam_2X.SetValue(1); m_btnBotCam_20X.SetValue(0); 
		pSystem.GetVisionComponent()->GetImageViewer( 3 )->ShowWindow( SW_SHOW );
		pSystem.GetVisionComponent()->GetImageViewer( 2 )->ShowWindow( SW_HIDE );
		m_pVision->mSel_BotCam_No= e2X_LENS;
	}
	else
	{
		m_btnBotCam_2X.SetValue(0); m_btnBotCam_20X.SetValue(1); 
		pSystem.GetVisionComponent()->GetImageViewer( 3 )->ShowWindow( SW_HIDE );
		pSystem.GetVisionComponent()->GetImageViewer( 2 )->ShowWindow( SW_SHOW );
		m_pVision->mSel_BotCam_No= e20X_LENS;
	}
}

void CfrmCameraView::DispCross(int camno, double posx, double posy, bool flag)
{
	COLORCROSS colorCross;
	colorCross.CreateObject( PDC_BLUE, posx, posy, 20 );

	CxGraphicObject& pGO = m_pImageCtrl[camno].GetGraphicObject();
	if(flag)	pGO.Reset();

	pGO.AddDrawCross( colorCross ); //화면에 선택된 좌표 표시

}

void CfrmCameraView::ClickJogSelect4()
{
	if(bTopCam_Live== _INSP_CAM_GRAB_CONTINUE)
	{
		m_btnTopCam_Live.SetValue(0);	m_btnTopCam_Live.SetCaption(_T("LIVE-STOP"));
		m_pVision->InspGrabContinueStop() ;
		bTopCam_Live= _INSP_CAM_GRAB;
	}
	else
	{
		if(mTopCameraNo== e2X_LENS)		{	m_pVision->InspGrabContinue(eTOP_CAM_2X);  m_pVision->ImageLiveCall(eTOP_CAM_2X);}
		else							{	m_pVision->InspGrabContinue(eTOP_CAM_20X); m_pVision->ImageLiveCall(eTOP_CAM_20X);}
		m_btnTopCam_Live.SetValue(1);	m_btnTopCam_Live.SetCaption(_T("LIVE"));
		bTopCam_Live= _INSP_CAM_GRAB_CONTINUE;
	}
}


void CfrmCameraView::ClickJogSelect5()
{
	if(bBotCam_Live== _INSP_CAM_GRAB_CONTINUE)
	{
		m_btnBotCam_Live.SetValue(0);	m_btnBotCam_Live.SetCaption(_T("LIVE-STOP"));
		m_pVision->InspGrabContinueStop() ;
		bBotCam_Live= _INSP_CAM_GRAB;
	}
	else
	{
		m_btnBotCam_Live.SetValue(1);	m_btnBotCam_Live.SetCaption(_T("LIVE"));
		bBotCam_Live= _INSP_CAM_GRAB_CONTINUE;
		if(mBotCameraNo== e2X_LENS)		{	m_pVision->InspGrabContinue(eBTM_CAM_2X);  m_pVision->ImageLiveCall(eBTM_CAM_2X);}
		else							{	m_pVision->InspGrabContinue(eBTM_CAM_20X); m_pVision->ImageLiveCall(eBTM_CAM_20X);}
	}
}


void CfrmCameraView::DblClickJogSelect0()
{
	CString strText;
// 	if(mTopCameraNo != e2X_LENS)
// 	{
		strText.Format(_T("상부 2X 위치 만큼 모터를 이동 하시겠습니까?"));
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
		{
			double Cur_X, Cur_Y, Cur_Z, ToPosition1;
			Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
			ToPosition1= /*Cur_X -*/ -ParamFile()->System_data.dTopCam20To2_X;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			ToPosition1= /*Cur_Y -*/ -ParamFile()->System_data.dTopCam20To2_Y;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			ToPosition1= /*Cur_Z -*/ -ParamFile()->System_data.dTopCam20To2_Z;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
//	}

	mTopCameraNo= e2X_LENS; CameraSeletUpdate();
	m_pVision->mSel_TopCam_No= e2X_LENS;
}


void CfrmCameraView::DblClickJogSelect1()
{
	CString strText;
// 	if(mTopCameraNo != e20X_LENS)
// 	{
		strText.Format(_T("상부 10X 위치 만큼 모터를 이동 하시겠습니까?"));
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
		{
			double Cur_X, Cur_Y, Cur_Z, ToPosition1;
			Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
			ToPosition1= /*Cur_X + */ParamFile()->System_data.dTopCam20To2_X;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			ToPosition1= /*Cur_Y +*/ ParamFile()->System_data.dTopCam20To2_Y;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			ToPosition1= /*Cur_Z +*/ ParamFile()->System_data.dTopCam20To2_Z;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
//	}

	mTopCameraNo= e20X_LENS; CameraSeletUpdate();
	m_pVision->mSel_TopCam_No= e20X_LENS;
}


void CfrmCameraView::DblClickJogSelect2()
{
	CString strText;

	double Cur_X, Cur_Y, Cur_Z, ToPosition1;

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);

	// 	if(Cur_X > pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY)+0.5)
	// 	{
	// 		MyMessageBox(_T("모터 이동이 실패했습니다.[X축 위치]"), _T("Position-Move Failed"), M_ICONERROR); return;
	// 	}
	strText.Format(_T("하부 2X 위치 만큼 모터를 이동 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
	{
		double curZ1, curZ2;
		curZ1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
		curZ2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z2);
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(!pSystem.GetMotionComponent()->Z_Up_PosCheck()) 
		{
			MyMessageBox(_T("모터 이동이 실패했습니다.[Z_UP]"), _T("Position-Move Failed"), M_ICONERROR); return;
			return;
		}


		// 		if(Cur_X <= 100.0)
		// 		{
		ToPosition1= -ParamFile()->System_data.dBotCam20To2_X;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 		}
		// 		else
		// 		{
		// 			ToPosition1= ParamFile()->System_data.dBotCam20To2_X;
		// 			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 		}
		ToPosition1= -ParamFile()->System_data.dBotCam20To2_Y;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(!pSystem.GetMotionComponent()->Z_Up_PosCheck()) 
		{
			MyMessageBox(_T("모터 이동이 실패했습니다.[Z_UP2]"), _T("Position-Move Failed"), M_ICONERROR); return;
			return;
		}

		// 		ToPosition1= Cur_Z + ParamFile()->System_data.dBotCam20To2_Z;
		// 		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, curZ1, curZ2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	}
}

//===================================================================================
void CfrmCameraView::DblClickJogSelect3()
{
	CString strText;

	double Cur_X, Cur_Y, Cur_Z, ToPosition1;

	 ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);

// 	if(Cur_X > pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY)+0.5)
// 	{
// 		MyMessageBox(_T("모터 이동이 실패했습니다.[X축 위치]"), _T("Position-Move Failed"), M_ICONERROR); return;
// 	}
	strText.Format(_T("하부 10X 위치 만큼 모터를 이동 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
	{
		double curZ1, curZ2;
		curZ1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
		curZ2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z2);
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(!pSystem.GetMotionComponent()->Z_Up_PosCheck()) 
		{
			MyMessageBox(_T("모터 이동이 실패했습니다.[Z_UP]"), _T("Position-Move Failed"), M_ICONERROR); return;
			return;
		}


// 		if(Cur_X <= 100.0)
// 		{
			ToPosition1= ParamFile()->System_data.dBotCam20To2_X;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		}
// 		else
// 		{
// 			ToPosition1= ParamFile()->System_data.dBotCam20To2_X;
// 			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		}
		ToPosition1= ParamFile()->System_data.dBotCam20To2_Y;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(!pSystem.GetMotionComponent()->Z_Up_PosCheck()) 
		{
			MyMessageBox(_T("모터 이동이 실패했습니다.[Z_UP2]"), _T("Position-Move Failed"), M_ICONERROR); return;
			return;
		}

// 		ToPosition1= Cur_Z + ParamFile()->System_data.dBotCam20To2_Z;
// 		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, curZ1, curZ2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	}
}
//===================================================================================
BOOL CfrmCameraView::MoveEndCheck(int Axis) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
	
	ProgWnd.SetRange(0, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
  
	ProgWnd.SetText( _T("%s\n  잠시만 기다려 주세요..."), strText);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
/*	pFrm->m_ProgWait->TotalTime= ParamFile()->System_data._AXIS_MOVE_TIME_OUT;
	pFrm->m_ProgWait->strTitle.Format("%s\n 잠시만 기다려 주세요...", strText);
	pFrm->m_ProgWait->strMsg= strText;
	pFrm->ProgWaitView(true);
	////////////////////////////////////////////////////////////////////////////////
	*/
	
	
	DWORD st= GetTickCount();
	DWORD progst= GetTickCount();
	((CTransfer_ControlApp *)AfxGetApp())->DoEvents();
	
	while(1) 
	{
		ProgWnd.SetPos(GetTickCount() - st);
		if(pSystem.GetMotionComponent()->isDone(Axis)) 
		{
				return TRUE;
		}
		Sleep(9);
		
		if(/*!pFrm->m_ProgWait->ProgEnable || */GetTickCount() - dwCheckTimeStart_Axis > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)  
		{
				pSystem.GetMotionComponent()->MoveStop(Axis); 
				return FALSE;
		}
		/// 사용자가 캔슬 버튼을 눌렀다.
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
		if ( ProgWnd.Cancelled() )
		{
			if     (Axis== _ACS_AXIS_IMP_ROLL_Z1 || Axis== _ACS_AXIS_IMP_ROLL_Z2)	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
			else if(Axis== _ACS_AXIS_ALIGN_Y1 || Axis== _ACS_AXIS_ALIGN_Y2)			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
			else 			pSystem.GetMotionComponent()->MoveStop(Axis); 

			Log()->SaveLogFile(_MOTION_LOG, _T("모션 동작이 취소 되었습니다......"));
			return FALSE;
		}
	}
	return FALSE;
}
//===================================================================================
BOOL CfrmCameraView::AF_MoveEndCheckNonDisp(int Axis) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
 	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
// 	
 	ProgWnd.SetRange(0, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
//   
 	ProgWnd.SetText( _T("%s\n  잠시만 기다려 주세요..."), strText);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
/*	pFrm->m_ProgWait->TotalTime= ParamFile()->System_data._AXIS_MOVE_TIME_OUT;
	pFrm->m_ProgWait->strTitle.Format("%s\n 잠시만 기다려 주세요...", strText);
	pFrm->m_ProgWait->strMsg= strText;
	pFrm->ProgWaitView(true);
	////////////////////////////////////////////////////////////////////////////////
	*/
	
	
	DWORD st= GetTickCount();
	DWORD progst= GetTickCount();
//	((CTransfer_ControlApp *)AfxGetApp())->DoEvents();
	
	while(1) 
	{
 		ProgWnd.SetPos(GetTickCount() - st);
		if(pSystem.GetMotionComponent()->isDone(Axis) /*&& !m_pVision->AF_End_Flag */&& !m_pVision->AF_GrabStart) 
		{
				return TRUE;
		}
		Sleep(9);
		
		if(/*!pFrm->m_ProgWait->ProgEnable || */GetTickCount() - dwCheckTimeStart_Axis > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)  
		{
				pSystem.GetMotionComponent()->MoveStop(Axis); 
				return FALSE;
		}
		/// 사용자가 캔슬 버튼을 눌렀다.
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
		if ( ProgWnd.Cancelled() )
		{
			if     (Axis== _ACS_AXIS_IMP_ROLL_Z1 || Axis== _ACS_AXIS_IMP_ROLL_Z2)	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
			else if(Axis== _ACS_AXIS_ALIGN_Y1 || Axis== _ACS_AXIS_ALIGN_Y2)			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
			else 			pSystem.GetMotionComponent()->MoveStop(Axis); 

			Log()->SaveLogFile(_MOTION_LOG, _T("모션 동작이 취소 되었습니다......"));
			return FALSE;
		}
	}
	return FALSE;
}


void CfrmCameraView::DblClickCmMarkLoad()
{
	double z_pos;
	if(mTopCameraNo== e2X_LENS) 
	{
		AF_Proc_Run(eTOP_CAM_2X, &z_pos);
		MoveEndCheck(_ACS_AXIS_UPPER_CAM_Z) ;
	}
	else
	{
		AF_Proc_Run(eTOP_CAM_20X, &z_pos);
		MoveEndCheck(_ACS_AXIS_UPPER_CAM_Z) ;
	}
/*
	int ret= m_pVision->AutoFocus();
	m_pVision->AF_BufferSave(36);
	if(ret> 0 && ret <= ParamFile()->System_data.iAF_TotalCount)
	{
		double pos= m_pVision->Z_AF_pos[ret];
		if(pos> 0.0 && pos <= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z))
			pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, pos);
	}
	else 
		AfxMessageBox(_T("A/F Error!!!"));
*/
}
BOOL CfrmCameraView::AF_Proc_Run(int CamNo, double *Z_Pos)
{
	*Z_Pos= 0.0;

	m_pVision->InspGrabContinueStop() ;
	double AF_Stepmm;

	if(CamNo== eTOP_CAM_2X || CamNo== eBTM_CAM_2X)
	{
		AF_Stepmm= ParamFile()->System_data.AF_Stepmm_2X;
		ParamFile()->System_data.iAF_TotalCount= 48;
	}
	else
	{
		AF_Stepmm= ParamFile()->System_data.AF_Stepmm_20X * 5.0;
		ParamFile()->System_data.iAF_TotalCount= 48;
	}
	pSystem.GetMotionComponent()->AF_Proc_Start(CamNo, AF_Stepmm, ParamFile()->System_data.iAF_TotalCount);
	if(CamNo== eTOP_CAM_2X || CamNo== eTOP_CAM_20X)		AF_MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z) ;
	else												AF_MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z) ;
	int ret= m_pVision->AutoFocus();
//	m_pVision->AF_BufferSave(ParamFile()->System_data.iAF_TotalCount);
	if(ret> 0 && ret <= ParamFile()->System_data.iAF_TotalCount)
	{
		double pos= m_pVision->Z_AF_pos[ret];
		double curPos= 0.0;
		
		if(CamNo== eTOP_CAM_2X || CamNo== eTOP_CAM_20X)	
		{
			curPos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
			if(pos> 0.0 && pos <= curPos)
			{
				*Z_Pos= pos;
				pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, pos, TRUE);
				AF_MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z) ;

				if(CamNo== eTOP_CAM_20X)
				{
					AF_Stepmm= ParamFile()->System_data.AF_Stepmm_20X;
					pSystem.GetMotionComponent()->AF_Proc_Start(CamNo, AF_Stepmm, ParamFile()->System_data.iAF_TotalCount);
					AF_MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z) ;
					ret= m_pVision->AutoFocus();
					if(ret> 0 && ret <= ParamFile()->System_data.iAF_TotalCount)
					{
						pos= m_pVision->Z_AF_pos[ret];
						if(pos> 0.0 && pos <= curPos)
						{
							curPos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
							*Z_Pos= pos;
							pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, pos, TRUE);
							AF_MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z) ;
							m_pVision->InspGrabContinue(CamNo) ;
							pSystem.GetVisionComponent()->AF_ProcStop() ;
							return TRUE;
						}
					}
				}
				else
				{
					m_pVision->InspGrabContinue(CamNo) ;
					pSystem.GetVisionComponent()->AF_ProcStop() ;
					return TRUE;
				}
			}
		}
		else
		{
			curPos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
			if(pos> 0.0 && pos <= curPos)
			{
				pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, pos, TRUE);
				*Z_Pos= pos;
				if(CamNo== eTOP_CAM_2X || CamNo== eTOP_CAM_20X)		AF_MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z) ;
				else												AF_MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z) ;
				pSystem.GetVisionComponent()->AF_ProcStop() ;
				m_pVision->InspGrabContinue(CamNo) ;
				return TRUE;
			}
		}
	}
	pSystem.GetVisionComponent()->AF_ProcStop() ;
	m_pVision->InspGrabContinue(CamNo) ;
	return FALSE;
}


void CfrmCameraView::DblClickCmMarkLoad2()
{
/*
	if(mBotCameraNo== e2X_LENS) 
	{
		pSystem.GetMotionComponent()->AF_Proc_Start(eBTM_CAM_2X);
		MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z) ;
	}
	else
	{
		pSystem.GetMotionComponent()->AF_Proc_Start(eBTM_CAM_20X);
		MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z) ;
	}
	int ret= m_pVision->AutoFocus();
	m_pVision->AF_BufferSave(36);
	if(ret> 0 && ret <= ParamFile()->System_data.iAF_TotalCount)
	{
		double pos= m_pVision->Z_AF_pos[ret];
		if(pos> -1.5 && pos <= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z))
			pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, pos);
	}
	else 
		AfxMessageBox(_T("A/F Error!!!"));
*/
}


void CfrmCameraView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
