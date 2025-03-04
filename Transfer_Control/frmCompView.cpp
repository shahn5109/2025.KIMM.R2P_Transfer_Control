// frmCompView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmCompView.h"
#include "MainFrm.h"


// CfrmCompView
extern CSystemClass pSystem;
CParameterFile* CfrmCompView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmCompView::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmCompView, CFormView)

CfrmCompView::CfrmCompView()
	: CFormView(CfrmCompView::IDD)
	, m_dCompPitch(0)
	, m_iCompCountX(0)
	, m_iCompCountY(0)
	, m_dCompRectSize(0)
	, m_bTopCompEnable(FALSE)
{
	m_ROI_Mode= false;
	m_iMovePix_Sel= 0;

	mCompProcStart= FALSE;
	mCompProc_Step= 0;
	for(int i= 0 ; i< 3; i++)	{
		m_bExec_1[i] = FALSE; m_pThread_1[i] = NULL;	
	}
	proc_run_flag= false;
	bComp_End_Flag= FALSE;

	for(int i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		CalData[i]= 0.0;
		CompCal_data.Top_data_X[i].MovePos= 0.0;
		CompCal_data.Top_data_X[i].GetPos= 0.0;
		CompCal_data.Top_data_X[i].Cal_um= 0.0;
		CompCal_data.Btm_data_X[i].MovePos= 0.0;
		CompCal_data.Btm_data_X[i].GetPos= 0.0;
		CompCal_data.Btm_data_X[i].Cal_um= 0.0;
		CompCal_data.Top_data_Y[i].MovePos= 0.0;
		CompCal_data.Top_data_Y[i].GetPos= 0.0;
		CompCal_data.Top_data_Y[i].Cal_um= 0.0;
	}
	CompCal_data.X_Count= 0;
	CompCal_data.Y_Count= 0;
	bTopCompEnable= FALSE;
	bBtmCompEnable= FALSE;
	TopLoadOkFlag=  BtmLoadOkFlag= FALSE;


}

CfrmCompView::~CfrmCompView()
{
}

void CfrmCompView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_MARK_ROI_SEL, m_btnROI_Set);
	DDX_Control(pDX, IDC_STATIC_TOP, m_ModelEditDisplay_L);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_ctrlSpeed);
	DDX_Control(pDX, IDC_COMBO_ACCURACY, m_ctrlAccuracy);
	DDX_Text(pDX, IDC_EDIT_SCORE_CERTANTY, m_dScoreCertanty);
	DDX_Text(pDX, IDC_EDIT_SCORE_ACCEPTANCE, m_dScoreAcceptance);
	DDX_Text(pDX, IDC_EDIT_SMOOTHNESS2, m_TimeOut);
	DDX_Text(pDX, IDC_EDIT_OFFSET_X, m_dOffsetX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_Y, m_dOffsetY);
	DDX_Text(pDX, IDC_BRUSH_SIZE, m_nBrushSize_L);
	DDX_Control(pDX, IDC_BRUSH_SLIDER, m_ctrlBrushSlider_L);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlLedSlider);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS9, m_iLedcontrolVal);

	DDX_Radio(pDX, IDC_RADIO21, m_iMovePix_Sel);
	DDX_Control(pDX, IDB_JOG_SELECT_ALIGN, m_btnTopSel);
	DDX_Control(pDX, IDB_JOG_SELECT_MASK3, m_btnBtmSel);
	DDX_Text(pDX, IDC_EDIT_SMOOTHNESS3, m_dCompPitch);
	DDX_Text(pDX, IDC_EDIT_SMOOTHNESS4, m_iCompCountX);
	DDX_Text(pDX, IDC_EDIT_SMOOTHNESS5, m_iCompCountY);
	DDX_Text(pDX, IDC_EDIT_SMOOTHNESS6, m_dCompRectSize);

	DDX_Control(pDX, IDC_CUSTOM_GRID, m_ctrlGrid1);
	DDX_Check(pDX, IDC_CHECK1, m_bTopCompEnable);
	DDX_Check(pDX, IDC_CHECK2, m_bBtmCompEnable);

	DDX_Control(pDX, IDB_OPU_ROLL_Z30, m_lblRunDisable_X);
	DDX_Control(pDX, IDB_OPU_ROLL_Z32, m_lblRunEnable_X);
}

BEGIN_MESSAGE_MAP(CfrmCompView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmCompView::OnBnClickedButton2)
	ON_MESSAGE(WM_MSG_DATA_UPDATE, &CfrmCompView::DataReload)
	ON_MESSAGE(WM_MSG_PAT_ROI_UPDATE, &CfrmCompView::SetRoi_Confirm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CfrmCompView::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON_U, &CfrmCompView::OnBnClickedButtonU)
	ON_BN_CLICKED(IDC_BUTTON_D, &CfrmCompView::OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_L2, &CfrmCompView::OnBnClickedButtonL2)
	ON_BN_CLICKED(IDC_BUTTON_R2, &CfrmCompView::OnBnClickedButtonR2)
	ON_EN_CHANGE(IDC_EDIT_SMOOTHNESS6, &CfrmCompView::OnEnChangeEditSmoothness6)
	ON_BN_CLICKED(IDC_MASK_EDIT_APPLY3, &CfrmCompView::OnBnClickedMaskEditApply3)
	ON_BN_CLICKED(IDC_MASK_EDIT_APPLY4, &CfrmCompView::OnBnClickedMaskEditApply4)
	ON_BN_CLICKED(IDC_MASK_EDIT_APPLY5, &CfrmCompView::OnBnClickedMaskEditApply5)
	ON_BN_CLICKED(IDC_CHECK1, &CfrmCompView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CfrmCompView::OnBnClickedCheck2)
	ON_MESSAGE(WM_MSG_TOP_LOADIMAGE, &CfrmCompView::TopLoadImage)
	ON_MESSAGE(WM_MSG_BTM_LOADIMAGE, &CfrmCompView::BtmLoadImage)
	ON_MESSAGE(WM_MSG_GRID_COMPUPDATE, &CfrmCompView::CompDataUpdate)
END_MESSAGE_MAP()


// CfrmCompView 진단입니다.

#ifdef _DEBUG
void CfrmCompView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmCompView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmCompView 메시지 처리기입니다.
//===================================================================================
void CfrmCompView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_CompView(this);

	m_pVision= pSystem.GetVisionComponent();

	LensNo= e20X_LENS;
	CamNo= eTOP_CAM;
	Stage_Proc= X_AXIS_COMP_PROC;
	m_radioCameraIdx=eTOP_CAM_20X;

	m_ModelEditDisplay_L.GetWindowRect(m_dispRect_L);
	ScreenToClient(m_dispRect_L);
	disp_rect_L= m_dispRect_L;

	ModelNo= mdTOP_COMP_START; 

	m_pVision->InitModelSet_2(m_ModelEditDisplay_L.GetSafeHwnd());

	m_ctrlLedSlider.SetRange(0, 255) ;

 	SetTimer(frmCompView_TIMER_ID, frmCompView_TIMER_INTERVAL, NULL);

	int ret= m_pVision->PatternLoad(mdTOP_COMP_START);
	ret= m_pVision->PatternLoad(mdBTM_COMP_START);

	m_btnTopSel.SetValue(1);	m_btnBtmSel.SetValue(0);

	DataUpdate();

	m_ctrlGrid1.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	for(int i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		CompCal_data.Top_data_X[i].GetPos= 0.;
		CompCal_data.Top_data_X[i].Cal_um= 0.;
	}
	m_lblRunDisable_X.SetValue(1); m_lblRunEnable_X.SetValue(0); X_Enable_Flag= FALSE;

	CalCompData();
	LoadCompData();

	GridInitC1_1();
	GridDispC1_1();

	GetDlgItem(IDB_Z_TEST_START5)->EnableWindow(TRUE);
	GetDlgItem(IDB_Z_TEST_START6)->EnableWindow(FALSE);

	g_bCfrmCompViewInit= TRUE;

	bTopCompEnable= m_bTopCompEnable= TRUE;

	UpdateData(FALSE);	

}

