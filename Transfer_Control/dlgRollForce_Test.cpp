// dlgRollForce_Test.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgRollForce_Test.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ComiAxcommon.h"
#include "dlgXZ12_Mat.h"

// CdlgRollForce_Test 대화 상자입니다.
extern CSystemClass pSystem;
CParameterFile* CdlgRollForce_Test::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgRollForce_Test::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

IMPLEMENT_DYNAMIC(CdlgRollForce_Test, CDialogEx)

CdlgRollForce_Test::CdlgRollForce_Test(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgRollForce_Test::IDD, pParent)
	, m_bRollZ_Enable(FALSE)
	, m_dZ_ControlForce_L(1.0)
	, m_dZ_ControlForce_R(1.0)
	, m_dPtpMove_mm(0)
	, m_dInc_MoveSpeed(1.0)
	, m_iLedcontrolVal1(0)
	, m_iLedcontrolVal2(0)
	, m_dZ_Test_Move_Dist(1.0)
	, m_dZ_Test_Move_Speed(0.1)
	, m_dZ_Test_Move_Delay(1000)
	, m_bGrapgRealDisp(FALSE)
	, m_dZ1_Pos(0)
	, m_dZ2_Pos(0)
	, m_iZ_ControlOption(0)
	, m_dBotX_MoveDistmm(2.0)
	, m_dZ_Test_Move_Speed_Up(0.1)
	, m_iZ_Control_Z1Hap(FALSE)
{
	bGr_Flag1= false;
	Thread_Start_Flag= FALSE; Thread_Z_Test_Step= 0;
	for(int i= 0 ; i< 2; i++)	{
		m_bExec_1[i] = FALSE; m_pThread_1[i] = NULL;	
	}
	dZ_Test_Move_Dist= 1.0;
	dZ_Test_Move_Speed= 0.1;
	dZ_Test_Move_Speed_Up= 0.1;
	dZ_Test_Move_Delay= 1000;
	bGrapgRealDisp= false;
	b_Graph_Start= false;

}

CdlgRollForce_Test::~CdlgRollForce_Test()
{
}

void CdlgRollForce_Test::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMIGRAPHCTRL1, m_cnGraph1);
	DDX_Control(pDX, IDS_COMMAND5_X4, m_btnTitle);
	DDX_Check(pDX, IDC_CHECK3, m_bRollZ_Enable);
	DDX_Control(pDX, IDB_JOG_SELECT_4, m_radio2X);
	DDX_Control(pDX, IDB_JOG_SELECT_13, m_radio20X);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
	DDX_Control(pDX, IDB_OPU_Z_CNT_DIS, m_btnZ_Cnt_Disable);
	DDX_Control(pDX, IDB_OPU_Z_CNT_ENA, m_btnZ_Cnt_Enable);
	DDX_Text(pDX, IDC_EDIT_Z_FORCE_LEFT, m_dZ_ControlForce_L);
	DDX_Text(pDX, IDC_EDIT_Z_FORCE_RIGHT, m_dZ_ControlForce_R);
	DDX_Text(pDX, IDC_EDIT1, m_dPtpMove_mm);
	DDX_Text(pDX, IDC_EDIT12, m_dInc_MoveSpeed);
	DDX_Control(pDX, IDB_OPU_Z_CNT_DIS2, m_btnSync_Cnt_Disable);
	DDX_Control(pDX, IDB_OPU_Z_CNT_ENA2, m_btnSync_Cnt_Enable);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS11, m_iLedcontrolVal1);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS12, m_iLedcontrolVal2);
	DDX_Control(pDX, IDC_SLIDER3, m_ctrlLedSlider1);
	DDX_Control(pDX, IDC_SLIDER5, m_ctrlLedSlider2);
	DDX_Text(pDX, IDC_EDIT13, m_dZ_Test_Move_Dist);
	DDX_Text(pDX, IDC_EDIT14, m_dZ_Test_Move_Speed);
	DDX_Text(pDX, IDC_EDIT15, m_dZ_Test_Move_Delay);
	DDX_Check(pDX, IDC_CHECK5, m_bGrapgRealDisp);
	DDX_Text(pDX, IDC_EDIT_14, m_dZ1_Pos);
	DDX_Text(pDX, IDC_EDIT_15, m_dZ2_Pos);
	DDX_Radio(pDX, IDC_RADIO1, m_iZ_ControlOption);
	DDX_Text(pDX, IDC_EDIT16, m_dBotX_MoveDistmm);
	DDX_Text(pDX, IDC_EDIT17, m_dZ_Test_Move_Speed_Up);
	DDX_Control(pDX, IDB_OPU_Z_CNT_DIS3, m_btnZMap_Cnt_Disable);
	DDX_Control(pDX, IDB_OPU_Z_CNT_ENA3, m_btnZMap_Cnt_Enable);

	DDX_Text(pDX, IDC_EDIT_OFFSET_16, m_dRoll_Diameter);
	DDX_Text(pDX, IDC_EDIT_OFFSET_31, m_dRoll_Diameter_AddVal);
	DDX_Check(pDX, IDC_CHECK4, m_iZ_Control_Z1Hap);
}


BEGIN_MESSAGE_MAP(CdlgRollForce_Test, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK3, &CdlgRollForce_Test::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgRollForce_Test::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CdlgRollForce_Test::OnBnClickedButton6)
	ON_MESSAGE(WM_MSG_OFF_GRAPH_DATA_UPDATE3, LoadCell_Get_Data3)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CdlgRollForce_Test::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CdlgRollForce_Test::OnNMCustomdrawSlider5)
	ON_BN_CLICKED(IDC_CHECK5, &CdlgRollForce_Test::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_BUTTON7, &CdlgRollForce_Test::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_RADIO1, &CdlgRollForce_Test::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO24, &CdlgRollForce_Test::OnBnClickedRadio24)
	ON_BN_CLICKED(IDC_BUTTON15, &CdlgRollForce_Test::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_CHECK4, &CdlgRollForce_Test::OnBnClickedCheck4)
END_MESSAGE_MAP()

