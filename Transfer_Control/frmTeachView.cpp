// frmTeachView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmTeachView.h"
#include "MainFrm.h"
#include "dlgData_MotionSpeed.h"
#include "dlgRollForce_Test.h"

// CfrmTeachView
extern CSystemClass pSystem;
CParameterFile* CfrmTeachView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmTeachView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmTeachView, CFormView)

	CfrmTeachView::CfrmTeachView()
	: CFormView(CfrmTeachView::IDD)
	, m_dTmp(0)
	, m_dPtpMove_mm(0)
	, m_bStageY_JogEnable(FALSE)
{
	m_bJog1_Enable= m_bJog2_Enable= false;
}

CfrmTeachView::~CfrmTeachView()
{
}

void CfrmTeachView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
	DDX_Control(pDX, IDB_Y_POS0, m_btnReady);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL1, m_btnStageSel1);

#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	DDX_Control(pDX, IDB_OPU_X,				m_btnCopySel[0]);
	DDX_Control(pDX, IDB_OPU_ROLL_ROTATE,	m_btnCopySel[2]);
	DDX_Control(pDX, IDB_OPU_TMP1,			m_btnCopySel[1]);
	DDX_Control(pDX, IDB_OPU_TMP3,			m_btnCopySel[3]);
	DDX_Control(pDX, IDB_OPU_TOP_Z,			m_btnCopySel[4]);
	DDX_Control(pDX, IDB_OPU_TMP5,			m_btnCopySel[5]);
	DDX_Control(pDX, IDB_OPU_TMP6,			m_btnCopySel[6]);
	DDX_Control(pDX, IDB_OPU_TMP7,			m_btnCopySel[7]);
	DDX_Control(pDX, IDB_OPU_TOP_Y,			m_btnCopySel[8]);
	DDX_Control(pDX, IDB_OPU_BOT_X,			m_btnCopySel[9]);
	DDX_Control(pDX, IDB_OPU_BOT_Y,			m_btnCopySel[10]);
	DDX_Control(pDX, IDB_OPU_BOT_Z,			m_btnCopySel[11]);
	DDX_Control(pDX, IDB_OPU_Y,				m_btnCopySel[12]);
	DDX_Control(pDX, IDB_OPU_TMP13,			m_btnCopySel[13]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z1,		m_btnCopySel[14]);
	DDX_Control(pDX, IDB_OPU_TMP15,			m_btnCopySel[15]);

	DDX_Control(pDX,  IDC_CURPOS_0,  m_cCurPos[0]);
	DDX_Control(pDX,  IDC_CURPOS_1,  m_cCurPos[1]);
	DDX_Control(pDX,  IDC_CURPOS_2,  m_cCurPos[2]);
	DDX_Control(pDX,  IDC_CURPOS_3,  m_cCurPos[3]);
	DDX_Control(pDX,  IDC_CURPOS_4,  m_cCurPos[4]);
	DDX_Control(pDX,  IDC_CURPOS_5,  m_cCurPos[5]);
	DDX_Control(pDX,  IDC_CURPOS_6,  m_cCurPos[6]);
	DDX_Control(pDX,  IDC_CURPOS_7,  m_cCurPos[7]);
	DDX_Control(pDX,  IDC_CURPOS_8,  m_cCurPos[8]);
	DDX_Control(pDX,  IDC_CURPOS_9,  m_cCurPos[9]);
	DDX_Control(pDX,  IDC_CURPOS_10, m_cCurPos[10]);
	DDX_Control(pDX,  IDC_CURPOS_11, m_cCurPos[11]);
	DDX_Control(pDX,  IDC_CURPOS_12, m_cCurPos[12]);
	DDX_Control(pDX,  IDC_CURPOS_13, m_cCurPos[13]);
	DDX_Control(pDX,  IDC_CURPOS_14, m_cCurPos[14]);
	DDX_Control(pDX,  IDC_CURPOS_15, m_cCurPos[15]);

	DDX_Text(pDX,  IDC_EDIT_0,  m_dPosition[0]);
	DDX_Text(pDX,  IDC_EDIT_1,  m_dPosition[2]);
	DDX_Text(pDX,  IDC_EDIT_2,  m_dPosition[1]);
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
	DDX_Text(pDX,  IDC_EDIT_OFFSET_1,  m_dPosition_Offset[2]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_2,  m_dPosition_Offset[1]);
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
	DDX_Control(pDX, IDB_OPU_X,				m_btnCopySel[0]);
	DDX_Control(pDX, IDB_OPU_ROLL_ROTATE,	m_btnCopySel[2]);
	DDX_Control(pDX, IDB_OPU_TMP1,			m_btnCopySel[1]);
	DDX_Control(pDX, IDB_OPU_TMP3,			m_btnCopySel[3]);
	DDX_Control(pDX, IDB_OPU_TOP_Z,			m_btnCopySel[8]);
	DDX_Control(pDX, IDB_OPU_TMP5,			m_btnCopySel[9]);
	DDX_Control(pDX, IDB_OPU_TMP6,			m_btnCopySel[14]);
	DDX_Control(pDX, IDB_OPU_TMP7,			m_btnCopySel[15]);
	DDX_Control(pDX, IDB_OPU_TOP_Y,			m_btnCopySel[4]);
	DDX_Control(pDX, IDB_OPU_BOT_X,			m_btnCopySel[5]);
	DDX_Control(pDX, IDB_OPU_BOT_Y,			m_btnCopySel[6]);
	DDX_Control(pDX, IDB_OPU_BOT_Z,			m_btnCopySel[7]);
	DDX_Control(pDX, IDB_OPU_Y,				m_btnCopySel[10]);
	DDX_Control(pDX, IDB_OPU_TMP13,			m_btnCopySel[11]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z1,		m_btnCopySel[12]);
	DDX_Control(pDX, IDB_OPU_TMP15,			m_btnCopySel[13]);

	DDX_Control(pDX,  IDC_CURPOS_0,  m_cCurPos[0]);
	DDX_Control(pDX,  IDC_CURPOS_1,  m_cCurPos[1]);
	DDX_Control(pDX,  IDC_CURPOS_2,  m_cCurPos[2]);
	DDX_Control(pDX,  IDC_CURPOS_3,  m_cCurPos[3]);
	DDX_Control(pDX,  IDC_CURPOS_4,  m_cCurPos[8]);
	DDX_Control(pDX,  IDC_CURPOS_5,  m_cCurPos[9]);
	DDX_Control(pDX,  IDC_CURPOS_6,  m_cCurPos[14]);
	DDX_Control(pDX,  IDC_CURPOS_7,  m_cCurPos[15]);
	DDX_Control(pDX,  IDC_CURPOS_8,  m_cCurPos[4]);
	DDX_Control(pDX,  IDC_CURPOS_9,  m_cCurPos[5]);
	DDX_Control(pDX,  IDC_CURPOS_10, m_cCurPos[6]);
	DDX_Control(pDX,  IDC_CURPOS_11, m_cCurPos[7]);
	DDX_Control(pDX,  IDC_CURPOS_12, m_cCurPos[10]);
	DDX_Control(pDX,  IDC_CURPOS_13, m_cCurPos[11]);
	DDX_Control(pDX,  IDC_CURPOS_14, m_cCurPos[12]);
	DDX_Control(pDX,  IDC_CURPOS_15, m_cCurPos[13]);

	DDX_Text(pDX,  IDC_EDIT_0,  m_dPosition[0]);
	DDX_Text(pDX,  IDC_EDIT_1,  m_dPosition[2]);
	DDX_Text(pDX,  IDC_EDIT_2,  m_dPosition[1]);
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
	DDX_Text(pDX,  IDC_EDIT_OFFSET_1,  m_dPosition_Offset[2]);
	DDX_Text(pDX,  IDC_EDIT_OFFSET_2,  m_dPosition_Offset[1]);
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
	DDX_Check(pDX, IDC_CHECK1, m_bStageY_JogEnable);
}