//===================================================================================
void CfrmCompView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(frmCompView_TIMER_ID);

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

//===================================================================================
void CfrmCompView::OnSize(UINT nType, int cx, int cy)
{
	//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmCompView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmCompView::OnTimer(UINT_PTR nIDEvent)
{
	CString strText;
	
	SetDlgItemInt(IDC_STATIC_STEP, mCompProc_Step);
/*
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
*/
	CFormView::OnTimer(nIDEvent);
}

//===================================================================================
void CfrmCompView::DataUpdate()
	//-----------------------------------------------------------------------------------
{

	int ret= m_pVision->PatternLoad(ModelNo);

	if(ParamFile()->Pattern_data[ModelNo].iSpeed == 2)		m_ctrlSpeed.SetCurSel(0);
	else if(ParamFile()->Pattern_data[ModelNo].iSpeed == 3)	m_ctrlSpeed.SetCurSel(1);
	else if(ParamFile()->Pattern_data[ModelNo].iSpeed == 4)	m_ctrlSpeed.SetCurSel(2);
	else {
		ParamFile()->Pattern_data[ModelNo].iSpeed =M_VERY_HIGH;	m_ctrlSpeed.SetCurSel(2);
	}

	if(ParamFile()->Pattern_data[ModelNo].iAccuracy == 2)		m_ctrlAccuracy.SetCurSel(0);
	else if(ParamFile()->Pattern_data[ModelNo].iAccuracy == 3)	m_ctrlAccuracy.SetCurSel(1);
	else {	
		ParamFile()->Pattern_data[ModelNo].iAccuracy =M_MEDIUM;	m_ctrlAccuracy.SetCurSel(0);
	}

	if(ParamFile()->Pattern_data[ModelNo].dScoreAcceptance<= 0.0) ParamFile()->Pattern_data[ModelNo].dScoreAcceptance= 50.0;
	m_dScoreAcceptance =		ParamFile()->Pattern_data[ModelNo].dScoreAcceptance;

	if(ParamFile()->Pattern_data[ModelNo].dScoreCertanty<= 0.0) ParamFile()->Pattern_data[ModelNo].dScoreCertanty= 60.0;
	m_dScoreCertanty =			ParamFile()->Pattern_data[ModelNo].dScoreCertanty; //이상해??? Pattern_data

	if(ParamFile()->Pattern_data[ModelNo].iTimeOut<= 0) ParamFile()->Pattern_data[ModelNo].iTimeOut= 1000;
	m_TimeOut =					ParamFile()->Pattern_data[ModelNo].iTimeOut;

	m_dOffsetX=					ParamFile()->Pattern_data[ModelNo].dOffsetX;
	m_dOffsetY=					ParamFile()->Pattern_data[ModelNo].dOffsetY;

	m_pVision->PatternImageLoadNdisp2(ModelNo);

	m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_20X[ePLATE_3];
	m_ctrlLedSlider.SetPos(m_iLedcontrolVal);	

	m_dCompPitch= ParamFile()->System_data.dCompPitch;
	m_iCompCountX= ParamFile()->System_data.iCompCountX;
	m_iCompCountY= ParamFile()->System_data.iCompCountY;
	m_dCompRectSize= ParamFile()->System_data.dCompRectSize;


	Invalidate();
}
//===================================================================================
void CfrmCompView::DataSave()
	//-----------------------------------------------------------------------------------
{
	UpdateData(TRUE);
	ParamFile()->System_data.dCompPitch= m_dCompPitch;
	ParamFile()->System_data.iCompCountX= m_iCompCountX;
	ParamFile()->System_data.iCompCountY= m_iCompCountY;
	ParamFile()->System_data.dCompRectSize= m_dCompRectSize;

	switch(m_ctrlSpeed.GetCurSel())
	{
	case 0:		ParamFile()->Pattern_data[ModelNo].iSpeed = M_MEDIUM;		break;
	case 1:		ParamFile()->Pattern_data[ModelNo].iSpeed = M_HIGH;		break;
	case 2:		ParamFile()->Pattern_data[ModelNo].iSpeed = M_VERY_HIGH;	break;
	default:	ParamFile()->Pattern_data[ModelNo].iSpeed = M_VERY_HIGH;	break;
	}
	switch(m_ctrlAccuracy.GetCurSel())
	{
	case 0:		ParamFile()->Pattern_data[ModelNo].iAccuracy = M_MEDIUM;	break;
	case 1:		ParamFile()->Pattern_data[ModelNo].iAccuracy = M_HIGH;	break;
	default:    ParamFile()->Pattern_data[ModelNo].iAccuracy = M_MEDIUM;	break;
	}

	ParamFile()->Pattern_data[ModelNo].dScoreCertanty= m_dScoreCertanty;
	ParamFile()->Pattern_data[ModelNo].dScoreAcceptance= m_dScoreAcceptance;
	ParamFile()->Pattern_data[ModelNo].iTimeOut= m_TimeOut;

	ParamFile()->Pattern_data[ModelNo].dOffsetX= m_dOffsetX;
	ParamFile()->Pattern_data[ModelNo].dOffsetY= m_dOffsetY;

	ParamFile()->SaveFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadFile();
	//-------------------------------------------------------------	
	m_pVision->PatternSave(ModelNo);

// 	SaveCompData();
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();

	DataUpdate();
}
//===================================================================================
void CfrmCompView::DataSave2()
	//-----------------------------------------------------------------------------------
{
	UpdateData(TRUE);
	ParamFile()->System_data.dCompPitch= m_dCompPitch;
	ParamFile()->System_data.iCompCountX= m_iCompCountX;
	ParamFile()->System_data.iCompCountY= m_iCompCountY;
	ParamFile()->System_data.dCompRectSize= m_dCompRectSize;

	SaveCompData();

	ParamFile()->SaveFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadFile();


	LoadCompData();
	GridInitC1_1();	GridDispC1_1();

}

//===================================================================================
void CfrmCompView::DispUpdate()
	//-----------------------------------------------------------------------------------
{
}
//===================================================================================
LRESULT CfrmCompView::SetRoi_Confirm(WPARAM wParam,LPARAM lParam)
{
	m_ROI_Mode= false;
	m_btnROI_Set.SetValue(0);

	CImageObj* pImg = m_pVision->GetImageObj( m_radioCameraIdx );

	//m_pVision->RegistModelFinderImageForMask( m_radioCameraIdx, pImg );

	m_pVision->PatternReDefine2( m_radioCameraIdx, pImg, ModelNo);	
	//	m_pVision->PatternSave(ModelNo);	
	DataSave();
	m_pVision->PatternLoad(ModelNo);

	DataUpdate(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
	return 0;
}
//===================================================================================
LRESULT CfrmCompView::DataReload(WPARAM wParam,LPARAM lParam)
{
	int ret= m_pVision->PatternLoad(mdTOP_COMP_START);
	ret= m_pVision->PatternLoad(mdBTM_COMP_START);

	DataUpdate();
	return 0;
}

LRESULT CfrmCompView::TopLoadImage(WPARAM wParam,LPARAM lParam)
{
	CString strText;

	int CalCount= (int)lParam;
	strText.Format(_T("d:\\Work\\Top\\%03d.tif"), CalCount);	pSystem.GetVisionComponent()->Load_Image(eTOP_CAM_20X, strText.GetBuffer(0));
	TopLoadOkFlag= TRUE;
	return 0;
}
LRESULT CfrmCompView::BtmLoadImage(WPARAM wParam,LPARAM lParam)
{
	CString strText;
	int CalCount= (int)lParam;
	strText.Format(_T("d:\\Work\\Btm\\%03d.tif"), CalCount);	pSystem.GetVisionComponent()->Load_Image(eBTM_CAM_20X, strText.GetBuffer(0));
	BtmLoadOkFlag= TRUE;
	return 0;
};
LRESULT CfrmCompView::CompDataUpdate(WPARAM wParam,LPARAM lParam)
{
	GridDispC1_1();
	return 0;
};
//===================================================================================
void CfrmCompView::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	if(CamNo== eTOP_CAM)
	{
		m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_20X[ePLATE_3]= m_ctrlLedSlider.GetPos();	
		UpdateData(FALSE) ;
	}
	else
	{
		m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_20X[ePLATE_3]= m_ctrlLedSlider.GetPos();	
		UpdateData(FALSE) ;
	}
	if(ParamFile()->System_data.Led_Remote)	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_3);
	//	SetLed(UsedNo);
	*pResult = 0;
}

