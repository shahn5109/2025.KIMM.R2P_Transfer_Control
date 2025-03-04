// dlgManual_IO_Monitor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgManual_IO_Monitor.h"
#include "afxdialogex.h"
#include "Mainfrm.h"

extern CSystemClass pSystem;
CParameterFile* CdlgManual_IO_Monitor::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgManual_IO_Monitor::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

// CdlgManual_IO_Monitor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgManual_IO_Monitor, CDialogEx)

CdlgManual_IO_Monitor::CdlgManual_IO_Monitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgManual_IO_Monitor::IDD, pParent)
{
	m_DialogBrush.CreateSolidBrush(CR_SUB_DIALOG_BK);//다이얼로그 창 배경색
}

CdlgManual_IO_Monitor::~CdlgManual_IO_Monitor()
{
}

void CdlgManual_IO_Monitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

// 	DDX_Control(pDX, IDC_STATIC_BOX1, m_GroupBox1);
// 	DDX_Control(pDX, IDC_STATIC_BOX2, m_GroupBox2);
	DDX_Control(pDX, IDB_PREVIOUS,	m_bInPrev);
	DDX_Control(pDX, IDB_NEXT,		m_bInNext);
	DDX_Control(pDX, IDB_PREVIOUS2, m_bOutPrev);
	DDX_Control(pDX, IDB_NEXT2,		m_bOutNext);

	DDX_Control(pDX, IDC_IO_TITLE0, m_lblInputTitle[0]);
	DDX_Control(pDX, IDC_IO_TITLE1, m_lblInputTitle[1]);
	DDX_Control(pDX, IDC_IO_TITLE2, m_lblOutputTitle[0]);
	DDX_Control(pDX, IDC_IO_TITLE3, m_lblOutputTitle[1]);

	DDX_Control(pDX, IDB_IO_0 , m_btnInput[0 ]);
	DDX_Control(pDX, IDB_IO_1 , m_btnInput[1 ]);
	DDX_Control(pDX, IDB_IO_2 , m_btnInput[2 ]);
	DDX_Control(pDX, IDB_IO_3 , m_btnInput[3 ]);
	DDX_Control(pDX, IDB_IO_4 , m_btnInput[4 ]);
	DDX_Control(pDX, IDB_IO_5 , m_btnInput[5 ]);
	DDX_Control(pDX, IDB_IO_6 , m_btnInput[6 ]);
	DDX_Control(pDX, IDB_IO_7 , m_btnInput[7 ]);
	DDX_Control(pDX, IDB_IO_8 , m_btnInput[8 ]);
	DDX_Control(pDX, IDB_IO_9 , m_btnInput[9 ]);
	DDX_Control(pDX, IDB_IO_10, m_btnInput[10]);
	DDX_Control(pDX, IDB_IO_11, m_btnInput[11]);
	DDX_Control(pDX, IDB_IO_12, m_btnInput[12]);
	DDX_Control(pDX, IDB_IO_13, m_btnInput[13]);
	DDX_Control(pDX, IDB_IO_14, m_btnInput[14]);
	DDX_Control(pDX, IDB_IO_15, m_btnInput[15]);

	DDX_Control(pDX, IDB_IO_16, m_btnOutput[0 ]);
	DDX_Control(pDX, IDB_IO_17, m_btnOutput[1 ]);
	DDX_Control(pDX, IDB_IO_18, m_btnOutput[2 ]);
	DDX_Control(pDX, IDB_IO_19, m_btnOutput[3 ]);
	DDX_Control(pDX, IDB_IO_20, m_btnOutput[4 ]);
	DDX_Control(pDX, IDB_IO_21, m_btnOutput[5 ]);
	DDX_Control(pDX, IDB_IO_22, m_btnOutput[6 ]);
	DDX_Control(pDX, IDB_IO_23, m_btnOutput[7 ]);
	DDX_Control(pDX, IDB_IO_24, m_btnOutput[8 ]);
	DDX_Control(pDX, IDB_IO_25, m_btnOutput[9 ]);
	DDX_Control(pDX, IDB_IO_26, m_btnOutput[10]);
	DDX_Control(pDX, IDB_IO_27, m_btnOutput[11]);
	DDX_Control(pDX, IDB_IO_28, m_btnOutput[12]);
	DDX_Control(pDX, IDB_IO_29, m_btnOutput[13]);
	DDX_Control(pDX, IDB_IO_30, m_btnOutput[14]);
	DDX_Control(pDX, IDB_IO_31, m_btnOutput[15]);
}


