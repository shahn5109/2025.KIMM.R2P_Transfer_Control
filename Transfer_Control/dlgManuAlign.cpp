// dlgManuAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgManuAlign.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "dlgLightControl.h"

extern CSystemClass pSystem;
CParameterFile* CdlgManuAlign::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgManuAlign::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgManuAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgManuAlign, CDialogEx)

CdlgManuAlign::CdlgManuAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgManuAlign::IDD, pParent)
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
	, m_bOverlay_Disable(FALSE)
	, m_bRollZ_Enable(FALSE)
	, m_dCur_Mark_Rect(0)
	, m_iPixSel(0)
	, m_dPitchSpeed(0)
	, m_bPitchSpeedSel(FALSE)
{
	iSelect_B_Layer= 2;
}

CdlgManuAlign::~CdlgManuAlign()
{
}

void CdlgManuAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_COMMAND5_X3, m_btnTitle);
	DDX_Control(pDX, IDB_JOG_SELECT_3, m_radio2X);
	DDX_Control(pDX, IDB_JOG_SELECT_4, m_radio20X);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL1, m_btnStageSel1);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL3, m_btnSelectPoint);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
	DDX_Control(pDX, IDB_JOG_SELECT_12, m_btnAlignModeSel);
	DDX_Control(pDX, IDB_JOG_SELECT_10, m_btnAlignModeSel1);


	/*
	DDX_Text(pDX, IDC_EDIT_OFFSET_33, m_dCamPos_1st_X);
	DDX_Text(pDX, IDC_EDIT_OFFSET_34, m_dCamPos_1st_Y);
	DDX_Text(pDX, IDC_EDIT_OFFSET_35, m_dCamPos_2nd_X);
	DDX_Text(pDX, IDC_EDIT_OFFSET_36, m_dCamPos_2nd_Y);
	*/
	DDX_Text(pDX, IDC_EDIT_OFFSET_37, m_dResult_Len);
	DDX_Text(pDX, IDC_EDIT_OFFSET_38, m_dResult_Deg);
	DDX_Radio(pDX, IDC_RADIO1, m_iAlignExc);
	DDX_Text(pDX, IDC_EDIT_OFFSET_39, m_dStX_1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_40, m_dStY_1);
	DDX_Text(pDX, IDC_EDIT_OFFSET_41, m_dStX_2);
	DDX_Text(pDX, IDC_EDIT_OFFSET_42, m_dStY_2);
	DDX_Check(pDX, IDC_CHECK1, m_bOverlay_Disable);
	DDX_Check(pDX, IDC_CHECK3, m_bRollZ_Enable);
	DDX_Text(pDX, IDC_EDIT_OFFSET_43, m_dCur_Mark_Rect);
	DDX_Radio(pDX, IDC_RADIO3, m_iPixSel);
	DDX_Text(pDX, IDC_EDIT2, m_dPitchSpeed);
	DDX_Check(pDX, IDC_CHECK4, m_bPitchSpeedSel);
}


BEGIN_MESSAGE_MAP(CdlgManuAlign, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgManuAlign::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CdlgManuAlign::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_CHECK1, &CdlgManuAlign::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &CdlgManuAlign::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON6, &CdlgManuAlign::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CdlgManuAlign::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_T, &CdlgManuAlign::OnBnClickedButtonT)
	ON_BN_CLICKED(IDC_BUTTON_B, &CdlgManuAlign::OnBnClickedButtonB)
	ON_BN_CLICKED(IDC_BUTTON_L, &CdlgManuAlign::OnBnClickedButtonL)
	ON_BN_CLICKED(IDC_BUTTON_R, &CdlgManuAlign::OnBnClickedButtonR)
END_MESSAGE_MAP()


// CdlgManuAlign 메시지 처리기입니다.


//===================================================================================
int CdlgManuAlign::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

//===================================================================================
BOOL CdlgManuAlign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strText1;

	GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
	GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_HIDE);		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_HIDE);  GetDlgItem(IDC_CHECK3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->ShowWindow(SW_HIDE);		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->ShowWindow(SW_HIDE);
	m_ImageCtrl.Create( &m_ImageMng, this );

	switch(m_StageNo)
	{
		case ROLL_ALIGN_PROC:
			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE9)->EnableWindow(FALSE);
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			break;
		case STAGE_1_ALIGN_PROC:
		case STAGE_2_ALIGN_PROC:
		case STAGE_3_ALIGN_PROC:
			GetDlgItem(IDB_CM_MOVE9)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDB_STAGE_JOG_SEL4)->EnableWindow(FALSE);
			if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE || ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_MANU_MODE)
			{
				m_btnAlignModeSel.SetCaption(_T("Align 위치 선택\n[ Roll-기준점 ]"));
			}
			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			CameraStageUsedNo= eTOP_CAM;
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			break;
		case MANUAL_STAMP_PROC://m_StageNo
			GetDlgItem(IDB_CM_MOVE9)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE)->EnableWindow(FALSE);
			GetDlgItem(IDB_STAGE_JOG_SEL4)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE);
			//			GetDlgItem(IDB_STAGE_JOG_SEL4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDB_JOG_STAGE_T_CW2)->ShowWindow(SW_SHOW);		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->ShowWindow(SW_SHOW);


			GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE8)->EnableWindow(FALSE);
			GetDlgItem(IDB_JOG_SELECT_10)->EnableWindow(FALSE);
			GetDlgItem(IDB_JOG_SELECT_11)->EnableWindow(FALSE);
			SetDlgItemText(IDB_JOG_SELECT_10, L"---");
			SetDlgItemText(IDB_JOG_SELECT_11, L"---");
			SetDlgItemText(IDB_JOG_SELECT_12, L"중심점으로 Align");
			SetDlgItemText(IDB_CM_MOVE6, L"Align 위치로 이동 [2X]");
			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);


			if(CameraStageUsedNo== eTOP_CAM)
			{
				GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_SHOW);
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				break;
			}
			else
			{
				GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_SHOW);		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_SHOW); GetDlgItem(IDC_CHECK3)->ShowWindow(SW_SHOW);
				GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				break;
			}
			break;
		case MANUAL_ROLL_ALIGN_PROC:
			GetDlgItem(IDB_CM_MOVE9)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE)->EnableWindow(FALSE);
			GetDlgItem(IDB_STAGE_JOG_SEL4)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE);
//			GetDlgItem(IDB_STAGE_JOG_SEL4)->ShowWindow(SW_HIDE);

			if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
			{
				GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
//				GetDlgItem(IDB_CM_MOVE8)->EnableWindow(FALSE);
				GetDlgItem(IDB_JOG_SELECT_10)->EnableWindow(FALSE);
				GetDlgItem(IDB_JOG_SELECT_11)->EnableWindow(FALSE);
				SetDlgItemText(IDB_JOG_SELECT_10, L"---");
				SetDlgItemText(IDB_JOG_SELECT_11, L"---");
				SetDlgItemText(IDB_JOG_SELECT_12, L"Align용 기준 위치");
				SetDlgItemText(IDB_CM_MOVE6, L"Roll-Align 위치로 이동 [2X]");
			}
			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			
	
			if(CameraStageUsedNo== eTOP_CAM)
			{
				GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_SHOW);
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				break;
			}
			else
			{
				GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_SHOW);		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_SHOW); GetDlgItem(IDC_CHECK3)->ShowWindow(SW_SHOW);
				GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				break;
			}
			break;
		case MANUAL_QT_ROLL_ALIGN_PROC:
			GetDlgItem(IDB_CM_MOVE9)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE)->EnableWindow(FALSE);
			GetDlgItem(IDB_STAGE_JOG_SEL4)->EnableWindow(FALSE);
			GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_HIDE);
			GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE);
			//			GetDlgItem(IDB_STAGE_JOG_SEL4)->ShowWindow(SW_HIDE);

			if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
			{
				GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
				//				GetDlgItem(IDB_CM_MOVE8)->EnableWindow(FALSE);
				GetDlgItem(IDB_JOG_SELECT_10)->EnableWindow(FALSE);
				GetDlgItem(IDB_JOG_SELECT_11)->EnableWindow(FALSE);
				SetDlgItemText(IDB_JOG_SELECT_10, L"---");
				SetDlgItemText(IDB_JOG_SELECT_11, L"---");
				SetDlgItemText(IDB_JOG_SELECT_12, L"Align용 기준 위치");
				SetDlgItemText(IDB_CM_MOVE6, L"Roll-Align 위치로 이동 [2X]");
			}
			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);


			if(CameraStageUsedNo== eTOP_CAM)
			{
				GetDlgItem(IDB_CM_MOVE9)->ShowWindow(SW_SHOW);
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				break;
			}
			else
			{
				GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_SHOW);		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_SHOW); GetDlgItem(IDC_CHECK3)->ShowWindow(SW_SHOW);
				GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				break;
			}
			break;
		default:
			break;
	}

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
	if(CameraStageUsedNo== eTOP_CAM)
	{
		GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE);
		SetDlgItemText(IDB_JOG_SELECT_9, _T("1st Mark\nPitch 이동"));
		SetDlgItemText(IDB_JOG_SELECT_8, _T("2nd Mark\nPitch 이동"));
	}
	else
	{
		GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);
		SetDlgItemText(IDB_JOG_SELECT_8, _T("1st Mark\nPitch 이동"));
		SetDlgItemText(IDB_JOG_SELECT_9, _T("2nd Mark\nPitch 이동"));
	}
	Align_Pos_Set_Flag= false;

	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_2X) );

	iSelMarkPos= 0;

	if(m_StageNo== MANUAL_QT_ROLL_ALIGN_PROC)
	{
		m_btnAlignModeSel.SetValue(0);	m_btnAlignModeSel1.SetValue(1);
		iSelect_B_Layer= SEL_STAMP_MODE;
		SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
		SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
		SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
		SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
		m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
		m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
	}
	else
	{
		m_btnAlignModeSel.SetValue(1);	m_btnAlignModeSel1.SetValue(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
//===================================================================================
void CdlgManuAlign::DispUpdate()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CString strText;
	switch(m_StageNo)
	{
	case ROLL_ALIGN_PROC:
		GetDlgItem(IDB_JOG_SELECT_10)->ShowWindow(SW_HIDE);	GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_CM_MOVE6)->ShowWindow(SW_HIDE);		GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_HIDE); GetDlgItem(IDB_CM_MOVE8)->ShowWindow(SW_HIDE);
		if(mTopCameraNo== e2X_LENS) 
		{
			m_btnTitle.SetCaption(_T("ROLL Align-Mark 설정\n하부 카메라 [Lens:2X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		else			
		{
			m_btnTitle.SetCaption(_T("ROLL Align-Mark 설정\n하부 카메라 [Lens:10X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		iStageJog_Select= STAGE_SEL_2;
		GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
		SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
		SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
		SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
		SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));

		strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[m_StageNo]);
		SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));

		break;
	case STAGE_1_ALIGN_PROC:
		GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE); GetDlgItem(IDB_JOG_SELECT_10)->ShowWindow(SW_HIDE);	GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);
		if(mTopCameraNo== e2X_LENS) 
		{
			m_btnTitle.SetCaption(_T("Stage #1 Align 설정\n상부 카메라 [Lens:2X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		else			
		{
			m_btnTitle.SetCaption(_T("Stage #1 Align 설정\n상부 카메라 [Lens:10X]"));
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
		strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[m_StageNo]);
		SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));
		break;
	case STAGE_2_ALIGN_PROC:
		GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE); GetDlgItem(IDB_JOG_SELECT_10)->ShowWindow(SW_HIDE);	GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);
		if(mTopCameraNo== e2X_LENS) 
		{
			m_btnTitle.SetCaption(_T("Stage #2 Align 설정\n상부 카메라 [Lens:2X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		else			
		{
			m_btnTitle.SetCaption(_T("Stage #2 Align 설정\n상부 카메라 [Lens:10X]"));
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
		strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[m_StageNo]);
		SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));
		break;
	case STAGE_3_ALIGN_PROC:
		GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE); GetDlgItem(IDB_JOG_SELECT_10)->ShowWindow(SW_HIDE);	GetDlgItem(IDB_JOG_SELECT_11)->ShowWindow(SW_HIDE);
		if(mTopCameraNo== e2X_LENS) 
		{
			m_btnTitle.SetCaption(_T("Stage #3 Align 설정\n상부 카메라 [Lens:2X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		else			
		{
			m_btnTitle.SetCaption(_T("Stage #3 Align 설정\n상부 카메라 [Lens:10X]"));
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
		strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[m_StageNo]);
		SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));
		break;
	case MANUAL_STAMP_PROC:
		GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE);
		m_dStX_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X;
		m_dStY_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y;
		m_dStX_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X;
		m_dStY_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y;

		if(CameraStageUsedNo== eTOP_CAM)
		{
			if(mTopCameraNo== e2X_LENS) 
			{
				m_btnTitle.SetCaption(_T("스템프 Align-Mark 설정\n상부 카메라 [Lens:2X]"));
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
			}
			else			
			{
				m_btnTitle.SetCaption(_T("스템프 Align-Mark 설정\n상부 카메라 [Lens:10X]"));
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
			SetDlgItemText(IDB_JOG_CAMERA_Y_CCW, _T("Stage Y+"));
			SetDlgItemText(IDB_JOG_CAMERA_Y_CW, _T("Stage Y-"));
			strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC]);
			SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));
		}
		else
		{
			if(mTopCameraNo== e2X_LENS) 
			{
				m_btnTitle.SetCaption(_T("스템프 Align-Mark 설정\n하부 카메라 [Lens:2X]"));
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
			}
			else			
			{
				m_btnTitle.SetCaption(_T("ROLL Align-Mark 설정\n하부 카메라 [Lens:10X]"));
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
			}
			iStageJog_Select= STAGE_SEL_2;
			GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
			SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
			SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
			SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));

			strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC]);
			SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));
		}
		break;
	case MANUAL_ROLL_ALIGN_PROC:
		GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE);
		m_dStX_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X;
		m_dStY_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y;
		m_dStX_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X;
		m_dStY_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y;

			if(mTopCameraNo== e2X_LENS) 
			{
				m_btnTitle.SetCaption(_T("Align-Mark 설정\n하부 카메라 [Lens:2X]"));
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
			}
			else			
			{
				m_btnTitle.SetCaption(_T("Align-Mark 설정\n하부 카메라 [Lens:10X]"));
				m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
				m_ImageCtrl.ImageUpdate();
				pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
			}
			iStageJog_Select= STAGE_SEL_2;
			GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
			SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
			SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
			SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));

			strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC]);
			SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));
		break;
	case MANUAL_QT_ROLL_ALIGN_PROC:
		GetDlgItem(IDB_CM_MOVE)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDB_JOG_SELECT_12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_CM_MOVE8)->ShowWindow(SW_HIDE);

		SetDlgItemText(IDB_JOG_SELECT_10, _T("ROLL-Mark 위치 선택\n[기준점 설정]"));
		SetDlgItemText(IDB_JOG_SELECT_11, _T("PCB-Mark 선택\n[PCB Align]"));
	
		m_dStX_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X;
		m_dStY_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y;
		m_dStX_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X;
		m_dStY_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y;

		if(mTopCameraNo== e2X_LENS) 
		{
			m_btnTitle.SetCaption(_T("Align-Mark 설정\n하부 카메라 [Lens:2X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		else			
		{
			m_btnTitle.SetCaption(_T("Align-Mark 설정\n하부 카메라 [Lens:10X]"));
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
			m_ImageCtrl.ImageUpdate();
			pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
			pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		}
		iStageJog_Select= STAGE_SEL_2;
		GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
		SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
		SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
		SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
		SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));

		strText.Format(_T("%.3f"), ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC]);
		SetDlgItemText(IDC_STATIC_MARKGAP, strText.GetBuffer(0));

		SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
		SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
		SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
		break;
	default:
		m_btnTitle.SetCaption(_T("???"));
		break;
	}