BEGIN_MESSAGE_MAP(CfrmTeachView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MSG_DATA_UPDATE, DataReload)
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmTeachView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CfrmTeachView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON3, &CfrmTeachView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CfrmTeachView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CfrmTeachView::OnBnClickedButton6)
END_MESSAGE_MAP()


// CfrmTeachView 진단입니다.

#ifdef _DEBUG
void CfrmTeachView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmTeachView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmTeachView 메시지 처리기입니다.

//===================================================================================
void CfrmTeachView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_TeachView(this);

	Pos_Sel= POSITION_READY;
	m_btnReady.SetValue(1);

	iStageJog_Select= STAGE_SEL_1;
	m_btnStageSel1.SetValue(1);
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("상부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("상부 Vision\nStage +"));
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

	GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_STAGE_Y_CW2)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X

	m_bJog1_Enable= m_bJog2_Enable= false;

	DataUpdate();

	g_bCfrmTeachViewInit= TRUE;

/*
	GetDlgItem(IDB_JOG_SELECT_0)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_SELECT_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_SELECT_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_SELECT_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_JOG1_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_X_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_X_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_PB_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_PB_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_Y_CW2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z1_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z1_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z2_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z2_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_T_CCW2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_JOG1_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);


	GetDlgItem(IDB_STAGE_JOG_SEL1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_STAGE_JOG_SEL2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_CAMERA_Y_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_CAMERA_Y_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_T_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_T_CCW)->ShowWindow(SW_HIDE);
*/

}

//===================================================================================
void CfrmTeachView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmTeachView_TIMER_ID);

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmTeachView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmTeachView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmTeachView::OnTimer(UINT_PTR nIDEvent)
{
	CString strText;
	double cur_pos= 0.0;
	pSystem.GetMotionComponent()->Position_RealAll();
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_STAGE_X]);		SetDlgItemText(IDC_CURPOS_0, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL]);		SetDlgItemText(IDC_CURPOS_1, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_UPPER_CAM_Y]);	SetDlgItemText(IDC_CURPOS_8, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_UPPER_CAM_Z]);	SetDlgItemText(IDC_CURPOS_4, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z1]);	SetDlgItemText(IDC_CURPOS_14, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_IMP_ROLL_Z2]);	SetDlgItemText(IDC_CURPOS_15, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_ALIGN_Y1]);		SetDlgItemText(IDC_CURPOS_12, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_ALIGN_Y2]);		SetDlgItemText(IDC_CURPOS_13, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_X]);	SetDlgItemText(IDC_CURPOS_9, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_Y]);	SetDlgItemText(IDC_CURPOS_10, strText.GetBuffer(0));
	strText.Format(_T("%.4f"), pSystem.GetMotionComponent()->CurPosition_Data[_ACS_AXIS_LOWER_CAM_Z]);	SetDlgItemText(IDC_CURPOS_11, strText.GetBuffer(0));
	CFormView::OnTimer(nIDEvent);
}

//===================================================================================
LRESULT CfrmTeachView::DataReload(WPARAM wParam,LPARAM lParam)
{
	DataUpdate();
	return 0;
}
//===================================================================================

//===================================================================================
void CfrmTeachView::DataUpdate()
	//-----------------------------------------------------------------------------------
{
	CString strTmp;

	DispUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	ParamFile()->Model_data.dPosition[POSITION_X_AXIS_COMP_BTM_START][_ACS_AXIS_STAGE_X]= pFrm->GetWindowHandler_CompView()->CalBtmPosData();

	//	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
//	ParamFile()->OpenModelConfigFile(ParamFile()->System_data.strCurrentModelName);

	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;

	if(ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]< -1.0) ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]= -1.0;
	if(ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]< -1.0) ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]= -1.0;
	if(ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]> 16.0) ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z1]= 16.0;
	if(ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]> 16.0) ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL_Z2]= 16.0;

	int i;
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
void CfrmTeachView::DataSave()
//-----------------------------------------------------------------------------------
{
	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
	UpdateData(TRUE);

	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;

	switch(dispPos_Sel)
	{
	case POSITION_READY:	
		if(m_dPosition[_ACS_AXIS_STAGE_X] > 20.0) m_dPosition[_ACS_AXIS_STAGE_X]= 20.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] > 1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] > 1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] < -1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= -1.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] < -1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= -1.0;
