// dlgMarkCopy.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgMarkCopy.h"
#include "afxdialogex.h"


// CdlgMarkCopy 대화 상자입니다.

IMPLEMENT_DYNAMIC(CdlgMarkCopy, CDialogEx)

CdlgMarkCopy::CdlgMarkCopy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgMarkCopy::IDD, pParent)
{

}

CdlgMarkCopy::~CdlgMarkCopy()
{
}

void CdlgMarkCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbModelData);
}


BEGIN_MESSAGE_MAP(CdlgMarkCopy, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgMarkCopy::OnBnClickedButton1)
END_MESSAGE_MAP()

//===================================================================================
// CdlgMarkCopy 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CdlgMarkCopy, CDialogEx)
	ON_EVENT(CdlgMarkCopy, IDB_EXIT, DISPID_CLICK, CdlgMarkCopy::ClickExit, VTS_NONE)
	ON_EVENT(CdlgMarkCopy, IDB_EXIT2, DISPID_CLICK, CdlgMarkCopy::ClickExit2, VTS_NONE)
END_EVENTSINK_MAP()

//===================================================================================
void CdlgMarkCopy::ClickExit()
{
	int sel= m_cbModelData.GetCurSel();

	switch(sel)
	{
	case 0:		SelModel= mdTOP_20X_STAEG1_START; break;
	case 1:		SelModel= mdTOP_20X_STAEG1_END; break;
	case 2:		SelModel= mdTOP_2X_STAEG1_START; break;
	case 3:		SelModel= mdTOP_2X_STAEG1_END; break;

	case 4:		SelModel= mdTOP_20X_STAEG2_START; break;
	case 5:		SelModel= mdTOP_20X_STAEG2_END; break;
	case 6:		SelModel= mdTOP_2X_STAEG2_START; break;
	case 7:		SelModel= mdTOP_2X_STAEG2_END; break;

	case 8:		SelModel= mdTOP_20X_STAEG3_START; break;
	case 9:		SelModel= mdTOP_20X_STAEG3_END; break;
	case 10:	SelModel= mdTOP_2X_STAEG3_START; break;
	case 11:	SelModel= mdTOP_2X_STAEG3_END; break;

	case 12:	SelModel= mdBTM_20X_ROLL_START; break;
	case 13:	SelModel= mdBTM_20X_ROLL_END; break;
	case 14:	SelModel= mdBTM_2X_ROLL_START; break;
	case 15:	SelModel= mdBTM_2X_ROLL_END; break;

	case 16:	SelModel= mdTOP_20X_MASK; break;
	case 17:	SelModel= mdTOP_2X_MASK; break;
	case 18:	SelModel= mdBTM_20X_MASK; break;
	case 19:	SelModel= mdBTM_2X_MASK; break;
	default:	SelModel= 0;			break;
	}
	OnOK();
}

//===================================================================================
void CdlgMarkCopy::ClickExit2()
{
	OnCancel();
}

//===================================================================================
BOOL CdlgMarkCopy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;//	rect.top= rect.top - 10;//top=172, b=411,l=637, r=1021

/*
	GetWindowRect(rect);
	//	rect.top= rect.top - 10;
	//	int disp_OffsetX= 1280 ;
	int disp_OffsetY = (1080 / 2) - (rect.Height() / 2) - 50;
	int disp_OffsetX = (1920 / 2) - (rect.Width() / 2) - 30;
	//	int disp_OffsetY= (1024 / 2) - (rect.Height() / 2) - 50;
	MoveWindow(CRect(_SPLASH_X_POS + disp_OffsetX, _SPLASH_Y_POS + disp_OffsetY, _SPLASH_X_POS + 384 + disp_OffsetX, _SPLASH_Y_POS + 289 + disp_OffsetY)); // H=239, W=384
*/
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SelModel= 0;