//	if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
//	{
// 		GetDlgItem(IDC_BUTTON_T)->EnableWindow(FALSE);	GetDlgItem(IDC_BUTTON_B)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BUTTON_L)->EnableWindow(FALSE);	GetDlgItem(IDC_BUTTON_R)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);	GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);	GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_BUTTON_T)->EnableWindow(TRUE);	GetDlgItem(IDC_BUTTON_B)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BUTTON_L)->EnableWindow(TRUE);	GetDlgItem(IDC_BUTTON_R)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);		GetDlgItem(IDC_RADIO4)->EnableWindow(TRUE);	GetDlgItem(IDC_RADIO5)->EnableWindow(TRUE);
// 	}
	UpdateData(FALSE);
}
//===================================================================================
void CdlgManuAlign::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(dlgManuAlign_TIMER_ID);
	pSystem.GetVisionComponent()->SetTempImageCtrl( NULL );

}

//===================================================================================
BOOL CdlgManuAlign::DestroyWindow()
{
	KillTimer(dlgManuAlign_TIMER_ID);
	pSystem.GetVisionComponent()->SetTempImageCtrl( NULL );

	return CDialogEx::DestroyWindow();
}

//===================================================================================
//===================================================================================
void CdlgManuAlign::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
BOOL CdlgManuAlign::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){ 
		if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)){          return TRUE;        }   
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//===================================================================================
BEGIN_EVENTSINK_MAP(CdlgManuAlign, CDialogEx)
	ON_EVENT(CdlgManuAlign, IDB_EXIT, DISPID_CLICK, CdlgManuAlign::ClickExit, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_3, DISPID_CLICK, CdlgManuAlign::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_4, DISPID_CLICK, CdlgManuAlign::ClickJogSelect4, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_STAGE_JOG_SEL1, DISPID_CLICK, CdlgManuAlign::ClickStageJogSel1, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_STAGE_JOG_SEL2, DISPID_CLICK, CdlgManuAlign::ClickStageJogSel2, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_3, DISPID_DBLCLICK, CdlgManuAlign::DblClickJogSelect3, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_4, DISPID_DBLCLICK, CdlgManuAlign::DblClickJogSelect4, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_6, DISPID_CLICK, CdlgManuAlign::ClickJogSelect6, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_7, DISPID_CLICK, CdlgManuAlign::ClickJogSelect7, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_STAGE_JOG_SEL3, DISPID_CLICK, CdlgManuAlign::ClickStageJogSel3, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_STAGE_JOG_SEL4, DISPID_CLICK, CdlgManuAlign::ClickStageJogSel4, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_EXIT2, DISPID_CLICK, CdlgManuAlign::ClickExit2, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_0, DISPID_CLICK, CdlgManuAlign::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_1, DISPID_CLICK, CdlgManuAlign::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_2, DISPID_CLICK, CdlgManuAlign::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_8, DISPID_CLICK, CdlgManuAlign::ClickJogSelect8, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_9, DISPID_CLICK, CdlgManuAlign::ClickJogSelect9, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_CM_MOVE, DISPID_CLICK, CdlgManuAlign::ClickCmMove, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_CM_MOVE6, DISPID_CLICK, CdlgManuAlign::ClickCmMove6, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_10, DISPID_CLICK, CdlgManuAlign::ClickJogSelect10, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_11, DISPID_CLICK, CdlgManuAlign::ClickJogSelect11, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_CM_MOVE7, DISPID_CLICK, CdlgManuAlign::ClickCmMove7, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_SELECT_12, DISPID_CLICK, CdlgManuAlign::ClickJogSelect12, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_CM_MOVE8, DISPID_CLICK, CdlgManuAlign::ClickCmMove8, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_CM_MOVE9, DISPID_CLICK, CdlgManuAlign::ClickCmMove9, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_Z_CW, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_CM_MARK_LOAD, DISPID_DBLCLICK, CdlgManuAlign::DblClickCmMarkLoad, VTS_NONE)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEDOWN, CdlgManuAlign::MouseDownJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEUP, CdlgManuAlign::MouseUpJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CdlgManuAlign, IDB_JOG_CAMERA_Y_CCW3, DISPID_CLICK, CdlgManuAlign::ClickJogCameraYCcw3, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CdlgManuAlign::ClickExit()
{
			double Cur_X, Cur_Y, Cur_Z, ToPosition1, ToPosition2;
			CString strText;
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
//			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
	{
// 		double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
// 		pSystem.GetMotionComponent()->GetStampPos(eBTM_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	}
	else
	{
		if(iStamInstRunFlag)
		{
			if(mTopCameraNo== e20X_LENS)
			{
				if(CameraStageUsedNo== eBTM_CAM)
				{
					ToPosition1= -ParamFile()->System_data.dBotCam20To2_X;
					if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					ToPosition1= -ParamFile()->System_data.dBotCam20To2_Y;
					if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				}
				else
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
		}

	}
	OnOK();
}
void CdlgManuAlign::ClickExit2()
{
	OnCancel();
}

//===================================================================================
void CdlgManuAlign::OnTimer(UINT_PTR nIDEvent)
{

	PostMessage(WM_CLOSE);

	CDialogEx::OnTimer(nIDEvent);
}

//===================================================================================
void APIENTRY CdlgManuAlign::OnDrawExt( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData ) 
{
	CdlgManuAlign* pThis = (CdlgManuAlign*)lpUsrData;
	CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
	CImageObj* pImgObj = pThis->m_ImageCtrl.GetVisibleImageObject(); 

	if ( !pThis->m_bOverlay_Disable )
	{
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

		double cnv_x, cnv_y;
		pDC->SelectObject( &DotPen1 );
		if(pThis->mTopCameraNo== e20X_LENS)
		{
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
				if(pThis->mTopCameraNo== e20X_LENS)
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
				if(pThis->mTopCameraNo== e20X_LENS)
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
		if(pThis->CameraStageUsedNo== eBTM_CAM)
		{
			if(pThis->m_dCamPos_1st_X> 0 && pThis->m_dCamPos_1st_X< _INSP_IMAGE_SIZE_X && pThis->m_dCamPos_1st_Y> 0 && pThis->m_dCamPos_1st_Y< _INSP_IMAGE_SIZE_Y)
			{
				ptCenter = pIDC->ImagePosToScreenPos(	(int)(pThis->m_dCamPos_1st_X), (int)(pThis->m_dCamPos_1st_Y) );
				CPen* pOldPen1 = pDC->SelectObject( &SoldPenB );
				pDC->MoveTo( ptCenter.x-25, ptCenter.y );		pDC->LineTo( ptCenter.x+25, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-25 );			pDC->LineTo( ptCenter.x, ptCenter.y+25 );
			}
			if(pThis->m_dCamPos_2nd_X> 0 && pThis->m_dCamPos_2nd_X< _INSP_IMAGE_SIZE_X && pThis->m_dCamPos_2nd_Y> 0 && pThis->m_dCamPos_2nd_Y< _INSP_IMAGE_SIZE_Y)
			{
				ptCenter = pIDC->ImagePosToScreenPos(	(int)(pThis->m_dCamPos_2nd_X), (int)(pThis->m_dCamPos_2nd_Y) );
				CPen* pOldPen1 = pDC->SelectObject( &SoldPenG );
				pDC->MoveTo( ptCenter.x-25, ptCenter.y );		pDC->LineTo( ptCenter.x+25, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-25 );			pDC->LineTo( ptCenter.x, ptCenter.y+25 );
			}
		}
		else
		{
			if(pThis->m_dCamPos_1st_X> 0 && pThis->m_dCamPos_1st_X< _INSP_IMAGE_SIZE_X && pThis->m_dCamPos_1st_Y> 0 && pThis->m_dCamPos_1st_Y< _INSP_IMAGE_SIZE_Y)
			{
				ptCenter = pIDC->ImagePosToScreenPos(	(int)(pThis->m_dCamPos_1st_X), (int)(pThis->m_dCamPos_1st_Y) );
				CPen* pOldPen1 = pDC->SelectObject( &SoldPenB );
				pDC->MoveTo( ptCenter.x-25, ptCenter.y );		pDC->LineTo( ptCenter.x+25, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-25 );			pDC->LineTo( ptCenter.x, ptCenter.y+25 );
			}
			if(pThis->m_dCamPos_2nd_X> 0 && pThis->m_dCamPos_2nd_X< _INSP_IMAGE_SIZE_X && pThis->m_dCamPos_2nd_Y> 0 && pThis->m_dCamPos_2nd_Y< _INSP_IMAGE_SIZE_Y)
			{
				ptCenter = pIDC->ImagePosToScreenPos(	(int)(pThis->m_dCamPos_2nd_X), (int)(pThis->m_dCamPos_2nd_Y) );
				CPen* pOldPen1 = pDC->SelectObject( &SoldPenG );
				pDC->MoveTo( ptCenter.x-25, ptCenter.y );		pDC->LineTo( ptCenter.x+25, ptCenter.y );
				pDC->MoveTo( ptCenter.x, ptCenter.y-25 );			pDC->LineTo( ptCenter.x, ptCenter.y+25 );
			}
		}

		pDC->SelectObject( pOldPen );		pDC->SetBkMode( nOldBkMode );
	}
}
//===================================================================================
BOOL APIENTRY CdlgManuAlign::OnFireMouseEvent(  DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData )
{
	CdlgManuAlign* pThis = (CdlgManuAlign*)lpUsrData;

	if( dwMsg == WM_MOUSEMOVE )
	{
		if( (pThis->m_nClickAble == 1) )
		{
			int aaa= pThis->GetDlgItemInt(IDC_EDIT_OFFSET_43);
			if(aaa< 5) aaa= 5;
			COLORCROSS colorCross;
			COLORALIGNMARK clrMark;
			colorCross.CreateObject( PDC_BLUE, pThis->m_ImageCtrl.GetMousePositionOnImage().x, pThis->m_ImageCtrl.GetMousePositionOnImage().y, 50,10 );
			CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
			pGO.Reset();

			pGO.AddDrawCross( colorCross ); //화면에 선택된 좌표 표시

			double dfact;
			int isumRect;
			if(pThis->mTopCameraNo== e2X_LENS)	dfact= pThis->ParamFile()->System_data.dTop_Cam_PixelToum_2X;
			else								dfact= pThis->ParamFile()->System_data.dTop_Cam_PixelToum_20X;

			pThis->m_pt1st.x = pThis->m_ImageCtrl.GetMousePositionOnImage().x;
			pThis->m_pt1st.y = pThis->m_ImageCtrl.GetMousePositionOnImage().y;
			isumRect= (int)((/*pThis->m_dCur_Mark_Rect*/aaa/*/1000.0*/) / dfact);

			//CString strPosition; 
			//strPosition.Format( _T("X : %d, Y : %d"), pThis->m_pt1st.x, pThis->m_pt1st.y);
			//COLORTEXT colorText;
			//colorText.CreateObject( PDC_BLUE, pThis->m_pt1st.x+10, pThis->m_pt1st.y-20, 12 );
			//colorText.SetText( strPosition );
			//pGO.AddDrawText( colorText );
			//m_dCur_Mark_Rect
			
			clrMark.CreateObject( PDC_BLUE, pThis->m_pt1st.x, pThis->m_pt1st.y, 300, isumRect/2, PS_SOLID, 1);
			pGO.AddDrawAlignMark( clrMark ); 

 			//TRACE(_T("%d/%d [%d:%d\n"), pThis->m_pt1st.x, pThis->m_pt1st.y, point.x, point.y);
			//pThis->SetDlgItemInt(IDC_EDIT_OFFSET_33, pThis->m_pt1st.x);
			//pThis->SetDlgItemInt(IDC_EDIT_OFFSET_34, pThis->m_pt1st.y);
		}
		else if( (pThis->m_nClickAble == 2) )
		{
			COLORCROSS colorCross;
			COLORALIGNMARK clrMark;
			colorCross.CreateObject( PDC_BLUE, pThis->m_ImageCtrl.GetMousePositionOnImage().x, pThis->m_ImageCtrl.GetMousePositionOnImage().y, 50,10 );
			CxGraphicObject& pGO = pThis->m_ImageCtrl.GetGraphicObject();
			pGO.Reset();

			pGO.AddDrawCross( colorCross ); //화면에 선택된 좌표 표시

			pThis->m_pt1st.x = pThis->m_ImageCtrl.GetMousePositionOnImage().x;
			pThis->m_pt1st.y = pThis->m_ImageCtrl.GetMousePositionOnImage().y;

			int aaa= pThis->GetDlgItemInt(IDC_EDIT_OFFSET_43);
			if(aaa< 5) aaa= 5;
			double dfact;
			int isumRect;
			if(pThis->mTopCameraNo== e2X_LENS)	dfact= pThis->ParamFile()->System_data.dTop_Cam_PixelToum_2X;
			else								dfact= pThis->ParamFile()->System_data.dTop_Cam_PixelToum_20X;
			pThis->m_pt1st.x = pThis->m_ImageCtrl.GetMousePositionOnImage().x;
			pThis->m_pt1st.y = pThis->m_ImageCtrl.GetMousePositionOnImage().y;
			isumRect= (int)((/*pThis->m_dCur_Mark_Rect*/aaa/*/1000.0*/) / dfact);

			clrMark.CreateObject( PDC_GREEN, pThis->m_pt1st.x, pThis->m_pt1st.y, isumRect, isumRect/2, PS_SOLID, 1);
			pGO.AddDrawAlignMark( clrMark ); 
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

			CImageObj* pImgObj = pThis->m_ImageCtrl.GetVisibleImageObject();
			pSystem.GetVisionComponent()->PatternTmpImageDefine(pThis->mTopCameraNo, pImgObj, mdTMP_MASK, pThis->m_pt1st);

			double cam1_Xmm, cam1_Ymm;//mTopCameraNo; //e2X_LENS 
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, pThis->mTopCameraNo, pThis->m_pt1st.x, pThis->m_pt1st.y, &cam1_Xmm, &cam1_Ymm);

			CString strPosition;

			CPoint ptCnvData;
			double cnv_Xpix, cnv1_Ypix;//mTopCameraNo; //e2X_LENS 
			pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(pThis->mTopCameraNo, cam1_Xmm, cam1_Ymm, &cnv_Xpix, &cnv1_Ypix);
			ptCnvData.x= (LONG)(cnv_Xpix); ptCnvData.y= (LONG)(cnv1_Ypix);

			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_33, ptCnvData.x);
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_34, ptCnvData.y);
			if(pThis->iSelect_B_Layer== 0) //스탬프위치
			{
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X= cam1_Xmm;
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y= cam1_Ymm;
				pThis->m_dStX_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X;
				pThis->m_dStY_1= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y;
				strPosition.Format(_T("%.3f"), cam1_Xmm);		pThis->SetDlgItemText(IDC_EDIT_OFFSET_39, strPosition.GetBuffer(0));
				strPosition.Format(_T("%.3f"), cam1_Ymm);		pThis->SetDlgItemText(IDC_EDIT_OFFSET_40, strPosition.GetBuffer(0));
				if(pThis->mTopCameraNo== e2X_LENS)
				{
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix= ptCnvData.x;
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix= ptCnvData.y;
				}
				else
				{
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix= ptCnvData.x;
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix= ptCnvData.y;
				}
			}
			else //나머지 : 마스크, 중심
			{
				pThis->m_dCamPos_1st_X= pThis->m_pt1st.x;
				pThis->m_dCamPos_1st_Y= pThis->m_pt1st.y;
				if(pThis->m_StageNo== MANUAL_STAMP_PROC)
				{
					if(pThis->CameraStageUsedNo== eBTM_CAM)
					{
						pThis->Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
						//pThis->Select_Pos_X1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
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

			double cam1_Xmm, cam1_Ymm;//mTopCameraNo; //e2X_LENS 
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, pThis->mTopCameraNo, pThis->m_pt2nd.x, pThis->m_pt2nd.y, &cam1_Xmm, &cam1_Ymm);

			CPoint ptCnvData;
			double cnv_Xpix, cnv1_Ypix;//mTopCameraNo; //e2X_LENS 
			pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(pThis->mTopCameraNo, cam1_Xmm, cam1_Ymm, &cnv_Xpix, &cnv1_Ypix);
			ptCnvData.x= (LONG)(cnv_Xpix); ptCnvData.y= (LONG)(cnv1_Ypix);

/*
			strPosition.Format( _T("X : %d, Y : %d"), ptCnvData.x, ptCnvData.y);
			COLORTEXT colorText;
			colorText.CreateObject( PDC_BLUE, ptCnvData.x+10, ptCnvData.y-20, 12 );
			colorText.SetText( strPosition );
			pGO.AddDrawText( colorText );
*/
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_35, ptCnvData.x);
			pThis->SetDlgItemInt(IDC_EDIT_OFFSET_36, ptCnvData.y);

			if(pThis->iSelect_B_Layer== 0)
			{
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X= cam1_Xmm;
				pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y= cam1_Ymm;
				pThis->m_dStX_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X;
				pThis->m_dStY_2= pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y;
				strPosition.Format(_T("%.3f"), cam1_Xmm);		pThis->SetDlgItemText(IDC_EDIT_OFFSET_41, strPosition.GetBuffer(0));
				strPosition.Format(_T("%.3f"), cam1_Ymm);		pThis->SetDlgItemText(IDC_EDIT_OFFSET_42, strPosition.GetBuffer(0));
				if(pThis->mTopCameraNo== e2X_LENS)
				{
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_2nd_Xpix= ptCnvData.x;
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_2nd_Ypix= ptCnvData.y;
				}
				else
				{
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Xpix= ptCnvData.x;
					pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Ypix= ptCnvData.y;
				}
				if(pThis->CameraStageUsedNo== eBTM_CAM)
				{ 
					if(pThis->m_SelectMarkNo== eALIGN_2ND_MARK)
					{
						g_dBtm_2nd_MaskMark_BX= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
						g_dBtm_2nd_MaskMark_BY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
					}
				}
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
void CdlgManuAlign::ClickJogSelect3() //2x
{
	mTopCameraNo= e2X_LENS;
	DispUpdate();
	m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dTop_Cam_PixelToum_2X) );
	switch(m_StageNo)
	{
	case ROLL_ALIGN_PROC:
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
		break;
	case STAGE_1_ALIGN_PROC:
	case STAGE_2_ALIGN_PROC:
	case STAGE_3_ALIGN_PROC:
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
		break;
	case MANUAL_STAMP_PROC:
		if(CameraStageUsedNo== eBTM_CAM)
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_2X ) );
		else
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_2X ) );
		break;
	}
  	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
}