//===================================================================================
BOOL CfrmCompView::MoveEndCheck(int Axis) 
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
void CfrmCompView::MovePos(int pos, BOOL Z_Flag)
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

	double toPos_X= 0.0, toPos_TopZ= 0.0, toPos_TopY= 0.0, toPos_BtmZ= 0.0, toPos_BtmX= 0.0, toPos_BtmY= 0.0;
	switch(dispPos_Sel) 
	{
	case COMP_POS_TOP_START:
		strPos.Format(_T("SX= %.3f:TY=%.3f:TZ=%.3f]"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START),												
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_X_AXIS_COMP_TOP_START),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_X_AXIS_COMP_TOP_START));
		break;
	case COMP_POS_TOP_END:
		strPos.Format(_T("SX= %.3f:TY=%.3f:TZ=%.3f]"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START),												
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_X_AXIS_COMP_TOP_START),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_X_AXIS_COMP_TOP_START));
		break;
	case COMP_POS_BTM_START:
		strPos.Format(_T("SX= %.3f:TY=%.3f:TZ=%.3f]"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START),												
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_X_AXIS_COMP_TOP_START),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_X_AXIS_COMP_TOP_START));
		break;
	case COMP_POS_BTM_END:
		strPos.Format(_T("SX= %.3f:TY=%.3f:TZ=%.3f]"),	
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START),												
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_X_AXIS_COMP_TOP_START),
			pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_X_AXIS_COMP_TOP_START));
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
//	if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
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
	case COMP_POS_TOP_START:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_X_AXIS_COMP_TOP_START);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_X_AXIS_COMP_TOP_START);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_X_AXIS_COMP_TOP_START);
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

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


void CfrmCompView::OnBnClickedButton2()
{
	pSystem.GetVisionComponent()->Load_Image(eTOP_CAM_20X, _T("d:\\Work\\Top.bmp"));
	pSystem.GetVisionComponent()->Load_Image(eBTM_CAM_20X, _T("d:\\Work\\Btm.bmp"));
}
//=================================================================================== 
BEGIN_EVENTSINK_MAP(CfrmCompView, CFormView)
	ON_EVENT(CfrmCompView, IDB_JOG_SELECT_ALIGN, DISPID_CLICK, CfrmCompView::ClickJogSelectAlign, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_JOG_SELECT_MASK3, DISPID_CLICK, CfrmCompView::ClickJogSelectMask3, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_MARK_ROI_SEL, DISPID_CLICK, CfrmCompView::ClickMarkRoiSel, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_MARK_TEST, DISPID_CLICK, CfrmCompView::ClickMarkTest, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_MARK_SAVE, DISPID_CLICK, CfrmCompView::ClickMarkSave, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_Z_TEST_START5, DISPID_CLICK, CfrmCompView::ClickZTestStart5, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_Z_TEST_START6, DISPID_CLICK, CfrmCompView::ClickZTestStart6, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_MARK_SAVE2, DISPID_CLICK, CfrmCompView::ClickMarkSave2, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_Z_TEST_START, DISPID_CLICK, CfrmCompView::ClickZTestStart, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_Z_TEST_START2, DISPID_CLICK, CfrmCompView::ClickZTestStart2, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_CM_MOVE11, DISPID_CLICK, CfrmCompView::ClickCmMove11, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_CM_MOVE17, DISPID_CLICK, CfrmCompView::ClickCmMove17, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_CM_MOVE2, DISPID_CLICK, CfrmCompView::ClickCmMove2, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_OPU_ROLL_Z30, DISPID_CLICK, CfrmCompView::ClickOpuRollZ30, VTS_NONE)
	ON_EVENT(CfrmCompView, IDB_OPU_ROLL_Z32, DISPID_CLICK, CfrmCompView::ClickOpuRollZ32, VTS_NONE)
END_EVENTSINK_MAP()

//=================================================================================== 
void CfrmCompView::ClickJogSelectAlign()
{
	m_btnTopSel.SetValue(1);	m_btnBtmSel.SetValue(0);
	LensNo= e20X_LENS;
	CamNo= eTOP_CAM;
	m_radioCameraIdx=eTOP_CAM_20X;

	ModelNo= mdTOP_COMP_START; 
	DataUpdate(); //m_pVision->PatternImageLoadNdisp(ModelNo);
}

//=================================================================================== 
void CfrmCompView::ClickJogSelectMask3()
{
	m_btnTopSel.SetValue(0);	m_btnBtmSel.SetValue(1);

	LensNo= e20X_LENS;
	CamNo= eBTM_CAM;
	m_radioCameraIdx=eBTM_CAM_20X;

	ModelNo= mdBTM_COMP_START; 
	DataUpdate(); //m_pVision->PatternImageLoadNdisp(ModelNo);
}

//=================================================================================== 
void CfrmCompView::ClickMarkRoiSel()
{
	UpdateData( TRUE );

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(m_btnROI_Set.GetValue())
	{
		m_ROI_Mode= true;
		if(CamNo== eTOP_CAM)
		{
			pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
		}
		else
		{
			pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
		}
		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

		m_pVision->SetTrackerMode( m_radioCameraIdx, TRUE );
	}
	else
	{
		m_ROI_Mode= false;
		m_pVision->SetTrackerMode( m_radioCameraIdx, FALSE );
	}

	UpdateData( FALSE );
}

