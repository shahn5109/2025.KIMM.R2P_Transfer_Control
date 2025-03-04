// frmDataView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmDataView.h"

#include "MainFrm.h"

// CfrmDataView
extern CSystemClass pSystem;
CParameterFile* CfrmDataView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmDataView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmDataView, CFormView)

CfrmDataView::CfrmDataView()
	: CFormView(CfrmDataView::IDD)
	, m_dStamp_Len_X(0)
	, m_dStamp_Len_Y(0)
	, m_dQuartz_PosX(0)
	, m_dQuartz_PosY(0)
	, dRollPlateStamp_PosX(0)
	, dRollPlateStamp_PosY(0)
	, m_dZ_SlowDownLen(0)
	, m_iRoll_AngleAlignMode(0)
	, m_dRoll_AngleAlign_Pitch(0)
	, m_iRoll_AngleAlign_Count(0)
	, m_dLED_Pitch_X(0)
	, m_dLED_Pitch_Y(0)
	, m_dShift_CamY(0)
{
	int i;
	for(i= 0; i< _MAX_RUN_PARAM; i++)
	{
		m_dRoll_Diameter[i]= 0.0;
		m_dRoll_Diameter_AddVal[i]= 0.0;
		m_dRun_Speed[i]= 0.0;
		m_dRun_Length[i]= 0.0;
		m_dRun_Force[i]= 0.0;
		m_dRun_Force2[i]= 0.0;
		m_dRun_DownSpeed[i]= 0.0;
		m_dRun_UpSpeed[i]= 0.0;
		m_iRun_Delay[i]= 0;
		m_dZ_Base_Position[i]= 0;
		m_iWaitFlag[i]= 0;

		m_dRun_Offset_X[i]= 0.0;
		m_dRun_Offset_Y[i]= 0.0;

		m_iRunEnable[i]= 0;
		m_iAlignEnable[i]= 0;
		m_iAlignMode[i]= 0;
		m_iRun_RollMode[i]= 0;

		m_iZ_Control_Mode[i]= _POSITION_MODE;
		m_iForceControlEnableFlag[i]= FALSE;
		m_dZ_Control_Fast_Length[i]= 0.0;
		m_dZ_Control_Slow_Length[i]= 0.0;

		m_dAlignMarkToMark[i]= 0.0;
		m_dAlignMarkToMarkY[i]= 0.0;
	}
}

CfrmDataView::~CfrmDataView()
{
}