//===================================================================================
int CdlgRollForce_Test::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
void CdlgRollForce_Test::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
BOOL CdlgRollForce_Test::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){ 
		if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)){          return TRUE;        }   
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//===================================================================================
// CdlgRollForce_Test 메시지 처리기입니다.
BOOL CdlgRollForce_Test::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//-----------------------------------------
	m_ImageCtrl.Create( &m_ImageMng, this );
	m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
	m_ImageCtrl.ImageUpdate();

	WINDOWPLACEMENT wndPlacement;
	CRect rcDlgSize;
	mTopCameraNo= e2X_LENS;
	if( m_ImageCtrl.GetSafeHwnd() )
	{
		GetDlgItem( IDC_DISPLAY_CAM_MANU_ALIGN )->GetWindowPlacement( &wndPlacement );
		GetDlgItem( IDC_DISPLAY_CAM_MANU_ALIGN )->GetClientRect( &rcDlgSize );
		m_ImageCtrl.MoveWindow( wndPlacement.rcNormalPosition.left, wndPlacement.rcNormalPosition.top, rcDlgSize.Width(), rcDlgSize.Height() );
	}

	m_radio2X.SetValue(1);

	REGISTER_CALLBACK regCB;
	memset( &regCB, 0, sizeof(REGISTER_CALLBACK) );
	regCB.lpUsrData[0] = regCB.lpUsrData[1] = regCB.lpUsrData[2] =
		regCB.lpUsrData[3] = regCB.lpUsrData[4] = this;

	regCB.fnOnFireMouseEvent = OnFireMouseEvent;
	regCB.fnOnDrawExt = OnDrawExt;

	m_ImageCtrl.SetRegisterCallBack( 0, &regCB );
	pSystem.GetVisionComponent()->SetTempImageCtrl( &m_ImageCtrl );

	m_ImageCtrl.ShowWindow( SW_SHOW );
	DispUpdate();
	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_2X) );
	//-----------------------------------------

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
	GetDlgItem(IDB_JOG_Z1_CCW)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_Z1_CW)->EnableWindow(FALSE);
	GetDlgItem(IDB_JOG_Z2_CCW)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_Z2_CW)->EnableWindow(FALSE);

	mTopCameraNo= e2X_LENS;
	m_radio2X.SetValue(1);	m_radio20X.SetValue(0);

	Jog_Mode= _MID_SPEED;
	m_btnJogModeSel1.SetValue(1);

	g_bZ_InterLockFlag= FALSE;

	pSystem.GetMotionComponent()->Z_ControlDisable();
	m_btnZ_Cnt_Disable.SetValue(1);
	m_btnZ_Cnt_Enable.SetValue(0);

	m_btnSync_Cnt_Disable.SetValue(1);
	m_btnSync_Cnt_Enable.SetValue(0);

	m_btnZMap_Cnt_Disable.SetValue(1);
	m_btnZMap_Cnt_Enable.SetValue(0);

	GraphDataInital() ;

	m_ctrlLedSlider1.SetRange(0, 255) ;
	m_ctrlLedSlider2.SetRange(0, 255) ;
	m_ctrlLedSlider1.SetPos(ParamFile()->System_data.iLED_Btm_Val_2X[ePLATE_3]) ;	m_iLedcontrolVal1 = ParamFile()->System_data.iLED_Btm_Val_2X[ePLATE_3];
	m_ctrlLedSlider2.SetPos(ParamFile()->System_data.iLED_Btm_Val_20X[ePLATE_3]) ;	m_iLedcontrolVal2 = ParamFile()->System_data.iLED_Btm_Val_20X[ePLATE_3];
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH3, _ON, m_iLedcontrolVal1);
	pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH4, _ON, m_iLedcontrolVal2);

	GetDlgItem(IDB_Z_TEST_START)->EnableWindow(TRUE);	GetDlgItem(IDB_Z_TEST_STOP)->EnableWindow(FALSE);

	RunThread_1(0);

	m_dZ1_Pos= ParamFile()->Model_data.dRollTest_Z1;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_STAGE3_ALIGN);
	m_dZ2_Pos= ParamFile()->Model_data.dRollTest_Z2;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_STAGE3_ALIGN);
	m_iZ_ControlOption= ParamFile()->Model_data.iZ_ControlOption;
	bBotX_TestMove= false;

	SetTimer(801, 2000, NULL);
	SetTimer(802, 500, NULL);

	m_dRoll_Diameter	= ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC];
	m_dRoll_Diameter_AddVal	= ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC];

	UpdateData(FALSE);
	//-----------------------------------------
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//===================================================================================
void CdlgRollForce_Test::DispUpdate()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CString strText;
	if(mTopCameraNo== e2X_LENS) 
	{
		m_btnTitle.SetCaption(_T("BOTTOM CAMERA\nLens\n:2X"));
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
		m_ImageCtrl.ImageUpdate();
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	}
	else			
	{
		m_btnTitle.SetCaption(_T("BOTTOM CAMERA\nLens\n:10X"));
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
		m_ImageCtrl.ImageUpdate();
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	}
	UpdateData(FALSE);
}

void CdlgRollForce_Test::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(801);
	KillTimer(802);
	KillThread_1(0);


	g_bZ_InterLockFlag= TRUE;
	KillTimer(dlgRollForce_Test_TIMER_ID1);
	KillTimer(dlgRollForce_Test_TIMER_ID2);
	KillTimer(dlgRollForce_Test_TIMER_ID3);
	pSystem.GetVisionComponent()->SetTempImageCtrl( NULL );
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void APIENTRY CdlgRollForce_Test::OnDrawExt( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData ) 
{
	CdlgRollForce_Test* pThis = (CdlgRollForce_Test*)lpUsrData;
	CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
	CImageObj* pImgObj = pThis->m_ImageCtrl.GetVisibleImageObject(); 

		CPen DotPen;		DotPen.CreatePen( PS_DASHDOT, 1, RGB(0xff, 0xff, 0) );
		CPen DotPen1;		DotPen1.CreatePen( PS_SOLID, 1, RGB(255, 155, 155) );
		CPen DotPenR;		DotPenR.CreatePen( PS_SOLID, 1, RGB(255, 50, 50) );
		CPen SoldPenG;		SoldPenG.CreatePen( PS_SOLID, 1, RGB(50, 255, 50) );
		CPen SoldPenB;		SoldPenB.CreatePen( PS_SOLID, 1, RGB(50, 50, 255) );
		CPen DotPenB;		DotPenB.CreatePen( PS_DASHDOT, 1, RGB(50, 50, 255) );

		int nOldBkMode = pDC->SetBkMode( TRANSPARENT );
		CPoint ptCenter = pIDC->ImagePosToScreenPos( (int)pImgObj->GetWidth()/2, (int)pImgObj->GetHeight()/2 );
		CPen* pOldPen = pDC->SelectObject( &DotPen );
		pDC->MoveTo( ptCenter.x, 0 );		pDC->LineTo( ptCenter.x, ptCenter.y*2 );
		pDC->MoveTo( 0, ptCenter.y );		pDC->LineTo( ptCenter.x*2, ptCenter.y ); 

		pDC->SelectObject( pOldPen );		pDC->SetBkMode( nOldBkMode );
}
//===================================================================================
BOOL APIENTRY CdlgRollForce_Test::OnFireMouseEvent(  DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData )
{
	CdlgRollForce_Test* pThis = (CdlgRollForce_Test*)lpUsrData;

	if( dwMsg == WM_MOUSEMOVE )
	{
	}
	else if( dwMsg == WM_LBUTTONUP )
	{
	}
	return FALSE;
}

//===================================================================================

