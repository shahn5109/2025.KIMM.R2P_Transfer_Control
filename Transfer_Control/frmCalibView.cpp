// frmCalibView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmCalibView.h"
#include "MainFrm.h"
#include "dlgData_MotionSpeed.h"
#include "dlgPlateStamp_Jog.h"
#include "dlgXZ12_Mat.h"


// CfrmCalibView
extern CSystemClass pSystem;
CParameterFile* CfrmCalibView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmCalibView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmCalibView, CFormView)

	CfrmCalibView::CfrmCalibView()
	: CFormView(CfrmCalibView::IDD)
	, m_dTmp(0)
	, m_dPtpMove_mm(0)
	, m_dTopCam20To2_X(0)
	, m_dTopCam20To2_Z(0)
	, m_dBotCam20To2_X(0)
	, m_dBotCam20To2_Z(0)
	, m_dRotCen_PosCamY_20X(0)
	, m_dRotCen_PosStageX_20X(0)
	, m_dTopCam20To2_Y(0)
	, m_dBotCam20To2_Y(0)
	, m_dStartX1(189.3)
	, m_dEndX(835)
	, m_dStartY(238.6)
	, m_dEndY(918)
	, m_dAngle(0.5)
	, m_dStage1ToStage2_Dist(0)
	, m_dStage2ToStage3_Dist(0)
	, m_dQuartz_PosX(0)
	, m_dQuartz_PosY(0)
	, m_dRollPlateStamp_PosX(0)
	, m_dRollPlateStamp_PosY(0)
	, m_dBot_Cam_20X_Offset_X(0)
	, m_dBot_Cam_20X_Offset_Y(0)
	, m_dStampMoveOffset_X(0)
{
	proc_run_flag= false;
}

CfrmCalibView::~CfrmCalibView()
{
}

void CfrmCalibView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
	DDX_Control(pDX, IDB_Y_POS0, m_btnReady);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL3, m_btnStageSel1);

#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	DDX_Control(pDX, IDB_OPU_CS0,			m_btnCopySel[0]);
	DDX_Control(pDX, IDB_OPU_CS1,			m_btnCopySel[1]);
	DDX_Control(pDX, IDB_OPU_CS2,			m_btnCopySel[2]);
	DDX_Control(pDX, IDB_OPU_CS3,			m_btnCopySel[3]);
	DDX_Control(pDX, IDB_OPU_CS4,			m_btnCopySel[4]);
	DDX_Control(pDX, IDB_OPU_CS5,			m_btnCopySel[5]);
	DDX_Control(pDX, IDB_OPU_CS6,			m_btnCopySel[6]);
	DDX_Control(pDX, IDB_OPU_CS7,			m_btnCopySel[7]);
	DDX_Control(pDX, IDB_OPU_CS8,			m_btnCopySel[8]);
	DDX_Control(pDX, IDB_OPU_CS9,			m_btnCopySel[9]);
	DDX_Control(pDX, IDB_OPU_CS10,			m_btnCopySel[10]);
	DDX_Control(pDX, IDB_OPU_CS11,			m_btnCopySel[11]);
	DDX_Control(pDX, IDB_OPU_CS12,			m_btnCopySel[12]);
	DDX_Control(pDX, IDB_OPU_CS13,			m_btnCopySel[13]);
	DDX_Control(pDX, IDB_OPU_CS14,			m_btnCopySel[14]);
	DDX_Control(pDX, IDB_OPU_CS15,			m_btnCopySel[15]);

	DDX_Control(pDX,  IDC_CURPOS_C0,  m_cCurPos[0]);
	DDX_Control(pDX,  IDC_CURPOS_C1,  m_cCurPos[1]);
	DDX_Control(pDX,  IDC_CURPOS_C2,  m_cCurPos[2]);
	DDX_Control(pDX,  IDC_CURPOS_C3,  m_cCurPos[3]);
	DDX_Control(pDX,  IDC_CURPOS_C4,  m_cCurPos[4]);
	DDX_Control(pDX,  IDC_CURPOS_C5,  m_cCurPos[5]);
	DDX_Control(pDX,  IDC_CURPOS_C6,  m_cCurPos[6]);
	DDX_Control(pDX,  IDC_CURPOS_C7,  m_cCurPos[7]);
	DDX_Control(pDX,  IDC_CURPOS_C8,  m_cCurPos[8]);
	DDX_Control(pDX,  IDC_CURPOS_C9,  m_cCurPos[9]);
	DDX_Control(pDX,  IDC_CURPOS_C10, m_cCurPos[10]);
	DDX_Control(pDX,  IDC_CURPOS_C11, m_cCurPos[11]);
	DDX_Control(pDX,  IDC_CURPOS_C12, m_cCurPos[12]);
	DDX_Control(pDX,  IDC_CURPOS_C13, m_cCurPos[13]);
	DDX_Control(pDX,  IDC_CURPOS_C14, m_cCurPos[14]);
	DDX_Control(pDX,  IDC_CURPOS_C15, m_cCurPos[15]);

	DDX_Text(pDX,  IDC_EDIT_0,  m_dPosition[0]);
	DDX_Text(pDX,  IDC_EDIT_1,  m_dPosition[1]);
	DDX_Text(pDX,  IDC_EDIT_2,  m_dPosition[2]);
	DDX_Text(pDX,  IDC_EDIT_3,  m_dPosition[3]);
	DDX_Text(pDX,  IDC_EDIT_4,  m_dPosition[4]);
	DDX_Text(pDX,  IDC_EDIT_5,  m_dPosition[5]);
	DDX_Text(pDX,  IDC_EDIT_6,  m_dPosition[6]);
	DDX_Text(pDX,  IDC_EDIT_7,  m_dPosition[7]);
	DDX_Text(pDX,  IDC_EDIT_8,  m_dPosition[8]);
	DDX_Text(pDX,  IDC_EDIT_9,  m_dPosition[9]);
	DDX_Text(pDX,  IDC_EDIT_10, m_dPosition[10]);
	DDX_Text(pDX,  IDC_EDIT_11, m_dPosition[11]);
	DDX_Text(pDX,  IDC_EDIT_12, m_dPosition[12]);
	DDX_Text(pDX,  IDC_EDIT_13, m_dPosition[13]);
	DDX_Text(pDX,  IDC_EDIT_14, m_dPosition[14]);
	DDX_Text(pDX,  IDC_EDIT_15, m_dPosition[15]);

	DDX_Text(pDX,  IDC_EDIT_OFFSET_0,  m_dPosition_Offset[0]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_1,  m_dPosition_Offset[1]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_2,  m_dPosition_Offset[2]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_3,  m_dPosition_Offset[3]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_4,  m_dPosition_Offset[4]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_5,  m_dPosition_Offset[5]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_6,  m_dPosition_Offset[6]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_7,  m_dPosition_Offset[7]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_8,  m_dPosition_Offset[8]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_9,  m_dPosition_Offset[9]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_10, m_dPosition_Offset[10]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_11, m_dPosition_Offset[11]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_12, m_dPosition_Offset[12]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_13, m_dPosition_Offset[13]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_14, m_dPosition_Offset[14]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_15, m_dPosition_Offset[15]);