//		m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
	//	m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;
		break;
	case POSITION_STAGE1_ALIGN:	
/*
		if(ParamFile()->Model_data.iAlignMode[POSITION_STAGE1_ALIGN]!= ALIGN_NONE_MODE)
		{
			m_dPosition[_ACS_AXIS_STAGE_X]=		ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_STAGE_X];
			m_dPosition[_ACS_AXIS_UPPER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_UPPER_CAM_Y];
			m_dPosition_Offset[_ACS_AXIS_STAGE_X]= 0.0;
			m_dPosition_Offset[_ACS_AXIS_UPPER_CAM_Y]= 0.0;
		}
*/

		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Y] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y])
			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if((m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]) > ParamFile()->System_data.dZ_Base_Position[STAGE_1_ALIGN_PROC])
		{
			m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->System_data.dZ_Base_Position[STAGE_1_ALIGN_PROC];
			m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		}
		if((m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]) > ParamFile()->System_data.dZ_Base_Position[STAGE_1_ALIGN_PROC])
		{
			m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->System_data.dZ_Base_Position[STAGE_1_ALIGN_PROC];
			m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		}
	break;
	case POSITION_STAGE2_ALIGN:	
		if(ParamFile()->Model_data.iAlignMode[POSITION_STAGE2_ALIGN]!= ALIGN_NONE_MODE)
		{
			m_dPosition[_ACS_AXIS_STAGE_X]=		ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_STAGE_X];
			m_dPosition[_ACS_AXIS_UPPER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_UPPER_CAM_Y];
			m_dPosition_Offset[_ACS_AXIS_STAGE_X]= 0.0;
			m_dPosition_Offset[_ACS_AXIS_UPPER_CAM_Y]= 0.0;
		}
		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Y] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y])
			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if((m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]) > ParamFile()->System_data.dZ_Base_Position[STAGE_2_ALIGN_PROC])
		{
			m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->System_data.dZ_Base_Position[STAGE_2_ALIGN_PROC];
			m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		}
		if((m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]) > ParamFile()->System_data.dZ_Base_Position[STAGE_2_ALIGN_PROC])
		{
			m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->System_data.dZ_Base_Position[STAGE_2_ALIGN_PROC];
			m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		}

		break;
	case POSITION_STAGE3_ALIGN:	
		if(ParamFile()->Model_data.iAlignMode[POSITION_STAGE3_ALIGN]!= ALIGN_NONE_MODE)
		{
			m_dPosition[_ACS_AXIS_STAGE_X]=		ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_STAGE_X];
			m_dPosition[_ACS_AXIS_UPPER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_UPPER_CAM_Y];
			m_dPosition_Offset[_ACS_AXIS_STAGE_X]= 0.0;
			m_dPosition_Offset[_ACS_AXIS_UPPER_CAM_Y]= 0.0;
		}
		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Y] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y])
			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if((m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]) > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC];
			m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		}
		if((m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]+m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]) > ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC])
		{
			m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->System_data.dZ_Base_Position[STAGE_3_ALIGN_PROC];
			m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		}

		break;
	case POSITION_ROLL_ALIGN:	
		if(ParamFile()->System_data.RollAlignPosMode!= _QT_POS_MODE)
		{
			if(m_dPosition[_ACS_AXIS_STAGE_X] > 0.0) 		m_dPosition[_ACS_AXIS_STAGE_X]=  0.0;
			m_dPosition_Offset[_ACS_AXIS_STAGE_X]= 0.0;
		}
		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;

		break;
	case POSITION_SYNC:	
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Y] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y])
			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;
		break;
	case POSITION_TOP_MASK:	
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] > 0.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] > 0.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;

		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Z] > 0.0) m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_X] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X]) 
			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];
		if(m_dPosition[_ACS_AXIS_LOWER_CAM_Y] != ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y])
			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;
		break;
	case POSITION_BTM_MASK:	
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] > 0.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] > 0.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y1] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;
		if(m_dPosition[_ACS_AXIS_ALIGN_Y2] > 0.0) m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;

		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;

// 		if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_PLATE_MODE)
// 		{
// 			m_dPosition[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->System_data.dRollPlateStamp_PosX;//ParamFile()->Model_data.dStamp_Len_X / 2.0;
// 			m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->System_data.dRollPlateStamp_PosY - (ParamFile()->Model_data.dStamp_Len_Y / 2.0);
// 		}
		break;
	case POSITION_X_AXIS_COMP_TOP_START:	
//		if(m_dPosition[_ACS_AXIS_STAGE_X] > 20.0) m_dPosition[_ACS_AXIS_STAGE_X]= 20.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] > 1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] > 1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] < -1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= -1.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] < -1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= -1.0;
		//		m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		//	m_dPosition_Offset[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;

		m_dPosition[_ACS_AXIS_LOWER_CAM_X]= 0.0;		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_X]= 0.0;
		m_dPosition[_ACS_AXIS_LOWER_CAM_Y]= 0.0;		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Y]= 0.0;
		m_dPosition[_ACS_AXIS_LOWER_CAM_Z]= 0.0;		m_dPosition_Offset[_ACS_AXIS_LOWER_CAM_Z]= 0.0;

		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;
		break;
	case POSITION_X_AXIS_COMP_BTM_START:	
		if(m_dPosition[_ACS_AXIS_STAGE_X] > 20.0) m_dPosition[_ACS_AXIS_STAGE_X]= 20.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] > 1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] > 1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z1] < -1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z1]= -1.0;
		if(m_dPosition[_ACS_AXIS_IMP_ROLL_Z2] < -1.0) m_dPosition[_ACS_AXIS_IMP_ROLL_Z2]= -1.0;

		m_dPosition[_ACS_AXIS_ALIGN_Y1]= 0.0;			m_dPosition[_ACS_AXIS_ALIGN_Y2]= 0.0;
		m_dPosition_Offset[_ACS_AXIS_ALIGN_Y1]= 0.0;	m_dPosition_Offset[_ACS_AXIS_ALIGN_Y2]= 0.0;
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
		ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
		//		ParamFile()->Model_data.dPosition_Offset[POSITION_READY][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
		ParamFile()->Model_data.dPosition_Offset[POSITION_READY][_ACS_AXIS_IMP_ROLL]= 0.0;

		ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
		ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
	
		ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
		ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

		ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];
		ParamFile()->Model_data.dPosition_Offset[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL];

	pSystem.GetMotionComponent()->CalAlignPos();

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CfrmTeachView 수정 / 저장"));
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
}