BEGIN_EVENTSINK_MAP(CdlgRollForce_Test, CDialogEx)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_0, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_1, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_2, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z_CW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_EXIT, DISPID_CLICK, CdlgRollForce_Test::ClickExit, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_EXIT2, DISPID_CLICK, CdlgRollForce_Test::ClickExit2, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_WAIT_POS3, DISPID_CLICK, CdlgRollForce_Test::ClickZWaitPos3, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_OPU_Z_CNT_DIS, DISPID_CLICK, CdlgRollForce_Test::ClickOpuZCntDis, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_OPU_Z_CNT_ENA, DISPID_CLICK, CdlgRollForce_Test::ClickOpuZCntEna, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_DATA_SET, DISPID_CLICK, CdlgRollForce_Test::ClickDataSet, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CCW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageXCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CCW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageXCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageXCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_X_CW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageXCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_CAMERA_Y_CCW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogCameraYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_CAMERA_Y_CCW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogCameraYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_CAMERA_Y_CW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogCameraYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_CAMERA_Y_CW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogCameraYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CW3, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageTCw3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CW3, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageTCw3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CCW3, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageTCcw3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CCW3, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageTCcw3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_WAIT_POS5, DISPID_CLICK, CdlgRollForce_Test::ClickZWaitPos5, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_3, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_OPU_Z_CNT_DIS2, DISPID_CLICK, CdlgRollForce_Test::ClickOpuZCntDis2, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_OPU_Z_CNT_ENA2, DISPID_CLICK, CdlgRollForce_Test::ClickOpuZCntEna2, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z1_CCW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z1_CCW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z2_CCW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z2_CCW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z1_CW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z1_CW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z2_CW, DISPID_MOUSEDOWN, CdlgRollForce_Test::MouseDownJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z2_CW, DISPID_MOUSEUP, CdlgRollForce_Test::MouseUpJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_4, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect4, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_13, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect13, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_4, DISPID_DBLCLICK, CdlgRollForce_Test::DblClickJogSelect4, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_13, DISPID_DBLCLICK, CdlgRollForce_Test::DblClickJogSelect13, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_SELECT_6, DISPID_CLICK, CdlgRollForce_Test::ClickJogSelect6, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDC_COMIGRAPHCTRL1, DISPID_DBLCLICK, CdlgRollForce_Test::DblClickComigraphctrl1, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_TEST_START, DISPID_CLICK, CdlgRollForce_Test::ClickZWaitPos6, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_TEST_STOP, DISPID_CLICK, CdlgRollForce_Test::ClickJogZCw3, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_WAIT_POS7, 8, CdlgRollForce_Test::ContainerChangeColorZWaitPos7, VTS_I4)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_WAIT_POS7, DISPID_CLICK, CdlgRollForce_Test::ClickZWaitPos7, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_JOG_Z_CW2, DISPID_CLICK, CdlgRollForce_Test::ClickJogZCw2, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_TEST_START2, DISPID_CLICK, CdlgRollForce_Test::ClickZTestStart2, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_CAL_4, DISPID_CLICK, CdlgRollForce_Test::ClickCal4, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_OPU_Z_CNT_DIS3, DISPID_CLICK, CdlgRollForce_Test::ClickOpuZCntDis3, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_OPU_Z_CNT_ENA3, DISPID_CLICK, CdlgRollForce_Test::ClickOpuZCntEna3, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_DATA_SAVE, DISPID_CLICK, CdlgRollForce_Test::ClickDataSave, VTS_NONE)
	ON_EVENT(CdlgRollForce_Test, IDB_Z_WAIT_POS61, DISPID_CLICK, CdlgRollForce_Test::ClickZWaitPos61, VTS_NONE)
END_EVENTSINK_MAP()


void CdlgRollForce_Test::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}


void CdlgRollForce_Test::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}


void CdlgRollForce_Test::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}


void CdlgRollForce_Test::ClickExit()
{
	UpdateData(TRUE);
	ParamFile()->Model_data.dRollTest_Z1= m_dZ1_Pos;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_STAGE3_ALIGN);
	ParamFile()->Model_data.dRollTest_Z2= m_dZ2_Pos;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_STAGE3_ALIGN);
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
	Thread_Start_Flag= FALSE;
	Thread_Z_Test_Step= 0;
	KillThread_1(0);

	KillTimer(dlgRollForce_Test_TIMER_ID1);
	KillTimer(dlgRollForce_Test_TIMER_ID2);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	pSystem.GetMotionComponent()->Z_ControlDisable();
	pSystem.GetMotionComponent()->Sync_Mode_Reset();
	pSystem.GetMotionComponent()->Z_Control_Mode_Send(0);
	pFrm->GetWindowHandler_MainView()->EndGraph(STAGE_3_ALIGN_PROC);
	g_bZ_InterLockFlag= TRUE;
	CString strText;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); }
	else
	{
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); }
	}
	pSystem.GetMotionComponent()->Z_Mapping_Stop();

	EndDialog(IDOK);
}


void CdlgRollForce_Test::ClickExit2()
{
	pSystem.GetMotionComponent()->Z_ControlDisable();
	pSystem.GetMotionComponent()->Sync_Mode_Reset();
	g_bZ_InterLockFlag= TRUE;
	CString strText;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); }
	EndDialog(IDCANCEL);
}
//===================================================================================
void CdlgRollForce_Test::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
	//		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -m_dPtpMove_mm, _SPD_MODE);		
		pSystem.GetMotionComponent()->INC_Move_Speed(_ACS_AXIS_STAGE_X, -m_dPtpMove_mm, m_dInc_MoveSpeed, _SPD_MODE);		
	}
	else
	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_STAGE_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_STAGE_X, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
	}
}
//===================================================================================


void CdlgRollForce_Test::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;//
		//pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, m_dPtpMove_mm, _SPD_MODE);		
			pSystem.GetMotionComponent()->INC_Move_Speed(_ACS_AXIS_STAGE_X, m_dPtpMove_mm, m_dInc_MoveSpeed, _SPD_MODE);		
	}
	else
	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_STAGE_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_STAGE_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_STAGE_X, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
	}
}
//===================================================================================
//===================================================================================
void CdlgRollForce_Test::MouseDownJogZCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(fabs(m_dPtpMove_mm)> 0.2) return;
// 		pSystem.GetMotionComponent()->INC_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, -m_dPtpMove_mm, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, _DIR_CCW, SetSpeed);
	}
}

void CdlgRollForce_Test::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
// 	if(Jog_Mode!= _PTP_MOVE) 
// 	{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
// 	}
}
//===================================================================================
void CdlgRollForce_Test::MouseDownJogZCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(fabs(m_dPtpMove_mm)> 0.2) return;
// 		pSystem.GetMotionComponent()->INC_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, m_dPtpMove_mm, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, _DIR_CW, SetSpeed);
	}
}

void CdlgRollForce_Test::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}

void CdlgRollForce_Test::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	if(m_bRollZ_Enable)
	{
		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(TRUE);
		GetDlgItem(IDB_JOG_Z1_CCW)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_Z1_CW)->EnableWindow(TRUE);
		GetDlgItem(IDB_JOG_Z2_CCW)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_Z2_CW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
		GetDlgItem(IDB_JOG_Z1_CCW)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_Z1_CW)->EnableWindow(FALSE);
		GetDlgItem(IDB_JOG_Z2_CCW)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_Z2_CW)->EnableWindow(FALSE);
	}
}




void CdlgRollForce_Test::ClickZWaitPos3()
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;


	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}

	strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY));

	strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;

	pSystem.GetMotionComponent()->Z_ControlDisable();
	m_btnZ_Cnt_Disable.SetValue(1);	m_btnZ_Cnt_Enable.SetValue(0);

	//-----------------
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	//-----------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
}
//===================================================================================
BOOL CdlgRollForce_Test::MoveEndCheck(int Axis) 
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


