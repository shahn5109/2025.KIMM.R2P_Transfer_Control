// frmStampView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmStampView.h"
#include "MainFrm.h"
#include "dlgData_MotionSpeed.h"
#include "dlgManuAlign.h"
#include "dlgStampInst_Jog.h"
#include "dlgRollForce_Test.h"

// CfrmStampView
extern CSystemClass pSystem;
CParameterFile* CfrmStampView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmStampView::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmStampView, CFormView)

	CfrmStampView::CfrmStampView()
	: CFormView(CfrmStampView::IDD)
	, m_dTmp(0)
	, m_dPtpMove_mm(0)
	, m_bStageY_JogEnable(FALSE)
{

}

CfrmStampView::~CfrmStampView()
{
}

void CfrmStampView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_JOG_SELECT_1, m_btnJogModeSel1);
	DDX_Control(pDX, IDB_Y_POS0, m_btnReady);
	DDX_Control(pDX, IDB_STAGE_JOG_SEL1, m_btnStageSel1);

	DDX_Text(pDX, IDC_EDIT1, m_dPtpMove_mm);
	DDX_Check(pDX, IDC_CHECK1, m_bStageY_JogEnable);

	DDX_Control(pDX, IDB_JOG_SELECT_TUSED2, m_btnManuStamp_TopUsed);
	DDX_Control(pDX, IDB_JOG_SELECT_BUSED2, m_btnManuStamp_BtmUsed);

}

BEGIN_MESSAGE_MAP(CfrmStampView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmStampView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CfrmStampView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON6, &CfrmStampView::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CfrmStampView::OnBnClickedButton7)
END_MESSAGE_MAP()


// CfrmStampView 진단입니다.

#ifdef _DEBUG
void CfrmStampView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmStampView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmStampView 메시지 처리기입니다.

//===================================================================================
void CfrmStampView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_StampView(this);

	Pos_Sel= POSITION_READY;

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

	SetTimer(frmStampView_TIMER_ID, frmStampView_TIMER_INTERVAL, NULL);

	GetDlgItem(IDB_JOG_STAGE_Y_CCW2)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_STAGE_Y_CW2)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X
	GetDlgItem(IDB_JOG_STAGE_T_CW2)->EnableWindow(FALSE);		GetDlgItem(IDB_JOG_STAGE_T_CCW2)->EnableWindow(FALSE); //_ACS_AXIS_STAGE_X

	DataUpdate();

	m_manuStamp_Camera= eTOP_CAM;
	m_btnManuStamp_TopUsed.SetValue(1); m_btnManuStamp_BtmUsed.SetValue(0);

	g_bCfrmStampViewInit= TRUE;
}

//===================================================================================
void CfrmStampView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmStampView_TIMER_ID);

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmStampView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmStampView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmStampView::OnTimer(UINT_PTR nIDEvent)
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
void CfrmStampView::DataUpdate()
	//-----------------------------------------------------------------------------------
{
}
//===================================================================================
void CfrmStampView::DataSave()
//-----------------------------------------------------------------------------------
{
}

//===================================================================================
void CfrmStampView::DispUpdate()
//-----------------------------------------------------------------------------------
{
}
//===================================================================================
void CfrmStampView::JogDispUpdate()
//-----------------------------------------------------------------------------------
{
	switch(iStageJog_Select)
	{
	case STAGE_SEL_1:		break;
	case STAGE_SEL_2:		break;
	}
}


