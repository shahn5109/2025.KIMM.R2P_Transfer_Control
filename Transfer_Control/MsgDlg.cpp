// MsgDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "MsgDlg.h"
#include "afxdialogex.h"


// CMsgDlg 대화 상자입니다.
extern CSystemClass pSystem;
CParameterFile* CMsgDlg::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CMsgDlg::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }


IMPLEMENT_DYNAMIC(CMsgDlg, CDialogEx)

	CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMsgDlg::IDD, pParent)
{

}

CMsgDlg::~CMsgDlg()
{
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_MYABORT, m_bMyAbort);
	DDX_Control(pDX, ID_MYCANCEL, m_bMyCancel);
	DDX_Control(pDX, ID_MYCANCEL3, m_bMyCancel3);
	DDX_Control(pDX, ID_MYIGNORE, m_bMyIgnore);
	DDX_Control(pDX, ID_MYNO, m_bMyNo);
	DDX_Control(pDX, ID_MYOK, m_bMyOk);
	DDX_Control(pDX, ID_MYOK2, m_bMyOk2);
	DDX_Control(pDX, ID_MYRETRY, m_bMyRetry);
	DDX_Control(pDX, ID_MYRETRY3, m_bMyRetry3);
	DDX_Control(pDX, ID_MYYES, m_bMyYes);
	DDX_Control(pDX, IDC_PICTURE, m_MyIcon);
	DDX_Control(pDX, IDC_EDITLABEL, m_lblMsg);
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMsgDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMsgDlg 메시지 처리기입니다.


//===================================================================================
BOOL CMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ) {
		//      if( pMsg->wParam == 13 ||  pMsg->wParam == VK_ESCAPE )
		if( pMsg->wParam == VK_ESCAPE )	{	pMsg->message = 0;	pMsg->wParam = 0;	}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//===================================================================================
void CMsgDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_icontype== M_ICONERROR)
	{
		if(m_bFlag) 
		{
			m_bFlag= false;
			m_lblMsg.SetForeColor(0x00FFFFFF);		m_lblMsg.SetBackColorInterior(0x000000FF);
		}
		else 
		{
			m_bFlag= true;
			m_lblMsg.SetForeColor(0x00000000);		m_lblMsg.SetBackColorInterior(0x00FFFFFF);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

//===================================================================================
HBRUSH CMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if( m_icontype != M_ICONERROR ) 	return hbr;

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//===================================================================================
void CMsgDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//===================================================================================
BOOL CMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strTemp;
	//	TCHAR pBuf[MAX_PATH] = {0};

	SetWindowText(m_title);

	CRect rect;
	GetWindowRect(&rect);
	int xLeft =   GetSystemMetrics(SM_CXSCREEN) / 2 - rect.Width() / 2;
	int yTop =    GetSystemMetrics(SM_CYSCREEN) / 2 - rect.Height() / 2;

	::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,xLeft, yTop, rect.right+xLeft, rect.bottom+yTop, SWP_NOSIZE | SWP_SHOWWINDOW);
	::SetForegroundWindow(this->GetSafeHwnd());
	::SetActiveWindow(this->GetSafeHwnd());

	m_bMyOk2.ShowWindow(SW_HIDE);
	m_bMyOk.ShowWindow(SW_HIDE);
	m_bMyCancel.ShowWindow(SW_HIDE);
	m_bMyYes.ShowWindow(SW_HIDE);
	m_bMyNo.ShowWindow(SW_HIDE);
	m_bMyAbort.ShowWindow(SW_HIDE);
	m_bMyRetry.ShowWindow(SW_HIDE);
	m_bMyIgnore.ShowWindow(SW_HIDE);
	m_bMyCancel3.ShowWindow(SW_HIDE);
	m_bMyRetry3.ShowWindow(SW_HIDE);

	switch(m_type)
	{
	case M_OK :
		m_bMyOk2.SetCaption(m_strYes);
		m_bMyOk2.ShowWindow(SW_SHOW);
		break;

	case M_OKCANCEL :
		m_bMyOk.ShowWindow(SW_SHOW);
		m_bMyCancel.ShowWindow(SW_SHOW);
		break;

	case M_YESNO :
		m_bMyYes.SetCaption(m_strYes);
		m_bMyNo.SetCaption(m_strNo);
		m_bMyYes.ShowWindow(SW_SHOW);
		m_bMyNo.ShowWindow(SW_SHOW);
		break;

	case M_ABORTRETRYIGNORE :
//		m_bMyAbort.ShowWindow(SW_SHOW);
		m_bMyRetry.ShowWindow(SW_SHOW);
		m_bMyIgnore.ShowWindow(SW_SHOW);
		m_bMyAbort.SetCaption(m_strYes);
		m_bMyRetry.SetCaption(m_strNo);
		m_bMyIgnore.SetCaption(_T("취소"));
		break;

	case M_RETRYCANCEL :
		m_bMyCancel3.ShowWindow(SW_SHOW);
		m_bMyRetry3.ShowWindow(SW_SHOW);
		break;

	default :
		m_bMyOk2.SetCaption(_T("확인"));
		m_bMyOk2.ShowWindow(SW_SHOW);
		break;
	}

	m_bFlag= false;

	m_lblMsg.SetForeColor(0x00000000);		m_lblMsg.SetBackColorInterior(0x00FFFFFF);
	//	_tcscpy(pBuf, "%s\\Bin\\Icon", _PROGRAM_ROOT_DIR);
	switch(m_icontype)
	{
	case M_ICONINFORMATION :  
		strTemp.Format(_T("%s\\Bin\\Icon\\infoicon.png"), _PROGRAM_ROOT_DIR);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;

	case M_ICONQUESTION :
		//		 ::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\Bin\\Icon\\quesicon.png"), _PROGRAM_ROOT_DIR);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);

		break;

	case M_ICONSTOP :
		//		 ::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\Bin\\Icon\\stopicon.png"), _PROGRAM_ROOT_DIR);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;

	case M_ICONERROR :
		//  		 ::GetCurrentDirectory(sizeof(pBuf), pBuf);
		//		 strTemp.Format("%s\\erricon.png", pBuf);//error.png
		strTemp.Format(_T("%s\\Bin\\Icon\\error.png"), _PROGRAM_ROOT_DIR);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);

		m_lblMsg.SetForeColor(0x00FFFFFF);		m_lblMsg.SetBackColorInterior(0x000000FF);
		//0x000000FF, 0x00FFFFFF
		m_bMyAbort.ShowWindow(SW_SHOW);
		break;

	default :
		//      	::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\Bin\\Icon\\question_and_answer.png"), _PROGRAM_ROOT_DIR);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;
	}

	m_lblMsg.SetCaption(m_message);  
	CString strText;
	strText.Format(_T("[MSG-BOX] => %s!"), m_message);
	Log()->SaveLogFile(_SYSTEM_LOG, m_message);

	SetTimer(9000, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//===================================================================================
void CMsgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(9000);
}