void CdlgRollForce_Test::ClickOpuZCntDis()
{
	pSystem.GetMotionComponent()->Z_ControlDisable();
	m_btnZ_Cnt_Disable.SetValue(1);	m_btnZ_Cnt_Enable.SetValue(0);
}

//===================================================================================
void CdlgRollForce_Test::ClickOpuZCntEna()
{
	if(Thread_Start_Flag)
	{
		m_btnZ_Cnt_Disable.SetValue(1);	m_btnZ_Cnt_Enable.SetValue(0);
	}
	else
	{
		UpdateData(TRUE);
		ParamFile()->Model_data.iZ_ControlOption= m_iZ_ControlOption;

		if(m_dZ_ControlForce_L < 0.1)	m_dZ_ControlForce_L= 0.5;
		if(m_dZ_ControlForce_R < 0.1)	m_dZ_ControlForce_R= 0.5;
		if(m_dZ_ControlForce_L > 20.0)	m_dZ_ControlForce_L= 20.0;
		if(m_dZ_ControlForce_R > 20.0)	m_dZ_ControlForce_R= 20.0;
		pSystem.GetMotionComponent()->RollForceControl_Start(m_dZ_ControlForce_L, m_dZ_ControlForce_R, STAGE_3_ALIGN_PROC);
		m_btnZ_Cnt_Disable.SetValue(0);	
		m_btnZ_Cnt_Enable.SetValue(1);
		UpdateData(FALSE);
	}
}

//===================================================================================
void CdlgRollForce_Test::ClickDataSet()
{
	UpdateData(TRUE);

	if(m_dZ_ControlForce_L < 0.1)	m_dZ_ControlForce_L= 0.5;
	if(m_dZ_ControlForce_R < 0.1)	m_dZ_ControlForce_R= 0.5;
	if(m_dZ_ControlForce_L > 45.0)	m_dZ_ControlForce_L= 45.0;
	if(m_dZ_ControlForce_R > 45.0)	m_dZ_ControlForce_R= 45.0;

	if(m_iZ_Control_Z1Hap)		ParamFile()->Model_data.iZ_Control_Z1Hap= 1;
	else						ParamFile()->Model_data.iZ_Control_Z1Hap= 0;

	if(m_btnZ_Cnt_Enable.GetValue())
		pSystem.GetMotionComponent()->Z_Control_Mode_Send(ParamFile()->Model_data.iZ_ControlOption);
	pSystem.GetMotionComponent()->RollForceControl_Set(m_dZ_ControlForce_L, m_dZ_ControlForce_R);
	UpdateData(FALSE);
}

//===================================================================================
void CdlgRollForce_Test::MouseDownJogStageXCcw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
			switch(Jog_Mode) 
			{
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_X, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageXCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_X);
	}
}

//===================================================================================
void CdlgRollForce_Test::MouseDownJogStageXCw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		switch(Jog_Mode) 
		{
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_X, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageXCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_X);
	}
}

//===================================================================================
void CdlgRollForce_Test::MouseDownJogCameraYCcw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		switch(Jog_Mode) 
		{
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Y, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogCameraYCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
	}
}

//===================================================================================
void CdlgRollForce_Test::MouseDownJogCameraYCw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		switch(Jog_Mode) 
		{
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Y, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogCameraYCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
	}
}

//===================================================================================
void CdlgRollForce_Test::MouseDownJogStageTCw3(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CW3)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
	}
	else
	{
		switch(Jog_Mode) 
		{
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Z, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageTCw3(short Button, short Shift, long x, long y)
{
	pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Z);
}

//===================================================================================
void CdlgRollForce_Test::MouseDownJogStageTCcw3(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CCW3)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) 
	{
		UpdateData(TRUE);
	}
	else
	{
		switch(Jog_Mode) 
		{
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Z, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageTCcw3(short Button, short Shift, long x, long y)
{
	pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Z);
}


void CdlgRollForce_Test::ClickZWaitPos5()
{
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, 0.0,	FALSE);
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, 0.0,	FALSE);
}


void CdlgRollForce_Test::ClickJogSelect3()
{
	Jog_Mode= _PTP_MOVE;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}
void CdlgRollForce_Test::OnBnClickedButton1()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_MainView()->GraphDataInital();

	GraphDataInital();
	pSystem.GetMotionComponent()->Grapg_Only_Start(STAGE_3_ALIGN_PROC);
	if(bGrapgRealDisp) SetTimer(dlgRollForce_Test_TIMER_ID1, 2000, NULL);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	b_Graph_Start= true;

}


void CdlgRollForce_Test::OnBnClickedButton6()
{
	KillTimer(dlgRollForce_Test_TIMER_ID1);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_MainView()->EndGraph(STAGE_3_GRAPH);//STAGE_3_ALIGN_PROC);
	EndGraph(STAGE_3_ALIGN_PROC);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	b_Graph_Start= false;
	bGrapgRealDisp= m_bGrapgRealDisp= false;
	UpdateData(FALSE);
}


void CdlgRollForce_Test::MouseDownJogStageYCcw2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_Y_CCW2
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->Align_JogStart(ALIGN_STAGE_Y, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageYCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CdlgRollForce_Test::MouseDownJogStageYCw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_Y_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->Align_JogStart(ALIGN_STAGE_Y, _DIR_CW, SetSpeed);
	}
}
void CdlgRollForce_Test::MouseUpJogStageYCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CdlgRollForce_Test::MouseDownJogStageTCw2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CW2
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(fabs(m_dPtpMove_mm)> 2.0) return;
// 		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->Align_JogStart(ALIGN_STAGE_T, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageTCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CdlgRollForce_Test::MouseDownJogStageTCcw2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CCW2
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
// 		if(m_dPtpMove_mm== 0) return;
// 		if(fabs(m_dPtpMove_mm)> 2.0) return;
// 		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->Align_JogStart(ALIGN_STAGE_T, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogStageTCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}


void CdlgRollForce_Test::ClickOpuZCntDis2()
{
	m_btnSync_Cnt_Disable.SetValue(1);
	m_btnSync_Cnt_Enable.SetValue(0);
	pSystem.GetMotionComponent()->Sync_Mode_Reset();
}


void CdlgRollForce_Test::ClickOpuZCntEna2()
{
	m_btnSync_Cnt_Disable.SetValue(0);
	m_btnSync_Cnt_Enable.SetValue(1);
	pSystem.GetMotionComponent()->Sync_Mode_Set(STAGE_1_ALIGN_PROC);
}


void CdlgRollForce_Test::MouseDownJogZ1Ccw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z1_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 0.2) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL_Z1, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z1];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z1];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z1];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL_Z1, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogZ1Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}


void CdlgRollForce_Test::MouseDownJogZ2Ccw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z2_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 0.2) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL_Z2, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL_Z2, _DIR_CCW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogZ2Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}


void CdlgRollForce_Test::MouseDownJogZ1Cw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z1_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 0.2) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL_Z1, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z1];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z1];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z1];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL_Z1, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogZ1Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}


void CdlgRollForce_Test::MouseDownJogZ2Cw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z2_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 0.2) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL_Z2, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		double SetSpeed;
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL_Z2, _DIR_CW, SetSpeed);
	}
}


void CdlgRollForce_Test::MouseUpJogZ2Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}


