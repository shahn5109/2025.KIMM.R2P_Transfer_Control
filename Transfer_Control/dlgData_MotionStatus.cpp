// dlgData_MotionStatus.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgData_MotionStatus.h"
#include "afxdialogex.h"

extern CSystemClass pSystem;
CParameterFile* CdlgData_MotionStatus::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgData_MotionStatus::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgData_MotionStatus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgData_MotionStatus, CDialogEx)

	CdlgData_MotionStatus::CdlgData_MotionStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgData_MotionStatus::IDD, pParent)
{

}

CdlgData_MotionStatus::~CdlgData_MotionStatus()
{
}

void CdlgData_MotionStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_STATIC_RDY_0 , m_btnRDY[0 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_1 , m_btnRDY[1 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_2 , m_btnRDY[2 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_3 , m_btnRDY[3 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_4 , m_btnRDY[4 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_5 , m_btnRDY[5 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_6 , m_btnRDY[6 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_7 , m_btnRDY[7 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_8 , m_btnRDY[8 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_9 , m_btnRDY[9 ]);
	DDX_Control(pDX, IDS_STATIC_RDY_10, m_btnRDY[10]);
	DDX_Control(pDX, IDS_STATIC_RDY_11, m_btnRDY[11]);
	DDX_Control(pDX, IDS_STATIC_RDY_12, m_btnRDY[12]);
	DDX_Control(pDX, IDS_STATIC_RDY_13, m_btnRDY[13]);
	DDX_Control(pDX, IDS_STATIC_RDY_14, m_btnRDY[14]);
	DDX_Control(pDX, IDS_STATIC_RDY_15, m_btnRDY[15]);

	DDX_Control(pDX, IDS_STATIC_SVRON_0 ,m_btnSvrOn[0 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_1 ,m_btnSvrOn[1 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_2 ,m_btnSvrOn[2 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_3 ,m_btnSvrOn[3 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_4 ,m_btnSvrOn[4 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_5 ,m_btnSvrOn[5 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_6 ,m_btnSvrOn[6 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_7 ,m_btnSvrOn[7 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_8 ,m_btnSvrOn[8 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_9 ,m_btnSvrOn[9 ]);
	DDX_Control(pDX, IDS_STATIC_SVRON_10,m_btnSvrOn[10]);
	DDX_Control(pDX, IDS_STATIC_SVRON_11,m_btnSvrOn[11]);
	DDX_Control(pDX, IDS_STATIC_SVRON_12,m_btnSvrOn[12]);
	DDX_Control(pDX, IDS_STATIC_SVRON_13,m_btnSvrOn[13]);
	DDX_Control(pDX, IDS_STATIC_SVRON_14,m_btnSvrOn[14]);
	DDX_Control(pDX, IDS_STATIC_SVRON_15,m_btnSvrOn[15]);

	DDX_Control(pDX, IDS_STATIC_PLIM_0 ,m_btnPlim[0 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_1 ,m_btnPlim[1 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_2 ,m_btnPlim[2 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_3 ,m_btnPlim[3 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_4 ,m_btnPlim[4 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_5 ,m_btnPlim[5 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_6 ,m_btnPlim[6 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_7 ,m_btnPlim[7 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_8 ,m_btnPlim[8 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_9 ,m_btnPlim[9 ]);
	DDX_Control(pDX, IDS_STATIC_PLIM_10,m_btnPlim[10]);
	DDX_Control(pDX, IDS_STATIC_PLIM_11,m_btnPlim[11]);
	DDX_Control(pDX, IDS_STATIC_PLIM_12,m_btnPlim[12]);
	DDX_Control(pDX, IDS_STATIC_PLIM_13,m_btnPlim[13]);
	DDX_Control(pDX, IDS_STATIC_PLIM_14,m_btnPlim[14]);
	DDX_Control(pDX, IDS_STATIC_PLIM_15,m_btnPlim[15]);

	DDX_Control(pDX, IDS_STATIC_NLIM_0 ,m_btnNlim[0 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_1 ,m_btnNlim[1 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_2 ,m_btnNlim[2 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_3 ,m_btnNlim[3 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_4 ,m_btnNlim[4 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_5 ,m_btnNlim[5 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_6 ,m_btnNlim[6 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_7 ,m_btnNlim[7 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_8 ,m_btnNlim[8 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_9 ,m_btnNlim[9 ]);
	DDX_Control(pDX, IDS_STATIC_NLIM_10,m_btnNlim[10]);
	DDX_Control(pDX, IDS_STATIC_NLIM_11,m_btnNlim[11]);
	DDX_Control(pDX, IDS_STATIC_NLIM_12,m_btnNlim[12]);
	DDX_Control(pDX, IDS_STATIC_NLIM_13,m_btnNlim[13]);
	DDX_Control(pDX, IDS_STATIC_NLIM_14,m_btnNlim[14]);
	DDX_Control(pDX, IDS_STATIC_NLIM_15,m_btnNlim[15]);

	DDX_Control(pDX, IDS_STATIC_ORG_0 ,m_btnOrg[0 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_1 ,m_btnOrg[1 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_2 ,m_btnOrg[2 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_3 ,m_btnOrg[3 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_4 ,m_btnOrg[4 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_5 ,m_btnOrg[5 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_6 ,m_btnOrg[6 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_7 ,m_btnOrg[7 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_8 ,m_btnOrg[8 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_9 ,m_btnOrg[9 ]);
	DDX_Control(pDX, IDS_STATIC_ORG_10,m_btnOrg[10]);
	DDX_Control(pDX, IDS_STATIC_ORG_11,m_btnOrg[11]);
	DDX_Control(pDX, IDS_STATIC_ORG_12,m_btnOrg[12]);
	DDX_Control(pDX, IDS_STATIC_ORG_13,m_btnOrg[13]);
	DDX_Control(pDX, IDS_STATIC_ORG_14,m_btnOrg[14]);
	DDX_Control(pDX, IDS_STATIC_ORG_15,m_btnOrg[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_0, m_dSW_Limit_High[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_1, m_dSW_Limit_High[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_2, m_dSW_Limit_High[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_3, m_dSW_Limit_High[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_4, m_dSW_Limit_High[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_5, m_dSW_Limit_High[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_6, m_dSW_Limit_High[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_7, m_dSW_Limit_High[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_8, m_dSW_Limit_High[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_9, m_dSW_Limit_High[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_10, m_dSW_Limit_High[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_11, m_dSW_Limit_High[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_12, m_dSW_Limit_High[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_13, m_dSW_Limit_High[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_14, m_dSW_Limit_High[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_15, m_dSW_Limit_High[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_0 , m_dSW_Limit_Low[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_1 , m_dSW_Limit_Low[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_2 , m_dSW_Limit_Low[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_3 , m_dSW_Limit_Low[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_4 , m_dSW_Limit_Low[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_5 , m_dSW_Limit_Low[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_6 , m_dSW_Limit_Low[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_7 , m_dSW_Limit_Low[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_8 , m_dSW_Limit_Low[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_9 , m_dSW_Limit_Low[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_10, m_dSW_Limit_Low[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_11, m_dSW_Limit_Low[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_12, m_dSW_Limit_Low[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_13, m_dSW_Limit_Low[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_14, m_dSW_Limit_Low[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_15, m_dSW_Limit_Low[15]);
}


BEGIN_MESSAGE_MAP(CdlgData_MotionStatus, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CdlgData_MotionStatus 메시지 처리기입니다.
BOOL CdlgData_MotionStatus::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}
BOOL CdlgData_MotionStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DataDispUpdate();
	DataUpdate();
	SetTimer(dlgData_MotionStatus_TIMER_ID,	dlgData_MotionStatus_TIMER_INTERVAL, NULL);

#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	SetDlgItemText(IDS_TITLE_AXIS_0, _T("0: Stage X축"));
	SetDlgItemText(IDS_TITLE_AXIS_1, _T("1: Sync. Scale"));
	SetDlgItemText(IDS_TITLE_AXIS_2, _T("2: Impressing - Roll"));
	SetDlgItemText(IDS_TITLE_AXIS_3, _T("3: "));
	SetDlgItemText(IDS_TITLE_AXIS_4, _T("4: 상부-Vision Z축"));
	SetDlgItemText(IDS_TITLE_AXIS_5, _T("5: "));
	SetDlgItemText(IDS_TITLE_AXIS_6, _T("6: "));
	SetDlgItemText(IDS_TITLE_AXIS_7, _T("7: "));
	SetDlgItemText(IDS_TITLE_AXIS_8, _T("8: 상부-Vision Y축"));
	SetDlgItemText(IDS_TITLE_AXIS_9, _T("9: 하부 - 비젼 X축"));
	SetDlgItemText(IDS_TITLE_AXIS_10, _T("10: 하부 - 비젼 Y축"));
	SetDlgItemText(IDS_TITLE_AXIS_11, _T("11: 하부 - 비젼 Z축"));
	SetDlgItemText(IDS_TITLE_AXIS_12, _T("12: Align-Stage Y1축"));
	SetDlgItemText(IDS_TITLE_AXIS_13, _T("13: Align-Stage Y2축"));
	SetDlgItemText(IDS_TITLE_AXIS_14, _T("14: Impressing-Roll Z1축"));
	SetDlgItemText(IDS_TITLE_AXIS_15, _T("15: Impressing-Roll Z2축"));
#else
	SetDlgItemText(IDS_TITLE_AXIS_0, _T("0: Stage X축"));
	SetDlgItemText(IDS_TITLE_AXIS_1, _T("1: Sync. Scale"));
	SetDlgItemText(IDS_TITLE_AXIS_2, _T("2: Impressing - Roll"));
	SetDlgItemText(IDS_TITLE_AXIS_3, _T("3: "));
	SetDlgItemText(IDS_TITLE_AXIS_4, _T("4: 상부-Vision Y축"));
	SetDlgItemText(IDS_TITLE_AXIS_5, _T("5: 하부 - 비젼 X축"));
	SetDlgItemText(IDS_TITLE_AXIS_6, _T("6: 하부 - 비젼 Y축"));
	SetDlgItemText(IDS_TITLE_AXIS_7, _T("7: 하부 - 비젼 Z축"));
	SetDlgItemText(IDS_TITLE_AXIS_8, _T("8: 상부-Vision Z축"));
	SetDlgItemText(IDS_TITLE_AXIS_9, _T("9: "));
	SetDlgItemText(IDS_TITLE_AXIS_10, _T("10: Align-Stage Y1축"));
	SetDlgItemText(IDS_TITLE_AXIS_11, _T("11: Align-Stage Y2축"));
	SetDlgItemText(IDS_TITLE_AXIS_12, _T("12: Impressing-Roll Z1축"));
	SetDlgItemText(IDS_TITLE_AXIS_13, _T("13: Impressing-Roll Z2축"));
	SetDlgItemText(IDS_TITLE_AXIS_14, _T("14: "));
	SetDlgItemText(IDS_TITLE_AXIS_15, _T("15: "));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//===================================================================================

void CdlgData_MotionStatus::OnDestroy() 
{
	CDialog::OnDestroy();

	KillTimer(dlgData_MotionStatus_TIMER_ID);

}
//===================================================================================

HBRUSH CdlgData_MotionStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
//===================================================================================

void CdlgData_MotionStatus::OnTimer(UINT nIDEvent) 
{
	DataDispUpdate();

	CDialog::OnTimer(nIDEvent);
}
//===================================================================================

BEGIN_EVENTSINK_MAP(CdlgData_MotionStatus, CDialog)
	//{{AFX_EVENTSINK_MAP(CdlgData_MotionStatus)
	ON_EVENT(CdlgData_MotionStatus, IDB_EXIT, -600 /* Click */, CdlgData_MotionStatus::OnClickExit1, VTS_NONE)
	ON_EVENT(CdlgData_MotionStatus, IDB_DATA_SAVE, -600 /* Click */, CdlgData_MotionStatus::OnClickDataSave, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//===================================================================================

void CdlgData_MotionStatus::OnClickExit1() 
{
	OnOK();	
}
//===================================================================================

void CdlgData_MotionStatus::DataDispUpdate()
{
	int i;
	for(i= 0; i< _MAX_AXIS; i++)
	{
		//		if(i== 5 || i== 7 || i== 16 || i== 17 || i== 18 || i== 19 || i==26 || i== 27) continue;
		if(pSystem.GetMotionComponent()->isORG_SensorCheck(i))		{		if(!m_btnOrg[i].GetValue())		m_btnOrg[i].SetValue(1);	}
		else														{		if(m_btnOrg[i].GetValue())		m_btnOrg[i].SetValue(0);	}

		if(pSystem.GetMotionComponent()->isServoOn(i))				{		if(!m_btnSvrOn[i].GetValue())	m_btnSvrOn[i].SetValue(TRUE);		}
		else														{		if(m_btnSvrOn[i].GetValue())	m_btnSvrOn[i].SetValue(FALSE);		}
		if(pSystem.GetMotionComponent()->isELP_SensorCheck(i))		{		if(!m_btnPlim[i].GetValue())	m_btnPlim[i].SetValue(TRUE);		}
		else														{		if(m_btnPlim[i].GetValue())		m_btnPlim[i].SetValue(FALSE);			}
		if(pSystem.GetMotionComponent()->isELN_SensorCheck(i))		{		if(!m_btnNlim[i].GetValue())	m_btnNlim[i].SetValue(TRUE);		}
		else														{		if(m_btnNlim[i].GetValue())		m_btnNlim[i].SetValue(FALSE);		}

		if(i>= 4 && i<= 7)
		{
			if(pSystem.GetMotionComponent()->isMotion_AlarmCheck(i))	{	if(m_btnRDY[i].GetValue())	m_btnRDY[i].SetValue(0);	}
			else														{	if(!m_btnRDY[i].GetValue())	m_btnRDY[i].SetValue(1);	}
		}
		else
		{
			// 			m_btnRDY[i].SetValue(pSystem.GetMotionComponent()->isMotion_AlarmCheck(i));//isMotion_ReadyCheck(i, false)); //Ready 시 TRUE리턴
			if(pSystem.GetMotionComponent()->isMotion_AlarmCheck(i))	{	if(!m_btnRDY[i].GetValue())	m_btnRDY[i].SetValue(1);	}
			else														{	if(m_btnRDY[i].GetValue())	m_btnRDY[i].SetValue(0);	}
		}
		// 		if(pSystem.GetMotionComponent()->isMotion_AlarmCheck(i)) m_btnRDY[i].SetValue(FALSE);
		// 		else	m_btnRDY[i].SetValue(TRUE);
		//isMotion_AlarmCheck(long axis) //알람 시 TRUE리턴
	}
}//5,7,16,17,24,25
//===================================================================================
void CdlgData_MotionStatus::OnClickDataSave() 
{
	if (MyMessageBox(_T("입력된 값을 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgData_MotionSpeed : OnClickDataSave.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgData_MotionSpeed : OnClickDataSave.!"));
	DataSave();
	DataUpdate();
}
//===================================================================================
void CdlgData_MotionStatus::DataUpdate()
	//-----------------------------------------------------------------------------------
{
	//	char cBeforeVal[20];
	CString strTmp;

	ParamFile()->OpenSytemConfigFile();//ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadMotionParamFile();//ParamFile()->System_data.strCurrentModelName);

	for(int i= 0; i< _MAX_AXIS; i++)
	{
		m_dSW_Limit_High[i] =			ParamFile()->Motion_data.dSW_Limit_High[i];
		m_dSW_Limit_Low[i] =			ParamFile()->Motion_data.dSW_Limit_Low[i];
	}

	UpdateData(FALSE);
}
//===================================================================================
void CdlgData_MotionStatus::DataSave()
	//-----------------------------------------------------------------------------------
{
	UpdateData(TRUE);

	m_dSW_Limit_High[_ACS_AXIS_IMP_ROLL_Z2]= m_dSW_Limit_High[_ACS_AXIS_IMP_ROLL_Z1];
	m_dSW_Limit_Low[_ACS_AXIS_IMP_ROLL_Z2]= m_dSW_Limit_Low[_ACS_AXIS_IMP_ROLL_Z1];
	m_dSW_Limit_High[_ACS_AXIS_ALIGN_Y2]= m_dSW_Limit_High[_ACS_AXIS_ALIGN_Y1];
	m_dSW_Limit_Low[_ACS_AXIS_ALIGN_Y2]= m_dSW_Limit_Low[_ACS_AXIS_ALIGN_Y1];

	for(int i= 0; i< _MAX_AXIS; i++)
	{
		ParamFile()->Motion_data.dSW_Limit_High[i]= m_dSW_Limit_High[i];	
		ParamFile()->Motion_data.dSW_Limit_Low[i]= m_dSW_Limit_Low[i];
	}

	Log()->SaveLogFile(_SYSTEM_LOG, _T("[ SAVE ] Motion OriginSpeed Data 수정 / 저장"));
	ParamFile()->SaveSytemConfigFile();//ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveMotionParamFile();//ParamFile()->System_data.strCurrentModelName);
}
