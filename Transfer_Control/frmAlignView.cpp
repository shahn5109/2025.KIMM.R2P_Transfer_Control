// frmAlignView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "frmAlignView.h"
#include "MainFrm.h"
#include "dlgManuAlign.h"
#include "dlgManuCenter.h"
#include "dlgMarkCopy.h"


// CfrmAlignView
extern CSystemClass pSystem;
CParameterFile* CfrmAlignView::ParamFile()	{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CfrmAlignView::Log()			{	return (CParameterFile*)(pSystem.GetParameterFileComponent());	}

IMPLEMENT_DYNCREATE(CfrmAlignView, CFormView)

CfrmAlignView::CfrmAlignView()
	: CFormView(CfrmAlignView::IDD)
	, m_dScoreCertanty(0)
	, m_dScoreAcceptance(0)
	, m_TimeOut(0)
	, m_dOffsetX(0)
	, m_dOffsetY(0)
	, m_nBrushSize_L(0)
	, m_DispZoom(1.0)
	, m_dAlignMarkToMark(0)
	, m_iMarkMaxCount(0)
	, m_dAlignAccuracy_umXY(0)
	, m_dAlignAccuracy_umT(0)
	, m_dAlignOffset_X(0)
	, m_dAlignOffset_Y(0)
	, m_dAlignOffset_T(0)
	, m_dOffsetTest_X(0)
	, m_dOffsetTest_Y(0)
	, m_dAlignMarkToMarkY(0)
	, m_bMaskEdit_L(FALSE)
	, m_iLedcontrolVal(0)
	, m_iMovePix_Sel(0)
	, m_iMarkSearchCount(0)
	, m_iRollAlignModeFlag(0)
{
	mAlignProcStart= FALSE;
	mAlignProc_Step= 0;
	for(int i= 0 ; i< 3; i++)	{
		m_bExec_1[i] = FALSE; m_pThread_1[i] = NULL;	
	}
	proc_run_flag= false;
	m_ROI_Mode= false;
	m_nBrushSize_L = 0;
	bAlign_End_Flag= FALSE;
	m_bAreaROI= FALSE;
}

CfrmAlignView::~CfrmAlignView()
{
}

void CfrmAlignView::DoDataExchange(CDataExchange* pDX)
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

	DDX_Control(pDX, IDB_JOG_SELECT_TOPCAM, m_btnTopCam);
	DDX_Control(pDX, IDB_JOG_SELECT_BOTCAM, m_btnBotCam);
	DDX_Control(pDX, IDB_JOG_SELECT_ALIGN, m_btnAlignStage1Mark);
	DDX_Control(pDX, IDB_JOG_SELECT_MASK, m_btnMaskMark);
	DDX_Control(pDX, IDB_JOG_SELECT_MASK2, m_btnPlateStampMark);
	DDX_Control(pDX, IDB_JOG_SELECT_ALIGN2, m_btnAlignStage2Mark);
	DDX_Control(pDX, IDB_JOG_SELECT_ALIGN3, m_btnAlignRollMark);

	DDX_Control(pDX, IDB_JOG_SELECT_TUSED, m_btnManuStamp_TopUsed);
	DDX_Control(pDX, IDB_JOG_SELECT_BUSED, m_btnManuStamp_BtmUsed);

	DDX_Control(pDX, IDB_JOG_SELECT_2X, m_btnSel2X);
	DDX_Control(pDX, IDB_JOG_SELECT_20X, m_btnSel20X);
	DDX_Control(pDX, IDB_JOG_SELECT_1ST, m_btnSel_1st);
	DDX_Control(pDX, IDB_JOG_SELECT_2ND, m_btnSel2nd);
	DDX_Control(pDX, IDB_MARK_TEST5, m_btnManuRollAlign);

	DDX_Control(pDX, IDB_MARK_ROI_SEL5, m_btnAreaROI);

	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS, m_dAlignMarkToMark);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS2, m_iMarkMaxCount);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS3, m_dAlignAccuracy_umXY);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS4, m_dAlignAccuracy_umT);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS7, m_dAlignOffset_X);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS5, m_dAlignOffset_Y);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS6, m_dAlignOffset_T);
	DDX_Text(pDX, IDC_EDIT2, m_dOffsetTest_X);
	DDX_Text(pDX, IDC_EDIT8, m_dOffsetTest_Y);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS8, m_dAlignMarkToMarkY);
	DDX_Check(pDX, IDC_CHECK_MASK_SET, m_bMaskEdit_L);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT_ALL_AXIS9, m_iLedcontrolVal);
	DDX_Radio(pDX, IDC_RADIO21, m_iMovePix_Sel);
	DDX_Radio(pDX, IDC_RADIO25, m_iMarkSearchCount);
	DDX_Radio(pDX, IDC_RADIO_R_ALL, m_iRollAlignModeFlag);
}

BEGIN_MESSAGE_MAP(CfrmAlignView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MSG_DATA_UPDATE, &CfrmAlignView::DataReload)
	ON_MESSAGE(WM_MSG_PAT_ROI_UPDATE, &CfrmAlignView::SetRoi_Confirm)
	ON_MESSAGE(WM_MSG_LED_UPDATE, &CfrmAlignView::SetLedUpdate)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO1, &CfrmAlignView::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CfrmAlignView::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON2, &CfrmAlignView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CfrmAlignView::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK_MASK_SET, &CfrmAlignView::OnBnClickedCheckMaskSet)
	ON_BN_CLICKED(IDC_MASK_EDIT_APPLY, &CfrmAlignView::OnBnClickedMaskEditApply)
	ON_BN_CLICKED(IDC_MASK_EDIT_CLEAR, &CfrmAlignView::OnBnClickedMaskEditClear)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BRUSH_SLIDER, &CfrmAlignView::OnNMCustomdrawBrushSlider)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON3, &CfrmAlignView::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CfrmAlignView::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON4, &CfrmAlignView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CfrmAlignView::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CfrmAlignView::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_U, &CfrmAlignView::OnBnClickedButtonU)
	ON_BN_CLICKED(IDC_BUTTON_D, &CfrmAlignView::OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_L2, &CfrmAlignView::OnBnClickedButtonL2)
	ON_BN_CLICKED(IDC_BUTTON_R2, &CfrmAlignView::OnBnClickedButtonR2)
	ON_BN_CLICKED(IDC_BUTTON9, &CfrmAlignView::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CfrmAlignView::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CfrmAlignView::OnBnClickedButton11)
END_MESSAGE_MAP()

// CfrmAlignView 진단입니다.

#ifdef _DEBUG
void CfrmAlignView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmAlignView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CfrmAlignView 메시지 처리기입니다.

//=================================================================================== 
void CfrmAlignView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->SetWindowHandler_AlignView(this);

	m_pVision= pSystem.GetVisionComponent();

	LensNo= 0;
	UsedNo= 0;
	CamNo= 0;
	MarkNo= 0;
	ModelGroupNo= ePLATE_1; Stage_Proc= STAGE_1_ALIGN_PROC;

	ModelNo= mdTOP_2X_STAEG1_START; 
	m_radioCameraIdx=eTOP_CAM_2X;

	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);

	m_ctrlLedSlider.SetRange(0, 255) ;

	m_ctrlBrushSlider_L.SetRange(1, 10) ;
	m_ctrlBrushSlider_L.SetPos(3) ;

	m_btnTopCam.SetValue(1);	m_btnBotCam.SetValue(0);
	m_btnAlignStage1Mark.SetValue(1);	m_btnMaskMark.SetValue(0);
	m_btnSel2X.SetValue(1);		m_btnSel20X.SetValue(0);
	m_btnSel_1st.SetValue(1);	m_btnSel2nd.SetValue(0);

	
	m_pVision->InitModelSet_1(m_ModelEditDisplay_L.GetSafeHwnd());

	if(!LED_cnt.OpenPort (_COM3, B19200)) 
	{
		AfxMessageBox(_T("LED Controller 초기화 ERROR!!!\nCOM-Port Open-Error."));
		ComFlag1= false;
	}

	int i;
	for(i= 0; i< _MAX_FIND_MODEL; i++)		m_pVision->PatternLoad(i);

	DataInit();
	
//	m_pVision->PatternImageLoadNdisp(ModelNo);

	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(FALSE);
	m_manuStamp_Camera= eBTM_CAM;
	m_btnManuStamp_TopUsed.SetValue(0); m_btnManuStamp_BtmUsed.SetValue(1);
	g_bCfrmAlignViewInit= TRUE;

	m_ModelEditDisplay_L.GetWindowRect(m_dispRect_L);
	ScreenToClient(m_dispRect_L);
	disp_rect_L= m_dispRect_L;

	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(FALSE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(FALSE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(FALSE);
}

//===================================================================================
int CfrmAlignView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

//===================================================================================
void CfrmAlignView::OnDestroy()
{
	CFormView::OnDestroy();

	LED_cnt.ClosePort ();		

	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject(); m_WhiteBrush.DeleteObject();
}

void CfrmAlignView::ReCOM_Conn()
{
	LED_cnt.ClosePort ();		Sleep(1000);

	if(!LED_cnt.OpenPort (_COM3, B19200)) 
	{
		AfxMessageBox(_T("LED Controller 초기화 ERROR!!!\nCOM-Port Open-Error."));
		ComFlag1= false;
	}

}
//===================================================================================
void CfrmAlignView::OnSize(UINT nType, int cx, int cy)
{
//	CFormView::OnSize(nType, cx, cy);
}

//===================================================================================
HBRUSH CfrmAlignView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

//===================================================================================
void CfrmAlignView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnTimer(nIDEvent);
}
//===================================================================================
int CfrmAlignView::GetModelNo(int no1, int no2, int no3, int no4, int no5 ) //CamNo, LensNo, UsedNo, MarkNo, StageNo
{
	int CalNo= 0;
	if(no1== eTOP_CAM)
	{
		if(no2== e2X_LENS)
		{
			if(no5== ePLATE_1)
			{
				if(no3== eMASK_MARK)
				{
					if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_2X_DEF_START;
					else								CalNo= mdTOP_2X_DEF_END;
				}
				else
				{
					if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_2X_STAEG1_START;
					else								CalNo= mdTOP_2X_STAEG1_END;
				}
			}
			else if(no5== ePLATE_2)
			{
				if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_2X_STAEG2_START;
				else								CalNo= mdTOP_2X_STAEG2_END;
			}
			else if(no5== ePLATE_3)
			{
				if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_2X_STAEG3_START;
				else								CalNo= mdTOP_2X_STAEG3_END;
			}
			else if(no5== ePHOTOMASK)				CalNo= mdTOP_2X_MASK;
			else
			{
				AfxMessageBox(_T("Error!!!")); return 0;
				CalNo= mdTOP_2X_STAEG1_START;
			}
			m_radioCameraIdx= eTOP_CAM_2X;
		}
		else
		{
			if(no5== ePLATE_1)
			{
				if(no3== eMASK_MARK)
				{
					if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_20X_DEF_START;
					else								CalNo= mdTOP_20X_DEF_END;
				}
				else
				{
					if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_20X_STAEG1_START;
					else								CalNo= mdTOP_20X_STAEG1_END;
				}
			}
			else if(no5== ePLATE_2)
			{
				if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_20X_STAEG2_START;
				else								CalNo= mdTOP_20X_STAEG2_END;
			}
			else if(no5== ePLATE_3)
			{
				if(no4== eALIGN_1ST_MARK)			CalNo= mdTOP_20X_STAEG3_START;
				else								CalNo= mdTOP_20X_STAEG3_END;
			}
			else if(no5== ePHOTOMASK)				CalNo= mdTOP_20X_MASK;
			else
			{
				AfxMessageBox(_T("Error!!!")); return 0;
				CalNo= mdTOP_2X_STAEG1_START;
			}
			m_radioCameraIdx= eTOP_CAM_20X;
		}
	}
	else
	{
		if(no2== e2X_LENS)
		{
			if(no5== eROLL)
			{
				if(no4== eALIGN_1ST_MARK)			CalNo= mdBTM_2X_ROLL_START;
				else								CalNo= mdBTM_2X_ROLL_END;
			}
			else if(no5== ePHOTOMASK)				CalNo= mdBTM_2X_MASK;
			else
			{
				AfxMessageBox(_T("Error!!!")); return 0;
				CalNo= mdBTM_2X_ROLL_START;
			}
			m_radioCameraIdx= eBTM_CAM_2X;
		}
		else
		{
			if(no5== eROLL)
			{
				if(no4== eALIGN_1ST_MARK)			CalNo= mdBTM_20X_ROLL_START;
				else								CalNo= mdBTM_20X_ROLL_END;
			}
			else if(no5== ePHOTOMASK)				CalNo= mdBTM_20X_MASK;
			else
			{
				AfxMessageBox(_T("Error!!!")); return 0;
				CalNo= mdBTM_20X_ROLL_START;
			}
			m_radioCameraIdx= eBTM_CAM_20X;
		}
	}
	CString strText, strText1;
	switch(CalNo)
	{
	case mdTOP_20X_STAEG1_START:	strText.Format(_T("상부_10X_STAEG1_START")); break;
	case mdTOP_20X_STAEG1_END:		strText.Format(_T("상부_10X_STAEG1_END")); break;
	case mdTOP_2X_STAEG1_START:	strText.Format(_T("상부_2X_STAEG1_START")); break;
	case mdTOP_2X_STAEG1_END:		strText.Format(_T("상부_2X_STAEG1_END")); break;
	case mdTOP_20X_STAEG2_START:	strText.Format(_T("상부_10X_STAEG2_START")); break;
	case mdTOP_20X_STAEG2_END:		strText.Format(_T("상부_10X_STAEG2_END")); break;
	case mdTOP_2X_STAEG2_START:	strText.Format(_T("상부_2X_STAEG2_START")); break;
	case mdTOP_2X_STAEG2_END:		strText.Format(_T("상부_2X_STAEG2_END")); break;
	case mdTOP_20X_STAEG3_START:	strText.Format(_T("상부_10X_STAEG3_START")); break;
	case mdTOP_20X_STAEG3_END:		strText.Format(_T("상부_10X_STAEG3_END")); break;
	case mdTOP_2X_STAEG3_START:	strText.Format(_T("상부_2X_STAEG3_START")); break;
	case mdTOP_2X_STAEG3_END:		strText.Format(_T("상부_2X_STAEG3_END")); break;
	case mdBTM_20X_ROLL_START:	strText.Format(_T("하부_ROLL_10X_START")); break;
	case mdBTM_20X_ROLL_END:		strText.Format(_T("하부_ROLL_10X_END")); break;
	case mdBTM_2X_ROLL_START:	strText.Format(_T("하부_ROLL_2X_START")); break;
	case mdBTM_2X_ROLL_END:		strText.Format(_T("하부_ROLL_2X_END")); break;
	case mdTOP_20X_MASK:	strText.Format(_T("상부_10X_MASK")); break;
	case mdTOP_2X_MASK:		strText.Format(_T("상부_2X_MASK")); break;
	case mdBTM_20X_MASK:	strText.Format(_T("하부_10X_MASK")); break;
	case mdBTM_2X_MASK:		strText.Format(_T("하부_2X_MASK")); break;
	case mdTOP_20X_DEF_START:	strText.Format(_T("상부_10X_평스탬프용 MASK")); break;
	case mdTOP_20X_DEF_END:		strText.Format(_T("상부_10X_평스탬프용 MASK")); break;
	case mdTOP_2X_DEF_START:	strText.Format(_T("상부_2X_평스탬프용 MASK")); break;
	case mdTOP_2X_DEF_END:		strText.Format(_T("상부_2X_평스탬프용 MASK")); break;
	default:				strText.Format(_T("???????")); break;
	}
	switch(m_radioCameraIdx)
	{
	case eTOP_CAM_2X:		strText1.Format(_T("상부 카메라_2X")); break;
	case eTOP_CAM_20X:		strText1.Format(_T("상부 카메라_10X")); break;
	case eBTM_CAM_2X:		strText1.Format(_T("하부 카메라_2X")); break;
	case eBTM_CAM_20X:		strText1.Format(_T("하부 카메라_10X")); break;
	default:				strText1.Format(_T("???????")); break;
	}
	strText= strText1 + _T(" : ") + strText;
	SetDlgItemText(IDC_SEL_MODEL_NAME, strText.GetBuffer(0));

	return CalNo;
}
//===================================================================================
LRESULT CfrmAlignView::DataReload(WPARAM wParam,LPARAM lParam)
{
	int i;
	for(i= 0; i< _MAX_FIND_MODEL; i++)		m_pVision->PatternLoad(i);
	DataInit();
	return 0;
}
//===================================================================================
void CfrmAlignView::DataInit()
{
	ResetEditCtrl();

	ParamFile()->Model_data.SearchROI_left[eBTM_CAM_20X]= 0;
	ParamFile()->Model_data.SearchROI_right[eBTM_CAM_20X]= _INSP_IMAGE_SIZE_X;
	ParamFile()->Model_data.SearchROI_top[eBTM_CAM_20X]= 0;
	ParamFile()->Model_data.SearchROI_bottom[eBTM_CAM_20X]= _INSP_IMAGE_SIZE_Y;
	
	ParamFile()->Model_data.SearchROI_left[eTOP_CAM_20X]= 0;
	ParamFile()->Model_data.SearchROI_right[eTOP_CAM_20X]= _INSP_IMAGE_SIZE_X;
	ParamFile()->Model_data.SearchROI_top[eTOP_CAM_20X]= 0;
	ParamFile()->Model_data.SearchROI_bottom[eTOP_CAM_20X]= _INSP_IMAGE_SIZE_Y;

	int ret= m_pVision->PatternLoad(ModelNo);

// 	ParamFile()->OpenSytemConfigFile();
// 	ParamFile()->OpenModelConfigFile(ParamFile()->System_data.strCurrentModelName);

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

	m_iMarkSearchCount=		ParamFile()->Model_data.iMarkSearchCount[Stage_Proc];
	m_iRollAlignModeFlag=		ParamFile()->Model_data.iRollAlignModeFlag[Stage_Proc];

	m_dAlignMarkToMark=		ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc];
	m_dAlignMarkToMarkY=	ParamFile()->Model_data.dAlignMarkToMarkY[Stage_Proc];
	m_iMarkMaxCount=		ParamFile()->Model_data.iMarkMaxCount;
	m_dAlignAccuracy_umXY=	ParamFile()->Model_data.dAlignAccuracy_umXY;
	m_dAlignAccuracy_umT=	ParamFile()->Model_data.dAlignAccuracy_umT;
	m_dAlignOffset_X=		ParamFile()->Model_data.dAlignOffset_X[Stage_Proc];
	m_dAlignOffset_Y=		ParamFile()->Model_data.dAlignOffset_Y[Stage_Proc];
	m_dAlignOffset_T=		ParamFile()->Model_data.dAlignOffset_T[Stage_Proc];

	if(CamNo== eTOP_CAM)	{	m_btnTopCam.SetValue(1); m_btnBotCam.SetValue(0); }
	else					{	m_btnTopCam.SetValue(0); m_btnBotCam.SetValue(1); }
	if(LensNo== e2X_LENS)	{	m_btnSel2X.SetValue(1); m_btnSel20X.SetValue(0); }
	else					{	m_btnSel2X.SetValue(0); m_btnSel20X.SetValue(1); }
	if(MarkNo== eALIGN_1ST_MARK)	{	m_btnSel_1st.SetValue(1); m_btnSel2nd.SetValue(0); }
	else							{	m_btnSel_1st.SetValue(0); m_btnSel2nd.SetValue(1); }

	if(ModelGroupNo== ePLATE_1)			
	{
		if(UsedNo== eMASK_MARK)
		{
			m_btnAlignStage1Mark.SetValue(0); m_btnAlignStage2Mark.SetValue(0); m_btnAlignRollMark.SetValue(0); m_btnMaskMark.SetValue(0); m_btnPlateStampMark.SetValue(1); 
		}
		else
		{
			m_btnAlignStage1Mark.SetValue(1); m_btnAlignStage2Mark.SetValue(0); m_btnAlignRollMark.SetValue(0); m_btnMaskMark.SetValue(0); m_btnPlateStampMark.SetValue(0); 
		}
	}
	else if(ModelGroupNo== ePLATE_2)	{	m_btnAlignStage1Mark.SetValue(0); m_btnAlignStage2Mark.SetValue(1); m_btnAlignRollMark.SetValue(0); m_btnMaskMark.SetValue(0); m_btnPlateStampMark.SetValue(0); }
	else if(ModelGroupNo== eROLL)		{	m_btnAlignStage1Mark.SetValue(0); m_btnAlignStage2Mark.SetValue(0); m_btnAlignRollMark.SetValue(1); m_btnMaskMark.SetValue(0); m_btnPlateStampMark.SetValue(0); }
	else if(ModelGroupNo== ePHOTOMASK)	{	m_btnAlignStage1Mark.SetValue(0); m_btnAlignStage2Mark.SetValue(0); m_btnAlignRollMark.SetValue(0); m_btnMaskMark.SetValue(1); m_btnPlateStampMark.SetValue(0); }
	else								{	m_btnAlignStage1Mark.SetValue(0); m_btnAlignStage2Mark.SetValue(0); m_btnAlignRollMark.SetValue(0); m_btnMaskMark.SetValue(0); m_btnPlateStampMark.SetValue(0); }


	m_pVision->PatternImageLoadNdisp(ModelNo);
	
	SetLedVal();

	Invalidate();