//=================================================================================== 
void CfrmCompView::ClickMarkTest()
{
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 

	m_pVision->OverlayClear(eTOP_CAM_2X);
	m_pVision->OverlayClear(eTOP_CAM_20X);
	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	if(CamNo== eTOP_CAM)	m_pVision->DrawUpdate(eTOP_CAM_20X);
	else					m_pVision->DrawUpdate(eBTM_CAM_20X);
}

//=================================================================================== 
void CfrmCompView::ClickMarkSave()
{
	CString strText;
	//-------------------------------------------------------------	
	if (MyMessageBox(_T("설정된 Mark 정보를 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmCompView::Data_Save.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmCompView::Data_Save.!"));
	//-------------------------------------------------------------	
	DataSave() ;
}

//=================================================================================== 
void CfrmCompView::OnBnClickedButtonU()
{
	UpdateData(TRUE);
	int mmm= 1;
	switch(m_iMovePix_Sel)
	{
	case 0: mmm= 1; break;
	case 1: mmm= 5; break;
	case 2: mmm= 10; break;
	default: mmm= 1; break;
	}
	//	ParamFile()->Pattern_data[ModelNo].dOffsetX= m_dOffsetX;
	ParamFile()->Pattern_data[ModelNo].dOffsetY-= mmm;
	//	ParamFile()->Pattern_data[iModelNo].dOffsetY-=mmm;
	m_dOffsetX=					ParamFile()->Pattern_data[ModelNo].dOffsetX;
	m_dOffsetY=					ParamFile()->Pattern_data[ModelNo].dOffsetY;
	UpdateData(FALSE);
	pSystem.GetVisionComponent()->PatternSave(ModelNo);	pSystem.GetVisionComponent()->PatternLoad(ModelNo);

	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	if(CamNo== eTOP_CAM)	m_pVision->DrawUpdate(eTOP_CAM_20X);
	else					m_pVision->DrawUpdate(eBTM_CAM_20X);
}


void CfrmCompView::OnBnClickedButtonD()
{
	UpdateData(TRUE);
	int mmm= 1;
	switch(m_iMovePix_Sel)
	{
	case 0: mmm= 1; break;
	case 1: mmm= 5; break;
	case 2: mmm= 10; break;
	default: mmm= 1; break;
	}
	ParamFile()->Pattern_data[ModelNo].dOffsetY+=mmm;
	m_dOffsetX=					ParamFile()->Pattern_data[ModelNo].dOffsetX;
	m_dOffsetY=					ParamFile()->Pattern_data[ModelNo].dOffsetY;
	UpdateData(FALSE);
	pSystem.GetVisionComponent()->PatternSave(ModelNo);	pSystem.GetVisionComponent()->PatternLoad(ModelNo);

	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	if(CamNo== eTOP_CAM)	m_pVision->DrawUpdate(eTOP_CAM_20X);
	else					m_pVision->DrawUpdate(eBTM_CAM_20X);
}


void CfrmCompView::OnBnClickedButtonL2()
{
	UpdateData(TRUE);
	int mmm= 1;
	switch(m_iMovePix_Sel)
	{
	case 0: mmm= 1; break;
	case 1: mmm= 5; break;
	case 2: mmm= 10; break;
	default: mmm= 1; break;
	}
	ParamFile()->Pattern_data[ModelNo].dOffsetX-=mmm;
	m_dOffsetX=					ParamFile()->Pattern_data[ModelNo].dOffsetX;
	m_dOffsetY=					ParamFile()->Pattern_data[ModelNo].dOffsetY;
	UpdateData(FALSE);
	pSystem.GetVisionComponent()->PatternSave(ModelNo);	pSystem.GetVisionComponent()->PatternLoad(ModelNo);

	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	if(CamNo== eTOP_CAM)	m_pVision->DrawUpdate(eTOP_CAM_20X);
	else					m_pVision->DrawUpdate(eBTM_CAM_20X);
}


void CfrmCompView::OnBnClickedButtonR2()
{
	UpdateData(TRUE);
	int mmm= 1;
	switch(m_iMovePix_Sel)
	{
	case 0: mmm= 1; break;
	case 1: mmm= 5; break;
	case 2: mmm= 10; break;
	default: mmm= 1; break;
	}
	ParamFile()->Pattern_data[ModelNo].dOffsetX+=mmm;
	m_dOffsetX=					ParamFile()->Pattern_data[ModelNo].dOffsetX;
	m_dOffsetY=					ParamFile()->Pattern_data[ModelNo].dOffsetY;
	UpdateData(FALSE);
	pSystem.GetVisionComponent()->PatternSave(ModelNo);	pSystem.GetVisionComponent()->PatternLoad(ModelNo);

	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	if(CamNo== eTOP_CAM)	m_pVision->DrawUpdate(eTOP_CAM_20X);
	else					m_pVision->DrawUpdate(eBTM_CAM_20X);
}
//===================================================================================
void CfrmCompView::GridInitC1_1()
{
	BOOL bVirtualMode = FALSE;
	int row, col, GridNo;
	GridNo = 0; 
// 	if(ParamFile()->System_data.iCompCountX > ParamFile()->System_data.iCompCountY)
		row = ParamFile()->System_data.iCompCountX+2;
// 	else 
// 		row= ParamFile()->System_data.iCompCountY+2;
	col = 5;
	col = 3;

	if (m_ctrlGrid1.GetColumnCount())
		m_ctrlGrid1.DeleteAllItems();

	m_ctrlGrid1.SetEditable(FALSE);
	//m_ctrlGrid1.SetEditable(TRUE);
	m_ctrlGrid1.SetVirtualMode(bVirtualMode);
	m_ctrlGrid1.SetListMode(TRUE);
	m_ctrlGrid1.EnableDragAndDrop(FALSE);
	m_ctrlGrid1.EnableSelection(FALSE);
	m_ctrlGrid1.SetFixedRowSelection(TRUE);
	m_ctrlGrid1.SetFixedColumnSelection(TRUE);
	m_ctrlGrid1.SetFrameFocusCell(FALSE);
	m_ctrlGrid1.SetTrackFocusCell(FALSE);
	m_ctrlGrid1.SetRowResize(FALSE);
	m_ctrlGrid1.SetColumnResize(FALSE);
	//m_ctrlGrid1.SetHeaderSort(TRUE);
	//m_ctrlGrid1.ExpandToFit(TRUE);

	m_ctrlGrid1.SetRowCount(row);
	m_ctrlGrid1.SetColumnCount(col);

	//int nLastMergeID = m_ctrlGrid1.MergeCells(CCellRange(nMinRow, nMinCol, nMaxRow, nMaxCol));
	m_ctrlGrid1.SetFixedColumnCount(1);	// 1 Fixed Column
	m_ctrlGrid1.SetFixedRowCount(1);		// 1 Fixed Row
	// 	m_ctrlGrid1.SetFixedRowCount(2);	// 1 Fixed Column
	// 	m_ctrlGrid1.SetFixedRowCount(3);	// 1 Fixed Column

	//m_ctrlGrid1.SetColumnWidth(1, 70);
	m_ctrlGrid1.SetRowHeight(0, 23);//0, 25
	m_ctrlGrid1.SetRowHeight(1, 23);//0, 25
	for (int i = 1; i < m_ctrlGrid1.GetRowCount(); i++)
		m_ctrlGrid1.SetRowHeight(i, 22);//0, 25
	m_ctrlGrid1.SetBkColor(RGB(197, 197, 187));
	m_ctrlGrid1.SetTextBkColor(RGB(235, 235, 235));
	//m_ctrlGrid1.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	//m_ctrlGrid1.SetAutoSizeStyle();

	for (int i = 0; i < m_ctrlGrid1.GetColumnCount(); i++)
	{
// 		if (i == 0)		m_ctrlGrid1.SetColumnWidth(i, 30);
// 		else if (i == 1 || i == 3)		m_ctrlGrid1.SetColumnWidth(i, 65);
// 		else							m_ctrlGrid1.SetColumnWidth(i, 50);

		if (i == 0)		m_ctrlGrid1.SetColumnWidth(i, 50);
		else if (i == 1 || i == 3)		m_ctrlGrid1.SetColumnWidth(i, 85);
		else							m_ctrlGrid1.SetColumnWidth(i, 80);

		m_ctrlGrid1.SetItemBkColour(0, i, RGB(146, 146, 141));
		m_ctrlGrid1.SetItemFgColour(0, i, RGB(255, 255, 255));
		// 		if (i > 0)
		// 		{
		// 			m_ctrlGrid1.SetItemBkColour(1, i, RGB(146, 146, 141));
		// 			m_ctrlGrid1.SetItemFgColour(1, i, RGB(255, 255, 255));
		// 			m_ctrlGrid1.SetItemBkColour(2, i, RGB(146, 146, 141));
		// 			m_ctrlGrid1.SetItemFgColour(2, i, RGB(255, 255, 255));
		// 		}
	}

	DWORD dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE;    // Text Style

/*
	LOGFONT lf;
	lf.lfHeight = 11;//13;                                                              // 크기
	lf.lfWidth = 0;                                                                 // 굵기 700(bold)
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;//FW_BOLD,FW_NORMAL
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(lf.lfFaceName, _T("Times New Romans"));//돋음                              // 글씨체
*/


	for (int row = 0; row < m_ctrlGrid1.GetRowCount(); row++)
	{
		for (int col = 0; col < m_ctrlGrid1.GetColumnCount(); col++)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
// 			lf = *(m_ctrlGrid1.GetItemFont(0, 0));
// 			lf.lfHeight = -16;//13;                                                              // 크기
			UINT state = m_ctrlGrid1.GetItemState(row, col);

			if (col < 1)
			{
// 				m_ctrlGrid1.SetItemFont(row, col, &lf);
// 				Item.nFormat = dwTextStyle;
				switch (row)
				{
				case 0:	Item.strText.Format(_T("No."));	break;
				default:	Item.strText.Format(_T("%d"), row-1);	break;
				}
			}
			else
			{
				if (row == 0)
				{
// 					m_ctrlGrid1.SetItemFont(row, col, &lf);
					switch (col)
					{
					case 1:
						Item.strText.Format(_T("Pos"));	break;
					case 2:
						Item.strText.Format(_T("X1~2"));	break;
					case 3:
						Item.strText.Format(_T("Pos"));	break;
					case 4:
						Item.strText.Format(_T("X3-4"));	break;
					}
					Item.nFormat = DT_CENTER | DT_WORDBREAK;
					m_ctrlGrid1.SetItemState(row, col, state | GVIS_READONLY);
				}
				else
				{
// 					lf.lfHeight = -25;//13;                                                              // 크기
// 					m_ctrlGrid1.SetItemFont(row, col, &lf);
					Item.nFormat = dwTextStyle;
					switch (col)
					{
					case 1:
					case 3:
						Item.strText.Format(_T("987.1256"));
						break;
					case 2:
					case 4:
						Item.strText.Format(_T("89.9"));
						break;
					}
				}
			}
			if (row > 1)
			{
				if (col > 1)
				{
					m_ctrlGrid1.SetItemBkColour(row, col, RGB(235, 235, 235));
			//		m_ctrlGrid1.SetItemState(row, col, state | GVIS_READONLY);
				}
			}
			m_ctrlGrid1.SetItem(&Item);
		}
	}
}
//===================================================================================
void CfrmCompView::GridDispC1_1()
{
	DWORD dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE;    // Text Style
	BOOL bVirtualMode = FALSE;
	int row, col, GridNo;
	GridNo = 0; 
// 	if(ParamFile()->System_data.iCompCountX > ParamFile()->System_data.iCompCountY)
		row = ParamFile()->System_data.iCompCountX;
// 	else 
// 		row= ParamFile()->System_data.iCompCountY;
	col = 5;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.row = row;
	Item.col = col;
	Item.nFormat = DT_CENTER | DT_WORDBREAK;

	row = 1;

	for (int row = 1; row < m_ctrlGrid1.GetRowCount(); row++)
	{
		for (int col = 1; col < m_ctrlGrid1.GetColumnCount(); col++)
		{
			UINT state = m_ctrlGrid1.GetItemState(row, col);
			Item.row = row;
			Item.col = col;

			switch(col)
			{
			case 1:
				Item.strText.Format(_T("%.4f"),CompCal_data.Top_data_X[row-1].GetPos);//MovePos);
				break;
			case 2:
				Item.strText.Format(_T("%.4f"), CompCal_data.Top_data_X[row-1].Cal_um);
				break;
			case 3:
				Item.strText.Format(_T("%.4f"),CompCal_data.Btm_data_X[row-1].GetPos);//MovePos);
				break;
			case 4:
				Item.strText.Format(_T("%.4f"), CompCal_data.Btm_data_X[row-1].Cal_um);
				break;
			default:
				break;
			}
				// 			CompCal_data.Top_data_X[i].MovePos= step_pos;
				// 			CompCal_data.Top_data_X[i].GetPos= 0.0;
				// 			CompCal_data.Top_data_X[i].Cal_um= 0.0;

//			m_ctrlGrid1.SetItemState(row, col, state | GVIS_READONLY);
			m_ctrlGrid1.SetItemState(row, col, state | GVIS_EDITABLE);
			m_ctrlGrid1.SetItem(&Item);
		}
		m_ctrlGrid1.RedrawRow(row);
	}
}

