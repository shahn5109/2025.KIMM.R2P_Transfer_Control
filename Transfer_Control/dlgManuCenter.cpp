// dlgManuCenter.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgManuCenter.h"
#include "afxdialogex.h"
#include "MainFrm.h"

extern CSystemClass pSystem;
CParameterFile* CdlgManuCenter::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgManuCenter::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgManuCenter 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgManuCenter, CDialogEx)

CdlgManuCenter::CdlgManuCenter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgManuCenter::IDD, pParent)
	, m_dCamPos_1st_X(0)
	, m_dCamPos_1st_Y(0)
	, m_dCamPos_2nd_X(0)
	, m_dCamPos_2nd_Y(0)
	, m_dResult_Len(0)
	, m_dResult_Deg(0)
	, m_iAlignExc(0)
	, m_dStX_1(0)
	, m_dStY_1(0)
	, m_dStX_2(0)
	, m_dStY_2(0)
{
	iSelect_B_Layer= 0;
}

CdlgManuCenter::~CdlgManuCenter()
{
}

void CdlgManuCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_COMMAND5_X3, m_btnTitle);
	DDX_Control(pDX, IDB_JOG_SELECT_3, m_radio2X);
	DDX_Control(pDX, IDB_JOG_SELECT_4, m_radio20X);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL1, m_btnStageSel1);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL3, m_btnSelectPoint);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);


	
	DDX_Text(pDX, IDC_EDIT_OFFSET_33, m_dCamPos_1st_X);
	DDX_Text(pDX, IDC_EDIT_OFFSET_34, m_dCamPos_1st_Y);
	DDX_Text(pDX, IDC_EDIT_OFFSET_35, m_dCamPos_2nd_X);
	DDX_Text(pDX, IDC_EDIT_OFFSET_36, m_dCamPos_2nd_Y);
	

	DDX_Text(pDX, IDC_EDIT_OFFSET_37, m_dResult_Len);
	DDX_Text(pDX, IDC_EDIT_OFFSET_38, m_dResult_Deg);
	DDX_Radio(pDX, IDC_RADIO1, m_iAlignExc);
	DDX_Text(pDX, IDC_EDIT_OFFSET_39, m_dStX_1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_40, m_dStY_1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_41, m_dStX_2);
	DDX_Text(pDX, IDC_EDIT_OFFSET_42, m_dStY_2);
}


BEGIN_MESSAGE_MAP(CdlgManuCenter, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgManuCenter::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CdlgManuCenter::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON7, &CdlgManuCenter::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CdlgManuCenter::OnBnClickedButton6)
END_MESSAGE_MAP()


// CdlgManuCenter 메시지 처리기입니다.


//===================================================================================
int CdlgManuCenter::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

//===================================================================================
BOOL CdlgManuCenter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ImageCtrl.Create( &m_ImageMng, this );
	m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
	m_ImageCtrl.ImageUpdate();

	WINDOWPLACEMENT wndPlacement;
	CRect rcDlgSize;

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

	m_pt1st.SetPoint( 0, 0 );
	m_pt2nd.SetPoint( 0, 0 );

	m_ImageCtrl.ShowWindow( SW_SHOW );

	m_SelectMarkNo= eALIGN_1ST_MARK;
	Jog_Mode= _MID_SPEED;
	m_btnJogModeSel1.SetValue(1);
	mTopCameraNo= e2X_LENS;
	m_radio2X.SetValue(1);	m_radio20X.SetValue(0);
	m_btnSelectPoint.SetValue(0);

	Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	Select_Pos_Y2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
	Select_Pos_Y2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

	DispUpdate();
	GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE);

	iSelMarkPos= 0;

	dCur_Pos_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	dCur_Pos_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

	CameraStageUsedNo= eTOP_CAM; mTopCameraNo= e2X_LENS;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
//===================================================================================
void CdlgManuCenter::DispUpdate()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CString strText;

	GetDlgItem(IDB_CM_MOVE6)->ShowWindow(SW_HIDE);		GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_HIDE);  GetDlgItem(IDB_CM_MOVE8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_SELECT_10)->ShowWindow(SW_HIDE);	GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);
	if(mTopCameraNo== e2X_LENS) 
	{
		m_btnTitle.SetCaption(_T("Align 기준 위치 설정\n상부 카메라 [Lens:2X]"));
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
		m_ImageCtrl.ImageUpdate();
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	}
	else			
	{
		m_btnTitle.SetCaption(_T("Align 기준 위치 설정\n상부 카메라 [Lens:10X]"));
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_20X ) );
		m_ImageCtrl.ImageUpdate();
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	}
	iStageJog_Select= STAGE_SEL_1;
//	m_btnStageSel1.SetValue(1);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("상부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("상부 Vision\nStage +"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("상부 Vision\nStage Z-"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("상부 Vision\nStage Z+"));
	strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[mdTOP_2X_STAEG1_START]);
	SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));

	SetDlgItemText(IDB_JOG_SELECT_8, _T("2nd Mark\nPitch 이동"));
	SetDlgItemText(IDB_JOG_SELECT_9, _T("1st Mark\nPitch 이동"));

	UpdateData(FALSE);
}
//===================================================================================
void CdlgManuCenter::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(dlgManuAlign_TIMER_ID);
	pSystem.GetVisionComponent()->SetTempImageCtrl( NULL );

}

//===================================================================================
BOOL CdlgManuCenter::DestroyWindow()
{
	KillTimer(dlgManuAlign_TIMER_ID);
	pSystem.GetVisionComponent()->SetTempImageCtrl( NULL );

	return CDialogEx::DestroyWindow();
}