#else
	DDX_Control(pDX, IDB_OPU_CS0,			m_btnCopySel[0]);
	DDX_Control(pDX, IDB_OPU_CS1,			m_btnCopySel[1]);
	DDX_Control(pDX, IDB_OPU_CS2,			m_btnCopySel[2]);
	DDX_Control(pDX, IDB_OPU_CS3,			m_btnCopySel[3]);
	DDX_Control(pDX, IDB_OPU_CS4,			m_btnCopySel[8]);
	DDX_Control(pDX, IDB_OPU_CS5,			m_btnCopySel[9]);
	DDX_Control(pDX, IDB_OPU_CS6,			m_btnCopySel[14]);
	DDX_Control(pDX, IDB_OPU_CS7,			m_btnCopySel[15]);
	DDX_Control(pDX, IDB_OPU_CS8,			m_btnCopySel[4]);
	DDX_Control(pDX, IDB_OPU_CS9,			m_btnCopySel[5]);
	DDX_Control(pDX, IDB_OPU_CS10,			m_btnCopySel[6]);
	DDX_Control(pDX, IDB_OPU_CS11,			m_btnCopySel[7]);
	DDX_Control(pDX, IDB_OPU_CS12,			m_btnCopySel[10]);
	DDX_Control(pDX, IDB_OPU_CS13,			m_btnCopySel[11]);
	DDX_Control(pDX, IDB_OPU_CS14,			m_btnCopySel[12]);
	DDX_Control(pDX, IDB_OPU_CS15,			m_btnCopySel[13]);

	DDX_Control(pDX,  IDC_CURPOS_C0,  m_cCurPos[0]);
	DDX_Control(pDX,  IDC_CURPOS_C1,  m_cCurPos[1]);
	DDX_Control(pDX,  IDC_CURPOS_C2,  m_cCurPos[2]);
	DDX_Control(pDX,  IDC_CURPOS_C3,  m_cCurPos[3]);
	DDX_Control(pDX,  IDC_CURPOS_C4,  m_cCurPos[8]);
	DDX_Control(pDX,  IDC_CURPOS_C5,  m_cCurPos[9]);
	DDX_Control(pDX,  IDC_CURPOS_C6,  m_cCurPos[14]);
	DDX_Control(pDX,  IDC_CURPOS_C7,  m_cCurPos[15]);
	DDX_Control(pDX,  IDC_CURPOS_C8,  m_cCurPos[4]);
	DDX_Control(pDX,  IDC_CURPOS_C9,  m_cCurPos[5]);
	DDX_Control(pDX,  IDC_CURPOS_C10, m_cCurPos[6]);
	DDX_Control(pDX,  IDC_CURPOS_C11, m_cCurPos[7]);
	DDX_Control(pDX,  IDC_CURPOS_C12, m_cCurPos[10]);
	DDX_Control(pDX,  IDC_CURPOS_C13, m_cCurPos[11]);
	DDX_Control(pDX,  IDC_CURPOS_C14, m_cCurPos[12]);
	DDX_Control(pDX,  IDC_CURPOS_C15, m_cCurPos[13]);

	DDX_Text(pDX,  IDC_EDIT_0,  m_dPosition[0]);
	DDX_Text(pDX,  IDC_EDIT_1,  m_dPosition[1]);
	DDX_Text(pDX,  IDC_EDIT_2,  m_dPosition[2]);
	DDX_Text(pDX,  IDC_EDIT_3,  m_dPosition[3]);
	DDX_Text(pDX,  IDC_EDIT_4,  m_dPosition[8]);
	DDX_Text(pDX,  IDC_EDIT_5,  m_dPosition[9]);
	DDX_Text(pDX,  IDC_EDIT_6,  m_dPosition[14]);
	DDX_Text(pDX,  IDC_EDIT_7,  m_dPosition[15]);
	DDX_Text(pDX,  IDC_EDIT_8,  m_dPosition[4]);
	DDX_Text(pDX,  IDC_EDIT_9,  m_dPosition[5]);
	DDX_Text(pDX,  IDC_EDIT_10, m_dPosition[6]);
	DDX_Text(pDX,  IDC_EDIT_11, m_dPosition[7]);
	DDX_Text(pDX,  IDC_EDIT_12, m_dPosition[10]);
	DDX_Text(pDX,  IDC_EDIT_13, m_dPosition[11]);
	DDX_Text(pDX,  IDC_EDIT_14, m_dPosition[12]);
	DDX_Text(pDX,  IDC_EDIT_15, m_dPosition[13]);

	DDX_Text(pDX,  IDC_EDIT_OFFSET_0,  m_dPosition_Offset[0]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_1,  m_dPosition_Offset[1]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_2,  m_dPosition_Offset[2]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_3,  m_dPosition_Offset[3]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_4,  m_dPosition_Offset[8]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_5,  m_dPosition_Offset[9]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_6,  m_dPosition_Offset[14]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_7,  m_dPosition_Offset[15]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_8,  m_dPosition_Offset[4]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_9,  m_dPosition_Offset[5]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_10, m_dPosition_Offset[6]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_11, m_dPosition_Offset[7]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_12, m_dPosition_Offset[10]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_13, m_dPosition_Offset[11]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_14, m_dPosition_Offset[12]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_15, m_dPosition_Offset[13]);
#endif
	DDX_Text(pDX, IDC_EDIT1, m_dPtpMove_mm);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y3, m_dTopCam20To2_X);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y10, m_dTopCam20To2_Z);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y11, m_dBotCam20To2_X);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y12, m_dBotCam20To2_Z);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y2, m_dRotCen_PosCamY_20X);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y4, m_dRotCen_PosStageX_20X);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y5, m_dTopCam20To2_Y);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y6, m_dBotCam20To2_Y);
	DDX_Text(pDX, IDC_EDIT3, m_dStartX1);
	DDX_Text(pDX, IDC_EDIT4, m_dEndX);
	DDX_Text(pDX, IDC_EDIT5, m_dStartY);
	DDX_Text(pDX, IDC_EDIT6, m_dEndY);
	DDX_Text(pDX, IDC_EDIT7, m_dAngle);

	DDX_Control(pDX, IDB_STAGE_JOG_SEL5, m_btnZ_InterLock);

	DDX_Text(pDX, IDC_EDIT_INK_START_Y7, m_dStage1ToStage2_Dist);
	DDX_Text(pDX, IDC_EDIT_INK_START_Y8, m_dStage2ToStage3_Dist);
	DDX_Text(pDX, IDC_EDIT_OFFSET_43, m_dQuartz_PosX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_44, m_dQuartz_PosY);
	DDX_Text(pDX, IDC_EDIT_OFFSET_46, m_dRollPlateStamp_PosX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_47, m_dRollPlateStamp_PosY);
	DDX_Text(pDX, IDC_EDIT_OFFSET_50, m_dBot_Cam_20X_Offset_X);
	DDX_Text(pDX, IDC_EDIT_OFFSET_51, m_dBot_Cam_20X_Offset_Y);
	DDX_Text(pDX, IDC_EDIT11, m_dStampMoveOffset_X);
}

BEGIN_MESSAGE_MAP(CfrmCalibView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmCalibView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CfrmCalibView::OnBnClickedButton3)
	ON_MESSAGE(WM_MSG_DATA_UPDATE, DataReload)
END_MESSAGE_MAP()


// CfrmCalibView 진단입니다.

#ifdef _DEBUG
void CfrmCalibView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmCalibView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmCalibView 메시지 처리기입니다.

//===================================================================================
void CfrmCalibView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_CalibView(this);
	m_pVision= pSystem.GetVisionComponent();

	Pos_Sel= POSITION_READY;
	m_btnReady.SetValue(1);

	iStageJog_Select= STAGE_SEL_1;
	m_btnStageSel1.SetValue(1);

