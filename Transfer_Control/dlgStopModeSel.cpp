// dlgStopModeSel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgStopModeSel.h"
#include "afxdialogex.h"

extern CSystemClass pSystem;
CParameterFile* CdlgStopModeSel::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgStopModeSel::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgStopModeSel 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgStopModeSel, CDialogEx)

CdlgStopModeSel::CdlgStopModeSel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgStopModeSel::IDD, pParent)
{

}

CdlgStopModeSel::~CdlgStopModeSel()
{
}

void CdlgStopModeSel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_PAUSE, m_btnStartNPause);
}


BEGIN_MESSAGE_MAP(CdlgStopModeSel, CDialogEx)
END_MESSAGE_MAP()


// CdlgStopModeSel 메시지 처리기입니다.
BOOL CdlgStopModeSel::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(	ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_PAUSE) {
		CString strTemp;
		//		char pBuf[MAX_PATH] = {0};
		//		strcpy(pBuf, "%s\\Bin\\Icon", _PROGRAM_ROOT_DIR);
		strTemp.Format(_T("%s\\Bin\\Icon\\gear_run.ico"), _PROGRAM_ROOT_DIR);
		m_btnStartNPause.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);
		m_btnStartNPause.SetCaption(_T("재 시작"));
	}
	else {
		CString strTemp;
		//		char pBuf[MAX_PATH] = {0};
		strTemp.Format(_T("%s\\Bin\\Icon\\gear_time.ico"), _PROGRAM_ROOT_DIR);
		m_btnStartNPause.SetPicture(strTemp); //	m_btnReady.SetPicturePosition(10);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CdlgStopModeSel, CDialog)
	//{{AFX_EVENTSINK_MAP(CdlgStopModeSel)
	ON_EVENT(CdlgStopModeSel, IDB_END_STOP, -600 /* Click */, CdlgStopModeSel::OnClickEndStop, VTS_NONE)
	ON_EVENT(CdlgStopModeSel, IDB_PAUSE, -600 /* Click */, CdlgStopModeSel::OnClickPause, VTS_NONE)
	ON_EVENT(CdlgStopModeSel, IDB_END, -600 /* Click */, CdlgStopModeSel::OnClickEnd, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CdlgStopModeSel::OnClickEndStop() 
{
	ParamFile()->System_data.bEND_STOP_FLAG= _CURRENT_ENDSTOP_ENA;
	OnOK();
}

void CdlgStopModeSel::OnClickPause() 
{
	if(	ParamFile()->System_data.iCURRENT_RUN_STATUS== _CURRENT_RUN_PAUSE) {
		ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_RUN;
	}
	else {
		ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_PAUSE;
	}
	OnOK();
}

void CdlgStopModeSel::OnClickEnd() 
{
	ParamFile()->System_data.iCURRENT_RUN_STATUS= _CURRENT_RUN_STOP;
	OnOK();
}
/*
//	int ParamFile()->System_data.iCURRENT_RUN_STATUS;
#define _CURRENT_RUN_STOP		0
#define _CURRENT_RUN_RUN		1
#define _CURRENT_RUN_PAUSE		2
#define _CURRENT_RUN_ERROR		9

//	BOOL ParamFile()->System_data.bEND_STOP_FLAG;
#define _CURRENT_ENDSTOP_DIS		FALSE
#define _CURRENT_ENDSTOP_ENA		TRUE
*/

BOOL CdlgStopModeSel::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) {
		//      if( pMsg->wParam == 13 ||  pMsg->wParam == VK_ESCAPE )
		if( pMsg->wParam == VK_ESCAPE )	{	pMsg->message = 0;	pMsg->wParam = 0;	}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