void CdlgRollForce_Test::ClickJogSelect4()
{
	mTopCameraNo= e2X_LENS;
	DispUpdate();
	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_2X) );
	m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
}


void CdlgRollForce_Test::ClickJogSelect13()
{
	mTopCameraNo= e20X_LENS;
	DispUpdate();
	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_20X) );
	m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_20X ) );
	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
}


void CdlgRollForce_Test::DblClickJogSelect4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CdlgRollForce_Test::DblClickJogSelect13()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CdlgRollForce_Test::ClickJogSelect6()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CdlgRollForce_Test::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent== 802)
	{
		CString strText;
		double cur_pos= 0.0;
		pSystem.GetMotionComponent()->Position_RealAll();
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_STAGE_X]);		SetDlgItemText(IDC_CURPOS_0, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL]);		SetDlgItemText(IDC_CURPOS_1, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_UPPER_CAM_Y]);	SetDlgItemText(IDC_CURPOS_99, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_UPPER_CAM_Z]);	SetDlgItemText(IDC_CURPOS_4, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z1]);	SetDlgItemText(IDC_CURPOS_14, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z2]);	SetDlgItemText(IDC_CURPOS_15, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_ALIGN_Y1]);		SetDlgItemText(IDC_CURPOS_12, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_ALIGN_Y2]);		SetDlgItemText(IDC_CURPOS_13, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_X]);	SetDlgItemText(IDC_CURPOS_9, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_Y]);	SetDlgItemText(IDC_CURPOS_10, strText.GetBuffer(0));
		strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_Z]);	SetDlgItemText(IDC_CURPOS_11, strText.GetBuffer(0));
	}
	else
	{
		if(nIDEvent== dlgRollForce_Test_TIMER_ID3)
		{
			if(pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_X))
			{
				if(bBotX_TestMove)
				{
					KillTimer(dlgRollForce_Test_TIMER_ID3);
					pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -m_dBotX_MoveDistmm, _SPD_MODE);	
					GetDlgItem(IDB_Z_TEST_START2)->EnableWindow(TRUE);
					bBotX_TestMove= false;
				}
				else	
				{
					GetDlgItem(IDB_Z_TEST_START2)->EnableWindow(TRUE);
					KillTimer(dlgRollForce_Test_TIMER_ID3);
				}
			}
		}
		else if(nIDEvent== 801)
		{
			if(pSystem.GetMotionComponent()->isMapRun())
			{
				m_btnZMap_Cnt_Disable.SetValue(0);
				m_btnZMap_Cnt_Enable.SetValue(1);
			}
			else
			{
				m_btnZMap_Cnt_Disable.SetValue(1);
				m_btnZMap_Cnt_Enable.SetValue(0);
			}
		}
		else
		{
			KillTimer(dlgRollForce_Test_TIMER_ID1);

			SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE3, 0, 0);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CdlgRollForce_Test::GraphDataInital() 
{
	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);
	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);
	m_cnGraph1.DeletePlot(4);	m_cnGraph1.DeletePlot(5);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용

	int i;
	for(i= 0; i< _MAX_LOADCELL_NO; i++)
	{
		PlotData_Time_1[i]= PlotData_L1_1[i]=	PlotData_L2_1[i]=	PlotData_XP_1[i] =	PlotData_ZP_1[i] =	PlotData_Z2P_1[i] =	PlotData_XS_1[i]=	PlotData_Enc_1[i]= 0.0;
		PlotData_Time_2[i]= PlotData_L1_2[i]=	PlotData_L2_2[i]=	PlotData_XP_2[i]=	PlotData_ZP_2[i]=	PlotData_Z2P_2[i]=	PlotData_XS_2[i]=	PlotData_Enc_2[i]= 0.0;
		PlotData_Time_3[i]= PlotData_L1_3[i]=	PlotData_L2_3[i]=	PlotData_XP_3[i]=	PlotData_ZP_3[i]=	PlotData_Z2P_3[i]=	PlotData_XS_3[i]=	PlotData_Enc_3[i]= 0.0;
		PlotData_Time_4[i]= PlotData_L1_S[i]=	PlotData_L2_S[i]=	PlotData_XP_S[i]=	PlotData_ZP_S[i]=	PlotData_Z2P_S[i]=	PlotData_XS_S[i]=	PlotData_Enc_S[i]= 0.0;
	}
	for(i= 0; i< 7; i++)
	{
		Graph_Y1_Max[i]= Graph_Y2_Max[i]=	Graph_Y3_Max[i]=	Graph_Y4_Max[i]= 0.0;
	}
	m_cnGraph1.DeletePlot(-1);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용

	Max_X1_Cnt=	0;
	Max_X2_Cnt=	0;
	Max_X3_Cnt=	0;
	Max_X4_Cnt=	0;

	AD_BufferRecCount1= AD_BufferRecCount2= AD_BufferRecCount3= AD_BufferRecCount4= 0;
}
void CdlgRollForce_Test::EndGraph(int ch)
{
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

// 	switch(ch)
// 	{
// 	case STAGE_1_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE1, 0, 0);	 break;
// 	case STAGE_2_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE2, 0, 0);	 break;
// 	case STAGE_3_GRAPH:	
// 		SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE3, 0, 0);	
		SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE3, 0, 0);