//	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("Stage -"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("Stage +"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("상부 Vision\nStage Z-"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("상부 Vision\nStage Z+"));

	Jog_Mode= _MID_SPEED;
	m_btnJogModeSel1.SetValue(1);

	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
		m_btnCopySel[i].SetValue(1);
	}

	SetTimer(frmTeachView_TIMER_ID, frmTeachView_TIMER_INTERVAL, NULL);

	DataUpdate();

	g_bCfrmCalibViewInit= TRUE;

}

//===================================================================================
void CfrmCalibView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmTeachView_TIMER_ID);

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmCalibView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmCalibView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmCalibView::OnTimer(UINT_PTR nIDEvent)
{
	CString strText;
	double cur_pos= 0.0;
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_STAGE_X]);		SetDlgItemText(IDC_CURPOS_C0, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL]);		SetDlgItemText(IDC_CURPOS_C1, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_UPPER_CAM_Y]);	SetDlgItemText(IDC_CURPOS_C8, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_UPPER_CAM_Z]);	SetDlgItemText(IDC_CURPOS_C4, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z1]);	SetDlgItemText(IDC_CURPOS_C14, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z2]);	SetDlgItemText(IDC_CURPOS_C15, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_ALIGN_Y1]);		SetDlgItemText(IDC_CURPOS_C12, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_ALIGN_Y2]);		SetDlgItemText(IDC_CURPOS_C13, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_X]);	SetDlgItemText(IDC_CURPOS_C9, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_Y]);	SetDlgItemText(IDC_CURPOS_C10, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_Z]);	SetDlgItemText(IDC_CURPOS_C11, strText.GetBuffer(0));

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
LRESULT CfrmCalibView::DataReload(WPARAM wParam,LPARAM lParam)
{
	DataUpdate();
	return 0;
}
//===================================================================================
void CfrmCalibView::DataUpdate()
	//-----------------------------------------------------------------------------------
{
	CString strTmp;

	DispUpdate();

	//	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
// 	ParamFile()->OpenModelConfigFile(ParamFile()->System_data.strCurrentModelName);
// 	ParamFile()->OpenSytemConfigFile();

	m_dStage1ToStage2_Dist= ParamFile()->System_data.dStage1ToStage2_Dist;
	m_dStage2ToStage3_Dist= ParamFile()->System_data.dStage2ToStage3_Dist;

	m_dTopCam20To2_X = ParamFile()->System_data.dTopCam20To2_X;
	m_dTopCam20To2_Y = ParamFile()->System_data.dTopCam20To2_Y;
	m_dTopCam20To2_Z = ParamFile()->System_data.dTopCam20To2_Z;
	m_dBotCam20To2_X = ParamFile()->System_data.dBotCam20To2_X;
	m_dBotCam20To2_Y = ParamFile()->System_data.dBotCam20To2_Y;
	m_dBotCam20To2_Z = ParamFile()->System_data.dBotCam20To2_Z;
	m_dRotCen_PosStageX_20X = ParamFile()->System_data.dRotCen_PosStageX_20X;
	m_dRotCen_PosCamY_20X = ParamFile()->System_data.dRotCen_PosCamY_20X;

	m_dRollPlateStamp_PosX= ParamFile()->System_data.dRollPlateStamp_PosX;
	m_dRollPlateStamp_PosY= ParamFile()->System_data.dRollPlateStamp_PosY;
	m_dQuartz_PosX= ParamFile()->System_data.dQuartz_PosX;
	m_dQuartz_PosY= ParamFile()->System_data.dQuartz_PosY;
	m_dStampMoveOffset_X= ParamFile()->System_data.dStampMoveOffset_X;
	m_dBot_Cam_20X_Offset_X= ParamFile()->System_data.dBot_Cam_20X_Offset_X;
	m_dBot_Cam_20X_Offset_Y= ParamFile()->System_data.dBot_Cam_20X_Offset_Y;
	
	UpdateData(FALSE);


	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;

	int i;

	switch(dispPos_Sel)
	{
	case POSITION_READY:	
		break;
	case POSITION_STAGE1_ALIGN:	
	case POSITION_STAGE2_ALIGN:	
	case POSITION_STAGE3_ALIGN:	
	case POSITION_ROLL_ALIGN:	
	case POSITION_SYNC:	
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
		break;
	case POSITION_TOP_MASK:
	case POSITION_TOP_MASK_20X:
		dispPos_Sel= POSITION_TOP_MASK;
		break;
	case POSITION_BTM_MASK:
	case POSITION_BTM_MASK_20X:
		dispPos_Sel= POSITION_BTM_MASK;
		break;
	default:
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
		break;
	}
	for(i= 0; i< _MAX_AXIS; i++)
	{
		ParamFile()->Model_data.dPosition[dispPos_Sel][i]=
			pSystem.GetMotionComponent()->PosCheckSWLimit(ParamFile()->Model_data.dPosition[dispPos_Sel][i], i);
		ParamFile()->Model_data.dPosition_Offset[dispPos_Sel][i]= 
			pSystem.GetMotionComponent()->PosOffsetCheckSWLimit(ParamFile()->Model_data.dPosition[dispPos_Sel][i], i, 
			ParamFile()->Model_data.dPosition_Offset[dispPos_Sel][i]);

		m_dPosition[i]= ParamFile()->Model_data.dPosition[dispPos_Sel][i];
		m_dPosition_Offset[i]= ParamFile()->Model_data.dPosition_Offset[dispPos_Sel][i];

	}

	UpdateData(FALSE);
}
//===================================================================================
void CfrmCalibView::DataSave()
//-----------------------------------------------------------------------------------
{
	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
	UpdateData(TRUE);

	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;
	
	ParamFile()->System_data.dStage1ToStage2_Dist = m_dStage1ToStage2_Dist;
	ParamFile()->System_data.dStage2ToStage3_Dist = m_dStage2ToStage3_Dist;

	ParamFile()->System_data.dTopCam20To2_X = m_dTopCam20To2_X;
	ParamFile()->System_data.dTopCam20To2_Y = m_dTopCam20To2_Y;
	ParamFile()->System_data.dTopCam20To2_Z = m_dTopCam20To2_Z;
	ParamFile()->System_data.dBotCam20To2_X = m_dBotCam20To2_X;
	ParamFile()->System_data.dBotCam20To2_Y = m_dBotCam20To2_Y;
	ParamFile()->System_data.dBotCam20To2_Z = m_dBotCam20To2_Z;
	ParamFile()->System_data.dRotCen_PosStageX_20X= m_dRotCen_PosStageX_20X;
	ParamFile()->System_data.dRotCen_PosCamY_20X= m_dRotCen_PosCamY_20X;

	ParamFile()->System_data.dRollPlateStamp_PosX= m_dRollPlateStamp_PosX;
	ParamFile()->System_data.dRollPlateStamp_PosY= m_dRollPlateStamp_PosY;
	ParamFile()->System_data.dQuartz_PosX= m_dQuartz_PosX;
	ParamFile()->System_data.dQuartz_PosY= m_dQuartz_PosY;

	ParamFile()->System_data.dStampMoveOffset_X= m_dStampMoveOffset_X;

	ParamFile()->System_data.dBot_Cam_20X_Offset_X= m_dBot_Cam_20X_Offset_X;
	ParamFile()->System_data.dBot_Cam_20X_Offset_Y= m_dBot_Cam_20X_Offset_Y;

	pSystem.GetMotionComponent()->CalAlignPos();
	
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveSytemConfigFile();

	switch(dispPos_Sel)
	{
	case POSITION_READY:	
		return;
		break;
	case POSITION_STAGE1_ALIGN:	
	case POSITION_STAGE2_ALIGN:	
	case POSITION_STAGE3_ALIGN:	
	case POSITION_ROLL_ALIGN:	
	case POSITION_SYNC:	
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
		break;
	case POSITION_TOP_MASK_20X:
	case POSITION_BTM_MASK_20X:
		return;
		break;
	case POSITION_TOP_MASK:	
		m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z1];
		m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z2];
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y1];
		m_dPosition[_ACS_AXIS_ALIGN_Y2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y2];
		m_dPosition[_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL];

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Y] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y])
			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];
		break;
	case POSITION_BTM_MASK:	
		m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z1];
		m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z2];
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y1];
		m_dPosition[_ACS_AXIS_ALIGN_Y2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y2];
		m_dPosition[_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL];
	
	//	if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
		m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		break;
	default:
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
	}

	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