//===================================================================================
BEGIN_EVENTSINK_MAP(CfrmStampView, CFormView)
	ON_EVENT(CfrmStampView, IDB_Y_POS0, DISPID_CLICK, CfrmStampView::ClickYPos0, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Y_POS8, DISPID_CLICK, CfrmStampView::ClickYPos8, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Y_POS1, DISPID_CLICK, CfrmStampView::ClickYPos1, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Y_POS9, DISPID_CLICK, CfrmStampView::ClickYPos9, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Y_POS10, DISPID_CLICK, CfrmStampView::ClickYPos10, VTS_NONE)
	ON_EVENT(CfrmStampView, IDC_BUTTON_JOG_POPUP, DISPID_CLICK, CfrmStampView::ClickButtonJogPopup, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_STAGE_JOG_SEL1, DISPID_CLICK, CfrmStampView::ClickStageJogSel1, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_STAGE_JOG_SEL2, DISPID_CLICK, CfrmStampView::ClickStageJogSel2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_SELECT_0, DISPID_CLICK, CfrmStampView::ClickJogSelect0, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_SELECT_1, DISPID_CLICK, CfrmStampView::ClickJogSelect1, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_SELECT_2, DISPID_CLICK, CfrmStampView::ClickJogSelect2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_SELECT_3, DISPID_CLICK, CfrmStampView::ClickJogSelect3, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_POS_SET2, DISPID_CLICK, CfrmStampView::ClickPosSet2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE, DISPID_CLICK, CfrmStampView::ClickCmMove, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE2, DISPID_CLICK, CfrmStampView::ClickCmMove2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Z_WAIT_POS, DISPID_CLICK, CfrmStampView::ClickZWaitPos, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Z_WAIT_POS7, DISPID_CLICK, CfrmStampView::ClickZWaitPos7, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MANUAL_SPEED, DISPID_CLICK, CfrmStampView::ClickCmManualSpeed, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MANUAL_SPEED2, DISPID_CLICK, CfrmStampView::ClickCmManualSpeed2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_DATA_SAVE, DISPID_CLICK, CfrmStampView::ClickDataSave, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_X_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_X_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_X_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_X_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_PB_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_PB_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogPbCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_PB_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_PB_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogPbCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z1_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z1_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogZ1Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z2_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z2_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogZ2Ccw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z2_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z2_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogZ2Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogZCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogZCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z1_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_Z1_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogZ1Cw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_Y_CCW2, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageYCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_Y_CW2, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageYCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CW2, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageTCw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CCW2, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageTCcw2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_X_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageXCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_X_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_X_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageXCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_CAMERA_Y_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogCameraYCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_CAMERA_Y_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogCameraYCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageTCw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEDOWN, CfrmStampView::MouseDownJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_JOG_STAGE_T_CCW, DISPID_MOUSEUP, CfrmStampView::MouseUpJogStageTCcw, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CfrmStampView, IDB_VACUUM_STAGE1_ON, DISPID_CLICK, CfrmStampView::ClickVacuumStage1On, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_VACUUM_STAGE1_OFF, DISPID_CLICK, CfrmStampView::ClickVacuumStage1Off, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_VACUUM_STAGE2_ON, DISPID_CLICK, CfrmStampView::ClickVacuumStage2On, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_VACUUM_STAGE2_OFF, DISPID_CLICK, CfrmStampView::ClickVacuumStage2Off, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Z_WAIT_POS3, DISPID_CLICK, CfrmStampView::ClickZWaitPos3, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Z_WAIT_POS5, DISPID_CLICK, CfrmStampView::ClickZWaitPos5, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_Z_WAIT_POS8, DISPID_CLICK, CfrmStampView::ClickZWaitPos8, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE6, DISPID_CLICK, CfrmStampView::ClickCmMove6, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_WAIT_MOVE, DISPID_CLICK, CfrmStampView::ClickCmWaitMove, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_SELECT_TUSED2, DISPID_CLICK, CfrmStampView::ClickJogSelectTused2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_JOG_SELECT_BUSED2, DISPID_CLICK, CfrmStampView::ClickJogSelectBused2, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_MARK_TEST4, DISPID_CLICK, CfrmStampView::ClickMarkTest4, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE7, DISPID_CLICK, CfrmStampView::ClickCmMove7, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE14, DISPID_CLICK, CfrmStampView::ClickCmMove14, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE15, DISPID_CLICK, CfrmStampView::ClickCmMove15, VTS_NONE)
	ON_EVENT(CfrmStampView, IDB_CM_MOVE16, DISPID_CLICK, CfrmStampView::ClickCmMove16, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CfrmStampView::ClickYPos0()
{	Pos_Sel= POSITION_READY;						DataUpdate(); }
//===================================================================================
void CfrmStampView::ClickYPos8()
{	Pos_Sel= POSITION_STAGE1_ALIGN;					DataUpdate(); }
//===================================================================================
void CfrmStampView::ClickYPos1()
{	Pos_Sel= POSITION_STAGE2_ALIGN;					DataUpdate(); }
//===================================================================================
void CfrmStampView::ClickYPos9()
{	Pos_Sel= POSITION_ROLL_ALIGN;					DataUpdate(); }
//===================================================================================
void CfrmStampView::ClickYPos10()
{	Pos_Sel= POSITION_SYNC;							DataUpdate(); }
//===================================================================================
void CfrmStampView::ClickButtonJogPopup()
{
	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
		m_btnCopySel[i].SetValue(1);
	}
}
//===================================================================================
void CfrmStampView::ClickStageJogSel1()
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