/*

	if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
	{
		GetDlgItem(IDB_MARK_TEST5)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_TUSED)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BUSED)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDB_MARK_TEST5)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_SELECT_TUSED)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_SELECT_BUSED)->EnableWindow(FALSE);
	}
*/

	UpdateData(FALSE);
}
//===================================================================================
void CfrmAlignView::DataSave()
{
	UpdateData(TRUE);

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


	ParamFile()->Model_data.dAlignAccuracy_umXY=		m_dAlignAccuracy_umXY;
	ParamFile()->Model_data.dAlignAccuracy_umT=			m_dAlignAccuracy_umT;

	ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]= m_iMarkSearchCount;
	ParamFile()->Model_data.iRollAlignModeFlag[Stage_Proc]= m_iRollAlignModeFlag;
	
	ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc]=	m_dAlignMarkToMark;
	ParamFile()->Model_data.dAlignMarkToMarkY[Stage_Proc]=	m_dAlignMarkToMarkY;

	ParamFile()->Model_data.dAlignOffset_X[Stage_Proc]=	m_dAlignOffset_X;
	ParamFile()->Model_data.dAlignOffset_Y[Stage_Proc]=	m_dAlignOffset_Y;
	ParamFile()->Model_data.dAlignOffset_T[Stage_Proc]=	m_dAlignOffset_T;

	ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC]=		m_dAlignMarkToMark;	ParamFile()->Model_data.dAlignMarkToMarkY[ROLL_ALIGN_PROC]=	m_dAlignMarkToMarkY;
	ParamFile()->Model_data.dAlignMarkToMark[STAGE_1_ALIGN_PROC]=	m_dAlignMarkToMark;	ParamFile()->Model_data.dAlignMarkToMarkY[STAGE_1_ALIGN_PROC]=	m_dAlignMarkToMarkY;
	ParamFile()->Model_data.dAlignMarkToMark[STAGE_2_ALIGN_PROC]=	m_dAlignMarkToMark;	ParamFile()->Model_data.dAlignMarkToMarkY[STAGE_2_ALIGN_PROC]=	m_dAlignMarkToMarkY;
	ParamFile()->Model_data.dAlignMarkToMark[STAGE_3_ALIGN_PROC]=	m_dAlignMarkToMark;	ParamFile()->Model_data.dAlignMarkToMarkY[STAGE_3_ALIGN_PROC]=	m_dAlignMarkToMarkY;
	ParamFile()->Model_data.dAlignMarkToMark[SYNC_PROC]=			m_dAlignMarkToMark;	ParamFile()->Model_data.dAlignMarkToMarkY[SYNC_PROC]=	m_dAlignMarkToMarkY;
	ParamFile()->Model_data.dAlignMarkToMark[MANUAL_STAMP_PROC]=	m_dAlignMarkToMark;	ParamFile()->Model_data.dAlignMarkToMarkY[MANUAL_STAMP_PROC]=	m_dAlignMarkToMarkY;
	ParamFile()->Model_data.dStamp_Len_X= m_dAlignMarkToMark;
	ParamFile()->Model_data.dStamp_Len_Y= m_dAlignMarkToMarkY;

	ParamFile()->Model_data.dAlignMarkToMark[MANUAL_ROLL_ALIGN_PROC] = ParamFile()->Model_data.dAlignMarkToMark[ROLL_ALIGN_PROC];
	ParamFile()->Model_data.dAlignMarkToMarkY[MANUAL_ROLL_ALIGN_PROC]= ParamFile()->Model_data.dAlignMarkToMarkY[ROLL_ALIGN_PROC];

	//-------------------------------------------------------------	
	ParamFile()->SaveFile(ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadFile();
	//-------------------------------------------------------------	
	m_pVision->PatternSave(ModelNo);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();

	DataInit();
}
//===================================================================================
void CfrmAlignView::Pattern_Model_Load(int model_no)
{
	int ret= m_pVision->PatternLoad(ModelNo);
	if(ret== 0) return;

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

	m_pVision->PatternImageLoadNdisp(ModelNo);
	Invalidate();

	UpdateData(FALSE);
}
//===================================================================================
BEGIN_EVENTSINK_MAP(CfrmAlignView, CFormView)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_TOPCAM, DISPID_CLICK, CfrmAlignView::ClickJogSelectTopcam, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_BOTCAM, DISPID_CLICK, CfrmAlignView::ClickJogSelectBotcam, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_ALIGN, DISPID_CLICK, CfrmAlignView::ClickJogSelectAlign, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_MASK, DISPID_CLICK, CfrmAlignView::ClickJogSelectMask, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_2X, DISPID_CLICK, CfrmAlignView::ClickJogSelect2x, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_20X, DISPID_CLICK, CfrmAlignView::ClickJogSelect20x, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_1ST, DISPID_CLICK, CfrmAlignView::ClickJogSelect1st, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_2ND, DISPID_CLICK, CfrmAlignView::ClickJogSelect2nd, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_ROI_SEL, DISPID_CLICK, CfrmAlignView::ClickMarkRoiSel, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_SAVE, DISPID_CLICK, CfrmAlignView::ClickMarkSave, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_TEST, DISPID_CLICK, CfrmAlignView::ClickMarkTest, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_CM_MOVE, DISPID_CLICK, CfrmAlignView::ClickCmMove, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_TEST2, DISPID_CLICK, CfrmAlignView::ClickMarkTest2, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_ALIGN2, DISPID_CLICK, CfrmAlignView::ClickJogSelectAlign2, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_ALIGN3, DISPID_CLICK, CfrmAlignView::ClickJogSelectAlign3, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_TUSED, DISPID_CLICK, CfrmAlignView::ClickJogSelectTused, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_BUSED, DISPID_CLICK, CfrmAlignView::ClickJogSelectBused, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_TEST3, DISPID_CLICK, CfrmAlignView::ClickMarkTest3, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_CM_MOVE6, DISPID_CLICK, CfrmAlignView::ClickCmMove6, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_CM_MARK_LOAD, DISPID_CLICK, CfrmAlignView::ClickCmMarkLoad, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_MASK2, DISPID_CLICK, CfrmAlignView::ClickJogSelectMask2, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_JOG_SELECT_MASK3, DISPID_CLICK, CfrmAlignView::ClickJogSelectMask3, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_TEST5, DISPID_CLICK, CfrmAlignView::ClickMarkTest5, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_ROI_SEL2, DISPID_CLICK, CfrmAlignView::ClickMarkRoiSel2, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_ROI_SEL3, DISPID_CLICK, CfrmAlignView::ClickMarkRoiSel3, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_ROI_SEL4, DISPID_CLICK, CfrmAlignView::ClickMarkRoiSel4, VTS_NONE)
	ON_EVENT(CfrmAlignView, IDB_MARK_ROI_SEL5, DISPID_CLICK, CfrmAlignView::ClickMarkRoiSel5, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

void CfrmAlignView::ClickJogSelectTopcam()
{
	CamNo= eTOP_CAM;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelectBotcam()
{
	CamNo= eBTM_CAM;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);

}
//===================================================================================
void CfrmAlignView::ClickJogSelectAlign()
{
	CamNo= eTOP_CAM;	UsedNo= eALIGN_MARK;	ModelGroupNo= ePLATE_1; Stage_Proc= STAGE_1_ALIGN_PROC;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS2)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS4)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS6)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS7)->EnableWindow(TRUE);

	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(FALSE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(FALSE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(FALSE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelectAlign2()
{
	CamNo= eTOP_CAM;	UsedNo= eALIGN_MARK;	ModelGroupNo= ePLATE_2; Stage_Proc= STAGE_2_ALIGN_PROC;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS2)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS4)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS6)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS7)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(FALSE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(FALSE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(FALSE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelectAlign3()
{
	CamNo= eBTM_CAM;	UsedNo= eALIGN_MARK;	ModelGroupNo= eROLL; Stage_Proc= ROLL_ALIGN_PROC;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(FALSE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS2)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS4)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS6)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS7)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(TRUE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(TRUE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(TRUE);
}

//===================================================================================
void CfrmAlignView::ClickJogSelectMask()
{
	CamNo= eTOP_CAM;	UsedNo= eMASK_MARK;	ModelGroupNo= ePHOTOMASK; Stage_Proc= ROLL_ALIGN_PROC;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS8)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS2)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS6)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS7)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(FALSE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(FALSE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(FALSE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelect2x()
{
	LensNo= e2X_LENS;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelect20x()
{
	LensNo= e20X_LENS;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelect1st()
{
	MarkNo= eALIGN_1ST_MARK;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelect2nd()
{
	MarkNo= eALIGN_2ND_MARK;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	//Invalidate(TRUE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelectMask2()
{
	CamNo= eTOP_CAM;	UsedNo= eMASK_MARK;	ModelGroupNo= ePLATE_1; Stage_Proc= STAGE_1_ALIGN_PROC;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS8)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS2)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS4)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS6)->EnableWindow(FALSE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS7)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(FALSE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(FALSE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(FALSE);
}
//===================================================================================
void CfrmAlignView::ClickJogSelectMask3()
{
	CamNo= eTOP_CAM;	UsedNo= eALIGN_MARK;	ModelGroupNo= ePLATE_3; Stage_Proc= STAGE_3_ALIGN_PROC;
	ModelNo= GetModelNo(CamNo, LensNo, UsedNo, MarkNo, ModelGroupNo);
	DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	GetDlgItem(IDB_JOG_SELECT_TOPCAM)->EnableWindow(TRUE);	GetDlgItem(IDB_JOG_SELECT_BOTCAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS2)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS4)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS6)->EnableWindow(TRUE);	GetDlgItem(IDC_EDIT_TIMEOUT_ALL_AXIS7)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_R_ALL)->EnableWindow(FALSE);			GetDlgItem(IDC_RADIO_R_L)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_R_C)->EnableWindow(FALSE);				GetDlgItem(IDC_RADIO_R_R)->EnableWindow(FALSE);
}

//===================================================================================
void CfrmAlignView::ClickMarkRoiSel() 
{
	ResetEditCtrl();
	UpdateData( TRUE );
	m_bAreaROI= FALSE;
	m_btnAreaROI.SetValue(FALSE);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(m_btnROI_Set.GetValue())
	{
		m_ROI_Mode= true;
		if(CamNo== eTOP_CAM)
		{
			if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
			else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
		}
		else
		{
			if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
			else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
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
void CfrmAlignView::ClickMarkSave()
{
	CString strText;
	//-------------------------------------------------------------	
	if (MyMessageBox(_T("설정된 마크를 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgModelSet_1::Data_Save.! => CANCEL"));
		ResetEditCtrl();
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmAlignView::Data_Save.!"));
	//-------------------------------------------------------------	
	DataSave() ;
	ResetEditCtrl();
}
//===================================================================================
void CfrmAlignView::ClickMarkTest()
{
	ResetEditCtrl();
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	//
	CRect setROI;
	double tor_x_Val= 0.0, tor_y_Val= 0.0;
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	setROI.left= 0;	setROI.top= 0;	setROI.right= _INSP_IMAGE_SIZE_X;	setROI.bottom= _INSP_IMAGE_SIZE_Y;
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.0 && ParamFile()->Model_data.dLED_Pitch_Y> 0.0)
	{
		if(CamNo== eTOP_CAM) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		setROI.left= 0;
		setROI.top= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val);
		setROI.right= _INSP_IMAGE_SIZE_X;
		setROI.bottom= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val);
		find_ROI_Set_Flag= _NORMAL_SELECT;
	}
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
	{
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	}
	else	
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);

//	pFrm->GetWindowHandler_CameraView()->DispCross(m_radioCameraIdx, 300.0, 100.0, true);
}
//===================================================================================
LRESULT CfrmAlignView::SetRoi_Confirm(WPARAM wParam,LPARAM lParam)
{
	if(	m_bAreaROI)
	{
		m_bAreaROI= FALSE;
		m_btnAreaROI.SetValue(0);
		CRect getRect= pSystem.GetVisionComponent()->GetROI();


		if(CamNo== eTOP_CAM)
		{
			ParamFile()->Model_data.SearchROI_left[eTOP_CAM_2X]= getRect.left;
			ParamFile()->Model_data.SearchROI_right[eTOP_CAM_2X]= getRect.right;
			ParamFile()->Model_data.SearchROI_top[eTOP_CAM_2X]= getRect.top;
			ParamFile()->Model_data.SearchROI_bottom[eTOP_CAM_2X]= getRect.bottom;
		}
		else
		{
			ParamFile()->Model_data.SearchROI_left[eBTM_CAM_2X]= getRect.left;
			ParamFile()->Model_data.SearchROI_right[eBTM_CAM_2X]= getRect.right;
			ParamFile()->Model_data.SearchROI_top[eBTM_CAM_2X]= getRect.top;
			ParamFile()->Model_data.SearchROI_bottom[eBTM_CAM_2X]= getRect.bottom;
		}

	}
	else
	{
		m_ROI_Mode= false;
		m_btnROI_Set.SetValue(0);

		CImageObj* pImg = m_pVision->GetImageObj( m_radioCameraIdx );

		//m_pVision->RegistModelFinderImageForMask( m_radioCameraIdx, pImg );

		m_pVision->PatternReDefine( m_radioCameraIdx, pImg, ModelNo);	
		//	m_pVision->PatternSave(ModelNo);	
		DataSave();
		m_pVision->PatternLoad(ModelNo);

		DataInit(); //m_pVision->PatternImageLoadNdisp(ModelNo);
	}
	//Invalidate(TRUE);
	return 0;
}


//===================================================================================
void CfrmAlignView::OnBnClickedRadio1()
{
	m_DispZoom= 1.0;
	m_pVision->PatDispZoom(m_DispZoom);
	Invalidate(TRUE);
}

//===================================================================================
void CfrmAlignView::OnBnClickedRadio2()
{
	m_DispZoom= 2.0;
	m_pVision->PatDispZoom(m_DispZoom);
	Invalidate(TRUE);
}

//===================================================================================
void CfrmAlignView::ClickCmMove()
{
	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN) return;
	switch(ModelNo)
	{
	case mdTOP_20X_STAEG1_START:		MovePos(POSITION_STAGE1_ALIGN, TRUE);		break;
	case mdTOP_20X_STAEG1_END:			MovePos(POSITION_STAGE1_ALIGN_END, TRUE);	break;
	case mdTOP_2X_STAEG1_START:			MovePos(POSITION_STAGE1_ALIGN, FALSE);		break;
	case mdTOP_2X_STAEG1_END:			MovePos(POSITION_STAGE1_ALIGN_END, FALSE);	break;
	case mdTOP_20X_STAEG2_START:		MovePos(POSITION_STAGE2_ALIGN, TRUE);		break;
	case mdTOP_20X_STAEG2_END :			MovePos(POSITION_STAGE2_ALIGN_END, TRUE);	break;
	case mdTOP_2X_STAEG2_START:			MovePos(POSITION_STAGE2_ALIGN, FALSE);		break;
	case mdTOP_2X_STAEG2_END:			MovePos(POSITION_STAGE2_ALIGN_END, FALSE);	break;
	case mdBTM_20X_ROLL_START:			MovePos(POSITION_ROLL_ALIGN, TRUE);			break;
	case mdBTM_20X_ROLL_END:			MovePos(POSITION_ROLL_ALIGN_END, TRUE);		break;
	case mdBTM_2X_ROLL_START:			MovePos(POSITION_ROLL_ALIGN, FALSE);		break;
	case mdBTM_2X_ROLL_END:				MovePos(POSITION_ROLL_ALIGN_END, FALSE);	break;
	case mdTOP_20X_MASK:				MovePos(POSITION_TOP_MASK_20X, TRUE);		break;
	case mdTOP_2X_MASK:					MovePos(POSITION_TOP_MASK, FALSE);			break;
	case mdBTM_20X_MASK:				MovePos(POSITION_BTM_MASK_20X, TRUE);		break;
	case mdBTM_2X_MASK:					MovePos(POSITION_BTM_MASK, FALSE);			break;
	default:
		MyMessageBox(_T("모델 선택 에러."), _T("Position-Move Failed"), M_ICONERROR); return ;
		break;
	}
}
//===================================================================================
BOOL CfrmAlignView::MoveEndCheckNonDisp(int Axis) 
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
BOOL CfrmAlignView::MoveEndCheck(int Axis, BOOL dispflag) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	if(dispflag)
	{
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
	}
	else
	{
		return MoveEndCheckNonDisp(Axis);
	}
	return FALSE;
}