//===================================================================================
BOOL CMsgDlg::OnEraseBkgnd(CDC* pDC)
{
	COLORREF BackColor;
	BackColor= RGB(255,255,255);

	CBrush brush(BackColor);
	CRect  rect;
	pDC->GetClipBox(rect);
	CBrush * pOldBrush = pDC->SelectObject(&brush);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
	return TRUE;
}

//===================================================================================
BEGIN_EVENTSINK_MAP(CMsgDlg, CDialogEx)
	ON_EVENT(CMsgDlg, ID_MYABORT, DISPID_CLICK, CMsgDlg::OnClickMyabort, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYOK, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyok, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYYES, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyyes, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYRETRY3, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyretry3, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYOK2, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyok2, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYRETRY, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyretry, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYCANCEL, DISPID_CLICK /* Click */, CMsgDlg::OnClickMycancel, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYCANCEL3, DISPID_CLICK /* Click */, CMsgDlg::OnClickMycancel3, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYNO, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyno, VTS_NONE)
	ON_EVENT(CMsgDlg, ID_MYIGNORE, DISPID_CLICK /* Click */, CMsgDlg::OnClickMyignore, VTS_NONE)
END_EVENTSINK_MAP()
//===================================================================================

//===================================================================================
void CMsgDlg::OnClickMyabort() 
{
	pSystem.GetIO()->BuzzerStop();
	// TODO: Add your control notification handler code here
//	KillTimer(900);
}
void CMsgDlg::OnClickMyok() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDOK!"));
	KillTimer(900);
	EndDialog(IDOK);
}


void CMsgDlg::OnClickMyyes() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDYES!"));
	KillTimer(900);
	EndDialog(IDYES);
}

void CMsgDlg::OnClickMyretry3() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDRETRY!"));
	KillTimer(900);
	EndDialog(IDRETRY);
}

void CMsgDlg::OnClickMyok2() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDOK!"));
	KillTimer(900);
	EndDialog(IDOK);
}

void CMsgDlg::OnClickMyretry() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDRETRY!"));
	KillTimer(900);
	EndDialog(IDRETRY);
}

void CMsgDlg::OnClickMycancel() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDCANCEL!"));
	KillTimer(900);
	EndDialog(IDCANCEL);
}

void CMsgDlg::OnClickMycancel3() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDCANCEL!"));
	KillTimer(900);
	EndDialog(IDCANCEL);
}

void CMsgDlg::OnClickMyno() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDNO!"));
	KillTimer(900);
	EndDialog(IDNO);
}

void CMsgDlg::OnClickMyignore() 
{
	// TODO: Add your control notification handler code here
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[MSG-BOX] => IDIGNORE!"));
	KillTimer(900);
	EndDialog(IDIGNORE);
}
//===================================================================================