void CfrmDataView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_11, m_dRoll_Diameter[ROLL_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_16, m_dRoll_Diameter[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_17, m_dRoll_Diameter[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_60, m_dRoll_Diameter[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_18, m_dRun_Speed[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_24, m_dRun_Speed[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_51, m_dRun_Speed[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_39, m_dRun_Speed[SYNC_PROC]);

	DDX_Text(pDX, IDC_EDIT_OFFSET_19, m_dRun_Length[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_25, m_dRun_Length[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_52, m_dRun_Length[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_40, m_dRun_Length[SYNC_PROC]);

	DDX_Text(pDX, IDC_EDIT_OFFSET_20, m_dRun_Force[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_26, m_dRun_Force[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_53, m_dRun_Force[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_68, m_dRun_Force2[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_69, m_dRun_Force2[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_70, m_dRun_Force2[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_21, m_dRun_DownSpeed[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_27, m_dRun_DownSpeed[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_54, m_dRun_DownSpeed[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_22, m_dRun_UpSpeed[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_28, m_dRun_UpSpeed[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_55, m_dRun_UpSpeed[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_23, m_iRun_Delay[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_29, m_iRun_Delay[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_56, m_iRun_Delay[STAGE_3_ALIGN_PROC]);

	DDX_Text(pDX, IDC_EDIT_OFFSET_47, m_dRun_Offset_X[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_49, m_dRun_Offset_X[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_58, m_dRun_Offset_X[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_48, m_dRun_Offset_Y[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_50, m_dRun_Offset_Y[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_59, m_dRun_Offset_Y[STAGE_3_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z25, m_lblRunEnable[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z31, m_lblRunEnable[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z32, m_lblRunEnable[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z43, m_lblRunEnable[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z41, m_lblRunEnable[SYNC_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z27, m_lblRunDisable[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z29, m_lblRunDisable[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z30, m_lblRunDisable[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z42, m_lblRunDisable[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z40, m_lblRunDisable[SYNC_PROC]);

	DDX_Radio(pDX, IDC_RADIO1, m_iWaitFlag[STAGE_1_ALIGN_PROC]);
	DDX_Radio(pDX, IDC_RADIO4, m_iWaitFlag[STAGE_2_ALIGN_PROC]);
	//	DDX_Radio(pDX, IDC_RADIO4, m_iWaitFlag[STAGE_3_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z1, m_lblRun_RollMode_Roll[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z4, m_lblRun_RollMode_Plate[ROLL_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_A_D, m_lblAlignMode_None[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_A_A, m_lblAlignMode_Auto[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_A_M, m_lblAlignMode_Manu[ROLL_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_S1_A_D, m_lblAlignMode_None[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_S1_A_A, m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_S1_A_M, m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_S2_A_D, m_lblAlignMode_None[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_S2_A_A, m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_S2_A_M, m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_S2_A_D2, m_lblAlignMode_None[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_S2_A_A2, m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_S2_A_M2, m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC]);

	DDX_Text(pDX, IDC_EDIT_OFFSET_30, m_dRoll_Diameter_AddVal[ROLL_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_31, m_dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_32, m_dRoll_Diameter_AddVal[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_61, m_dRoll_Diameter_AddVal[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_33, m_dZ_Base_Position[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_34, m_dZ_Base_Position[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_57, m_dZ_Base_Position[STAGE_3_ALIGN_PROC]);

	DDX_Check(pDX, IDC_CHECK1, m_iForceControlEnableFlag[STAGE_1_ALIGN_PROC]);
	DDX_Check(pDX, IDC_CHECK2, m_iForceControlEnableFlag[STAGE_2_ALIGN_PROC]);
	DDX_Check(pDX, IDC_CHECK4, m_iForceControlEnableFlag[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_35, m_dZ_Control_Fast_Length[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_37, m_dZ_Control_Fast_Length[STAGE_2_ALIGN_PROC]);
	//	DDX_Text(pDX, IDC_EDIT_OFFSET_37, m_dZ_Control_Fast_Length[STAGE_3_ALIGN_PROC]);

	DDX_Text(pDX, IDC_EDIT_OFFSET_62, m_dAlignMarkToMark[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_63, m_dAlignMarkToMarkY[STAGE_1_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_64, m_dAlignMarkToMark[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_65, m_dAlignMarkToMarkY[STAGE_2_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_66, m_dAlignMarkToMark[STAGE_3_ALIGN_PROC]);
	DDX_Text(pDX, IDC_EDIT_OFFSET_67, m_dAlignMarkToMarkY[STAGE_3_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_S1_A_D2, m_btnPos_Mode_S1);
	DDX_Control(pDX, IDB_OPU_S1_A_A2, m_btnForce_Mode_S1);
	DDX_Control(pDX, IDB_OPU_S1_A_D3, m_btnPos_Mode_S2);
	DDX_Control(pDX, IDB_OPU_S1_A_A3, m_btnForce_Mode_S2);
	DDX_Control(pDX, IDB_OPU_S1_A_D4, m_btnPos_Mode_S3);
	DDX_Control(pDX, IDB_OPU_S1_A_A4, m_btnForce_Mode_S3);
	DDX_Text(pDX, IDC_EDIT_OFFSET_41, m_dStamp_Len_X);
	DDX_Text(pDX, IDC_EDIT_OFFSET_42, m_dStamp_Len_Y);
	DDX_Text(pDX, IDC_EDIT_OFFSET_43, m_dQuartz_PosX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_44, m_dQuartz_PosY);
	DDX_Text(pDX, IDC_EDIT_OFFSET_45, dRollPlateStamp_PosX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_46, dRollPlateStamp_PosY);

	DDX_Control(pDX, IDB_OPU_ROLL_Z52, m_btnRollPosMode_Dis);
	DDX_Control(pDX, IDB_OPU_ROLL_Z53, m_btnRollPosMode_En);
	DDX_Text(pDX, IDC_EDIT_OFFSET_38, m_dZ_SlowDownLen);

	DDX_Radio(pDX, IDC_RADIO4, m_iDownEnd_WaitFlag);
	DDX_Radio(pDX, IDC_RADIO25, m_iRoll_AngleAlignMode);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY16, m_dRoll_AngleAlign_Pitch);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY17, m_iRoll_AngleAlign_Count);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY18, m_dLED_Pitch_X);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY19, m_dLED_Pitch_Y);
	DDX_Text(pDX, IDC_EDIT_CAM_DELAY20, m_dShift_CamY);
}
BEGIN_MESSAGE_MAP(CfrmDataView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CfrmDataView::OnBnClickedButton1)
	ON_MESSAGE(WM_MSG_DATA_UPDATE, DataReload)
	ON_BN_CLICKED(IDC_CHECK1, &CfrmDataView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CfrmDataView::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmDataView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK4, &CfrmDataView::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CfrmDataView 진단입니다.

#ifdef _DEBUG
void CfrmDataView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmDataView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmDataView 메시지 처리기입니다.
//===================================================================================
void CfrmDataView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_DataView(this);

	BackColor= RGB(153, 179, 233);
	m_brush.CreateSolidBrush(BackColor); 	brush1.CreateSolidBrush(RGB(255, 0, 0)); 
	m_GreenBrush.CreateSolidBrush(RGB(212, 255, 212));	m_BlueBrush.CreateSolidBrush(RGB(212, 212, 255));
	m_YellowBrush.CreateSolidBrush(RGB(255, 255, 212));	m_RedBrush.CreateSolidBrush(RGB(255, 212, 212));
	m_GrayBrush.CreateSolidBrush(RGB(196, 196, 196));
	m_WhiteBrush.CreateSolidBrush(RGB(255, 255, 255));

	DataUpdate();

	g_bCfrmDataViewInit= TRUE;
}

//===================================================================================
void CfrmDataView::OnDestroy()
{
	CFormView::OnDestroy();

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmDataView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmDataView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkMode(TRANSPARENT); 
	if(pWnd->GetDlgCtrlID()==IDC_STATIC || 
		pWnd->GetDlgCtrlID()==IDC_RADIO25 || pWnd->GetDlgCtrlID()==IDC_RADIO26) {	pDC->SetTextColor(RGB(0, 0, 0));		return (HBRUSH)m_WhiteBrush;	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmDataView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
LRESULT CfrmDataView::DataReload(WPARAM wParam,LPARAM lParam)
{
	DataUpdate();
	return 0;
}
//===================================================================================
void CfrmDataView::DataUpdate()
	//-----------------------------------------------------------------------------------
{
	CString strTmp;

	//	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
// 	ParamFile()->OpenSytemConfigFile();
// 	ParamFile()->OpenModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	int i;

	m_dZ_SlowDownLen= ParamFile()->Model_data.dZ_SlowDownLen;
	m_iDownEnd_WaitFlag= ParamFile()->Model_data.iDownEnd_WaitFlag;

	if(ParamFile()->System_data.RollAlignPosMode== _OPEN_POS_MODE)
	{//ParamFile()->System_data.iCAM_20X_USED_FLAG
		m_btnRollPosMode_Dis.SetValue(1);		m_btnRollPosMode_En.SetValue(0);
	}
	else
	{
		m_btnRollPosMode_Dis.SetValue(0);		m_btnRollPosMode_En.SetValue(1);
	}

	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]!= RUN_ROLL_MODE)
	{
		ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]= RUN_DISABLE;
		ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]= ALIGN_NONE_MODE;
	}
	for(i= 0; i< _MAX_RUN_PARAM; i++) 
	{
		m_dAlignMarkToMark[i]= ParamFile()->Model_data.dAlignMarkToMark[i]= ParamFile()->Model_data.dStamp_Len_X;
		m_dAlignMarkToMarkY[i]=	ParamFile()->Model_data.dAlignMarkToMarkY[i]= ParamFile()->Model_data.dStamp_Len_Y;

		m_dRoll_Diameter[i]	= ParamFile()->System_data.dRoll_Diameter[i];
		m_dRoll_Diameter_AddVal[i]	= ParamFile()->System_data.dRoll_Diameter_AddVal[i];

		m_iWaitFlag[i]= ParamFile()->Model_data.iWaitFlag[i];

		m_iRunEnable[i]	= ParamFile()->Model_data.iRunEnable[i];
		m_iAlignMode[i]= ParamFile()->Model_data.iAlignMode[i];
		m_iRun_RollMode[i]= ParamFile()->Model_data.iRun_RollMode[i];

		m_dRun_Speed[i]= ParamFile()->Model_data.dRun_Speed[i];
		m_dRun_Length[i]= ParamFile()->Model_data.dRun_Length[i];
		m_dRun_Force[i]= ParamFile()->Model_data.dRun_Force[i];
		m_dRun_Force2[i]= ParamFile()->Model_data.dRun_Force2[i];
		m_dRun_DownSpeed[i]= ParamFile()->Model_data.dRun_DownSpeed[i];
		m_dRun_UpSpeed[i]= ParamFile()->Model_data.dRun_UpSpeed[i];
		m_iRun_Delay[i]= ParamFile()->Model_data.iRun_Delay[i];
	
		m_dRun_Offset_X[i]= ParamFile()->Model_data.dRun_Offset_X[i];
		m_dRun_Offset_Y[i]= ParamFile()->Model_data.dRun_Offset_Y[i];

		m_dZ_Base_Position[i]	= ParamFile()->System_data.dZ_Base_Position[i];

		m_dZ_Control_Fast_Length[i]= ParamFile()->Model_data.dZ_Control_Fast_Length[i];
		m_dZ_Control_Slow_Length[i]= ParamFile()->Model_data.dZ_Control_Slow_Length[i];
	}
	ParamFile()->System_data.dRoll_Diameter[SYNC_PROC]= ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC];
	ParamFile()->System_data.dRoll_Diameter_AddVal[SYNC_PROC]= ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC];

		//---------------------------
		if(ParamFile()->Model_data.iZ_Control_Mode[STAGE_1_ALIGN_PROC]== _POSITION_MODE)	{	m_btnPos_Mode_S1.SetValue(1);	m_btnForce_Mode_S1.SetValue(0); m_iZ_Control_Mode[STAGE_1_ALIGN_PROC]= _POSITION_MODE; }
		else																				{	m_btnPos_Mode_S1.SetValue(0);	m_btnForce_Mode_S1.SetValue(1); m_iZ_Control_Mode[STAGE_1_ALIGN_PROC]= _FORCE_MODE;  }
		if(ParamFile()->Model_data.iZ_Control_Mode[STAGE_2_ALIGN_PROC]== _POSITION_MODE)	{	m_btnPos_Mode_S2.SetValue(1);	m_btnForce_Mode_S2.SetValue(0); m_iZ_Control_Mode[STAGE_2_ALIGN_PROC]= _POSITION_MODE; }
		else																				{	m_btnPos_Mode_S2.SetValue(0);	m_btnForce_Mode_S2.SetValue(1); m_iZ_Control_Mode[STAGE_2_ALIGN_PROC]= _FORCE_MODE;  }
		if(ParamFile()->Model_data.iZ_Control_Mode[STAGE_3_ALIGN_PROC]== _POSITION_MODE)	{	m_btnPos_Mode_S3.SetValue(1);	m_btnForce_Mode_S3.SetValue(0); m_iZ_Control_Mode[STAGE_3_ALIGN_PROC]= _POSITION_MODE; }
		else																				{	m_btnPos_Mode_S3.SetValue(0);	m_btnForce_Mode_S3.SetValue(1); m_iZ_Control_Mode[STAGE_3_ALIGN_PROC]= _FORCE_MODE;  }

		if(ParamFile()->Model_data.iForceControlEnableFlag[STAGE_1_ALIGN_PROC]== RUN_ENABLE)	{	m_iForceControlEnableFlag[STAGE_1_ALIGN_PROC]= TRUE; }
		else																					{	m_iForceControlEnableFlag[STAGE_1_ALIGN_PROC]= FALSE; }
		if(ParamFile()->Model_data.iForceControlEnableFlag[STAGE_2_ALIGN_PROC]== RUN_ENABLE)	{	m_iForceControlEnableFlag[STAGE_2_ALIGN_PROC]= TRUE; }
		else																					{	m_iForceControlEnableFlag[STAGE_2_ALIGN_PROC]= FALSE; }
		if(ParamFile()->Model_data.iForceControlEnableFlag[STAGE_3_ALIGN_PROC]== RUN_ENABLE)	{	m_iForceControlEnableFlag[STAGE_3_ALIGN_PROC]= TRUE; }
		else																					{	m_iForceControlEnableFlag[STAGE_3_ALIGN_PROC]= FALSE; }
		//---------------------------
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE)
		{	
			m_lblRunEnable[ROLL_ALIGN_PROC].SetValue(1);	m_lblRunDisable[ROLL_ALIGN_PROC].SetValue(0);
		}
		else
		{
			m_lblRunEnable[ROLL_ALIGN_PROC].SetValue(0);	m_lblRunDisable[ROLL_ALIGN_PROC].SetValue(1);
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC]== RUN_ENABLE)
		{
			m_lblRunEnable[STAGE_1_ALIGN_PROC].SetValue(1);	m_lblRunDisable[STAGE_1_ALIGN_PROC].SetValue(0);
		}
		else
		{
			m_lblRunEnable[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_1_ALIGN_PROC].SetValue(1);
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC]== RUN_ENABLE)
		{
			m_lblRunEnable[STAGE_2_ALIGN_PROC].SetValue(1);	m_lblRunDisable[STAGE_2_ALIGN_PROC].SetValue(0);
		}
		else
		{	
			m_lblRunEnable[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_2_ALIGN_PROC].SetValue(1);
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE)
		{
			m_lblRunEnable[STAGE_3_ALIGN_PROC].SetValue(1);	m_lblRunDisable[STAGE_3_ALIGN_PROC].SetValue(0);
		}
		else
		{	
			m_lblRunEnable[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_3_ALIGN_PROC].SetValue(1);
		}

		if(ParamFile()->Model_data.iRunEnable[SYNC_PROC]== RUN_ENABLE)
		{
			m_lblRunEnable[SYNC_PROC].SetValue(1);	m_lblRunDisable[SYNC_PROC].SetValue(0);
		}
		else
		{	
			m_lblRunEnable[SYNC_PROC].SetValue(0);	m_lblRunDisable[SYNC_PROC].SetValue(1);
		}

		//---------------------------
		if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		{	
			m_lblRun_RollMode_Roll[ROLL_ALIGN_PROC].SetValue(1);		m_lblRun_RollMode_Plate[ROLL_ALIGN_PROC].SetValue(0);		
			GetDlgItem(IDC_EDIT_OFFSET_18)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_OFFSET_19)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_OFFSET_24)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_OFFSET_25)->EnableWindow(TRUE);
		}
		else
		{
			m_lblRun_RollMode_Roll[ROLL_ALIGN_PROC].SetValue(0);		m_lblRun_RollMode_Plate[ROLL_ALIGN_PROC].SetValue(1);		
			GetDlgItem(IDC_EDIT_OFFSET_18)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OFFSET_19)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OFFSET_24)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OFFSET_25)->EnableWindow(FALSE);
		}
		//---------------------------

		if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetValue(0);
			ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);

			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;

			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;

			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_2nd_Xpix= _INSP_IMAGE_SIZE_X/2.0;

			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_2nd_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_2nd_Ypix= _INSP_IMAGE_SIZE_Y/2.0;

			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
		}
		else if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetValue(0);
		}
		else
		{	
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetValue(1);
		}

		if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetValue(0);
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetValue(0);
		}
		else
		{	
			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetValue(1);
		}

		if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetValue(0);
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetValue(0);
		}
		else
		{	
			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetValue(1);
		}

		if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetValue(0);
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetValue(1);	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetValue(0);
		}
		else
		{	
			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetValue(1);
		}

		m_dStamp_Len_X= ParamFile()->Model_data.dStamp_Len_X;
		m_dStamp_Len_Y= ParamFile()->Model_data.dStamp_Len_Y;
		m_dQuartz_PosX= ParamFile()->System_data.dQuartz_PosX;
		m_dQuartz_PosY= ParamFile()->System_data.dQuartz_PosY;
		dRollPlateStamp_PosX= ParamFile()->System_data.dRollPlateStamp_PosX;
		dRollPlateStamp_PosY= ParamFile()->System_data.dRollPlateStamp_PosY;
		m_dShift_CamY= ParamFile()->System_data.dShift_CamY; 

		if(ParamFile()->Model_data.iRoll_AngleAlignMode== RUN_DISABLE)	strTmp.Format(_T("★ Angle 계산 : 2 Point Align [수정->설비설정]"));
		else															strTmp.Format(_T("★ Angle 계산 : Multi-Point [수정->설비설정]\n     - 간격: %.3fmm, - Count: %d"), 
																					ParamFile()->Model_data.dRoll_AngleAlign_Pitch, ParamFile()->Model_data.iRoll_AngleAlign_Count);
	
	m_dLED_Pitch_X= ParamFile()->Model_data.dLED_Pitch_X;	
	m_dLED_Pitch_Y= ParamFile()->Model_data.dLED_Pitch_Y;	
	m_iRoll_AngleAlignMode= ParamFile()->Model_data.iRoll_AngleAlignMode;	
	m_dRoll_AngleAlign_Pitch= ParamFile()->Model_data.dRoll_AngleAlign_Pitch;	
	m_iRoll_AngleAlign_Count= ParamFile()->Model_data.iRoll_AngleAlign_Count;	

	SetDlgItemText(IDS_COMMAND6, strTmp.GetBuffer(0));
	UpdateData(FALSE);
}
//===================================================================================
void CfrmDataView::DataSave()
	//-----------------------------------------------------------------------------------
{
	UpdateData(TRUE);

	int i;

	if(	ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE)
	{
		m_iRunEnable[STAGE_1_ALIGN_PROC]= RUN_DISABLE;
		m_iRunEnable[STAGE_2_ALIGN_PROC]= RUN_DISABLE;
		m_iRunEnable[STAGE_3_ALIGN_PROC]= RUN_ENABLE;
		m_lblRunEnable[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_1_ALIGN_PROC].SetValue(1);
		m_lblRunEnable[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_2_ALIGN_PROC].SetValue(1);
		m_lblRunEnable[STAGE_3_ALIGN_PROC].SetValue(1);	m_lblRunDisable[STAGE_3_ALIGN_PROC].SetValue(0);
		m_iAlignMode[STAGE_3_ALIGN_PROC]= ALIGN_MANU_MODE;
		m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetValue(1);
	}

	for(i= 0; i< _MAX_RUN_PARAM; i++)
	{
		ParamFile()->Model_data.dAlignMarkToMark[i]= m_dAlignMarkToMark[i] =m_dStamp_Len_X;
		ParamFile()->Model_data.dAlignMarkToMarkY[i]= m_dAlignMarkToMarkY[i]= m_dStamp_Len_Y;

		ParamFile()->System_data.dRoll_Diameter[i]= m_dRoll_Diameter[i];
		ParamFile()->System_data.dRoll_Diameter_AddVal[i]= m_dRoll_Diameter_AddVal[i];
		ParamFile()->System_data.dZ_Base_Position[i]= m_dZ_Base_Position[i];

		ParamFile()->Model_data.iWaitFlag[i]= m_iWaitFlag[i];

		ParamFile()->Model_data.iRunEnable[i]= m_iRunEnable[i];
		ParamFile()->Model_data.iAlignMode[i]= m_iAlignMode[i];
		ParamFile()->Model_data.iRun_RollMode[i]= m_iRun_RollMode[i];

		ParamFile()->Model_data.dRun_Speed[i]= m_dRun_Speed[i];
		ParamFile()->Model_data.dRun_Length[i]= m_dRun_Length[i];
		ParamFile()->Model_data.dRun_Force[i]= m_dRun_Force[i];
		ParamFile()->Model_data.dRun_Force2[i]= m_dRun_Force2[i];
		ParamFile()->Model_data.dRun_DownSpeed[i]= m_dRun_DownSpeed[i];
		ParamFile()->Model_data.dRun_UpSpeed[i]= m_dRun_UpSpeed[i];
		ParamFile()->Model_data.iRun_Delay[i]= m_iRun_Delay[i];

		ParamFile()->Model_data.dRun_Offset_X[i]= m_dRun_Offset_X[i];
		ParamFile()->Model_data.dRun_Offset_Y[i]= m_dRun_Offset_Y[i];

		ParamFile()->Model_data.dZ_Control_Fast_Length[i]= m_dZ_Control_Fast_Length[i];
		ParamFile()->Model_data.dZ_Control_Fast_Length[i]= m_dZ_Control_Fast_Length[i];

		ParamFile()->Model_data.iForceControlEnableFlag[i]= m_iForceControlEnableFlag[i];
		ParamFile()->Model_data.iZ_Control_Mode[i]= m_iZ_Control_Mode[i];
	}
	ParamFile()->System_data.dRoll_Diameter[SYNC_PROC]= ParamFile()->System_data.dRoll_Diameter[STAGE_1_ALIGN_PROC];
	ParamFile()->System_data.dRoll_Diameter_AddVal[SYNC_PROC]= ParamFile()->System_data.dRoll_Diameter_AddVal[STAGE_1_ALIGN_PROC];

	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_PLATE_MODE)
	{
		ParamFile()->Model_data.dStamp_Len_X= m_dStamp_Len_X;
	}
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]!= RUN_ROLL_MODE)
	{
		ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]= RUN_DISABLE;
		ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]= ALIGN_NONE_MODE;
	}

	ParamFile()->Model_data.dStamp_Len_X= m_dStamp_Len_X;
	ParamFile()->Model_data.dStamp_Len_Y= m_dStamp_Len_Y;
	ParamFile()->Model_data.dAlignMarkToMark[MANUAL_STAMP_PROC]= m_dStamp_Len_X;
	ParamFile()->Model_data.dAlignMarkToMarkY[MANUAL_STAMP_PROC]= m_dStamp_Len_Y;

	ParamFile()->Model_data.dAlignMarkToMark[MANUAL_ROLL_ALIGN_PROC] = ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC];
	ParamFile()->Model_data.dAlignMarkToMarkY[MANUAL_ROLL_ALIGN_PROC]= ParamFile()->Model_data.dAlignMarkToMarkY[ROLL_ALIGN_PROC];

	ParamFile()->System_data.dQuartz_PosX= m_dQuartz_PosX;
	ParamFile()->System_data.dQuartz_PosY= m_dQuartz_PosY= 0.0;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);
	ParamFile()->System_data.dRollPlateStamp_PosX= dRollPlateStamp_PosX;
	ParamFile()->System_data.dRollPlateStamp_PosY= dRollPlateStamp_PosY= 0.0;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK);

	ParamFile()->Model_data.dZ_SlowDownLen= m_dZ_SlowDownLen;
	ParamFile()->Model_data.iDownEnd_WaitFlag= m_iDownEnd_WaitFlag;

// 	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_PLATE_MODE)
// 	{
// 		ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->System_data.dRollPlateStamp_PosX;//ParamFile()->Model_data.dStamp_Len_X / 2.0;
// 		ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->System_data.dRollPlateStamp_PosY - (ParamFile()->Model_data.dStamp_Len_Y / 2.0);
// 	}
	ParamFile()->Model_data.dLED_Pitch_X= m_dLED_Pitch_X;	
	ParamFile()->Model_data.dLED_Pitch_Y= m_dLED_Pitch_Y;	
	ParamFile()->Model_data.iRoll_AngleAlignMode= m_iRoll_AngleAlignMode;	
	if(m_dRoll_AngleAlign_Pitch <= 0.0) m_dRoll_AngleAlign_Pitch= 1.0;
	if(m_dRoll_AngleAlign_Pitch > 99.0) m_dRoll_AngleAlign_Pitch= 99.0;
	ParamFile()->Model_data.dRoll_AngleAlign_Pitch= m_dRoll_AngleAlign_Pitch;	
	if(m_iRoll_AngleAlign_Count < 1) m_iRoll_AngleAlign_Count= 1;
	if(m_iRoll_AngleAlign_Count > 99) m_iRoll_AngleAlign_Count= 99;
	ParamFile()->Model_data.iRoll_AngleAlign_Count= m_iRoll_AngleAlign_Count;	
	ParamFile()->System_data.dShift_CamY= m_dShift_CamY; 

	pSystem.GetMotionComponent()->CalAlignPos();

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CfrmDataView 수정 / 저장"));
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveSytemConfigFile();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
}


void CfrmDataView::OnBnClickedButton1()
{
}
BEGIN_EVENTSINK_MAP(CfrmDataView, CFormView)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z27, DISPID_CLICK, CfrmDataView::ClickOpuRollZ27, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z25, DISPID_CLICK, CfrmDataView::ClickOpuRollZ25, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z1, DISPID_CLICK, CfrmDataView::ClickOpuRollZ1, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z4, DISPID_CLICK, CfrmDataView::ClickOpuRollZ4, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_A_D, DISPID_CLICK, CfrmDataView::ClickOpuRollZ39, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_A_A, DISPID_CLICK, CfrmDataView::ClickOpuRollZ2, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_A_M, DISPID_CLICK, CfrmDataView::ClickOpuRollZ3, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z29, DISPID_CLICK, CfrmDataView::ClickOpuRollZ29, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z31, DISPID_CLICK, CfrmDataView::ClickOpuRollZ31, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_D, DISPID_CLICK, CfrmDataView::ClickOpuRollZ37, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_A, DISPID_CLICK, CfrmDataView::ClickOpuRollZ33, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_M, DISPID_CLICK, CfrmDataView::ClickOpuRollZ35, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z30, DISPID_CLICK, CfrmDataView::ClickOpuRollZ30, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z32, DISPID_CLICK, CfrmDataView::ClickOpuRollZ32, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S2_A_D, DISPID_CLICK, CfrmDataView::ClickOpuRollZ38, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S2_A_A, DISPID_CLICK, CfrmDataView::ClickOpuRollZ34, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S2_A_M, DISPID_CLICK, CfrmDataView::ClickOpuRollZ36, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_DATA_SAVE, DISPID_CLICK, CfrmDataView::ClickDataSave, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_D2, DISPID_CLICK, CfrmDataView::ClickOpuS1AD2, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_A2, DISPID_CLICK, CfrmDataView::ClickOpuS1AA2, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_D3, DISPID_CLICK, CfrmDataView::ClickOpuS1AD3, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_A3, DISPID_CLICK, CfrmDataView::ClickOpuS1AA3, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z40, DISPID_CLICK, CfrmDataView::ClickOpuRollZ40, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z41, DISPID_CLICK, CfrmDataView::ClickOpuRollZ41, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z42, DISPID_CLICK, CfrmDataView::ClickOpuRollZ42, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z43, DISPID_CLICK, CfrmDataView::ClickOpuRollZ43, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S2_A_D2, DISPID_CLICK, CfrmDataView::ClickOpuS2AD2, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S2_A_A2, DISPID_CLICK, CfrmDataView::ClickOpuS2AA2, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S2_A_M2, DISPID_CLICK, CfrmDataView::ClickOpuS2AM2, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_D4, DISPID_CLICK, CfrmDataView::ClickOpuS1AD4, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_S1_A_A4, DISPID_CLICK, CfrmDataView::ClickOpuS1AA4, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z52, DISPID_CLICK, CfrmDataView::ClickOpuRollZ52, VTS_NONE)
	ON_EVENT(CfrmDataView, IDB_OPU_ROLL_Z53, DISPID_CLICK, CfrmDataView::ClickOpuRollZ53, VTS_NONE)
END_EVENTSINK_MAP()

void CfrmDataView::ClickOpuRollZ40()	{	m_iRunEnable[SYNC_PROC]= RUN_DISABLE;	}
void CfrmDataView::ClickOpuRollZ41()	{	m_iRunEnable[SYNC_PROC]= RUN_ENABLE;	}

//===================================================================================
void CfrmDataView::ClickOpuRollZ27()	{	m_iRunEnable[ROLL_ALIGN_PROC]= RUN_DISABLE;	}
void CfrmDataView::ClickOpuRollZ25()	{	m_iRunEnable[ROLL_ALIGN_PROC]= RUN_ENABLE;	}
void CfrmDataView::ClickOpuRollZ1()		{	m_iRun_RollMode[ROLL_ALIGN_PROC]= RUN_ROLL_MODE;	}
void CfrmDataView::ClickOpuRollZ4()		{	m_iRun_RollMode[ROLL_ALIGN_PROC]= RUN_PLATE_MODE; m_iRunEnable[SYNC_PROC]= RUN_DISABLE;	}
void CfrmDataView::ClickOpuRollZ39()	{	m_iAlignMode[ROLL_ALIGN_PROC]= ALIGN_NONE_MODE;	}
void CfrmDataView::ClickOpuRollZ2()		
{
	m_btnRollPosMode_Dis.SetValue(1);		m_btnRollPosMode_En.SetValue(0);
	if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE) MyMessageBox(_T("자동모드는 쿼츠 위에서 Roll-Align동작을 지원 안합니다."), _T("Align-Mode"), M_ICONINFORMATION);
	ParamFile()->System_data.RollAlignPosMode= _OPEN_POS_MODE;
	m_iAlignMode[ROLL_ALIGN_PROC]= ALIGN_AUTO_MODE;	
/*
	if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 1)
	{
		MyMessageBox(_T("자동 Align-모드에서는 10배 렌즈를 사용 안합니다."), _T("Align-Mode"), M_ICONINFORMATION);

		//ParamFile()->System_data.iCAM_20X_USED_FLAG= 0;
		m_iAlignMode[ROLL_ALIGN_PROC]= ALIGN_MANU_MODE;	
		m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetValue(1);
	}
*/
}
void CfrmDataView::ClickOpuRollZ3()		
{
	m_iAlignMode[ROLL_ALIGN_PROC]= ALIGN_MANU_MODE;	
	if(ParamFile()->System_data.iCAM_20X_USED_FLAG== 1)
	{
		if(ParamFile()->System_data.RollAlignPosMode== _OPEN_POS_MODE)
		{
			MyMessageBox(_T("수동 10X사용 Align-모드는 쿼츠 위에서 동작 되어야 합니다."), _T("Align-Mode"), M_ICONINFORMATION);
			ParamFile()->System_data.RollAlignPosMode= _QT_POS_MODE;
			m_btnRollPosMode_Dis.SetValue(0);		m_btnRollPosMode_En.SetValue(1);
		}
	}
}
//===================================================================================
void CfrmDataView::ClickOpuRollZ29()	{	m_iRunEnable[STAGE_1_ALIGN_PROC]= RUN_DISABLE;	}
void CfrmDataView::ClickOpuRollZ31()	{	m_iRunEnable[STAGE_1_ALIGN_PROC]= RUN_ENABLE;	}
void CfrmDataView::ClickOpuRollZ37()	{	m_iAlignMode[STAGE_1_ALIGN_PROC]= ALIGN_NONE_MODE;	}
void CfrmDataView::ClickOpuRollZ33()	{	m_iAlignMode[STAGE_1_ALIGN_PROC]= ALIGN_AUTO_MODE;	}
void CfrmDataView::ClickOpuRollZ35()	{	m_iAlignMode[STAGE_1_ALIGN_PROC]= ALIGN_MANU_MODE;	}
//===================================================================================
void CfrmDataView::ClickOpuRollZ30()	{	m_iRunEnable[STAGE_2_ALIGN_PROC]= RUN_DISABLE;	}
void CfrmDataView::ClickOpuRollZ32()	{	m_iRunEnable[STAGE_2_ALIGN_PROC]= RUN_ENABLE;	}
void CfrmDataView::ClickOpuRollZ38()	{	m_iAlignMode[STAGE_2_ALIGN_PROC]= ALIGN_NONE_MODE;	}
void CfrmDataView::ClickOpuRollZ34()	{	m_iAlignMode[STAGE_2_ALIGN_PROC]= ALIGN_AUTO_MODE;	}
void CfrmDataView::ClickOpuRollZ36()	{	m_iAlignMode[STAGE_2_ALIGN_PROC]= ALIGN_MANU_MODE;	}
//===================================================================================
void CfrmDataView::ClickOpuRollZ42()	{	m_iRunEnable[STAGE_3_ALIGN_PROC]= RUN_DISABLE;	}
void CfrmDataView::ClickOpuRollZ43()	{	m_iRunEnable[STAGE_3_ALIGN_PROC]= RUN_ENABLE;	}
void CfrmDataView::ClickOpuS2AD2()		{	m_iAlignMode[STAGE_3_ALIGN_PROC]= ALIGN_NONE_MODE;	}
void CfrmDataView::ClickOpuS2AA2()		{	m_iAlignMode[STAGE_3_ALIGN_PROC]= ALIGN_AUTO_MODE;	}
void CfrmDataView::ClickOpuS2AM2()		{	m_iAlignMode[STAGE_3_ALIGN_PROC]= ALIGN_MANU_MODE;	}
//===================================================================================

void CfrmDataView::ClickDataSave()
{
	if (MyMessageBox(_T("입력된 값을 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmDataView : OnClickDataSave.! => CANCEL"));
		return;
	}

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmDataView : OnClickDataSave.!"));
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;	
	pSystem.GetMotionComponent()->CalAlignPos();
	DataSave();

	DataUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
}

//===================================================================================
void CfrmDataView::ClickOpuS1AD2()
{
	m_btnPos_Mode_S1.SetValue(1);	m_btnForce_Mode_S1.SetValue(0); m_iZ_Control_Mode[STAGE_1_ALIGN_PROC]= _POSITION_MODE;
}
void CfrmDataView::ClickOpuS1AA2()
{
	m_btnPos_Mode_S1.SetValue(0);	m_btnForce_Mode_S1.SetValue(1); m_iZ_Control_Mode[STAGE_1_ALIGN_PROC]= _FORCE_MODE;
}
void CfrmDataView::ClickOpuS1AD3()
{
	m_btnPos_Mode_S2.SetValue(1);	m_btnForce_Mode_S2.SetValue(0); m_iZ_Control_Mode[STAGE_2_ALIGN_PROC]= _POSITION_MODE;
}
void CfrmDataView::ClickOpuS1AA3()
{
	m_btnPos_Mode_S2.SetValue(0);	m_btnForce_Mode_S2.SetValue(1); m_iZ_Control_Mode[STAGE_2_ALIGN_PROC]= _FORCE_MODE;
}
void CfrmDataView::ClickOpuS1AD4()
{
	m_btnPos_Mode_S3.SetValue(1);	m_btnForce_Mode_S3.SetValue(0); m_iZ_Control_Mode[STAGE_3_ALIGN_PROC]= _POSITION_MODE;
}
void CfrmDataView::ClickOpuS1AA4()
{
	m_btnPos_Mode_S3.SetValue(0);	m_btnForce_Mode_S3.SetValue(1); m_iZ_Control_Mode[STAGE_3_ALIGN_PROC]= _FORCE_MODE;
}

void CfrmDataView::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	if(m_iForceControlEnableFlag[STAGE_1_ALIGN_PROC])	ParamFile()->Model_data.iForceControlEnableFlag[STAGE_1_ALIGN_PROC]= 1;
	else												ParamFile()->Model_data.iForceControlEnableFlag[STAGE_1_ALIGN_PROC]= 0;
}
void CfrmDataView::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	if(m_iForceControlEnableFlag[STAGE_2_ALIGN_PROC])	ParamFile()->Model_data.iForceControlEnableFlag[STAGE_2_ALIGN_PROC]= 1;
	else												ParamFile()->Model_data.iForceControlEnableFlag[STAGE_2_ALIGN_PROC]= 0;
}
void CfrmDataView::OnBnClickedCheck4()
{
	UpdateData(TRUE);
	if(m_iForceControlEnableFlag[STAGE_3_ALIGN_PROC])	ParamFile()->Model_data.iForceControlEnableFlag[STAGE_3_ALIGN_PROC]= 1;
	else												ParamFile()->Model_data.iForceControlEnableFlag[STAGE_3_ALIGN_PROC]= 0;
}

//===================================================================================
void CfrmDataView::OnBnClickedButton2()
{
	;
}

void CfrmDataView::ClickOpuRollZ52()
{
	m_btnRollPosMode_Dis.SetValue(1);		m_btnRollPosMode_En.SetValue(0);

// 	if(ParamFile()->System_data.iCAM_20X_USED_FLAG)
// 	{//
// 		MyMessageBox(_T("수동 10X사용 Align-모드는 쿼츠 위에서 동작 되어야 합니다."), _T("Align-Mode"), M_ICONINFORMATION);
// 		ParamFile()->System_data.RollAlignPosMode= _QT_POS_MODE;
// 		m_btnRollPosMode_Dis.SetValue(0);		m_btnRollPosMode_En.SetValue(1);
// 	}
// 	else
		ParamFile()->System_data.RollAlignPosMode= _OPEN_POS_MODE;
}


void CfrmDataView::ClickOpuRollZ53()
{
	m_btnRollPosMode_Dis.SetValue(0);		m_btnRollPosMode_En.SetValue(1);
	ParamFile()->System_data.RollAlignPosMode= _QT_POS_MODE;

	if(m_iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
	{
			MyMessageBox(_T("쿼츠 위에서 동작은 수동모드로 되어야 합니다."), _T("Align-Mode"), M_ICONINFORMATION);
			m_iAlignMode[ROLL_ALIGN_PROC]= ALIGN_MANU_MODE;	
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetValue(1);
	}
	m_iRunEnable[STAGE_1_ALIGN_PROC]= RUN_DISABLE;
	m_iRunEnable[STAGE_2_ALIGN_PROC]= RUN_DISABLE;
	m_iRunEnable[STAGE_3_ALIGN_PROC]= RUN_ENABLE;
	m_lblRunEnable[STAGE_1_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_1_ALIGN_PROC].SetValue(1);
	m_lblRunEnable[STAGE_2_ALIGN_PROC].SetValue(0);	m_lblRunDisable[STAGE_2_ALIGN_PROC].SetValue(1);
	m_lblRunEnable[STAGE_3_ALIGN_PROC].SetValue(1);	m_lblRunDisable[STAGE_3_ALIGN_PROC].SetValue(0);
	m_iAlignMode[STAGE_3_ALIGN_PROC]= ALIGN_MANU_MODE;
	m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetValue(0);	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetValue(1);
}