//===================================================================================
BOOL CfrmAlignView::MovePos(int pos, BOOL x20_Flag, BOOL dispFlag, BOOL AT_Flag) 
{
	CString strText;
	CString strPos;
	double ToPosition1= 0.0, ToPosition2= 0.0;
	int dispPos_Sel;
	dispPos_Sel= pos;

	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	if(!pSystem.GetMotionComponent()->isMotionReadyCheck()) 
	{
		MyMessageBox(_T("이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
	}
	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	if(dispFlag)
	{
		switch(dispPos_Sel) 
		{
		case POSITION_STAGE1_ALIGN:
		case POSITION_STAGE2_ALIGN:
		case POSITION_STAGE3_ALIGN:
			strPos.Format(_T("SX= %.3f:PB= %.3f:Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f]"),	
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel));
			break;
		case POSITION_ROLL_ALIGN:
			strPos.Format(_T("SX= %.3f:PB= %.3f:Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f]"),	
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel));
			break;
		case POSITION_ROLL_ALIGN_END:
			strPos.Format(_T("SX= %.3f:PB= %.3f:Z=%.3f[%.3f]\nBX=%.3f:BY=%.3f:BZ=%.3f]"),	
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_ROLL_ALIGN),												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN)+ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc],												
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_ROLL_ALIGN),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_ROLL_ALIGN),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN),
				pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_ROLL_ALIGN));
			break;
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
		case POSITION_STAGE1_ALIGN_END:
		case POSITION_STAGE2_ALIGN_END:
			break;
		default:
			MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
			break;
		}
		//-----------------
		strText.Format(_T("선택된 Mark 위치로 이동 하시겠습니까?"));
		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return FALSE;
		//-----------------
	}
	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//-------------
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_READY);  if(ToPosition1> 0.0) ToPosition1= 0.0;
	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	TRUE)) 
	{	MyMessageBox(_T("[BOT_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//-------------

//	if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1, dispFlag)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2, dispFlag)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	if(!pSystem.GetMotionComponent()->AlignStage_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[ALIGN_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//-----------------

	if(!pSystem.GetMotionComponent()->Safe_PosCheck(_ACS_AXIS_STAGE_X, dispPos_Sel)) 
	{
		// 		strText.Format("Z축이 대기 위치보다 아래로 내려와 있습니다.\n선택된 축들을 설정 위치로 이동 하시겠습니까? \n%s", strPos);
		// 		if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONERROR|M_YESNO, _T("예"), _T("아니오")) == IDNO)
		// 		{
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
		return FALSE;
		//		}
	}

	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	double add_pos_rx= 0.0, add_pos_x= 0.0, add_pos_y= 0.0, add_pos_z= 0.0;
	double add_pos_btm_rx= 0.0, add_pos_btm_x= 0.0, add_pos_btm_y= 0.0, add_pos_btm_z= 0.0;
	if(x20_Flag)
	{
		add_pos_x= ParamFile()->System_data.dTopCam20To2_X;
		add_pos_y= ParamFile()->System_data.dTopCam20To2_Y;
		add_pos_z= ParamFile()->System_data.dTopCam20To2_Z;

		add_pos_btm_x= ParamFile()->System_data.dBotCam20To2_X;
		add_pos_btm_y= ParamFile()->System_data.dBotCam20To2_Y;
		add_pos_btm_z= ParamFile()->System_data.dBotCam20To2_Z;
	}
// 	if((dispPos_Sel== POSITION_ROLL_ALIGN || dispPos_Sel== POSITION_ROLL_ALIGN_END) && pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X > 20))
// 	{
// 		MyMessageBox(_T("X축 위치 에러!!!\n모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
// 		return FALSE;
// 	}

	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	switch(dispPos_Sel) 
	{
	case POSITION_STAGE1_ALIGN_END:
		add_pos_x= add_pos_x + ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc];
	case POSITION_STAGE1_ALIGN:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN) + add_pos_x;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_STAGE1_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_STAGE1_ALIGN) + add_pos_z;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE1_ALIGN) + add_pos_y;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//-----------------
		break;
	case POSITION_STAGE2_ALIGN_END:
		add_pos_x= add_pos_x + ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc];
	case POSITION_STAGE2_ALIGN:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE2_ALIGN)  + add_pos_x;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_STAGE2_ALIGN)  + add_pos_z;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE2_ALIGN) + add_pos_y;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_STAGE2_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//-----------------
		break;
	case POSITION_STAGE3_ALIGN_END:
		add_pos_x= add_pos_x + ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc];
	case POSITION_STAGE3_ALIGN:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE3_ALIGN)  + add_pos_x;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_STAGE3_ALIGN)  + add_pos_z;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_STAGE3_ALIGN) + add_pos_y;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_STAGE3_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//-----------------
		break;
	case POSITION_ROLL_ALIGN_END:
		add_pos_x= ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc];
		add_pos_rx= pSystem.GetMotionComponent()->Roll_GetmmToDeg(ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc]);
	case POSITION_ROLL_ALIGN:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_ROLL_ALIGN);
		if(ToPosition1 > 20.0)
		{
			strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]\nStage-X 위치가 부적절 합니다."), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; 
		}
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		if(ToPosition1 > 10.0)
		{
			strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]\n하부 Stage-X 위치가 부적절 합니다."), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; 
		}
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN) + add_pos_rx;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1+add_pos_btm_x,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1+add_pos_btm_y,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_ROLL_ALIGN);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_ROLL_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_ROLL_ALIGN);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1/*+add_pos_btm_z*/,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//-----------------
		break;
	case POSITION_TOP_MASK:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		break;
	case POSITION_TOP_MASK_20X:

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_X;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_Z;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, POSITION_TOP_MASK)+ParamFile()->System_data.dTopCam20To2_Y;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		break;
	case POSITION_BTM_MASK:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
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
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, dispPos_Sel);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		break;
	case POSITION_BTM_MASK_20X:
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_X;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, POSITION_READY);
		ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------------------
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_STAGE_X, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_READY);
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_Y;
		if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		//-----------------
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_BTM_MASK) + ParamFile()->System_data.dBotCam20To2_Z;
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		break;
	default:
		MyMessageBox(_T("축 번호 이상...\n이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return  FALSE;
		break;
	}
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z1), _ACS_AXIS_IMP_ROLL_Z1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL_Z2), _ACS_AXIS_IMP_ROLL_Z2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z, dispFlag)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL, dispFlag)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(AT_Flag) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	
	return TRUE;
}

//============================================================================================
void CfrmAlignView::RunThread_1(int nID)
{
	m_bExec_1[nID] = TRUE; 
	tagTREADPARAMS_ALIGN *pThreadParams = new tagTREADPARAMS_ALIGN;
	pThreadParams->pContinue = &m_bExec_1[nID];
	//	pThreadParams->pWnd =	((CMainFrame*)(AfxGetApp()->GetMainWnd()));
	pThreadParams->pWnd =  ((CfrmDebugView*)this);
	if(m_pThread_1[nID] == NULL) {
		switch(nID) {
		case 0: 
			m_pThread_1[nID] = AfxBeginThread(Thread_Align_Proc_1, pThreadParams);  break;
		case 1: 
			break;
		case 2: 
			break;
		}
	}
	m_pThread_1[nID]->m_bAutoDelete = true;
}
void CfrmAlignView::KillThread_1(int nID)
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
UINT Thread_Align_Proc_1(LPVOID pParam)
//-----------------------------------------------------------------------------------	
{
	tagTREADPARAMS_ALIGN *pThreadParams = (tagTREADPARAMS_ALIGN *)pParam;
	CWnd *pWnd = pThreadParams->pWnd;//THREADPAPAMS_Mag1;
	BOOL *pContinue = pThreadParams->pContinue;
	delete pThreadParams;
	
	CfrmAlignView* pOwner =  (CfrmAlignView*)pWnd ;
	CString strText;
	bool ret= false;
	double angle= 0.0, posx= 0.0, posy= 0.0;
	DWORD st= 0, st1= 0;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	while(*pContinue)
	{
		if(PROGRAM_EXIT) { Sleep(10); continue; }
		if(!pOwner->mAlignProcStart)  { Sleep(10); continue; }
/*
		switch(pOwner->mAlignProc_Step)
		{
		case 0: continue;
		case _ALIGN_SAFE_POS_MOVE:
			break;
		case _ALIGN_SAFE_POS_MOVE_END:
			break;
//ROLL
		case _ALIGN_ROLL_MODE_CHECK:
			break;

		case _ALIGN_ROLL_START_POS_MOVE:
			break;
		case _ALIGN_ROLL_START_POS_MOVE_END:
			break;
		}
*/
		Sleep(1);
	}
	return 0;
}
//===================================================================================
void CfrmAlignView::AlignProc_Start()
{
	mAlignProcStart= TRUE;
	mAlignProc_Step= 1;
}
bool CfrmAlignView::isAlignProc_End()
{
	if(mAlignProc_Step== 999)
		return true;
	return false;
}
void CfrmAlignView::AlignProc_End()
{
	mAlignProcStart= FALSE;
	mAlignProc_Step= 0;
}
//===================================================================================
void CfrmAlignView::ClickMarkTest2()
{
	//AlignProc_Start();
	if(proc_run_flag) return;
	if(ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_RUN) return;

	proc_run_flag= true;
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CString strText;

	strText.Format(_T("Align-Test를 진행 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("Align"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
		proc_run_flag= false;
		return;
	}

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	m_pVision->OverlayClear(m_radioCameraIdx);
	//------------------------------------------------------------------
/*	double cam_Xmm, cam_Ymm;
	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	m_pVision->GetCam_ROICood(LensNo, posx, posy, &cam_Xmm, &cam_Ymm);

	double real_X= 0.0, real_Y= 0.0;
	m_pVision->Trans_RealCoord(LensNo, cam_Xmm, cam_Ymm, &real_X, &real_Y);
	CString strText;
	strText.Format(_T("Align : x=%.3f, y=%.3f => Y=%.3f, Y=%.3f\n"), posx, posy, real_X, real_Y);
	TRACE(strText.GetBuffer(0));
*/
	g_AutoRunAlignFlag= TRUE;
	double Cur_X= 0.0, Cur_Y= 0.0, Cur_Z= 0.0;
	double tarPosX= 0.0, tarPosY= 0.0;
	if(Align_Proc()) //20배로 얼라인 완료, 위치는 20배 위치
	{
		Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-ParamFile()->System_data.dTopCam20To2_X;
		Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-ParamFile()->System_data.dTopCam20To2_Y;
		Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
/*
		ToPosition1= / *Cur_X -* / -ParamFile()->System_data.dTopCam20To2_X;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		ToPosition1= / *Cur_Y -* / -ParamFile()->System_data.dTopCam20To2_Y;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		ToPosition1= / *Cur_Z -* / -ParamFile()->System_data.dTopCam20To2_Z;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
*/
//		pSystem.GetMotionComponent()->GetPickPlace_Pos(Cur_X , Cur_Y, &tarPosX,  &tarPosY );
		AfxMessageBox(_T("Aling-Test  완료......."));
		bAlign_End_Flag= TRUE; 
	}
	else
	{
		bAlign_End_Flag= FALSE;
		AfxMessageBox(_T("Aling-Test  실패!......."));
	}

	proc_run_flag= false;
//	RollAlignProc();
}

//===================================================================================
void CfrmAlignView::OnBnClickedButton2()
{
/*
	CString strText;
	UpdateData(TRUE);
//	SetTimer(dlgManuAlign_TIMER_ID, / *dlgManuAlign_TIMER_INTERVAL* /2000, NULL);
	double UpData[_MAX_AXIS];
	int i;
	int stage_no= STAGE_1_ALIGN_PROC;

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


	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	double Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-ParamFile()->System_data.dTopCam20To2_X;
	double Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-ParamFile()->System_data.dTopCam20To2_Y;
	double Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Z);
/ *
		ToPosition1= / *Cur_X -* / -ParamFile()->System_data.dTopCam20To2_X;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		ToPosition1= / *Cur_Y -* / -ParamFile()->System_data.dTopCam20To2_Y;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
		ToPosition1= / *Cur_Z -* / -ParamFile()->System_data.dTopCam20To2_Z;
		if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
* /
	double tarPosX= 0.0, tarPosY= 0.0;
// 	ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-ParamFile()->System_data.dTopCam20To2_Y;
// 	ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-ParamFile()->System_data.dTopCam20To2_X;
// 	ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
// 	ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);

	pSystem.GetMotionComponent()->GetPickPlace_Pos(stage_no, ParamFile()->Align_Result_data[stage_no].dPosData[_ACS_AXIS_STAGE_X], 
		ParamFile()->Align_Result_data[stage_no].dPosData[_ACS_AXIS_UPPER_CAM_Y], 
		&tarPosX,  &tarPosY );

	double ang_moveX= 0.0, ang_moveY= 0.0;

	pSystem.GetVisionComponent()->GetRotationCoord(m_dOffsetTest_X, m_dOffsetTest_Y, ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dTheta, &ang_moveX, &ang_moveY);

	double CurY1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
	double CurY2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);


	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, tarPosX+ang_moveX,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToPosition1	= ParamFile()->Align_Result_data[stage_no].dPosData[_ACS_AXIS_ALIGN_Y1] + tarPosY + ang_moveY;
	ToPosition2	= ParamFile()->Align_Result_data[stage_no].dPosData[_ACS_AXIS_ALIGN_Y2] + tarPosY + ang_moveY;
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

/ *
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, tarPosX+ang_moveX,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	ToPosition1	= ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]+ang_moveY;
	ToPosition2	= ParamFile()->Align_Result_data[STAGE_1_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]+ang_moveY;
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
* /


*/

}
//===================================================================================
BOOL CfrmAlignView::Align_Proc(bool dispflag)
{
	CString strTTTTT;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	m_pVision->OverlayClear(eBTM_CAM_2X);	
	m_pVision->InspGrabContinue();
	m_pVision->OverlayClear(eBTM_CAM_20X);		m_pVision->OverlayClear(eTOP_CAM_2X);		m_pVision->OverlayClear(eTOP_CAM_20X);

	if(dispflag) 
	{
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
		{
			CString strText;
			if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
			{
				if(dispflag) 
				{
					strText.Format(_T("Roll-Align을 진행 하시겠습니까?"));
					if (MyMessageBox(strText.GetBuffer(0), _T("Align"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDYES)	
					{
						if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
						{	
							if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
							{
								pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, eROLL);
								if(RollAlignProc()== FALSE)
								{
									pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
									pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);

									Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Roll #1 [ALIGN_AUTO_MODE]"));
									ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);
									m_pVision->InspGrabContinue();
									pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
									return FALSE;
								}
								pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
							}
						}//if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
					}
				}
				else
				{
					if(ParamFile()->Model_data.iRun_RollMode[ROLL_ALIGN_PROC]== RUN_ROLL_MODE)
					{
						pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, eROLL);
						if(RollAlignProc()== FALSE)
						{
							pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
							pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);

							Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Roll #1 [ALIGN_AUTO_MODE]"));
							ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);
							m_pVision->InspGrabContinue();
							pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
							return FALSE;
						}
						pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
					}
				}
			}
			else if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_MANU_MODE)
			{
				if(dispflag) MyMessageBox(_T("Roll 수동 Align-모드 입니다.\nRoll 수동 얼라인 정보 기준으로 동작됩니다."), _T("Align-Mode"), M_ICONINFORMATION);
			}
			else
			{
				if(dispflag) MyMessageBox(_T("Align-모드 비활성화 상태입니다."), _T("Align-Mode"), M_ICONINFORMATION);
			}

			if(dispflag)  { if(!g_AutoRunAlignFlag) return FALSE; }
			pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
			pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);
		}
		else
		{
			//기준점 화면으로 셋팅하면서 주석처리
			ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Xpix= _INSP_IMAGE_SIZE_X/2.0;

			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_20X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
			pSystem.GetParameterFileComponent()->Model_data.dStamp_Btm_2X_1st_Ypix= _INSP_IMAGE_SIZE_Y/2.0;
		}
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}




	m_pVision->InspGrabContinue();


	//Stage #1   ------------------------------------------------
	if(dispflag)  { if(!g_AutoRunAlignFlag) return FALSE; }
	if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC]== RUN_ENABLE)
	{
		ParamFile()->AlignDataClear(STAGE_1_ALIGN_PROC);
		if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_1);
			if(PlateAlignProc(STAGE_1_ALIGN_PROC)== FALSE)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Stage #1 [ALIGN_AUTO_MODE]"));
				m_pVision->InspGrabContinue();
				return FALSE;
			}
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_1_ALIGN_PROC]== ALIGN_MANU_MODE)
		{	
			pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_1);
			CdlgManuAlign dlg;
			dlg.m_StageNo= STAGE_1_ALIGN_PROC; dlg.iStamInstRunFlag= 0;
			dlg.CameraStageUsedNo= eTOP_CAM;
			if(dlg.DoModal()== IDCANCEL)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Stage #1 [ALIGN_AUTO_MODE]"));
			}
			if(!g_AutoRunAlignFlag) return FALSE;
			return FALSE;
		}
	}
	//Stage #2   ------------------------------------------------
	if(dispflag)  { if(!g_AutoRunAlignFlag) return FALSE; }
	if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC]== RUN_ENABLE)
	{
		ParamFile()->AlignDataClear(STAGE_2_ALIGN_PROC);
		if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_2);
			if(PlateAlignProc(STAGE_2_ALIGN_PROC)== FALSE)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Stage #2 [ALIGN_AUTO_MODE]"));
				m_pVision->InspGrabContinue();
				return FALSE;
			}
			if(!g_AutoRunAlignFlag) return FALSE;
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_2_ALIGN_PROC]== ALIGN_MANU_MODE)
		{	
			pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_2);
			CdlgManuAlign dlg;
			dlg.m_StageNo= STAGE_2_ALIGN_PROC; dlg.iStamInstRunFlag= 0;
			dlg.CameraStageUsedNo= eTOP_CAM;
			if(dlg.DoModal()== IDCANCEL)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Stage #2 [ALIGN_MANU_MODE]"));
				return FALSE;
			}
			if(!g_AutoRunAlignFlag) return FALSE;
		}
	}
	//Stage #3   ------------------------------------------------
	if(dispflag)  { if(!g_AutoRunAlignFlag) return FALSE; }
	if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE)
	{
		ParamFile()->AlignDataClear(STAGE_3_ALIGN_PROC);
		if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{	
			pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_3);
			if(PlateAlignProc(STAGE_3_ALIGN_PROC)== FALSE)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Stage #3 [ALIGN_AUTO_MODE]"));
				m_pVision->InspGrabContinue();
				return FALSE;
			}
			if(!g_AutoRunAlignFlag) return FALSE;
		}
		else if(ParamFile()->Model_data.iAlignMode[STAGE_3_ALIGN_PROC]== ALIGN_MANU_MODE)
		{	
			pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_3);
			CdlgManuAlign dlg;
			dlg.m_StageNo= STAGE_3_ALIGN_PROC; dlg.iStamInstRunFlag= 0;
			dlg.CameraStageUsedNo= eTOP_CAM;
			if(dlg.DoModal()== IDCANCEL)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN-FAIL ] Stage #3 [ALIGN_MANU_MODE]"));
				return FALSE;
			}
			if(!g_AutoRunAlignFlag) return FALSE;
		}
	}
	//---------------------------------------------
	if(dispflag)  { if(!g_AutoRunAlignFlag) return FALSE; }
	m_pVision->InspGrabContinue();
	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, POSITION_ROLL_ALIGN);
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE);

	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);

	return TRUE;
}

