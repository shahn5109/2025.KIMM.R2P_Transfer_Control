// frmMainView.cpp : 구현 파일입니다.
//
//0:LC1,1:LC2,2:X-Pos,3:X-Vel,4:Enc

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmMainView.h"
#include "MainFrm.h"
#include "dlgOrigin.h"
#include "dlgStopModeSel.h"
#include "dlgAutoRunInfo.h"
#include "ComiAxcommon.h"
#include "dlgModelLoad.h"
#include "dlgRecipeSet.h"

// CfrmMainView
extern CSystemClass pSystem;
CParameterFile* CfrmMainView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmMainView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmMainView, CFormView)

	CfrmMainView::CfrmMainView()
	: CFormView(CfrmMainView::IDD)
	, m_Wait_Flag(FALSE)
{
	bGr_Flag1= false;
}

CfrmMainView::~CfrmMainView()
{
}

void CfrmMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMIGRAPHCTRL1, m_cnGraph1);
	DDX_Control(pDX, IDB_VACUUM_ON4, m_btnFoot_CW);
	DDX_Control(pDX, IDB_READY, m_btnReady);

	DDX_Control(pDX, IDC_TEACHING_POS3, m_lblBase[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDC_TEACHING_POS, m_lblBase[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDC_TEACHING_POS2, m_lblBase[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDC_TEACHING_POS4, m_lblBase[STAGE_3_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z1, m_lblRun_RollMode_Roll[ROLL_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z25, m_lblRunEnable[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z24, m_lblRunEnable[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z26, m_lblRunEnable[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z51, m_lblRunEnable[STAGE_3_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z30, m_lblAlignMode_None[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z28, m_lblAlignMode_None[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z31, m_lblAlignMode_None[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z40, m_lblAlignMode_None[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z2, m_lblAlignMode_Auto[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z7, m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z8, m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z44, m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z3, m_lblAlignMode_Manu[ROLL_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z10, m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z11, m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z45, m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC]);

	DDX_Control(pDX, IDB_OPU_ROLL_Z12, m_lblRun_Speed[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z13, m_lblRun_Speed[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z46, m_lblRun_Speed[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z14, m_lblRun_Length[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z15, m_lblRun_Length[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z38, m_lblRun_Length[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z16, m_lblRun_Force[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z17, m_lblRun_Force[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z47, m_lblRun_Force[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z18, m_lblRun_DownSpeed[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z20, m_lblRun_DownSpeed[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z48, m_lblRun_DownSpeed[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z19, m_lblRun_UpSpeed[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z21, m_lblRun_UpSpeed[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z49, m_lblRun_UpSpeed[STAGE_3_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z22, m_lblRun_Delay[STAGE_1_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z23, m_lblRun_Delay[STAGE_2_ALIGN_PROC]);
	DDX_Control(pDX, IDB_OPU_ROLL_Z50, m_lblRun_Delay[STAGE_3_ALIGN_PROC]);
	DDX_Check(pDX, IDC_CHECK2, m_Wait_Flag);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CfrmMainView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MSG_DATA_UPDATE, DataReload)
	ON_BN_CLICKED(IDC_BUTTON1, &CfrmMainView::OnBnClickedButton1)
	ON_MESSAGE(WM_MSG_OFF_GRAPH_DATA_UPDATE1, LoadCell_Get_Data1)
	ON_MESSAGE(WM_MSG_OFF_GRAPH_DATA_UPDATE2, LoadCell_Get_Data2)
	ON_MESSAGE(WM_MSG_OFF_GRAPH_DATA_UPDATE3, LoadCell_Get_Data3)
	ON_MESSAGE(WM_MSG_OFF_GRAPH_DATA_UPDATE4, LoadCell_Get_Data4)
	ON_MESSAGE(WM_MSG_OFF_GRAPH_DATA_UPDATE5, LoadCell_Get_Data5)
	ON_BN_CLICKED(IDC_CHECK2, &CfrmMainView::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CfrmMainView 진단입니다.

#ifdef _DEBUG
void CfrmMainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmMainView 메시지 처리기입니다.

//===================================================================================
void CfrmMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_MainView(this);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_bCfrmMainViewInit= TRUE;

	SetTimer(frmMainView_TIMER_ID, frmMainView_TIMER_INTERVAL, NULL);

	m_btnFoot_CW.SetValue(1);



	PlotData_Time_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_L1_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_L2_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_XP_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_ZP_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_Z2P_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_XS_1= new double [_MAX_LOADCELL_NO]; 
	PlotData_Enc_1= new double [_MAX_LOADCELL_NO]; 

	PlotData_Time_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_L1_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_L2_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_XP_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_ZP_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_Z2P_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_XS_2= new double [_MAX_LOADCELL_NO]; 
	PlotData_Enc_2= new double [_MAX_LOADCELL_NO]; 

	PlotData_Time_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_L1_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_L2_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_XP_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_ZP_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_Z2P_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_XS_3= new double [_MAX_LOADCELL_NO]; 
	PlotData_Enc_3= new double [_MAX_LOADCELL_NO]; 

	PlotData_Time_4= new double [_MAX_LOADCELL_NO]; 
	PlotData_L1_S= new double [_MAX_LOADCELL_NO]; 
	PlotData_L2_S= new double [_MAX_LOADCELL_NO]; 
	PlotData_XP_S= new double [_MAX_LOADCELL_NO]; 
	PlotData_ZP_S= new double [_MAX_LOADCELL_NO]; 
	PlotData_Z2P_S= new double [_MAX_LOADCELL_NO]; 
	PlotData_XS_S= new double [_MAX_LOADCELL_NO]; 
	PlotData_Enc_S= new double [_MAX_LOADCELL_NO]; 

	g_FootSW_Dir= _DIR_CW; m_btnFoot_CW.SetValue(1);

	m_ListCtrl.SetBkColor(RGB(255,230,230));
	m_ListCtrl.SetTextBkColor(RGB(255,230,230));
	// 리스트 컨트롤의 행 선택,Flat,라인.. 스타일
	m_ListCtrl.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("Command"), LVCFMT_RIGHT, 120, 0);
	m_ListCtrl.InsertColumn(1, _T("Op1"), LVCFMT_RIGHT, 80, 0);
	m_ListCtrl.InsertColumn(2, _T("Op2"), LVCFMT_RIGHT, 90, 0);
	m_ListCtrl.InsertColumn(3, _T("Op3"), LVCFMT_RIGHT, 70, 0);
	m_ListCtrl.InsertColumn(4, _T("Op4"), LVCFMT_RIGHT, 65, 0);
	m_ListCtrl.InsertColumn(5, _T("Op5"), LVCFMT_RIGHT, 70, 0);
	m_ListCtrl.InsertColumn(6, _T(" "), LVCFMT_RIGHT, 70, 0);

	ListCnt= 0;

	DataUpdate();
	GraphDataInital() ;
	CString strText;
	strText.Format(_T("%s"), ParamFile()->System_data.strCurrentModelName);
	SetDlgItemText(IDC_MODEL_NAME, strText.GetBuffer(0));

	for(int i= 0; i< _MAX_ALIGN_RESULT_DATA; i++)
		ParamFile()->Align_Result_data[i].iAlignEndFlag= 0;
}

//===================================================================================
void CfrmMainView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmMainView_TIMER_ID);

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();

	delete [] PlotData_Time_1; 
	delete [] PlotData_L1_1; 
	delete [] PlotData_L2_1; 
	delete [] PlotData_XP_1; 
	delete [] PlotData_ZP_1; 
	delete [] PlotData_Z2P_1; 
	delete [] PlotData_XS_1; 
	delete [] PlotData_Enc_1; 

	delete [] PlotData_Time_2; 
	delete [] PlotData_L1_2; 
	delete [] PlotData_L2_2; 
	delete [] PlotData_XP_2; 
	delete [] PlotData_ZP_2; 
	delete [] PlotData_Z2P_2; 
	delete [] PlotData_XS_2; 
	delete [] PlotData_Enc_2; 

	delete [] PlotData_Time_3; 
	delete [] PlotData_L1_3; 
	delete [] PlotData_L2_3; 
	delete [] PlotData_XP_3; 
	delete [] PlotData_ZP_3; 
	delete [] PlotData_Z2P_3; 
	delete [] PlotData_XS_3; 
	delete [] PlotData_Enc_3; 

	delete [] PlotData_Time_4; 
	delete [] PlotData_L1_S; 
	delete [] PlotData_L2_S; 
	delete [] PlotData_XP_S; 
	delete [] PlotData_ZP_S; 
	delete [] PlotData_Z2P_S; 
	delete [] PlotData_XS_S; 
	delete [] PlotData_Enc_S; 
}

//===================================================================================
void CfrmMainView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmMainView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmMainView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString strText;

	strText.Format(_T("F:%.1f"), pSystem.GetMotionComponent()->GetADValue(_AD_LOADCELL_F));			SetDlgItemText(IDS_COMMAND, strText.GetBuffer(0));
	strText.Format(_T("R:%.1f"), pSystem.GetMotionComponent()->GetADValue(_AD_LOADCELL_R));			SetDlgItemText(IDS_COMMAND2, strText.GetBuffer(0));
	
	switch(ParamFile()->System_data.iCURRENT_RUN_STATUS)
	{
	case _CURRENT_RUN_STOP:
//		if(!g_Buzzer_Enable) pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_OFF);
		if(pSystem.GetMotionComponent()->isAllMotorHomeEnd())
		{
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_OFF);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,		_ON);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_OFF);
		}
		else
		{
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_OFF);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,		_OFF);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_OFF);
		}
		break;
	case _CURRENT_RUN_RUN:
//		if(!g_Buzzer_Enable) pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_OFF);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_OFF);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,	_OFF);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_ON);
		break;
	case _CURRENT_RUN_PAUSE:
//		if(!g_Buzzer_Enable) pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_OFF);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_OFF);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_ON);
		break;
	case _CURRENT_RUN_ERROR:
//		pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_ON);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_ON);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,	_ON);
			pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_ON);
		break;
	case _CURRENT_RUN_ERROR1: // POWER OFF
		//		pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,	_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_ON);
		break;
	case _CURRENT_RUN_ERROR2: // MOTOR-ERROR
		//		pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,	_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_ON);
		break;
	default:
		//		pSystem.GetMotionComponent()->OutputBit(O_BUZZER,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_R,		_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_Y,	_ON);
		pSystem.GetMotionComponent()->OutputBit(_OUTPUT_TowerRamp_G,		_ON);
		break;
	}
		switch(ParamFile()->System_data.iCURRENT_RUN_STATUS)
		{
		case _CURRENT_RUN_STOP:
			strText.Format(_T("AUTO_STOP"));
			break;
		case _CURRENT_RUN_RUN:
			strText.Format(_T("AUTO_RUN"));
			break;
		case _CURRENT_RUN_PAUSE:
			strText.Format(_T("AUTO_PAUSE"));
			break;
		case _CURRENT_RUN_ERROR:
			strText.Format(_T("AUTO_ERROR"));
			break;
		case _CURRENT_RUN_ERROR1: // POWER OFF
			strText.Format(_T("AUTO_ERROR"));
			break;
		case _CURRENT_RUN_ERROR2: // MOTOR-ERROR
			strText.Format(_T("AUTO_ERROR"));
			break;
		default:
			strText.Format(_T("AUTO_???"));
			break;
		}
	SetDlgItemText(IDS_STATIC_RUN_STATUS, strText.GetBuffer(0));

		switch(ParamFile()->System_data.iWORK_PROCESS_MODE)
		{
		case _PROCESS_AUTO_RUN:		strText.Format(_T("AUTO_RUN"));
			break;
		case _PROCESS_CYCLE_RUN:	strText.Format(_T("CYCLE_RUN"));		
			break;
		default:					strText.Format(_T("_RUN_MODE ?"));
			break;
		}


	SetDlgItemText(IDS_STATIC_RUN_MODE, strText.GetBuffer(0));
	strText.Format(_T("%04d"), pSystem.GetProcessThread_MainComponent()->GetStep());
	SetDlgItemText(IDS_STATIC_STEP, strText.GetBuffer(0));


	if(ParamFile()->System_data.iCURRENT_RUN_STATUS != _CURRENT_RUN_RUN)
	{
			if(pSystem.GetMotionComponent()->InputBit(_INPUT_SW_Foot))
			{
				m_bFootSW_Flag= true;
				if(pSystem.GetMotionComponent()->isDone(_ACS_AXIS_IMP_ROLL))
				{
					if(g_FootSW_Dir== _DIR_CW)
						pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL, _DIR_CW, ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL]);
					else
						pSystem.GetMotionComponent()->JogStart(_ACS_AXIS_IMP_ROLL, _DIR_CCW, ParamFile()->Motion_data.dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL]);
				}
			}
			else
			{
				if(m_bFootSW_Flag)
					pSystem.GetMotionComponent()->JogStop(_ACS_AXIS_IMP_ROLL);
				m_bFootSW_Flag= false;
			}
	}
	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
LRESULT CfrmMainView::DataReload(WPARAM wParam,LPARAM lParam)
{
	DataUpdate();
	return 0;
}
//===================================================================================
void CfrmMainView::DataUpdate()
	//-----------------------------------------------------------------------------------
{
	CString strTmp;

	//	if(Pos_Sel>= _MAX_MOVE_SET_POS) return;
	ParamFile()->OpenSytemConfigFile();
	ParamFile()->OpenModelConfigFile(ParamFile()->System_data.strCurrentModelName);

		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_Speed[STAGE_1_ALIGN_PROC]);		m_lblRun_Speed[STAGE_1_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));	
		strTmp.Format(_T("%.3f mm"), ParamFile()->Model_data.dRun_Length[STAGE_1_ALIGN_PROC]);			m_lblRun_Length[STAGE_1_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%.1f kg (편측)"), ParamFile()->Model_data.dRun_Force[STAGE_1_ALIGN_PROC]);	m_lblRun_Force[STAGE_1_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_DownSpeed[STAGE_1_ALIGN_PROC]);	m_lblRun_DownSpeed[STAGE_1_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));	
		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_UpSpeed[STAGE_1_ALIGN_PROC]);		m_lblRun_UpSpeed[STAGE_1_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%d msec"), ParamFile()->Model_data.iRun_Delay[STAGE_1_ALIGN_PROC]);			m_lblRun_Delay[STAGE_1_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));

		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_Speed[STAGE_2_ALIGN_PROC]);		m_lblRun_Speed[STAGE_2_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));	
		strTmp.Format(_T("%.3f mm"), ParamFile()->Model_data.dRun_Length[STAGE_2_ALIGN_PROC]);			m_lblRun_Length[STAGE_2_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%.1f kg (편측)"), ParamFile()->Model_data.dRun_Force[STAGE_2_ALIGN_PROC]);	m_lblRun_Force[STAGE_2_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_DownSpeed[STAGE_2_ALIGN_PROC]);	m_lblRun_DownSpeed[STAGE_2_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));	
		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_UpSpeed[STAGE_2_ALIGN_PROC]);		m_lblRun_UpSpeed[STAGE_2_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%d msec"), ParamFile()->Model_data.iRun_Delay[STAGE_2_ALIGN_PROC]);			m_lblRun_Delay[STAGE_2_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));

		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_Speed[STAGE_3_ALIGN_PROC]);		m_lblRun_Speed[STAGE_3_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));	
		strTmp.Format(_T("%.3f mm"), ParamFile()->Model_data.dRun_Length[STAGE_3_ALIGN_PROC]);			m_lblRun_Length[STAGE_3_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%.1f kg (편측)"), ParamFile()->Model_data.dRun_Force[STAGE_3_ALIGN_PROC]);	m_lblRun_Force[STAGE_3_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_DownSpeed[STAGE_3_ALIGN_PROC]);	m_lblRun_DownSpeed[STAGE_3_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));	
		strTmp.Format(_T("%.3f mm/sec"), ParamFile()->Model_data.dRun_UpSpeed[STAGE_3_ALIGN_PROC]);		m_lblRun_UpSpeed[STAGE_3_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));
		strTmp.Format(_T("%d msec"), ParamFile()->Model_data.iRun_Delay[STAGE_3_ALIGN_PROC]);			m_lblRun_Delay[STAGE_3_ALIGN_PROC].SetCaption(strTmp.GetBuffer(0));

		//---------------------------
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE)
		{	
			m_lblBase[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(255,255,255));
			m_lblRunEnable[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));	m_lblRunEnable[ROLL_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblRunEnable[ROLL_ALIGN_PROC].SetCaption(_T("Enable"));
		}
		else
		{
			m_lblBase[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(200, 200, 200));
			m_lblRunEnable[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblRunEnable[ROLL_ALIGN_PROC].SetForeColor(0x00000000);
			m_lblRunEnable[ROLL_ALIGN_PROC].SetCaption(_T("Disable"));
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC]== RUN_ENABLE)
		{
			m_lblBase[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(255,255,255));
			m_lblRunEnable[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));	m_lblRunEnable[STAGE_1_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblRunEnable[STAGE_1_ALIGN_PROC].SetCaption(_T("Enable"));
		}
		else
		{
			m_lblBase[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(200, 200, 200));
			m_lblRunEnable[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblRunEnable[STAGE_1_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblRunEnable[STAGE_1_ALIGN_PROC].SetCaption(_T("Disable"));
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC]== RUN_ENABLE)
		{
			m_lblBase[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(255,255,255));
			m_lblRunEnable[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));	m_lblRunEnable[STAGE_2_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblRunEnable[STAGE_2_ALIGN_PROC].SetCaption(_T("Enable"));
		}
		else
		{
			m_lblBase[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(200, 200, 200));
			m_lblRunEnable[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblRunEnable[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblRunEnable[STAGE_2_ALIGN_PROC].SetCaption(_T("Disable"));
		}
		if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE)
		{
			m_lblBase[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(255,255,255));
			m_lblRunEnable[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));	m_lblRunEnable[STAGE_3_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblRunEnable[STAGE_3_ALIGN_PROC].SetCaption(_T("Enable"));
		}
		else
		{
			m_lblBase[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(200, 200, 200));
			m_lblRunEnable[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblRunEnable[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblRunEnable[STAGE_3_ALIGN_PROC].SetCaption(_T("Disable"));
		}
		//---------------------------
		if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		{	
			
			SetDlgItemText(IDC_OPMODE_3, _T("ROLL Stamp 사용"));
			m_lblRun_RollMode_Roll[ROLL_ALIGN_PROC].SetCaption(_T("ROLL 회전"));	
			GetDlgItem(IDB_OPU_ROLL_Z12)->EnableWindow(TRUE);
			GetDlgItem(IDB_OPU_ROLL_Z14)->EnableWindow(TRUE);
			GetDlgItem(IDB_OPU_ROLL_Z13)->EnableWindow(TRUE);
			GetDlgItem(IDB_OPU_ROLL_Z15)->EnableWindow(TRUE);
		}
		else
		{
			SetDlgItemText(IDC_OPMODE_3, _T("평 Stamp 사용"));
			m_lblRun_RollMode_Roll[ROLL_ALIGN_PROC].SetCaption(_T("평 Stamp"));	
			GetDlgItem(IDB_OPU_ROLL_Z12)->EnableWindow(FALSE);
			GetDlgItem(IDB_OPU_ROLL_Z14)->EnableWindow(FALSE);
			GetDlgItem(IDB_OPU_ROLL_Z13)->EnableWindow(FALSE);
			GetDlgItem(IDB_OPU_ROLL_Z15)->EnableWindow(FALSE);
		}
		//---------------------------

		if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else
		{	
			m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}

		if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else
		{	
			m_lblAlignMode_Manu[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Manu[ROLL_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Auto[STAGE_1_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[ROLL_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}

		if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else
		{	
			m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Manu[STAGE_2_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[STAGE_2_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_NONE_MODE)
		{	
			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}
		else
		{	
			m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(255,0,0));			m_lblAlignMode_Manu[STAGE_3_ALIGN_PROC].SetForeColor(RGB(255,255,255));
			m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_None[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
			m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetBackColorInterior(RGB(227, 227, 227));	m_lblAlignMode_Auto[STAGE_3_ALIGN_PROC].SetForeColor(RGB(0,0,0));
		}

		if(ParamFile()->Model_data.iRoll_AngleAlignMode== RUN_DISABLE)	strTmp.Format(_T("★ Angle 계산 : 2 Point"));
		else															strTmp.Format(_T("★ Angle 계산 : Multi-Point\n- 간격:%.3fmm,-Count:%d"),
													ParamFile()->Model_data.dRoll_AngleAlign_Pitch, ParamFile()->Model_data.iRoll_AngleAlign_Count);
		SetDlgItemText(IDS_COMMAND7, strTmp.GetBuffer(0));
	ListUpdate();

	UpdateData(FALSE);
}
//===================================================================================
void CfrmMainView::DataSave()
	//-----------------------------------------------------------------------------------
{
/*
	UpdateData(TRUE);

	int i;
	for(i= 0; i< _MAX_RUN_PARAM; i++)
	{
		ParamFile()->Model_data.iRunEnable[i]= m_iRunEnable[i];
		ParamFile()->Model_data.iAlignMode[i]= m_iAlignMode[i];
		ParamFile()->Model_data.iRun_RollMode[i]= m_iRun_RollMode[i];

		ParamFile()->Model_data.dRun_Speed[i]= m_dRun_Speed[i];
		ParamFile()->Model_data.dRun_Length[i]= m_dRun_Length[i];
		ParamFile()->Model_data.dRun_Force[i]= m_dRun_Force[i];
		ParamFile()->Model_data.dRun_DownSpeed[i]= m_dRun_DownSpeed[i];
		ParamFile()->Model_data.dRun_UpSpeed[i]= m_dRun_UpSpeed[i];
		ParamFile()->Model_data.iRun_Delay[i]= m_iRun_Delay[i];
	}

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CfrmDataView 수정 / 저장"));
	ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveSytemConfigFile();

*/
}
BEGIN_EVENTSINK_MAP(CfrmMainView, CFormView)
	ON_EVENT(CfrmMainView, IDB_ORIGIN_RETURN3, DISPID_CLICK, CfrmMainView::ClickOriginReturn3, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_ORIGIN_RETURN, DISPID_CLICK, CfrmMainView::ClickOriginReturn, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_ORIGIN_RETURN2, DISPID_CLICK, CfrmMainView::ClickOriginReturn2, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_READY, DISPID_CLICK, CfrmMainView::ClickReady, VTS_NONE)
	ON_EVENT(CfrmMainView, IDS_COMMAND, DISPID_DBLCLICK, CfrmMainView::DblClickCommand, VTS_NONE)
	ON_EVENT(CfrmMainView, IDS_COMMAND2, DISPID_DBLCLICK, CfrmMainView::DblClickCommand2, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_VACUUM_ON4, DISPID_CLICK, CfrmMainView::ClickVacuumOn4, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_VACUUM_ON2, DISPID_CLICK, CfrmMainView::ClickVacuumOn2, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_VAC_ONOFF_1, DISPID_CLICK, CfrmMainView::ClickVacOnoff1, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_VAC_ONOFF_2, DISPID_CLICK, CfrmMainView::ClickVacOnoff2, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_VAC_ONOFF_4, DISPID_CLICK, CfrmMainView::ClickVacOnoff4, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_VAC_ONOFF_5, DISPID_CLICK, CfrmMainView::ClickVacOnoff5, VTS_NONE)
	ON_EVENT(CfrmMainView, IDC_CURPOS_34, DISPID_DBLCLICK, CfrmMainView::DblClickCurpos34, VTS_NONE)
	ON_EVENT(CfrmMainView, IDC_COMIGRAPHCTRL1, DISPID_DBLCLICK, CfrmMainView::DblClickComigraphctrl1, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_CM_MANUAL_SPEED, DISPID_CLICK, CfrmMainView::ClickCmManualSpeed, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_CM_MOVE11, DISPID_CLICK, CfrmMainView::ClickCmMove11, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_CM_MOVE12, DISPID_CLICK, CfrmMainView::ClickCmMove12, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_CM_MOVE13, DISPID_CLICK, CfrmMainView::ClickCmMove13, VTS_NONE)
	ON_EVENT(CfrmMainView, IDB_DATA_SAVE9, DISPID_CLICK, CfrmMainView::ClickDataSave9, VTS_NONE)
END_EVENTSINK_MAP()


//===================================================================================
void CfrmMainView::ClickOriginReturn3()
{
}

//===================================================================================
void CfrmMainView::ClickOriginReturn()
{
	CdlgOrigin dlg;
	dlg.DoModal();
}

//===================================================================================
void CfrmMainView::ClickOriginReturn2()
{
}

//===================================================================================
void CfrmMainView::ClickReady()
{
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	UpdateData(TRUE);

	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN) 
	{
		CdlgStopModeSel	dlg;
		dlg.DoModal();

		if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_PAUSE)
			PauseProcess();
		else if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_STOP)
		{
			StopProcess();
		}
		if(ParamFile()->System_data.bEND_STOP_FLAG== _CURRENT_ENDSTOP_ENA) 
		{
			strText.Format(_T("  [OnClickReady] => bEND_STOP_FLAG : END-STOP-START"));		Log()->SaveLogFile(_SYSTEM_LOG, strText);
			m_btnReady.SetCaption(_T("종료 대기"));
			pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [OnClickReady] => 종료 대기"));
			CString strTemp;
			strTemp.Format(_T("%s\\Bin\\Icon\\Gloss PNGSyncronize-64.ico"), _PROGRAM_ROOT_DIR);
			m_btnReady.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);
		}
	}
	else {
		if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_PAUSE) {
			CdlgStopModeSel	dlg;
			dlg.DoModal();

			long lOutPulse= 0;
			if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN) 
			{
				m_btnReady.SetCaption(_T("운전 정지"));
				Log()->SaveLogFile(_SYSTEM_LOG,_T("  [OnClickReady] => ================================================"));
				Log()->SaveLogFile(_SYSTEM_LOG, _T("  [OnClickReady] => 자동 동작 재시작"));
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [OnClickReady] => 자동 동작 재시작"));
				ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_RUN;
				CString strTemp;
				strTemp.Format(_T("%s\\Bin\\Icon\\Gloss PNGRestart-64.ico"), _PROGRAM_ROOT_DIR);
				m_btnReady.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);

			}
			else if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_STOP) 
			{
				StopProcess();
			}
			if(ParamFile()->System_data.bEND_STOP_FLAG== _CURRENT_ENDSTOP_ENA) 
			{
				strText.Format(_T("  [OnClickReady] => bEND_STOP_FLAG : END-STOP-START"));		Log()->SaveLogFile(_SYSTEM_LOG, strText);
				m_btnReady.SetCaption(_T("종료 대기"));
				pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [OnClickReady] => 종료 대기"));
				CString strTemp;
				strTemp.Format(_T("%s\\Bin\\Icon\\Gloss PNGSyncronize-64.ico"), _PROGRAM_ROOT_DIR);
				m_btnReady.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);
			}
		}
		else
		{
			if(!AutoRunStart())
			{
				StopProcess();
				MyMessageBox(_T("자동 운전이 취소되었습니다."), _T("Auto-Run Cancel"), M_ICONERROR);
			}
		}
	}
}
//===================================================================================
bool CfrmMainView::AutoRunStart()
{
	CString strText;
	CdlgAutoRunInfo dlg;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("모터 상태를 확인 하십시요."), _T("AutoRunStart"), M_ICONERROR); 
		Log()->SaveLogFile(_SYSTEM_LOG, _T("  [AutoRunStart] => 자동 동작 취소"));
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [AutoRunStart] => 자동 동작 취소"));
		return false;
	}
	if(dlg.DoModal()== IDOK) 
	{
//		DWORD sss;

		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
		pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_2X);		pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_20X);

		Log()->SaveLogFile(_SYSTEM_LOG, _T("  [AutoRunStart] => ================================================"));
		switch(ParamFile()->System_data.iWORK_PROCESS_MODE)
		{
		case _PROCESS_AUTO_RUN:		strText.Format(_T("  [AutoRunStart] => iWORK_PROCESS_MODE : AUTO_RUN"));		break;
		case _PROCESS_CYCLE_RUN:	strText.Format(_T("  [AutoRunStart] => iWORK_PROCESS_MODE : CYCLE_RUN"));		break;
		default:					strText.Format(_T("  [AutoRunStart] => iWORK_PROCESS_MODE : _RUN_MODE ?"));	break;
		}
		Log()->SaveLogFile(_SYSTEM_LOG, strText);
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText);

		if(ParamFile()->System_data.bEND_STOP_FLAG== _CURRENT_ENDSTOP_DIS)
		{								strText.Format(_T("  [AutoRunStart] => bEND_STOP_FLAG : NONE"));			}
		else if(ParamFile()->System_data.bEND_STOP_FLAG== _CURRENT_ENDSTOP_ENA)
		{								strText.Format(_T("  [AutoRunStart] => bEND_STOP_FLAG : END-STOP"));		}
		else {							strText.Format(_T("  [AutoRunStart] => bEND_STOP_FLAG : ???"));			}
		Log()->SaveLogFile(_SYSTEM_LOG, strText);
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText);

		strText.Format(_T("  [AutoRunStart] => iPROCESS_CYCLE_COUNT : %d"), ParamFile()->System_data.iPROCESS_CYCLE_COUNT);
		Log()->SaveLogFile(_SYSTEM_LOG, strText);
		pFrm->GetWindowHandler_DebugView()->ListPrint(strText);

		Log()->SaveLogFile(_SYSTEM_LOG, _T("  [AutoRunStart] => ================================================"));
		Log()->SaveLogFile(_SYSTEM_LOG, _T("  [AutoRunStart] => 자동 동작 시작"));
		pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [AutoRunStart] => 자동 동작 시작"));

		CString strTemp;
		strTemp.Format(_T("%s\\Bin\\Icon\\Gloss PNGRestart-64.ico"), _PROGRAM_ROOT_DIR);
		m_btnReady.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);

		ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_RUN;
		ParamFile()->System_data.iPROCESS_RUNNING_COUNT= 0;

		pSystem.GetProcessThread_MainComponent()->StepProcessStart();

		m_btnReady.SetCaption(_T("운전 정지"));

		return true;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("  [AutoRunStart] => 자동 동작 취소"));
	pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [AutoRunStart] => 자동 동작 취소"));
	return false;

}
//===================================================================================
void CfrmMainView::StopProcess()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_btnReady.SetCaption(_T("운전 시작"));

	ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_STOP;
	ParamFile()->System_data.bEND_STOP_FLAG= _CURRENT_ENDSTOP_DIS;
	Log()->SaveLogFile(_SYSTEM_LOG, _T("  [StopProcess] => ================================================"));
	Log()->SaveLogFile(_SYSTEM_LOG, _T("  [StopProcess] => 자동 동작 정지"));

	pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [StopProcess] => 자동 동작 정지"));

	CString strTemp;
	strTemp.Format(_T("%s\\Bin\\Icon\\Gloss PNGShutdown_Quit_Dock-64.ico"), _PROGRAM_ROOT_DIR);
	m_btnReady.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);

	pSystem.GetProcessThread_MainComponent()->StepProcessStop();
}
//===================================================================================
void CfrmMainView::PauseProcess()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_btnReady.SetCaption(_T("재 시작"));
	//	m_btnReady.SetBackColorInterior(RGB(255,255,128));
	Log()->SaveLogFile(_SYSTEM_LOG, _T("  [PauseProcess] => ================================================"));
	Log()->SaveLogFile(_SYSTEM_LOG, _T("  [PauseProcess] => 자동 동작 일시 정지"));
	pFrm->GetWindowHandler_DebugView()->ListPrint(_T("  [PauseProcess] => 자동 동작 일시 정지"));

	CString strTemp;
	strTemp.Format(_T("%s\\Bin\\Icon\\Gloss PNGStandby-64.ico"), _PROGRAM_ROOT_DIR);
	m_btnReady.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);
}

//===================================================================================
void CfrmMainView::DblClickCommand()
{
	pSystem.GetIO()->LoadCell_Reset();
}

//===================================================================================
void CfrmMainView::DblClickCommand2()
{
	pSystem.GetIO()->LoadCell_Reset();
}

//===================================================================================
void CfrmMainView::ClickVacuumOn4()
{//Foot 정
	g_FootSW_Dir= _DIR_CW;
}

//===================================================================================
void CfrmMainView::ClickVacuumOn2()
{//foot 역
	g_FootSW_Dir= _DIR_CCW;
}
//===================================================================================

void CfrmMainView::OnBnClickedButton1()
{
	MyMessageBox(_T("Main Air를 확인 하십시요."), _T("STOP"), M_ICONERROR);
}
//===================================================================================

void CfrmMainView::GraphDataInital() 
{
	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);
	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);
	m_cnGraph1.DeletePlot(4);
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
//===================================================================================
LRESULT CfrmMainView::LoadCell_Get_Data1(WPARAM wParam,LPARAM lParam) //Left-Roll
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

		AD_BufferRecCount1= pSystem.GetMotionComponent()->GetGetherData(PlotData_XP_1, PlotData_XS_1, PlotData_L1_1, PlotData_L2_1, PlotData_Enc_1, PlotData_Time_1, PlotData_ZP_1, PlotData_Z2P_1);


		if(AD_BufferRecCount1<= 0)
		{
			m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
			m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
			return 0;
		}

		Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
		for(i= 0, Max_X1_Cnt= 0; i< AD_BufferRecCount1; i++, Max_X1_Cnt++)
		{
			if(i> _MAX_LOADCELL_NO) break;

			if(fabs(PlotData_XP_1[i]) > Graph_Y1_Max[0])	Graph_Y1_Max[0]= fabs(PlotData_XP_1[i]);
			if(fabs(PlotData_XS_1[i]) > Graph_Y1_Max[1])	Graph_Y1_Max[1]= fabs(PlotData_XS_1[i]);
			if(fabs(PlotData_L1_1[i]) > Graph_Y1_Max[2])	Graph_Y1_Max[2]= fabs(PlotData_L1_1[i]);
			if(fabs(PlotData_Enc_1[i]) > Graph_Y1_Max[3])	Graph_Y1_Max[3]= fabs(PlotData_Enc_1[i]);
			if(fabs(PlotData_ZP_1[i]) > Graph_Y1_Max[4])	Graph_Y1_Max[4]= fabs(PlotData_ZP_1[i]);
			PlotData_Time_1[i]= i * time_fact;
		}
	}
	Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
	Graph_Y1_Max[0]= Graph_Y1_Max[1]=Graph_Y1_Max[2]=Graph_Y1_Max[3]=Graph_Y1_Max[4]= 0.0;
	for(i= 0, Max_X1_Cnt= 0; i< AD_BufferRecCount1; i++, Max_X1_Cnt++)
	{
		if(i> _MAX_LOADCELL_NO) break;

		if(fabs(PlotData_XP_1[i]) > Graph_Y1_Max[0])	Graph_Y1_Max[0]= fabs(PlotData_XP_1[i]);
		if(fabs(PlotData_XS_1[i]) > Graph_Y1_Max[1])	Graph_Y1_Max[1]= fabs(PlotData_XS_1[i]);
		if(fabs(PlotData_L1_1[i]) > Graph_Y1_Max[2])	Graph_Y1_Max[2]= fabs(PlotData_L1_1[i]);
		if(fabs(PlotData_Enc_1[i]) > Graph_Y1_Max[3])	Graph_Y1_Max[3]= fabs(PlotData_Enc_1[i]);
		if(fabs(PlotData_ZP_1[i]) > Graph_Y1_Max[4])	Graph_Y1_Max[4]= fabs(PlotData_ZP_1[i]);
		PlotData_Time_1[i]= i * time_fact;
	}
	int aaa= (int)(Graph_Y1_Max[2] / 10.0);

	Graph_Y1_Max[2]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y1_Max[0] / 10.0);	Graph_Y1_Max[0]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y1_Max[1] / 5.0);	Graph_Y1_Max[1]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y1_Max[4] / 5.0);	Graph_Y1_Max[4]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y1_Max[3] / 1.0);	Graph_Y1_Max[3]= (aaa+1) * 1.0;
	//--------------
	int x_max= (int)(Max_X1_Cnt * time_fact) + 1;

	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용

	if(AD_BufferRecCount1<= 0) return 0;

	m_cnGraph1.SetXAxisRange (0, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (1, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (2, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (3, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (4, 0.0, (double)x_max, true);	

	double tY_Max1_0= 0.0;
	double tY_Max1_1= 0.0;

	m_cnGraph1.SetYAxisRange (0, -Graph_Y1_Max[2], Graph_Y1_Max[2], true);	
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.SetYAxisRange (1, -Graph_Y1_Max[0], Graph_Y1_Max[0], true);	
	else
		m_cnGraph1.SetYAxisRange (1, -Graph_Y1_Max[4], Graph_Y1_Max[4], true);	
	m_cnGraph1.SetYAxisRange (2, -Graph_Y1_Max[1], Graph_Y1_Max[1], true);	
	m_cnGraph1.SetYAxisRange (3, -Graph_Y1_Max[3], Graph_Y1_Max[3], true);	
//	m_cnGraph1.SetYAxisRange (4, -Graph_Y1_Max[3], Graph_Y1_Max[3], true);	

	m_cnGraph1.PlotXY_VT(0, (VARIANT *)PlotData_Time_1, cmVT_DOUBLE, (VARIANT *)PlotData_L1_1, cmVT_DOUBLE, (long)(Max_X1_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(1, (VARIANT *)PlotData_Time_1, cmVT_DOUBLE, (VARIANT *)PlotData_L2_1, cmVT_DOUBLE, (long)(Max_X1_Cnt));//AD_BufferRecCount1+10);
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_1, cmVT_DOUBLE, (VARIANT *)PlotData_XP_1, cmVT_DOUBLE, (long)(Max_X1_Cnt));//AD_BufferRecCount1+10);
	else
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_1, cmVT_DOUBLE, (VARIANT *)PlotData_ZP_1, cmVT_DOUBLE, (long)(Max_X1_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(3, (VARIANT *)PlotData_Time_1, cmVT_DOUBLE, (VARIANT *)PlotData_XS_1, cmVT_DOUBLE, (long)(Max_X1_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(4, (VARIANT *)PlotData_Time_1, cmVT_DOUBLE, (VARIANT *)PlotData_Enc_1, cmVT_DOUBLE, (long)(Max_X1_Cnt));//AD_BufferRecCount1+10);

	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	Invalidate();

	return 0;
}

LRESULT CfrmMainView::LoadCell_Get_Data2(WPARAM wParam,LPARAM lParam) //Left-Roll
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

		AD_BufferRecCount2= pSystem.GetMotionComponent()->GetGetherData(PlotData_XP_2, PlotData_XS_2, PlotData_L1_2, PlotData_L2_2, PlotData_Enc_2, PlotData_Time_2, PlotData_ZP_2, PlotData_Z2P_2);


		if(AD_BufferRecCount2<= 0)
		{
			m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
			m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
			return 0;
		}

		Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
		for(i= 0, Max_X2_Cnt= 0; i< AD_BufferRecCount2; i++, Max_X2_Cnt++)
		{
			if(i> _MAX_LOADCELL_NO) break;

			if(fabs(PlotData_XP_2[i]) > Graph_Y2_Max[0])	Graph_Y2_Max[0]= fabs(PlotData_XP_2[i]);
			if(fabs(PlotData_XS_2[i]) > Graph_Y2_Max[1])	Graph_Y2_Max[1]= fabs(PlotData_XS_2[i]);
			if(fabs(PlotData_L1_2[i]) > Graph_Y2_Max[2])	Graph_Y2_Max[2]= fabs(PlotData_L1_2[i]);
			if(fabs(PlotData_Enc_2[i]) > Graph_Y2_Max[3])	Graph_Y2_Max[3]= fabs(PlotData_Enc_2[i]);
			if(fabs(PlotData_ZP_2[i]) > Graph_Y2_Max[4])	Graph_Y2_Max[4]= fabs(PlotData_ZP_2[i]);
			PlotData_Time_2[i]= i * time_fact;
		}
	}
	Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
	Graph_Y2_Max[0]= Graph_Y2_Max[1]=Graph_Y2_Max[2]=Graph_Y2_Max[3]=Graph_Y2_Max[4]= 0.0;
	for(i= 0, Max_X2_Cnt= 0; i< AD_BufferRecCount2; i++, Max_X2_Cnt++)
	{
		if(i> _MAX_LOADCELL_NO) break;

		if(fabs(PlotData_XP_2[i]) > Graph_Y2_Max[0])	Graph_Y2_Max[0]= fabs(PlotData_XP_2[i]);
		if(fabs(PlotData_XS_2[i]) > Graph_Y2_Max[1])	Graph_Y2_Max[1]= fabs(PlotData_XS_2[i]);
		if(fabs(PlotData_L1_2[i]) > Graph_Y2_Max[2])	Graph_Y2_Max[2]= fabs(PlotData_L1_2[i]);
		if(fabs(PlotData_Enc_2[i]) > Graph_Y2_Max[3])	Graph_Y2_Max[3]= fabs(PlotData_Enc_2[i]);
		if(fabs(PlotData_ZP_2[i]) > Graph_Y2_Max[4])	Graph_Y2_Max[4]= fabs(PlotData_ZP_2[i]);
		PlotData_Time_2[i]= i * time_fact;
	}
	int aaa= (int)(Graph_Y2_Max[2] / 10.0);

	Graph_Y2_Max[2]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y2_Max[0] / 10.0);	Graph_Y2_Max[0]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y2_Max[1] / 5.0);	Graph_Y2_Max[1]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y2_Max[4] / 5.0);	Graph_Y2_Max[4]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y2_Max[3] / 1.0);	Graph_Y2_Max[3]= (aaa+1) * 1.0;
	//--------------
	int x_max= (int)(Max_X2_Cnt * time_fact) + 1;

	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용

	if(AD_BufferRecCount2<= 0) return 0;

	m_cnGraph1.SetXAxisRange (0, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (1, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (2, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (3, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (4, 0.0, (double)x_max, true);	

	double tY_Max1_0= 0.0;
	double tY_Max1_1= 0.0;

	m_cnGraph1.SetYAxisRange (0, -Graph_Y2_Max[2], Graph_Y2_Max[2], true);	
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.SetYAxisRange (1, -Graph_Y2_Max[0], Graph_Y2_Max[0], true);	
	else
		m_cnGraph1.SetYAxisRange (1, -Graph_Y2_Max[4], Graph_Y2_Max[4], true);	
	m_cnGraph1.SetYAxisRange (2, -Graph_Y2_Max[1], Graph_Y2_Max[1], true);	
	m_cnGraph1.SetYAxisRange (3, -Graph_Y2_Max[3], Graph_Y2_Max[3], true);	
	//	m_cnGraph1.SetYAxisRange (4, -Graph_Y2_Max[3], Graph_Y2_Max[3], true);	

	m_cnGraph1.PlotXY_VT(0, (VARIANT *)PlotData_Time_2, cmVT_DOUBLE, (VARIANT *)PlotData_L1_2, cmVT_DOUBLE, (long)(Max_X2_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(1, (VARIANT *)PlotData_Time_2, cmVT_DOUBLE, (VARIANT *)PlotData_L2_2, cmVT_DOUBLE, (long)(Max_X2_Cnt));//AD_BufferRecCount1+10);
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_2, cmVT_DOUBLE, (VARIANT *)PlotData_XP_2, cmVT_DOUBLE, (long)(Max_X2_Cnt));//AD_BufferRecCount1+10);
	else
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_2, cmVT_DOUBLE, (VARIANT *)PlotData_ZP_2, cmVT_DOUBLE, (long)(Max_X2_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(3, (VARIANT *)PlotData_Time_2, cmVT_DOUBLE, (VARIANT *)PlotData_XS_2, cmVT_DOUBLE, (long)(Max_X2_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(4, (VARIANT *)PlotData_Time_2, cmVT_DOUBLE, (VARIANT *)PlotData_Enc_2, cmVT_DOUBLE, (long)(Max_X2_Cnt));//AD_BufferRecCount1+10);

	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	Invalidate();

	return 0;
}
LRESULT CfrmMainView::LoadCell_Get_Data3(WPARAM wParam,LPARAM lParam) //Left-Roll
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

	m_cnGraph1.SetYAxisRange (0, -Graph_Y3_Max[2], Graph_Y3_Max[2], true);	
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.SetYAxisRange (1, -Graph_Y3_Max[0], Graph_Y3_Max[0], true);	
	else
		m_cnGraph1.SetYAxisRange (1, -Graph_Y3_Max[4], Graph_Y3_Max[4], true);	
	m_cnGraph1.SetYAxisRange (2, -Graph_Y3_Max[1], Graph_Y3_Max[1], true);	
	m_cnGraph1.SetYAxisRange (3, -Graph_Y3_Max[3], Graph_Y3_Max[3], true);	
	//	m_cnGraph1.SetYAxisRange (4, -Graph_Y3_Max[3], Graph_Y3_Max[3], true);	

	m_cnGraph1.PlotXY_VT(0, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_L1_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(1, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_L2_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_XP_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	else
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_ZP_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(3, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_XS_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(4, (VARIANT *)PlotData_Time_3, cmVT_DOUBLE, (VARIANT *)PlotData_Enc_3, cmVT_DOUBLE, (long)(Max_X3_Cnt));//AD_BufferRecCount1+10);

	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	Invalidate();

	return 0;
}
LRESULT CfrmMainView::LoadCell_Get_Data4(WPARAM wParam,LPARAM lParam) //Left-Roll
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

		AD_BufferRecCount4= pSystem.GetMotionComponent()->GetGetherData(PlotData_XP_S, PlotData_XS_S, PlotData_L1_S, PlotData_L2_S, PlotData_Enc_S, PlotData_Time_4, PlotData_ZP_S, PlotData_Z2P_S);


		if(AD_BufferRecCount4<= 0)
		{
			m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
			m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
			return 0;
		}

		Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
		for(i= 0, Max_X4_Cnt= 0; i< AD_BufferRecCount4; i++, Max_X4_Cnt++)
		{
			if(i> _MAX_LOADCELL_NO) break;

			if(fabs(PlotData_XP_S[i]) > Graph_Y4_Max[0])	Graph_Y4_Max[0]= fabs(PlotData_XP_S[i]);
			if(fabs(PlotData_XS_S[i]) > Graph_Y4_Max[1])	Graph_Y4_Max[1]= fabs(PlotData_XS_S[i]);
			if(fabs(PlotData_L1_S[i]) > Graph_Y4_Max[2])	Graph_Y4_Max[2]= fabs(PlotData_L1_S[i]);
			if(fabs(PlotData_Enc_S[i]) > Graph_Y4_Max[3])	Graph_Y4_Max[3]= fabs(PlotData_Enc_S[i]);
			if(fabs(PlotData_ZP_S[i]) > Graph_Y4_Max[4])	Graph_Y4_Max[4]= fabs(PlotData_ZP_S[i]);
			PlotData_Time_4[i]= i * time_fact;
		}
	}
	Y_Max1_0= 0.0;	Y_Max1_1= 0.0;
	Graph_Y4_Max[0]= Graph_Y4_Max[1]=Graph_Y4_Max[2]=Graph_Y4_Max[3]=Graph_Y4_Max[4]= 0.0;
	for(i= 0, Max_X4_Cnt= 0; i< AD_BufferRecCount4; i++, Max_X4_Cnt++)
	{
		if(i> _MAX_LOADCELL_NO) break;

		if(fabs(PlotData_XP_S[i]) > Graph_Y4_Max[0])	Graph_Y4_Max[0]= fabs(PlotData_XP_S[i]);
		if(fabs(PlotData_XS_S[i]) > Graph_Y4_Max[1])	Graph_Y4_Max[1]= fabs(PlotData_XS_S[i]);
		if(fabs(PlotData_L1_S[i]) > Graph_Y4_Max[2])	Graph_Y4_Max[2]= fabs(PlotData_L1_S[i]);
		if(fabs(PlotData_Enc_S[i]) > Graph_Y4_Max[3])	Graph_Y4_Max[3]= fabs(PlotData_Enc_S[i]);
		if(fabs(PlotData_ZP_S[i]) > Graph_Y4_Max[4])	Graph_Y4_Max[4]= fabs(PlotData_ZP_S[i]);
		PlotData_Time_4[i]= i * time_fact;
	}
	int aaa= (int)(Graph_Y4_Max[2] / 10.0);

	Graph_Y4_Max[2]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y4_Max[0] / 10.0);	Graph_Y4_Max[0]= (aaa+1) * 10.0;
	aaa= (int)(Graph_Y4_Max[1] / 5.0);	Graph_Y4_Max[1]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y4_Max[4] / 5.0);	Graph_Y4_Max[4]= (aaa+1) * 5.0;
	aaa= (int)(Graph_Y4_Max[3] / 1.0);	Graph_Y4_Max[3]= (aaa+1) * 1.0;
	//--------------
	int x_max= (int)(Max_X4_Cnt * time_fact) + 1;

	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용

	if(AD_BufferRecCount4<= 0) return 0;

	m_cnGraph1.SetXAxisRange (0, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (1, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (2, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (3, 0.0, (double)x_max, true);	
	m_cnGraph1.SetXAxisRange (4, 0.0, (double)x_max, true);	

	double tY_Max1_0= 0.0;
	double tY_Max1_1= 0.0;

	m_cnGraph1.SetYAxisRange (0, -Graph_Y4_Max[2], Graph_Y4_Max[2], true);	
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.SetYAxisRange (1, -Graph_Y4_Max[0], Graph_Y4_Max[0], true);	
	else
		m_cnGraph1.SetYAxisRange (1, -Graph_Y4_Max[4], Graph_Y4_Max[4], true);	
	m_cnGraph1.SetYAxisRange (2, -Graph_Y4_Max[1], Graph_Y4_Max[1], true);	
	m_cnGraph1.SetYAxisRange (3, -Graph_Y4_Max[3], Graph_Y4_Max[3], true);	
	//	m_cnGraph1.SetYAxisRange (4, -Graph_Y4_Max[3], Graph_Y4_Max[3], true);	

	m_cnGraph1.PlotXY_VT(0, (VARIANT *)PlotData_Time_4, cmVT_DOUBLE, (VARIANT *)PlotData_L1_S, cmVT_DOUBLE, (long)(Max_X4_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(1, (VARIANT *)PlotData_Time_4, cmVT_DOUBLE, (VARIANT *)PlotData_L2_S, cmVT_DOUBLE, (long)(Max_X4_Cnt));//AD_BufferRecCount1+10);
	if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_4, cmVT_DOUBLE, (VARIANT *)PlotData_XP_S, cmVT_DOUBLE, (long)(Max_X4_Cnt));//AD_BufferRecCount1+10);
	else
		m_cnGraph1.PlotXY_VT(2, (VARIANT *)PlotData_Time_4, cmVT_DOUBLE, (VARIANT *)PlotData_ZP_S, cmVT_DOUBLE, (long)(Max_X4_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(3, (VARIANT *)PlotData_Time_4, cmVT_DOUBLE, (VARIANT *)PlotData_XS_S, cmVT_DOUBLE, (long)(Max_X4_Cnt));//AD_BufferRecCount1+10);
	m_cnGraph1.PlotXY_VT(4, (VARIANT *)PlotData_Time_4, cmVT_DOUBLE, (VARIANT *)PlotData_Enc_S, cmVT_DOUBLE, (long)(Max_X4_Cnt));//AD_BufferRecCount1+10);

	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	Invalidate();

	return 0;
}
LRESULT CfrmMainView::LoadCell_Get_Data5(WPARAM wParam,LPARAM lParam) //Left-Roll
{
	return 0;
}
void CfrmMainView::EndGraph(int ch)
{
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	switch(ch)
	{
	case STAGE_1_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE1, 0, 0);	 break;
	case STAGE_2_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE2, 0, 0);	 break;
	case STAGE_3_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE3, 0, 0);	 break;
	case SYNC_GRAPH:	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE4, 0, 0);	 break;
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
	/////////////////////////////////////////////////////////////////////////////////
//	m_cnGraph1.SaveToClipboard ();
	/////////////////////////////////////////////////////////////////////////////////
}


void CfrmMainView::ClickVacOnoff1()
{
	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);
	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);
	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE1, 0, 1);
}


void CfrmMainView::ClickVacOnoff2()
{
	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);
	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);
	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE2, 0, 1);
}


void CfrmMainView::ClickVacOnoff4()
{
	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);
	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);
	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE3, 0, 0);
}


void CfrmMainView::ClickVacOnoff5()
{
	m_cnGraph1.DeletePlot(0);	m_cnGraph1.DeletePlot(1);
	m_cnGraph1.DeletePlot(2);	m_cnGraph1.DeletePlot(3);
	m_cnGraph1.DeletePlot(4);
	m_cnGraph1.UpdatePlot(); // 중요: Plotting관련 함수의 작업 내용을 화면에 적용
	SendMessage(WM_MSG_OFF_GRAPH_DATA_UPDATE4, 0, 1);
}


void CfrmMainView::DblClickCurpos34()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CfrmMainView::DblClickComigraphctrl1()
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


void CfrmMainView::ClickCmManualSpeed()
{
	CdlgModelLoad dlg;
	if(dlg.DoModal()== IDOK) {
		CString strText;
		strText.Format(_T("%s"), ParamFile()->System_data.strCurrentModelName);
		SetDlgItemText(IDC_MODEL_NAME, strText.GetBuffer(0));

		strText.Format(_T("- 모델 변경 : %s"), ParamFile()->System_data.strCurrentModelName);
		Log()->SaveLogFile(_SYSTEM_LOG, _T("================================================"));
		Log()->SaveLogFile(_SYSTEM_LOG, strText);
		Log()->SaveLogFile(_SYSTEM_LOG, _T("================================================"));

		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
		pFrm->Data_Reload();
	}
}


void CfrmMainView::ClickCmMove11()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE1_ALIGN, FALSE);
}