//===================================================================================
//===================================================================================
void CdlgManuCenter::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
BOOL CdlgManuCenter::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){ 
		if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)){          return TRUE;        }   
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//===================================================================================
BEGIN_EVENTSINK_MAP(CdlgManuCenter, CDialogEx)
	ON_EVENT(CdlgManuCenter, IDB_EXIT, DISPID_CLICK, CdlgManuCenter::ClickExit, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_3, DISPID_CLICK, CdlgManuCenter::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_4, DISPID_CLICK, CdlgManuCenter::ClickJogSelect4, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CdlgManuCenter::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CdlgManuCenter::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CdlgManuCenter::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CdlgManuCenter::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEDOWN, CdlgManuCenter::MouseDownJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEUP, CdlgManuCenter::MouseUpJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEDOWN, CdlgManuCenter::MouseDownJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEUP, CdlgManuCenter::MouseUpJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_T_CW, DISPID_MOUSEDOWN, CdlgManuCenter::MouseDownJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_T_CW, DISPID_MOUSEUP, CdlgManuCenter::MouseUpJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEDOWN, CdlgManuCenter::MouseDownJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEUP, CdlgManuCenter::MouseUpJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuCenter, IDB_STAGE_JOG_SEL1, DISPID_CLICK, CdlgManuCenter::ClickStageJogSel1, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_STAGE_JOG_SEL2, DISPID_CLICK, CdlgManuCenter::ClickStageJogSel2, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_3, DISPID_DBLCLICK, CdlgManuCenter::DblClickJogSelect3, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_4, DISPID_DBLCLICK, CdlgManuCenter::DblClickJogSelect4, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_6, DISPID_CLICK, CdlgManuCenter::ClickJogSelect6, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_7, DISPID_CLICK, CdlgManuCenter::ClickJogSelect7, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_STAGE_JOG_SEL3, DISPID_CLICK, CdlgManuCenter::ClickStageJogSel3, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_STAGE_JOG_SEL4, DISPID_CLICK, CdlgManuCenter::ClickStageJogSel4, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_EXIT2, DISPID_CLICK, CdlgManuCenter::ClickExit2, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_0, DISPID_CLICK, CdlgManuCenter::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_1, DISPID_CLICK, CdlgManuCenter::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_2, DISPID_CLICK, CdlgManuCenter::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_8, DISPID_CLICK, CdlgManuCenter::ClickJogSelect8, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_9, DISPID_CLICK, CdlgManuCenter::ClickJogSelect9, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_CM_MOVE, DISPID_CLICK, CdlgManuCenter::ClickCmMove, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_CM_MOVE6, DISPID_CLICK, CdlgManuCenter::ClickCmMove6, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_10, DISPID_CLICK, CdlgManuCenter::ClickJogSelect10, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_11, DISPID_CLICK, CdlgManuCenter::ClickJogSelect11, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_CM_MOVE7, DISPID_CLICK, CdlgManuCenter::ClickCmMove7, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_JOG_SELECT_12, DISPID_CLICK, CdlgManuCenter::ClickJogSelect12, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_CM_MOVE8, DISPID_CLICK, CdlgManuCenter::ClickCmMove8, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_CM_MARK_LOAD, DISPID_DBLCLICK, CdlgManuCenter::DblClickCmMarkLoad, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_CM_MOVE9, DISPID_CLICK, CdlgManuCenter::ClickCmMove9, VTS_NONE)
	ON_EVENT(CdlgManuCenter, IDB_STAGE_JOG_SEL6, DISPID_CLICK, CdlgManuCenter::ClickStageJogSel6, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CdlgManuCenter::ClickExit()
{
	OnOK();
}
void CdlgManuCenter::ClickExit2()
{
	OnCancel();
}

//===================================================================================
void CdlgManuCenter::OnTimer(UINT_PTR nIDEvent)
{

	PostMessage(WM_CLOSE);

	CDialogEx::OnTimer(nIDEvent);
}

//===================================================================================
void CdlgManuCenter::OnBnClickedButton1()
{
	CString strText;

//	SetTimer(dlgManuAlign_TIMER_ID, /*dlgManuAlign_TIMER_INTERVAL*/2000, NULL);
	double UpData[_MAX_AXIS];
	int i;

	for(i= 0; i< _MAX_AXIS; i++)	UpData[i]= 0.0;

	UpData[_ACS_AXIS_IMP_ROLL_Z1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	UpData[_ACS_AXIS_IMP_ROLL_Z2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);

	UpData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	UpData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
	UpData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
	UpData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	UpData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
	UpData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
	UpData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);


	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];

	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]= UpData[_ACS_AXIS_LOWER_CAM_X]-ParamFile()->System_data.dBotCam20To2_X;
	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y]-ParamFile()->System_data.dBotCam20To2_Y;

	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Z]= 
		ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Z];
	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_ALIGN_Y1]= UpData[_ACS_AXIS_ALIGN_Y1];
	ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_ALIGN_Y2]= UpData[_ACS_AXIS_ALIGN_Y2];

	 
	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	double masktobtm_X, masktobtm_Y;
	double pos1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	double pos2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK);
	masktobtm_X= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]- pos1;
	masktobtm_Y= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]- pos2;

	double totopmask_x, totopmask_y;
	pos1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	pos2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);
	totopmask_x= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]-pos1;
	totopmask_y= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_UPPER_CAM_Y]- pos2;

	double tobotmask_x;
	pos1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	tobotmask_x= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]-pos1;

	double move_xpos= totopmask_x- tobotmask_x - masktobtm_X;
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, move_xpos,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, masktobtm_Y,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