void CfrmAlignView::RollAlignDataClear()
{
	for(int i= 0; i< _MAX_ROLL_ALING_DATA; i++)
	{
		RollAlign_Pos[i]= RollAlign_X_Pix[i]= RollAlign_Y_Pix[i]= RollAlign_Angle[i]= 0.0;
	}
}

//===================================================================================
BOOL CfrmAlignView::RollAlignProc(BOOL AtMode)
{
	bool	ret= false;
	double cam1_Xmm, cam1_Ymm;
	double cam2_Xmm, cam2_Ymm;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	int i= 0, RollAlignCount= 0;

	CString strFile__Name, strDir__Name, strSaveFileNameText;
	CString strData;
	int cnt= 0;

	SYSTEMTIME st; 
	GetLocalTime(&st);
	_wmkdir(_T("d:\\RollAlign_ImageSave"));
	strDir__Name.Format(_T("d:\\RollAlign_ImageSave\\%04d%02d%02d_%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond); //strDirMeasImageSavePath);
	_wmkdir(strDir__Name);
	strFile__Name.Format(_T("d:\\RollAlign_ImageSave\\%04d%02d%02d_%02d%02d%02d\\Report_.txt"),	st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond); //strDirMeasImageSavePath);

	ParamFile()->AlignDataClear(ROLL_ALIGN_PROC);

	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, eROLL);

	CRect setROI;
	double tor_x_Val= 0.0, tor_y_Val= 0.0;
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	setROI.left= 0;	setROI.top= 0;	setROI.right= _INSP_IMAGE_SIZE_X;	setROI.bottom= _INSP_IMAGE_SIZE_Y;
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.0 && ParamFile()->Model_data.dLED_Pitch_Y> 0.0)
	{
		if(CamNo== eTOP_CAM) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		setROI.left= 0;
		setROI.top= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val);
		setROI.right= _INSP_IMAGE_SIZE_X;
		setROI.bottom= (LONG)((_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val);
		find_ROI_Set_Flag= _NORMAL_SELECT;
	}

	if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
	if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC])
	{
		RollAlignDataClear();
		move_pos_no= POSITION_ROLL_ALIGN;
		if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
		{
			if(!g_AutoRunAlignFlag) return FALSE;
			move_pos_no= POSITION_ROLL_ALIGN;
			if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
			Log()->SaveLogFile(_SYSTEM_LOG, _T("MovePos(move_pos_no, FALSE, FALSE)"));
			if(MovePos(move_pos_no, FALSE, FALSE, AtMode)== FALSE)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_ROLL_ALIGN]"));
				pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
				return FALSE;
			}

			for(int a_cnt= 0; a_cnt< 3; a_cnt++)
			{
				m_pVision->InspGrabContinueStop();
				Sleep(ParamFile()->System_data.iImageGrab_Delay);
				m_pVision->OverlayClear(eBTM_CAM_2X);
				m_pVision->InspGrab(eBTM_CAM_2X);
				find_model_no= mdBTM_2X_ROLL_START;

				if(ParamFile()->Model_data.iMarkSearchCount[ROLL_ALIGN_PROC]) //Stage_Proc= STAGE_1_ALIGN_PROC)
				{
					ret= m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eBTM_CAM_2X, true, true, true);
				}
				else	
					ret= m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eBTM_CAM_2X, true, true, true);

				if(	ret == false)
				{
					m_pVision->InspGrabContinue();
					Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] Roll #1 [MARK-Search-Error!] [S-1]"));
					pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
					return FALSE;
				}
				double cha_x= posx1 - (_INSP_IMAGE_SIZE_X/2.0);
				double cha_y= posy1 - (_INSP_IMAGE_SIZE_Y/2.0);
				strSaveFileNameText.Format(_T("%s\\%02d.tif"), strDir__Name, RollAlignCount);	pSystem.GetVisionComponent()->Save_Image(eBTM_CAM_2X, strSaveFileNameText);
				m_pVision->InspGrabContinue();

				if(fabs(cha_x) > 3.0)
				{
					double movemm= cha_x*(ParamFile()->System_data.dBot_Cam_PixelToum_2X/1000.0);
					BOOL rrr= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -movemm, _SPD_MODE);
					MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL);
				}
				else
				{
					break;
				}
				Sleep(200);
			}
			//-------- Multi-Point Align
// 			if(ParamFile()->Model_data.iRoll_AngleAlignMode== RUN_ENABLE)
// 			{
// 			}
			RollAlign_Pos[RollAlignCount]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
			RollAlign_X_Pix[RollAlignCount]= posx1;		RollAlign_Y_Pix[RollAlignCount]= posy1;		RollAlign_Angle[RollAlignCount]= 0.0;
			RollAlignCount++;
			//--------
			//중심 위치로 티칭값 변경
			double curpos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= curpos;
			curpos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
			ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= curpos;
			ParamFile()->Model_data.dStamp_Btm_2X_1st_Xpix= posx1;
			ParamFile()->Model_data.dStamp_Btm_2X_1st_Ypix= posy1;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_2X_1st_Xpix;
			ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix= posy1;
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

			m_pVision->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
			ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;
			strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, cam1_Xmm, cam1_Ymm);
			TRACE(strText.GetBuffer(0));
			pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));

			if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0)
			{
				double chaX, chaY, mmX1, mmY1, mmX2, mmY2;
				mmX1= RollAlign_X_Pix[0] * (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0);
				mmY1= RollAlign_Y_Pix[0] * (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0);
				//-------- Multi-Point Align 
				if(ParamFile()->Model_data.iRoll_AngleAlignMode== RUN_ENABLE && ParamFile()->Model_data.iRoll_AngleAlign_Count > 1 && ParamFile()->Model_data.dRoll_AngleAlign_Pitch > 0.0)
				{
					//ParamFile()->Model_data.dRoll_AngleAlign_Pitch, ParamFile()->Model_data.iRoll_AngleAlign_Count
					double get_angle= 0.0, get_posx= 0.0, get_posy= 0.0;
					for(cnt= 1; cnt< ParamFile()->Model_data.iRoll_AngleAlign_Count; cnt++)
					{
//AfxMessageBox(_T("."));
						if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
						pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dRoll_AngleAlign_Pitch, _SPD_MODE, TRUE);		
						if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

						m_pVision->InspGrabContinueStop();
						Sleep(ParamFile()->System_data.iImageGrab_Delay); Sleep(200);
						m_pVision->InspGrab(eBTM_CAM_2X);
						//pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

						pSystem.GetVisionComponent()->OverlayClear(eBTM_CAM_2X);
				//-------------------------------------------
				//		pSystem.GetVisionComponent()->PatternFinderPos(&get_angle, &get_posx, &get_posy, mdBTM_2X_ROLL_START, eBTM_CAM_2X, true, true, true);
				//Roll-Align시 전체중에서 Center부분만 찾게
						if(ParamFile()->Model_data.iMarkSearchCount[ROLL_ALIGN_PROC]) //Stage_Proc= STAGE_1_ALIGN_PROC)
						{
							m_pVision->PatternFinderPosAll(_CENTER_SELECT, &get_angle, &get_posx, &get_posy, mdBTM_2X_ROLL_START, eBTM_CAM_2X, true, true, true);
						}
						else	
							m_pVision->PatternFinderPos(&get_angle, &get_posx, &get_posy, mdBTM_2X_ROLL_START, eBTM_CAM_2X, true, true, true);
	
						strSaveFileNameText.Format(_T("%s\\%02d.tif"), strDir__Name, RollAlignCount);	pSystem.GetVisionComponent()->Save_Image(eBTM_CAM_2X, strSaveFileNameText);
						RollAlign_Pos[RollAlignCount]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
						RollAlign_X_Pix[RollAlignCount]= get_posx;		RollAlign_Y_Pix[RollAlignCount]= get_posy;		
						RollAlign_Angle[RollAlignCount]= 0.0;
						double va2l= 0.0, val= 0.0;
						if(get_posx > 0.0 && get_posy > 0.0)
						{
							double x_cha= 0.0;
							mmX2= RollAlign_X_Pix[RollAlignCount] * (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0);
							mmY2= RollAlign_Y_Pix[RollAlignCount] * (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0);
							chaX= mmX1- mmX2;				chaY=  mmY2-mmY1; //chaY= mmY1- mmY2;

							val= atan((chaY)/((ParamFile()->Model_data.dRoll_AngleAlign_Pitch * RollAlignCount)/*+chaX*/));
							va2l= pSystem.GetVisionComponent()->radianToDegree(val);
						}
						RollAlign_Angle[RollAlignCount]= -va2l;
						RollAlignCount++;
					}
					//--------------------------------------------------------------------------------------------------------------
					if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
					pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, RollAlign_Pos[0], TRUE);
					Sleep(200);
					if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					Sleep(500);
				}
				//--------



				//2nd
				move_pos_no= POSITION_ROLL_ALIGN_END;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dStamp_Len_X, _SPD_MODE, TRUE)== FALSE)
				{
					Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_ROLL_ALIGN_END]"));
					pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
					return FALSE;
				}
				Sleep(200);
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				Sleep(1000);
				Sleep(ParamFile()->System_data.iImageGrab_Delay); 
				m_pVision->InspGrabContinueStop();
				Sleep(200);
				m_pVision->InspGrab(eBTM_CAM_2X);
				find_model_no= mdBTM_2X_ROLL_END;

				if(ParamFile()->Model_data.iMarkSearchCount[ROLL_ALIGN_PROC]) //Stage_Proc= STAGE_1_ALIGN_PROC)
				{
					ret= m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx2, &posy2, find_model_no, eBTM_CAM_2X, true, true, true);
				}
				else	
					ret= m_pVision->PatternFinderPos(&angle, &posx2, &posy2, find_model_no, eBTM_CAM_2X, true, true, true);

				if(	ret  == false)
				{
					m_pVision->InspGrabContinue();
					Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] Stage #1 [MARK-Search-Error!] [E-1]"));
					pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
					return FALSE;
				}
				strSaveFileNameText.Format(_T("%s\\%02d.tif"), strDir__Name, RollAlignCount);	pSystem.GetVisionComponent()->Save_Image(eBTM_CAM_2X, strSaveFileNameText);
				m_pVision->InspGrabContinue();

				ParamFile()->Model_data.dStamp_Btm_2X_2nd_Xpix= posx2/* + (ParamFile()->System_data.Q_Offset_2X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0)) */;
 				ParamFile()->Model_data.dStamp_Btm_2X_2nd_Ypix= posy2;
				ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix= _INSP_IMAGE_SIZE_X-ParamFile()->Model_data.dStamp_Btm_2X_2nd_Xpix;
				ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix= posy2;

				if(!g_AutoRunAlignFlag) return FALSE;
				m_pVision->GetCam_ROICood(eBTM_CAM, e2X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= cam2_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= cam2_Ymm;

				double cal_x= fabs(ParamFile()->Model_data.dStamp_Len_X - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x + ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
				double cal_y= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= -m_pVision->GetAngle(
					0, 0, cal_x, cal_y);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
				strText.Format(_T("Align-E : x=%.3f, y=%.3f => X=%.3f, Y=%.3f : Len=%.3f, Angle=%.3f\n"), posx2, posy2, cam2_Xmm, cam2_Ymm,
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance, ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta);
				TRACE(strText.GetBuffer(0));
				pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));


double x_len= ((ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix - ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix)*ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0) +
		ParamFile()->Model_data.dStamp_Len_X;
double y_len= ((ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix - ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix)*ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0);
double Rollangle_2X= -pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, x_len, y_len);
/*
x_len= ((ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix - ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix)*ParamFile()->System_data.dTop_Cam_PixelToum_20X/1000.0) +
ParamFile()->Model_data.dStamp_Len_X;
y_len= ((ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix - ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix)*ParamFile()->System_data.dTop_Cam_PixelToum_20X/1000.0);
Rollangle_20X= -pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, x_len, y_len);
*/


				//-------- Multi-Point Align 
					RollAlign_Pos[RollAlignCount]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
					RollAlign_X_Pix[RollAlignCount]= posx2;			RollAlign_Y_Pix[RollAlignCount]= posy2;
					RollAlign_Angle[RollAlignCount]= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta;
					RollAlignCount++;
				//--------

//------------------------------------------------------------------------------------------------------------
				//-------
				double sum_angle= 0.0; int data_cnt= 0;
				FILE* fout = _wfopen(strFile__Name.GetBuffer(0), _T("a+"));   // 파일에 계속 추가시킨다. //
				if(fout != NULL) 	
				{
					strData.Format(_T("==================================================================="));		fwprintf(fout, _T("%s\n"), strData);
					strData.Format(_T("===== [Mode= %d] [간격= %f] [Count= %d] => [%d] ====="), 
						ParamFile()->Model_data.iRoll_AngleAlignMode, ParamFile()->Model_data.dRoll_AngleAlign_Pitch, ParamFile()->Model_data.iRoll_AngleAlign_Count, RollAlignCount) ;
					fwprintf(fout, _T("%s\n"), strData);
					strData.Format(_T("No.\tPos\tX\tY\tAngle"));													fwprintf(fout, _T("%s\n"), strData);

					for(cnt= 0; cnt< RollAlignCount; cnt++)
					{
						strData.Format(_T("%d\t%f\t%f\t%f\t%f"), cnt, RollAlign_Pos[cnt], RollAlign_X_Pix[cnt], RollAlign_Y_Pix[cnt], RollAlign_Angle[cnt]);
						fwprintf(fout, _T("%s\n"), strData);
					}
					sum_angle= 0.0; data_cnt= 0;
					for(int scnt= 0; scnt< ParamFile()->Model_data.iRoll_AngleAlign_Count; scnt++)	
					{
						if(RollAlign_Angle[scnt] != 0.0) {		sum_angle= sum_angle + RollAlign_Angle[scnt]; data_cnt++; }
					}
					sum_angle= sum_angle / (data_cnt);
					strData.Format(_T("Result = %f, Avg = %f"), ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta, sum_angle);		fwprintf(fout, _T("%s\n"), strData);

					strData.Format(_T("==================================================================="));		fwprintf(fout, _T("%s\n"), strData);
					fclose(fout);
				}

				strText.Format(_T(" ===> Roll-Angle= %.3f,     Avg= %.3f\n"), ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta, sum_angle);
				if(ParamFile()->Model_data.iRoll_AngleAlignMode== RUN_ENABLE && ParamFile()->Model_data.iRoll_AngleAlign_Count > 1 && ParamFile()->Model_data.dRoll_AngleAlign_Pitch > 0.0)
				{
					ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= sum_angle;
				}
				TRACE(strText.GetBuffer(0));
				pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));
				//-------
//------------------------------------------------------------------------------------------------------------
				double ang_moveX= 0.0, ang_moveY= 0.0;
				double ToPosition1= 0.0;

				if(!g_AutoRunAlignFlag) return FALSE;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dStamp_Len_X, _SPD_MODE, TRUE)== FALSE)
				{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Roll #1 [MovePos:mdBTM_2X_ROLL_START]"));	return FALSE;			}
				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			}//if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0)
			else
			{
		//20배 Align-Start
				//20배 위치로 이동
				double Cur_X, Cur_Y, Cur_Z, ToPosition1;
				Cur_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
				Cur_Y= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
				Cur_Z= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
				double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(Cur_X > 5.0)
				{
					if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }


					if(!pSystem.GetMotionComponent()->Z_Up_PosCheck()) 
					{
						pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
						MyMessageBox(_T("모터 이동이 실패했습니다.[Z_UP]"), _T("Position-Move Failed"), M_ICONERROR); return FALSE;
					}
				}

				ToPosition1= ParamFile()->System_data.dBotCam20To2_X;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				ToPosition1= ParamFile()->System_data.dBotCam20To2_Y;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				ToPosition1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_ROLL_ALIGN) + ParamFile()->System_data.dBotCam20To2_Z;

				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, ToPosition1, 	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

				pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_ROLL_ALIGN);
				ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_ROLL_ALIGN);
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

				double AF_Z_Pos= 0.0;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(ParamFile()->System_data.bRollAF_Enable && pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eBTM_CAM_20X, &AF_Z_Pos))
				{
					;
					//ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Z]= AF_Z_Pos - ParamFile()->System_data.dBotCam20To2_Z;
				}

//				return FALSE;
				for(int a_cnt= 0; a_cnt< 3; a_cnt++)
				{

					m_pVision->InspGrabContinueStop();
					Sleep(ParamFile()->System_data.iImageGrab_Delay);
					m_pVision->InspGrab(eBTM_CAM_20X);
					find_model_no= mdBTM_20X_ROLL_START;
					if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eBTM_CAM_20X, true, true, true) == false)
					{
						m_pVision->InspGrabContinue();
						Log()->SaveLogFile(_SYSTEM_LOG, _T("[ 10X-FAIL ] Roll #1 [MARK-Search-Error!] [S-1]"));
						return FALSE;
					}
					double cha_x= posx1 - (_INSP_IMAGE_SIZE_X/2.0);
					double cha_y= posy1 - (_INSP_IMAGE_SIZE_Y/2.0);
					m_pVision->InspGrabContinue();
					if(fabs(cha_x) > 2.0)
					{
						double movemm= cha_x*(ParamFile()->System_data.dBot_Cam_PixelToum_20X/1000.0);
						BOOL rrr= pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -movemm, _SPD_MODE);
						MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL);
						Sleep(200);
					}
					else break;
					m_pVision->OverlayClear(eBTM_CAM_20X);
				}
				//중심 위치로 티핑값 변경