//		if(	m_btnCopySel[i].GetValue())
//		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][i]= pSystem.GetMotionComponent()->PosCheckSWLimit(m_dPosition[i], i);
			ParamFile()->Model_data.dPosition_Offset[dispPos_Sel][i]= 
				pSystem.GetMotionComponent()->PosOffsetCheckSWLimit(m_dPosition[i], i, m_dPosition_Offset[i]);
//		}
	}


	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CfrmCalibView 수정 / 저장"));
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
}

//===================================================================================
void CfrmCalibView::DispUpdate()
//-----------------------------------------------------------------------------------
{
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]!= RUN_PLATE_MODE) //평스탬프 모드
	{
		GetDlgItem(IDB_CM_MOVE5)->EnableWindow(FALSE);
		GetDlgItem(IDB_CM_MOVE4)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_CM_MOVE5)->EnableWindow(TRUE);
		GetDlgItem(IDB_CM_MOVE4)->EnableWindow(TRUE);
	}

	switch(Pos_Sel) 
	{
		case POSITION_READY:
		case POSITION_STAGE1_ALIGN:
		case POSITION_STAGE2_ALIGN:
		case POSITION_STAGE3_ALIGN:
		case POSITION_ROLL_ALIGN:
		case POSITION_SYNC:
		case POSITION_TOP_MASK_20X:
		case POSITION_BTM_MASK_20X:
			GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_TOP_MASK:
			GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Y
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_BTM_MASK:
			GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
	}
}
//===================================================================================
void CfrmCalibView::JogDispUpdate()
//-----------------------------------------------------------------------------------
{
	switch(iStageJog_Select)
	{
	case STAGE_SEL_1:

		break;
	case STAGE_SEL_2:
		break;
	}
}


//===================================================================================
BEGIN_EVENTSINK_MAP(CfrmCalibView, CFormView)
	ON_EVENT(CfrmCalibView, IDB_Y_POS0, DISPID_CLICK, CfrmCalibView::ClickYPos0, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Y_POS8, DISPID_CLICK, CfrmCalibView::ClickYPos8, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Y_POS1, DISPID_CLICK, CfrmCalibView::ClickYPos1, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Y_POS9, DISPID_CLICK, CfrmCalibView::ClickYPos9, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Y_POS10, DISPID_CLICK, CfrmCalibView::ClickYPos10, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDC_BUTTON_JOG_POPUP, DISPID_CLICK, CfrmCalibView::ClickButtonJogPopup, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_STAGE_JOG_SEL3, DISPID_CLICK, CfrmCalibView::ClickStageJogSel1, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_STAGE_JOG_SEL4, DISPID_CLICK, CfrmCalibView::ClickStageJogSel2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_SELECT_0, DISPID_CLICK, CfrmCalibView::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_SELECT_1, DISPID_CLICK, CfrmCalibView::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_SELECT_2, DISPID_CLICK, CfrmCalibView::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_SELECT_3, DISPID_CLICK, CfrmCalibView::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_POS_SET2, DISPID_CLICK, CfrmCalibView::ClickPosSet2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CM_MOVE, DISPID_CLICK, CfrmCalibView::ClickCmMove, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CM_MOVE3, DISPID_CLICK, CfrmCalibView::ClickCmMove2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Z_WAIT_POS2, DISPID_CLICK, CfrmCalibView::ClickZWaitPos, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Z_WAIT_POS7, DISPID_CLICK, CfrmCalibView::ClickZWaitPos7, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CM_MANUAL_SPEED, DISPID_CLICK, CfrmCalibView::ClickCmManualSpeed, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CM_MANUAL_SPEED2, DISPID_CLICK, CfrmCalibView::ClickCmManualSpeed2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_DATA_SAVE, DISPID_CLICK, CfrmCalibView::ClickDataSave, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_PB_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_PB_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_PB_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_PB_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z1_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z1_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z2_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z2_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z2_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z2_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z1_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_Z1_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_VACUUM_STAGE1_ON, DISPID_CLICK, CfrmCalibView::ClickVacuumStage1On, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_VACUUM_STAGE1_OFF, DISPID_CLICK, CfrmCalibView::ClickVacuumStage1Off, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_VACUUM_STAGE2_ON, DISPID_CLICK, CfrmCalibView::ClickVacuumStage2On, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_VACUUM_STAGE2_OFF, DISPID_CLICK, CfrmCalibView::ClickVacuumStage2Off, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CAL_1, DISPID_CLICK, CfrmCalibView::ClickCal1, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_Z_WAIT_POS4, DISPID_CLICK, CfrmCalibView::ClickZWaitPos4, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_STAGE_JOG_SEL5, DISPID_CLICK, CfrmCalibView::ClickStageJogSel5, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CW2, DISPID_CLICK, CfrmCalibView::ClickJogXCw2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CW2, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogXCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CW2, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogXCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CCW2, DISPID_MOUSEDOWN, CfrmCalibView::MouseDownJogXCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_JOG_X_CCW2, DISPID_MOUSEUP, CfrmCalibView::MouseUpJogXCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmCalibView, IDB_CM_MOVE5, DISPID_CLICK, CfrmCalibView::ClickCmMove5, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CM_MOVE4, DISPID_CLICK, CfrmCalibView::ClickCmMove4, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CAL_2, DISPID_CLICK, CfrmCalibView::ClickCal2, VTS_NONE)
	ON_EVENT(CfrmCalibView, IDB_CAL_3, DISPID_CLICK, CfrmCalibView::ClickCal3, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CfrmCalibView::ClickYPos0()
{	Pos_Sel= POSITION_READY;					DataUpdate(); }
//===================================================================================
void CfrmCalibView::ClickYPos8()
{	Pos_Sel= POSITION_TOP_MASK;					DataUpdate(); }
//===================================================================================
void CfrmCalibView::ClickYPos1()
{	Pos_Sel= POSITION_TOP_MASK_20X;				DataUpdate(); }
//===================================================================================
void CfrmCalibView::ClickYPos9()
{	Pos_Sel= POSITION_BTM_MASK;					DataUpdate(); }
//===================================================================================
void CfrmCalibView::ClickYPos10()
{	Pos_Sel= POSITION_BTM_MASK_20X;				DataUpdate(); }
//===================================================================================
void CfrmCalibView::ClickButtonJogPopup()
{
	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
		m_btnCopySel[i].SetValue(1);
	}
}
//===================================================================================
void CfrmCalibView::ClickStageJogSel1()
{
	iStageJog_Select= STAGE_SEL_1;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("Stage -"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("Stage +"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("상부 Vision\nStage Z-"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("상부 Vision\nStage Z+"));
}
//===================================================================================

void CfrmCalibView::ClickStageJogSel2()
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
void CfrmCalibView::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmCalibView::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmCalibView::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmCalibView::ClickJogSelect3()
{
	Jog_Mode= _PTP_MOVE;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}

//===================================================================================
void CfrmCalibView::ClickPosSet2()
{
	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;

	CString strText, strTitle;
	CString strPos;
	double ToPosition= 0.0;
	strText= _T("");	strTitle= _T("");
	double UpData[_MAX_AXIS];
	int i;

	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;

	switch(dispPos_Sel)
	{
	case POSITION_READY:	
	case POSITION_STAGE1_ALIGN:	
	case POSITION_STAGE2_ALIGN:	
	case POSITION_STAGE3_ALIGN:	
	case POSITION_ROLL_ALIGN:	
	case POSITION_SYNC:	
	case POSITION_TOP_MASK_20X:
	case POSITION_BTM_MASK_20X:
		return;
		break;
	case POSITION_TOP_MASK:
	case POSITION_BTM_MASK:
		break;
	default:
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
		break;
	}

	for(i= 0; i< _MAX_AXIS; i++)	UpData[i]= 0.0;

	UpData[_ACS_AXIS_IMP_ROLL_Z1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	UpData[_ACS_AXIS_IMP_ROLL_Z2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z2);

	UpData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	UpData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
	UpData[_ACS_AXIS_UPPER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
	UpData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
	UpData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
	UpData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	UpData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
	UpData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
	UpData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);

	strTitle.Format(_T("현재 위치로 값을 변경하시겠습니까?")); strText= strText + strTitle; 
	if (MyMessageBox(strText.GetBuffer(0), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[현재 위치로 값을 변경] => [전체] - 현재 위치 변경. => CANCEL"));
		return;
	}

	for(i= 0; i< _MAX_AXIS; i++)
	{
		if(	m_btnCopySel[i].GetValue())		{			ParamFile()->Model_data.dPosition[dispPos_Sel][i]= UpData[i];		}
	}
	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z1];
	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z2];
	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]=	ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y1];
	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]=	ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y2];
	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]=	ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL];
	
	switch(dispPos_Sel)
	{
	case POSITION_READY:	
		return;
		break;
	case POSITION_STAGE1_ALIGN:	
	case POSITION_STAGE2_ALIGN:	
	case POSITION_STAGE3_ALIGN:	
	case POSITION_ROLL_ALIGN:	
	case POSITION_SYNC:	
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
		break;
	case POSITION_TOP_MASK:	
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= UpData[_ACS_AXIS_LOWER_CAM_Z];
	break;
	case POSITION_TOP_MASK_20X:	
		return;
		break;
	case POSITION_BTM_MASK:	
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= UpData[_ACS_AXIS_LOWER_CAM_Z];
		break;
	case POSITION_BTM_MASK_20X:	
		return;
		break;
	default:
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
	}
	ParamFile()->SaveFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadFile();

	DataUpdate();
}