//===================================================================================
void CdlgManuAlign::ClickJogSelect4() //20x
{ 
	mTopCameraNo= e20X_LENS;
	DispUpdate();
	switch(m_StageNo)
	{
	case ROLL_ALIGN_PROC:
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_20X ) );
		m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
		break;
	case STAGE_1_ALIGN_PROC:
	case STAGE_2_ALIGN_PROC:
	case STAGE_3_ALIGN_PROC:
		m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_20X ) );
		m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
		break;
	case MANUAL_STAMP_PROC:
		m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
		if(CameraStageUsedNo== eBTM_CAM)
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_20X ) );
		else
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_20X ) );
		break;
	}
 	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
}
//===================================================================================
void CdlgManuAlign::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
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
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		if(iStageJog_Select== STAGE_SEL_1)
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
		else
		{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_X, _DIR_CW, SetSpeed);
		}
	}
}


void CdlgManuAlign::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
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


void CdlgManuAlign::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_X_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_X_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		if(iStageJog_Select== STAGE_SEL_1)
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
		else
		{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_X];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_X, _DIR_CCW, SetSpeed);
		}
	}
}


void CdlgManuAlign::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
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


void CdlgManuAlign::MouseDownJogCameraYCcw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_CAMERA_Y_CCW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, -m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		if(m_StageNo== MANUAL_STAMP_PROC) 
		{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_ALIGN_Y1];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_ALIGN_Y1];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_ALIGN_Y1];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->Align_JogStart(ALIGN_STAGE_Y, _DIR_CW, SetSpeed);
		}
		else
		{
			if(iStageJog_Select== STAGE_SEL_1)
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
			else
			{
				switch(Jog_Mode) {
				case _PTP_MOVE:
				case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
				case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
				case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
				default:		return ;
				}
				pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Y, _DIR_CCW, SetSpeed);
			}
		}
	}
}


void CdlgManuAlign::MouseUpJogCameraYCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(m_StageNo== MANUAL_STAMP_PROC) 
		{//MANUAL_STAMP_PROC://m_StageNo
			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
		}
		else
		{
			if(iStageJog_Select== STAGE_SEL_1)
				pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Y);
			else
				pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
		}
	}
}
//===================================================================================


void CdlgManuAlign::MouseDownJogCameraYCw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_CAMERA_Y_CW
	CRect disp_rect;
	GetDlgItem(IDB_JOG_CAMERA_Y_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		if(m_StageNo== MANUAL_STAMP_PROC) 
		{
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_ALIGN_Y1];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_ALIGN_Y1];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_ALIGN_Y1];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->Align_JogStart(ALIGN_STAGE_Y, _DIR_CCW, SetSpeed);
		}
		else
		{
			if(iStageJog_Select== STAGE_SEL_1)
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
			else
			{
				switch(Jog_Mode) {
				case _PTP_MOVE:
				case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
				case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
				case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Y];	break;
				default:		return ;
				}
				pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Y, _DIR_CW, SetSpeed);
			}
		}
	}
}


void CdlgManuAlign::MouseUpJogCameraYCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(m_StageNo== MANUAL_STAMP_PROC) 
		{//MANUAL_STAMP_PROC://m_StageNo
			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
		}
		else
		{
			if(iStageJog_Select== STAGE_SEL_1)
				pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Y);
			else
				pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
		}
	}
}
//===================================================================================


