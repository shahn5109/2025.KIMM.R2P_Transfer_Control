// frmSetupView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmSetupView.h"
#include "MainFrm.h"


// CfrmSetupView
extern CSystemClass pSystem;
CParameterFile* CfrmSetupView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmSetupView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmSetupView, CFormView)

	CfrmSetupView::CfrmSetupView()
	: CFormView(CfrmSetupView::IDD)
	, m_HOME_TIME_OUT(0)
	, m_AXIS_MOVE_TIME_OUT(0)
	, m_dTop_Cam_PixelToum_2X(0)
	, m_dTop_Cam_PixelToum_20X(0)
	, m_dBot_Cam_PixelToum_2X(0)
	, m_dBot_Cam_PixelToum_20X(0)
	, m_iImageGrab_Delay(0)
	, m_dZ_Axis_Interlock(0)
	, m_X_SAFE_FREEZONE_L(0)
	, m_X_SAFE_FREEZONE_H(0)
	, m_X_SAFE_ZONE_L1(0)
	, m_X_SAFE_ZONE_L2(0)
	, m_dX_Axis_Interlock(0)
	, m_AF_Stepmm_20X(0)
	, m_AF_Stepmm_2X(0)
	, m_iAF_TotalCount(0)
	, iCAM_20X_USED_FLAG(0)
	, m_dHome_Offset_BX(0)
	, m_Q_Offset_2X(0)
	, m_Q_Offset_20X(0)
	, m_Roll_Offset_2X(0)
	, m_Roll_Offset_20X(0)
	, m_Led_Remote(0)
	, m_dRollAlign_ZPos(0)
	, m_iRollTeachingDataUsed(0)
	, m_dRollRun_BZ_Pos(0)
	, m_dRectSizeX(0)
	, m_dRectSizeY(0)
	, m_iRoll_AngleAlignMode(0)
	, m_dRoll_AngleAlign_Pitch(0)
	, m_iRoll_AngleAlign_Count(0)
	, m_dZ2HomeOffsetVal(0)
	, m_bRollAF_Enable(FALSE)
{

}

CfrmSetupView::~CfrmSetupView()
{
}

void CfrmSetupView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ORG, m_HOME_TIME_OUT);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS, m_AXIS_MOVE_TIME_OUT);
	DDX_Text(pDX, IDC_EDIT_CAM_RES_T_X2, m_dTop_Cam_PixelToum_2X);
	DDX_Text(pDX, IDC_EDIT_CAM_RES_T_X20, m_dTop_Cam_PixelToum_20X);
	DDX_Text(pDX, IDC_EDIT_CAM_RES_B_X2, m_dBot_Cam_PixelToum_2X);
	DDX_Text(pDX, IDC_EDIT_CAM_RES_B_X20, m_dBot_Cam_PixelToum_20X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY, m_iImageGrab_Delay);
	DDX_Text(pDX, IDC_EDIT_OFFSET_39, m_dZ_Axis_Interlock);
	DDX_Text(pDX, IDC_EDIT_OFFSET_36, m_X_SAFE_FREEZONE_L);
	DDX_Text(pDX, IDC_EDIT_OFFSET_35, m_X_SAFE_FREEZONE_H);
	DDX_Text(pDX, IDC_EDIT_OFFSET_38, m_X_SAFE_ZONE_L1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_37, m_X_SAFE_ZONE_L2);
	DDX_Text(pDX, IDC_EDIT_OFFSET_40, m_dX_Axis_Interlock);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY3, m_AF_Stepmm_20X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY2, m_AF_Stepmm_2X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY4, m_iAF_TotalCount);
	DDX_Radio(pDX, IDC_RADIO1, iCAM_20X_USED_FLAG);
	DDX_Text(pDX, IDC_EDIT_OFFSET_41, m_dHome_Offset_BX);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY5, m_Q_Offset_2X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY6, m_Q_Offset_20X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY7, m_Roll_Offset_2X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY8, m_Roll_Offset_20X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY9, m_Q_Offset_2X_Y);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY10, m_Q_Offset_20X_Y);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY11, m_Roll_Offset_2X_Y);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY12, m_Roll_Offset_20X_Y);
	DDX_Radio(pDX, IDC_RADIO7, m_Led_Remote);
	DDX_Text(pDX, IDC_EDIT_OFFSET_42, m_dRollAlign_ZPos);
	DDX_Radio(pDX, IDC_RADIO18, m_iRollTeachingDataUsed);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY13, m_dRollRun_BZ_Pos);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY14, m_dRectSizeX);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY15, m_dRectSizeY);
	DDX_Radio(pDX, IDC_RADIO19, m_iRoll_AngleAlignMode);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY16, m_dRoll_AngleAlign_Pitch);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY17, m_iRoll_AngleAlign_Count);
	DDX_Text(pDX, IDC_EDIT_OFFSET_43, m_dZ2HomeOffsetVal);
	DDX_Check(pDX, IDC_CHECK3, m_bRollAF_Enable);
}