//	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);


}
//===================================================================================
void APIENTRY CdlgManuCenter::OnDrawExt( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData ) 
{
	CdlgManuCenter* pThis = (CdlgManuCenter*)lpUsrData;
	CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
	CImageObj* pImgObj = pThis->m_ImageCtrl.GetVisibleImageObject();

	pThis->CameraStageUsedNo= eTOP_CAM;
	if ( TRUE )
	{
		CPen DotPen;
		DotPen.CreatePen( PS_DASHDOT, 1, RGB(0xff, 0xff, 0) );
		CPen DotPen1;
		DotPen1.CreatePen( PS_SOLID, 1, RGB(255, 155, 155) );
		CPen DotPenR;
		DotPenR.CreatePen( PS_SOLID, 1, RGB(255, 50, 50) );
		CPen DotPenB;
		DotPenB.CreatePen( PS_DASHDOT, 1, RGB(50, 50, 255) );

			int nOldBkMode = pDC->SetBkMode( TRANSPARENT );
			CPoint ptCenter = pIDC->ImagePosToScreenPos( (int)pImgObj->GetWidth()/2, (int)pImgObj->GetHeight()/2 );
			CPen* pOldPen = pDC->SelectObject( &DotPen );
			pDC->MoveTo( ptCenter.x, 0 );
			pDC->LineTo( ptCenter.x, ptCenter.y*2 );
			pDC->MoveTo( 0, ptCenter.y );
			pDC->LineTo( ptCenter.x*2, ptCenter.y );

			pDC->SelectObject( &DotPen1 );
			double cnv_x, cnv_y;
			if(pThis->mTopCameraNo== e20X_LENS)
			{
				// 				cnv_x= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
				// 				cnv_y= pSystem.GetParameterFileComponent()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y / (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
				// 				cnv_x= (_INSP_IMAGE_SIZE_X/2.0)-cnv_x;			cnv_y= (_INSP_IMAGE_SIZE_Y/2.0)-cnv_y;
				if(pThis->CameraStageUsedNo== eTOP_CAM)
				{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix;			cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix;	}
				else
				{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix;			cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix;	}

				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x-10, ptCenter.y );
				pDC->LineTo( ptCenter.x+10, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-10 );
				pDC->LineTo( ptCenter.x, ptCenter.y+10 );

				if(pThis->CameraStageUsedNo== eTOP_CAM)
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
					if(pThis->CameraStageUsedNo== eTOP_CAM) { cnv_x= (_INSP_IMAGE_SIZE_X/2.0);			cnv_y= (_INSP_IMAGE_SIZE_Y/2.0); }
					else
					{
						cnv_x= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix-(_INSP_IMAGE_SIZE_X/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_X/2.0);
						cnv_y= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix-(_INSP_IMAGE_SIZE_Y/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_Y/2.0);
					}
				}
				else
				{
					if(pThis->CameraStageUsedNo== eTOP_CAM)
					{	cnv_x= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix;	cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix; }
					else
					{	cnv_x= /*_INSP_IMAGE_SIZE_X-*/pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix;	cnv_y= pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix; }
				}
				ptCenter = pIDC->ImagePosToScreenPos( (int)(cnv_x),	(int)(cnv_y));
				pDC->MoveTo( ptCenter.x-10, ptCenter.y );
				pDC->LineTo( ptCenter.x+10, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-10 );
				pDC->LineTo( ptCenter.x, ptCenter.y+10 );

				if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG)
				{
					if(pThis->CameraStageUsedNo== eTOP_CAM) { cnv_x= (_INSP_IMAGE_SIZE_X/2.0);			cnv_y= (_INSP_IMAGE_SIZE_Y/2.0); }
					else
					{
						cnv_x= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Xpix-(_INSP_IMAGE_SIZE_X/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_X/2.0);
						cnv_y= ((pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Ypix-(_INSP_IMAGE_SIZE_Y/2.0))*cal_fac)+(_INSP_IMAGE_SIZE_Y/2.0);
					}
				}
				else
				{
					if(pThis->CameraStageUsedNo== eTOP_CAM)
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
	}
}
//===================================================================================
BOOL APIENTRY CdlgManuCenter::OnFireMouseEvent(  DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData )
{
	CdlgManuCenter* pThis = (CdlgManuCenter*)lpUsrData;

	if( dwMsg == WM_MOUSEMOVE )
	{
		if( (pThis->m_nClickAble == 1) )
		{
// 			COLORCROSS colorCross;
// 			colorCross.CreateObject( PDC_BLUE, pThis->m_ImageCtrl.GetMousePositionOnImage().x, pThis->m_ImageCtrl.GetMousePositionOnImage().y, 50,10 );
// 			CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
// 			pGO.Reset();
// 
// 			pGO.AddDrawCross( colorCross ); //화면에 선택된 좌표 표시

// 			pThis->m_pt1st.x = pThis->m_ImageCtrl.GetMousePositionOnImage().x;
// 			pThis->m_pt1st.y = pThis->m_ImageCtrl.GetMousePositionOnImage().y;
// 
// 			CString strPosition;
// 			strPosition.Format( _T("X : %d, Y : %d"), pThis->m_pt1st.x, pThis->m_pt1st.y);
// 			COLORTEXT colorText;
// 			colorText.CreateObject( PDC_BLUE, pThis->m_pt1st.x+10, pThis->m_pt1st.y-20, 12 );
// 			colorText.SetText( strPosition );
// 			pGO.AddDrawText( colorText );
 			TRACE(_T("%d/%d [%d:%d\n"), pThis->m_pt1st.x, pThis->m_pt1st.y, point.x, point.y);
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_33, pThis->m_pt1st.x);
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_34, pThis->m_pt1st.y);
		}
	}
	else if( dwMsg == WM_LBUTTONUP )
	{
		if( (pThis->m_nClickAble == 1) )
		{
			COLORCROSS colorCross;
			colorCross.CreateObject( PDC_GREENYELLOW, pThis->m_ImageCtrl.GetMousePositionOnImage().x, pThis->m_ImageCtrl.GetMousePositionOnImage().y, 150 );

			CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
			pGO.Reset();

			pGO.AddDrawCross( colorCross ); //화면에 선택된 좌표 표시

			pThis->m_pt1st.x = pThis->m_ImageCtrl.GetMousePositionOnImage().x;
			pThis->m_pt1st.y = pThis->m_ImageCtrl.GetMousePositionOnImage().y;

			CString strPosition;
			strPosition.Format( _T("X : %d, Y : %d"), pThis->m_pt1st.x, pThis->m_pt1st.y);
			COLORTEXT colorText;
			colorText.CreateObject( PDC_GREEN, pThis->m_pt1st.x+10, pThis->m_pt1st.y-20, 12 );
			colorText.SetText( strPosition );
			pGO.AddDrawText( colorText );

			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_33, pThis->m_pt1st.x);
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_34, pThis->m_pt1st.y);
			if(pThis->iSelect_B_Layer== 0)
			{
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X= pThis->m_pt1st.x;
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y= pThis->m_pt1st.y;
				pThis->m_dStX_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X;
				pThis->m_dStY_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y;
				pThis->SetDlgItemInt(IDC_EDIT_OFFSET_39, (UINT)(pThis->m_dStX_1));
				pThis->SetDlgItemInt(IDC_EDIT_OFFSET_40, (UINT)(pThis->m_dStY_1));
				return FALSE;
			}
			else
			{
				pThis->m_dCamPos_1st_X= pThis->m_pt1st.x;
				pThis->m_dCamPos_1st_Y= pThis->m_pt1st.y;
				if(pThis->m_StageNo== MANUAL_STAMP_PROC)
				{
					if(pThis->CameraStageUsedNo== eBTM_CAM)
					{
						//pThis->Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
						pThis->Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
						pThis->Select_Pos_Y1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
					}
					else
					{
						pThis->Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
						pThis->Select_Pos_Y1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
					}
				}
				else
				{
					pThis->Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
					pThis->Select_Pos_Y1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
				}
			}
		}

		if( (pThis->m_nClickAble == 2) )
		{
			COLORCROSS colorCross;
			colorCross.CreateObject( PDC_GREENYELLOW, pThis->m_ImageCtrl.GetMousePositionOnImage().x, pThis->m_ImageCtrl.GetMousePositionOnImage().y, 150 );

			CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
			pGO.Reset();

			pGO.AddDrawCross( colorCross ); //화면에 선택된 좌표 표시

			pThis->m_pt2nd.x = pThis->m_ImageCtrl.GetMousePositionOnImage().x;
			pThis->m_pt2nd.y = pThis->m_ImageCtrl.GetMousePositionOnImage().y;

			CString strPosition;
			strPosition.Format( _T("X : %d, Y : %d"), pThis->m_pt1st.x, pThis->m_pt2nd.y);
			COLORTEXT colorText;
			colorText.CreateObject( PDC_BLUE, pThis->m_pt2nd.x+10, pThis->m_pt2nd.y-20, 12 );
			colorText.SetText( strPosition );
			pGO.AddDrawText( colorText );
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_35, pThis->m_pt2nd.x);
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_36, pThis->m_pt2nd.y);

			if(pThis->iSelect_B_Layer== 0)
			{
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X= pThis->m_pt2nd.x;
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y= pThis->m_pt2nd.y;
				pThis->m_dStX_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X;
				pThis->m_dStY_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y;
				pThis->SetDlgItemInt(IDC_EDIT_OFFSET_41, (UINT)(pThis->m_dStX_2));
				pThis->SetDlgItemInt(IDC_EDIT_OFFSET_42, (UINT)(pThis->m_dStY_2));
				return FALSE;
			}
			else
			{
				pThis->m_dCamPos_2nd_X= pThis->m_pt2nd.x;
				pThis->m_dCamPos_2nd_Y= pThis->m_pt2nd.y;

				if(pThis->m_StageNo== MANUAL_STAMP_PROC)
				{
					if(pThis->CameraStageUsedNo== eBTM_CAM)
					{
						pThis->Select_Pos_X2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
						pThis->Select_Pos_Y2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
					}
					else
					{
						pThis->Select_Pos_X2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
						pThis->Select_Pos_Y2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
					}
				}
				else
				{
					pThis->Select_Pos_X2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
					pThis->Select_Pos_Y2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
				}
			}
		}
	}
	return FALSE;
}