/*
				double curpos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
				curpos= curpos - / *(ParamFile()->Model_data.dStamp_Len_Y / 2.0) - * /ParamFile()->System_data.dBotCam20To2_Y;
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y]= curpos;
				curpos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= curpos;
*/
				ParamFile()->Model_data.dStamp_Btm_20X_1st_Xpix= posx1;
				ParamFile()->Model_data.dStamp_Btm_20X_1st_Ypix= posy1;
				ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix= _INSP_IMAGE_SIZE_X-posx1;
				ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix= posy1;
				ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);

				m_pVision->GetCam_ROICood(eBTM_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= (cam1_Xmm);		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= (cam1_Ymm);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= cam1_Xmm;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= cam1_Ymm;
				strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, cam1_Xmm, cam1_Ymm);
				TRACE(strText.GetBuffer(0));
				pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));

				curpos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
				ParamFile()->Model_data.dPosition[POSITION_READY][_ACS_AXIS_IMP_ROLL]= curpos;
				ParamFile()->Model_data.dPosition[POSITION_STAGE1_ALIGN][_ACS_AXIS_IMP_ROLL]= curpos;
				ParamFile()->Model_data.dPosition[POSITION_STAGE2_ALIGN][_ACS_AXIS_IMP_ROLL]= curpos;
				ParamFile()->Model_data.dPosition[POSITION_STAGE3_ALIGN][_ACS_AXIS_IMP_ROLL]= curpos;
				ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_IMP_ROLL]= curpos;

			//2nd
				move_pos_no= POSITION_ROLL_ALIGN_END;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, ParamFile()->Model_data.dStamp_Len_X, _SPD_MODE, TRUE)== FALSE)
				{
					Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Stage #1 [MovePos:POSITION_ROLL_ALIGN_END]"));
					return FALSE;
				}
				Sleep(200);
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				Sleep(1000);
				Sleep(ParamFile()->System_data.iImageGrab_Delay); 
				m_pVision->InspGrabContinueStop();
				Sleep(200);
				m_pVision->InspGrab(eBTM_CAM_2X);
				find_model_no= mdBTM_20X_ROLL_END;
				if(	m_pVision->PatternFinderPos(&angle, &posx2, &posy2, find_model_no, eBTM_CAM_20X, true, true, true)  == false)
				{
					m_pVision->InspGrabContinue();
					Log()->SaveLogFile(_SYSTEM_LOG, _T("[ FAIL ] Stage #1 [MARK-Search-Error!] [E-1]"));
					return FALSE;
				}
				m_pVision->InspGrabContinue();
				ParamFile()->Model_data.dStamp_Btm_20X_2nd_Xpix= posx2;
				ParamFile()->Model_data.dStamp_Btm_20X_2nd_Ypix= posy2;
				ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix= _INSP_IMAGE_SIZE_X-posx2;
				ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix= posy2;

				if(!g_AutoRunAlignFlag) return FALSE;
				m_pVision->GetCam_ROICood(eBTM_CAM, e20X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= (cam2_Ymm);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= cam2_Xmm;	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= cam2_Ymm;
				strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta, posx2, posy2, cam2_Xmm, cam2_Ymm);
				TRACE(strText.GetBuffer(0));
				pFrm->GetWindowHandler_DebugView()->ListPrint(strText.GetBuffer(0));

				double cal_x= fabs(ParamFile()->Model_data.dStamp_Len_X - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x + ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x);
				double cal_y= fabs(ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta= m_pVision->GetAngle(
					0, 0,
					cal_x, 
					cal_y);
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
				double ang_moveX= 0.0, ang_moveY= 0.0;
				ToPosition1= 0.0;

				if(!g_AutoRunAlignFlag) return FALSE;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_IMP_ROLL, -ParamFile()->Model_data.dStamp_Len_X, _SPD_MODE, TRUE)== FALSE)
				{	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ MOVE-FAIL ] Roll #1 [MovePos:mdBTM_2X_ROLL_START]"));	return FALSE;			}
				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

				//2배로 재이동
				pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;		pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

				pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }

				Cur_X, Cur_Y, Cur_Z, ToPosition1;
				ToPosition1= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Z, POSITION_ROLL_ALIGN);
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

				ToPosition1= ParamFile()->System_data.dBotCam20To2_X;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_X, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				ToPosition1= ParamFile()->System_data.dBotCam20To2_Y;
				if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_LOWER_CAM_Y, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_LOWER_CAM_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_LOWER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

				//중심으로 Align
/*
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x= 0.0;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.x= 0.0;		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_2nd.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.x= 0.0;	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_1st.y= 0.0;
				ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.x= 0.0;	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptRealPoint_2nd.y= 0.0;
				ParamFile()->Model_data.dStamp_Btm_20X_1st_Xpix= 0.0;						ParamFile()->Model_data.dStamp_Btm_20X_1st_Ypix= 0.0;
				ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix= 0.0;						ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix= 0.0;
*/
			}//if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0) else
		}//if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC]== ALIGN_AUTO_MODE)
		if(!g_AutoRunAlignFlag) return FALSE;

 		m_pVision->OverlayClear(eBTM_CAM_2X);		m_pVision->InspGrabContinue();
		m_pVision->OverlayClear(eBTM_CAM_20X);		m_pVision->OverlayClear(eTOP_CAM_2X);		m_pVision->OverlayClear(eTOP_CAM_20X);

		if(!g_AutoRunAlignFlag) return FALSE;
		if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, 0.0, 0.0,	FALSE);
		if(AtMode) { if(ParamFile()->System_data.iCURRENT_RUN_STATUS!= _CURRENT_RUN_RUN) return FALSE; }
		pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE);
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_IMP_ROLL]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_LOWER_CAM_Z]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
		ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