//===================================================================================
void CfrmTeachView::DispUpdate()
//-----------------------------------------------------------------------------------
{
	switch(Pos_Sel) 
	{
		case POSITION_READY:
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Y
				GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_9)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_X
				GetDlgItem(IDC_EDIT_10)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(TRUE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(TRUE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_STAGE1_ALIGN:
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Z
			if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_NONE_MODE)
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_8)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Y
			}
			else
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
				GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
				GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			}
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_STAGE2_ALIGN:
			if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_NONE_MODE)
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_8)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Y
			}
			else
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
				GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
				GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			}
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_STAGE3_ALIGN:
			if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_NONE_MODE)
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_8)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Y
			}
			else
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
				GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
				GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			}
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2 
			break;
		case POSITION_ROLL_ALIGN:
			GetDlgItem(IDC_EDIT_1)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_NONE_MODE)
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_9)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_X
				GetDlgItem(IDC_EDIT_10)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Y
			}
			else
			{
				GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
				GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
				GetDlgItem(IDC_EDIT_10)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Y
			}
			GetDlgItem(IDC_EDIT_11)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_SYNC:
			GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
			GetDlgItem(IDC_EDIT_1)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
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
			GetDlgItem(IDC_EDIT_9)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(TRUE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_X_AXIS_COMP_TOP_START:
			GetDlgItem(IDC_EDIT_0)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(TRUE); //_ACS_AXIS_UPPER_CAM_Y
			GetDlgItem(IDC_EDIT_9)->EnableWindow(FALSE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(FALSE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
		case POSITION_X_AXIS_COMP_BTM_START:
			GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_0)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
			GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_1)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL
			GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_4)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Z
			GetDlgItem(IDC_EDIT_8)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_8)->EnableWindow(FALSE); //_ACS_AXIS_UPPER_CAM_Y
			GetDlgItem(IDC_EDIT_9)->EnableWindow(TRUE);		GetDlgItem(IDC_EDIT_OFFSET_9)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_X
			GetDlgItem(IDC_EDIT_10)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_10)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Y
			GetDlgItem(IDC_EDIT_11)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_OFFSET_11)->EnableWindow(TRUE); //_ACS_AXIS_LOWER_CAM_Z
			GetDlgItem(IDC_EDIT_12)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_12)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y1
			GetDlgItem(IDC_EDIT_13)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_13)->EnableWindow(FALSE); //_ACS_AXIS_ALIGN_Y2
			GetDlgItem(IDC_EDIT_14)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_14)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL_Z1
			GetDlgItem(IDC_EDIT_15)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_OFFSET_15)->EnableWindow(FALSE); //_ACS_AXIS_IMP_ROLL_Z2
			break;
	}
}
//===================================================================================
void CfrmTeachView::JogDispUpdate()
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
BEGIN_EVENTSINK_MAP(CfrmTeachView, CFormView)
	ON_EVENT(CfrmTeachView, IDB_Y_POS0, DISPID_CLICK, CfrmTeachView::ClickYPos0, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS8, DISPID_CLICK, CfrmTeachView::ClickYPos8, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS1, DISPID_CLICK, CfrmTeachView::ClickYPos1, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS9, DISPID_CLICK, CfrmTeachView::ClickYPos9, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS10, DISPID_CLICK, CfrmTeachView::ClickYPos10, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDC_BUTTON_JOG_POPUP, DISPID_CLICK, CfrmTeachView::ClickButtonJogPopup, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_STAGE_JOG_SEL1, DISPID_CLICK, CfrmTeachView::ClickStageJogSel1, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_STAGE_JOG_SEL2, DISPID_CLICK, CfrmTeachView::ClickStageJogSel2, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_JOG_SELECT_0, DISPID_CLICK, CfrmTeachView::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_JOG_SELECT_1, DISPID_CLICK, CfrmTeachView::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_JOG_SELECT_2, DISPID_CLICK, CfrmTeachView::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_JOG_SELECT_3, DISPID_CLICK, CfrmTeachView::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_POS_SET2, DISPID_CLICK, CfrmTeachView::ClickPosSet2, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MOVE, DISPID_CLICK, CfrmTeachView::ClickCmMove, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MOVE2, DISPID_CLICK, CfrmTeachView::ClickCmMove2, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Z_WAIT_POS, DISPID_CLICK, CfrmTeachView::ClickZWaitPos, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Z_WAIT_POS7, DISPID_CLICK, CfrmTeachView::ClickZWaitPos7, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MANUAL_SPEED, DISPID_CLICK, CfrmTeachView::ClickCmManualSpeed, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MANUAL_SPEED2, DISPID_CLICK, CfrmTeachView::ClickCmManualSpeed2, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_DATA_SAVE, DISPID_CLICK, CfrmTeachView::ClickDataSave, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_JOG_X_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_X_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_X_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_X_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_PB_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_PB_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_PB_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_PB_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z1_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z1_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z2_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z2_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z2_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z2_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z1_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_Z1_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEDOWN, CfrmTeachView::MouseDownJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEUP, CfrmTeachView::MouseUpJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmTeachView, IDB_VACUUM_STAGE1_ON, DISPID_CLICK, CfrmTeachView::ClickVacuumStage1On, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_VACUUM_STAGE1_OFF, DISPID_CLICK, CfrmTeachView::ClickVacuumStage1Off, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_VACUUM_STAGE2_ON, DISPID_CLICK, CfrmTeachView::ClickVacuumStage2On, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_VACUUM_STAGE2_OFF, DISPID_CLICK, CfrmTeachView::ClickVacuumStage2Off, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Z_WAIT_POS3, DISPID_CLICK, CfrmTeachView::ClickZWaitPos3, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Z_WAIT_POS5, DISPID_CLICK, CfrmTeachView::ClickZWaitPos5, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Z_WAIT_POS8, DISPID_CLICK, CfrmTeachView::ClickZWaitPos8, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MOVE6, DISPID_CLICK, CfrmTeachView::ClickCmMove6, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MOVE10, DISPID_CLICK, CfrmTeachView::ClickCmMove10, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDC_TEACHING_POS5, DISPID_DBLCLICK, CfrmTeachView::DblClickTeachingPos5, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDC_TEACHING_POS6, DISPID_DBLCLICK, CfrmTeachView::DblClickTeachingPos6, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS2, DISPID_CLICK, CfrmTeachView::ClickYPos2, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_CM_MOVE16, DISPID_CLICK, CfrmTeachView::ClickCmMove16, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS15, DISPID_CLICK, CfrmTeachView::ClickYPos15, VTS_NONE)
	ON_EVENT(CfrmTeachView, IDB_Y_POS17, DISPID_CLICK, CfrmTeachView::ClickYPos17, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CfrmTeachView::ClickYPos0()
{	Pos_Sel= POSITION_READY;						DataUpdate(); }
//===================================================================================
void CfrmTeachView::ClickYPos8()
{	Pos_Sel= POSITION_STAGE1_ALIGN;					DataUpdate(); }
//===================================================================================
void CfrmTeachView::ClickYPos1()
{	Pos_Sel= POSITION_STAGE2_ALIGN;					DataUpdate(); }
//===================================================================================
void CfrmTeachView::ClickYPos9()
{	Pos_Sel= POSITION_ROLL_ALIGN;					DataUpdate(); }
//===================================================================================
void CfrmTeachView::ClickYPos10()
{	Pos_Sel= POSITION_SYNC;							DataUpdate(); }
void CfrmTeachView::ClickYPos2()
{	Pos_Sel= POSITION_STAGE3_ALIGN;					DataUpdate(); }
void CfrmTeachView::ClickYPos15()
{	Pos_Sel= POSITION_X_AXIS_COMP_TOP_START;					DataUpdate(); }
void CfrmTeachView::ClickYPos17()
{	Pos_Sel= POSITION_X_AXIS_COMP_BTM_START;					DataUpdate(); }

//===================================================================================
void CfrmTeachView::ClickButtonJogPopup()
{
	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
		m_btnCopySel[i].SetValue(1);
	}
}
//===================================================================================
void CfrmTeachView::ClickStageJogSel1()
{
	iStageJog_Select= STAGE_SEL_1;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("상부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("상부 Vision\nStage +"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("상부 Vision\nStage Z-"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("상부 Vision\nStage Z+"));

	SetDlgItemText(IDC_TEACHING_POS6, _T("상부 Vision부 JOG 운전"));
	
}
//===================================================================================

void CfrmTeachView::ClickStageJogSel2()
{
	iStageJog_Select= STAGE_SEL_2;
	GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
	GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
	SetDlgItemText(IDB_JOG_STAGE_X_CCW, _T("하부 Vision\nStage +"));
	SetDlgItemText(IDB_JOG_STAGE_X_CW, _T("하부 Vision\nStage -"));
	SetDlgItemText(IDB_JOG_STAGE_T_CW, _T("하부 Vision\nStage Z+"));
	SetDlgItemText(IDB_JOG_STAGE_T_CCW, _T("하부 Vision\nStage Z-"));
	SetDlgItemText(IDC_TEACHING_POS6, _T("하부 Vision부 JOG 운전"));
}
//===================================================================================
void CfrmTeachView::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmTeachView::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmTeachView::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmTeachView::ClickJogSelect3()
{
	Jog_Mode= _PTP_MOVE;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}

//===================================================================================
void CfrmTeachView::ClickPosSet2()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;

	CString strText, strTitle;
	CString strPos;
	double ToPosition= 0.0;
	strText= _T("");	strTitle= _T("");
	double UpData[_MAX_AXIS];
	int i;

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

	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;


/*
	for(i= 0; i< _MAX_AXIS; i++)
	{
		if(	m_btnCopySel[i].GetValue())		{			ParamFile()->Model_data.dPosition[dispPos_Sel][i]= UpData[i];		}
	}
*/

	switch(dispPos_Sel)
	{
	case POSITION_READY:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	
		{
			if(UpData[_ACS_AXIS_IMP_ROLL_Z1] > 0.0) UpData[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			if(UpData[_ACS_AXIS_IMP_ROLL_Z2] > 0.0) UpData[_ACS_AXIS_IMP_ROLL_Z2]= 0.0;
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}

		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())
		{
			UpData[_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= UpData[_ACS_AXIS_LOWER_CAM_X];
		}
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	
		{
			UpData[_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y];
		}
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	
		{
			if(UpData[_ACS_AXIS_LOWER_CAM_Z] > 0.0) UpData[_ACS_AXIS_LOWER_CAM_Z]= 0.0;
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= UpData[_ACS_AXIS_LOWER_CAM_Z];
		}
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())
		{
			if(UpData[_ACS_AXIS_ALIGN_Y1] != 0.0) UpData[_ACS_AXIS_ALIGN_Y1]= 0.0;
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= UpData[_ACS_AXIS_ALIGN_Y1];
		}
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())
		{
			if(UpData[_ACS_AXIS_ALIGN_Y2] != 0.0) UpData[_ACS_AXIS_ALIGN_Y2]= 0.0;
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= UpData[_ACS_AXIS_ALIGN_Y2];
		}
		break;
	case POSITION_STAGE1_ALIGN:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	
		{
				ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}

		if(ParamFile()->Model_data.iAlignMode[POSITION_STAGE1_ALIGN]== ALIGN_NONE_MODE)
		{
			if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
			if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		}
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]=  ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]=  ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_STAGE2_ALIGN:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())
		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}

		if(ParamFile()->Model_data.iAlignMode[POSITION_STAGE2_ALIGN]== ALIGN_NONE_MODE)
		{
			if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
			if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		}
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_STAGE3_ALIGN:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	
		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}
		if(ParamFile()->Model_data.iAlignMode[POSITION_STAGE3_ALIGN]== ALIGN_NONE_MODE)
		{
			if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
			if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		}
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_ROLL_ALIGN:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())
		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}
		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_SYNC:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	
		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}
		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_TOP_MASK:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())
		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= 0.0;//UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= 0.0;//UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}
		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= 0.0;//UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
	break;
	case POSITION_BTM_MASK:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	
		{
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];
		}
		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= 0.0;//UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= 0.0;//UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_X_AXIS_COMP_TOP_START: //
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z1];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z2];

		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		ParamFile()->Model_data.dPosition[POSITION_X_AXIS_COMP_BTM_START][_ACS_AXIS_STAGE_X]= pFrm->GetWindowHandler_CompView()->CalBtmPosData();

		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL];//UpData[_ACS_AXIS_IMP_ROLL];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];;//UpData[_ACS_AXIS_UPPER_CAM_Z];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];;//UpData[_ACS_AXIS_UPPER_CAM_Y];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Z];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y1];//UpData[_ACS_AXIS_ALIGN_Y1];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y2];//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_X_AXIS_COMP_BTM_START: //
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z1];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL_Z2];

		//ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		ParamFile()->Model_data.dPosition[POSITION_X_AXIS_COMP_BTM_START][_ACS_AXIS_STAGE_X]= pFrm->GetWindowHandler_CompView()->CalBtmPosData();

		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL];//UpData[_ACS_AXIS_IMP_ROLL];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_UPPER_CAM_Z];//UpData[_ACS_AXIS_UPPER_CAM_Z];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_UPPER_CAM_Y];//UpData[_ACS_AXIS_UPPER_CAM_Y];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= UpData[_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= UpData[_ACS_AXIS_LOWER_CAM_Y];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= UpData[_ACS_AXIS_LOWER_CAM_Z];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y1];//UpData[_ACS_AXIS_ALIGN_Y1];
		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_ALIGN_Y2];//UpData[_ACS_AXIS_ALIGN_Y2];
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
void CfrmTeachView::ClickCmMove()
{
	MovePos(Pos_Sel, TRUE);
}