void CdlgManuAlign::MouseDownJogStageTCw(short Button, short Shift, long x, long y)
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
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, -m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		if(iStageJog_Select== STAGE_SEL_1)
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
		else
		{
			double SetSpeed;
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Z, _DIR_CW, SetSpeed);
		}
	}
}


void CdlgManuAlign::MouseUpJogStageTCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Z);
		else
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Z);
	}
}
//===================================================================================


void CdlgManuAlign::MouseDownJogStageTCcw(short Button, short Shift, long x, long y)
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
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, m_dPtpMove_mm, _SPD_MODE);		
		else
			pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Z, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		if(iStageJog_Select== STAGE_SEL_1)
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
		else
		{
			double SetSpeed;
			switch(Jog_Mode) {
			case _PTP_MOVE:
			case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_LOWER_CAM_Z];	break;
			default:		return ;
			}
			pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_LOWER_CAM_Z, _DIR_CCW, SetSpeed);
		}
	}
}


void CdlgManuAlign::MouseUpJogStageTCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Z);
		else
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Z);
	}
}
//===================================================================================
BOOL CdlgManuAlign::MoveEndCheck(int Axis) 
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
void CdlgManuAlign::ClickStageJogSel1()
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

void CdlgManuAlign::ClickStageJogSel2()
{
	iStageJog_Select= STAGE_SEL_2;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));
}

//===================================================================================
void CdlgManuAlign::DblClickJogSelect3()
{
		CString strText;
		double Cur_X, Cur_Y, Cur_Z, ToPosition1;
		// 	if(mTopCameraNo != e2X_LENS)
		// 	{
		if(	m_StageNo== STAGE_1_ALIGN_PROC || m_StageNo== STAGE_2_ALIGN_PROC || (m_StageNo== MANUAL_STAMP_PROC && CameraStageUsedNo== eTOP_CAM) )
		{
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
		else
		{
/*
			strText.Format(_T("하부 2X 위치 만큼 모터를 이동 하시겠습니까?\nZ축 제외"));
			if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
			{
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z);
				strText.Format(_T("상부 2X 위치 만큼 모터를 이동 하시겠습니까?"));
				Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
				Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
				Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
				ToPosition1= Cur_X - ParamFile()->System_data.dTopCam20To2_X;
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				ToPosition1= Cur_Y - ParamFile()->System_data.dTopCam20To2_Y;
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(	m_StageNo== STAGE_1_ALIGN_PROC || m_StageNo== STAGE_2_ALIGN_PROC)
				{
					ToPosition1= Cur_Z - ParamFile()->System_data.dTopCam20To2_Z;
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				}
			}
*/
		}
}

//===================================================================================
void CdlgManuAlign::DblClickJogSelect4()
{
	CString strText;
	// 	if(mTopCameraNo != e20X_LENS)
	// 	{
	if(	m_StageNo== STAGE_1_ALIGN_PROC || m_StageNo== STAGE_2_ALIGN_PROC || (m_StageNo== MANUAL_STAMP_PROC && CameraStageUsedNo== eTOP_CAM) )
	{
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
	//	}
}

//===================================================================================
void CdlgManuAlign::ClickJogSelect6()
{//1st 이동
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_SelectMarkNo= eALIGN_1ST_MARK;
	m_nClickAble= 0;
	m_btnSelectPoint.SetValue(0);

/*
	CString strText;
	double ToPosition1, ToPosition2;
	double add_x= 0.0, add_y= 0.0, add_z= 0.0;
	if(m_StageNo == STAGE_1_ALIGN_PROC)
	{
		if(mTopCameraNo== e2X_LENS)
		{
			strText.Format(_T("상부 카메라] 2X 위치 만큼 모터를 이동 하시겠습니까?"));
		}
		else
		{
			strText.Format(_T("상부 카메라] 2X 위치 만큼 모터를 이동 하시겠습니까?"));
			add_x= ParamFile()->System_data.dTopCam20To2_X;
			add_y= ParamFile()->System_data.dTopCam20To2_Y;
			add_z= ParamFile()->System_data.dTopCam20To2_Z;
		}
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN)+add_x;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_STAGE1_ALIGN)+add_z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE1_ALIGN)+add_y;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

// 			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_STAGE1_ALIGN);
// 			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_STAGE1_ALIGN);
// 			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
	}
	else if(m_StageNo == STAGE_2_ALIGN_PROC)
	{
	}
	else
	{
	}
*/
}

//===================================================================================
void CdlgManuAlign::ClickJogSelect7()
{//2nd 이동
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_SelectMarkNo= eALIGN_2ND_MARK;
	m_nClickAble= 0;
	m_btnSelectPoint.SetValue(0);
	GetDlgItem(IDB_STAGE_JOG_SEL4)->EnableWindow(TRUE);

/*
	CString strText;
	double ToPosition1, ToPosition2;
	double add_x= 0.0, add_y= 0.0, add_z= 0.0;
	if(m_StageNo == STAGE_1_ALIGN_PROC)
	{
		if(mTopCameraNo== e2X_LENS)
		{
			strText.Format(_T("상부 카메라] 2X 위치 만큼 모터를 이동 하시겠습니까?"));
			add_x= -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo];
		}
		else
		{
			strText.Format(_T("상부 카메라] 2X 위치 만큼 모터를 이동 하시겠습니까?"));
			add_x= ParamFile()->System_data.dTopCam20To2_X -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo];
			add_y= ParamFile()->System_data.dTopCam20To2_Y;
			add_z= ParamFile()->System_data.dTopCam20To2_Z;
		}
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN)+add_x;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_STAGE1_ALIGN)+add_z;//+ParamFile()->System_data.dTopCam20To2_Z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE1_ALIGN)+add_y;//+ParamFile()->System_data.dTopCam20To2_Y;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

// 			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_STAGE1_ALIGN);
// 			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_STAGE1_ALIGN);
// 			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
	}
	else if(m_StageNo == STAGE_2_ALIGN_PROC)
	{
	}
	else
	{
	}
*/
}

//===================================================================================
void CdlgManuAlign::ClickStageJogSel3()
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
void CdlgManuAlign::ClickStageJogSel4()
{//계산
	UpdateData(TRUE);
	if(iSelect_B_Layer== 0)
	{
		if(m_StageNo==MANUAL_QT_ROLL_ALIGN_PROC)
		{
			if(iSelect_B_Layer== SEL_STAMP_MODE)
			{
				BOOL ret= FALSE;
				iSelMarkPos= 0;
				if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
				{
					if(CameraStageUsedNo== eTOP_CAM)
						AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
					else
					{

						CString strText;
						double cur_pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
						double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
						if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
						{
							if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
							if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
						}

						ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
						if(ret)	ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);

						//--------------
						if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
						{
							if(cur_pos < 17.0)
							{
								if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
								if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
								if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, cur_pos,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
								if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
							}
						}
						//--------------
					}
				}
				else if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0 && 
					ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
				{
					if(CameraStageUsedNo== eTOP_CAM)
						AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
					else
					{
						ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
						GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0;
					}
				}
				else if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 1 && 
					ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
				{
					if(CameraStageUsedNo== eTOP_CAM)
						AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
					else
					{
						ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
						GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0;
					}
				}
				else
				{
					AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
				}

				if(ret)
				{
					GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		
				}
				else
				{	GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE);		iSelMarkPos= 1; }
				if(CameraStageUsedNo== eTOP_CAM) iSelMarkPos= 1;
				else							 iSelMarkPos= 0;

				return;
			}
		}
		AfxMessageBox(_T("마스크 위치 선택시에는 계산이 안됩니다.\n첫번째 마크 위치로 이동 후 다음 작업을 하십시요."));
		return;
	}

	if(m_StageNo==MANUAL_STAMP_PROC)	GetDlgItem(IDB_CM_MOVE9)->EnableWindow(TRUE);

	if(CameraStageUsedNo== eTOP_CAM)
	{
		GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE);
		GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);
		GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(iSelect_B_Layer!= 0)
	{
		if(m_StageNo== MANUAL_ROLL_ALIGN_PROC && pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG)
		{
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
			m_nClickAble= 0;
			m_btnSelectPoint.SetValue(0);

			Cal_Align();

			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			else													GetDlgItem(IDB_CM_MOVE7)->EnableWindow(TRUE);
			if(m_StageNo==MANUAL_STAMP_PROC || m_StageNo==MANUAL_ROLL_ALIGN_PROC)
			{
				GetDlgItem(IDB_CM_MOVE8)->EnableWindow(TRUE);
			}
		}
		else if(m_StageNo== STAGE_1_ALIGN_PROC || m_StageNo== STAGE_3_ALIGN_PROC || m_StageNo== STAGE_2_ALIGN_PROC)
		{
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
			m_nClickAble= 0;
			m_btnSelectPoint.SetValue(0);

			Cal_Align();

			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			else													GetDlgItem(IDB_CM_MOVE7)->EnableWindow(TRUE);
			if(m_StageNo==MANUAL_STAMP_PROC || m_StageNo==MANUAL_ROLL_ALIGN_PROC)
			{
				GetDlgItem(IDB_CM_MOVE8)->EnableWindow(TRUE);
			}
		}
		else
		{
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
			m_nClickAble= 0;
			m_btnSelectPoint.SetValue(0);

			CString strText;
			double cur_pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
			double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
			if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
			{
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			}

			Cal_Align();

			//--------------
			if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
			{
				if(cur_pos < 17.0)
				{
					if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, cur_pos,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				}
			}
			//--------------

			if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0)		GetDlgItem(IDB_CM_MOVE7)->EnableWindow(FALSE);
			else													GetDlgItem(IDB_CM_MOVE7)->EnableWindow(TRUE);
			if(m_StageNo==MANUAL_STAMP_PROC || m_StageNo==MANUAL_ROLL_ALIGN_PROC)
			{
				GetDlgItem(IDB_CM_MOVE8)->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		AfxMessageBox(_T("마스크 위치 선택시에는 계산이 안됩니다."));
	}
}
//===================================================================================
void CdlgManuAlign::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
}


void CdlgManuAlign::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
}