//		ParamFile()->SaveModelConfigFileSemi(ParamFile()->System_data.strCurrentModelName);
		ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
	}
	ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].iAlignEndFlag= 1;

	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();
	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, g_Led_Sel_Num);
	return TRUE;
}
//===================================================================================
BOOL CfrmAlignView::PlateAlignProc(int stageNo)
{
	CString strTTTTT;
	double cam1_Xmm, cam1_Ymm;
	double cam2_Xmm, cam2_Ymm;
	int move_pos_no= 0, find_model_no= 0;
	double angle= 0.0, posx1= 0.0, posy1= 0.0, posx2= 0.0, posy2= 0.0;

	double real_X1= 0.0, real_Y1= 0.0;
	double real_X2= 0.0, real_Y2= 0.0;
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	double AF_Z_Pos= 0.0;
	double AlignCur1st_Pos_X= 0.0;
	double AlignCur1st_Pos_CY= 0.0;
	double AlignCur2nd_Pos_X= 0.0;
	double AlignCur2nd_Pos_CY= 0.0;

	ParamFile()->System_data.dBasePos_Stage1_X= 0.0;
	ParamFile()->System_data.dBasePos_Stage1_CamY= 0.0;

	if(stageNo== STAGE_1_ALIGN_PROC)		pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_1);
	else if(stageNo== STAGE_2_ALIGN_PROC)	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_2);
	else if(stageNo== STAGE_3_ALIGN_PROC)	pFrm->GetWindowHandler_AlignView()->SendMessage(WM_MSG_LED_UPDATE, 0, ePLATE_3);

	ParamFile()->AlignDataClear((PROC_IDX)stageNo);

	if(ParamFile()->Model_data.iRunEnable[stageNo])
	{
		if(pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix < 10.0 && pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix > 1020)
		{
			strText.Format(_T("[ FAIL-%d ] Roll-Align Data - 10x-X"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix < 10.0 && pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix > 1020)
		{
			strText.Format(_T("[ FAIL-%d ] Roll-Align Data - 10x-Y"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix < 10.0 && pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix > 1020)
		{
			strText.Format(_T("[ FAIL-%d ] Roll-Align Data - 2x-X"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix < 10.0 && pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix > 1020)
		{
			strText.Format(_T("[ FAIL-%d ] Roll-Align Data - 2x-Y"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
		if(!g_AutoRunAlignFlag) return FALSE;
		if(stageNo== STAGE_1_ALIGN_PROC)	move_pos_no= POSITION_STAGE1_ALIGN;		else	move_pos_no= POSITION_STAGE2_ALIGN;
		double Rollangle_2X= 0.0;
		double Rollangle_20X= 0.0;

		double x_len= ((ParamFile()->Model_data.dStamp_Top_2X_2nd_Xpix - ParamFile()->Model_data.dStamp_Top_2X_1st_Xpix)*ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0) +
			ParamFile()->Model_data.dStamp_Len_X;
		double y_len= ((ParamFile()->Model_data.dStamp_Top_2X_2nd_Ypix - ParamFile()->Model_data.dStamp_Top_2X_1st_Ypix)*ParamFile()->System_data.dTop_Cam_PixelToum_2X/1000.0);
		Rollangle_2X= -pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, x_len, y_len);

		if(ParamFile()->Model_data.iRoll_AngleAlignMode== RUN_ENABLE && ParamFile()->Model_data.iRoll_AngleAlign_Count > 1 && ParamFile()->Model_data.dRoll_AngleAlign_Pitch > 0.0)
		{
			Rollangle_2X= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta;//= sum_angle;
		}

		x_len= ((ParamFile()->Model_data.dStamp_Top_20X_2nd_Xpix - ParamFile()->Model_data.dStamp_Top_20X_1st_Xpix)*ParamFile()->System_data.dTop_Cam_PixelToum_20X/1000.0) +
			ParamFile()->Model_data.dStamp_Len_X;
		y_len= ((ParamFile()->Model_data.dStamp_Top_20X_2nd_Ypix - ParamFile()->Model_data.dStamp_Top_20X_1st_Ypix)*ParamFile()->System_data.dTop_Cam_PixelToum_20X/1000.0);
		Rollangle_20X= -pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, x_len, y_len);

		if(ParamFile()->Model_data.iAlignMode[stageNo]== ALIGN_AUTO_MODE)
		{
			//1st
			if(stageNo== STAGE_1_ALIGN_PROC)		move_pos_no= POSITION_STAGE1_ALIGN;	
			else if(stageNo== STAGE_2_ALIGN_PROC)	move_pos_no= POSITION_STAGE2_ALIGN;
			else									move_pos_no= POSITION_STAGE3_ALIGN;
			if(MovePos(move_pos_no, FALSE, FALSE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [MovePos:POSITION_STAGE_ALIGN] [S-1]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
///*A/F*/		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_2X, &AF_Z_Pos))
//			{
//	//			ParamFile()->Model_data.dPosition[move_pos_no][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos;
//			}

			m_pVision->InspGrabContinueStop();
			Sleep(ParamFile()->System_data.iImageGrab_Delay);

			m_pVision->InspGrab(eTOP_CAM_2X);
			if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_2X_STAEG1_START;
			else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG2_START;
			else											find_model_no= mdTOP_2X_STAEG3_START;
			AlignCur1st_Pos_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			AlignCur1st_Pos_CY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
			if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
			{
				int find_ROI_Set_Flag= _NORMAL_SELECT;
				if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
				{
					m_pVision->InspGrabContinue();
					strText.Format(_T("[ FAIL ] Stage #%d [MARK-Search_All-Error!] [S-1]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
					return FALSE;
				}
			}
			else
			{
				if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
				{
					m_pVision->InspGrabContinue();
					strText.Format(_T("[ FAIL ] Stage #%d [MARK-Search-Error!] [S-1]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
					return FALSE;
				}
			}
			m_pVision->InspGrabContinue();
			m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
			m_pVision->Trans_RealCoord(e2X_LENS, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
			strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f [CamX=%.3f:Y=%.3f]\n"),
				posx1, posy1, real_X1, real_Y1, cam1_Xmm, cam1_Ymm);
			TRACE(strText.GetBuffer(0));
			//2nd
			if(!g_AutoRunAlignFlag) return FALSE;
			if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_END;
			else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_END;
			else									find_model_no= mdTOP_2X_STAEG3_END;
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[stageNo], _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [MovePos:POSITION_STAGE_ALIGN] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

// /*A/F*/		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_2X, &AF_Z_Pos))
// 			{
// 				ParamFile()->Model_data.dPosition[move_pos_no+1][_ACS_AXIS_UPPER_CAM_Z]= AF_Z_Pos;
// 			}
			AlignCur2nd_Pos_X= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
			AlignCur2nd_Pos_CY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
			m_pVision->InspGrabContinueStop();
			Sleep(ParamFile()->System_data.iImageGrab_Delay);
			m_pVision->InspGrab(eTOP_CAM_2X);
			if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_2X_STAEG1_END;
			else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG2_END;
			else											find_model_no= mdTOP_2X_STAEG3_END;
			pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
			if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
			{
				int find_ROI_Set_Flag= _NORMAL_SELECT;
				if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx2, &posy2, find_model_no, eTOP_CAM_2X, true, true, true)  == false)
				{
					m_pVision->InspGrabContinue();
					strText.Format(_T("[ FAIL ] Stage #%d [MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
					return FALSE;
				}
			}
			else
			{
				if(	m_pVision->PatternFinderPos(&angle, &posx2, &posy2, find_model_no, eTOP_CAM_2X, true, true, true)  == false)
				{
					m_pVision->InspGrabContinue();
					strText.Format(_T("[ FAIL ] Stage #%d [MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
					return FALSE;
				}
			}
			m_pVision->InspGrabContinue();

			m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
			m_pVision->Trans_RealCoord(e2X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
			ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;

			if(!g_AutoRunAlignFlag) return FALSE;
			double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
			double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
			ParamFile()->Align_Result_data[stageNo].dTheta= m_pVision->GetAngle(
				ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
				ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
			ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
			double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
			double ToPosition1= 0.0;
			if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 1)		Rollangle_2X= 0.0;
			ang_moveD= /*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta*/Rollangle_2X - ParamFile()->Align_Result_data[stageNo].dTheta;

			if(fabs(ang_moveD) > 3.0) 
			{
				strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			m_pVision->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
				ang_moveD, &ang_moveX, &ang_moveY);

			if(!g_AutoRunAlignFlag) return FALSE;
			pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
			
			if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_2X_STAEG1_START;
			else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG2_START;
			else											find_model_no= mdTOP_2X_STAEG3_START;
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[stageNo], _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, move_pos_no);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			 
			double chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
			double chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;

			ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
			ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;

			if(!g_AutoRunAlignFlag) return FALSE;
			if(fabs(ang_moveX) > 25.0) 
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Align-X 계산 오류!]\nMax: +-5mm : 계산: %.3f"), stageNo, ang_moveX);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(fabs(ang_moveY) > 25.0) 
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

			strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
			TRACE(strText.GetBuffer(0));

			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			if(!g_AutoRunAlignFlag) return FALSE;
			//-- Cen
			//AfxMessageBox(_T("-0..."));
	
			m_pVision->InspGrabContinueStop();
			Sleep(ParamFile()->System_data.iImageGrab_Delay);
			m_pVision->InspGrab(eTOP_CAM_2X);
			if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_2X_STAEG1_START;
			else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG2_START;
			else											find_model_no= mdTOP_2X_STAEG3_START;
			pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
			if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
			{
				int find_ROI_Set_Flag= _NORMAL_SELECT;
				if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
				{
					m_pVision->InspGrabContinue();
					strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
					return FALSE;
				}
			}
			else
			{
				if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
				{
					m_pVision->InspGrabContinue();
					strText.Format(_T("[ FAIL ] Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
					return FALSE;
				}
			}
			m_pVision->OverlayClear(eTOP_CAM_2X);
			m_pVision->InspGrabContinue();
			m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
			ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);

			//AfxMessageBox(_T("0..."));

			chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
			chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;
// 			double Oldcam1_Xmm= 0.0, Oldcam1_Ymm= 0.0;
// 			m_pVision->GetCam_ROICood(e2X_LENS, 
// 				pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix, 
// 				&Oldcam1_Xmm, &Oldcam1_Ymm);
// 
// 			chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - Oldcam1_Xmm;
// 			chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - Oldcam1_Ymm;

			if(!g_AutoRunAlignFlag) return FALSE;
			if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;
			}
			if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
			{
				strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
				return FALSE;	
			}
			if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

			if(!g_AutoRunAlignFlag) return FALSE;

			if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0)
			{
				int align_count= 0;

				for(align_count= 0; align_count< ParamFile()->Model_data.iMarkMaxCount; align_count++)
				{
					if(!g_AutoRunAlignFlag) return FALSE;
					if(stageNo== STAGE_1_ALIGN_PROC)	move_pos_no= POSITION_STAGE1_ALIGN;		else	move_pos_no= POSITION_STAGE2_ALIGN;
					m_pVision->InspGrabContinueStop();
					Sleep(ParamFile()->System_data.iImageGrab_Delay);
					m_pVision->InspGrab(eTOP_CAM_2X);
					if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG2_START;
					else											find_model_no= mdTOP_2X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
			}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
					m_pVision->Trans_RealCoord(e2X_LENS, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
					ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
					strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
					TRACE(strText.GetBuffer(0));
					//2nd
					if(!g_AutoRunAlignFlag) return FALSE;
					if(stageNo== STAGE_1_ALIGN_PROC)			move_pos_no= POSITION_STAGE1_ALIGN_END;		
					else if(stageNo== STAGE_2_ALIGN_PROC)		move_pos_no= POSITION_STAGE2_ALIGN_END;
					else										move_pos_no= POSITION_STAGE3_ALIGN_END;
					if(stageNo== STAGE_1_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG1_START;		
					else	if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_START;
					else										find_model_no= mdTOP_2X_STAEG3_START;
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[stageNo], _SPD_MODE, TRUE)== FALSE)
					{
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}
					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	/*
					if(align_count== 0)
					{
			/ *A/F* /		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_2X, &AF_Z_Pos))
						{
							Z_2X_Pos_2nd= AF_Z_Pos;
						}
					}
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, Z_2X_Pos_2nd, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	*/

					m_pVision->InspGrabContinueStop();
					Sleep(ParamFile()->System_data.iImageGrab_Delay);
					m_pVision->InspGrab(eTOP_CAM_2X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_END;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_END;
					else									find_model_no= mdTOP_2X_STAEG3_END;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx2, &posy2, find_model_no, eTOP_CAM_2X, true, true, true)  == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
			}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx2, &posy2, find_model_no, eTOP_CAM_2X, true, true, true)  == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->InspGrabContinue();

					m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
					m_pVision->Trans_RealCoord(e2X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
					ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;

					double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
					double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[stageNo].dTheta= m_pVision->GetAngle(
						ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
						ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
					double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
					double ToPosition1= 0.0;
					ang_moveD= /*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta*/Rollangle_2X - ParamFile()->Align_Result_data[stageNo].dTheta;
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_START;
					else									find_model_no= mdTOP_2X_STAEG3_START;
					ang_moveD= ang_moveD + ParamFile()->Model_data.dAlignOffset_T[Stage_Proc];

					if(fabs(ang_moveD) > 3.0) 
					{
						strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}

					m_pVision->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
						ang_moveD, &ang_moveX, &ang_moveY);

					if(!g_AutoRunAlignFlag) return FALSE;
					pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
	//				return FALSE;
					if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_2X_STAEG2_START;
					else											find_model_no= mdTOP_2X_STAEG3_START;
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[stageNo], _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

					double chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
					double chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;

					ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
					ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;

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
					if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, -ang_moveY, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}

					strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
					TRACE(strText.GetBuffer(0));
	//				AfxMessageBox(_T("1..."));

					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					//-- Cen
					if(!g_AutoRunAlignFlag) return FALSE;
					m_pVision->InspGrabContinueStop();		Sleep(ParamFile()->System_data.iImageGrab_Delay);		m_pVision->InspGrab(eTOP_CAM_2X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_START;
					else									find_model_no= mdTOP_2X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
	}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->OverlayClear(eTOP_CAM_2X);				m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);

					chaX=  ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x/*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x*/;
					chaY=  ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y/*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y*/;

					if(!g_AutoRunAlignFlag) return FALSE;
				//	AfxMessageBox(_T("2..."));
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//				AfxMessageBox(_T("3..."));

					m_pVision->InspGrabContinueStop();		Sleep(ParamFile()->System_data.iImageGrab_Delay);		m_pVision->InspGrab(eTOP_CAM_2X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_START;
					else									find_model_no= mdTOP_2X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->OverlayClear(eTOP_CAM_2X);		m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);

					chaX=  ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x/* - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x*/;
					chaY=  ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y/* - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y*/;
					//ang_moveD= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - ParamFile()->Align_Result_data[stageNo].dTheta;
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_START;
					else									find_model_no= mdTOP_2X_STAEG3_START;
					if(ParamFile()->Model_data.dAlignAccuracy_umT > fabs(ang_moveD-ParamFile()->Model_data.dAlignOffset_T[Stage_Proc]))
					{
						strText.Format(_T("[ T-END ] Theta-Align... [%d][%.5f][%.5f-%.5f]"), align_count, ParamFile()->Align_Result_data[stageNo].dTheta, ang_moveD, ParamFile()->Model_data.dAlignOffset_T[Stage_Proc]);
						Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						break;
					}
					//AfxMessageBox(_T(",,,"));
				}

				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				for(align_count= 0; align_count< ParamFile()->Model_data.iMarkMaxCount; align_count++)
				{
					if(!g_AutoRunAlignFlag) return FALSE;
					m_pVision->OverlayClear(eTOP_CAM_2X);		m_pVision->InspGrabContinue();

					m_pVision->InspGrabContinueStop();		Sleep(ParamFile()->System_data.iImageGrab_Delay);		m_pVision->InspGrab(eTOP_CAM_2X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_2X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_2X_STAEG2_START;
					else									find_model_no= mdTOP_2X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_2X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
			}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_2X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 2X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
					double Oldcam1_Xmm= 0.0, Oldcam1_Ymm= 0.0;
					m_pVision->GetCam_ROICood(eTOP_CAM, e2X_LENS, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix, 
						&Oldcam1_Xmm, &Oldcam1_Ymm);

					chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - Oldcam1_Xmm;
					chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - Oldcam1_Ymm;
					if(ParamFile()->Model_data.dAlignAccuracy_umXY/1000.0 > fabs(chaX) && ParamFile()->Model_data.dAlignAccuracy_umXY/1000.0 > fabs(chaY))
					{
						strText.Format(_T("[ S1 : A-END ] Align-END... [%d][T:%.5f][X:%.5f][Y:%.5f]"), align_count, ParamFile()->Align_Result_data[stageNo].dTheta, chaX, chaY);
						Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));

						if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX + (ParamFile()->Model_data.dAlignOffset_X[Stage_Proc]/1000.0), _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
						if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY + (ParamFile()->Model_data.dAlignOffset_Y[Stage_Proc]/1000.0), _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
						if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
						if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
						if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					//2X기준으로 데이터 저장
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y);
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X);
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
						ParamFile()->SaveModelConfigFileSemi(ParamFile()->System_data.strCurrentModelName);
						break;
					}
					else
					{
						if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
						if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
					}
					//AfxMessageBox(_T(",,,2"));
				}
			}//if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG== 0)
			else
			{
				pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
				pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

				double Cur_X, Cur_Y, Cur_Z;
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

				double Z_20X_Pos_1st= 0.0, Z_20X_Pos_2nd= 0.0;
	/*A/F*/		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos))
				{
					Z_20X_Pos_1st= AF_Z_Pos;
				}
	/*
	A/F		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos))
				{
					Z_20X_Pos_1st= AF_Z_Pos;
				}
	*/
	m_pVision->InspGrabContinueStop();
	Sleep(ParamFile()->System_data.iImageGrab_Delay);
	m_pVision->InspGrab(eTOP_CAM_20X);
	if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_20X_STAEG1_START;
	else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_20X_STAEG2_START;
	else											find_model_no= mdTOP_20X_STAEG3_START;

	if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
	{
		int find_ROI_Set_Flag= _NORMAL_SELECT;
		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
		if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
		{
			m_pVision->InspGrabContinue();
			strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
	}
	else
	{
		pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
		if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
		{
			m_pVision->InspGrabContinue();
			strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
			return FALSE;
		}
	}
	m_pVision->OverlayClear(eTOP_CAM_20X);				m_pVision->InspGrabContinue();
	m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
	//AfxMessageBox(_T("c	"));
	chaX= /*- */ ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
	chaY= /*- */ ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;

	if(!g_AutoRunAlignFlag) return FALSE;
	if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
	{	
		strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;			
	}
	if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
	{	
		strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;			
	}
	if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

				for(align_count= 0; align_count< ParamFile()->Model_data.iMarkMaxCount; align_count++)
				{
					if(!g_AutoRunAlignFlag) return FALSE;
					if(stageNo== STAGE_1_ALIGN_PROC)	move_pos_no= POSITION_STAGE1_ALIGN;		else	move_pos_no= POSITION_STAGE2_ALIGN;
					m_pVision->InspGrabContinueStop();
					Sleep(ParamFile()->System_data.iImageGrab_Delay);
					m_pVision->InspGrab(eTOP_CAM_20X);
					if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_20X_STAEG2_START;
					else											find_model_no= mdTOP_20X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
					m_pVision->Trans_RealCoord(e20X_LENS, cam1_Xmm,	cam1_Ymm,  	&real_X1, &real_Y1);
					ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x= real_X1;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y= real_Y1;
					strText.Format(_T("Align-S : x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), posx1, posy1, real_X1, real_Y1);
					TRACE(strText.GetBuffer(0));
					//2nd
					if(!g_AutoRunAlignFlag) return FALSE;
					if(stageNo== STAGE_1_ALIGN_PROC)			move_pos_no= POSITION_STAGE1_ALIGN_END;		
					else if(stageNo== STAGE_2_ALIGN_PROC)		move_pos_no= POSITION_STAGE2_ALIGN_END;
					else										move_pos_no= POSITION_STAGE3_ALIGN_END;
					if(stageNo== STAGE_1_ALIGN_PROC)			find_model_no= mdTOP_20X_STAEG1_START;		
					else	if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_START;
					else										find_model_no= mdTOP_20X_STAEG3_START;
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ParamFile()->Model_data.dAlignMarkToMark[stageNo], _SPD_MODE, TRUE)== FALSE)
					{
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}
					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	/*
					if(align_count== 0)
					{
			/ *A/F* /		if(pFrm->GetWindowHandler_CameraView()->AF_Proc_Run(eTOP_CAM_20X, &AF_Z_Pos))
						{
							Z_20X_Pos_2nd= AF_Z_Pos;
						}
					}
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, Z_20X_Pos_2nd, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	*/

					m_pVision->InspGrabContinueStop();
					Sleep(ParamFile()->System_data.iImageGrab_Delay);
					m_pVision->InspGrab(eTOP_CAM_20X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_20X_STAEG1_END;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_END;
					else									find_model_no= mdTOP_20X_STAEG3_END;
					m_pVision->Trans_RealCoord(e20X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx2, &posy2, find_model_no, eTOP_CAM_20X, true, true, true)  == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx2, &posy2, find_model_no, eTOP_CAM_20X, true, true, true)  == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->InspGrabContinue();

					m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx2, posy2, &cam2_Xmm, &cam2_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.x= (cam2_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_2nd.y= (cam2_Ymm);
					m_pVision->Trans_RealCoord(e20X_LENS, cam2_Xmm,	cam2_Ymm,  	&real_X2, &real_Y2);
					ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x= real_X2;		ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y= real_Y2;

					double cal_x= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x);
					double cal_y= fabs(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[stageNo].dTheta= m_pVision->GetAngle(
						ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y,
						ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_2nd.y);
					ParamFile()->Align_Result_data[stageNo].dDistance= sqrt(cal_x*cal_x + cal_y*cal_y);
					double ang_moveX= 0.0, ang_moveY= 0.0, ang_moveD= 0.0;
					double ToPosition1= 0.0;
					ang_moveD= /*ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta*/Rollangle_20X - ParamFile()->Align_Result_data[stageNo].dTheta;
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_START;
					else									find_model_no= mdTOP_20X_STAEG3_START;
					ang_moveD= ang_moveD + ParamFile()->Model_data.dAlignOffset_T[Stage_Proc];

					if(fabs(ang_moveD) > 3.0) 
					{
						strText.Format(_T("[ FAIL ] Stage #%d [각도 계산 오류!]\nMax: +-3도 : 계산: %.3f"), stageNo, ang_moveD);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;
					}

					m_pVision->GetRotationCoord(ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x, ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y, 
						ang_moveD, &ang_moveX, &ang_moveY);

					if(!g_AutoRunAlignFlag) return FALSE;
					pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_T, ang_moveD, _SPD_MODE);		
	//				return FALSE;
					if(stageNo== STAGE_1_ALIGN_PROC)				find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)			find_model_no= mdTOP_20X_STAEG2_START;
					else											find_model_no= mdTOP_20X_STAEG3_START;
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, ParamFile()->Model_data.dAlignMarkToMark[stageNo], _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, Z_20X_Pos_1st, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

					double chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
					double chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;

					ang_moveX= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.x - ang_moveX) - chaX;
					ang_moveY= (ParamFile()->Align_Result_data[stageNo].dptRealPoint_1st.y - ang_moveY) + chaY;

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

					strText.Format(_T("Align-E : [%.3f] x=%.3f, y=%.3f => X=%.3f, Y=%.3f\n"), ParamFile()->Align_Result_data[stageNo].dTheta, posx2, posy2, real_X2, real_Y2);
					TRACE(strText.GetBuffer(0));
//					AfxMessageBox(_T("0	"));

					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					//-- Cen
					if(!g_AutoRunAlignFlag) return FALSE;
					m_pVision->InspGrabContinueStop();		Sleep(ParamFile()->System_data.iImageGrab_Delay);		m_pVision->InspGrab(eTOP_CAM_20X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_START;
					else									find_model_no= mdTOP_20X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
	}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->OverlayClear(eTOP_CAM_20X);				m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
//AfxMessageBox(_T("1	"));
					chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
					chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;

					if(!g_AutoRunAlignFlag) return FALSE;
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	//				AfxMessageBox(_T("2	"));

					m_pVision->InspGrabContinueStop();		Sleep(ParamFile()->System_data.iImageGrab_Delay);		m_pVision->InspGrab(eTOP_CAM_20X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_START;
					else									find_model_no= mdTOP_20X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)

						{
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
	}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)

						{
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [S]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->OverlayClear(eTOP_CAM_20X);		m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);

					chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.x;
					chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dptPoint_1st.y;
					//ang_moveD= ParamFile()->Align_Result_data[ROLL_ALIGN_PROC].dTheta - ParamFile()->Align_Result_data[stageNo].dTheta;
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_START;
					else									find_model_no= mdTOP_20X_STAEG3_START;
					if(ParamFile()->Model_data.dAlignAccuracy_umT > fabs(ang_moveD-ParamFile()->Model_data.dAlignOffset_T[Stage_Proc]))
					{
						strText.Format(_T("[ T-END ] Theta-Align... [%d][%.5f][%.5f-%.5f]"), align_count, ParamFile()->Align_Result_data[stageNo].dTheta, ang_moveD, ParamFile()->Model_data.dAlignOffset_T[Stage_Proc]);
						Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						break;
					}
					else
					{
						CString strT;
						strT.Format(_T("     - T [%d] => %.4f   [%.4f]"), align_count, ang_moveD, ParamFile()->Model_data.dAlignAccuracy_umT);
						Log()->SaveLogFile(_SYSTEM_LOG, strT.GetBuffer(0));
	}
					//AfxMessageBox(_T(",,,"));
				}

				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		//		AfxMessageBox(_T("3	"));
				for(align_count= 0; align_count< ParamFile()->Model_data.iMarkMaxCount; align_count++)
				{
					if(!g_AutoRunAlignFlag) return FALSE;
					m_pVision->OverlayClear(eTOP_CAM_20X);		m_pVision->InspGrabContinue();

	/*
					if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
					if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
					{	
						strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
						return FALSE;			
					}
	*/

					m_pVision->InspGrabContinueStop();		Sleep(ParamFile()->System_data.iImageGrab_Delay);		m_pVision->InspGrab(eTOP_CAM_20X);
					if(stageNo== STAGE_1_ALIGN_PROC)		find_model_no= mdTOP_20X_STAEG1_START;
					else if(stageNo== STAGE_2_ALIGN_PROC)	find_model_no= mdTOP_20X_STAEG2_START;
					else									find_model_no= mdTOP_20X_STAEG3_START;
					pSystem.GetVisionComponent()->OverlayClear(eTOP_CAM_20X);
					if(ParamFile()->Model_data.iMarkSearchCount[stageNo]) //Stage_Proc= STAGE_1_ALIGN_PROC)
					{
						int find_ROI_Set_Flag= _NORMAL_SELECT;
						if(	m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					else
					{
						if(	m_pVision->PatternFinderPos(&angle, &posx1, &posy1, find_model_no, eTOP_CAM_20X, true, true, true) == false)
						{
							m_pVision->InspGrabContinue();
							strText.Format(_T("[ FAIL ] 10X Stage #%d MARK-Search-Error] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;
						}
					}
					m_pVision->InspGrabContinue();
					m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, posx1, posy1, &cam1_Xmm, &cam1_Ymm);
					ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x= (cam1_Xmm);	ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y= (cam1_Ymm);
					double Oldcam1_Xmm= 0.0, Oldcam1_Ymm= 0.0;
					m_pVision->GetCam_ROICood(eTOP_CAM, e20X_LENS, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix, pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix, 
						&Oldcam1_Xmm, &Oldcam1_Ymm);

					chaX= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.x - Oldcam1_Xmm;
					chaY= ParamFile()->Align_Result_data[stageNo].dptPoint_1st.y - Oldcam1_Ymm;
					if( (ParamFile()->Model_data.dAlignAccuracy_umXY/1000.0 > fabs(chaX) && ParamFile()->Model_data.dAlignAccuracy_umXY/1000.0 > fabs(chaY)) || 
						(align_count>= ParamFile()->Model_data.iMarkMaxCount))
					{
						strText.Format(_T("[ S1 : A-END ] Align-END... [%d][T:%.5f][X:%.5f][Y:%.5f]"), align_count, ParamFile()->Align_Result_data[stageNo].dTheta, chaX, chaY);
						Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));

						if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX + (ParamFile()->Model_data.dAlignOffset_X[Stage_Proc]/1000.0), _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
						if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY + (ParamFile()->Model_data.dAlignOffset_Y[Stage_Proc]/1000.0), _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
						if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
						if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
						if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
					//2X기준으로 데이터 저장
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_UPPER_CAM_Y]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_UPPER_CAM_Y)-ParamFile()->System_data.dTopCam20To2_Y;
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_STAGE_X]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X)-ParamFile()->System_data.dTopCam20To2_X;
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y1]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y1);
						ParamFile()->Align_Result_data[stageNo].dPosData[_ACS_AXIS_ALIGN_Y2]= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_ALIGN_Y2);
						ParamFile()->SaveModelConfigFileSemi(ParamFile()->System_data.strCurrentModelName);
						break;
					}
					else
					{
						strText.Format(_T("       [ S1 : A-- ] Align... [%d][T:%.5f][X:%.5f][Y:%.5f]"), 
							align_count, ParamFile()->Align_Result_data[stageNo].dTheta, chaX, chaY);
						Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));

						if(pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -chaX, _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
						if(pSystem.GetMotionComponent()->Align_INC_Move(ALIGN_STAGE_Y, chaY, _SPD_MODE, TRUE)== FALSE)
						{	
							strText.Format(_T("[ FAIL ] Stage #%d [Inc. MovePos:POSITION_STAGE_ALIGN] [E]"), stageNo);	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
							return FALSE;			
						}
					}
					//AfxMessageBox(_T(",,,2"));
				}
// 				AfxMessageBox(_T("..."));
// 				return FALSE;

				//2X위치로 재 이동
				ToPosition1= ParamFile()->System_data.dTopCam20To2_X;
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_STAGE_X, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				ToPosition1= ParamFile()->System_data.dTopCam20To2_Y;
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Y, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				ToPosition1= ParamFile()->System_data.dTopCam20To2_Z;
				if(!pSystem.GetMotionComponent()->INC_Move(_ACS_AXIS_UPPER_CAM_Z, -ToPosition1, _SPD_MODE,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE)		{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Y)== FALSE)	{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
				if(MoveEndCheckNonDisp(_ACS_AXIS_UPPER_CAM_Z)== FALSE)	{	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
			}
		}
		m_pVision->InspGrabContinue();
		m_pVision->OverlayClear(eTOP_CAM_20X);		m_pVision->OverlayClear(eTOP_CAM_2X);
		m_pVision->OverlayClear(eBTM_CAM_20X);		m_pVision->OverlayClear(eBTM_CAM_2X);

	}
	ParamFile()->Align_Result_data[stageNo].iAlignEndFlag= 1;
	bAlign_End_Flag= true;
	return TRUE;
}
//===================================================================================
BOOL CfrmAlignView::Move_AlignEndPos(int stage_no, double *retX, double *retY1)
{
	return FALSE;
}
/*
switch(ModelNo)
{
case mdTOP_20X_STAEG1_START:		MovePos(POSITION_STAGE1_ALIGN, TRUE);		break;
case mdTOP_20X_STAEG1_END:			MovePos(POSITION_STAGE1_ALIGN_END, TRUE);	break;
case mdTOP_2X_STAEG1_START:			MovePos(POSITION_STAGE1_ALIGN, FALSE);		break;
case mdTOP_2X_STAEG1_END:			MovePos(POSITION_STAGE1_ALIGN_END, FALSE);	break;
case mdTOP_20X_STAEG2_START:		MovePos(POSITION_STAGE2_ALIGN, TRUE);		break;
case mdTOP_20X_STAEG2_END :			MovePos(POSITION_STAGE2_ALIGN_END, TRUE);	break;
case mdTOP_2X_STAEG2_START:			MovePos(POSITION_STAGE2_ALIGN, FALSE);		break;
case mdTOP_2X_STAEG2_END:			MovePos(POSITION_STAGE2_ALIGN_END, FALSE);	break;
case mdBTM_20X_ROLL_START:			MovePos(POSITION_ROLL_ALIGN, TRUE);			break;
case mdBTM_20X_ROLL_END:			MovePos(POSITION_ROLL_ALIGN_END, TRUE);		break;
case mdBTM_2X_ROLL_START:			MovePos(POSITION_ROLL_ALIGN, FALSE);		break;
case mdBTM_2X_ROLL_END:				MovePos(POSITION_ROLL_ALIGN_END, FALSE);	break;
case mdTOP_20X_MASK:				MovePos(POSITION_STAGE1_ALIGN, TRUE);		break;
case mdTOP_2X_MASK:					MovePos(POSITION_TOP_MASK_20X, FALSE);		break;
case mdBTM_20X_MASK:				MovePos(POSITION_STAGE1_ALIGN, TRUE);		break;
case mdBTM_2X_MASK:					MovePos(POSITION_BTM_MASK_20X, FALSE);		break;
default:
	MyMessageBox(_T("모델 선택 에러."), _T("Position-Move Failed"), M_ICONERROR); return ;
	break;
}
*/

//===================================================================================
void CfrmAlignView::ClickJogSelectTused()
{
	m_manuStamp_Camera= eTOP_CAM;
}