void CfrmStampView::ClickStageJogSel2()
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
void CfrmStampView::ClickJogSelect0()
{
	Jog_Mode= _MAX_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmStampView::ClickJogSelect1()
{
	Jog_Mode= _MID_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmStampView::ClickJogSelect2()
{
	Jog_Mode= _MIN_SPEED;
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		
}
//===================================================================================
void CfrmStampView::ClickJogSelect3()
{
	Jog_Mode= _PTP_MOVE;
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
}

//===================================================================================
void CfrmStampView::ClickPosSet2()
{
/*
	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;

	CString strText, strTitle;
	CString strPos;
	double ToPosition= 0.0;
	strText= _T("");	strTitle= _T("");
	double UpData[_MAX_AXIS];
	int i;

	for(i= 0; i< _MAX_AXIS; i++)	UpData[i]= 0.0;

	UpData[_ACS_AXIS_IMP_ROLL_Z1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
	UpData[_ACS_AXIS_IMP_ROLL_Z2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);

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

	for(i= 0; i< _MAX_AXIS; i++)
	{
		if(	m_btnCopySel[i].GetValue())		{			ParamFile()->Model_data.dPosition[dispPos_Sel][i]= UpData[i];		}
	}
	switch(dispPos_Sel)
	{
	case POSITION_READY:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	
		{
			if(UpData[_ACS_AXIS_IMP_ROLL_Z1] > 0.0) UpData[_ACS_AXIS_IMP_ROLL_Z1]= 0.0;
			ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
		}
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())
		{
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
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];

		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]=  ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]=  ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_STAGE2_ALIGN:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];

		if(	m_btnCopySel[_ACS_AXIS_STAGE_X].GetValue())		ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_STAGE_X]= UpData[_ACS_AXIS_STAGE_X];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL]= UpData[_ACS_AXIS_IMP_ROLL];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Z]= UpData[_ACS_AXIS_UPPER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_UPPER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_UPPER_CAM_Y]= UpData[_ACS_AXIS_UPPER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_X].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_X]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_X];//UpData[_ACS_AXIS_LOWER_CAM_X];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Y].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Y]= ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_LOWER_CAM_Y];//UpData[_ACS_AXIS_LOWER_CAM_Y];
		if(	m_btnCopySel[_ACS_AXIS_LOWER_CAM_Z].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_LOWER_CAM_Z]= 0.0;//UpData[_ACS_AXIS_LOWER_CAM_Z];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y1]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y1];
		if(	m_btnCopySel[_ACS_AXIS_ALIGN_Y2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_ALIGN_Y2]= 0.0;//UpData[_ACS_AXIS_ALIGN_Y2];
		break;
	case POSITION_ROLL_ALIGN:	
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];

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
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];

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
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= 0.0;//UpData[_ACS_AXIS_IMP_ROLL_Z1];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= 0.0;//UpData[_ACS_AXIS_IMP_ROLL_Z2];

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
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z1].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z1]= UpData[_ACS_AXIS_IMP_ROLL_Z1];
		if(	m_btnCopySel[_ACS_AXIS_IMP_ROLL_Z2].GetValue())	ParamFile()->Model_data.dPosition[dispPos_Sel][_ACS_AXIS_IMP_ROLL_Z2]= UpData[_ACS_AXIS_IMP_ROLL_Z2];

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
	default:
		MyMessageBox(_T("설정이 실패했습니다.\n위치 선택 확인!!! [MAX]"), _T("Position-Save Failed"), M_ICONERROR);
		return;
	}
	ParamFile()->SaveFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadFile();

	DataUpdate();
*/
}

//===================================================================================
void CfrmStampView::ClickCmMove()
{
	MovePos(Pos_Sel, TRUE);
}

void CfrmStampView::MovePos(int pos, BOOL Z_Flag)
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
void CfrmStampView::ClickCmMove2()//Z-Wait & All-Move
{
	MovePos(Pos_Sel, FALSE);
}

//===================================================================================
void CfrmStampView::ClickZWaitPos() // Z-Up
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
void CfrmStampView::ClickZWaitPos7() // Z-Down
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
		if(chaval > 3.0)
		{
			MyMessageBox(_T("이동이 실패했습니다.\nX축 위치가 설정 위치를 벗어나 있습니다."), _T("Position-Move Failed"), M_ICONERROR); return ;
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
		if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
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
		if(!pSystem.GetMotionComponent()->Roll_Z_Down_Proc(dispPos_Sel)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
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
void CfrmStampView::ClickCmManualSpeed()
{
//	CdlgData_MotionSpeed dlg;
	//dlg.DoModal();
}

//===================================================================================
void CfrmStampView::ClickCmManualSpeed2()
{
	CdlgData_MotionSpeed dlg;
	dlg.DoModal();
}

//===================================================================================
void CfrmStampView::ClickDataSave()
{
}
//===================================================================================
//===================================================================================
void CfrmStampView::MouseDownJogXCw(short Button, short Shift, long x, long y)
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
void CfrmStampView::MouseUpJogXCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================


void CfrmStampView::MouseDownJogXCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogXCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_STAGE_X);
}
//===================================================================================
void CfrmStampView::MouseDownJogPbCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogPbCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CfrmStampView::MouseDownJogPbCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogPbCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
}
//===================================================================================
void CfrmStampView::MouseDownJogZ1Ccw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogZ1Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmStampView::MouseDownJogZ1Cw(short Button, short Shift, long x, long y)
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

void CfrmStampView::MouseUpJogZ1Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}