//===================================================================================
void CdlgManuCenter::ClickJogSelect3() //2x
{
	mTopCameraNo= e2X_LENS;
	DispUpdate();
	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_2X) );
	m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
  	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
}

//===================================================================================
void CdlgManuCenter::ClickJogSelect4() //20x
{ 
	mTopCameraNo= e20X_LENS;
	DispUpdate();
	m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_20X ) );
	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
 	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
}
//===================================================================================
void CdlgManuCenter::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
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
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -m_dPtpMove_mm, _SPD_MODE);		
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


void CdlgManuCenter::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
		else
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_X);
	}
}
//===================================================================================


void CdlgManuCenter::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, m_dPtpMove_mm, _SPD_MODE);		
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


void CdlgManuCenter::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
		else
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_X);
	}
}
//===================================================================================


void CdlgManuCenter::MouseDownJogCameraYCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_CAMERA_Y_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_UPPER_CAM_Y, _DIR_CCW, SetSpeed);
	}
}


void CdlgManuCenter::MouseUpJogCameraYCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Y);
	}
}
//===================================================================================


void CdlgManuCenter::MouseDownJogCameraYCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_CAMERA_Y_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Y];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_UPPER_CAM_Y, _DIR_CW, SetSpeed);
	}
}


void CdlgManuCenter::MouseUpJogCameraYCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Y);
	}
}
//===================================================================================


