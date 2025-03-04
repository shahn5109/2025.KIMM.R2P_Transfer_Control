// dlgAutoRunInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgAutoRunInfo.h"
#include "afxdialogex.h"

#include "direct.h"

// CdlgAutoRunInfo 대화 상자입니다.
extern CSystemClass pSystem;
CParameterFile* CdlgAutoRunInfo::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgAutoRunInfo::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

IMPLEMENT_DYNAMIC(CdlgAutoRunInfo, CDialogEx)

CdlgAutoRunInfo::CdlgAutoRunInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgAutoRunInfo::IDD, pParent)
{
	m_bVac_AutoStartFlag = FALSE;
	m_iVac_AutoStart_Delay = 0;
	m_iPROCESS_CYCLE_COUNT = 1;
	m_strReportPath = _T("");
	m_strUserName = _T("");
	m_strUseInk = _T("");
	m_strUseBlanket = _T("");
	m_dUseTemp = 0.0;
	m_dUseHumidity = 0.0;
	m_strMemo = _T("");
	m_bImageLogDisplay = FALSE;
	m_dPressure = 0.0;
	m_dWaitTime_Off = 0.0;
	m_dWaitTime_Set = 0.0;
	m_dStart_Speed = 0.0;
	m_dApply_Speed = 0.0;
	m_dPrint_Speed = 0.0;
}

CdlgAutoRunInfo::~CdlgAutoRunInfo()
{
}

void CdlgAutoRunInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_TOP_FEED_MODE5, m_btnMode5);
	DDX_Control(pDX, IDB_TOP_FEED_MODE6, m_btnMode6);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_iPROCESS_CYCLE_COUNT);
	DDX_Text(pDX, IDC_EDIT_USER2, m_strReportPath);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_TEMP, m_dUseTemp);
	DDX_Text(pDX, IDC_EDIT_HUM, m_dUseHumidity);
	DDX_Text(pDX, IDC_EDIT_MEMO, m_strMemo);
	DDX_Text(pDX, IDC_EDIT_PRESSURE, m_dPressure);
	DDX_Text(pDX, IDC_EDIT_SPEED1, m_dStart_Speed);
	DDX_Text(pDX, IDC_EDIT_SPEED2, m_dApply_Speed);
	DDX_Text(pDX, IDC_EDIT_SPEED3, m_dPrint_Speed);
}


BEGIN_MESSAGE_MAP(CdlgAutoRunInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CdlgAutoRunInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CdlgAutoRunInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CdlgAutoRunInfo 메시지 처리기입니다.
//===================================================================================
BOOL CdlgAutoRunInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strText;

//	ParamFile()->ReadUserParamFile();

	m_DialogBrush.CreateSolidBrush(CR_SUB_DIALOG_BKB);//다이얼로그 창 배경색

	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_CYCLE_RUN;

	switch(ParamFile()->System_data.iWORK_PROCESS_MODE)
	{
		case _PROCESS_AUTO_RUN:		strText.Format(_T("AUTO_RUN"));	
			m_btnMode5.SetValue(1);		m_btnMode6.SetValue(0); 	
			GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);
			break;
		case _PROCESS_CYCLE_RUN:
			strText.Format(_T("CYCLE_RUN"));
			strText.Format(_T("%d"), ParamFile()->System_data.iPROCESS_CYCLE_COUNT);
			GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
			m_btnMode5.SetValue(0);		m_btnMode6.SetValue(1); 	
			break;
/*
		case _PROCESS_RECIPE_RUN:
			m_btnMode8.SetValue(1); m_btnMode5.SetValue(0);		m_btnMode6.SetValue(0);
			GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);
			ParamFile()->System_data.iPROCESS_CYCLE_COUNT= 0;
			strText.Format("RECIPE_RUN");
			break;
*/
		default:					strText.Format(_T("_RUN_MODE ?"));
			m_btnMode5.SetValue(0);		m_btnMode6.SetValue(0);
			break;
	}
	SetDlgItemText(IDS_STATIC_RUN_MODE, strText.GetBuffer(0));

	ParamFile()->System_data.bEND_STOP_FLAG= _CURRENT_ENDSTOP_DIS;

// 	m_bVac_AutoStartFlag=	g_bVac_AutoStartFlag;
// 	m_iVac_AutoStart_Delay= g_iVac_AutoStart_Delay;