void CfrmTeachView::MovePos(int pos, BOOL Z_Flag)
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;
	int dispPos_Sel;
	dispPos_Sel= pos;

	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}

	switch(dispPos_Sel) 
	{
	case POSITION_READY:
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:
	case POSITION_ROLL_ALIGN:
	case POSITION_SYNC:
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
	case POSITION_X_AXIS_COMP_TOP_START: //
		strPos.Format(_T("SX= %.3f:TY=%.3f:TZ=%.3f]"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel),												
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel));
		break;
	case POSITION_X_AXIS_COMP_BTM_START: //
		strPos.Format(_T("SX= %.3f:BX=%.3f:BY=%.3f:BZ=%.3f]"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel),												
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel));
		break;
	default:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
	//-----------------
	strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
	//-----------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	//-----------------
	
	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X, dispPos_Sel)) 
	{
		// 		strText.Format("Z축이 대기 위치보다 아래로 내려와 있습니다.\n선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s", strPos);
		// 		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONERROR|M_YESNO, _T("예"), _T("아니오")) == IDNO)
		// 		{
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
		return;
		//		}
	}

	switch(dispPos_Sel) 
	{
	case POSITION_READY:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		break;
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		//-----------------
		if(Z_Flag)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
//			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		break;
	case POSITION_ROLL_ALIGN:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 
		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 
		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, Pos_Sel);
		// 		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(Z_Flag)
		{
			//-----------------
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			//-----------------
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
//			if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}

		break;
	case POSITION_SYNC:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 
		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 
		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, Pos_Sel);
		// 		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		//-----------------
		if(Z_Flag)
		{
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
//			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
		break;
	case POSITION_TOP_MASK:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		break;
	case POSITION_BTM_MASK:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		// 
		// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, Pos_Sel);
		// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		//-----------------------------
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		break;
	case POSITION_X_AXIS_COMP_TOP_START: //
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		break;
	case POSITION_X_AXIS_COMP_BTM_START: //
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		break;
	default:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}