void CdlgManuAlign::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
}
//===================================================================================
BOOL CdlgManuAlign::Cal_Align()
{
	double cam1_Xmm, cam1_Ymm;
	double cam2_Xmm, cam2_Ymm;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double curZ1= 0.0, curZ2= 0.0;
	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;

	int stageNo= m_StageNo;
	posx1= m_dCamPos_1st_X;	posy1= m_dCamPos_1st_Y;
	posx2= m_dCamPos_2nd_X;	posy2= m_dCamPos_2nd_Y;

	curZ1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);	curZ2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z2);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	double cnv_Stamp_Xpix, cnv_Stamp_Ypix;//mTopCameraNo; //e2X_LENS 
	double cnv_Stamp_Xpix2, cnv_Stamp_Ypix2;//mTopCameraNo; //e2X_LENS 
	pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 
		pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X, pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y, &cnv_Stamp_Xpix, &cnv_Stamp_Ypix);
	pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 
		pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X, pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y, &cnv_Stamp_Xpix2, &cnv_Stamp_Ypix2);

	int cam_Lens_Sel= mTopCameraNo; //e2X_LENS
	 
	if(posx1 <= 0.0 || posy1 <= 0.0 || posx2 <= 0.0 || posy2 <= 0.0 )
	{
		MyMessageBox(_T("계산에 실패했습니다. [Zero.]"), _T("Calc. Failed"), M_ICONERROR); return FALSE;
	}
	if(m_StageNo==MANUAL_ROLL_ALIGN_PROC || m_StageNo==MANUAL_QT_ROLL_ALIGN_PROC )
	{
		if(CameraStageUsedNo== eTOP_CAM )
		{
			MyMessageBox(_T("상부 카메라 미지원"), _T("Failed"), M_ICONERROR); return FALSE;
		}
		else
		{
			if( ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)	
			{
				int move_pos_no= POSITION_ROLL_ALIGN;

				if(mTopCameraNo== e2X_LENS)
				{
					if(pFrm->GetWindowHandler_AlignView()->MovePos(move_pos_no, FALSE, FALSE)== FALSE)
					{
						Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage [2X : MovePos:POSITION_ROLL_ALIGN]"));
						return FALSE;
					}
				}
				else
				{
					if(pFrm->GetWindowHandler_AlignView()->MovePos(move_pos_no, TRUE, FALSE)== FALSE)
					{
						Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage [10X : MovePos:POSITION_ROLL_ALIGN]"));
						return FALSE;
					}
				}

				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;

				if(mTopCameraNo== e2X_LENS) 
				{
					ParamFile()->Model_data.dStamp_Btm_2X_1st_Xpix= posx1/* + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0))*/ ;
					ParamFile()->Model_data.dStamp_Btm_2X_1st_Ypix= posy1;
					ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_2X_1st_Xpix;
					ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= posy1;
					pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;

					//---------
					pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -cam1_Xmm,  _SPD_MODE, TRUE); Sleep(500);
					double Cur_T= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
					double Offset_T= Cur_T - ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= Offset_T;

					ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
					ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
					ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
					ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					m_dCamPos_1st_X= ParamFile()->Model_data.dStamp_Btm_2X_1st_Xpix= _INSP_IMAGE_SIZE_X / 2.0;
					ParamFile()->Model_data.dStamp_Btm_2X_1st_Ypix= posy1;
					ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_2X_1st_Xpix;
					ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= posy1;
					pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;
					//---------

					ParamFile()->Model_data.dStamp_Btm_2X_2nd_Xpix= posx2/* + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0)) */;
					ParamFile()->Model_data.dStamp_Btm_2X_2nd_Ypix= posy2;
					ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_2X_2nd_Xpix;
					ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix= posy2;

					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);

					pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
					//			m_pVision->Trans_RealCoord(e2X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= cam2_Xmm;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= cam2_Ymm;

					double cal_x= fabs(ParamFile()->Model_data.dStamp_Len_X - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x + ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
					double cal_y= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= pSystem.GetVisionComponent()->GetAngle(0, 0,	cal_x, 	cal_y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);

					ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
				}
				else
				{
					ParamFile()->Model_data.dStamp_Btm_20X_1st_Xpix= posx1/* + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0))*/ ;
					ParamFile()->Model_data.dStamp_Btm_20X_1st_Ypix= posy1;
					ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_20X_1st_Xpix;
					ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix= posy1;
					pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;

					//---------
					pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -cam1_Xmm,  _SPD_MODE, TRUE); Sleep(500);
					double Cur_T= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
					double Offset_T= Cur_T - ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= Offset_T;

					ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
					ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
					ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
					ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

					m_dCamPos_1st_X= ParamFile()->Model_data.dStamp_Btm_20X_1st_Xpix= _INSP_IMAGE_SIZE_X / 2.0;
					ParamFile()->Model_data.dStamp_Btm_20X_1st_Ypix= posy1;
					ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_20X_1st_Xpix;
					ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix= posy1;
					pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;
					//---------

					ParamFile()->Model_data.dStamp_Btm_20X_2nd_Xpix= posx2/* + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0)) */;
					ParamFile()->Model_data.dStamp_Btm_20X_2nd_Ypix= posy2;
					ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_20X_2nd_Xpix;
					ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix= posy2;

					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);

					pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
					//			m_pVision->Trans_RealCoord(e2X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= cam2_Xmm;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= cam2_Ymm;

					double cal_x= fabs(ParamFile()->Model_data.dStamp_Len_X - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x + ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
					double cal_y= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= pSystem.GetVisionComponent()->GetAngle(0, 0,	cal_x, 	cal_y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);

					ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
				}
				return TRUE;
			}
			else 
			{
				if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
					ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
				{
					pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
					double bot_x= L_X;
					pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
					//			return FALSE;
						ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
						if(mTopCameraNo== e2X_LENS)
						{
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
						}
						else
						{
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
							ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
						}
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN),	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

					if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					Sleep(500);
					//return FALSE;
					//		void CMatrox::GetCam_ROICood_Pos(int lensno, double x, double y, double *retx, double *rety, double cenx, double ceny, bool h_rev, bool v_rev)
					if(iSelect_B_Layer== 1) //마스크 위치
					{
						cam_Lens_Sel= e2X_LENS;
						pSystem.GetVisionComponent()->GetCam_ROICood_Pos(mTopCameraNo, posx1, posy1, &cam1_Xmm, &cam1_Ymm, 
							cnv_Stamp_Xpix, cnv_Stamp_Ypix, true, false);
					} 
					else
					{
						pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, posx1, posy1, &cam1_Xmm, &cam1_Ymm, true, false);
					}
					//		pSystem.GetVisionComponent()->GetCam_ROICood(cam_Lens_Sel, posx1, posy1, &cam1_Xmm, &cam1_Ymm, true, false);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
					pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1, L_X, TopCamY, Stage_Y);
					ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
					strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
					TRACE(strText.GetBuffer(0));
					//2nd
					//		void CMatrox::GetCam_ROICood_Pos(int lensno, double x, double y, double *retx, double *rety, double cenx, double ceny, bool h_rev, bool v_rev)
					if(iSelect_B_Layer== 1) //마스크 위치
					{
						pSystem.GetVisionComponent()->GetCam_ROICood_Pos(mTopCameraNo, posx2, posy2, &cam2_Xmm, &cam2_Ymm, 
							cnv_Stamp_Xpix, cnv_Stamp_Ypix, true, false);
					}
					else
					{
						pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx2, posy2, &cam2_Xmm, &cam2_Ymm, true, false);
					}
					double posx1_tmp= 0.0, posx2_tmp= 0.0, posy1_tmp= 0.0, posy2_tmp= 0.0;
					double real_X_tmp= 0.0, real_Y_tmp= 0.0, real_X2_tmp= 0.0, real_Y2_tmp= 0.0, angle_tmp=0.0;
					if(iSelect_B_Layer== 1) //마스크 위치
					{
						//------------
						pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, cnv_Stamp_Xpix, cnv_Stamp_Ypix, &posx1_tmp, &posy1_tmp, true, false);

						pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, cnv_Stamp_Xpix2, cnv_Stamp_Ypix2, &posx2_tmp, &posy2_tmp, true, false);

						pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, posx1_tmp,	posy1_tmp,  	&real_X_tmp, &real_Y_tmp, 
							L_X, TopCamY, Stage_Y);
						pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, posx2_tmp,	posy2_tmp,  	&real_X2_tmp, &real_Y2_tmp, 
							L_X-ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], TopCamY, Stage_Y);
						angle_tmp= pSystem.GetVisionComponent()->GetAngle(real_X_tmp, real_Y_tmp, real_X2_tmp, real_Y2_tmp);
						//------------
					}

					ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
					pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2, 
						L_X-ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], TopCamY, Stage_Y);
					ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;
					double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
					double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[stageNo].dTheta= pSystem.GetVisionComponent()->GetAngle(
						ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
						ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
					double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
					double ToPosition1= 0.0;
					ang_moveD= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - ParamFile()->Align_Result_data[stageNo].dTheta;
					if(fabs(ang_moveD) > 3.0) 
					{
						strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}
					if(iSelect_B_Layer== 1) //마스크 센터
					{
						ang_moveD= ang_moveD +angle_tmp;
					}
					//	return FALSE;
					pSystem.GetVisionComponent()->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
						ang_moveD, &ang_moveX, &ang_moveY);

					//		return FALSE;
					if(fabs(ang_moveD) > 0.00005 && m_iAlignExc== 0)
					{
						//			return FALSE;
						pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
					}
					//		return FALSE;

					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					//return FALSE;
					double chaX= 0.0,  chaY= 0.0;
					if(iSelect_B_Layer== 1) //마스크 센터 
					{
						chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
						chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;
					}
					else
					{
						chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
						chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;
					}
					if(fabs(ang_moveD) < 0.00005 || m_iAlignExc== 1)
					{
						ang_moveD= 0.0;
						ang_moveX= -chaX;
						ang_moveY= chaY;
					}
					else
					{ 
						if(iSelect_B_Layer== 1) //마스크 위치
						{
							ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
							ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;
						}
						else
						{
							ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
							ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;
						}
					}
					strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
					TRACE(strText.GetBuffer(0));

					if(fabs(ang_moveX) > 20.0) 
					{
						strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveX);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}
					if(fabs(ang_moveY) > 20.0) 
					{
						strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveY);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ang_moveX, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;		
					}
					if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, ang_moveY, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;	
					}
					if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
				}
				else
				{
					MyMessageBox(_T("미지원"), _T("Failed"), M_ICONERROR); return FALSE;
				}
			}
		}
	}
	else if((m_StageNo== MANUAL_STAMP_PROC && CameraStageUsedNo== eTOP_CAM) )/*m_StageNo== STAGE_1_ALIGN_PROC || m_StageNo== STAGE_2_ALIGN_PROC || m_StageNo== STAGE_3_ALIGN_PROC || */
	{
		if(CameraStageUsedNo== eBTM_CAM || m_StageNo== MANUAL_STAMP_PROC)
		{
			if(mTopCameraNo== e2X_LENS)
			{
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
			}
			else
			{
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
			}
		}
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, Select_Pos_Y1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		Sleep(500);
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		pSystem.GetVisionComponent()->Trans_RealCoord(cam_Lens_Sel, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
		strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
		TRACE(strText.GetBuffer(0));
		//2nd
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X2,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, Select_Pos_Y2,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		Sleep(500);

		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
		pSystem.GetVisionComponent()->Trans_RealCoord(cam_Lens_Sel, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;

		double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
		double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
		ParamFile()->Align_Result_data[stageNo].dTheta= pSystem.GetVisionComponent()->GetAngle(
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
		ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
		double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
		double ToPosition1= 0.0;
		ang_moveD= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - ParamFile()->Align_Result_data[stageNo].dTheta;
		ang_moveD= /*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - */ParamFile()->Align_Result_data[stageNo].dTheta;
		ang_moveD= 0.0 - ParamFile()->Align_Result_data[stageNo].dTheta;
		if(fabs(ang_moveD) > 3.0) 
		{
			strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		pSystem.GetVisionComponent()->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
			ang_moveD, &ang_moveX, &ang_moveY);

		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, Select_Pos_Y1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(fabs(ang_moveD) > 0.00005 && m_iAlignExc== 0)
		{
			//			return FALSE;
			pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
		}
		 
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		double chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
		double chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;

		if(fabs(ang_moveD) < 0.00005 || m_iAlignExc== 1)
		{
			ang_moveD= 0.0;
			ang_moveX= -chaX;
			ang_moveY= chaY;
		}
/*
		if(fabs(ang_moveD) < 0.00005)
		{
			ang_moveX= -chaX;
			ang_moveY= chaY;
		}
*/
		else
		{
			ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
			ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;
		}
	
		strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
		TRACE(strText.GetBuffer(0));

		if(fabs(ang_moveX) > 20.0) 
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveX);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(fabs(ang_moveY) > 20.0) 
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveY);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ang_moveX, _SPD_MODE, TRUE)== FALSE)
		{	
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;		
		}
		if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, ang_moveY, _SPD_MODE, TRUE)== FALSE)
		{	
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;	
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//----------------------
		if(m_iAlignExc== 0)
		{
			Sleep(ParamFile()->System_data.iImageGrab_Delay);
			pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_2X);
			find_model_no= mdTMP_MASK;
			int find_camNo;
			if(mTopCameraNo== e2X_LENS)		find_camNo= eTOP_CAM_2X;
			else							find_camNo= eBTM_CAM_20X;

			if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, find_camNo, true, true, true) == false)
			{
				pSystem.GetVisionComponent()->InspGrabContinue();
				strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			pSystem.GetVisionComponent()->OverlayClear(find_camNo);
			pSystem.GetVisionComponent()->InspGrabContinue();
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, posx1, posy1, &cam1_Xmm, &cam1_Ymm);

			if(!g_AutoRunAlignFlag) return FALSE;
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -cam1_Xmm, _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, cam1_Ymm, _SPD_MODE, TRUE)== FALSE)
			{	
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;	
			}
			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		}
	//----------------------

		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
		return TRUE;
	}//스탬프 얼라인
	else if(m_StageNo== MANUAL_STAMP_PROC && CameraStageUsedNo== eBTM_CAM && ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
	{
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			double bot_x= L_X;
			pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
//						return FALSE;
			if(m_StageNo== MANUAL_STAMP_PROC)
			{
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
				if(mTopCameraNo== e2X_LENS)
				{
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
				}
				else
				{
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
				}
			}
// 			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN),	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
// 			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
 			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, Select_Pos_Y1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			Sleep(500);
			//return FALSE;
			//		void CMatrox::GetCam_ROICood_Pos(int lensno, double x, double y, double *retx, double *rety, double cenx, double ceny, bool h_rev, bool v_rev)
			if(iSelect_B_Layer== 1) //마스크 위치
			{
				cam_Lens_Sel= e2X_LENS;
				pSystem.GetVisionComponent()->GetCam_ROICood_Pos(mTopCameraNo, posx1, posy1, &cam1_Xmm, &cam1_Ymm, 
					cnv_Stamp_Xpix, cnv_Stamp_Ypix, true, false);
			} 
			else
			{
				pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, posx1, posy1, &cam1_Xmm, &cam1_Ymm, true, false);
			}
			//		pSystem.GetVisionComponent()->GetCam_ROICood(cam_Lens_Sel, posx1, posy1, &cam1_Xmm, &cam1_Ymm, true, false);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
			pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1, L_X, TopCamY, Stage_Y);
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
			strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
			TRACE(strText.GetBuffer(0));
			//2nd
			//		void CMatrox::GetCam_ROICood_Pos(int lensno, double x, double y, double *retx, double *rety, double cenx, double ceny, bool h_rev, bool v_rev)
			if(iSelect_B_Layer== 1) //마스크 위치
			{
				pSystem.GetVisionComponent()->GetCam_ROICood_Pos(mTopCameraNo, posx2, posy2, &cam2_Xmm, &cam2_Ymm, 
					cnv_Stamp_Xpix, cnv_Stamp_Ypix, true, false);
			}
			else
			{
				pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx2, posy2, &cam2_Xmm, &cam2_Ymm, true, false);
			}
			double posx1_tmp= 0.0, posx2_tmp= 0.0, posy1_tmp= 0.0, posy2_tmp= 0.0;
			double real_X_tmp= 0.0, real_Y_tmp= 0.0, real_X2_tmp= 0.0, real_Y2_tmp= 0.0, angle_tmp=0.0;
			if(iSelect_B_Layer== 1) //마스크 위치
			{
				//------------
				pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, cnv_Stamp_Xpix, cnv_Stamp_Ypix, &posx1_tmp, &posy1_tmp, true, false);

				pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, cnv_Stamp_Xpix2, cnv_Stamp_Ypix2, &posx2_tmp, &posy2_tmp, true, false);

				pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, posx1_tmp,	posy1_tmp,  	&real_X_tmp, &real_Y_tmp, 
					L_X, TopCamY, Stage_Y);
				pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, posx2_tmp,	posy2_tmp,  	&real_X2_tmp, &real_Y2_tmp, 
					L_X-ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], TopCamY, Stage_Y);
				angle_tmp= pSystem.GetVisionComponent()->GetAngle(real_X_tmp, real_Y_tmp, real_X2_tmp, real_Y2_tmp);
				//------------
			}

			ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
			pSystem.GetVisionComponent()->Trans_RealCoord_FixPos(cam_Lens_Sel, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2, 
				L_X-ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], TopCamY, Stage_Y);
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;
			double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
			double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
			ParamFile()->Align_Result_data[stageNo].dTheta= pSystem.GetVisionComponent()->GetAngle(
				ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
				ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
			ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
			double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
			double ToPosition1= 0.0;
			ang_moveD= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - ParamFile()->Align_Result_data[stageNo].dTheta;
			if(fabs(ang_moveD) > 3.0) 
			{
				strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(iSelect_B_Layer== 1) //마스크 센터
			{
				ang_moveD= ang_moveD +angle_tmp;
			}
			//	return FALSE;
			pSystem.GetVisionComponent()->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
				ang_moveD, &ang_moveX, &ang_moveY);

			//		return FALSE;
			if(fabs(ang_moveD) > 0.00005 && m_iAlignExc== 0)
			{
				//			return FALSE;
				pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
			}
			//		return FALSE;

			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			//return FALSE;
			double chaX= 0.0,  chaY= 0.0;
			if(iSelect_B_Layer== 1) //마스크 센터 
			{
				chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
				chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;
			}
			else
			{
				chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
				chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;
			}
			if(fabs(ang_moveD) < 0.00005 || m_iAlignExc== 1)
			{
				ang_moveD= 0.0;
				ang_moveX= -chaX;
				ang_moveY= chaY;
			}
			else
			{ 
				if(iSelect_B_Layer== 1) //마스크 위치
				{
					ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
					ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;
				}
				else
				{
					ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
					ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;
				}
			}
			strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
			TRACE(strText.GetBuffer(0));

			if(fabs(ang_moveX) > 20.0) 
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveX);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(fabs(ang_moveY) > 20.0) 
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveY);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ang_moveX, _SPD_MODE, TRUE)== FALSE)
			{	
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;		
			}
			if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, ang_moveY, _SPD_MODE, TRUE)== FALSE)
			{	
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;	
			}
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
		return TRUE;
	} //스탬프 하부
	else if(m_StageNo== STAGE_1_ALIGN_PROC || m_StageNo== STAGE_2_ALIGN_PROC || m_StageNo== STAGE_3_ALIGN_PROC && CameraStageUsedNo== eTOP_CAM)
	{
		double Rollangle_2X= 0.0;
		double Rollangle_20X= 0.0;

		double x_len= ((ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix - ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix)*ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0) +
			ParamFile()->Model_data.dStamp_Len_X;
		double y_len= ((ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix - ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix)*ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0);
		Rollangle_2X= -pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, x_len, y_len);

		x_len= ((ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix - ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix)*ParamFile()->System_data.dTop_Cam_PixelToum_20X/1000.0) +
			ParamFile()->Model_data.dStamp_Len_X;
		y_len= ((ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix - ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix)*ParamFile()->System_data.dTop_Cam_PixelToum_20X/1000.0);
		Rollangle_20X= -pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, x_len, y_len);

		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, Select_Pos_Y1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		Sleep(500);
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		pSystem.GetVisionComponent()->Trans_RealCoord(cam_Lens_Sel, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
		strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
		TRACE(strText.GetBuffer(0));
		//2nd
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X2,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, Select_Pos_Y2,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		Sleep(500);

		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
		pSystem.GetVisionComponent()->Trans_RealCoord(cam_Lens_Sel, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;

		double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
		double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
		ParamFile()->Align_Result_data[stageNo].dTheta= pSystem.GetVisionComponent()->GetAngle(
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
		ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
		double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
		double ToPosition1= 0.0;
		ang_moveD= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - ParamFile()->Align_Result_data[stageNo].dTheta;

		if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 1)		Rollangle_2X= 0.0;
		if(cam_Lens_Sel== e2X_LENS)
			ang_moveD= /*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta*/Rollangle_2X - ParamFile()->Align_Result_data[stageNo].dTheta;
		else
			ang_moveD= /*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta*/Rollangle_20X - ParamFile()->Align_Result_data[stageNo].dTheta;
		if(fabs(ang_moveD) > 3.0) 
		{
			strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		pSystem.GetVisionComponent()->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
			ang_moveD, &ang_moveX, &ang_moveY);

		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, Select_Pos_X1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, Select_Pos_Y1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(fabs(ang_moveD) > 0.00005 && m_iAlignExc== 0)
		{
			//			return FALSE;
			pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
		}
		 
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		double chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
		double chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;
/*
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		double Oldcam1_Xmm= 0.0, Oldcam1_Ymm= 0.0;
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix, 
			&Oldcam1_Xmm, &Oldcam1_Ymm);
*/
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
		double Oldcam1_Xmm= 0.0, Oldcam1_Ymm= 0.0;
		if(cam_Lens_Sel== e2X_LENS)
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix, 
				&Oldcam1_Xmm, &Oldcam1_Ymm);
		else
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, cam_Lens_Sel, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix, 
				&Oldcam1_Xmm, &Oldcam1_Ymm);

		chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - Oldcam1_Xmm;
		chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - Oldcam1_Ymm;

		if(fabs(ang_moveD) < 0.00005 || m_iAlignExc== 1)
		{
			ang_moveD= 0.0;
			ang_moveX= -chaX;
			ang_moveY= chaY;
		}