//===================================================================================
void CfrmCalibView::ClickCmMove()
{
	MovePos(Pos_Sel, TRUE, TRUE);
}

BOOL CfrmCalibView::MovePos(int pos, BOOL Z_Flag, BOOL dispFlag)
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;
	int dispPos_Sel;
	dispPos_Sel= pos;

	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
	}

	if(dispFlag)
	{
		switch(dispPos_Sel) 
		{
		case POSITION_STAGE1_ALIGN:
		case POSITION_STAGE2_ALIGN:
		case POSITION_STAGE3_ALIGN:
		case POSITION_ROLL_ALIGN:
		case POSITION_SYNC:
			MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return  FALSE;
			break;
		case POSITION_READY:
		case POSITION_TOP_MASK:
		case POSITION_BTM_MASK:
			strPos.Format(_T("SX= %.3f:PB= %.3f:Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f]"),	
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel));
			break;
		case POSITION_TOP_MASK_20X:
			strPos.Format(_T("SX= %.3f:PB= %.3f:Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f]"),	
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_X,												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_TOP_MASK),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_TOP_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_TOP_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_TOP_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_TOP_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_TOP_MASK));
			break;
		case POSITION_BTM_MASK_20X:
			strPos.Format(_T("SX= %.3f:PB= %.3f:Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f]"),	
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK)+ParamFile()->System_data.dBotCam20To2_X,												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_BTM_MASK),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_BTM_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_BTM_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK));
			break;
		default:
			MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return  FALSE;
			break;
		}
		//-----------------
		strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return FALSE;
		//-----------------
	}
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);

	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//-----------------
	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X, dispPos_Sel)) 
	{
		// 		strText.Format("Z축이 대기 위치보다 아래로 내려와 있습니다.\n선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s", strPos);
		// 		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONERROR|M_YESNO, _T("예"), _T("아니오")) == IDNO)
		// 		{
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed[Safe_PosCheck]"), M_ICONERROR); return FALSE;
		return FALSE;
		//		}
	}

	switch(dispPos_Sel) 
	{
	case POSITION_READY:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		break;
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:
	case POSITION_ROLL_ALIGN:
	case POSITION_SYNC:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE ;
		break;
	case POSITION_TOP_MASK:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		break;
	case POSITION_TOP_MASK_20X:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_X;
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_Z;
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_Y;
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		break;
	case POSITION_BTM_MASK:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------------------
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------
		if(Z_Flag)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		}

		break;
	case POSITION_BTM_MASK_20X:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_X;
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------------------
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_Y;
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------
		if(Z_Flag)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_Z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		}

		break;
	default:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return  FALSE;
		break;
	}
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	return TRUE;
}
//===================================================================================
void CfrmCalibView::ClickCmMove2()//Z-Wait & All-Move
{
	MovePos(Pos_Sel, FALSE, TRUE);
}