//		break;
// 	case SYNC_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE4, 0, 0);	 break;
// 	}
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
	/////////////////////////////////////////////////////////////////////////////////
	//	m_cnGraph1.SaveToClipboard ();
	/////////////////////////////////////////////////////////////////////////////////
}
LRESULT CdlgRollForce_Test::LoadCell_Get_Data3(WPARAM wParam,LPARAM lParam) //Left-Roll
{
	int i= 0, k= 0;
	double sumData= 0.0;
	int sumDataCount= 0;

	double ChVal= 0.0;//2.5;

	double aa= 0.002;//1.0/ParamFile()->System_data.g_dAD_FREQ;
	double bb= 0.0;
	int SumFilterCount= 10;
	double Y_Max1_0= 0.0;	double Y_Max1_1= 0.0;

	double time_fact= 0.01; //ParamFile()->System_data.iGatherPeriod * 0.0005;
	double Y_Max_LBR_Vel= 0.0;
	double Y_Max_RBR_Vel= 0.0;
	double Y_Max_LBR_T= 0.0;
	double Y_Max_RBR_T= 0.0;

	if(lParam== 0)
	{
		//GLOBAL REAL rBR_VEL_L(10000), rBR_TOQ_L(10000), rBR_VEL_R(10000), rBR_TOQ_R(10000);

		//"X_POS_1", "X_VEL_1","Z1_INPUT_1", "Z2_INPUT_1", "ENC_POS_1", "TIME_1", "Z_POS", // variable name

		AD_BufferRecCount3= pSystem.GetMotionComponent()->GetGetherData(PlotData_XP_3, PlotData_XS_3, PlotData_L1_3, PlotData_L2_3, PlotData_Enc_3, PlotData_Time_3, PlotData_ZP_3, PlotData_Z2P_3);


		if(AD_BufferRecCount3<= 0)
		{
			m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
			m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
			return 0;
		}

		Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
		for(i= 0, Max_X3_Cnt= 0; i< AD_BufferRecCount3; i++, Max_X3_Cnt++)
		{
			if(i> _MAX_LOADCELL_NO) break;

			if(fabs(PlotData_XP_3[i]) > Graph_Y3_Max[0])	Graph_Y3_Max[0]= fabs(PlotData_XP_3[i]);
			if(fabs(PlotData_XS_3[i]) > Graph_Y3_Max[1])	Graph_Y3_Max[1]= fabs(PlotData_XS_3[i]);
			if(fabs(PlotData_L1_3[i]) > Graph_Y3_Max[2])	Graph_Y3_Max[2]= fabs(PlotData_L1_3[i]);
			if(fabs(PlotData_Enc_3[i]) > Graph_Y3_Max[3])	Graph_Y3_Max[3]= fabs(PlotData_Enc_3[i]);
			if(fabs(PlotData_ZP_3[i]) > Graph_Y3_Max[4])	Graph_Y3_Max[4]= fabs(PlotData_ZP_3[i]);
			PlotData_Time_3[i]= i * time_fact;
//			PlotData_Time_3[i]= PlotData_Time_3[i];
		}
	}
	Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
	Graph_Y3_Max[0]= Graph_Y3_Max[1]=Graph_Y3_Max[2]=Graph_Y3_Max[3]=Graph_Y3_Max[4]= 0.0;
	for(i= 0, Max_X3_Cnt= 0; i< AD_BufferRecCount3; i++, Max_X3_Cnt++)
	{
		if(i> _MAX_LOADCELL_NO) break;

		if(fabs(PlotData_XP_3[i]) > Graph_Y3_Max[0])	Graph_Y3_Max[0]= fabs(PlotData_XP_3[i]);
		if(fabs(PlotData_XS_3[i]) > Graph_Y3_Max[1])	Graph_Y3_Max[1]= fabs(PlotData_XS_3[i]);
		if(fabs(PlotData_L1_3[i]) > Graph_Y3_Max[2])	Graph_Y3_Max[2]= fabs(PlotData_L1_3[i]);
		if(fabs(PlotData_Enc_3[i]) > Graph_Y3_Max[3])	Graph_Y3_Max[3]= fabs(PlotData_Enc_3[i]);
		if(fabs(PlotData_ZP_3[i]) > Graph_Y3_Max[4])	Graph_Y3_Max[4]= fabs(PlotData_ZP_3[i]);
		PlotData_Time_3[i]= i * time_fact;
//		PlotData_Time_3[i]= PlotData_Time_3[i];
	}
	int aaa= (int)(Graph_Y3_Max[2] / 10.0);

	Graph_Y3_Max[2]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y3_Max[0] / 10.0);	Graph_Y3_Max[0]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y3_Max[1] / 5.0);	Graph_Y3_Max[1]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y3_Max[4] / 5.0);	Graph_Y3_Max[4]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y3_Max[3] / 1.0);	Graph_Y3_Max[3]= (aaa+1) * 1.0;
	//--------------
	int x_max= (int)(Max_X3_Cnt * time_fact) + 1;

	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용

	if(AD_BufferRecCount3<= 0) return 0;

	m_cnGraph1.SetXAxisRange (0, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (1, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (2, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (3, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (4, 0.0, (double)x_max, true);	

	double tY_Max1_0= 0.0;
	double tY_Max1_1= 0.0;

/*
	m_cnGraph1.SetYAxisRange (0, -Graph_Y3_Max[2], Graph_Y3_Max[2], true);	
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.SetYAxisRange (1, -Graph_Y3_Max[0], Graph_Y3_Max[0], true);	
	else
		m_cnGraph1.SetYAxisRange (1, -Graph_Y3_Max[4], Graph_Y3_Max[4], true);	
	m_cnGraph1.SetYAxisRange (2, -Graph_Y3_Max[1], Graph_Y3_Max[1], true);	
	m_cnGraph1.SetYAxisRange (3, -Graph_Y3_Max[3], Graph_Y3_Max[3], true);	
	m_cnGraph1.SetYAxisRange (3, -15.0, 15.0, true);	
	m_cnGraph1.SetYAxisRange (4, -15.0, 15.0, true);	
*/
	m_cnGraph1.SetYAxisRange (0, 1.0, 4.0, true);	
	m_cnGraph1.SetYAxisRange (1, 450.0, 500, true);	
	m_cnGraph1.SetYAxisRange (2, -Graph_Y3_Max[1], Graph_Y3_Max[1], true);	
	m_cnGraph1.SetYAxisRange (3, -Graph_Y3_Max[3], Graph_Y3_Max[3], true);	
	m_cnGraph1.SetYAxisRange (3, 14.2, 14.3, true);	
	m_cnGraph1.SetYAxisRange (4, 14.2, 14.3, true);	
//------------------------------------------------------
	m_cnGraph1.PlotXY_VT(0, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_L1_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(1, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_L2_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
//	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_XP_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
// 	else
// 		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_ZP_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(3, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_XS_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	//	m_cnGraph1.PlotXY_VT(4, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_Enc_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(4, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_ZP_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(5, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_Z2P_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);

	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	Invalidate();

	if(bGrapgRealDisp) SetTimer(dlgRollForce_Test_TIMER_ID1, 2000, NULL);

	return 0;
}


void CdlgRollForce_Test::DblClickComigraphctrl1()
{
	if(bGr_Flag1) 
	{
		bGr_Flag1= false;
		m_cnGraph1.SetCTBVisible( FALSE );
	}
	else 
	{
		bGr_Flag1= true;
		m_cnGraph1.SetCTBVisible( TRUE );
	}
}


void CdlgRollForce_Test::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iLedcontrolVal1 = m_ctrlLedSlider1.GetPos();	
	UpdateData(FALSE) ;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH3, _ON, m_iLedcontrolVal1);

	*pResult = 0;
}


void CdlgRollForce_Test::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iLedcontrolVal2 = m_ctrlLedSlider2.GetPos();	
	UpdateData(FALSE) ;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_AlignView()->LED_cnt.LightVal(_LED_CH4, _ON, m_iLedcontrolVal2);

	*pResult = 0;
}


void CdlgRollForce_Test::ClickZWaitPos6()
{
	if(m_btnZ_Cnt_Disable.GetValue() && !m_btnZ_Cnt_Enable.GetValue())
	{
		UpdateData(TRUE);

		if(m_dZ_Test_Move_Dist < 0.0)		m_dZ_Test_Move_Dist= 0.01;
		if(m_dZ_Test_Move_Dist > 10.0)		m_dZ_Test_Move_Dist= 10.0;
		if(m_dZ_Test_Move_Speed < 0.0)		m_dZ_Test_Move_Speed= 0.01;
		if(m_dZ_Test_Move_Speed > 10.0)		m_dZ_Test_Move_Speed= 10.0;
		if(m_dZ_Test_Move_Speed_Up < 0.0)		m_dZ_Test_Move_Speed_Up= 0.01;
		if(m_dZ_Test_Move_Speed_Up > 10.0)		m_dZ_Test_Move_Speed_Up= 10.0;
		if(m_dZ_Test_Move_Delay < 0)		m_dZ_Test_Move_Delay= 100;
		if(m_dZ_Test_Move_Delay > 20000)	m_dZ_Test_Move_Delay= 20000;

		dZ_Test_Move_Dist= m_dZ_Test_Move_Dist;
		dZ_Test_Move_Speed= m_dZ_Test_Move_Speed;
		dZ_Test_Move_Speed_Up= m_dZ_Test_Move_Speed_Up;
		dZ_Test_Move_Delay= m_dZ_Test_Move_Delay;
		m_bRollZ_Enable= FALSE;
		UpdateData(FALSE);

		GetDlgItem(IDB_Z_TEST_START)->EnableWindow(FALSE);	GetDlgItem(IDB_Z_TEST_STOP)->EnableWindow(TRUE);

		dCurrZ1_Pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
		dCurrZ2_Pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z2);

		Thread_Z_Test_Step= 100;
		Thread_Start_Flag= TRUE;

		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
		GetDlgItem(IDB_JOG_Z1_CCW)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_Z1_CW)->EnableWindow(FALSE);
		GetDlgItem(IDB_JOG_Z2_CCW)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_Z2_CW)->EnableWindow(FALSE);
	}
}