void CdlgManuCenter::MouseDownJogStageTCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 2.0) return;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
			double SetSpeed;
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Z];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Z];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Z];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_UPPER_CAM_Z, _DIR_CCW, SetSpeed);
	}
}


void CdlgManuCenter::MouseUpJogStageTCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Z);
	}
}
//===================================================================================


void CdlgManuCenter::MouseDownJogStageTCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 2.0) return;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
			double SetSpeed;
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_UPPER_CAM_Z];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_UPPER_CAM_Z];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_UPPER_CAM_Z];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_UPPER_CAM_Z, _DIR_CW, SetSpeed);
	}
}


void CdlgManuCenter::MouseUpJogStageTCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Z);
	}
}
//===================================================================================
BOOL CdlgManuCenter::MoveEndCheck(int Axis) 
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
//===================================================================================
void CdlgManuCenter::ClickStageJogSel1()
{
	iStageJog_Select= STAGE_SEL_1;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("상부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("상부 Vision\nStage +"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("상부 Vision\nStage Z-"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("상부 Vision\nStage Z+"));
}
//===================================================================================

void CdlgManuCenter::ClickStageJogSel2()
{
}

//===================================================================================
void CdlgManuCenter::DblClickJogSelect3()
{
		CString strText;
		double Cur_X, Cur_Y, Cur_Z, ToPosition1;
		// 	if(mTopCameraNo != e2X_LENS)
		// 	{
			strText.Format(_T("상부 2X 위치 만큼 모터를 이동 하시겠습니까?"));
			if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
			{
				Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
				Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
				Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
				ToPosition1= Cur_X - ParamFile()->System_data.dTopCam20To2_X;
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				ToPosition1= Cur_Y - ParamFile()->System_data.dTopCam20To2_Y;
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1= Cur_Z - ParamFile()->System_data.dTopCam20To2_Z;
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			}
}

//===================================================================================
void CdlgManuCenter::DblClickJogSelect4()
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
			ToPosition1= Cur_X + ParamFile()->System_data.dTopCam20To2_X;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			ToPosition1= Cur_Y + ParamFile()->System_data.dTopCam20To2_Y;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			ToPosition1= Cur_Z + ParamFile()->System_data.dTopCam20To2_Z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
}

//===================================================================================
void CdlgManuCenter::ClickJogSelect6()
{//1st 이동
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_SelectMarkNo= eALIGN_1ST_MARK;
	m_nClickAble= 0;
	m_btnSelectPoint.SetValue(0);
}

//===================================================================================
void CdlgManuCenter::ClickJogSelect7()
{//2nd 이동
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_SelectMarkNo= eALIGN_2ND_MARK;
	m_nClickAble= 0;
	m_btnSelectPoint.SetValue(0);
}

//===================================================================================
void CdlgManuCenter::ClickStageJogSel3()
{//마우스 선택
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_btnSelectPoint.GetValue())
	{
		if(m_SelectMarkNo== eALIGN_1ST_MARK)
			m_nClickAble= 1;
		else
			m_nClickAble= 2;
	}
	else
	{
		m_nClickAble= 0;
	}
}

//===================================================================================
void CdlgManuCenter::ClickStageJogSel4()
{//계산
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE);

	if(Cal_Align())
	{
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
		ParamFile()->SaveModelConfigFileSemi(ParamFile()->System_data.strCurrentModelName);
		AfxMessageBox(_T("완료되었습니다."));
	}
	else
		AfxMessageBox(_T("실패!"));

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
	pFrm->Data_Reload();

}
//===================================================================================
void CdlgManuCenter::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
}


void CdlgManuCenter::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
}


void CdlgManuCenter::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
}
//===================================================================================
BOOL CdlgManuCenter::Cal_Align()
{

	double cam1_Xmm= 0.0, cam1_Ymm= 0.0;
	double cam2_Xmm= 0.0, cam2_Ymm= 0.0;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

		if(!g_AutoRunAlignFlag) return FALSE;

	int i= 0;

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	double chaX, chaY;
	int stageNo= STAGE_1_ALIGN_PROC;

	for(i= 0;i< 3;i++)
	{
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;

		pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_2X);
		find_model_no= mdTOP_2X_STAEG1_START;
		if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
		{
			pSystem.GetVisionComponent()->InspGrabContinue();
			strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
		pSystem.GetVisionComponent()->InspGrabContinue();
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= (cam1_Ymm);

		chaX= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
		chaY= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;

		if(!g_AutoRunAlignFlag) return FALSE;
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, chaY, _SPD_MODE, TRUE)== FALSE)
//		if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;	
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}

	//20X--------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(!g_AutoRunAlignFlag) return FALSE;
	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	double Cur_X, Cur_Y, Cur_Z, ToPosition1;
	int align_count= 0;

	Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
	Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
	ToPosition1= ParamFile()->System_data.dTopCam20To2_X;
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	ToPosition1= ParamFile()->System_data.dTopCam20To2_Y;
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	ToPosition1= ParamFile()->System_data.dTopCam20To2_Z;
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE)		{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE)	{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	double AF_Z_Pos1;//, AF_Z_Pos2;
/*A/F*/
	if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos1))
	{
		//ParamFile()->Model_data.dPosition[move_pos_no+1][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos1;
	}

	for(i= 0;i< 3;i++)
	{
		pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_20X);
		find_model_no= mdTOP_20X_STAEG1_START;
		if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
		{
			pSystem.GetVisionComponent()->InspGrabContinue();
			strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
		pSystem.GetVisionComponent()->InspGrabContinue();
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= (cam1_Ymm);

		chaX= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
		chaY= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;

		if(!g_AutoRunAlignFlag) return FALSE;
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, chaY, _SPD_MODE, TRUE)== FALSE)
			//		if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;	
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}


	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_20X);
	find_model_no= mdTOP_20X_STAEG1_START;
	if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
	{
		pSystem.GetVisionComponent()->InspGrabContinue();
		strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;
	}
	pSystem.GetVisionComponent()->InspGrabContinue();
	pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
	pSystem.GetVisionComponent()->Trans_RealCoord(e20X_LENS, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= real_Y1;
	strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);

	//처음은 중심에
	ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
	ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);

	TRACE(strText.GetBuffer(0));
	//2nd
	if(stageNo== STAGE_1_ALIGN_PROC)	move_pos_no= POSITION_STAGE1_ALIGN_END;		else	move_pos_no= POSITION_STAGE2_ALIGN_END;
	if(stageNo== STAGE_1_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG1_END;
	else								find_model_no= mdTOP_20X_STAEG2_END;
	if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[find_model_no], _SPD_MODE, TRUE)== FALSE)
	{
		strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;
	}
	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