void CfrmMainView::ClickCmMove12()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE2_ALIGN, FALSE);
}


void CfrmMainView::ClickCmMove13()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE3_ALIGN, FALSE);
}


void CfrmMainView::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	if(m_Wait_Flag) g_Wait_Flag= true;
	else			g_Wait_Flag= false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CfrmMainView::ClickDataSave9()
{
	CdlgRecipeSet dlg;
	dlg.DoModal();
	ListUpdate();
}
//===================================================================================
void CfrmMainView::ListUpdate()
{
	CString str; 
	CString strOP1= _T(""); 
	CString strOP2= _T(""); 
	CString strOP3= _T("");  
	CString strOP4= _T(""); 
	CString strOP5= _T(""); 
	CString strcharOP= _T(""); 
	int cnt= 0;

	m_ListCtrl.DeleteAllItems ();
	m_ListCtrl.SetBkColor(RGB(255,235,235));
	m_ListCtrl.SetTextBkColor(RGB(255,230,230));
	// 리스트 컨트롤의 행 선택,Flat,라인.. 스타일
	m_ListCtrl.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	for(cnt= 0; cnt< _MAX_RECIPE; cnt++)
	{
		if(ParamFile()->Recipe_data[cnt].iCommand> 0)
		{
			switch(ParamFile()->Recipe_data[cnt].iCommand)
			{
			case _OP_NONE:				
				//	_tcscpy(pBuf, "%s\\Bin\\Icon", _PROGRAM_ROOT_DIR);
				str.Format(_T("Not-Define"));				
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//0
			case _OP_MOVE_WAIT_ALL:		
				str.Format(_T("대기 위치로 전체 이동"));		
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//1
			case _OP_MOVE_WAIT_X:		
				str.Format(_T("대기 위치로 Stage-X 이동"));	
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//2
			case _OP_MOVE_WAIT_Y:		
				str.Format(_T("대기 위치로 Stage-Y 이동"));
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//3
			case _OP_MOVE_WAIT_RZ:		
				str.Format(_T("대기 위치로 Roll-Z 이동"));
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//4
			case _OP_PROC_STAGE1:	//m_dOP_Val1= m_dCoatingOffsetmm	
				//m_dOP_Val1= m_dStage1_Len;		m_dOP_Val2= m_dStage1_Speed;		m_dOP_Val3= m_dStage1_Y_Offset;
				str.Format(_T("Stage #1 동작"));					
				ParamFile()->Recipe_data[cnt].dOP_Val1= ParamFile()->Model_data.dRun_Length[STAGE_1_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val2= ParamFile()->Model_data.dRun_Speed[STAGE_1_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val3= ParamFile()->Model_data.dRun_Offset_X[STAGE_1_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val4= ParamFile()->Model_data.dRun_Offset_Y[STAGE_1_ALIGN_PROC];
				strOP1.Format(_T("L:%.1fmm"), (ParamFile()->Recipe_data[cnt].dOP_Val1));	
				strOP2.Format(_T("S:%.1fmm/s"), (ParamFile()->Recipe_data[cnt].dOP_Val2));	
				strOP3.Format(_T("X:%.3f"), (ParamFile()->Recipe_data[cnt].dOP_Val3));	
				strOP4.Format(_T("Y:%.3f"), (ParamFile()->Recipe_data[cnt].dOP_Val4));	
				strOP5.Format(_T(" "));	
				strcharOP.Format(_T(" "));
				break;//5
			case _OP_PROC_STAGE2:	//m_dOP_Val1= m_dCoatingOffsetmm	
				str.Format(_T("Stage #2 동작"));					
				ParamFile()->Recipe_data[cnt].dOP_Val1= ParamFile()->Model_data.dRun_Length[STAGE_2_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val2= ParamFile()->Model_data.dRun_Speed[STAGE_2_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val3= ParamFile()->Model_data.dRun_Offset_X[STAGE_2_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val4= ParamFile()->Model_data.dRun_Offset_Y[STAGE_2_ALIGN_PROC];
				strOP1.Format(_T("L:%.1fmm"), (ParamFile()->Recipe_data[cnt].dOP_Val1));	
				strOP2.Format(_T("S:%.1fmm/s"), (ParamFile()->Recipe_data[cnt].dOP_Val2));	
				strOP3.Format(_T("X:%.3f"), (ParamFile()->Recipe_data[cnt].dOP_Val3));	
				strOP4.Format(_T("Y:%.3f"), (ParamFile()->Recipe_data[cnt].dOP_Val4));	
				strOP5.Format(_T(" "));	
				strcharOP.Format(_T(" "));
				break;//5
			case _OP_PROC_STAGE3:	//m_dOP_Val1= m_dCoatingOffsetmm	
				str.Format(_T("Stage #3 동작"));					
				ParamFile()->Recipe_data[cnt].dOP_Val1= ParamFile()->Model_data.dRun_Length[STAGE_3_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val2= ParamFile()->Model_data.dRun_Speed[STAGE_3_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val3= ParamFile()->Model_data.dRun_Offset_X[STAGE_3_ALIGN_PROC];
				ParamFile()->Recipe_data[cnt].dOP_Val4= ParamFile()->Model_data.dRun_Offset_Y[STAGE_3_ALIGN_PROC];
				strOP1.Format(_T("L:%.1fmm"), (ParamFile()->Recipe_data[cnt].dOP_Val1));	
				strOP2.Format(_T("S:%.1fmm/s"), (ParamFile()->Recipe_data[cnt].dOP_Val2));	
				strOP3.Format(_T("X:%.3f"), (ParamFile()->Recipe_data[cnt].dOP_Val3));	
				strOP4.Format(_T("Y:%.3f"), (ParamFile()->Recipe_data[cnt].dOP_Val4));	
				strOP5.Format(_T(" "));	
				strcharOP.Format(_T(" "));
				break;//5
			case _OP_PROC_PLATE_ALIGN:		
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				str.Format(_T("Align 동작"));			
				if	   (ParamFile()->Recipe_data[cnt].dOP_Val1== 0)		strOP1.Format(_T("Roll-Align"));
				else if(ParamFile()->Recipe_data[cnt].dOP_Val1== 1)		strOP1.Format(_T("Stage Align"));
				else if(ParamFile()->Recipe_data[cnt].dOP_Val1== 2)		strOP1.Format(_T("Stage #2-Align"));
				else if(ParamFile()->Recipe_data[cnt].dOP_Val1== 3)		strOP1.Format(_T("Stage #3-Align"));
				else	{												strOP1.Format(_T("Roll-Align")); ParamFile()->Recipe_data[cnt].dOP_Val1= 0; }
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//9
			case _OP_PROC_VACUUM:		
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				str.Format(_T("진공 On/Off 동작"));			
				if(ParamFile()->Recipe_data[cnt].dOP_Val1== 1)		strOP1.Format(_T("진공-ON"));
				else												strOP1.Format(_T("진공-OFF"));
				if(ParamFile()->Recipe_data[cnt].dOP_Val2== 1)		strOP2.Format(_T("진공-ON"));
				else												strOP2.Format(_T("진공-OFF"));
				if(ParamFile()->Recipe_data[cnt].dOP_Val3== 1)		strOP3.Format(_T("진공-ON"));
				else												strOP3.Format(_T("진공-OFF"));
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//9
			case _OP_PROC_DELAY:		
				str.Format(_T("Delay 동작"));				
				strOP1.Format(_T("%d msec"), (int)(ParamFile()->Recipe_data[cnt].dOP_Val1));	
				strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//10
			case _OP_PROC_MESSAGE:		
				str.Format(_T("키 입력 대기"));					
				strOP1.Format(_T("%s"), ParamFile()->Recipe_data[cnt].chrOP_Val);	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	strOP4.Format(_T(" "));				
				strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//11
			case _OP_PROC_END:			
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//12
			default:					
				str.Format(_T("Define-Error"));			
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;
			}

			m_ListCtrl.InsertItem(cnt, str); 
			m_ListCtrl.SetItemText(cnt, 1, strOP1); 
			m_ListCtrl.SetItemText(cnt, 2, strOP2); 
			m_ListCtrl.SetItemText(cnt, 3, strOP3); 
			m_ListCtrl.SetItemText(cnt, 4, strOP4); 
			m_ListCtrl.SetItemText(cnt, 5, strOP5); 
			m_ListCtrl.SetItemText(cnt, 6, strcharOP); 
		}
		else
		{
			break;
		}
	}
	ParamFile()->SaveRecipeConfigFile(ParamFile()->System_data.strCurrentRecipeName);
}