/*
enum FIND_MODEL_IDX 
{
mdTOP_20X_STAEG1_START = 0, 	mdTOP_20X_STAEG1_END, 
mdTOP_2X_STAEG1_START = 2,		mdTOP_2X_STAEG1_END,
mdTOP_20X_STAEG2_START = 4,	 	mdTOP_20X_STAEG2_END, 
mdTOP_2X_STAEG2_START = 6,		mdTOP_2X_STAEG2_END,
mdBTM_20X_ROLL_START = 8,		mdBTM_20X_ROLL_END, 
mdBTM_2X_ROLL_START = 10,		mdBTM_2X_ROLL_END,
mdTOP_20X_MASK = 12,			mdTOP_2X_MASK, 
mdBTM_20X_MASK = 14,			mdBTM_2X_MASK,
mdTOP_20X_DEF_START = 16, 		mdTOP_20X_DEF_END, 
mdTOP_2X_DEF_START = 18,		mdTOP_2X_DEF_END,
mdTMP_MASK = 20,
mdTOP_20X_STAEG3_START = 21,	mdTOP_20X_STAEG3_END, 
mdTOP_2X_STAEG3_START = 23,		mdTOP_2X_STAEG3_END
};*/
	CString strText;

	m_cbModelData.AddString(_T("[10X][상부][STAGE #1-1st Mark]")); 
	m_cbModelData.AddString(_T("[10X][상부][STAGE #1-2nd Mark]")); 
	m_cbModelData.AddString(_T("[  2X][상부][STAGE #1-1st Mark]")); 
	m_cbModelData.AddString(_T("[  2X][상부][STAGE #1-2nd Mark]")); 

	m_cbModelData.AddString(_T("[10X][상부][STAGE #2-1st Mark]")); 
	m_cbModelData.AddString(_T("[10X][상부][STAGE #2-2nd Mark]")); 
	m_cbModelData.AddString(_T("[  2X][상부][STAGE #2-1st Mark]")); 
	m_cbModelData.AddString(_T("[  2X][상부][STAGE #2-2nd Mark]")); 

	m_cbModelData.AddString(_T("[10X][상부][STAGE #3-1st Mark]")); 
	m_cbModelData.AddString(_T("[10X][상부][STAGE #3-2nd Mark]")); 
	m_cbModelData.AddString(_T("[  2X][상부][STAGE #3-1st Mark]")); 
	m_cbModelData.AddString(_T("[  2X][상부][STAGE #3-2nd Mark]")); 

	m_cbModelData.AddString(_T("[10X][하부][ROLL-1st Mark]")); 
	m_cbModelData.AddString(_T("[10X][하부][ROLL-2nd Mark]")); 
	m_cbModelData.AddString(_T("[  2X][하부][ROLL-1st Mark]")); 
	m_cbModelData.AddString(_T("[  2X][하부][ROLL-2nd Mark]")); 

	m_cbModelData.AddString(_T("[10X][상부][Photo-Mask]")); 
	m_cbModelData.AddString(_T("[  2X][상부][Photo-Mask]")); 
	m_cbModelData.AddString(_T("[10X][하부][Photo-Mask]")); 
	m_cbModelData.AddString(_T("[  2X][하부][Photo-Mask]")); 

	m_cbModelData.SetCurSel(0);

	UpdateData(FALSE);
	int DispNo= 0;
	switch(CurModel)
	{
	case 0:		DispNo= 0; break;
	case 1:		DispNo= 1; break;
	case 2:		DispNo= 2; break;
	case 3:		DispNo= 3; break;

	case 4:		DispNo= 4; break;
	case 5:		DispNo= 5; break;
	case 6:		DispNo= 6; break;
	case 7:		DispNo= 7; break;

	case 8:		DispNo= 12; break;
	case 9:		DispNo= 13; break;
	case 10:	DispNo= 14; break;
	case 11:	DispNo= 15; break;

	case 12:	DispNo= 16; break;
	case 13:	DispNo= 17; break;
	case 14:	DispNo= 18; break;
	case 15:	DispNo= 19; break;

	case 16:	DispNo= 0; break;
	case 17:	DispNo= 0; break;
	case 18:	DispNo= 0; break;
	case 19:	DispNo= 0; break;

	case 20:	DispNo= 0; break;
	case 21:	DispNo= 8; break;
	case 22:	DispNo= 9; break;
	case 23:	DispNo= 10; break;
	case 24:	DispNo= 11; break;

	case 25:	DispNo= 0; break;
	default:	DispNo= 0;			break;
	}

	m_cbModelData.GetLBText(DispNo, strText);

	SetDlgItemText(IDB_OPU_ROLL_Z12, strText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
//===================================================================================


void CdlgMarkCopy::OnBnClickedButton1()
{
//	SelModel= m_cbModelData.GetCurSel();
}