BEGIN_MESSAGE_MAP(CfrmSetupView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MSG_DATA_UPDATE, DataReload)
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmSetupView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &CfrmSetupView::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CfrmSetupView::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO7, &CfrmSetupView::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO18, &CfrmSetupView::OnBnClickedRadio18)
	ON_BN_CLICKED(IDC_BUTTON5, &CfrmSetupView::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK3, &CfrmSetupView::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CfrmSetupView 진단입니다.

#ifdef _DEBUG
void CfrmSetupView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmSetupView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmSetupView 메시지 처리기입니다.

//===================================================================================
void CfrmSetupView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_SetupView(this);

	DataUpdate();

// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_bCfrmSetupViewInit= TRUE;
}

//===================================================================================
void CfrmSetupView::OnDestroy()
{
	CFormView::OnDestroy();

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmSetupView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmSetupView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmSetupView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
LRESULT CfrmSetupView::DataReload(WPARAM wParam,LPARAM lParam)
{
	DataUpdate();
	return 0;
}
//===================================================================================
void CfrmSetupView::DataUpdate()
{
	CString strTmp;

	//	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
// 	ParamFile()->OpenSytemConfigFile();
// 	ParamFile()->OpenModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	if(ParamFile()->Motion_data.dRollRun_BZ_Pos > 0.0) ParamFile()->Motion_data.dRollRun_BZ_Pos= 0.0;
	if(ParamFile()->Motion_data.dRollRun_BZ_Pos < -4.0) ParamFile()->Motion_data.dRollRun_BZ_Pos= -4.0;
		m_dRollRun_BZ_Pos= ParamFile()->Motion_data.dRollRun_BZ_Pos;

	m_dTop_Cam_PixelToum_2X= ParamFile()->System_data.dTop_Cam_PixelToum_2X;
	m_dTop_Cam_PixelToum_20X= ParamFile()->System_data.dTop_Cam_PixelToum_20X;
	m_dBot_Cam_PixelToum_2X= ParamFile()->System_data.dBot_Cam_PixelToum_2X;
	m_dBot_Cam_PixelToum_20X= ParamFile()->System_data.dBot_Cam_PixelToum_20X;

	m_iImageGrab_Delay= ParamFile()->System_data.iImageGrab_Delay;

	m_HOME_TIME_OUT= ParamFile()->System_data._HOME_TIME_OUT;
	m_AXIS_MOVE_TIME_OUT= ParamFile()->System_data._AXIS_MOVE_TIME_OUT;

	m_dZ_Axis_Interlock= ParamFile()->Motion_data.dZ_Axis_Interlock;

	m_X_SAFE_FREEZONE_L= ParamFile()->System_data.X_SAFE_FREEZONE_L;
	m_X_SAFE_FREEZONE_H= ParamFile()->System_data.X_SAFE_FREEZONE_H;
	m_X_SAFE_ZONE_L1= ParamFile()->System_data.X_SAFE_ZONE_L1;
	m_X_SAFE_ZONE_L2= ParamFile()->System_data.X_SAFE_ZONE_L2;

	m_dX_Axis_Interlock= ParamFile()->System_data.dX_Axis_Interlock;

	m_AF_Stepmm_20X= ParamFile()->System_data.AF_Stepmm_20X;
	m_AF_Stepmm_2X= ParamFile()->System_data.AF_Stepmm_2X;
	m_iAF_TotalCount= ParamFile()->System_data.iAF_TotalCount;

	iCAM_20X_USED_FLAG= ParamFile()->System_data.iCAM_20X_USED_FLAG;

	m_dHome_Offset_BX= ParamFile()->System_data.dHome_Offset_BX;
	m_Q_Offset_2X= ParamFile()->System_data.TB_Offset_2X_X;
// 	m_Q_Offset_20X= ParamFile()->System_data.Q_Offset_20X;
// 	m_Roll_Offset_2X= ParamFile()->System_data.Roll_Offset_2X;
// 	m_Roll_Offset_20X= ParamFile()->System_data.Roll_Offset_20X;

	m_Q_Offset_2X_Y= ParamFile()->System_data.TB_Offset_2X_Y;
// 	m_Q_Offset_20X_Y= ParamFile()->System_data.Q_Offset_20X_Y;
// 	m_Roll_Offset_2X_Y= ParamFile()->System_data.Roll_Offset_2X_Y;
// 	m_Roll_Offset_20X_Y= ParamFile()->System_data.Roll_Offset_20X_Y;

	m_Led_Remote= ParamFile()->System_data.Led_Remote;

	m_dRollAlign_ZPos= ParamFile()->System_data.dRollAlign_ZPos;

	m_iRollTeachingDataUsed= ParamFile()->System_data.iRollTeachingDataUsed;

	m_dRectSizeX= ParamFile()->System_data.dRectSizeX;
	m_dRectSizeY= ParamFile()->System_data.dRectSizeY;

	m_iRoll_AngleAlignMode= ParamFile()->Model_data.iRoll_AngleAlignMode;	
	m_dRoll_AngleAlign_Pitch= ParamFile()->Model_data.dRoll_AngleAlign_Pitch;	
	m_iRoll_AngleAlign_Count= ParamFile()->Model_data.iRoll_AngleAlign_Count;	
	if(ParamFile()->System_data.bRollAF_Enable) 
		m_bRollAF_Enable= TRUE;
	else
		m_bRollAF_Enable= FALSE;

	m_dZ2HomeOffsetVal= ParamFile()->System_data.dZ2HomeOffsetVal;
	UpdateData(FALSE);
}
//===================================================================================
void CfrmSetupView::DataSave()
{
	UpdateData(TRUE);

	if(m_bRollAF_Enable) 
		ParamFile()->System_data.bRollAF_Enable= 1;
	else
		ParamFile()->System_data.bRollAF_Enable= 0;

	ParamFile()->System_data.dRectSizeX= m_dRectSizeX;
	ParamFile()->System_data.dRectSizeY= m_dRectSizeY;
	ParamFile()->Motion_data.dRollRun_BZ_Pos= m_dRollRun_BZ_Pos;
	if(ParamFile()->Motion_data.dRollRun_BZ_Pos > 0.0) ParamFile()->Motion_data.dRollRun_BZ_Pos= 0.0;
	if(ParamFile()->Motion_data.dRollRun_BZ_Pos < -4.0) ParamFile()->Motion_data.dRollRun_BZ_Pos= -4.0;

	ParamFile()->System_data.AF_Stepmm_20X= m_AF_Stepmm_20X;
	ParamFile()->System_data.AF_Stepmm_2X= m_AF_Stepmm_2X;
	ParamFile()->System_data.iAF_TotalCount= m_iAF_TotalCount;

	ParamFile()->System_data.dTop_Cam_PixelToum_2X= m_dTop_Cam_PixelToum_2X;
	ParamFile()->System_data.dTop_Cam_PixelToum_20X= m_dTop_Cam_PixelToum_20X;
	ParamFile()->System_data.dBot_Cam_PixelToum_2X= m_dBot_Cam_PixelToum_2X;
	ParamFile()->System_data.dBot_Cam_PixelToum_20X= m_dBot_Cam_PixelToum_20X;

	ParamFile()->System_data.iImageGrab_Delay= m_iImageGrab_Delay;

	ParamFile()->System_data._HOME_TIME_OUT= m_HOME_TIME_OUT;
	ParamFile()->System_data._AXIS_MOVE_TIME_OUT= m_AXIS_MOVE_TIME_OUT;

	ParamFile()->Motion_data.dZ_Axis_Interlock= m_dZ_Axis_Interlock;
	
	if(m_X_SAFE_FREEZONE_L> 0.0 || m_X_SAFE_FREEZONE_L< -10.0) m_X_SAFE_FREEZONE_L= -10.0;
	if(m_X_SAFE_FREEZONE_H> 10.0) m_X_SAFE_FREEZONE_H= 10.0;


	ParamFile()->System_data.X_SAFE_FREEZONE_L= m_X_SAFE_FREEZONE_L;
	ParamFile()->System_data.X_SAFE_FREEZONE_H= m_X_SAFE_FREEZONE_H;
	ParamFile()->System_data.X_SAFE_ZONE_L1= m_X_SAFE_ZONE_L1;
	ParamFile()->System_data.X_SAFE_ZONE_L2= m_X_SAFE_ZONE_L2;

	ParamFile()->System_data.iCAM_20X_USED_FLAG= iCAM_20X_USED_FLAG;

	ParamFile()->System_data.dX_Axis_Interlock= m_dX_Axis_Interlock;

	ParamFile()->System_data.dHome_Offset_BX= m_dHome_Offset_BX;

	ParamFile()->System_data.TB_Offset_2X_X= m_Q_Offset_2X;
//	ParamFile()->System_data.Q_Offset_20X= m_Q_Offset_20X;
//	ParamFile()->System_data.Roll_Offset_2X= m_Roll_Offset_2X;
//	ParamFile()->System_data.Roll_Offset_20X= m_Roll_Offset_20X;
	ParamFile()->System_data.TB_Offset_2X_Y= m_Q_Offset_2X_Y;
//	ParamFile()->System_data.Q_Offset_20X_Y= m_Q_Offset_20X_Y;
//	ParamFile()->System_data.Roll_Offset_2X_Y= m_Roll_Offset_2X_Y;
//	ParamFile()->System_data.Roll_Offset_20X_Y= m_Roll_Offset_20X_Y;

	ParamFile()->System_data.Led_Remote= m_Led_Remote;

	ParamFile()->System_data.dRollAlign_ZPos= m_dRollAlign_ZPos;
	ParamFile()->System_data.iRollTeachingDataUsed= m_iRollTeachingDataUsed;

	ParamFile()->Model_data.iRoll_AngleAlignMode= m_iRoll_AngleAlignMode;	
	if(m_dRoll_AngleAlign_Pitch <= 0.0) m_dRoll_AngleAlign_Pitch= 1.0;
	if(m_dRoll_AngleAlign_Pitch > 99.0) m_dRoll_AngleAlign_Pitch= 99.0;
	ParamFile()->Model_data.dRoll_AngleAlign_Pitch= m_dRoll_AngleAlign_Pitch;	
	if(m_iRoll_AngleAlign_Count < 1) m_iRoll_AngleAlign_Count= 1;
	if(m_iRoll_AngleAlign_Count > 99) m_iRoll_AngleAlign_Count= 99;
	ParamFile()->Model_data.iRoll_AngleAlign_Count= m_iRoll_AngleAlign_Count;	

	ParamFile()->System_data.dZ2HomeOffsetVal= m_dZ2HomeOffsetVal;

/*
	if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE || ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
	{
		if(ParamFile()->System_data.iCAM_20X_USED_FLAG)
		{
			MyMessageBox(_T("자/수동 10X사용 Align-모드는 쿼츠 위에서 동작 되어야 합니다."), _T("Align-Mode"), M_ICONINFORMATION);
			ParamFile()->System_data.iCAM_20X_USED_FLAG= 0;
		}
	}
*/
	if(ParamFile()->System_data.iCAM_20X_USED_FLAG)
	{
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{
			MyMessageBox(_T("자/수동 10X사용 Align-모드는 수동 Align 동작 되어야 합니다."), _T("Align-Mode"), M_ICONINFORMATION);
			ParamFile()->System_data.iCAM_20X_USED_FLAG= 0;
		}
	}
	pSystem.GetMotionComponent()->CalAlignPos();

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CfrmDataView 수정 / 저장"));
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveSytemConfigFile();
	ParamFile()->SaveMotionParamFile();//ParamFile()->System_data.strCurrentModelName);
	pSystem.GetVisionComponent()->SetCameraResolution();

}
//===================================================================================
BEGIN_EVENTSINK_MAP(CfrmSetupView, CFormView)
	ON_EVENT(CfrmSetupView, IDB_DATA_SAVE, DISPID_CLICK, CfrmSetupView::ClickDataSave, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CfrmSetupView::ClickDataSave()
{
	if (MyMessageBox(_T("입력된 값을 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmSetupView : OnClickDataSave.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmSetupView : OnClickDataSave.!"));

	DataSave();

	DataUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
}


void CfrmSetupView::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CfrmSetupView::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// void CfrmSetupView::OnBnClickedRadio3() //10배 사용
// {
// // 	if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE || ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_MANU_MODE)
// // 	{
// // 		MyMessageBox(_T("자/수동 10X사용 Align-모드는 쿼츠 위에서 동작 되어야 합니다."), _T("Align-Mode"), M_ICONINFORMATION);
// // 		iCAM_20X_USED_FLAG= ParamFile()->System_data.iCAM_20X_USED_FLAG= 0;
// // 		UpdateData(FALSE);
// // 	}
// }


void CfrmSetupView::OnBnClickedRadio3()
{
}


void CfrmSetupView::OnBnClickedRadio7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CfrmSetupView::OnBnClickedRadio18()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CfrmSetupView::OnBnClickedButton5()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_AlignView()->ReCOM_Conn();
}


void CfrmSetupView::OnBnClickedCheck3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