/*
		if(fabs(ang_moveD) < 0.00005)
		{
			ang_moveX= -chaX;
			ang_moveY= chaY;
		}
*/
		else
		{
			ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
			ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;
		}
	
		strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
		TRACE(strText.GetBuffer(0));

		if(fabs(ang_moveX) > 20.0) 
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveX);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(fabs(ang_moveY) > 20.0) 
		{
			strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveY);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ang_moveX, _SPD_MODE, TRUE)== FALSE)
		{	
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;		
		}
		if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, ang_moveY, _SPD_MODE, TRUE)== FALSE)
		{	
			strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;	
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//----------------------
		if(m_iAlignExc== 0)
		{
			Sleep(ParamFile()->System_data.iImageGrab_Delay);
			pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_2X);
			find_model_no= mdTMP_MASK;
			int find_camNo;
			if(mTopCameraNo== e2X_LENS)		find_camNo= eTOP_CAM_2X;
			else							find_camNo= eTOP_CAM_20X;

			if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, find_camNo, true, true, true) == false)
			{
				pSystem.GetVisionComponent()->InspGrabContinue();
				strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			pSystem.GetVisionComponent()->OverlayClear(find_camNo);
			pSystem.GetVisionComponent()->InspGrabContinue();
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, mTopCameraNo, posx1, posy1, &cam1_Xmm, &cam1_Ymm);

			if(!g_AutoRunAlignFlag) return FALSE;
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -cam1_Xmm, _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, cam1_Ymm, _SPD_MODE, TRUE)== FALSE)
			{	
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;	
			}
			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		}
	//----------------------
		double add_x, add_y;
		if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 0) 
		{
			add_x= 0.0; add_y= 0.0;
		}
		else
		{
			add_x= ParamFile()->System_data.dTopCam20To2_X; add_y= ParamFile()->System_data.dTopCam20To2_Y;
		}

// 		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
// 		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
// 		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
// 		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);

		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-add_y;
		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-add_x;
		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
		ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
		return TRUE;
	}//Stage 얼라인

	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, curZ1, curZ2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	return TRUE;
}
//===================================================================================
BOOL CdlgManuAlign::MoveEndCheckNonDisp(int Axis) 
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

void CdlgManuAlign::ClickJogSelect8() 
{
	BOOL ret= FALSE;
	iSelMarkPos= 0;
	UpdateData(TRUE);
	if(m_StageNo == STAGE_1_ALIGN_PROC)
		ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	else if(m_StageNo == STAGE_2_ALIGN_PROC)
		ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	else if(m_StageNo == STAGE_3_ALIGN_PROC)
		ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	else if(m_StageNo == ROLL_ALIGN_PROC)
	{
		if(CameraStageUsedNo== eBTM_CAM)
			ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
		else
			ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	}
	else if(m_StageNo == MANUAL_STAMP_PROC)
	{
		if(CameraStageUsedNo== eTOP_CAM)
		{
			if(m_bPitchSpeedSel)
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _JOG_MODE, FALSE);
			else
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
		}
		else
			ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ParamFile()->Model_data.dStamp_Len_X, _SPD_MODE, FALSE);
	}
	else if(m_StageNo == MANUAL_ROLL_ALIGN_PROC || m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
	{
		//		AfxMessageBox(L"???");
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && 
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE &&
			ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			if(CameraStageUsedNo== eTOP_CAM)
				AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
			else
			{
				//------------
				CString strText;
				double cur_pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
				double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
				if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC && mTopCameraNo== e20X_LENS)
				{
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				}

				double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
				pSystem.GetMotionComponent()->GetStampPos(eBTM_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
				if(mTopCameraNo== e20X_LENS)
				{
					bot_X= bot_X + ParamFile()->System_data.dBotCam20To2_X;
					bot_Y= bot_Y + ParamFile()->System_data.dBotCam20To2_Y;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				//--------------

				if(m_bPitchSpeedSel)
					ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _JOG_MODE, FALSE);
				else
					ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				if(ret)	ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);

				//--------------
				if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
				{
					if(cur_pos < 17.0)
					{
						if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
						if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
						if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, cur_pos,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
						if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					}
				}
				//--------------
			}
		}
		else if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0 && 
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			if(CameraStageUsedNo== eTOP_CAM)
				AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
			else
			{
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0;
			}
		}
		else if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 1 && 
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			if(CameraStageUsedNo== eTOP_CAM)
				AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
			else
			{
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0;
			}
		}
		else
		{
			AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
		}

	}
	
	if(ret)
	{
		GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		
	}
	else
	{	GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE);		iSelMarkPos= 1; }
	if(CameraStageUsedNo== eTOP_CAM) iSelMarkPos= 1;
	else							 iSelMarkPos= 0;
}