//=================================================================================== 
void CfrmCompView::RunThread_1(int nID)
{
	m_bExec_1[nID] = TRUE; 
	tagTREADPARAMS_COMP *pThreadParams = new tagTREADPARAMS_COMP;
	pThreadParams->pContinue = &m_bExec_1[nID];
	//	pThreadParams->pWnd =	((CMainFrame*)(AfxGetApp()->GetMainWnd()));
	pThreadParams->pWnd =  ((CfrmCompView*)this);
	if(m_pThread_1[nID] == NULL) {
		switch(nID) {
		case 0: 
			m_pThread_1[nID] = AfxBeginThread(Thread_Comp_Proc_1, pThreadParams);  break;
		case 1: 
			break;
		case 2: 
			break;
		}
	}
	m_pThread_1[nID]->m_bAutoDelete = true;
}
void CfrmCompView::KillThread_1(int nID)
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
UINT Thread_Comp_Proc_1(LPVOID pParam)
//-----------------------------------------------------------------------------------	
{
	tagTREADPARAMS_COMP *pThreadParams = (tagTREADPARAMS_COMP *)pParam;
	CWnd *pWnd = pThreadParams->pWnd;//THREADPAPAMS_Mag1;
	BOOL *pContinue = pThreadParams->pContinue;
	delete pThreadParams;
	
	CfrmCompView* pOwner =  (CfrmCompView*)pWnd ;
	int CalCount= 0;
	CString strText;
	bool ret1= false, ret2= false;
	DWORD st= 0, st1= 0, dwWaitTime= 0;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	double angle1= 0.0, posx1= 0.0, posy1= 0.0;
	double angle2= 0.0, posx2= 0.0, posy2= 0.0;
	double def_posx1= 0.0, def_posy1= 0.0;
	double def_posx2= 0.0, def_posy2= 0.0;
	double ToPosition1	= 0.0;

	while(*pContinue)
	{
		if(PROGRAM_EXIT) { Sleep(10); continue; }
		if(!pOwner->mCompProcStart)  { Sleep(10); continue; }

		switch(pOwner->mCompProc_Step)
		{
		case 0: Sleep(10);	continue;
		//---------------------------------------------------------------------------------------
		case 100:
			if(pSystem.GetMotionComponent()->isDone(_ACS_AXIS_STAGE_X)) 
			{
				pSystem.GetVisionComponent()->InspGrabContinue();
				if(!pOwner->bTopCompEnable)
				{
					pOwner->mCompProc_Step= 600;
				}
				else
				{
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
					pOwner->TopLoadOkFlag=  pOwner->BtmLoadOkFlag= FALSE;
					//pOwner->SendMessage(WM_MSG_TOP_LOADIMAGE, 0, (LPARAM)CalCount);
					dwWaitTime= GetTickCount();
					pOwner->mCompProc_Step= 200;
				}
			}
			break;
		case 200:
//			if(pOwner->TopLoadOkFlag)
			if(GetTickCount() - dwWaitTime > 500)
			{
				angle1= 0.0; posx1= 0.0; posy1= 0.0;
				angle2= 0.0; posx2= 0.0; posy2= 0.0;

				pSystem.GetVisionComponent()->InspGrab(eTOP_CAM_20X);
				ret1= pSystem.GetVisionComponent()->PatternFinderPos(&angle1, &posx1, &posy1, mdTOP_COMP_START, eTOP_CAM_20X, true, true, true);
				if(CalCount== 0)
				{
					def_posx1= posx1; def_posy1= posy1;
				}
				pSystem.GetVisionComponent()->InspGrabContinue();


				pOwner->mCompProc_Step= 300;
				pOwner->TopLoadOkFlag=  pOwner->BtmLoadOkFlag= FALSE;
			}
			break;
		case 300:
			pOwner->CompCal_data.Top_data_X[CalCount].GetPos= pOwner->CompCal_data.Top_data_X[CalCount].MovePos;
			if(ret1)
				pOwner->CompCal_data.Top_data_X[CalCount].Cal_um= ( (def_posx1-posx1)*pOwner->ParamFile()->System_data.dTop_Cam_PixelToum_20X ) / 1000.0;
			else
				pOwner->CompCal_data.Top_data_X[CalCount].Cal_um= 0.0;
//			pOwner->CalData[]
			pOwner->mCompProc_Step= 500;
			break;
		//---------------------------------------------------------------------------------------
		case 500:
			CalCount++;
			if(CalCount >= pOwner->CompCal_data.X_Count)
			{
				pOwner->mCompProc_Step= 600;
			}
			else
			{
				ToPosition1= pOwner->CompCal_data.Top_data_X[CalCount].MovePos;
				if(ToPosition1> 0.0)
				{
					//				pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->System_data.dCompPitch, _SPD_MODE, TRUE);		
					// 				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
					pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE);
				}
				pOwner->mCompProc_Step= 100;
			}
			pOwner->SendMessage(WM_MSG_GRID_COMPUPDATE, 0, 0);
		break;
		//---------------------------------------------------------------------------------------
		case 600:
			pOwner->mCompProc_Step= 900;
			break;
// 			if(!pOwner->bBtmCompEnable)		{		pOwner->mCompProc_Step= 900;				}
// 			else							{		CalCount= 0;	pOwner->mCompProc_Step= 700;	}
// 			break;
		//---------------------------------------------------------------------------------------
		case 700:
// 			if(!pOwner->bTopCompEnable)
// 			{
// 				pOwner->mCompProc_Step= 900;
// 			}
// 			else
// 			{
// 				pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_20X);
// 				pOwner->TopLoadOkFlag=  pOwner->BtmLoadOkFlag= FALSE;
// 				pOwner->SendMessage(WM_MSG_BTM_LOADIMAGE, 0, (LPARAM)CalCount);
// 
// 				pOwner->mCompProc_Step= 800;
// 			}
			break;
		case 800:
// 			if(pOwner->BtmLoadOkFlag)
// 			{
// 				angle2= 0.0; posx2= 0.0; posy2= 0.0;
// 
// 				ret2= pSystem.GetVisionComponent()->PatternFinderPos(&angle2, &posx2, &posy2, mdBTM_COMP_START, eBTM_CAM_20X, true, true, true);
// 				if(CalCount== 0)
// 				{
// 					def_posx2= posx2; def_posy2= posy2;
// 				}
// 
// 				pOwner->mCompProc_Step= 850;
// 				pOwner->TopLoadOkFlag=  pOwner->BtmLoadOkFlag= FALSE;
// 			}
			break;
		case 850:
// 			pOwner->CompCal_data.Btm_data_X[CalCount].GetPos= pOwner->CompCal_data.Btm_data_X[CalCount].MovePos;
// 			pOwner->CompCal_data.Btm_data_X[CalCount].Cal_um= ( (def_posx2-posx2)*pOwner->ParamFile()->System_data.dBot_Cam_PixelToum_20X );
// 
// 			pOwner->mCompProc_Step= 900;
			break;
	//---------------------------------------------------------------------------------------
		case 900:
			CalCount++;
			if(CalCount >= pOwner->CompCal_data.X_Count)
			{
				pOwner->mCompProc_Step= 0;
				pOwner->SendMessage(WM_MSG_GRID_COMPUPDATE, 0, 0);
				AfxMessageBox(_T("완료 되었습니다."));
				pOwner->GetDlgItem(IDB_Z_TEST_START5)->EnableWindow(TRUE);
				pOwner->GetDlgItem(IDB_Z_TEST_START6)->EnableWindow(FALSE);
			}
			else
				pOwner->mCompProc_Step= 700;
			break;
		}
		TRACE(" --- Thread_Comp_Proc_1 => %d  [%d]\n", pOwner->mCompProc_Step, CalCount);
		Sleep(100);
	}
	return 0;
}
//===================================================================================
void CfrmCompView::CompProc_Start()
{
	CString strText;
	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	mCompProcStart= TRUE;
	mCompProc_Step= 100;
	RunThread_1(0);
}
bool CfrmCompView::isCompProc_End()
{
	if(mCompProc_Step== 999)
		return true;
	return false;
}
void CfrmCompView::CompProc_End()
{
	mCompProcStart= FALSE;
	mCompProc_Step= 0;
	
	pSystem.GetMotionComponent()->MotorAllStop();

	KillThread_1(0);
}