//===================================================================================
void CfrmCalibView::ClickZWaitPos() // Z-Up
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;
	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;


	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}
	//-----------------
	ToPosition1	= 0.0;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, Pos_Sel);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	//-----------------
	double X_CurPos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	double chaval= 0.0;
	switch(dispPos_Sel) 
	{
	case POSITION_READY:
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:
	case POSITION_SYNC:
	case POSITION_TOP_MASK:
	case POSITION_TOP_MASK_20X:
		MyMessageBox(_T("선택 위치에서는 하부 Z축을사용 안합니다.\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	case POSITION_ROLL_ALIGN:
		chaval= fabs(X_CurPos - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_ROLL_ALIGN));
		if(chaval > 3.0)
		{
			MyMessageBox(_T("이동이 실패했습니다.\nX축 위치가 설정 위치를 벗어나 있습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		}
		strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_ROLL_ALIGN),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_ROLL_ALIGN),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_ROLL_ALIGN));
		break;
	case POSITION_BTM_MASK_20X:
		chaval= fabs(X_CurPos - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK)- ParamFile()->System_data.dBotCam20To2_X);
		if(chaval > 3.0)
		{
			MyMessageBox(_T("이동이 실패했습니다.\nX축 위치가 설정 위치를 벗어나 있습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		}
		strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK));
		break;
	case POSITION_BTM_MASK:
		chaval= fabs(X_CurPos - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK));
		if(chaval > 3.0)
		{
			MyMessageBox(_T("이동이 실패했습니다.\nX축 위치가 설정 위치를 벗어나 있습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		}
		strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK));
		break;
	default:
		MyMessageBox(_T("선택 위치에서는 하부 Z축을사용 안합니다.\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel)) 
	{
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
		return;
	}
	else 
	{
		strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
	}

	switch(dispPos_Sel) 
	{
	case POSITION_READY:
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:
	case POSITION_TOP_MASK:
		MyMessageBox(_T("선택 위치에서는 하부 Z축을사용 안합니다.\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	case POSITION_ROLL_ALIGN:
		X_CurPos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		if(X_CurPos >= ParamFile()->System_data.X_SAFE_FREEZONE_H)
		{
			MyMessageBox(_T("이동이 실패했습니다. Roll-Align 위치 에러!\nX축 위치가 설정 위치를 벗어나 있습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		}
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, Pos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		//-----------------
		break;
	case POSITION_BTM_MASK:
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		//-----------------
		break;
	case POSITION_BTM_MASK_20X:
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_Z;
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		//-----------------
		break;
	default:
		MyMessageBox(_T("선택 위치에서는 하부 Z축을사용 안합니다.\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}

//===================================================================================
void CfrmCalibView::ClickZWaitPos7() // Z-Down
{//하강
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;


	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}

	strPos.Format(_T("BZ=%.3f"),	
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY));

	strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;

	//-----------------
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}

//===================================================================================
void CfrmCalibView::ClickCmManualSpeed()
{
//	CdlgData_MotionSpeed dlg;
	//dlg.DoModal();
}

//===================================================================================
void CfrmCalibView::ClickCmManualSpeed2()
{
	CdlgData_MotionSpeed dlg;
	dlg.DoModal();
}

//===================================================================================
void CfrmCalibView::ClickDataSave()
{
	if (MyMessageBox(_T("입력된 값을 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgTeach_Stage2 : OnClickDataSave.! => CANCEL"));
		return;
	}
	UpdateData(TRUE);
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgTeach_Stage2 : OnClickDataSave.!"));
	if( m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1] > ParamFile()->Motion_data.dZ_Axis_Interlock ||
		m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2] > ParamFile()->Motion_data.dZ_Axis_Interlock)
	{
		MyMessageBox(_T("모터 설정이 실패했습니다.\nZ축 설정 위치 확인!!! [MAX]"), _T("Position-Move Failed"), M_ICONERROR);
	}
	else	DataSave();

	DataUpdate();
}
//===================================================================================
//===================================================================================
void CfrmCalibView::MouseDownJogXCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_X_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
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
void CfrmCalibView::MouseUpJogXCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================


void CfrmCalibView::MouseDownJogXCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_X_CCW)->GetWindowRect(&disp_rect);
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


void CfrmCalibView::MouseUpJogXCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================
void CfrmCalibView::MouseDownJogPbCw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_PB_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL, _DIR_CW, SetSpeed);
	}
}


void CfrmCalibView::MouseUpJogPbCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CfrmCalibView::MouseDownJogPbCcw(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_PB_CW)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -m_dPtpMove_mm, _SPD_MODE);		
	}
	else
	{
		switch(Jog_Mode) {
		case _PTP_MOVE:
		case _MAX_SPEED:		SetSpeed= ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MID_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MidSpeed[_ACS_AXIS_IMP_ROLL];	break;
		case _MIN_SPEED: 		SetSpeed= ParamFile()->Motion_data.dJog_MinSpeed[_ACS_AXIS_IMP_ROLL];	break;
		default:		return ;
		}
		pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL, _DIR_CCW, SetSpeed);
	}
}


void CfrmCalibView::MouseUpJogPbCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CfrmCalibView::MouseDownJogZ1Ccw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogZ1Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmCalibView::MouseDownJogZ1Cw(short Button, short Shift, long x, long y)
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

void CfrmCalibView::MouseUpJogZ1Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}

//===================================================================================
void CfrmCalibView::MouseDownJogZ2Ccw(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

void CfrmCalibView::MouseUpJogZ2Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmCalibView::MouseDownJogZ2Cw(short Button, short Shift, long x, long y)
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

void CfrmCalibView::MouseUpJogZ2Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmCalibView::MouseDownJogZCcw(short Button, short Shift, long x, long y)
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

void CfrmCalibView::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmCalibView::MouseDownJogZCw(short Button, short Shift, long x, long y)
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

void CfrmCalibView::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================

void CfrmCalibView::MouseDownJogStageYCcw2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_Y_CCW2
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, -m_dPtpMove_mm, _SPD_MODE);		
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


void CfrmCalibView::MouseUpJogStageYCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmCalibView::MouseDownJogStageYCw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_Y_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, m_dPtpMove_mm, _SPD_MODE);		
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
void CfrmCalibView::MouseUpJogStageYCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmCalibView::MouseDownJogStageTCw2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CW2
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 2.0) return;
		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, m_dPtpMove_mm, _SPD_MODE);		
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


void CfrmCalibView::MouseUpJogStageTCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmCalibView::MouseDownJogStageTCcw2(short Button, short Shift, long x, long y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.IDB_JOG_STAGE_T_CCW2
	CRect disp_rect;
	GetDlgItem(IDB_JOG_STAGE_T_CCW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	if(Jog_Mode== _PTP_MOVE) {
		//		return;
		UpdateData(TRUE);
		if(m_dPtpMove_mm== 0) return;
		if(fabs(m_dPtpMove_mm)> 2.0) return;
		pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, -m_dPtpMove_mm, _SPD_MODE);		
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


void CfrmCalibView::MouseUpJogStageTCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmCalibView::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseDownJogCameraYCcw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogCameraYCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Y);
		else
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
	}
}
//===================================================================================


void CfrmCalibView::MouseDownJogCameraYCw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogCameraYCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		if(iStageJog_Select== STAGE_SEL_1)
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_UPPER_CAM_Y);
		else
			pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_LOWER_CAM_Y);
	}
}
//===================================================================================


void CfrmCalibView::MouseDownJogStageTCw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogStageTCw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseDownJogStageTCcw(short Button, short Shift, long x, long y)
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


void CfrmCalibView::MouseUpJogStageTCcw(short Button, short Shift, long x, long y)
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
BOOL CfrmCalibView::MoveEndCheckNonDisp(int Axis) 
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
BOOL CfrmCalibView::MoveEndCheck(int Axis) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
	
	ProgWnd.SetRange(0, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
  
	ProgWnd.SetText(_T("%s\n 잠시만 기다려 주세요..."), strText);
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


void CfrmCalibView::OnBnClickedButton2()
{
// 	CString strText;
// 	strText.Format("모터 이동이 실패했습니다.\n %s [%d]", pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1);
// 	MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); 
	MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1) ;
}

//===================================================================================
void CfrmCalibView::ClickVacuumStage1On()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmCalibView::ClickVacuumStage1Off()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmCalibView::ClickVacuumStage2On()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmCalibView::ClickVacuumStage2Off()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmCalibView::ClickCal1()
{
	if(proc_run_flag) return;
	proc_run_flag= true;

	CString strText;

	strText.Format(_T("카메라 Calibration을  진행 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("Calib."), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
		proc_run_flag= false;
		return;
	}
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePHOTOMASK);

	m_pVision->OverlayClear(eBTM_CAM_2X);		m_pVision->InspGrabContinue();
	m_pVision->OverlayClear(eBTM_CAM_20X);		m_pVision->OverlayClear(eTOP_CAM_2X);		m_pVision->OverlayClear(eTOP_CAM_20X);

//  	if(TopCam_Calib())
//  	{
		if(BotCam_Calib())	pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE);
		else				MyMessageBox(_T("[하부] 실패했습니다."), _T("Camera & Mask Calibration-Error!"), M_ICONERROR);