//===================================================================================
void CdlgManuAlign::ClickJogSelect9()
{
	UpdateData(TRUE);
	BOOL ret= FALSE;
	iSelMarkPos= 1;
	if(m_StageNo == STAGE_1_ALIGN_PROC)
		ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	else if(m_StageNo == STAGE_2_ALIGN_PROC)
		ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	else if(m_StageNo == STAGE_3_ALIGN_PROC)
		ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	else if(m_StageNo == ROLL_ALIGN_PROC)
	{
		if(CameraStageUsedNo== eBTM_CAM)
			ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
		else
			ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
	}
	else if(m_StageNo == MANUAL_STAMP_PROC)
	{
		if(CameraStageUsedNo== eTOP_CAM)
		{
			if(m_bPitchSpeedSel)
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _JOG_MODE, FALSE);
			else
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
		}
		else
			ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -ParamFile()->Model_data.dStamp_Len_X, _SPD_MODE, FALSE);
	}
	else if(m_StageNo == MANUAL_ROLL_ALIGN_PROC || m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
	{
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
			ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			if(CameraStageUsedNo== eTOP_CAM)
				AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
			else
			{
				//------------
				CString strText;
				double cur_pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
				double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);
				if(m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC && mTopCameraNo== e20X_LENS)
				{
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				}

				double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
				pSystem.GetMotionComponent()->GetStampPos(eBTM_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);

				if(mTopCameraNo== e20X_LENS)
				{
					bot_X= bot_X + ParamFile()->System_data.dBotCam20To2_X;
					bot_Y= bot_Y + ParamFile()->System_data.dBotCam20To2_Y;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				//--------------
				if(m_bPitchSpeedSel)
					ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _JOG_MODE, FALSE);
				else
					ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				if(ret)
					ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				else
				{	GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0; }
				
				if(/*mTopCameraNo== e2X_LENS && */m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC)
				{
					if(cur_pos < 17.0)
					{
						if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
						if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
						if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, cur_pos,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
						if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					}
				}
			}
		}
		else if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0 && 
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			if(CameraStageUsedNo== eTOP_CAM)
				AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
			else
			{
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0;
			}
		}
		else if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 1 && 
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			if(CameraStageUsedNo== eTOP_CAM)
				AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
			else
			{
				ret= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dAlignMarkToMark[m_StageNo], _SPD_MODE, FALSE);
				GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0;
			}
		}
		else
		{
			AfxMessageBox(L"??? 미지원 모드... 동작 취소!!!");
		}
	}

	if(ret) { GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE);		 }
	else
	{	GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);		iSelMarkPos= 0; }
	if(CameraStageUsedNo== eTOP_CAM) iSelMarkPos= 0;
	else							 iSelMarkPos= 1;
}

//===================================================================================
void CdlgManuAlign::ClickCmMove()
{
	CString strText;
	double ToPosition1, ToPosition2;
	double add_x= 0.0, add_y= 0.0, add_z= 0.0;
	if(m_StageNo == STAGE_1_ALIGN_PROC)
	{
		if(mTopCameraNo== e2X_LENS)
		{
			strText.Format(_T("상부 카메라] 2X 위치로 모터를 이동 하시겠습니까?"));
		}
		else
		{
			strText.Format(_T("상부 카메라] 10X 위치로 모터를 이동 하시겠습니까?"));
			add_x= ParamFile()->System_data.dTopCam20To2_X;
			add_y= ParamFile()->System_data.dTopCam20To2_Y;
			add_z= ParamFile()->System_data.dTopCam20To2_Z;
		}
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN)+add_x;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_STAGE1_ALIGN)+add_z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE1_ALIGN)+add_y;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

// 			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_STAGE1_ALIGN);
// 			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_STAGE1_ALIGN);
// 			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		}
	}
	else if(m_StageNo == STAGE_2_ALIGN_PROC)
	{
	}
	else
	{
	}
}

//===================================================================================
void CdlgManuAlign::ClickCmMove6()
{
	CString strText;
	double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
	double ToPosition1, ToPosition2;

	strText.Format(_T("Align 초기 위치로 이동 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) != IDYES)
	{
		strText.Format(_T("모터 이동을 취소했습니다.")); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return;
	}
	if(m_StageNo==MANUAL_STAMP_PROC)	GetDlgItem(IDB_CM_MOVE9)->EnableWindow(FALSE);

	Align_Pos_Set_Flag= false;
	
	if(ParamFile()->System_data.iCAM_20X_USED_FLAG)
		GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_SHOW);

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= 0.0;
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(m_StageNo == MANUAL_STAMP_PROC)
	{
		if(CameraStageUsedNo== eTOP_CAM)
		{
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, TopCamY,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		else
		{
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
	} 
	else if(m_StageNo == STAGE_1_ALIGN_PROC || m_StageNo == STAGE_2_ALIGN_PROC || m_StageNo == STAGE_3_ALIGN_PROC)
	{
		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
		int move_pos_no= 0;
		if(m_StageNo== STAGE_1_ALIGN_PROC)		move_pos_no= POSITION_STAGE1_ALIGN;	
		else if(m_StageNo== STAGE_2_ALIGN_PROC)	move_pos_no= POSITION_STAGE2_ALIGN;
		else									move_pos_no= POSITION_STAGE3_ALIGN;
		if(pFrm->GetWindowHandler_AlignView()->MovePos(move_pos_no, FALSE, FALSE)== FALSE)
		{
			strText.Format(_T("모터 이동이 실패했습니다.") ); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR);
			strText.Format(_T("[ FAIL ] Stage #%d [MovePos:POSITION_STAGE_ALIGN] [S-1]"), m_StageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return ;
		}
	}
	else
	{
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE &&
			ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
		{
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		else
		{ 
			if(CameraStageUsedNo== eTOP_CAM)
			{
				pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, TopCamY,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			}
			else
			{
				if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE)	//m_StageNo==MANUAL_ROLL_ALIGN_PROC
				{
					pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
					if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				}
				else
				{
					int move_pos_no= POSITION_ROLL_ALIGN;
					CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
					if(pFrm->GetWindowHandler_AlignView()->MovePos(move_pos_no, FALSE, FALSE)== FALSE)
					{
						Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage [MovePos:POSITION_ROLL_ALIGN]"));
						return ;
					}
				}
			}
		}
	}
	if(m_StageNo == MANUAL_STAMP_PROC) GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_SHOW);
	if(CameraStageUsedNo== eTOP_CAM)
	{	GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE); }
	else
	{	
		GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);  GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); 
		g_dBtm_1st_MaskMark_LX= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		g_dBtm_1st_MaskMark_BX= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
		g_dBtm_1st_MaskMark_BY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
		g_dBtm_2nd_MaskMark_LX= 0.0;
		g_dBtm_2nd_MaskMark_BX= 0.0;
		g_dBtm_2nd_MaskMark_BY= 0.0;
	}
}



void CdlgManuAlign::ClickJogSelect10()
{
	iSelect_B_Layer= SEL_STAMP_MODE;
	SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
	m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
	m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
}


void CdlgManuAlign::ClickJogSelect11()
{
	iSelect_B_Layer= SEL_DEF_MASK_MODE;
	SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
	m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
	m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
}

void CdlgManuAlign::ClickJogSelect12()
{
	iSelect_B_Layer= SEL_MANUAL_CENTER_MODE;
	SetDlgItemInt(IDC_EDIT_OFFSET_33, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_34, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_35, 0);
	SetDlgItemInt(IDC_EDIT_OFFSET_36, 0);
	m_dCamPos_1st_X= 0.0;	m_dCamPos_1st_Y= 0.0;
	m_dCamPos_2nd_X= 0.0;	m_dCamPos_2nd_Y= 0.0;
}


void CdlgManuAlign::ClickCmMove7()  
{ 

	CString strText;
	double L_X= 0.0, Stage_Y= 0.0, TopCamY= 0.0, bot_X= 0.0, bot_Y= 0.0;
	double ToPosition1;
	
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	strText.Format(_T("10배 위치로 이동 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) != IDYES)
	{
		strText.Format(_T("모터 이동을 취소했습니다.")); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return;
	}
	//카메라 전환
	mTopCameraNo= e20X_LENS;
	DispUpdate();
		m_ImageCtrl.SetRealSizePerPixel((float)(ParamFile()->System_data.dBot_Cam_PixelToum_20X) );
		if(CameraStageUsedNo== eBTM_CAM)
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eBTM_CAM_20X ) );
		else
			m_ImageCtrl.SetImageObject( pSystem.GetVisionComponent()->GetImageObj( eTOP_CAM_20X ) );
	m_ImageCtrl.ImageUpdate();
	pSystem.GetVisionComponent()->InspGrabContinue();
	m_radio2X.SetValue(0);	m_radio20X.SetValue(1);

	///
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN);
	if( m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC &&
		ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
		ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
	{
		;
	}
	else
	{
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	}

	double cnv_Stamp_Xpix, cnv_Stamp_Ypix;//mTopCameraNo; //e2X_LENS 
	double cnv_Stamp_Xpix2, cnv_Stamp_Ypix2;//mTopCameraNo; //e2X_LENS 
	if(iSelect_B_Layer!= SEL_MANUAL_CENTER_MODE)
	{
		pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, //mTopCameraNo=20X
			pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_X, pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_Y, &cnv_Stamp_Xpix, &cnv_Stamp_Ypix);
		pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 
			pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_X, pSystem.GetParameterFileComponent()->Model_data.dStamp_20X_2nd_Y, &cnv_Stamp_Xpix2, &cnv_Stamp_Ypix2);
	}
	else
	{
		pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 0.0, 0.0, &cnv_Stamp_Xpix, &cnv_Stamp_Ypix);
		pSystem.GetVisionComponent()->GetCam_ROICood_pixPoint(mTopCameraNo, 0.0, 0.0, &cnv_Stamp_Xpix2, &cnv_Stamp_Ypix2);
	}

	if(m_StageNo == MANUAL_STAMP_PROC && ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
//	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_PLATE_MODE)
	{
		//		return;
		if(CameraStageUsedNo== eTOP_CAM)
		{
			MyMessageBox(_T("[설정 오류] 지원하지 않는 모드입니다."), _T("Failed"), M_ICONERROR); 
			return; 
			double chax= 0.0, chay= 0.0;
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, cnv_Stamp_Xpix, cnv_Stamp_Ypix,
				&chax, &chay, true, false);

			if(fabs(chax) > 3.0 || fabs(chay)> 3.0)
			{
				MyMessageBox(_T("[계산 오류] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
			}
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->System_data.dTopCam20To2_X-chax,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ParamFile()->System_data.dTopCam20To2_Y+chay, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		else
		{
			double chax= 0.0, chay= 0.0;
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, cnv_Stamp_Xpix, cnv_Stamp_Ypix,	&chax, &chay, true, false);
//			pSystem.GetVisionComponent()->GetCam_ROICood(e2X_LENS, cnv_Stamp_Xpix, cnv_Stamp_Ypix,	&chax, &chay, true, false);

			if(fabs(chax) > 3.0 || fabs(chay)> 3.0)
			{
				MyMessageBox(_T("[계산 오류] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
			}
//			return;
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ParamFile()->System_data.dBotCam20To2_X-chax,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ParamFile()->System_data.dBotCam20To2_Y+chay, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
	}
	else
	{
		if(CameraStageUsedNo== eTOP_CAM)
		{
			double chax= 0.0, chay= 0.0;
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, cnv_Stamp_Xpix, cnv_Stamp_Ypix,
				&chax, &chay, true, false);

			if(fabs(chax) > 3.0 || fabs(chay)> 3.0)
			{
				MyMessageBox(_T("[계산 오류] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
			}
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->System_data.dTopCam20To2_X-chax,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ParamFile()->System_data.dTopCam20To2_Y+chay, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		
			double Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
			double ToPositionTZ= Cur_Z + ParamFile()->System_data.dTopCam20To2_Z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPositionTZ,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_UPPER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_UPPER_CAM_Z), _ACS_AXIS_UPPER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		else
		{
			double chax= 0.0, chay= 0.0;
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, cnv_Stamp_Xpix, cnv_Stamp_Ypix,	&chax, &chay, true, false);

			if(fabs(chax) > 3.0 || fabs(chay)> 3.0)
			{
				MyMessageBox(_T("[계산 오류] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
			}
			if( m_StageNo == MANUAL_QT_ROLL_ALIGN_PROC &&
				ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
				ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
			{
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ParamFile()->System_data.dBotCam20To2_X,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ParamFile()->System_data.dBotCam20To2_Y, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			}
			else
			{
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ParamFile()->System_data.dBotCam20To2_X-chax,	_SPD_MODE, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ParamFile()->System_data.dBotCam20To2_Y+chay, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			}
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	
		}
	}
	GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_HIDE);
}



void CdlgManuAlign::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CdlgManuAlign::ClickCmMove8()
{
	CString strText;
	double add_x= 0.0, add_y= 0.0;
	strText.Format(_T("현재 정보를 이용 Align기준 좌표로 사용 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)
	{
		Align_Pos_Set_Flag= true; 

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

		if(CameraStageUsedNo== eBTM_CAM)
		{
			if(m_StageNo==MANUAL_ROLL_ALIGN_PROC)	
			{
				if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)	//m_StageNo==MANUAL_ROLL_ALIGN_PROC
				{
					double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
					double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
					if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
					if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

					ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
					CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
					return;
				}
			}

			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];

			if(mTopCameraNo== e2X_LENS)
			{
				add_x= 0.0; add_y= 0.0;
			}
			else
			{
				add_x= ParamFile()->System_data.dBotCam20To2_X; add_y= ParamFile()->System_data.dBotCam20To2_Y;
			}
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]= UpData[_ACS_AXIS_LOWER_CAM_X]-add_x;
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y]-add_y;

			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Z]= 
				ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Z];
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_ALIGN_Y1]= UpData[_ACS_AXIS_ALIGN_Y1];
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_ALIGN_Y2]= UpData[_ACS_AXIS_ALIGN_Y2];

			//-------------------------------
			double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X],	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y],	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
//			AfxMessageBox(_T("..."));
			//-------------------------------

			double masktobtm_X, masktobtm_Y;
			double pos1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			double pos2= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK);
			masktobtm_X= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]- pos1;
			masktobtm_Y= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]- pos2;
			double toTopmask_x;
			double toBotmask_x; 
			double CurtoBotmask_x; 
			double maskTomask= 0.0;
			double maskTomask_BX= 0.0;
			double maskTomask_BY= 0.0;
			toTopmask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			toBotmask_x= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			maskTomask_BX= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X] - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			maskTomask_BY= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y] - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
			maskTomask= toBotmask_x - toTopmask_x;

			CurtoBotmask_x= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X] - toBotmask_x;
			CurtoBotmask_x= CurtoBotmask_x +maskTomask_BX + toTopmask_x;