//===================================================================================
void CfrmCompView::OnEnChangeEditSmoothness6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//===================================================================================
void CfrmCompView::OnBnClickedMaskEditApply3()
{
	LoadCompData();

	GridInitC1_1();
	GridDispC1_1();

	return;

	//CalCompData();
	//m_pVision->Test1();

	int row, col;
	col = 5;
	col = 3;

	col= m_ctrlGrid1.GetColumnCount();
	row= m_ctrlGrid1.GetRowCount();

	double aaa1= 0., aaa2= 0.;
	CString strText1, strText2;
	for (int row = 1; row < m_ctrlGrid1.GetRowCount(); row++)
	{
		strText1= m_ctrlGrid1.GetItemText(row, 1);
		strText2= m_ctrlGrid1.GetItemText(row, 2);
		aaa1= _wtof(strText1.GetBuffer(0)); strText1.ReleaseBuffer();
		aaa2= _wtof(strText2.GetBuffer(0)); strText2.ReleaseBuffer();
	}
}

//===================================================================================
void CfrmCompView::OnBnClickedMaskEditApply4()
{
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;	

	for(int i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		CompCal_data.Top_data_X[i].MovePos= 0.0;
		CompCal_data.Top_data_X[i].GetPos= 0.0;
		CompCal_data.Top_data_X[i].Cal_um= 0.0;
		CompCal_data.Btm_data_X[i].MovePos= 0.0;
		CompCal_data.Btm_data_X[i].GetPos= 0.0;
		CompCal_data.Btm_data_X[i].Cal_um= 0.0;
		CompCal_data.Top_data_Y[i].MovePos= 0.0;
		CompCal_data.Top_data_Y[i].GetPos= 0.0;
		CompCal_data.Top_data_Y[i].Cal_um= 0.0;

		CalData[i]= 0.0;
	}
	CalCompData();

	GridInitC1_1();
	GridDispC1_1();

	SaveCompData();
	LoadCompData();
	GridInitC1_1();
	GridDispC1_1();
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
}