// /*A/F*/		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos2))
// 	{
// 		ParamFile()->Model_data.dPosition[move_pos_no+1][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos2;
// 	}

	pSystem.GetVisionComponent()->InspGrabContinueStop();
	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_20X);
	if(stageNo== STAGE_1_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG1_END;
	else								find_model_no= mdTOP_20X_STAEG2_END;
	if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx2, &posy2, find_model_no, eTOP_CAM_20X, true, true, true)  == false)
	{
		pSystem.GetVisionComponent()->InspGrabContinue();
		strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;
	}
	pSystem.GetVisionComponent()->InspGrabContinue();

	//두번째는 차이를 처음꺼에 더하기
	ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix= ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix+(posx2 - posx1);
	ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix= ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix+(posy2 - posy1);

	pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
	pSystem.GetVisionComponent()->Trans_RealCoord(e20X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= real_Y2;

	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= pSystem.GetVisionComponent()->GetAngle(
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x, ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y,
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
	double cal_x= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
	double cal_y= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);

	if(stageNo== STAGE_1_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG1_START;
	else								find_model_no= mdTOP_20X_STAEG2_START;
	if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[find_model_no], _SPD_MODE, TRUE)== FALSE)
	{
		strText.Format(_T("[ FAIL ] Stage #%d [MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;
	}
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, AF_Z_Pos1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);		pSystem.GetVisionComponent()->InspGrabContinue();
	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	ToPosition1= ParamFile()->System_data.dTopCam20To2_X;
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	ToPosition1= ParamFile()->System_data.dTopCam20To2_Y;
	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	Sleep(500);

	ParamFile()->System_data.dBasePos_Stage1_X= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_STAGE_X]= 
		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) + ParamFile()->System_data.dStage1ToStage2_Dist + ParamFile()->System_data.dStage2ToStage3_Dist;
	ParamFile()->System_data.dBasePos_Stage1_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

	ParamFile()->System_data.dBasePos_Stage2_X= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_STAGE_X]=
		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) + ParamFile()->System_data.dStage2ToStage3_Dist;
	ParamFile()->System_data.dBasePos_Stage2_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

	ParamFile()->System_data.dBasePos_Stage3_X= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_STAGE_X]=
		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	ParamFile()->System_data.dBasePos_Stage3_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);


	ParamFile()->SaveSytemConfigFile();
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	ParamFile()->SaveResultDataFile();
	return TRUE;
}
BOOL CdlgManuCenter::Cal_Alig_Manual()
{

	double cam1_Xmm= 0.0, cam1_Ymm= 0.0;
	double cam2_Xmm= 0.0, cam2_Ymm= 0.0;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	if(!g_AutoRunAlignFlag) return FALSE;

	int i= 0;

	double chaX, chaY;
	int stageNo= STAGE_1_ALIGN_PROC;
	int cur_camNo= mTopCameraNo;//= e2X_LENS;

	double Cur_X, Cur_Y, Cur_Z, ToPosition1;
	int align_count= 0;

	double dCur_Pos_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	double dCur_Pos_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

	m_nClickAble= 0;
	m_btnSelectPoint.SetValue(0);

	pSystem.GetVisionComponent()->InspGrabContinue();
	if(cur_camNo== e2X_LENS)
	{
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;

		if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0 )
		{
			posx1= m_dCamPos_1st_X; posy1= m_dCamPos_1st_Y;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= posx1;// + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0)) ;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= posy1;

			pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;

			posx2= m_dCamPos_2nd_X; posy2= m_dCamPos_2nd_Y;
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix= posx2;// + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0)) ;
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix= posy2;

			pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
			//			m_pVision->Trans_RealCoord(e2X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= cam2_Xmm;
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= cam2_Ymm;

			double cal_x= fabs(ParamFile()->Model_data.dStamp_Len_X - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x + ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
			double cal_y= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= pSystem.GetVisionComponent()->GetAngle(0, 0,	cal_x, 	cal_y);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);

			ParamFile()->System_data.dBasePos_Stage1_X= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_STAGE_X]= 
				pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) + ParamFile()->System_data.dStage1ToStage2_Dist + ParamFile()->System_data.dStage2ToStage3_Dist;
			ParamFile()->System_data.dBasePos_Stage1_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
				pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

			ParamFile()->System_data.dBasePos_Stage2_X= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_STAGE_X]=
				pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) + ParamFile()->System_data.dStage2ToStage3_Dist;
			ParamFile()->System_data.dBasePos_Stage2_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
				pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

			ParamFile()->System_data.dBasePos_Stage3_X= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_STAGE_X]=
				pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			ParamFile()->System_data.dBasePos_Stage3_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
				pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
		}
		else
		{
			pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;

			posx1= m_dCamPos_1st_X; posy1= m_dCamPos_1st_Y;
			pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= (cam1_Ymm);

			chaX= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
			chaY= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;

			pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
			chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
			chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);

			if(!g_AutoRunAlignFlag) return FALSE;
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, chaY, _SPD_MODE, TRUE)== FALSE)
				//		if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;	
			}
			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();


			Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
			ToPosition1= ParamFile()->System_data.dTopCam20To2_X;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			ToPosition1= ParamFile()->System_data.dTopCam20To2_Y;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			ToPosition1= ParamFile()->System_data.dTopCam20To2_Z;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE)		{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE)	{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }


			mTopCameraNo= e20X_LENS; m_radio2X.SetValue(0);	m_radio20X.SetValue(1);
			DispUpdate();
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_20X ) );
			m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
			m_ImageCtrl.ImageUpdate();
			pSystem.GetVisionComponent()->InspGrabContinue();

			AfxMessageBox(_T("10X 카메라로 마크 위치를 선택 하십시요."));
		}
		return TRUE;
	}
	else
	{
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;

// 		pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_20X);
// 		find_model_no= mdTOP_20X_STAEG1_START;
// 		if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
// 		{
// 			pSystem.GetVisionComponent()->InspGrabContinue();
// 			strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
// 			return FALSE;
// 		}
// 		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
// 		pSystem.GetVisionComponent()->InspGrabContinue();
		posx1= m_dCamPos_1st_X; posy1= m_dCamPos_1st_Y;
		pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= (cam1_Ymm);

		//처음은 중심에
		ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
		ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);
		chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
		chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;
		//////////////////////////////////////////////////////////////////////////


		if(!g_AutoRunAlignFlag) return FALSE;
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, chaY, _SPD_MODE, TRUE)== FALSE)
			//		if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;	
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------
		posx1= m_dCamPos_1st_X; posy1= m_dCamPos_1st_Y;

		pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
		pSystem.GetVisionComponent()->Trans_RealCoord(e20X_LENS, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= real_Y1;
		strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
		TRACE(strText.GetBuffer(0));
		//2nd
		posx2= m_dCamPos_2nd_X; posy2= m_dCamPos_2nd_Y;
		//두번째는 차이를 처음꺼에 더하기
		ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix= ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix+(m_dCamPos_2nd_X - m_dCamPos_1st_X);
		ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix= ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix+(m_dCamPos_2nd_Y - m_dCamPos_1st_Y);
		//////////////////////////////////////////////////////////////////////////
		if(stageNo== STAGE_1_ALIGN_PROC)	move_pos_no= POSITION_STAGE1_ALIGN_END;		else	move_pos_no= POSITION_STAGE2_ALIGN_END;
		if(stageNo== STAGE_1_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG1_END;
		else								find_model_no= mdTOP_20X_STAEG2_END;
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[find_model_no], _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		pSystem.GetVisionComponent()->GetCam_ROICood(eBTM_CAM, e20X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
		pSystem.GetVisionComponent()->Trans_RealCoord(e20X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= real_Y2;

		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= pSystem.GetVisionComponent()->GetAngle(
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x, ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y,
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
		double cal_x= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
		double cal_y= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
		 //- 원위치로
		if(stageNo== STAGE_1_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG1_START;
		else								find_model_no= mdTOP_20X_STAEG2_START;
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[find_model_no], _SPD_MODE, TRUE)== FALSE)
		{
			strText.Format(_T("[ FAIL ] Stage #%d [MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);		pSystem.GetVisionComponent()->InspGrabContinue();
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//---

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1= ParamFile()->System_data.dTopCam20To2_X;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		ToPosition1= ParamFile()->System_data.dTopCam20To2_Y;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		Sleep(500);

		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);		pSystem.GetVisionComponent()->InspGrabContinue();

		ParamFile()->System_data.dBasePos_Stage1_X= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_STAGE_X]= 
			pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) + ParamFile()->System_data.dStage1ToStage2_Dist + ParamFile()->System_data.dStage2ToStage3_Dist;
		ParamFile()->System_data.dBasePos_Stage1_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
			pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

		ParamFile()->System_data.dBasePos_Stage2_X= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_STAGE_X]=
			pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) + ParamFile()->System_data.dStage2ToStage3_Dist;
		ParamFile()->System_data.dBasePos_Stage2_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
			pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

		ParamFile()->System_data.dBasePos_Stage3_X= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_STAGE_X]=
			pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		ParamFile()->System_data.dBasePos_Stage3_CamY= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y]= 
			pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

		ParamFile()->SaveSytemConfigFile();
		ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

		ParamFile()->SaveResultDataFile();

		mTopCameraNo= e2X_LENS; m_radio2X.SetValue(1);	m_radio20X.SetValue(0);
		DispUpdate();
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
		m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_2X) );
		m_ImageCtrl.ImageUpdate();
		pSystem.GetVisionComponent()->InspGrabContinue();
	}
	//20X--------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
	double AF_Z_Pos1;//, AF_Z_Pos2;
	/ *A/F* /		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos1))
	{
		//ParamFile()->Model_data.dPosition[move_pos_no+1][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos1;
	}