void CdlgRollForce_Test::ClickJogZCw3()
{
	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);

	Thread_Start_Flag= FALSE;
	Thread_Z_Test_Step= 0;
	GetDlgItem(IDB_Z_TEST_START)->EnableWindow(TRUE);	GetDlgItem(IDB_Z_TEST_STOP)->EnableWindow(FALSE);
	Sleep(500);

	if(dCurrZ1_Pos < -1.0) return;
	if(dCurrZ2_Pos < -1.0) return;
	if(dCurrZ1_Pos > 20.0) return;
	if(dCurrZ2_Pos > 20.0) return;

	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, dCurrZ1_Pos, dCurrZ2_Pos,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
}
void CdlgRollForce_Test::TestError()
{
	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
	Thread_Start_Flag= FALSE;
	Thread_Z_Test_Step= 0;
	GetDlgItem(IDB_Z_TEST_START)->EnableWindow(TRUE);	GetDlgItem(IDB_Z_TEST_STOP)->EnableWindow(FALSE);
}
//============================================================================================
void CdlgRollForce_Test::RunThread_1(int nID)
{
	m_bExec_1[nID] = TRUE; 
	TREADPARAMS_Z_TEST *pThreadParams = new TREADPARAMS_Z_TEST;
	pThreadParams->pContinue = &m_bExec_1[nID];
	//	pThreadParams->pWnd =	((CMainFrame*)(AfxGetApp()->GetMainWnd()));
	pThreadParams->pWnd =  ((CdlgRollForce_Test*)this);
	if(m_pThread_1[nID] == NULL) {
		switch(nID) {
		case 0: 
			m_pThread_1[nID] = AfxBeginThread(Thread_Z_Test_Proc_1, pThreadParams);  break;
		case 1: 
			break;
		case 2: 
			break;
		}
	}
	m_pThread_1[nID]->m_bAutoDelete = true;
}
void CdlgRollForce_Test::KillThread_1(int nID)
{
	if(m_pThread_1[nID] != NULL)	{
		HANDLE hThread = m_pThread_1[nID]->m_hThread;
		m_bExec_1[nID] = FALSE;
		::WaitForSingleObject(hThread,1000);
		TerminateThread(m_pThread_1[nID]->m_hThread, 0);
		m_pThread_1[nID] = NULL;
	}
}
//===================================================================================
UINT Thread_Z_Test_Proc_1(LPVOID pParam)
//-----------------------------------------------------------------------------------	
{
	TREADPARAMS_Z_TEST *pThreadParams = (TREADPARAMS_Z_TEST *)pParam;
	CWnd *pWnd = pThreadParams->pWnd;//THREADPAPAMS_Mag1;
	BOOL *pContinue = pThreadParams->pContinue;
	delete pThreadParams;
	
	CdlgRollForce_Test* pOwner =  (CdlgRollForce_Test*)pWnd ;
	CString strText;
	bool ret= false;
	double angle= 0.0, posx= 0.0, posy= 0.0;
	DWORD st= 0, st1= 0;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

// 	dZ_Test_Move_Dist= m_dZ_Test_Move_Dist;
// 	dZ_Test_Move_Speed= m_dZ_Test_Move_Speed;
// 	dZ_Test_Move_Delay= m_dZ_Test_Move_Delay;
	while(*pContinue)
	{
		if(PROGRAM_EXIT) { Sleep(10); continue; }
		if(!pOwner->Thread_Start_Flag)  { Sleep(10); continue; }
		switch(pOwner->Thread_Z_Test_Step)
		{
		case 0: Sleep(10); continue;
			break;
		case 100: //-
			if( pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_STAGE_X)  && pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL) &&
				pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Z) &&
				pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z2))
			{
				pSystem.GetMotionComponent()->INC_Move_Dual_Speed(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 
					-pOwner->dZ_Test_Move_Dist, -pOwner->dZ_Test_Move_Dist, 
					pOwner->dZ_Test_Move_Speed_Up, _SPD_MODE);		
				pOwner->Thread_Z_Test_Step= 200;
				Sleep(100);
			}
			else
			{
				strText.Format(_T("[RUN] %s ->위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );	
				MyMessageBox(strText.GetBuffer(0), L"ERROR!", M_ICONERROR);
				pOwner->TestError();
			}
			break;
		case 200: //End
			if( pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1)) 
			{
				st= GetTickCount();
				pOwner->Thread_Z_Test_Step= 300;
			}
			break;
		case 300: //wait
			if(GetTickCount()-st > pOwner->dZ_Test_Move_Delay) pOwner->Thread_Z_Test_Step= 400;
			break;

		case 400: //+
			if( pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_STAGE_X)  && pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL) &&
				pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_LOWER_CAM_Z) &&
				pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isServoOn(_ACS_AXIS_IMP_ROLL_Z2))
			{
				pSystem.GetMotionComponent()->INC_Move_Dual_Speed(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 
					pOwner->dZ_Test_Move_Dist, pOwner->dZ_Test_Move_Dist, 
					pOwner->dZ_Test_Move_Speed, _SPD_MODE);		
				pOwner->Thread_Z_Test_Step= 500;
				Sleep(100);
			}
			else
			{
				strText.Format(_T("[RUN] %s -> 위치 이동 명령 에러!"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1) );	
				MyMessageBox(strText.GetBuffer(0), L"ERROR!", M_ICONERROR);
				pOwner->TestError();
			}
			break;
		case 500: //End
			if( pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1) && pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL_Z1)) 
			{
				st= GetTickCount();
				pOwner->Thread_Z_Test_Step= 600;
			}
			break;
		case 600: //wait
			if(GetTickCount()-st > pOwner->dZ_Test_Move_Delay) pOwner->Thread_Z_Test_Step= 100;
			break;
		}
		Sleep(1);
	}
	return 0;
}


void CdlgRollForce_Test::OnBnClickedCheck5()
{
	UpdateData(TRUE);
	if(b_Graph_Start)
	{
		if(!bGrapgRealDisp) SetTimer(dlgRollForce_Test_TIMER_ID1, 2000, NULL);
	}
	else
	{
		m_bGrapgRealDisp= FALSE;
		UpdateData(FALSE);
	}
	if(m_bGrapgRealDisp)	bGrapgRealDisp= true;
	else					bGrapgRealDisp= false;
}