void CfrmCompView::OnBnClickedMaskEditApply5()
{
	BuildCompData();
}
//===================================================================================
void CfrmCompView::CalCompData()
{
	double X_St= 800.0;
	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_BTM_START);
	double dist= ToPosition2 - ToPosition1;
	if(dist < 5.0)
	{
		if(ToPosition1 > 0.0 && ToPosition1 > 0.0)	
			AfxMessageBox(_T("보정 데이터 생성이 실패 했습니다.[ Position ]"));
		return;
	}
	int step= (int)(dist / ParamFile()->System_data.dCompPitch);

	double chgToPosition2= ParamFile()->System_data.dCompPitch * step;
	chgToPosition2= chgToPosition2 + ToPosition1;
	ParamFile()->Model_data.dPosition[POSITION_X_AXIS_COMP_BTM_START][_ACS_AXIS_STAGE_X]= chgToPosition2;

	dist= 800.0 - ToPosition1;
	int stepCnt= (int)(dist / ParamFile()->System_data.dCompPitch);
	if(stepCnt<= 0 || stepCnt >= _MAX_COMP_CAL_DATA_COUNT)
	{
		AfxMessageBox(_T("보정 데이터 생성이 실패 했습니다. [stepCnt]"));
		return;
	}
	for(int i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		CompCal_data.Top_data_X[i].MovePos= 0.0;
		CompCal_data.Top_data_X[i].GetPos= 0.0;
		CompCal_data.Top_data_X[i].Cal_um= 0.0;
		CompCal_data.Btm_data_X[i].MovePos= 0.0;
		CompCal_data.Btm_data_X[i].GetPos= 0.0;
		CompCal_data.Btm_data_X[i].Cal_um= 0.0;
		CompCal_data.Top_data_Y[i].MovePos= 0.0;
		CompCal_data.Top_data_Y[i].GetPos= 0.0;
		CompCal_data.Top_data_Y[i].Cal_um= 0.0;

		CalData[i]= 0.0;
	}
	CompCal_data.X_Count= ParamFile()->System_data.iCompCountX+1;
	CompCal_data.Y_Count= ParamFile()->System_data.iCompCountY+1;
	if(CompCal_data.X_Count<= 0)
	{
		AfxMessageBox(_T("보정 데이터 생성이 실패 했습니다. [X_Count]"));
		return;
	}

	double step_pos= 0.0;
	for(int i= 0; i< CompCal_data.X_Count; i++)
	{
		step_pos= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START) + 
			(i * ParamFile()->System_data.dCompPitch);

		CompCal_data.Top_data_X[i].MovePos= step_pos;
		CompCal_data.Top_data_X[i].GetPos= step_pos;
		CompCal_data.Top_data_X[i].Cal_um= 0.0;
	}
	for(int i= 0; i< CompCal_data.X_Count; i++)
	{
		step_pos= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_BTM_START) + 
			(i * ParamFile()->System_data.dCompPitch);

		CompCal_data.Btm_data_X[i].MovePos= step_pos;
		CompCal_data.Btm_data_X[i].GetPos= step_pos;
		CompCal_data.Btm_data_X[i].Cal_um= 0.0;
	}
}
//===================================================================================
double CfrmCompView::CalBtmPosData()
{
	double X_St= 800.0;
	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) - pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK);

	int step= (int)(ToPosition2 / ParamFile()->System_data.dCompPitch);

	double chgToPosition2= ParamFile()->System_data.dCompPitch * step;

	chgToPosition2= ToPosition1 + chgToPosition2;

	return chgToPosition2;
}
//===================================================================================
void CfrmCompView::BuildCompData()
{

	double step_pos= 0.0;
	int i= 0, xx= 0;

	for( i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		step_pos= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START) + 
			(i * ParamFile()->System_data.dCompPitch);
		if(bTopCompEnable)
		{
			CalData[i]= CompCal_data.Top_data_X[xx].Cal_um;
		}
		xx++;
		if(xx >= CompCal_data.X_Count)
		{
			i++;
			CalData[i]= CompCal_data.Top_data_X[CompCal_data.X_Count-1].Cal_um;
			i++;
			CalData[i]= CompCal_data.Top_data_X[CompCal_data.X_Count-1].Cal_um;
			break;
		}
	}
	 
	double btmStartpos= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
	for( xx= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		step_pos= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START) + 
			(i * ParamFile()->System_data.dCompPitch);

		if(step_pos < CompCal_data.Btm_data_X[0].MovePos)
		{
			CalData[i]= 0.0;//CompCal_data.Top_data_X[CompCal_data.X_Count-1].Cal_um;
		}
		else
		{
			if(bBtmCompEnable)
			{
				CalData[i]= CompCal_data.Btm_data_X[xx].Cal_um;
			}
			else
			{
				;
			}
			xx++;
			if(xx >= CompCal_data.X_Count)
			{
				i++;
				CalData[i]= CompCal_data.Btm_data_X[CompCal_data.X_Count-1].Cal_um;
				i++;
				CalData[i]= CompCal_data.Btm_data_X[CompCal_data.X_Count-1].Cal_um;
				break;
			}
		}
	}
	for( xx= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		CalData[i]= 0.0;
	}
	SaveCompData();
}
//===================================================================================
void CfrmCompView::LoadCompData()
{
	TCHAR buf[500], value[500], ReadModel[500];
	CString strText;
	TCHAR szParamFile[255];
	int i= 0;

	_stprintf(szParamFile, _T("%s\\Comp.dat_"), ParamFile()->strDirConfigPath);

	_stprintf(ReadModel, _T("COMP_DATA"));

	for(i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++) 
	{
		_stprintf(buf, _T("Data_%d"), i);	GetPrivateProfileString(ReadModel, buf, _T("200.0"),  value, sizeof(value), szParamFile);	CalData[i]= _wtof(value);
	
		_stprintf(buf, _T("Pos_%d"), i);
		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile);	
		CompCal_data.Top_data_X[i].GetPos= _wtof(value);

		_stprintf(buf, _T("Calc_%d"), i);
		GetPrivateProfileString(ReadModel, buf, _T("0.0"),  value, sizeof(value), szParamFile);
		CompCal_data.Top_data_X[i].Cal_um= _wtof(value);
	}
}
void CfrmCompView::SaveCompData()
{
	int row, col;
	col = 5;
	col = 3;
	int i= 0, cnt= 0;

	col= m_ctrlGrid1.GetColumnCount();
	row= m_ctrlGrid1.GetRowCount();
	CString strText1, strText2;
	double aaa1, aaa2;

	for (int row = 1; row < m_ctrlGrid1.GetRowCount(); row++)
	{
// 		for (int col = 1; col < m_ctrlGrid1.GetColumnCount(); col++)
// 		{
			strText1= m_ctrlGrid1.GetItemText(row, 1);
			strText2= m_ctrlGrid1.GetItemText(row, 2);
			aaa1= _wtof(strText1.GetBuffer(0)); strText1.ReleaseBuffer();
			aaa2= _wtof(strText2.GetBuffer(0)); strText2.ReleaseBuffer();
			CompCal_data.Top_data_X[cnt].MovePos= CompCal_data.Top_data_X[cnt].GetPos= aaa1;
			CompCal_data.Top_data_X[cnt].Cal_um= aaa2;
			cnt++;
// 		}
	}

	//---------------------------------------------------------------------------
	TCHAR buf[500], value[500], ReadModel[500];
	CString strText;
	TCHAR szParamFile[255];

	_stprintf(szParamFile, _T("%s\\Comp.dat_"), ParamFile()->strDirConfigPath);
	_stprintf(ReadModel, _T("COMP_DATA"));

	for(i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++) 
	{
		_stprintf(value, _T("%f"), CalData[i]	);	_stprintf(buf, _T("Data_%d"), i);
		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), CompCal_data.Top_data_X[i].GetPos	);	_stprintf(buf, _T("Pos_%d"), i);
		WritePrivateProfileString(ReadModel, buf, value, szParamFile);

		_stprintf(value, _T("%f"), CompCal_data.Top_data_X[i].Cal_um	);	_stprintf(buf, _T("Calc_%d"), i);
		WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}
}