//  	}
//  	else
//  		MyMessageBox(_T("[상부] 실패했습니다."), _T("Camera & Mask Calibration-Error!"), M_ICONERROR);
// 
	pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE);

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	if(MovePos(POSITION_READY, FALSE, FALSE)== FALSE)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASK-10X [MovePos:POSITION_STAGE_ALIGN]"));
	}
	else 
		MyMessageBox(_T("완료 되었습니다."), _T("Camera & Mask Calibration!"), M_ICONINFORMATION);
	proc_run_flag= false;
	
	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);

	pFrm->Data_Reload();
}
//===================================================================================
void CfrmCalibView::ClickCal3()
{
	if(proc_run_flag) return;
	proc_run_flag= true;

	CString strText;

	strText.Format(_T("카메라 Calibration을  진행 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("Calib."), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
		proc_run_flag= false;
		return;
	}
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePHOTOMASK);

	m_pVision->OverlayClear(eBTM_CAM_2X);		m_pVision->InspGrabContinue();
	m_pVision->OverlayClear(eBTM_CAM_20X);		m_pVision->OverlayClear(eTOP_CAM_2X);		m_pVision->OverlayClear(eTOP_CAM_20X);

	if(TopCam_Calib())
	{
// 		if(BotCam_Calib())	pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE);
// 		else				MyMessageBox(_T("[하부] 실패했습니다."), _T("Camera & Mask Calibration-Error!"), M_ICONERROR);
	}
	else
		MyMessageBox(_T("[상부] 실패했습니다."), _T("Camera & Mask Calibration-Error!"), M_ICONERROR);

	pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE);

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	if(MovePos(POSITION_READY, FALSE, FALSE)== FALSE)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASK-10X [MovePos:POSITION_STAGE_ALIGN]"));
	}
	else 
		MyMessageBox(_T("완료 되었습니다."), _T("Camera & Mask Calibration!"), M_ICONINFORMATION);
	proc_run_flag= false;

	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);

	pFrm->Data_Reload();
}

//===================================================================================


void CfrmCalibView::ClickZWaitPos4()
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
void CfrmCalibView::MouseDownJogXCw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_X_CW2)->GetWindowRect(&disp_rect);
	if(x< 0 || y< 0 || x> disp_rect.Width() || y> disp_rect.Height() ) return;

	double SetSpeed;
	if(Jog_Mode== _PTP_MOVE) {
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

//===================================================================================
void CfrmCalibView::MouseUpJogXCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
	}
}

//===================================================================================
void CfrmCalibView::MouseDownJogXCcw2(short Button, short Shift, long x, long y)
{
	CRect disp_rect;
	GetDlgItem(IDB_JOG_X_CCW2)->GetWindowRect(&disp_rect);
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

//===================================================================================
void CfrmCalibView::MouseUpJogXCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
	{
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
	}
}

//===================================================================================
void CfrmCalibView::ClickStageJogSel5()
{
	if(m_btnZ_InterLock.GetValue())
	{
		g_bZ_InterLockFlag= FALSE; m_btnZ_InterLock.SetCaption(_T("Z축 인터록 OFF"));
	}
	else
	{
		g_bZ_InterLockFlag= TRUE; m_btnZ_InterLock.SetCaption(_T("Z축 인터록 ON"));
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmCalibView::OnBnClickedButton3()
{
	AfxMessageBox(_T("Pass!!"));
	return; 

	double retX, retY;
	UpdateData(TRUE);
	double cx= (m_dEndX - m_dStartX1) * ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0;
	double cy= -(m_dEndY - m_dStartY) * ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0;
	pSystem.GetVisionComponent()->Calc_OrgPosition(0.0, 0.0, cx, cy, m_dAngle, &retX, &retY);

	double motX= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-retX;
	double motY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-retY;
	m_dRotCen_PosStageX_20X= motX+ParamFile()->System_data.dTopCam20To2_X;
	m_dRotCen_PosCamY_20X= motY+ParamFile()->System_data.dTopCam20To2_Y;
	// 
	UpdateData(FALSE);
}
//===================================================================================
BOOL CfrmCalibView::TopCam_Calib()
{
	double cam1_Xmm, cam1_Ymm;
//	double cam2_Xmm, cam2_Ymm;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;

	int cnt;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
			//1st
	if(MovePos(POSITION_TOP_MASK, FALSE, FALSE)== FALSE)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASL-2X [MovePos:POSITION_STAGE_ALIGN]"));
		return FALSE;
	}
	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }


double AF_Z_Pos= 0.0;

/* 	if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_2X, &AF_Z_Pos))
 	{
 		ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos;
 	}
*/
	for(cnt= 0; cnt< 5; cnt++)
	{
		m_pVision->InspGrabContinueStop();
		Sleep(500);
		Sleep(ParamFile()->System_data.iImageGrab_Delay);
		m_pVision->InspGrab(eTOP_CAM_2X);

		if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdTOP_2X_MASK, eTOP_CAM_2X, true, true, true) == false)
		{
			Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] MASL-2X [MARK-Search-Error!] [S-1]"));
			m_pVision->InspGrabContinue();
			return FALSE;
		}
		m_pVision->InspGrabContinue();
		m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -cam1_Xmm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASL-2X [MovePos:mdTOP_2X_MASK]"));	return FALSE;			}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, cam1_Ymm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASL-2X [MovePos:mdTOP_2X_MASK]"));	return FALSE;			}

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		m_pVision->OverlayClear(eTOP_CAM_2X);
		Sleep(500);
	}
	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	m_pVision->InspGrab(eTOP_CAM_2X);
	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdTOP_2X_MASK, eTOP_CAM_2X, true, true, true);
	m_pVision->InspGrabContinue();

	ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_STAGE_X]=		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Z]=	pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
	ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y]=  pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
	ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_UPPER_CAM_Y]=		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);

	//20X--------------------------------------------------------------------------------------------------------------------------------------------------------------------

	if(MovePos(POSITION_TOP_MASK_20X, TRUE, FALSE)== FALSE)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASK-10X [MovePos:POSITION_STAGE_ALIGN]"));
		return FALSE;
	}

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	Sleep(1000);
	Invalidate();

 	if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos))
 	{
 		ParamFile()->Model_data.dPosition[POSITION_TOP_MASK_20X][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos;
 	}

	for(cnt= 0; cnt< 5; cnt++)
	{
		m_pVision->InspGrabContinueStop();
		Sleep(500);
		Sleep(ParamFile()->System_data.iImageGrab_Delay);
		m_pVision->InspGrab(eTOP_CAM_20X);

		if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdTOP_20X_MASK, eTOP_CAM_20X, true, true, true) == false)
		{
			Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] MASK-10X [MARK-Search-Error!] [S-1]"));
			m_pVision->InspGrabContinue();
			return FALSE;
		}
		m_pVision->InspGrabContinue();
		m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -cam1_Xmm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_TOP_MASK_10X]"));	return FALSE;			}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, cam1_Ymm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_TOP_MASK_10X]"));	return FALSE;			}

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		m_pVision->OverlayClear(eTOP_CAM_20X);
		Sleep(500);
	}
	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	m_pVision->InspGrab(eTOP_CAM_20X);
	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdTOP_20X_MASK, eTOP_CAM_20X, true, true, true);
	m_pVision->InspGrabContinue();

	double cal_x= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)- ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_STAGE_X];
	double cal_z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z)- ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Z];
	double cal_y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)- ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y];
	if(fabs(cal_z)< 0.001) cal_z= 0.0;
	if(fabs(cal_y)< 0.001) cal_y= 0.0;
	if(fabs(cal_x)< 0.001) cal_x= 0.0;
	ParamFile()->System_data.dTopCam20To2_X = cal_x;
	ParamFile()->System_data.dTopCam20To2_Y = cal_y;
	ParamFile()->System_data.dTopCam20To2_Z = cal_z;