void CdlgRollForce_Test::OnBnClickedButton7()
{
	if(b_Graph_Start)
	{
		SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE3, 0, 0);
	}
}


void CdlgRollForce_Test::ContainerChangeColorZWaitPos7(long Color)
{
}


void CdlgRollForce_Test::ClickZWaitPos7()
{
	UpdateData(TRUE);
	ParamFile()->Model_data.dRollTest_Z1= m_dZ1_Pos;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_STAGE3_ALIGN);
	ParamFile()->Model_data.dRollTest_Z2= m_dZ2_Pos;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_STAGE3_ALIGN);
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;
	int dispPos_Sel;

	dispPos_Sel= POSITION_STAGE3_ALIGN;

	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}

	ToPosition1	= ParamFile()->Model_data.dRollTest_Z1;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
	ToPosition2	= ParamFile()->Model_data.dRollTest_Z2;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);

	strPos.Format(_T("Z=%.3f[%.3f]"),	
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel));

// 	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X, dispPos_Sel)) 
// 	{
// 		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
// 		return;
// 	}
// 	else 
// 	{
		strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
//	}

	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}


void CdlgRollForce_Test::ClickJogZCw2()
{
	pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}


void CdlgRollForce_Test::ClickZTestStart2()
{
	if(!bBotX_TestMove)
	{
		bBotX_TestMove= false;
		UpdateData(TRUE);
		if(pSystem.GetMotionComponent()->isDone(_ACS_AXIS_LOWER_CAM_X) && !pSystem.GetMotionComponent()->isDone(_ACS_AXIS_STAGE_X))
		{
			//	pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -m_dBotX_MoveDistmm / 2.0, _SPD_MODE);		
			MoveEndCheck(_ACS_AXIS_LOWER_CAM_X);
			bBotX_TestMove= true;
			pSystem.GetMotionComponent()->INC_Move_Speed(_ACS_AXIS_LOWER_CAM_X, m_dBotX_MoveDistmm , m_dInc_MoveSpeed, _SPD_MODE);	
			SetTimer(dlgRollForce_Test_TIMER_ID3, 200, NULL);
			GetDlgItem(IDB_Z_TEST_START2)->EnableWindow(FALSE);
		}
		else
		{
			MyMessageBox(_T("Stage 구동 상태가 아닙니다."), _T("Position-Move Failed"), M_ICONERROR); return;
		}
	}
}


void CdlgRollForce_Test::OnBnClickedRadio1()
{
	UpdateData(TRUE);
	ParamFile()->Model_data.iZ_ControlOption= m_iZ_ControlOption;
}


void CdlgRollForce_Test::OnBnClickedRadio24()
{
	UpdateData(TRUE);
	ParamFile()->Model_data.iZ_ControlOption= m_iZ_ControlOption;
}


void CdlgRollForce_Test::ClickCal4()
{
	if(b_Graph_Start)
	{
		KillTimer(dlgRollForce_Test_TIMER_ID1);
		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		pFrm->GetWindowHandler_MainView()->EndGraph(STAGE_3_GRAPH);//STAGE_3_ALIGN_PROC);
		EndGraph(STAGE_3_ALIGN_PROC);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
		b_Graph_Start= false;
		bGrapgRealDisp= m_bGrapgRealDisp= false;
		UpdateData(FALSE);
	}

	CdlgXZ12_Mat dlg;
	dlg.DoModal();
}


void CdlgRollForce_Test::ClickOpuZCntDis3()
{
	pSystem.GetMotionComponent()->Z_Mapping_Stop();
	m_btnZMap_Cnt_Disable.SetValue(1);
	m_btnZMap_Cnt_Enable.SetValue(0);
}


void CdlgRollForce_Test::ClickOpuZCntEna3()
{
	m_btnZMap_Cnt_Disable.SetValue(0);
	m_btnZMap_Cnt_Enable.SetValue(1);
}


void CdlgRollForce_Test::OnBnClickedButton15()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CdlgRollForce_Test::ClickDataSave()
{
	UpdateData(TRUE);

	if(m_dZ_Test_Move_Dist < 0.0)		m_dZ_Test_Move_Dist= 0.001;
	if(m_dZ_Test_Move_Dist > 10.0)		m_dZ_Test_Move_Dist= 10.0;
	if(m_dZ_Test_Move_Speed < 0.0)		m_dZ_Test_Move_Speed= 0.0001;
	if(m_dZ_Test_Move_Speed > 10.0)		m_dZ_Test_Move_Speed= 10.0;
	if(m_dZ_Test_Move_Speed_Up < 0.0)		m_dZ_Test_Move_Speed_Up= 0.0;
	if(m_dZ_Test_Move_Speed_Up > 10.0)		m_dZ_Test_Move_Speed_Up= 0.0001;
	if(m_dZ_Test_Move_Delay < 0)		m_dZ_Test_Move_Delay= 100;
	if(m_dZ_Test_Move_Delay > 20000)	m_dZ_Test_Move_Delay= 20000;

	dZ_Test_Move_Dist= m_dZ_Test_Move_Dist;
	dZ_Test_Move_Speed= m_dZ_Test_Move_Speed;
	dZ_Test_Move_Speed_Up= m_dZ_Test_Move_Speed_Up;
	dZ_Test_Move_Delay= m_dZ_Test_Move_Delay;



	ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC]= m_dRoll_Diameter;
	ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]= m_dRoll_Diameter_AddVal;

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CdlgRollForce_Test 수정 / 저장"));
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveSytemConfigFile();
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
}


void CdlgRollForce_Test::OnBnClickedCheck4()
{
	UpdateData(TRUE);
	if(m_iZ_Control_Z1Hap)		ParamFile()->Model_data.iZ_Control_Z1Hap= 1;
	else						ParamFile()->Model_data.iZ_Control_Z1Hap= 0;
}


void CdlgRollForce_Test::ClickZWaitPos61()
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;


	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}

	strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY),
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY));

	strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;

	pSystem.GetMotionComponent()->Z_ControlDisable();
	m_btnZ_Cnt_Disable.SetValue(1);	m_btnZ_Cnt_Enable.SetValue(0);

	//-----------------
// 	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 	//-----------------
// 	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
// 	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);


	////////////////////////////////
	if(dZ_Test_Move_Dist <= 0.0)
	{
		MyMessageBox(_T("Z축 저속 구간 입력이 -값입니다.\n저속 이동 명령이 취소 되었습니다."), _T("Position-Move Failed"), M_ICONERROR);
	}
	else
	{
		if(m_dZ_Test_Move_Speed_Up <= 0.0)
		{
			MyMessageBox(_T("Z축 저속 구간 속도 입력이 0보다 작거나 같은 값입니다.\n저속 이동 명령이 취소 되었습니다."), _T("Position-Move Failed"), M_ICONERROR);
		}
		else
		{
			pSystem.GetMotionComponent()->INC_Move_Dual_Speed(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 
				-dZ_Test_Move_Dist, -dZ_Test_Move_Dist, 
				dZ_Test_Move_Speed_Up, _SPD_MODE);		

			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 			Sleep(m_dZ_Test_Move_Delay);
		}
	}

// 	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
// 	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
// 	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	///////////////////////////////

// 	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
// 	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
}