*/

	return TRUE;
}
//===================================================================================
BOOL CdlgManuCenter::MoveEndCheckNonDisp(int Axis) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
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
	}
	return FALSE;
}

//===================================================================================

void CdlgManuCenter::ClickJogSelect8()
{
	iSelMarkPos= 1;
	GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE); GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE);		
	pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dStamp_Len_X/*ParamFile()->Model_data.dAlignMarkToMark[mdTOP_2X_STAEG1_START]*/, _SPD_MODE, FALSE);
}

//===================================================================================
void CdlgManuCenter::ClickJogSelect9()
{
	iSelMarkPos= 0;
	GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE);		
	pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dStamp_Len_X/*ParamFile()->Model_data.dAlignMarkToMark[mdTOP_2X_STAEG1_START]*/, _SPD_MODE, FALSE);
}

//===================================================================================
void CdlgManuCenter::ClickCmMove()
{
}

//===================================================================================
void CdlgManuCenter::ClickCmMove6()
{
}



void CdlgManuCenter::ClickJogSelect10()
{
	iSelect_B_Layer= 0;
	SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
	m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
	m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
}


void CdlgManuCenter::ClickJogSelect11()
{
	iSelect_B_Layer= 1;
	SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
	m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
	m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
}


void CdlgManuCenter::ClickCmMove7()
{
	GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_HIDE);

}