/*
	m_GroupBox1.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GroupBox1.SetCatptionTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_GroupBox1.SetBackgroundColor(CR_SUB_DIALOG_BK);//::GetSysColor(COLOR_3DFACE));
	m_GroupBox1.SetFontBold(TRUE);
	m_GroupBox2.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GroupBox2.SetCatptionTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_GroupBox2.SetBackgroundColor(CR_SUB_DIALOG_BK);//::GetSysColor(COLOR_3DFACE));
	m_GroupBox2.SetFontBold(TRUE);

	m_GroupBox3.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GroupBox3.SetCatptionTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_GroupBox3.SetBackgroundColor(CR_SUB_DIALOG_BK);//::GetSysColor(COLOR_3DFACE));
	m_GroupBox3.SetFontBold(TRUE);

	m_GroupBox4.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GroupBox4.SetCatptionTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_GroupBox4.SetBackgroundColor(CR_SUB_DIALOG_BK);//::GetSysColor(COLOR_3DFACE));
	m_GroupBox4.SetFontBold(TRUE);
*/

/*
	SYSTEMTIME st; 
	GetLocalTime(&st); 

	sprintf(ParamFile()->strDirProcessReportSavePath,	"%s\\Report\\%04d%02d%02d_%02d%02d%02d",
		ParamFile()->strDirRootPath, 
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);//strDirMeasImageSavePath

	sprintf(ParamFile()->strDirReportSavePath,	"%s\\Image",
		ParamFile()->strDirProcessReportSavePath);//strDirMeasImageSavePath

	m_strReportPath.Format("%s", ParamFile()->strDirProcessReportSavePath);
	
	if(ParamFile()->User_data.iImageLogDisplay)	m_bImageLogDisplay= TRUE;
	else										m_bImageLogDisplay= FALSE;

	m_strUserName.Format("%s", ParamFile()->User_data.strUserName);
	m_strUseInk.Format("%s", ParamFile()->User_data.strUseInk);
	m_strUseBlanket.Format("%s", ParamFile()->User_data.strUseBlanket);
	m_strMemo.Format("%s", ParamFile()->User_data.strMemo);

	m_dUseTemp= ParamFile()->User_data.dUseTemp;
	m_dUseHumidity= ParamFile()->User_data.dUseHumidity;

	m_dPressure = ParamFile()->User_data.dPressure;
	
	m_dStart_Speed = ParamFile()->Model_data.dStart_Speed;
	m_dApply_Speed = ParamFile()->Model_data.dApply_Speed;
	m_dPrint_Speed = ParamFile()->Model_data.dPrint_Speed;

	m_dWaitTime_Off = ParamFile()->Model_data.dApply_DRY_Time;
	m_dWaitTime_Set = ParamFile()->Model_data.dPrint_DRY_Time;

	ParamFile()->User_data.dPressure1R= 0.0;
	ParamFile()->User_data.dPressure2R= 0.0;
	ParamFile()->User_data.dPressure3R= 0.0;
	ParamFile()->User_data.dPressure1L= 0.0;
	ParamFile()->User_data.dPressure2L= 0.0;
	ParamFile()->User_data.dPressure3L= 0.0;

	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_CYCLE_RUN;
	ParamFile()->SaveSytemConfigFile(ParamFile()->System_data.strCurrentModelName);
	GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_CYCLE_RUN;
	m_btnMode5.SetValue(0);		m_btnMode6.SetValue(1); 	m_btnMode8.SetValue(0);
	ParamFile()->System_data.iPROCESS_CYCLE_COUNT= m_iPROCESS_CYCLE_COUNT;
*/

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CdlgAutoRunInfo, CDialog)
    //{{AFX_EVENTSINK_MAP(CdlgAutoRunInfo)
	ON_EVENT(CdlgAutoRunInfo, IDB_TOP_FEED_MODE5, -600 /* Click */, OnClickTopFeedMode5, VTS_NONE)
	ON_EVENT(CdlgAutoRunInfo, IDB_TOP_FEED_MODE6, -600 /* Click */, OnClickTopFeedMode6, VTS_NONE)
	ON_EVENT(CdlgAutoRunInfo, IDB_TOP_FEED_MODE7, -600 /* Click */, OnClickTopFeedMode7, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CdlgAutoRunInfo::OnClickTopFeedMode5() 
{
/*
	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_AUTO_RUN;
	ParamFile()->SaveSytemConfigFile(ParamFile()->System_data.strCurrentModelName);
	GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);

	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_AUTO_RUN;
	m_btnMode5.SetValue(1);		m_btnMode6.SetValue(0);
*/
}

void CdlgAutoRunInfo::OnClickTopFeedMode6() 
{

/*
	UpdateData(TRUE);
	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_CYCLE_RUN;
	ParamFile()->SaveSytemConfigFile(ParamFile()->System_data.strCurrentModelName);
	GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_CYCLE_RUN;
	m_btnMode5.SetValue(0);		m_btnMode6.SetValue(1); m_btnMode8.SetValue(0);	
	ParamFile()->System_data.iPROCESS_CYCLE_COUNT= m_iPROCESS_CYCLE_COUNT;
*/

}
void CdlgAutoRunInfo::OnClickTopFeedMode8() 
{
/*
	UpdateData(TRUE);
	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_RECIPE_RUN;
	ParamFile()->SaveSytemConfigFile(ParamFile()->System_data.strCurrentModelName);
	GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);
	ParamFile()->System_data.iWORK_PROCESS_MODE= _PROCESS_RECIPE_RUN;
	m_btnMode5.SetValue(0);	 m_btnMode8.SetValue(1);		m_btnMode6.SetValue(0);
	ParamFile()->System_data.iPROCESS_CYCLE_COUNT= m_iPROCESS_CYCLE_COUNT= 0;
*/
}

void CdlgAutoRunInfo::OnOK() 
{
	UpdateData(TRUE);

/*
//	strcpy(ParamFile()->strDirReportSavePath, m_strReportPath.GetBuffer(0));
	strcpy(ParamFile()->strDirProcessReportSavePath, m_strReportPath.GetBuffer(0));

	if(m_bImageLogDisplay)	ParamFile()->User_data.iImageLogDisplay= 1;
	else					ParamFile()->User_data.iImageLogDisplay= 0;
	
//	m_strUserName.Format("%s", ParamFile()->User_data.strUserName);
	strcpy(ParamFile()->User_data.strUserName, m_strUserName.GetBuffer(0));

//	m_strUseInk.Format("%s", ParamFile()->User_data.strUseInk);
	strcpy(ParamFile()->User_data.strUseInk, m_strUseInk.GetBuffer(0));

//	m_strUseBlanket.Format("%s", ParamFile()->User_data.strUseBlanket);
	strcpy(ParamFile()->User_data.strUseBlanket, m_strUseBlanket.GetBuffer(0));

	ParamFile()->User_data.dUseTemp= m_dUseTemp;
	ParamFile()->User_data.dUseHumidity= m_dUseHumidity;
	ParamFile()->User_data.dPressure= m_dPressure;

	m_dStart_Speed = ParamFile()->Model_data.dStart_Speed;
	m_dApply_Speed = ParamFile()->Model_data.dApply_Speed;
	m_dPrint_Speed = ParamFile()->Model_data.dPrint_Speed;
	
	m_dWaitTime_Off = ParamFile()->Model_data.dApply_DRY_Time;
	m_dWaitTime_Set = ParamFile()->Model_data.dPrint_DRY_Time;

//	m_strMemo.Format("%s", ParamFile()->User_data.strMemo);
	strcpy(ParamFile()->User_data.strMemo, m_strMemo.GetBuffer(0));

	ParamFile()->SaveUserParamFile();
	
	ParamFile()->System_data.iPROCESS_CYCLE_COUNT= m_iPROCESS_CYCLE_COUNT;
	
	_mkdir(ParamFile()->strDirReportSavePath);
	_mkdir(ParamFile()->strDirProcessReportSavePath);
*/

	CDialog::OnOK();
}

int CALLBACK MyCallback2(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        break;
    }
    return 0;
}