//			tobotmask_x= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]-pos1;//현재위치에서 하부캠 마스크 위치
			if(mTopCameraNo== e2X_LENS) 
			{
				add_x= 0.0; add_y= 0.0;
			}
			else
			{
				add_x= ParamFile()->System_data.dBotCam20To2_X; add_y= ParamFile()->System_data.dBotCam20To2_Y;
			}

			double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
			pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, CurtoBotmask_x, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, TopCamY, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
	}
}


void CdlgManuAlign::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	CxGraphicObject& pGO = m_ImageCtrl.GetGraphicObject();
	pGO.Reset();
}


void CdlgManuAlign::ClickCmMove9()
{
	CString strText;
	double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
	double ToPosition1, ToPosition2;

	strText.Format(_T("스템프 장착 위치로 이동 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) != IDYES)
	{
		strText.Format(_T("모터 이동을 취소했습니다.")); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return;
	}
	Align_Pos_Set_Flag= false; 

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= 0.0;
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_PLATE_MODE)
	{
		//		return;
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	}
	else
	{
		//if(CameraStageUsedNo== eTOP_CAM)			if(mTopCameraNo== e2X_LENS) 
			pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			double YopStampPosX= L_X;
			double Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);

			pSystem.GetMotionComponent()->GetStampPos(eBTM_CAM, mTopCameraNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			L_X= L_X + ParamFile()->System_data.dStampMoveOffset_X;

//			return;
//			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, Stage_Y, Stage_Y,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, L_X+(Cur_X-YopStampPosX),	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, bot_X,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, bot_Y,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
//			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
	}
/*
	if(m_StageNo == MANUAL_STAMP_PROC) GetDlgItem(IDB_CM_MOVE7)->ShowWindow(SW_SHOW);
	if(CameraStageUsedNo== eTOP_CAM)
	{	GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(TRUE); GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(FALSE); }
	else
	{	GetDlgItem(IDB_JOG_SELECT_8)->EnableWindow(FALSE);  GetDlgItem(IDB_JOG_SELECT_9)->EnableWindow(TRUE); }
*/
}

//===================================================================================
void CdlgManuAlign::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	if(m_bRollZ_Enable)
	{
		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDB_JOG_Z_CCW)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_Z_CW)->EnableWindow(FALSE);
	}
}

//===================================================================================
void CdlgManuAlign::MouseDownJogZCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CCW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 0.2) return;
		pSystem.GetMotionComponent()->INC_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, -m_dPtpMove_mm, -m_dPtpMove_mm, _SPD_MODE);		
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

void CdlgManuAlign::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CdlgManuAlign::MouseDownJogZCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_Z_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 0.2) return;
		pSystem.GetMotionComponent()->INC_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, m_dPtpMove_mm, m_dPtpMove_mm, _SPD_MODE);		
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

void CdlgManuAlign::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}


void CdlgManuAlign::DblClickCmMarkLoad()
{
/*
	if(CameraStageUsedNo== eTOP_CAM)
	{
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
	}
	else
	{
		if(mTopCameraNo== e2X_LENS) 
		{
			pSystem.GetMotionComponent()->AF_Proc_Start(eBTM_CAM_2X);
			MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z) ;
		}
		else
		{
			pSystem.GetMotionComponent()->AF_Proc_Start(eBTM_CAM_20X);
			MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z) ;
		}
		int ret= pSystem.GetVisionComponent()->AutoFocus();
		pSystem.GetVisionComponent()->AF_BufferSave(36);
		if(ret> 0 && ret <= ParamFile()->System_data.iAF_TotalCount)
		{
			double pos= pSystem.GetVisionComponent()->Z_AF_pos[ret];
			if(pos> -1.5 && pos <= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z))
				pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, pos);
		}
		else 
			AfxMessageBox(_T("A/F Error!!!"));
	}
*/
}

//===================================================================================
void CdlgManuAlign::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//===================================================================================
void CdlgManuAlign::OnBnClickedButton7()
{
	double ToX, ToY1, ToY2, ToCy, ToCz, ToBx, ToBy, ToBz, ToPosition1, ToPosition2;
	CString strText;

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

ToX= 235.8225;
ToY1= 8.0717;
ToY2= -1.0432;
ToCy= 67.9035;
ToCz= 14.0586;
ToBx= 0.0;
ToBy= 0.0;
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
//===================================================================================
void CdlgManuAlign::OnBnClickedButton1()
{
	UpdateData(TRUE);
}
//===================================================================================


void CdlgManuAlign::OnBnClickedButtonT()
{
	UpdateData(TRUE);

	int inc_pix= 0;
	switch(m_iPixSel)
	{
	case 0:		inc_pix= 1; break;
	case 1:		inc_pix= 2; break;
	case 2:		inc_pix= 5; break;
	default:	inc_pix= 1; break;
	}

	if(m_nClickAble == 1)
	{
		//m_pt1st.x;
		if((m_dCamPos_1st_Y-inc_pix) > 0) m_dCamPos_1st_Y= m_dCamPos_1st_Y - inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_33, (UINT)m_dCamPos_1st_X);		SetDlgItemInt(IDC_EDIT_OFFSET_34, (UINT)m_dCamPos_1st_Y);
	}
	else if(m_nClickAble == 2)
	{
		//m_pt1st.x;
		if((m_dCamPos_2nd_Y-inc_pix) > 0) m_dCamPos_2nd_Y= m_dCamPos_2nd_Y - inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_35, (UINT)m_dCamPos_2nd_X);		SetDlgItemInt(IDC_EDIT_OFFSET_36, (UINT)m_dCamPos_2nd_Y);
	}
}


void CdlgManuAlign::OnBnClickedButtonB()
{
	UpdateData(TRUE);

	int inc_pix= 0;
	switch(m_iPixSel)
	{
	case 0:		inc_pix= 1; break;
	case 1:		inc_pix= 2; break;
	case 2:		inc_pix= 5; break;
	default:	inc_pix= 1; break;
	}

	if(m_nClickAble == 1)
	{
		//m_pt1st.x;
		if((m_dCamPos_1st_Y+inc_pix) < _INSP_CAM_SIZE_Y) m_dCamPos_1st_Y= m_dCamPos_1st_Y + inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_33, (UINT)m_dCamPos_1st_X);		SetDlgItemInt(IDC_EDIT_OFFSET_34, (UINT)m_dCamPos_1st_Y);
	}
	else if(m_nClickAble == 2)
	{
		//m_pt1st.x;
		if((m_dCamPos_2nd_Y+inc_pix) < _INSP_CAM_SIZE_Y) m_dCamPos_2nd_Y= m_dCamPos_2nd_Y + inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_35, (UINT)m_dCamPos_2nd_X);		SetDlgItemInt(IDC_EDIT_OFFSET_36, (UINT)m_dCamPos_2nd_Y);
	}
}


void CdlgManuAlign::OnBnClickedButtonL()
{
	UpdateData(TRUE);

	int inc_pix= 0;
	switch(m_iPixSel)
	{
	case 0:		inc_pix= 1; break;
	case 1:		inc_pix= 2; break;
	case 2:		inc_pix= 5; break;
	default:	inc_pix= 1; break;
	}

	if(m_nClickAble == 1)
	{
		//m_pt1st.x;
		if((m_dCamPos_1st_X-inc_pix) > 0) m_dCamPos_1st_X= m_dCamPos_1st_X - inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_33, (UINT)m_dCamPos_1st_X);		SetDlgItemInt(IDC_EDIT_OFFSET_34, (UINT)m_dCamPos_1st_Y);
	}
	else if(m_nClickAble == 2)
	{
		//m_pt1st.x;
		if((m_dCamPos_2nd_X-inc_pix) > 0) m_dCamPos_2nd_X= m_dCamPos_2nd_X - inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_35, (UINT)m_dCamPos_2nd_X);		SetDlgItemInt(IDC_EDIT_OFFSET_36, (UINT)m_dCamPos_2nd_Y);
	}
}


void CdlgManuAlign::OnBnClickedButtonR()
{
	UpdateData(TRUE);

	int inc_pix= 0;
	switch(m_iPixSel)
	{
	case 0:		inc_pix= 1; break;
	case 1:		inc_pix= 2; break;
	case 2:		inc_pix= 5; break;
	default:	inc_pix= 1; break;
	}

	if(m_nClickAble == 1)
	{
		//m_pt1st.x;
		if((m_dCamPos_1st_X+inc_pix) < _INSP_CAM_SIZE_X)				m_dCamPos_1st_X= m_dCamPos_1st_X + inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_33, (UINT)m_dCamPos_1st_X);		SetDlgItemInt(IDC_EDIT_OFFSET_34, (UINT)m_dCamPos_1st_Y);
	}
	else if(m_nClickAble == 2)
	{
		//m_pt1st.x;
		if((m_dCamPos_2nd_X+inc_pix) < _INSP_CAM_SIZE_X)				m_dCamPos_2nd_X= m_dCamPos_2nd_X + inc_pix;
		SetDlgItemInt(IDC_EDIT_OFFSET_35, (UINT)m_dCamPos_2nd_X);		SetDlgItemInt(IDC_EDIT_OFFSET_36, (UINT)m_dCamPos_2nd_Y);
	}
}
BOOL CdlgManuAlign::Cal_Alig_Manual()
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

	pSystem.GetVisionComponent()->InspGrabContinue();
	if(cur_camNo== e2X_LENS)
	{
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= 0.0;
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0; ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;

//		pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_2X);
//		find_model_no= mdTOP_2X_STAEG1_START;
//		if(	pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
//		{
//			pSystem.GetVisionComponent()->InspGrabContinue();
//			strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
//			return FALSE;
//		}
// 		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
// 		pSystem.GetVisionComponent()->InspGrabContinue();
		posx1= m_dCamPos_1st_X; posy1= m_dCamPos_1st_Y;
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);

		chaX= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
		chaY= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y + ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;

		if(ParamFile()->System_data.iCAM_20X_USED_FLAG)
		{
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
			chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
			chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);
		}
		else
		{
			posx1= (posx1 - ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix) + (_INSP_IMAGE_SIZE_X/2.0);
			posy1= (posy1 - ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix) + (_INSP_IMAGE_SIZE_Y/2.0);
			pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
			chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x;
			chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix= (_INSP_IMAGE_SIZE_X/2.0);
			ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix= (_INSP_IMAGE_SIZE_Y/2.0);
		}
		
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

		if(!g_AutoRunAlignFlag) return FALSE;
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
		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);

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

		pSystem.GetVisionComponent()->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
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

		//----------------------
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
		//-----------------------

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
void CdlgManuAlign::MouseDownJogStageTCw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

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


void CdlgManuAlign::MouseUpJogStageTCw2(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}

//===================================================================================
void CdlgManuAlign::MouseDownJogStageTCcw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

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


void CdlgManuAlign::MouseUpJogStageTCcw2(short Button, short Shift, long x, long y)
{
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}


void CdlgManuAlign::ClickJogCameraYCcw3()
{
	CdlgLightControl dlg;
	dlg.DoModal();
}
