// dlgData_MotionSpeed.cpp : ±∏«ˆ ∆ƒ¿œ¿‘¥œ¥Ÿ.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgData_MotionSpeed.h"
#include "afxdialogex.h"

extern CSystemClass pSystem;
CParameterFile* CdlgData_MotionSpeed::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgData_MotionSpeed::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgData_MotionSpeed ¥Î»≠ ªÛ¿⁄¿‘¥œ¥Ÿ.

IMPLEMENT_DYNAMIC(CdlgData_MotionSpeed, CDialogEx)

CdlgData_MotionSpeed::CdlgData_MotionSpeed(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgData_MotionSpeed::IDD, pParent)
{
	for(int i= 0; i< _MAX_AXIS; i++)
	{
		m_dWork_Speed[i] =			0.0;
		m_dWork_Accel[i] =			0.0;
		m_dOrigin_Speed[i] =		0.0;

		m_dJog_MaxSpeed[i] =		0.0;
		m_dJog_MidSpeed[i] =		0.0;
		m_dJog_MinSpeed[i] =		0.0;

		m_dPulsePerMM[i] =			0.0;
	}
}

CdlgData_MotionSpeed::~CdlgData_MotionSpeed()
{
}

void CdlgData_MotionSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_0, m_dOrigin_Speed[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_1, m_dOrigin_Speed[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_2, m_dOrigin_Speed[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_3, m_dOrigin_Speed[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_4, m_dOrigin_Speed[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_5, m_dOrigin_Speed[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_6, m_dOrigin_Speed[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_7, m_dOrigin_Speed[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_8, m_dOrigin_Speed[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_9, m_dOrigin_Speed[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_10, m_dOrigin_Speed[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_11, m_dOrigin_Speed[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_12, m_dOrigin_Speed[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_13, m_dOrigin_Speed[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_14, m_dOrigin_Speed[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ORG_15, m_dOrigin_Speed[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_0, m_dWork_Speed[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_1, m_dWork_Speed[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_2, m_dWork_Speed[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_3, m_dWork_Speed[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_4, m_dWork_Speed[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_5, m_dWork_Speed[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_6, m_dWork_Speed[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_7, m_dWork_Speed[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_8, m_dWork_Speed[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_9, m_dWork_Speed[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_10, m_dWork_Speed[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_11, m_dWork_Speed[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_12, m_dWork_Speed[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_13, m_dWork_Speed[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_14, m_dWork_Speed[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_SPEED_15, m_dWork_Speed[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_0 , m_dWork_Accel[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_1 , m_dWork_Accel[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_2 , m_dWork_Accel[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_3 , m_dWork_Accel[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_4 , m_dWork_Accel[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_5 , m_dWork_Accel[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_6 , m_dWork_Accel[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_7 , m_dWork_Accel[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_8 , m_dWork_Accel[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_9 , m_dWork_Accel[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_10, m_dWork_Accel[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_11, m_dWork_Accel[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_12, m_dWork_Accel[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_13, m_dWork_Accel[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_14, m_dWork_Accel[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_ACCDEC_15, m_dWork_Accel[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_0, m_dJog_MaxSpeed[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_1, m_dJog_MaxSpeed[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_2, m_dJog_MaxSpeed[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_3, m_dJog_MaxSpeed[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_4, m_dJog_MaxSpeed[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_5, m_dJog_MaxSpeed[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_6, m_dJog_MaxSpeed[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_7, m_dJog_MaxSpeed[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_8, m_dJog_MaxSpeed[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_9, m_dJog_MaxSpeed[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_10, m_dJog_MaxSpeed[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_11, m_dJog_MaxSpeed[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_12, m_dJog_MaxSpeed[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_13, m_dJog_MaxSpeed[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_14, m_dJog_MaxSpeed[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_H_15, m_dJog_MaxSpeed[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_0, m_dJog_MidSpeed[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_1, m_dJog_MidSpeed[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_2, m_dJog_MidSpeed[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_3, m_dJog_MidSpeed[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_4, m_dJog_MidSpeed[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_5, m_dJog_MidSpeed[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_6, m_dJog_MidSpeed[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_7, m_dJog_MidSpeed[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_8, m_dJog_MidSpeed[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_9, m_dJog_MidSpeed[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_10, m_dJog_MidSpeed[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_11, m_dJog_MidSpeed[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_12, m_dJog_MidSpeed[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_13, m_dJog_MidSpeed[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_14, m_dJog_MidSpeed[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_M_15, m_dJog_MidSpeed[15]);

	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_0, m_dJog_MinSpeed[0]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_1, m_dJog_MinSpeed[1]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_2, m_dJog_MinSpeed[2]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_3, m_dJog_MinSpeed[3]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_4, m_dJog_MinSpeed[4]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_5, m_dJog_MinSpeed[5]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_6, m_dJog_MinSpeed[6]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_7, m_dJog_MinSpeed[7]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_8, m_dJog_MinSpeed[8]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_9, m_dJog_MinSpeed[9]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_10, m_dJog_MinSpeed[10]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_11, m_dJog_MinSpeed[11]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_12, m_dJog_MinSpeed[12]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_13, m_dJog_MinSpeed[13]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_14, m_dJog_MinSpeed[14]);
	DDX_Text(pDX,  IDC_EDIT_MOVE_JOG_S_15, m_dJog_MinSpeed[15]);

	DDX_Text(pDX,  IDC_EDIT_UNIT_0, m_dPulsePerMM[0]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_1, m_dPulsePerMM[1]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_2, m_dPulsePerMM[2]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_3, m_dPulsePerMM[3]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_4, m_dPulsePerMM[4]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_5, m_dPulsePerMM[5]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_6, m_dPulsePerMM[6]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_7, m_dPulsePerMM[7]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_8, m_dPulsePerMM[8]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_9, m_dPulsePerMM[9]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_10, m_dPulsePerMM[10]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_11, m_dPulsePerMM[11]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_12, m_dPulsePerMM[12]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_13, m_dPulsePerMM[13]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_14, m_dPulsePerMM[14]);
	DDX_Text(pDX,  IDC_EDIT_UNIT_15, m_dPulsePerMM[15]);
}


BEGIN_MESSAGE_MAP(CdlgData_MotionSpeed, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CdlgData_MotionSpeed ∏ﬁΩ√¡ˆ √≥∏Æ±‚¿‘¥œ¥Ÿ.
BOOL CdlgData_MotionSpeed::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){ 
		if((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN)){          return TRUE;        }   
	}
	return CDialog::PreTranslateMessage(pMsg);
}
BOOL CdlgData_MotionSpeed::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

/*
	CFont font;
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_0)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_1)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_2)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_3)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_4)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_5)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_6)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_7)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_8)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_9)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_10)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_11)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_12)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_13)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_14)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_15)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_16)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_17)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_18)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_19)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_20)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_21)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_22)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_23)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_24)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_25)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_26)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_27)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_28)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_29)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_30)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_31)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_32)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_SPEED_33)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();

	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_0)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_1)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_2)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_3)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_4)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_5)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_6)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_7)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_8)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_9)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_10)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_11)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_12)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_13)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_14)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_15)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_16)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_17)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_18)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_19)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_20)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_21)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_22)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_23)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_24)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_25)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_26)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_27)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_28)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_29)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_30)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_31)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_32)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	font.CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, "±º∏≤");	GetDlgItem(IDC_EDIT_MOVE_ACCDEC_33)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
*/

	DataUpdate();

#ifdef _LGE_TRANSFER_VER1_SYSTEM_INFO_H
	SetDlgItemText(IDS_TITLE_AXIS_0, _T("0: Stage X√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_1, _T("1: Sync. Scale"));
	SetDlgItemText(IDS_TITLE_AXIS_2, _T("2: Impressing - Roll"));
	SetDlgItemText(IDS_TITLE_AXIS_3, _T("3: "));
	SetDlgItemText(IDS_TITLE_AXIS_4, _T("4: ªÛ∫Œ-Vision Z√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_5, _T("5: "));
	SetDlgItemText(IDS_TITLE_AXIS_6, _T("6: "));
	SetDlgItemText(IDS_TITLE_AXIS_7, _T("7: "));
	SetDlgItemText(IDS_TITLE_AXIS_8, _T("8: ªÛ∫Œ-Vision Y√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_9, _T("9: «œ∫Œ - ∫Ò¡Ø X√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_10, _T("10: «œ∫Œ - ∫Ò¡Ø Y√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_11, _T("11: «œ∫Œ - ∫Ò¡Ø Z√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_12, _T("12: Align-Stage Y1√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_13, _T("13: Align-Stage Y2√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_14, _T("14: Impressing-Roll Z1√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_15, _T("15: Impressing-Roll Z2√‡"));
#else
	SetDlgItemText(IDS_TITLE_AXIS_0, _T("0: Stage X√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_1, _T("1: Sync. Scale"));
	SetDlgItemText(IDS_TITLE_AXIS_2, _T("2: Impressing - Roll"));
	SetDlgItemText(IDS_TITLE_AXIS_3, _T("3: "));
	SetDlgItemText(IDS_TITLE_AXIS_4, _T("4: ªÛ∫Œ-Vision Y√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_5, _T("5: «œ∫Œ - ∫Ò¡Ø X√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_6, _T("6: «œ∫Œ - ∫Ò¡Ø Y√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_7, _T("7: «œ∫Œ - ∫Ò¡Ø Z√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_8, _T("8: ªÛ∫Œ-Vision Z√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_9, _T("9: "));
	SetDlgItemText(IDS_TITLE_AXIS_10, _T("10: Align-Stage Y1√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_11, _T("11: Align-Stage Y2√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_12, _T("12: Impressing-Roll Z1√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_13, _T("13: Impressing-Roll Z2√‡"));
	SetDlgItemText(IDS_TITLE_AXIS_14, _T("14: "));
	SetDlgItemText(IDS_TITLE_AXIS_15, _T("15: "));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//===================================================================================
void CdlgData_MotionSpeed::OnDestroy() 
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here

}
//===================================================================================
HBRUSH CdlgData_MotionSpeed::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//===================================================================================
void CdlgData_MotionSpeed::DataUpdate()
	//-----------------------------------------------------------------------------------
{
	//	char cBeforeVal[20];
	CString strTmp;

	ParamFile()->OpenSytemConfigFile();//ParamFile()->System_data.strCurrentModelName);
	ParamFile()->ReadMotionParamFile();//ParamFile()->System_data.strCurrentModelName);

	for(int i= 0; i< _MAX_AXIS; i++)
	{
		m_dWork_Speed[i] =			ParamFile()->Motion_data.dWork_Speed[i];
		m_dWork_Accel[i] =			ParamFile()->Motion_data.dWork_Accel[i];


		m_dOrigin_Speed[i] =		ParamFile()->Motion_data.dOrigin_Speed[i];
		m_dJog_MaxSpeed[i] =		ParamFile()->Motion_data.dJog_MaxSpeed[i];
		m_dJog_MidSpeed[i] =		ParamFile()->Motion_data.dJog_MidSpeed[i];
		m_dJog_MinSpeed[i] =		ParamFile()->Motion_data.dJog_MinSpeed[i];
		m_dPulsePerMM[i] =			ParamFile()->Motion_data.dPulsePerMM[i];

	}

	UpdateData(FALSE);
}
//===================================================================================
void CdlgData_MotionSpeed::DataSave()
	//-----------------------------------------------------------------------------------
{
	UpdateData(TRUE);

	m_dOrigin_Speed[_ACS_AXIS_IMP_ROLL_Z2]= m_dOrigin_Speed[_ACS_AXIS_IMP_ROLL_Z1];
	m_dOrigin_Speed[_ACS_AXIS_ALIGN_Y2]= m_dOrigin_Speed[_ACS_AXIS_ALIGN_Y1];

	m_dWork_Speed[_ACS_AXIS_IMP_ROLL_Z2]= m_dWork_Speed[_ACS_AXIS_IMP_ROLL_Z1];
	m_dWork_Speed[_ACS_AXIS_ALIGN_Y2]= m_dWork_Speed[_ACS_AXIS_ALIGN_Y1];

	m_dWork_Accel[_ACS_AXIS_IMP_ROLL_Z2]= m_dWork_Accel[_ACS_AXIS_IMP_ROLL_Z1];
	m_dWork_Accel[_ACS_AXIS_ALIGN_Y2]= m_dWork_Accel[_ACS_AXIS_ALIGN_Y1];

	m_dPulsePerMM[_ACS_AXIS_IMP_ROLL_Z2]= m_dPulsePerMM[_ACS_AXIS_IMP_ROLL_Z1];
	m_dPulsePerMM[_ACS_AXIS_ALIGN_Y2]= m_dPulsePerMM[_ACS_AXIS_ALIGN_Y1];

	m_dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z2]= m_dJog_MaxSpeed[_ACS_AXIS_IMP_ROLL_Z1];
	m_dJog_MaxSpeed[_ACS_AXIS_ALIGN_Y2]= m_dJog_MaxSpeed[_ACS_AXIS_ALIGN_Y1];
	m_dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z2]= m_dJog_MidSpeed[_ACS_AXIS_IMP_ROLL_Z1];
	m_dJog_MidSpeed[_ACS_AXIS_ALIGN_Y2]= m_dJog_MidSpeed[_ACS_AXIS_ALIGN_Y1];
	m_dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z2]= m_dJog_MinSpeed[_ACS_AXIS_IMP_ROLL_Z1];
	m_dJog_MinSpeed[_ACS_AXIS_ALIGN_Y2]= m_dJog_MinSpeed[_ACS_AXIS_ALIGN_Y1];

	for(int i= 0; i< _MAX_AXIS; i++)
	{
		ParamFile()->Motion_data.dStart_Speed[i]= 0.0;	
		ParamFile()->Motion_data.dWork_Speed[i]= m_dWork_Speed[i];	
		ParamFile()->Motion_data.dWork_Decel[i]= ParamFile()->Motion_data.dWork_Accel[i]= m_dWork_Accel[i];

		ParamFile()->Motion_data.dOrigin_Speed[i]= m_dOrigin_Speed[i];
		ParamFile()->Motion_data.dOrigin_Decel[i]= ParamFile()->Motion_data.dOrigin_Accel[i]= m_dWork_Accel[i];

		ParamFile()->Motion_data.dJog_MaxSpeed[i]= m_dJog_MaxSpeed[i];	
		ParamFile()->Motion_data.dJog_MidSpeed[i]= m_dJog_MidSpeed[i];	
		ParamFile()->Motion_data.dJog_MinSpeed[i]= m_dJog_MinSpeed[i];	

		ParamFile()->Motion_data.dPulsePerMM[i]= m_dPulsePerMM[i];	
	}

	Log()->SaveLogFile(_SYSTEM_LOG,_T("[ SAVE ] Motion OriginSpeed Data ºˆ¡§ / ¿˙¿Â"));
	ParamFile()->SaveSytemConfigFile();//ParamFile()->System_data.strCurrentModelName);
	ParamFile()->SaveMotionParamFile();//ParamFile()->System_data.strCurrentModelName);
}

//===================================================================================
BEGIN_EVENTSINK_MAP(CdlgData_MotionSpeed, CDialogEx)
	//{{AFX_EVENTSINK_MAP(CdlgData_MotionSpeed)
	ON_EVENT(CdlgData_MotionSpeed, IDB_DATA_SAVE, -600 /* Click */, CdlgData_MotionSpeed::OnClickDataSave, VTS_NONE)
	ON_EVENT(CdlgData_MotionSpeed, IDB_EXIT, -600 /* Click */, CdlgData_MotionSpeed::OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//===================================================================================
void CdlgData_MotionSpeed::OnClickDataSave() 
{
	if (MyMessageBox(_T("¿‘∑¬µ» ∞™¿ª ¿˙¿Â «œΩ√∞⁄Ω¿¥œ±Ó? ."), _T("»Æ¿Œ"), M_ICONQUESTION|M_YESNO, _T("øπ"), _T("æ∆¥œø¿")) == IDNO) 
	{
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgData_MotionSpeed : OnClickDataSave.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => CdlgData_MotionSpeed : OnClickDataSave.!"));
	DataSave();
	DataUpdate();
}
//===================================================================================
void CdlgData_MotionSpeed::OnClickExit() 
	//-----------------------------------------------------------------------------------
{
	OnOK();
}