void CdlgManuCenter::ClickJogSelect12()
{
	iSelect_B_Layer= 2;
	SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
	m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
	m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
}


void CdlgManuCenter::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CdlgManuCenter::ClickCmMove8()
{
}


void CdlgManuCenter::DblClickCmMarkLoad()
{
/*
	if(mTopCameraNo== e2X_LENS) 
	{
		pSystem.GetMotionComponent()->AF_Proc_Start(eTOP_CAM_2X);
		MoveEndCheck(_ACS_AXIS_UPPER_CAM_Z) ;
	}
	else
	{
		pSystem.GetMotionComponent()->AF_Proc_Start(eTOP_CAM_20X);
		MoveEndCheck(_ACS_AXIS_UPPER_CAM_Z) ;
	}
	int ret= pSystem.GetVisionComponent()->AutoFocus();
	pSystem.GetVisionComponent()->AF_BufferSave(36);
	if(ret> 0 && ret <= ParamFile()->System_data.iAF_TotalCount)
	{
		double pos= pSystem.GetVisionComponent()->Z_AF_pos[ret];
		if(pos> -1.5 && pos <= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z))
			pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, pos);
	}
	else 
		AfxMessageBox(_T("A/F Error!!!"));
*/
}


void CdlgManuCenter::ClickCmMove9()
{
	double toTopmask_x;
	double toBotmask_x; 
	double CurtoBotmask_x; 
	double maskTomask= 0.0;
	double maskTomask_BX= 0.0;
	double maskTomask_BY= 0.0;
	CString strText;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	toTopmask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	toBotmask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	maskTomask_BX= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X] - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	maskTomask_BY= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y] - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
	
	maskTomask= toBotmask_x - toTopmask_x;
	CurtoBotmask_x= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X] - toBotmask_x;
	CurtoBotmask_x= CurtoBotmask_x +maskTomask_BX + toTopmask_x;

	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_ALIGN_Y1];
	ToPosition2	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_ALIGN_Y2];
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
	pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, CurtoBotmask_x, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, TopCamY, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
}


void CdlgManuCenter::ClickStageJogSel6()
{
	UpdateData(TRUE);
	if(m_dCamPos_1st_X<= 0.0 || m_dCamPos_1st_Y<= 0.0 || m_dCamPos_2nd_X<= 0.0 || m_dCamPos_2nd_Y<= 0.0)
	{
		AfxMessageBox(_T("[ 동작 취소 ]\n좌표 선택이 안되어 있습니다."));
	}
	else
	{
		if(iSelMarkPos== 1)
		{
			iSelMarkPos= 0;
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[mdTOP_2X_STAEG1_START], _SPD_MODE, TRUE);
		}
		GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE);

		if(Cal_Alig_Manual())
		{
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
			ParamFile()->SaveModelConfigFileSemi(ParamFile()->System_data.strCurrentModelName);
			if(mTopCameraNo== e2X_LENS)
			{
				CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
				pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
				pSystem.GetVisionComponent()->InspGrabContinue();
				pFrm->Data_Reload();
				AfxMessageBox(_T("완료되었습니다."));
				double chax= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-dCur_Pos_X;
				double chaY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-dCur_Pos_Y;
// 				ParamFile()->Model_data.dStampToBaseAlignDist_X=ParamFile()->Model_data.dStampToBaseAlignDist_X+chax;
// 				ParamFile()->Model_data.dStampToBaseAlignDist_Y=ParamFile()->Model_data.dStampToBaseAlignDist_Y+chaY;
			}
			return;
		}
		else
			AfxMessageBox(_T("실패!"));

		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		pSystem.GetVisionComponent()->InspGrabContinue();
		pFrm->Data_Reload();
	}
}


void CdlgManuCenter::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CdlgManuCenter::OnBnClickedButton6()
{
	double ToX, ToY1, ToY2, ToCy, ToCz, ToBx, ToBy, ToBz, ToPosition1, ToPosition2;
	CString strText;

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToX= 230.7553;
	ToY1= -3.6641;
	ToY2= 0.105;
	ToCy= 67.906;
	ToCz= 14.0586;
	ToBx= 0.0;
	ToBy= 4.8986;
	ToBz= 0.0;

	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToY1, ToY2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToX,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToCy,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToBx,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToBy,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}