//===================================================================================

void CfrmCompView::ClickZTestStart5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GetDlgItem(IDB_Z_TEST_START5)->EnableWindow(FALSE);
	GetDlgItem(IDB_Z_TEST_START6)->EnableWindow(TRUE);

	for(int i= 0; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		CompCal_data.Top_data_X[i].GetPos= 0.0;
		CompCal_data.Top_data_X[i].Cal_um= 0.0;
	}

	CalCompData();
	GridInitC1_1();	GridDispC1_1();
	AfxMessageBox(_T("Start..."));
	CompProc_Start();
}


void CfrmCompView::ClickZTestStart6()
{
	GetDlgItem(IDB_Z_TEST_START5)->EnableWindow(TRUE);
	GetDlgItem(IDB_Z_TEST_START6)->EnableWindow(FALSE);

	CompProc_End();
}

//===================================================================================
void CfrmCompView::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	bTopCompEnable= m_bTopCompEnable;
	bBtmCompEnable= m_bBtmCompEnable;
}


void CfrmCompView::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	bTopCompEnable= m_bTopCompEnable;
	bBtmCompEnable= m_bBtmCompEnable;
}


void CfrmCompView::ClickMarkSave2()
{
	CString strText;
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
	//-------------------------------------------------------------	
	if (MyMessageBox(_T("설정된 위치 정보를 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE2] => CfrmCompView::Data_Save.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE2] => CfrmCompView::Data_Save.!"));
	//-------------------------------------------------------------	

	DataSave2() ;

}


void CfrmCompView::ClickZTestStart()
{
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;
	CString strText;
	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) 
	{	
		SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
		MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
	}

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE)) 
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
		MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
}


void CfrmCompView::ClickZTestStart2()
{
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;
	CString strText;
	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) 
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
		MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; 
	}

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_X_AXIS_COMP_TOP_START);
	ToPosition1= ToPosition1 + (ParamFile()->System_data.dCompPitch * ParamFile()->System_data.iCompCountX);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE))
	{	
		SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
		MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
	}
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
}


void CfrmCompView::ClickCmMove11()
{
	pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->System_data.dCompPitch, _SPD_MODE);		
}


void CfrmCompView::ClickCmMove17()
{
	pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->System_data.dCompPitch, _SPD_MODE);		
}


void CfrmCompView::ClickCmMove2()
{
	CString strText;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

}

void CfrmCompView::DownLoad_X_Data()
{
	double x_pos[_MAX_COMP_CAL_DATA_COUNT]; 
	double x_cal[_MAX_COMP_CAL_DATA_COUNT]; 

	x_pos[0]= 0.0;
	x_cal[0]= 0.0;
	double bal_pos= 0.;
	for(int i= 1; i< _MAX_COMP_CAL_DATA_COUNT; i++)
	{
		if(CompCal_data.Top_data_X[i-1].GetPos > 0)
		{
			x_pos[i]= CompCal_data.Top_data_X[i-1].GetPos;
			x_cal[i]= CompCal_data.Top_data_X[i-1].Cal_um;
			bal_pos= x_pos[i];
		}
		else
		{
			x_pos[i]= bal_pos;
			x_cal[i]= 0.0;
		}
	}
	pSystem.GetMotionComponent()->DownLoadMatData_X(x_pos, x_cal, ParamFile()->System_data.iCompCountX+1);

}

void CfrmCompView::ClickOpuRollZ30()
{
	CString strText;
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE)) 
	{	
		MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR);
		if(X_Enable_Flag)
		{
			m_lblRunDisable_X.SetValue(0); m_lblRunEnable_X.SetValue(1); X_Enable_Flag= TRUE;
		}
		else
		{
			m_lblRunDisable_X.SetValue(1); m_lblRunEnable_X.SetValue(0); X_Enable_Flag= FALSE;
		}
		return; 
	}

	m_lblRunDisable_X.SetValue(1); m_lblRunEnable_X.SetValue(0); X_Enable_Flag= FALSE;
	pSystem.GetMotionComponent()->ACS_SYS.BufferRun(17);

	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
}


void CfrmCompView::ClickOpuRollZ32()
{
	CString strText;
	SetCursor(LoadCursor(NULL, IDC_WAIT)) ;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	SetCursor(LoadCursor(NULL, IDC_ARROW)) ; strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= 0.0;
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	TRUE))
	{	
		MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); 
		if(X_Enable_Flag)
		{
			m_lblRunDisable_X.SetValue(0); m_lblRunEnable_X.SetValue(1); X_Enable_Flag= TRUE;
		}
		else
		{
			m_lblRunDisable_X.SetValue(1); m_lblRunEnable_X.SetValue(0); X_Enable_Flag= FALSE;
		}
	return; 
	}

	m_lblRunDisable_X.SetValue(0); m_lblRunEnable_X.SetValue(1); X_Enable_Flag= TRUE;
	DownLoad_X_Data();
	pSystem.GetMotionComponent()->ACS_SYS.BufferRun(16);
	SetCursor(LoadCursor(NULL, IDC_ARROW)) ;
}