//===================================================================================
void CfrmAlignView::ClickJogSelectBused()
{
	m_manuStamp_Camera= eBTM_CAM;
}

//===================================================================================
void CfrmAlignView::ClickMarkTest3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	proc_run_flag= true;
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CString strText;

	strText.Format(_T("스템프 Align 동작을 진행 하시겠습니까?"));
	if (MyMessageBox(strText.GetBuffer(0), _T("Align"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
		proc_run_flag= false;
		return;
	}

	CdlgManuAlign dlg;
	dlg.m_StageNo= MANUAL_STAMP_PROC;
	dlg.CameraStageUsedNo= m_manuStamp_Camera; dlg.iStamInstRunFlag= 0;
	if(dlg.DoModal()== IDCANCEL)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ 스템프 ALIGN-FAIL ] [스템프 ALIGN_MODE]"));
	}
	else
	{
		if(dlg.Align_Pos_Set_Flag)
		{
			CdlgManuCenter dlg;
			if(dlg.DoModal()== IDCANCEL)
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN 기준 위치 설정 - 취소 ]"));
			}
			else
			{
				CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
			}
		}
	}

	proc_run_flag= false;
}

//===================================================================================
void CfrmAlignView::ClickCmMove6()
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
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, LensNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, LensNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, LensNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
			pSystem.GetMotionComponent()->GetStampPos(CameraStageUsedNo, LensNo, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
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
void CfrmAlignView::ClickCmMarkLoad()
{ 
	int SelModelNo;
	CdlgMarkCopy dlg;
	dlg.CurModel= ModelNo;
	if(dlg.DoModal()== IDOK)
	{
		SelModelNo= dlg.SelModel;

		int ret= m_pVision->PatternLoad(SelModelNo);

		if(ParamFile()->Pattern_data[SelModelNo].iSpeed == 2)		m_ctrlSpeed.SetCurSel(0);
		else if(ParamFile()->Pattern_data[SelModelNo].iSpeed == 3)	m_ctrlSpeed.SetCurSel(1);
		else if(ParamFile()->Pattern_data[SelModelNo].iSpeed == 4)	m_ctrlSpeed.SetCurSel(2);
		else {
			ParamFile()->Pattern_data[SelModelNo].iSpeed =M_VERY_HIGH;	m_ctrlSpeed.SetCurSel(2);
		}

		if(ParamFile()->Pattern_data[SelModelNo].iAccuracy == 2)		m_ctrlAccuracy.SetCurSel(0);
		else if(ParamFile()->Pattern_data[SelModelNo].iAccuracy == 3)	m_ctrlAccuracy.SetCurSel(1);
		else {	
			ParamFile()->Pattern_data[SelModelNo].iAccuracy =M_MEDIUM;	m_ctrlAccuracy.SetCurSel(0);
		}

		if(ParamFile()->Pattern_data[SelModelNo].dScoreAcceptance<= 0.0) ParamFile()->Pattern_data[SelModelNo].dScoreAcceptance= 50.0;
		m_dScoreAcceptance =		ParamFile()->Pattern_data[SelModelNo].dScoreAcceptance;

		if(ParamFile()->Pattern_data[SelModelNo].dScoreCertanty<= 0.0) ParamFile()->Pattern_data[SelModelNo].dScoreCertanty= 60.0;
		m_dScoreCertanty =			ParamFile()->Pattern_data[SelModelNo].dScoreCertanty; //이상해??? Pattern_data

		if(ParamFile()->Pattern_data[SelModelNo].iTimeOut<= 0) ParamFile()->Pattern_data[SelModelNo].iTimeOut= 1000;
		m_TimeOut =					ParamFile()->Pattern_data[SelModelNo].iTimeOut;

		m_dOffsetX=					ParamFile()->Pattern_data[SelModelNo].dOffsetX;
		m_dOffsetY=					ParamFile()->Pattern_data[SelModelNo].dOffsetY;

		m_dAlignMarkToMark=		ParamFile()->Model_data.dAlignMarkToMark[Stage_Proc];
		m_dAlignMarkToMarkY=	ParamFile()->Model_data.dAlignMarkToMarkY[Stage_Proc];
		m_iMarkMaxCount=		ParamFile()->Model_data.iMarkMaxCount;
		m_dAlignAccuracy_umXY=	ParamFile()->Model_data.dAlignAccuracy_umXY;
		m_dAlignAccuracy_umT=	ParamFile()->Model_data.dAlignAccuracy_umT;
		m_dAlignOffset_X=		ParamFile()->Model_data.dAlignOffset_X[Stage_Proc];
		m_dAlignOffset_Y=		ParamFile()->Model_data.dAlignOffset_Y[Stage_Proc];
		m_dAlignOffset_T=		ParamFile()->Model_data.dAlignOffset_T[Stage_Proc];

		m_pVision->PatternCopy(SelModelNo, ModelNo);
		m_pVision->PatternImageLoadNdisp(SelModelNo);
		m_pVision->PatternSave(ModelNo);
		UpdateData(FALSE);
//		Invalidate();
 		DataSave();
 		DataInit();
	}

}


void CfrmAlignView::OnBnClickedButton5()
{
 	CdlgManuCenter dlg;
 	if(dlg.DoModal()== IDCANCEL)
 	{
 		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN 기준 위치 설정 - 취소 ]"));
 	}

/*
	double ToX, ToY1, ToY2, ToCy, ToCz, ToBx, ToBy, ToBz, ToPosition1, ToPosition2;
	CString strText;

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

	ToX= 234.4693;
	ToY1= 5.9873;
	ToY2= -0.9163;
	ToCy= 67.9035;
	ToCz= 14.0589;
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
*/

}

void CfrmAlignView::OnBnClickedCheckMaskSet()
{
	//	m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	// 	pFrm->GetWindowHandler_CameraView()->Left_PatternROISetFlag_1(false);
	// 	pFrm->GetWindowHandler_CameraView()->Right_PatternROISetFlag_1(false);
	UpdateData(TRUE);	
}


void CfrmAlignView::OnBnClickedMaskEditApply()
{
	pSystem.GetVisionComponent()->PatternMaskApply(ModelNo, 0.0);
	m_bMaskEdit_L= FALSE;
	UpdateData(FALSE);
}


void CfrmAlignView::OnBnClickedMaskEditClear()
{
	pSystem.GetVisionComponent()->PatternMaskEditClear(ModelNo, 0.0);	
	m_bMaskEdit_L= FALSE;
	UpdateData(FALSE);
}


void CfrmAlignView::OnNMCustomdrawBrushSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nBrushSize_L = m_ctrlBrushSlider_L.GetPos() ;	
	UpdateData(FALSE) ;
	*pResult = 0;
}


void CfrmAlignView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bMaskEdit_L) {
		if(m_dispRect_L.PtInRect(point))
		{
			UpdateData() ;
			CPoint Start = point - m_dispRect_L.TopLeft() ;
			pSystem.GetVisionComponent()->PatternDrawFillPoint(Start, 1, m_nBrushSize_L) ;
		}
	}


	CFormView::OnLButtonDown(nFlags, point);
}


void CfrmAlignView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnLButtonUp(nFlags, point);
}


void CfrmAlignView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bMaskEdit_L) {
		if(m_dispRect_L.PtInRect(point) && (nFlags & MK_LBUTTON) == MK_LBUTTON)
		{
			UpdateData() ;
			CPoint Start = point - m_dispRect_L.TopLeft() ;
			pSystem.GetVisionComponent()->PatternDrawFillPoint(Start, 1, m_nBrushSize_L) ;
		}
	}

	CFormView::OnMouseMove(nFlags, point);
}
void CfrmAlignView::ResetEditCtrl()
{
	m_bMaskEdit_L= FALSE;
	UpdateData(FALSE);

}

void CfrmAlignView::ClickMarkTest5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_AUTO_MODE)
	{
		MyMessageBox(_T("Roll-Aling 동작이 자동모드 입니다.\n수동 모드에서 동작"), _T("Aling-mode"), M_ICONERROR); return;
	}
	if(m_manuStamp_Camera== eTOP_CAM)
	{
		MyMessageBox(_T("상부 카메라는 지원하지 않습니다."), _T("Aling-mode"), M_ICONERROR); return;
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE)
		{
			if (MyMessageBox(_T("Roll-Aling 동작이 활성화 상태입니다.\nRoll-Align을 비활성화 후 진행하시겠습니까?"), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
			{
				Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CfrmAlignView : Align-Test.! => CANCEL"));
				return;
			}
			ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]= RUN_DISABLE;
			Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] CfrmDataView 수정 / 저장"));
			ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
		}
	}
	else
	{
		if(ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]!= RUN_ENABLE || ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_NONE_MODE)
		{
			MyMessageBox(_T("Roll-Aling 동작이 비 활성화 상태 입니다."), _T("Aling-mode"), M_ICONERROR); return;
		}
	}

	proc_run_flag= true;
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CString strText;

	if(m_manuStamp_Camera== eBTM_CAM)	
	{
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE)	strText.Format(_T("[하부카메라]\n쿼츠 위에서 Roll 기준 위치 설정 동작을\n 진행 하시겠습니까?"));
		else					
		{
				strText.Format(_T("[하부카메라]\n Roll 수동-Align 동작을 진행 하시겠습니까?"));
		}
	}
	else		
	{
			MyMessageBox(_T("상부 카메라는 지원하지 않습니다."), _T("Aling-mode"), M_ICONERROR); return;
	//		strText.Format(_T("[상부카메라]\n기준 위치 설정 동작을 진행 하시겠습니까?"));
	}
	if (MyMessageBox(strText.GetBuffer(0), _T("Align"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)	
	{
		proc_run_flag= false;
		return;
	}
	if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && m_manuStamp_Camera== eTOP_CAM)	
	{
		MyMessageBox(_T("상부 카메라는 지원하지 않습니다."), _T("Aling-mode"), M_ICONERROR); return;
	}
	CdlgManuAlign dlg;
	if(	ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE && ParamFile()->Model_data.iRunEnable[ROLL_ALIGN_PROC]== RUN_ENABLE && 
		ParamFile()->Model_data.iAlignMode[ROLL_ALIGN_PROC] == ALIGN_MANU_MODE)
																		dlg.m_StageNo= MANUAL_ROLL_ALIGN_PROC;
	else																dlg.m_StageNo= MANUAL_ROLL_ALIGN_PROC;

	dlg.CameraStageUsedNo= m_manuStamp_Camera; dlg.iStamInstRunFlag= 0;

	if(dlg.DoModal()== IDCANCEL)
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[ Roll 수동 Align-FAIL ] [스템프 ALIGN_MODE]"));
	}
	else
	{
		if(ParamFile()->System_data.RollAlignPosMode== _QT_POS_MODE)// && m_StageNo == MANUAL_ROLL_ALIGN_PROC)	//m_StageNo==MANUAL_ROLL_ALIGN_PROC
		{
			if(m_manuStamp_Camera== eBTM_CAM)	
			{
				if(dlg.Align_Pos_Set_Flag)
				{
					CdlgManuCenter dlg;
					if(dlg.DoModal()== IDCANCEL)
					{
						Log()->SaveLogFile(_SYSTEM_LOG, _T("[ ALIGN 기준 위치 설정 - 취소 ]"));
					}
					else
					{
						CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 	pFrm->Data_Reload();
					}
				}
			}
			else
			{

			}
		}
	}

	proc_run_flag= false;
}

BOOL CfrmAlignView::PP_Pos_Move(int no, bool dispflag)
{
	CString strText;

	int StageSelNo= no;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	
	if(dispflag)
	{
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	else
	{
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	return FALSE; }
	}

	if(dispflag)
	{
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
		ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	double tarPosX= 0.0, tarPosY= 0.0;

	pSystem.GetMotionComponent()->GetPickPlace_Pos(StageSelNo, ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_STAGE_X], 
		ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_UPPER_CAM_Y], 
		&tarPosX,  &tarPosY ); 

	//	return FALSE;

	double ang_moveX= 0.0, ang_moveY= 0.0;

	pSystem.GetVisionComponent()->GetRotationCoord( -ParamFile()->Model_data.dRun_Offset_X[StageSelNo], 
													-ParamFile()->Model_data.dRun_Offset_Y[StageSelNo],
													ParamFile()->Align_Result_data[StageSelNo].dTheta, &ang_moveX, &ang_moveY);

	//------------
	double xxx= 0.0;
	double yyy= 0.0;
	if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG)
	{
		xxx= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Xpix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix;
		yyy= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Ypix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix;
		xxx= xxx * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
		yyy= yyy * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
	}
	else
	{
		xxx= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Xpix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix;
		yyy= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Ypix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix;
		xxx= xxx * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_2X / 1000.0);
		yyy= yyy * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_2X / 1000.0);
	}

 	double angle_tmp= pSystem.GetVisionComponent()->GetAngle(0.0, 0.0, xxx + ParamFile()->Model_data.dAlignMarkToMark[StageSelNo], yyy);
 	pSystem.GetVisionComponent()->GetRotationCoord( -ParamFile()->Model_data.dRun_Offset_X[StageSelNo], -ParamFile()->Model_data.dRun_Offset_Y[StageSelNo], 
		-angle_tmp, &ang_moveX, &ang_moveY);
	//------------

//	return FALSE;

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, tarPosX+ang_moveX,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	ToPosition1	= ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_ALIGN_Y1]+ang_moveY;
	ToPosition2	= ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_ALIGN_Y2]+ang_moveY;
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(dispflag)
	{
		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	else
	{
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	return FALSE; }
	}
	return TRUE;
}
BOOL CfrmAlignView::PP_Pos_Move_Test(int no, bool dispflag)
{
	CString strText;

	int StageSelNo= no;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	if(dispflag)
	{
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	else
	{
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	return FALSE; }
	}

	if(dispflag)
	{
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
		ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	double tarPosX= 0.0, tarPosY= 0.0;

	pSystem.GetMotionComponent()->GetPickPlace_Pos(StageSelNo, ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_STAGE_X], 
		ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_UPPER_CAM_Y], 
		&tarPosX,  &tarPosY ); 

	//	return FALSE;

	double ang_moveX= 0.0, ang_moveY= 0.0;

	//------------
	double xxx= 0.0;
	double yyy= 0.0;
	if(pSystem.GetParameterFileComponent()->System_data.iCAM_20X_USED_FLAG)
	{
		xxx= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Xpix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Xpix;
		yyy= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_2nd_Ypix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_20X_1st_Ypix;
		xxx= xxx * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
		yyy= yyy * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_20X / 1000.0);
	}
	else
	{
		xxx= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Xpix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Xpix;
		yyy= pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_2nd_Ypix - pSystem.GetParameterFileComponent()->Model_data.dStamp_Top_2X_1st_Ypix;
		xxx= xxx * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_2X / 1000.0);
		yyy= yyy * (pSystem.GetParameterFileComponent()->System_data.dTop_Cam_PixelToum_2X / 1000.0);
	}

	//------------

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, tarPosX+ang_moveX,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	ToPosition1	= ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_ALIGN_Y1]+ang_moveY;
	ToPosition2	= ParamFile()->Align_Result_data[StageSelNo].dPosData[_ACS_AXIS_ALIGN_Y2]+ang_moveY;
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(dispflag)
	{
		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	else
	{
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	return FALSE; }
	}
	return TRUE;
}
BOOL CfrmAlignView::NonAling_PP_Pos_Move(int no, bool dispflag)
{
	CString strText;
	int StageSelNo= no;

	double ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	double ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	if(dispflag)
	{
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	else
	{
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	return FALSE; }
	}

	if(dispflag)
	{
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
		ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
		ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
		if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	double L_X, Stage_Y, TopCamY, bot_X, bot_Y, Roll_pos;

	pSystem.GetMotionComponent()->GetStampPos(eTOP_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
	double YopStampPosX= L_X;
	double Cur_X= 0.0;
	double Cur_Y= 0.0;
	switch(StageSelNo)
	{
	case STAGE_1_ALIGN_PROC:	
		Cur_X= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE1_ALIGN);
		break;
	case STAGE_2_ALIGN_PROC:	
		Cur_X= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE2_ALIGN);
		break;
	case STAGE_3_ALIGN_PROC:	
		Cur_X= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, POSITION_STAGE3_ALIGN);//+move_stamp_posbx;//+ParamFile()->System_data.dRollPlateStamp_PosX;
		break;
	default:
		AfxMessageBox(L"Stage-number -> Not Define!!!");
		Cur_X= 0.0;
		return FALSE;
	}

	pSystem.GetMotionComponent()->GetStampPos(eBTM_CAM, e2X_LENS, &L_X, &Stage_Y, &TopCamY, &bot_X, &bot_Y, &Roll_pos);
	L_X= L_X + ParamFile()->System_data.dStampMoveOffset_X;
	double moveXpos= L_X+(Cur_X-YopStampPosX);

	double ang_moveX= 0.0, ang_moveY= 0.0;

	//------------
	double xxx= 0.0;
	double yyy= 0.0;
 	double angle_tmp= 0.0;
 	pSystem.GetVisionComponent()->GetRotationCoord( -ParamFile()->Model_data.dRun_Offset_X[StageSelNo], -ParamFile()->Model_data.dRun_Offset_Y[StageSelNo], 
		-angle_tmp, &ang_moveX, &ang_moveY);
	//------------

//	return FALSE;

	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, moveXpos+ang_moveX,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	ToPosition1	= ang_moveY;
	ToPosition2	= ang_moveY;
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }

	if(dispflag)
	{
		if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
		if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return FALSE; }
	}
	else
	{
		if(MoveEndCheckNonDisp(_ACS_AXIS_STAGE_X)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y1)== FALSE) {	return FALSE; }
		if(MoveEndCheckNonDisp(_ACS_AXIS_ALIGN_Y2)== FALSE) {	return FALSE; }
	}

	return TRUE;
}


void CfrmAlignView::OnBnClickedButton3()
{
	CdlgManuAlign dlg;
	dlg.m_StageNo= MANUAL_QT_ROLL_ALIGN_PROC;

	dlg.CameraStageUsedNo= eBTM_CAM; 
	dlg.iStamInstRunFlag= 0;

	dlg.DoModal();
/*
	CString strText;
	//	double L_X, Stage_Y, TopCamY, bot_X, bot_Y;
	double ToPosition1, ToPosition2;

	int Stage_No= STAGE_3_ALIGN_PROC;

	if(!bAlign_End_Flag && ParamFile()->Model_data.iAlignMode[Stage_No]!= ALIGN_NONE_MODE)
	{
		MyMessageBox(_T("Align 동작이 안되었습니다."), _T("1-Cycle Failed"), M_ICONERROR); return ; 
	}

	if (MyMessageBox(_T("Align 데이터을 기준으로\n1-Cycle 동작을 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[확인] => ClickOriginReturn : OnClickReset.! => CANCEL"));
		return;
	}

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(ParamFile()->Model_data.iRunEnable[Stage_No]== RUN_ENABLE) 
	{
		if(ParamFile()->Model_data.iAlignMode[Stage_No]== ALIGN_NONE_MODE)
		{
			// 			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
			// 			pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE2_ALIGN, FALSE);
			// 			NonAling_PP_Pos_Move(STAGE_2_ALIGN_PROC);
			if(!NonAling_PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
		else
		{
				if(!PP_Pos_Move(Stage_No))
				{
					MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
				}
		}
	}
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
*/

/*
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_READY, FALSE);
*/
}