//===================================================================================
void CfrmStampView::MouseDownJogZ2Ccw(short Button, short Shift, long x, long y)
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

void CfrmStampView::MouseUpJogZ2Ccw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmStampView::MouseDownJogZ2Cw(short Button, short Shift, long x, long y)
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

void CfrmStampView::MouseUpJogZ2Cw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmStampView::MouseDownJogZCcw(short Button, short Shift, long x, long y)
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

void CfrmStampView::MouseUpJogZCcw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================
void CfrmStampView::MouseDownJogZCw(short Button, short Shift, long x, long y)
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

void CfrmStampView::MouseUpJogZCw(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
}
//===================================================================================

void CfrmStampView::MouseDownJogStageYCcw2(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageYCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmStampView::MouseDownJogStageYCw2(short Button, short Shift, long x, long y)
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
void CfrmStampView::MouseUpJogStageYCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmStampView::MouseDownJogStageTCw2(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageTCw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmStampView::MouseDownJogStageTCcw2(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageTCcw2(short Button, short Shift, long x, long y)
{
	if(Jog_Mode!= _PTP_MOVE) 
		pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
}
//===================================================================================


void CfrmStampView::MouseDownJogStageXCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageXCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseDownJogStageXCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageXCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseDownJogCameraYCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogCameraYCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseDownJogCameraYCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogCameraYCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseDownJogStageTCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageTCw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseDownJogStageTCcw(short Button, short Shift, long x, long y)
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


void CfrmStampView::MouseUpJogStageTCcw(short Button, short Shift, long x, long y)
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
BOOL CfrmStampView::MoveEndCheck(int Axis) 
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


void CfrmStampView::OnBnClickedButton2()
{
// 	CString strText;
// 	strText.Format("모터 이동이 실패했습니다.\n %s [%d]", pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1);
// 	MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); 
//	MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1) ;
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
}

//===================================================================================
void CfrmStampView::ClickVacuumStage1On()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmStampView::ClickVacuumStage1Off()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmStampView::ClickVacuumStage2On()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmStampView::ClickVacuumStage2Off()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmStampView::OnBnClickedCheck1()
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
void CfrmStampView::ClickZWaitPos3()
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
void CfrmStampView::ClickZWaitPos5()
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
void CfrmStampView::ClickZWaitPos8()
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

void CfrmStampView::ClickCmMove6()
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
	CameraStageUsedNo= m_manuStamp_Camera; //= eTOP_CAM;

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

//===================================================================================
void CfrmStampView::ClickCmWaitMove()
{
	MovePos(POSITION_READY, FALSE);
}

//===================================================================================
void CfrmStampView::ClickJogSelectTused2()
{
	m_manuStamp_Camera= eTOP_CAM;
}


void CfrmStampView::ClickJogSelectBused2()
{
	m_manuStamp_Camera= eBTM_CAM;
}
//===================================================================================

void CfrmStampView::ClickMarkTest4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	proc_run_flag= true;
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CString strText;

	strText.Format(_T("스템프 Align 동작을 진행 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("Align"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
//		proc_run_flag= false;
		return;
	}
	if(m_manuStamp_Camera== eBTM_CAM)
	{
		if(ParamFile()->Model_data.dAlignMarkToMark[MANUAL_STAMP_PROC] > 25.0)
		{
			MyMessageBox(_T("스탬프 길이가 25.0mm보다 큽니다."), _T("Failed"), M_ICONERROR); return ;
		}
	}

	CdlgManuAlign dlg;
// 	if(m_manuStamp_Camera== eTOP_CAM)
// 	{
// 		MyMessageBox(_T("상부 카메라는 지원하지 않습니다."), _T("Align-Mode"), M_ICONINFORMATION);
// 		return;
// 	}

	dlg.m_StageNo= MANUAL_STAMP_PROC;
	dlg.CameraStageUsedNo= m_manuStamp_Camera; dlg.iStamInstRunFlag= 1;
	if(dlg.DoModal()== IDCANCEL)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ 스템프 ALIGN-FAIL ] [스템프 ALIGN_MODE]"));
	}

//	proc_run_flag= false;
}

//===================================================================================
void CfrmStampView::OnBnClickedButton6()
{
	pSystem.GetMotionComponent()->Sync_Mode_Set(STAGE_1_ALIGN_PROC);
}

//===================================================================================
void CfrmStampView::OnBnClickedButton7()
{
	pSystem.GetMotionComponent()->Sync_Mode_Reset();
}

//===================================================================================
void CfrmStampView::ClickCmMove7()
{
	CString strText;
	if(m_manuStamp_Camera== eTOP_CAM)
	{
		strText.Format(_T("스템프 장착 위치로 이동 하시겠습니까?"));
		if (MyMessageBox(strText.GetBuffer(0), _T("Align"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)	
		{
			double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;
	//		double ToPosition1, ToPosition2;

			pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
			double YopStampPosX= L_X;
			double Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);

			pSystem.GetMotionComponent()->GetStampPos(eBTM_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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

			ParamFile()->Model_data.dPosition_Offset[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= 0.0;
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
			//			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();

			double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_STAGE3_ALIGN);
			double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_STAGE3_ALIGN);
			double chaZ= ToPosition2 - ToPosition1;
			pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, chaZ,	FALSE);
		}
	}
	CdlgStampInst_Jog dlg;
	dlg.DoModal();
}

//===================================================================================
void CfrmStampView::ClickCmMove14()
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

//===================================================================================
void CfrmStampView::ClickCmMove15()
{
	CdlgStampInst_Jog dlg;
	dlg.DoModal();
}

//===================================================================================
void CfrmStampView::ClickCmMove16()
{
	CdlgRollForce_Test dlg;
	dlg.DoModal();
}