BEGIN_MESSAGE_MAP(CdlgManual_IO_Monitor, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CdlgManual_IO_Monitor 메시지 처리기입니다.
BOOL CdlgManual_IO_Monitor::OnInitDialog() 
{
	CDialog::OnInitDialog();

/*
	m_GroupBox1.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GroupBox1.SetCatptionTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_GroupBox1.SetBackgroundColor(CR_SUB_DIALOG_BK);//::GetSysColor(COLOR_3DFACE));
	m_GroupBox1.SetFontBold(TRUE);

	m_GroupBox2.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GroupBox2.SetCatptionTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_GroupBox2.SetBackgroundColor(CR_SUB_DIALOG_BK);//::GetSysColor(COLOR_3DFACE));
	m_GroupBox2.SetFontBold(TRUE);
*/

	m_iInPage= 0;		m_iOutPage= 0;
	m_iInAddrBase= 0;	m_iOutAddrBase= 0;

	ParamFile()->ReadIOTableParamFile();

	SetTimer(dlgManual_IO_Monitor_TIMER_ID,	dlgManual_IO_MonitorSync, NULL);

	DisplayUpdate();

	CString strTemp;
	int i;
	strTemp.Format(_T("%s\\Bin\\icon\\Green.ico"), _PROGRAM_ROOT_DIR);	//gear_forbidden, gear_ok
	for(i= 0; i< 16; i++) {	m_btnInput[i].SetPicturePressed(strTemp.GetBuffer(0)); m_btnOutput[i].SetPicturePressed(strTemp.GetBuffer(0)); }
	strTemp.Format(_T("%s\\Bin\\icon\\Gray.ico"), _PROGRAM_ROOT_DIR);	//gear_forbidden, gear_ok
	for(i= 0; i< 16; i++) {	m_btnInput[i].SetPicture(strTemp.GetBuffer(0)); m_btnOutput[i].SetPicture(strTemp.GetBuffer(0)); }

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CdlgManual_IO_Monitor::OnDestroy() 
{
	CDialog::OnDestroy();

	m_DialogBrush.DeleteObject ();

	KillTimer(dlgManual_IO_Monitor_TIMER_ID);

}

HBRUSH CdlgManual_IO_Monitor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{    
	case CTLCOLOR_STATIC:
		pDC->SetBkColor(CR_SUB_DIALOG_BK);
		return m_DialogBrush;
	case CTLCOLOR_EDIT:
		pDC->SetBkColor(CR_STATIC_BK);
		return hbr;
	case CTLCOLOR_DLG:     
		return m_DialogBrush;
	} 

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CdlgManual_IO_Monitor::OnTimer(UINT nIDEvent) 
{
	m_iInAddrBase = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iInPage;
	m_iOutAddrBase = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iOutPage;

	for( int i=0 ; i< DEF_IOMONITOR_MAX_IO_PER_PAGE ; i++ )	// Input Display인 경우
	{
		if(pSystem.GetMotionComponent()->InputBit(m_iInAddrBase + i))
		{
			m_btnInput[i].SetValue(TRUE);
		}
		else
		{
			m_btnInput[i].SetValue(FALSE);
		}
		if(pSystem.GetMotionComponent()->isOutputBit(m_iOutAddrBase + i ))
		{
			m_btnOutput[i].SetValue(TRUE);
		}
		else
		{
			m_btnOutput[i].SetValue(FALSE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CdlgManual_IO_Monitor::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CdlgManual_IO_Monitor, CDialog)
	//{{AFX_EVENTSINK_MAP(CdlgManual_IO_Monitor)
	ON_EVENT(CdlgManual_IO_Monitor, IDOK, -600 /* Click */, CdlgManual_IO_Monitor::OnClickOk, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_PREVIOUS, -600 /* Click */, CdlgManual_IO_Monitor::OnClickPrevious, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_NEXT, -600 /* Click */, CdlgManual_IO_Monitor::OnClickNext, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_PREVIOUS2, -600 /* Click */, CdlgManual_IO_Monitor::OnClickPrevious2, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_NEXT2, -600 /* Click */, CdlgManual_IO_Monitor::OnClickNext2, VTS_NONE)

	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_16, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo16, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_17, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo17, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_18, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo18, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_19, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo19, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_20, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo20, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_21, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo21, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_22, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo22, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_23, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo23, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_24, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo24, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_25, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo25, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_26, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo26, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_27, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo27, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_28, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo28, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_29, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo29, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_30, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo30, VTS_NONE)
	ON_EVENT(CdlgManual_IO_Monitor, IDB_IO_31, -600 /* Click */, CdlgManual_IO_Monitor::OnClickIo31, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CdlgManual_IO_Monitor::OnClickOk() 
{
	OnOK();
}

void CdlgManual_IO_Monitor::DisplayUpdate()
{
	int i, tempNum;

	m_iInAddrBase = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iInPage;
	m_iOutAddrBase = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iOutPage;

	CString m_sInputTitleText[DEF_IOMONITOR_MAX_TITLE_PER_PAGE * DEF_IOMONITOR_MAX_INPUT_PAGE] = {	
		_T("X000 ~ X007"), _T("X008 ~ X015"), 
		_T("X016 ~ X023"), _T("X024 ~ X031"),
		_T("X032 ~ X039"), _T("X040 ~ X047"), 
		_T("X048 ~ X055"), _T("X056 ~ X063")
	};

	CString m_sOutputTitleText[DEF_IOMONITOR_MAX_TITLE_PER_PAGE * DEF_IOMONITOR_MAX_OUTPUT_PAGE] = {	
		_T("Y000 ~ Y007"), _T("Y008 ~ Y015"), 
		_T("Y016 ~ Y023"), _T("Y024 ~ Y031"),
		_T("Y032 ~ Y039"), _T("Y040 ~ Y047"), 
		_T("Y048 ~ Y055"), _T("Y056 ~ Y063")
	};
	if (m_iInPage == 0)	{	/*m_bInPrev.ShowWindow(SW_HIDE);*/		m_bInPrev.EnableWindow(FALSE);	}
	else				{	/*m_bInPrev.ShowWindow(SW_SHOW);*/		m_bInPrev.EnableWindow(TRUE);	}
	if	(m_iInPage == DEF_IOMONITOR_MAX_INPUT_PAGE - 1)	
	{	/*m_bInPrev.ShowWindow(SW_HIDE);*/		m_bInNext.EnableWindow(FALSE);	}
	else				{	/*m_bInPrev.ShowWindow(SW_SHOW);*/		m_bInNext.EnableWindow(TRUE);	}

	if (m_iOutPage == 0){	/*m_bOutPrev.ShowWindow(SW_HIDE);*/		m_bOutPrev.EnableWindow(FALSE);	}
	else				{	/*m_bOutPrev.ShowWindow(SW_SHOW);*/		m_bOutPrev.EnableWindow(TRUE);	}
	if(m_iOutPage == DEF_IOMONITOR_MAX_OUTPUT_PAGE - 1)
	{	/*m_bOutPrev.ShowWindow(SW_HIDE);*/		m_bOutNext.EnableWindow(FALSE);	}
	else				{	/*m_bInPrev.ShowWindow(SW_SHOW);*/		m_bOutNext.EnableWindow(TRUE);	}

	for (i = 0; i < DEF_IOMONITOR_MAX_TITLE_PER_PAGE; i++)
	{
			tempNum = DEF_IOMONITOR_MAX_TITLE_PER_PAGE * m_iInPage + i;
			m_lblInputTitle[i].SetCaption(m_sInputTitleText[tempNum]);

			tempNum = DEF_IOMONITOR_MAX_TITLE_PER_PAGE * m_iOutPage + i;
			m_lblOutputTitle[i].SetCaption(m_sOutputTitleText[tempNum]);
	}

	for( i=0 ; i< DEF_IOMONITOR_MAX_IO_PER_PAGE ; i++ )
	{
		if((m_iInAddrBase+i) > _ACS_INOUT_DIO2_END_NO )
		{
			m_btnInput[i].SetCaption(_T("..."));
		}
		if((m_iOutAddrBase+i) > _ACS_INOUT_DIO2_END_NO )
		{
			m_btnOutput[i].SetCaption(_T("..."));
		}
		else
		{
			m_btnInput[i].SetWindowText(ParamFile()->Input_Table[m_iInAddrBase+i].Name);
			m_btnOutput[i].SetWindowText(ParamFile()->Output_Table[m_iOutAddrBase+i].Name);
		}
	}
}

void CdlgManual_IO_Monitor::OnClickPrevious() 
{
	if (m_iInPage > 0)	
		m_iInPage--;
	DisplayUpdate();
}

void CdlgManual_IO_Monitor::OnClickNext() 
{
	if (m_iInPage < DEF_IOMONITOR_MAX_INPUT_PAGE - 1)
		m_iInPage++;
	DisplayUpdate();
}

void CdlgManual_IO_Monitor::OnClickPrevious2() 
{
	if (m_iOutPage > 0)	
		m_iOutPage--;
	DisplayUpdate();
}

void CdlgManual_IO_Monitor::OnClickNext2() 
{
	if (m_iOutPage < DEF_IOMONITOR_MAX_OUTPUT_PAGE - 1)
		m_iOutPage++;
	DisplayUpdate();
}

void CdlgManual_IO_Monitor::OnClickIo16() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 0 );	}
void CdlgManual_IO_Monitor::OnClickIo17() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 1 );	}
void CdlgManual_IO_Monitor::OnClickIo18() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 2 );	}
void CdlgManual_IO_Monitor::OnClickIo19() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 3 );	}
void CdlgManual_IO_Monitor::OnClickIo20() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 4 );	}
void CdlgManual_IO_Monitor::OnClickIo21() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 5 );	}
void CdlgManual_IO_Monitor::OnClickIo22() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 6 );	}
void CdlgManual_IO_Monitor::OnClickIo23() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 7 );	}
void CdlgManual_IO_Monitor::OnClickIo24() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 8 );	}
void CdlgManual_IO_Monitor::OnClickIo25() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 9 );	}
void CdlgManual_IO_Monitor::OnClickIo26() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 10);	}
void CdlgManual_IO_Monitor::OnClickIo27() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 11);	}
void CdlgManual_IO_Monitor::OnClickIo28() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 12);	}
void CdlgManual_IO_Monitor::OnClickIo29() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 13);	}
void CdlgManual_IO_Monitor::OnClickIo30() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 14);	}
void CdlgManual_IO_Monitor::OnClickIo31() {	pSystem.GetMotionComponent()->OutputToggle(m_iOutAddrBase + 15);	}