// 	double ToPosition1= /*Cur_X + */-ParamFile()->System_data.dTopCam20To2_X;
// 	if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
// 	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
// 
// 	ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_STAGE_X]=		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
// 	ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Z]=	pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
// 	ParamFile()->Model_data.dPosition[POSITION_TOP_MASK][_ACS_AXIS_UPPER_CAM_Y]=	pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);


	ParamFile()->SaveSytemConfigFile();
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	DataUpdate();

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	AfxMessageBox(_T("Top-Calib. - End!!!"));
	return TRUE;
}
//===================================================================================
BOOL CfrmCalibView::BotCam_Calib()
{
	double cam1_Xmm, cam1_Ymm;
	//	double cam2_Xmm, cam2_Ymm;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;

	int cnt;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	//1st
	if(MovePos(POSITION_BTM_MASK, TRUE, FALSE)== FALSE)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASL-2X [MovePos:POSITION_STAGE_ALIGN]"));
		return FALSE;
	}

	for(cnt= 0; cnt< 5; cnt++)
	{
		m_pVision->InspGrabContinueStop();
		Sleep(500);
		Sleep(ParamFile()->System_data.iImageGrab_Delay);
		m_pVision->InspGrab(eBTM_CAM_2X);

		if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdBTM_2X_MASK, eBTM_CAM_2X, true, true, true) == false)
		{
			Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] MASL-2X [MARK-Search-Error!] [S-1]"));
			m_pVision->InspGrabContinue();
			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			return FALSE;
		}
		m_pVision->InspGrabContinue();
		m_pVision->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	
			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {
			if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
		}

		if(pSystem.GetMotionComponent()->INC_Move_SafeMode(_ACS_AXIS_STAGE_X, cam1_Xmm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASL-2X [MovePos:mdTOP_2X_MASK]"));	return FALSE;			}
		if(pSystem.GetMotionComponent()->INC_Move_SafeMode(_ACS_AXIS_LOWER_CAM_Y, cam1_Ymm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASL-2X [MovePos:mdTOP_2X_MASK]"));	return FALSE;			}

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		m_pVision->OverlayClear(eBTM_CAM_2X);
		Sleep(500);
	}
	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	m_pVision->InspGrab(eBTM_CAM_2X);
	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdBTM_2X_MASK, eBTM_CAM_2X, true, true, true);
	m_pVision->InspGrabContinue();

	ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_STAGE_X]=		pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
	ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_UPPER_CAM_Z]=	pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
	ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_Y]=  pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);

// 	//20X--------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(MovePos(POSITION_BTM_MASK_20X, TRUE, FALSE)== FALSE)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] MASK-10X [MovePos:POSITION_STAGE_ALIGN]"));
		return FALSE;
	}
	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	Sleep(1000);
	double CurCamZ= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);

	double AF_Z_Pos= 0.0;
	if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eBTM_CAM_20X, &AF_Z_Pos))
	{
		ParamFile()->Model_data.dPosition[POSITION_BTM_MASK_20X][_ACS_AXIS_LOWER_CAM_Z]= AF_Z_Pos;
	}

	for(cnt= 0; cnt< 5; cnt++)
	{
		m_pVision->InspGrabContinueStop();
		Sleep(500);
		Sleep(ParamFile()->System_data.iImageGrab_Delay);
		m_pVision->InspGrab(eBTM_CAM_20X);

		if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdBTM_20X_MASK, eBTM_CAM_20X, true, true, true) == false)
		{
			Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] MASK-10X [MARK-Search-Error!] [S-1]"));
			m_pVision->InspGrabContinue();
			return FALSE;
		}
		m_pVision->InspGrabContinue();
		m_pVision->GetCam_ROICood(eBTM_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
		
		CurCamZ= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);

		if(fabs(cam1_Xmm)>= 1.0 || fabs(cam1_Ymm)>= 1.0)
		{
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	TRUE)) 
			{	MyMessageBox(_T("1[BOT_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		}

		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X,cam1_Xmm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_BTM_MASK_10X]"));	return FALSE;			}
		if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, cam1_Ymm, _SPD_MODE, TRUE)== FALSE)
		{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_BTM_MASK_10X]"));	return FALSE;			}

		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(fabs(cam1_Xmm)>= 1.0 || fabs(cam1_Ymm)>= 1.0)
		{
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, CurCamZ,	TRUE)) 
			{	MyMessageBox(_T("2[BOT_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		}
		m_pVision->OverlayClear(eBTM_CAM_20X);
		Sleep(500);
	}
	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	m_pVision->InspGrab(eBTM_CAM_20X);
	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, mdBTM_20X_MASK, eBTM_CAM_20X, true, true, true);
	m_pVision->InspGrabContinue();

	double cal_x= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)- ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_STAGE_X];
	double cal_z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z)- ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_Z];
	double cal_y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y)- ParamFile()->Model_data.dPosition[POSITION_BTM_MASK][_ACS_AXIS_LOWER_CAM_Y];
	if(fabs(cal_z)< 0.001) cal_z= 0.0;
	if(fabs(cal_y)< 0.001) cal_y= 0.0;
	if(fabs(cal_x)< 0.001) cal_x= 0.0;
	ParamFile()->System_data.dBotCam20To2_X = cal_x + ParamFile()->System_data.dBot_Cam_20X_Offset_X;
	ParamFile()->System_data.dBotCam20To2_Y = cal_y + ParamFile()->System_data.dBot_Cam_20X_Offset_Y;
	ParamFile()->System_data.dBotCam20To2_Z = cal_z;
	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	ParamFile()->SaveSytemConfigFile();
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	DataUpdate();

	AfxMessageBox(_T("Bot-Calib. - End!!!"));
	return TRUE;
}

//===================================================================================
void CfrmCalibView::ClickJogXCw2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



//===================================================================================
void CfrmCalibView::ClickCmMove5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
/*
	CdlgPlateStamp_Jog dlg;
	dlg.m_calib_Mode= 0; // 0= X보정(Roll회전 사용), 1= Y보정 (Z축 사용)
	if( dlg.DoModal() == IDOK )
	{

	}
*/

}

//===================================================================================
void CfrmCalibView::ClickCmMove4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
/*
	CdlgPlateStamp_Jog dlg;
	dlg.m_calib_Mode= 1; // 0= X보정(Roll회전 사용), 1= Y보정 (Z축 사용)

	if( dlg.DoModal() == IDOK )
	{

	}
*/
}
//===================================================================================


void CfrmCalibView::ClickCal2()
{
	CdlgXZ12_Mat dlg;
	dlg.DoModal();
}