void CfrmAlignView::SetLedVal()
{
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)
		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Top_Val_2X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_2X[ModelGroupNo];	}
		else
		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Top_Val_20X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_20X[ModelGroupNo];	}
		UpdateData(FALSE) ;
	}
	else
	{
		if(LensNo== e2X_LENS)
		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Btm_Val_2X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_2X[ModelGroupNo];	}
		else
		{	m_ctrlLedSlider.SetPos(ParamFile()->System_data.iLED_Btm_Val_20X[ModelGroupNo]) ;	m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_20X[ModelGroupNo];	}
		UpdateData(FALSE) ;
	}
	SetLed(ModelGroupNo);
}

void CfrmAlignView::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)
			m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_2X[ModelGroupNo]= m_ctrlLedSlider.GetPos();	
		else
			m_iLedcontrolVal = ParamFile()->System_data.iLED_Top_Val_20X[ModelGroupNo]= m_ctrlLedSlider.GetPos();	
		UpdateData(FALSE) ;
	}
	else
	{
		if(LensNo== e2X_LENS)
			m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_2X[ModelGroupNo]= m_ctrlLedSlider.GetPos();	
		else
			m_iLedcontrolVal = ParamFile()->System_data.iLED_Btm_Val_20X[ModelGroupNo]= m_ctrlLedSlider.GetPos();	
		UpdateData(FALSE) ;
	}
	if(ParamFile()->System_data.Led_Remote)	SendMessage(WM_MSG_LED_UPDATE, 0, ModelGroupNo);
//	SetLed(UsedNo);
	*pResult = 0;
}
void CfrmAlignView::SetLed(int flagVal)
{
	//ePLATE_1 = 0, ePLATE_2,  eROLL, ePHOTOMASK, eSYNC, ePLATE_3
	if(ParamFile()->System_data.Led_Remote)	SendMessage(WM_MSG_LED_UPDATE, 0, flagVal);
}

LRESULT CfrmAlignView::SetLedUpdate(WPARAM wParam,LPARAM lParam)
{
	if(ParamFile()->System_data.Led_Remote)
	{
		LED_cnt.LightVal(_LED_CH1, _ON, ParamFile()->System_data.iLED_Top_Val_2X[lParam]); 
		LED_cnt.LightVal(_LED_CH2, _ON, ParamFile()->System_data.iLED_Top_Val_20X[lParam]);
		LED_cnt.LightVal(_LED_CH3, _ON, ParamFile()->System_data.iLED_Btm_Val_2X[lParam]);
		LED_cnt.LightVal(_LED_CH4, _ON, ParamFile()->System_data.iLED_Btm_Val_20X[lParam]);
	}
	return 0;
}


void CfrmAlignView::OnBnClickedButton4()
{
	UpdateData(TRUE);

	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)
		{	ParamFile()->System_data.iLED_Top_Val_2X[ModelGroupNo]= m_iLedcontrolVal; m_ctrlLedSlider.SetPos(m_iLedcontrolVal);	}
		else
		{	ParamFile()->System_data.iLED_Top_Val_20X[ModelGroupNo]= m_iLedcontrolVal; m_ctrlLedSlider.SetPos(m_iLedcontrolVal);	}
		UpdateData(FALSE) ;
	}
	else
	{
		if(LensNo== e2X_LENS)
		{	ParamFile()->System_data.iLED_Btm_Val_2X[ModelGroupNo]= m_iLedcontrolVal; m_ctrlLedSlider.SetPos(m_iLedcontrolVal);	}
		else
		{	ParamFile()->System_data.iLED_Btm_Val_20X[ModelGroupNo]= m_iLedcontrolVal; m_ctrlLedSlider.SetPos(m_iLedcontrolVal);	}
	}
	if(ParamFile()->System_data.Led_Remote)	SendMessage(WM_MSG_LED_UPDATE, 0, ModelGroupNo);
}


void CfrmAlignView::ClickMarkRoiSel2()
{
	CString strText;
	//	double L_X, Stage_Y, TopCamY, bot_X, bot_Y;
	double ToPosition1, ToPosition2;

	int Stage_No= STAGE_1_ALIGN_PROC;

	if(!bAlign_End_Flag && ParamFile()->Model_data.iAlignMode[Stage_No]!= ALIGN_NONE_MODE)
	{
		MyMessageBox(_T("Align 동작이 안되었습니다."), _T("1-Cycle Failed"), M_ICONERROR); return ; 
	}

	if (MyMessageBox(_T("Align 데이터을 기준으로\n1-Cycle 동작을 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[확인] => ClickOriginReturn : OnClickReset.! => CANCEL"));
		return;
	}

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(ParamFile()->Model_data.iRunEnable[Stage_No]== RUN_ENABLE) 
	{
		if(ParamFile()->Model_data.iAlignMode[Stage_No]== ALIGN_NONE_MODE)
		{
			// 			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
			// 			pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE2_ALIGN, FALSE);
			// 			NonAling_PP_Pos_Move(STAGE_2_ALIGN_PROC);
			if(!NonAling_PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
		else
		{
			if(!PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
	}
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
}


void CfrmAlignView::ClickMarkRoiSel3()
{
	CString strText;
	//	double L_X, Stage_Y, TopCamY, bot_X, bot_Y;
	double ToPosition1, ToPosition2;

	int Stage_No= STAGE_2_ALIGN_PROC;

	if(!bAlign_End_Flag && ParamFile()->Model_data.iAlignMode[Stage_No]!= ALIGN_NONE_MODE)
	{
		MyMessageBox(_T("Align 동작이 안되었습니다."), _T("1-Cycle Failed"), M_ICONERROR); return ; 
	}

	if (MyMessageBox(_T("Align 데이터을 기준으로\n1-Cycle 동작을 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[확인] => ClickOriginReturn : OnClickReset.! => CANCEL"));
		return;
	}

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(ParamFile()->Model_data.iRunEnable[Stage_No]== RUN_ENABLE) 
	{
		if(ParamFile()->Model_data.iAlignMode[Stage_No]== ALIGN_NONE_MODE)
		{
			// 			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
			// 			pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE2_ALIGN, FALSE);
			// 			NonAling_PP_Pos_Move(STAGE_2_ALIGN_PROC);
			if(!NonAling_PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
		else
		{
			if(!PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
	}
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
}


void CfrmAlignView::ClickMarkRoiSel4()
{
	CString strText;
	//	double L_X, Stage_Y, TopCamY, bot_X, bot_Y;
	double ToPosition1, ToPosition2;

	int Stage_No= STAGE_3_ALIGN_PROC;

	if(!bAlign_End_Flag && ParamFile()->Model_data.iAlignMode[Stage_No]!= ALIGN_NONE_MODE)
	{
		MyMessageBox(_T("Align 동작이 안되었습니다."), _T("1-Cycle Failed"), M_ICONERROR); return ; 
	}

	if (MyMessageBox(_T("Align 데이터을 기준으로\n1-Cycle 동작을 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[확인] => ClickOriginReturn : OnClickReset.! => CANCEL"));
		return;
	}

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_X];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, POSITION_ROLL_ALIGN);
	ToPosition1	= ParamFile()->Model_data.dPosition[POSITION_ROLL_ALIGN][_ACS_AXIS_LOWER_CAM_Y];
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(ParamFile()->Model_data.iRunEnable[Stage_No]== RUN_ENABLE) 
	{
		if(ParamFile()->Model_data.iAlignMode[Stage_No]== ALIGN_NONE_MODE)
		{
			// 			CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
			// 			pFrm->GetWindowHandler_TeachView()->MovePos(POSITION_STAGE2_ALIGN, FALSE);
			// 			NonAling_PP_Pos_Move(STAGE_2_ALIGN_PROC);
			if(!NonAling_PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
		else
		{
			if(!PP_Pos_Move(Stage_No))
			{
				MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; 
			}
		}
	}
	ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
	ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
	if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }

	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return ; }
	if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return ; }
}


void CfrmAlignView::OnBnClickedButton7() 
{
	CString strText;
	int dispPos_Sel;
	double ToPosition1, ToPosition2;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.Stage_Proc= STAGE_3_ALIGN_PROC
	if(CamNo== eTOP_CAM)
	{
		if(Stage_Proc== STAGE_1_ALIGN_PROC || Stage_Proc== STAGE_2_ALIGN_PROC || Stage_Proc== STAGE_3_ALIGN_PROC)
		{
			if(Stage_Proc== STAGE_1_ALIGN_PROC)			dispPos_Sel= POSITION_STAGE1_ALIGN;//POSITION_ROLL_ALIGN
			else if(Stage_Proc== STAGE_2_ALIGN_PROC)	dispPos_Sel= POSITION_STAGE2_ALIGN;//POSITION_ROLL_ALIGN
			else										dispPos_Sel= POSITION_STAGE3_ALIGN;//POSITION_ROLL_ALIGN
			if(MarkNo== eALIGN_1ST_MARK)
			{
				//-----------------
				strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n"));
				if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
				//-----------------
				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
				ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
				if(LensNo== e20X_LENS)
				{
					ToPosition1= ToPosition1 + ParamFile()->System_data.dTopCam20To2_X;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
				if(LensNo== e20X_LENS)
				{
					ToPosition1= ToPosition1 + ParamFile()->System_data.dTopCam20To2_Z;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
				if(LensNo== e20X_LENS)
				{
					ToPosition1= ToPosition1 + ParamFile()->System_data.dTopCam20To2_Y;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

// 				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, dispPos_Sel);
// 				ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, dispPos_Sel);
// 				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			}
			else
			{
				//-----------------
				strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까?"));
				if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
				//-----------------
				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
				ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel)-ParamFile()->Model_data.dStamp_Len_X;
				if(LensNo== e20X_LENS)
				{
					ToPosition1= ToPosition1 + ParamFile()->System_data.dTopCam20To2_X;
				}

				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel);
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_IMP_ROLL, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_IMP_ROLL] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Z, dispPos_Sel);
				if(LensNo== e20X_LENS)
				{
					ToPosition1= ToPosition1 + ParamFile()->System_data.dTopCam20To2_Z;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Z, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_UPPER_CAM_Y, dispPos_Sel);
				if(LensNo== e20X_LENS)
				{
					ToPosition1= ToPosition1 + ParamFile()->System_data.dTopCam20To2_Y;
				}
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_UPPER_CAM_Y, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_UPPER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				// 				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y1, dispPos_Sel);
				// 				ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_ALIGN_Y2, dispPos_Sel);
				// 				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2, ToPosition1, ToPosition2,	TRUE)) {	MyMessageBox(_T("[_ALING_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

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
	}
	else
	{
		if(Stage_Proc== ROLL_ALIGN_PROC)
		{
			double currZ1= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
			double currZ2= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
			double currBZ= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Z);
			double currBX= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_X);
			double currBY= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_LOWER_CAM_Y);
	//-----------------
			strText.Format(_T("선택된 축들을 설정 위치로 이동 하시겠습니까? \n"));
			if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
			//-----------------
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z1, POSITION_READY);
			ToPosition2	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL_Z2, POSITION_READY);
			if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, ToPosition1, ToPosition2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			//-----------------
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, 0.0, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
			//---------------------------------------------------------------------------------------------------------------
			dispPos_Sel= POSITION_ROLL_ALIGN;
			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_STAGE_X, dispPos_Sel);
			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition1,	FALSE)) {	MyMessageBox(_T("[_ACS_AXIS_STAGE_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

			if(MarkNo== eALIGN_1ST_MARK)
			{
				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel) ;
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
// 				if(!pSystem.GetMotionComponent()->BotVision_ReadyPos_Move(TRUE)) {	MyMessageBox(_T("[BOT_XYZ] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_ALIGN_Y1)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y1), _ACS_AXIS_ALIGN_Y1); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_ALIGN_Y2)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_ALIGN_Y2), _ACS_AXIS_ALIGN_Y2); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }

				if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)		{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_STAGE_X), _ACS_AXIS_STAGE_X); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_IMP_ROLL), _ACS_AXIS_IMP_ROLL); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		
		}
			else
			{
				//174.2
				double dRoll_Diameter = ParamFile()->System_data.dRoll_Diameter[ROLL_ALIGN_PROC] + ParamFile()->System_data.dRoll_Diameter_AddVal[ROLL_ALIGN_PROC];
				double RollFact= 360.0 / (dRoll_Diameter * PI);
				ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_IMP_ROLL, dispPos_Sel)+ (ParamFile()->Model_data.dStamp_Len_X*RollFact);
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
			}
// 			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_X, dispPos_Sel);
// 			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
// 
// 			ToPosition1	= pSystem.GetMotionComponent()->LoadPosdata(_ACS_AXIS_LOWER_CAM_Y, dispPos_Sel);
// 			if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, ToPosition1,	TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
	
			if(pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_STAGE_X) <= 1.0)
			{
				if(!pSystem.GetMotionComponent()->ABS_Move_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2, currZ1, currZ2,	FALSE)) {	MyMessageBox(_T("[_AXIS_ROLL_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_X, currBX, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_X] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Y, currBY, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Y] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(!pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_LOWER_CAM_Z, currBZ, TRUE)) {	MyMessageBox(_T("[_ACS_AXIS_LOWER_CAM_Z] 모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_LOWER_CAM_Z); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z1)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
				if(MoveEndCheck(_ACS_AXIS_IMP_ROLL_Z2)== FALSE) {	MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return; }
			}

// 			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_X)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_X), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
// 			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Y)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Y), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
// 			if(MoveEndCheck(_ACS_AXIS_LOWER_CAM_Z)== FALSE)	{	strText.Format(_T("모터 이동이 실패했습니다.\n %s [%d]"), pSystem.GetMotionComponent()->GetMotionName(_ACS_AXIS_LOWER_CAM_Z), _ACS_AXIS_UPPER_CAM_Y); MyMessageBox(strText.GetBuffer(0), _T("Position-Move Failed"), M_ICONERROR); return; }
		}
	}
}


void CfrmAlignView::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CfrmAlignView::OnBnClickedButtonU()
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

	double angle, posx, posy;
	ResetEditCtrl();
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}


void CfrmAlignView::OnBnClickedButtonD()
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

	double angle, posx, posy;
	ResetEditCtrl();
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}


void CfrmAlignView::OnBnClickedButtonL2()
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

	double angle, posx, posy;
	ResetEditCtrl();
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}


void CfrmAlignView::OnBnClickedButtonR2()
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

	ResetEditCtrl();
	double angle, posx, posy;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}



void CfrmAlignView::OnBnClickedButton9()
{
	ResetEditCtrl();
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	//
	CRect setROI;
	double tor_x_Val= 0.0, tor_y_Val= 0.0;
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	setROI.left= 0;	setROI.top= 0;	setROI.right= _INSP_IMAGE_SIZE_X;	setROI.bottom= _INSP_IMAGE_SIZE_Y;
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.0 && ParamFile()->Model_data.dLED_Pitch_Y> 0.0)
	{
/*
		if(CamNo== eTOP_CAM) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		setROI.left= 0;
		setROI.top= (_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val;
		setROI.right= _INSP_IMAGE_SIZE_X;
		setROI.bottom= (_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val;
*/
		find_ROI_Set_Flag= _FIRST_SELECT;
	}
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else	
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}


void CfrmAlignView::OnBnClickedButton10()
{
	ResetEditCtrl();
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	//
	CRect setROI;
	double tor_x_Val= 0.0, tor_y_Val= 0.0;
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	setROI.left= 0;	setROI.top= 0;	setROI.right= _INSP_IMAGE_SIZE_X;	setROI.bottom= _INSP_IMAGE_SIZE_Y;
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.0 && ParamFile()->Model_data.dLED_Pitch_Y> 0.0)
	{
/*
		if(CamNo== eTOP_CAM) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		setROI.left= 0;
		setROI.top= (_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val;
		setROI.right= _INSP_IMAGE_SIZE_X;
		setROI.bottom= (_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val;
*/
		find_ROI_Set_Flag= _CENTER_SELECT;
	}
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else	
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}


void CfrmAlignView::OnBnClickedButton11()
{
	ResetEditCtrl();
	double angle= 0.0, posx= 0.0, posy= 0.0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(CamNo== eTOP_CAM)
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
	}
	else
	{
		if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
		else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
	}
	pFrm->GetWindowHandler_CameraView()->CameraSeletUpdate();

	m_pVision->OverlayClear(m_radioCameraIdx);
	//
	CRect setROI;
	double tor_x_Val= 0.0, tor_y_Val= 0.0;
	int find_ROI_Set_Flag= _NORMAL_SELECT;
	setROI.left= 0;	setROI.top= 0;	setROI.right= _INSP_IMAGE_SIZE_X;	setROI.bottom= _INSP_IMAGE_SIZE_Y;
	if(ParamFile()->Model_data.dLED_Pitch_X > 0.0 && ParamFile()->Model_data.dLED_Pitch_Y> 0.0)
	{
/*
		if(CamNo== eTOP_CAM) 
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dTop_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		else
		{
			tor_x_Val= (ParamFile()->Model_data.dLED_Pitch_X / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
			tor_y_Val= (ParamFile()->Model_data.dLED_Pitch_Y / (ParamFile()->System_data.dBot_Cam_PixelToum_2X / 1000.0) /2.0 * 1.5);
		}
		setROI.left= 0;
		setROI.top= (_INSP_IMAGE_SIZE_Y / 2.0) - tor_y_Val;
		setROI.right= _INSP_IMAGE_SIZE_X;
		setROI.bottom= (_INSP_IMAGE_SIZE_Y / 2.0) + tor_y_Val;
*/
		find_ROI_Set_Flag= _LAST_SELECT;
	}
	if(ParamFile()->Model_data.iMarkSearchCount[Stage_Proc]) //Stage_Proc= STAGE_1_ALIGN_PROC)
		m_pVision->PatternFinderPosAll(find_ROI_Set_Flag, &angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
	else	
		m_pVision->PatternFinderPos(&angle, &posx, &posy, ModelNo, m_radioCameraIdx, true, true, true);
}


void CfrmAlignView::ClickMarkRoiSel5()
{
	ResetEditCtrl();
	UpdateData( TRUE );
	m_ROI_Mode= false;
	m_btnROI_Set.SetValue(FALSE);

	if(LensNo== e20X_LENS)
	{
		m_bAreaROI= FALSE;
		m_btnAreaROI.SetValue(FALSE);
		return;
	}

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(m_btnAreaROI.GetValue())
	{
		m_bAreaROI= true;
		if(CamNo== eTOP_CAM)
		{
			if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e2X_LENS;
			else					pFrm->GetWindowHandler_CameraView()->mTopCameraNo= e20X_LENS;
		}
		else
		{
			if(LensNo== e2X_LENS)	pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e2X_LENS;
			else					pFrm->GetWindowHandler_CameraView()->mBotCameraNo= e20X_LENS;
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