//===================================================================================
void CfrmTeachView::ClickCmMove2()//Z-Wait & All-Move
{
	MovePos(Pos_Sel, FALSE);
}

//===================================================================================
void CfrmTeachView::ClickZWaitPos() // Z-Up
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;

	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
	}
// 	if(Pos_Sel== POSITION_ROLL_ALIGN)
// 	{
// 		//-----------------
// 		ToPosition1	= 0.0;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, Pos_Sel);
// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		//-----------------
// 	}

	strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_READY),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY));

/*
	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X)) 
	{
		// 		strText.Format("Z축이 대기 위치보다 아래로 내려와 있습니다.\n선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s", strPos);
		// 		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONERROR|M_YESNO, _T("예"), _T("아니오")) == IDNO)
		// 		{
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
		return;
		//		}
	}
	else 
	{
*/
		strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s"), strPos);
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
//	}
		//-----------------
		ToPosition1	= 0.0;//pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, Pos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		//-----------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

}

//===================================================================================
void CfrmTeachView::ClickZWaitPos7() // Z-Down
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
	if(pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel)> 1.0 && pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel)> 1.0)
	{
		double chaval= fabs(X_CurPos - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel));
		if(chaval > 10.0)
		{
			if(MyMessageBox(_T("X축 위치가 설정 위치를 벗어나 있습니다.\nZ축 이동을 진행하시겠습니까?"), _T("Position-Move Failed"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) != IDYES)
				return ;
		}
	}

	switch(dispPos_Sel) 
	{
	case POSITION_READY:
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:
	case POSITION_ROLL_ALIGN:
	case POSITION_SYNC:
	case POSITION_TOP_MASK:
	case POSITION_BTM_MASK:
		strPos.Format(_T("Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel));
		break;
	default:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X, dispPos_Sel)) 
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
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		break;
	case POSITION_STAGE1_ALIGN:
	case POSITION_STAGE2_ALIGN:
	case POSITION_STAGE3_ALIGN:
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
		//		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 		if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		break;
	case POSITION_ROLL_ALIGN:
		X_CurPos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		if(X_CurPos >= ParamFile()->System_data.X_SAFE_FREEZONE_H)
		{
			MyMessageBox(_T("이동이 실패했습니다. Roll-Align 위치 에러!\nX축 위치가 설정 위치를 벗어나 있습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		}
		//-----------------
// 		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, Pos_Sel);
// 		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		break;
	case POSITION_SYNC:
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
		//		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
//		if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		break;
	case POSITION_TOP_MASK:
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	break;
	case POSITION_BTM_MASK:
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		break;
	default:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}

//===================================================================================
void CfrmTeachView::ClickCmManualSpeed()
{
//	CdlgData_MotionSpeed dlg;
	//dlg.DoModal();
}

//===================================================================================
void CfrmTeachView::ClickCmManualSpeed2()
{
	CdlgData_MotionSpeed dlg;
	dlg.DoModal();
}

//===================================================================================
void CfrmTeachView::ClickDataSave()
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
void CfrmTeachView::MouseDownJogXCw(short Button, short Shift, long x, long y)
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
void CfrmTeachView::MouseUpJogXCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================


void CfrmTeachView::MouseDownJogXCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogXCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================
void CfrmTeachView::MouseDownJogPbCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogPbCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CfrmTeachView::MouseDownJogPbCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogPbCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CfrmTeachView::MouseDownJogZ1Ccw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogZ1Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmTeachView::MouseDownJogZ1Cw(short Button, short Shift, long x, long y)
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

void CfrmTeachView::MouseUpJogZ1Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}

//===================================================================================
void CfrmTeachView::MouseDownJogZ2Ccw(short Button, short Shift, long x, long y)
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

void CfrmTeachView::MouseUpJogZ2Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmTeachView::MouseDownJogZ2Cw(short Button, short Shift, long x, long y)
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

void CfrmTeachView::MouseUpJogZ2Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmTeachView::MouseDownJogZCcw(short Button, short Shift, long x, long y)
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

void CfrmTeachView::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmTeachView::MouseDownJogZCw(short Button, short Shift, long x, long y)
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

void CfrmTeachView::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================

void CfrmTeachView::MouseDownJogStageYCcw2(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageYCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmTeachView::MouseDownJogStageYCw2(short Button, short Shift, long x, long y)
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
void CfrmTeachView::MouseUpJogStageYCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmTeachView::MouseDownJogStageTCw2(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageTCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmTeachView::MouseDownJogStageTCcw2(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageTCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmTeachView::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseDownJogCameraYCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogCameraYCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseDownJogCameraYCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogCameraYCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseDownJogStageTCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageTCw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseDownJogStageTCcw(short Button, short Shift, long x, long y)
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


void CfrmTeachView::MouseUpJogStageTCcw(short Button, short Shift, long x, long y)
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
BOOL CfrmTeachView::MoveEndCheck(int Axis) 
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
BOOL CfrmTeachView::MoveEndCheckStampSync(int Axis, int timeout) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
	
	ProgWnd.SetRange(0, timeout);
  
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
		if(pSystem.GetMotionComponent()->isDone(Axis) && pSystem.GetMotionComponent()->isStampc_End()) 
		{
				return TRUE;
		}
		Sleep(9);
		
		if(/*!pFrm->m_ProgWait->ProgEnable || */GetTickCount() - dwCheckTimeStart_Axis > (DWORD)(timeout))  
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


void CfrmTeachView::OnBnClickedButton2()
{
// 	CString strText;
// 	strText.Format("모터 이동이 실패했습니다.\n %s [%d]", pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1);
// 	MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); 
//	MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1) ;
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
}

//===================================================================================
void CfrmTeachView::ClickVacuumStage1On()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmTeachView::ClickVacuumStage1Off()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmTeachView::ClickVacuumStage2On()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmTeachView::ClickVacuumStage2Off()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmTeachView::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	if(m_bStageY_JogEnable)
	{
		GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_STAGE_Y_CW2)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
		GetDlgItem(IDB_JOG_STAGE_T_CW2)->EnableWindow(TRUE);		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->EnableWindow(TRUE); //_ACS_AXIS_STAGE_X
	}
	else
	{
		GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_STAGE_Y_CW2)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
		GetDlgItem(IDB_JOG_STAGE_T_CW2)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
	}
}

//===================================================================================
void CfrmTeachView::ClickZWaitPos3()
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
void CfrmTeachView::ClickZWaitPos5()
{//하부 상승
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
	case POSITION_X_AXIS_COMP_BTM_START: //
		strPos.Format(_T("BZ=%.3f"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_X_AXIS_COMP_BTM_START));
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
	case POSITION_X_AXIS_COMP_BTM_START: //
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_X_AXIS_COMP_BTM_START);
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
void CfrmTeachView::ClickZWaitPos8()
{//하부 하강
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
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
}
//===================================================================================

void CfrmTeachView::ClickCmMove6()
{
	CString strText;
	double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
	double ToPosition1, ToPosition2;

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	int CameraStageUsedNo= eTOP_CAM;
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_PLATE_MODE)
	{
		if(CameraStageUsedNo== eTOP_CAM)
		{
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
	else
	{
		if(CameraStageUsedNo== eTOP_CAM)
		{
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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

}


void CfrmTeachView::OnBnClickedButton3()
{
//	return;
//	g_ImageSaveCount= 0; g_ImageSaveFlag_Test= true;

	int cnt= 0;
	CString strText;
	double angle, posx, posy;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	double dCurr_Pos= 0.0;
	dCurr_Pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);

	CString strFile__Name;
	CString strDir__Name;
	CString strData;
	CString strResult;

	SYSTEMTIME st; 
	GetLocalTime(&st); 
	_wmkdir(_T("d:\\RollAlign_ImageSave"));
	strDir__Name.Format(_T("d:\\RollAlign_ImageSave\\%04d%02d%02d_%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond); //strDirMeasImageSavePath);
	_wmkdir(strDir__Name);
	
	for(cnt= 0; cnt< 7; cnt++)
	{
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

		pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_2X);
		pSystem.GetVisionComponent()->PatternFinderPos(&angle, &posx, &posy, mdBTM_2X_ROLL_START, eBTM_CAM_2X, true, true, true);
		strText.Format(_T("%s\\%02d.tif"), strDir__Name, cnt);
		pSystem.GetVisionComponent()->Save_Image(eBTM_CAM_2X, strText);
		//-------

		strFile__Name.Format(_T("%s\\Report_.txt"), strDir__Name, 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond); //strDirMeasImageSavePath);

		FILE* fout = _wfopen(strFile__Name.GetBuffer(0), _T("a+"));   // 파일에 계속 추가시킨다. //
		if(fout == NULL) 	{		return;		}

		strData.Format(_T("%d\t%f\t%f\t%f"), cnt, angle, posx, posy);

		fwprintf(fout, _T("%s\n"), strData);

		fclose(fout);

		//-------
		pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, 5.0, _SPD_MODE, TRUE);		
		((CTransfer_ControlApp *)AfxGetApp())->DoEvents();
		while(1)
		{
			if(pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL))   break;
			Sleep(50);
		}
		Sleep(500);
	}
	Sleep(500);
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, dCurr_Pos);
}


void CfrmTeachView::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	g_ImageSaveFlag_Test= false;
}


void CfrmTeachView::ClickCmMove10()
{
/*
	CString strText;
//	double L_X, Stage_Y, TopCamY, bot_X, bot_Y;
	double ToPosition1, ToPosition2;

	int dispPos_Sel;
	dispPos_Sel= Pos_Sel;
	int data_no= STAGE_1_ALIGN_PROC;
	int graph_no= 0;

	if     (dispPos_Sel== POSITION_STAGE1_ALIGN) { data_no= STAGE_1_ALIGN_PROC; graph_no= STAGE_1_GRAPH; }
	else if(dispPos_Sel== POSITION_STAGE2_ALIGN) { data_no= STAGE_2_ALIGN_PROC; graph_no= STAGE_2_GRAPH; }
	else
	{
		MyMessageBox(_T("[선택 확인] 동작이 실패했습니다."), _T("Position Failed"), M_ICONERROR); return ;
	}
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	strText.Format(_T("스탬프 1Cycle동작을 하시겠습니까? \n스탬프 Down -> Up\nFast-Move : %.3f, S-Spd : %.3f[%.1f]\n편측 Kg : %.3f, Delay : %dms "), 
		pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel), 
		ParamFile()->Model_data.dRun_DownSpeed[data_no],  
		ParamFile()->Model_data.dRun_UpSpeed[data_no],
		ParamFile()->Model_data.dRun_Force[data_no],
		ParamFile()->Model_data.iRun_Delay[data_no]	);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;

	if(!pSystem.GetMotionComponent()->Stamp_Start(data_no))
	{
		MyMessageBox(_T("[에러] 1Cycle 동작이 실패했습니다."), _T("Position Failed"), M_ICONERROR); return ;
	}
	MoveEndCheckStampSync(_ACS_AXIS_IMP_ROLL_Z1, ParamFile()->Model_data.iRun_Delay[data_no] + 60000);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrm->GetWindowHandler_MainView()->EndGraph(graph_no);
	pSystem.GetMotionComponent()->Stamp_Stop();

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	strText.Format(_T("동작이 완료 되었습니다.\nZ축을 상승 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
			return;
	}
	else 
	{
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual_Speed(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	
			 ParamFile()->Model_data.dRun_UpSpeed[data_no], FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	}
*/
}
/*
m_dRun_Speed[i]= ParamFile()->Model_data.dRun_Speed[i];
m_dRun_Length[i]= ParamFile()->Model_data.dRun_Length[i];
m_dRun_Force[i]= ParamFile()->Model_data.dRun_Force[i];
m_dRun_DownSpeed[i]= ParamFile()->Model_data.dRun_DownSpeed[i];
m_dRun_UpSpeed[i]= ParamFile()->Model_data.dRun_UpSpeed[i];
m_iRun_Delay[i]= ParamFile()->Model_data.iRun_Delay[i];
*/


void CfrmTeachView::DblClickTeachingPos5()
{
/*
	if(!m_bJog1_Enable)
	{
		m_bJog1_Enable= true;
		GetDlgItem(IDB_JOG_SELECT_0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_SELECT_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_SELECT_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_SELECT_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_JOG1_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_X_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_X_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_PB_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_PB_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_Y_CW2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_Z1_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_Z1_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_Z2_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_Z2_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_T_CW2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_JOG1_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_bJog1_Enable= false;
		GetDlgItem(IDB_JOG_SELECT_0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_SELECT_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_SELECT_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_SELECT_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_JOG1_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_X_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_X_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_PB_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_PB_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_Y_CW2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z1_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z1_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z2_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z2_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_Z_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_T_CW2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_JOG1_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
	}
*/
}


void CfrmTeachView::DblClickTeachingPos6()
{
/*
	if(!m_bJog2_Enable)
	{
		m_bJog2_Enable= true;
		GetDlgItem(IDB_STAGE_JOG_SEL1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_STAGE_JOG_SEL2)->ShowWindow(SW_SHOW);
		if(iStageJog_Select= STAGE_SEL_2)
		{
			GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_SHOW);
		}
		GetDlgItem(IDB_JOG_CAMERA_Y_CCW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_CAMERA_Y_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_T_CW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_JOG_STAGE_T_CCW)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_bJog2_Enable= false;
		GetDlgItem(IDB_STAGE_JOG_SEL1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_STAGE_JOG_SEL2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_X_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_X_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_CAMERA_Y_CCW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_CAMERA_Y_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_T_CW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_JOG_STAGE_T_CCW)->ShowWindow(SW_HIDE);
	}
*/
}




void CfrmTeachView::ClickCmMove16()
{
	CdlgRollForce_Test dlg;
	dlg.DoModal();
}


void CfrmTeachView::OnBnClickedButton6()
{
	pSystem.GetIO()->LoadCell_Reset();
}