void CdlgAutoRunInfo::OnClickTopFeedMode7() 
{
/*
	UpdateData(TRUE);
//	BOOL      bFind;
	CString   strFindFiles;
	CString   strFindDirName;
	
	CFileFind find;
	
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	LPTSTR pFolderName = _T("D:\\"); //ParamFile()->strDirReportSavePath; 
//	strcpy(pFolderName, (LPCTSTR)ParamFile()->strDirReportSavePath);

	sprintf(ParamFile()->strDirProcessReportSavePath,	"%s\\Report",
		ParamFile()->strDirRootPath);//strDirMeasImageSavePath
	sprintf(ParamFile()->strDirReportSavePath,	"%s\\Image",
		ParamFile()->strDirProcessReportSavePath);//strDirMeasImageSavePath
	
	m_strReportPath.Format("%s", ParamFile()->strDirProcessReportSavePath);

	pFolderName= ParamFile()->strDirProcessReportSavePath;

	TCHAR szBuff[MAX_PATH] = {0}, szDisp[MAX_PATH] = {0}, szPath[MAX_PATH] = {0};
	TCHAR szRootName[MAX_PATH] = {0};
	lstrcpy(szBuff, "Select a target folder that driver files will be copied");
	::ZeroMemory( &bi, sizeof(BROWSEINFO) ); 
	bi.hwndOwner = this->GetSafeHwnd();
	bi.lpfn = MyCallback2;
	bi.pidlRoot = NULL;
	bi.lpszTitle = static_cast<LPSTR>(szBuff); 
	bi.pszDisplayName = static_cast<LPSTR>(szDisp);
	//bi.ulFlags = BIF_RETURNONLYFSDIRS;    // BIF_USENEWUI 는 '새 폴더 만들기' 기능을 위한 플래그. 없는 것보단 있는게 낫겠죠?
	//		bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_VALIDATE;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_VALIDATE|BIF_USENEWUI;
	//		bi.lParam = (LPARAM)this;
	//	bi.lpfn = MyBrowseCallbackProc;
	/ *
	-bInfo.ulFlags 플래그
	BIF_BROWSEFORCOMPUTER    : 네트워크의 컴퓨터만 선택가능
	BIF_BROWSEFORPRINTER        : 프린터만 선택가능
	BIF_BROWSEINCLUDEFILES      : 파일도 표시
	BIF_DONTGOBELOWDOMAIN  : 네트워크의 컴퓨터를 표시하지 않는다
	BIF_EDITBOX                      : 에디트 박스를 표시한다
	BIF_RETURNFSANCESTORS     : 네트워크의 컴퓨터만 선택가능
	BIF_RETURNONLYFSDIRS        : 폴더만 선택가능
	BIF_STATUSTEXT                  : 스테이터스 텍스트를 표시한다
	BIF_VALIDATE                     : 부정 입력시에, BFFM_VALIDATEFAILED 이벤트
	* /
	bi.pidlRoot = NULL;
	bi.lpszTitle = "이미지 저장 폴더 선택...";
	
	bi.lParam = (LPARAM)(LPCTSTR)pFolderName;
	strcpy(szRootName, ParamFile()->strDirProcessReportSavePath); 
	strcpy(szPath, ParamFile()->strDirProcessReportSavePath); 
	
	if( !(pidl=SHBrowseForFolder(&bi)) )	return;
	ZeroMemory(szPath, MAX_PATH);
	SHGetPathFromIDList(pidl, szPath);
	
//	ParamFile()->strDirReportSavePath.Format("%s", szPath);
	strcpy(ParamFile()->strDirReportSavePath, szPath); 
	m_strReportPath.Format("Image_%s", ParamFile()->strDirReportSavePath);
	
	strcpy(ParamFile()->strDirProcessReportSavePath, szPath); 
	m_strReportPath.Format("%s", ParamFile()->strDirReportSavePath);

*/
	UpdateData(FALSE);

}

BOOL CdlgAutoRunInfo::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) {
		//      if( pMsg->wParam == 13 ||  pMsg->wParam == VK_ESCAPE )
		if( pMsg->wParam == VK_ESCAPE )	{	pMsg->message = 0;	pMsg->wParam = 0;	}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CdlgAutoRunInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (nCtlColor)
	{    
	case CTLCOLOR_STATIC:
		pDC->SetBkColor(CR_SUB_DIALOG_BKB);
		return m_DialogBrush;
	case CTLCOLOR_EDIT:
		pDC->SetBkColor(CR_STATIC_BK);
		return hbr;
	case CTLCOLOR_DLG:     
		return m_DialogBrush;
	} 
	return hbr;
}



void CdlgAutoRunInfo::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void CdlgAutoRunInfo::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}
