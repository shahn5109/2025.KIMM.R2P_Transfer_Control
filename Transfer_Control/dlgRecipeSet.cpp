// dlgRecipeSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgRecipeSet.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "dlgRecipePrintEdit.h"


// CdlgRecipeSet 대화 상자입니다.
extern CSystemClass pSystem;
CParameterFile* CdlgRecipeSet::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgRecipeSet::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

IMPLEMENT_DYNAMIC(CdlgRecipeSet, CDialogEx)

CdlgRecipeSet::CdlgRecipeSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgRecipeSet::IDD, pParent)
{
	m_dStage1_Len = 0.0;
	m_dStage2_Len = 0.0;
	m_dStage3_Len = 0.0;
	m_dStage1_Speed = 0.0;
	m_dStage2_Speed = 0.0;
	m_dStage3_Speed = 0.0;
	m_dStage1_Y_Offset = 0.0;
	m_dStage2_Y_Offset = 0.0;
	m_dStage3_Y_Offset = 0.0;
	m_dStage1_X_Offset = 0.0;
	m_dStage2_X_Offset = 0.0;
	m_dStage3_X_Offset = 0.0;
	m_iDelayTime = 100;
	m_strMessage = _T("");
	m_iVacOnOffSel1 = -1;
	m_iVacOnOffSel2 = -1;
	m_iVacOnOffSel3 = -1;

	_stprintf(m_chrOP_Val, _T(" "));//	_tcscpy(Date, _T(__DATE__));
	m_iOP_Command= 0;
	m_dOP_Val1= 0.0;
	m_dOP_Val2= 0.0;
	m_dOP_Val3= 0.0;

}
//===================================================================================
CdlgRecipeSet::~CdlgRecipeSet()
{
}
//===================================================================================
void CdlgRecipeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdlgRecipeSet)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_X_LEN_1, m_dStage1_Len);
	DDX_Text(pDX, IDC_EDIT_X_LEN_2, m_dStage2_Len);
	DDX_Text(pDX, IDC_EDIT_X_LEN_3, m_dStage3_Len);
	DDX_Text(pDX, IDC_EDIT_RUN_SPEED_1, m_dStage1_Speed);
	DDX_Text(pDX, IDC_EDIT_RUN_SPEED_4, m_dStage2_Speed);
	DDX_Text(pDX, IDC_EDIT_RUN_SPEED_5, m_dStage3_Speed);
	DDX_Text(pDX, IDC_EDIT8, m_dStage1_Y_Offset);
	DDX_Text(pDX, IDC_EDIT9, m_dStage2_Y_Offset);
	DDX_Text(pDX, IDC_EDIT10, m_dStage3_Y_Offset);
	DDX_Text(pDX, IDC_EDIT_X1, m_dStage1_X_Offset);
	DDX_Text(pDX, IDC_EDIT_X2, m_dStage2_X_Offset);
	DDX_Text(pDX, IDC_EDIT_X3, m_dStage3_X_Offset);
	DDX_Text(pDX, IDC_EDIT4, m_iDelayTime);
	DDX_Text(pDX, IDC_EDIT3, m_strMessage);
	DDX_Radio(pDX, IDC_RADIO11, m_iVacOnOffSel1);
	DDX_Radio(pDX, IDC_RADIO13, m_iVacOnOffSel2);
	DDX_Radio(pDX, IDC_RADIO15, m_iVacOnOffSel3);
	DDX_Control(pDX, IDB_ENABLE_3, m_btnAlignR);
	DDX_Control(pDX, IDB_ENABLE_7, m_btnAlign1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CdlgRecipeSet, CDialogEx)
	//{{AFX_MSG_MAP(CdlgRecipeSet)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgRecipeSet::OnBnClickedButton1)
END_MESSAGE_MAP()


// CdlgRecipeSet 메시지 처리기입니다.
//===================================================================================

BOOL CdlgRecipeSet::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}
//===================================================================================
BOOL CdlgRecipeSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ListCtrl.SetBkColor(RGB(255,230,230));
	m_ListCtrl.SetTextBkColor(RGB(255,230,230));
	// 리스트 컨트롤의 행 선택,Flat,라인.. 스타일
	m_ListCtrl.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("Command"), LVCFMT_RIGHT, 150, 0);
	m_ListCtrl.InsertColumn(1, _T("Op1"), LVCFMT_RIGHT, 80, 0);
	m_ListCtrl.InsertColumn(2, _T("Op2"), LVCFMT_RIGHT, 80, 0);
	m_ListCtrl.InsertColumn(3, _T("X"), LVCFMT_RIGHT, 80, 0);
	m_ListCtrl.InsertColumn(4, _T("Y"), LVCFMT_RIGHT, 80, 0);
	m_ListCtrl.InsertColumn(5, _T("T"), LVCFMT_RIGHT, 80, 0);
	m_ListCtrl.InsertColumn(6, _T(" "), LVCFMT_RIGHT, 70, 0);

	ListCnt= 0;

	CString strText;

	strText.Format(_T("%s"), ParamFile()->System_data.strCurrentRecipeName);
	SetDlgItemText(IDS_LABEL_MODELNAME, strText.GetBuffer(0));

	m_dStage1_Len= ParamFile()->Model_data.dRun_Length[STAGE_1_ALIGN_PROC];
	m_dStage2_Len= ParamFile()->Model_data.dRun_Length[STAGE_2_ALIGN_PROC];
	m_dStage3_Len= ParamFile()->Model_data.dRun_Length[STAGE_3_ALIGN_PROC];
	m_dStage1_Speed= ParamFile()->Model_data.dRun_Speed[STAGE_1_ALIGN_PROC];	  
	m_dStage2_Speed= ParamFile()->Model_data.dRun_Speed[STAGE_2_ALIGN_PROC];	  
	m_dStage3_Speed= ParamFile()->Model_data.dRun_Speed[STAGE_3_ALIGN_PROC];	  


	UpdateData(TRUE);

	DispDisable() ;
	Data_Update();
	ListUpdate();

	m_btnAlignR.SetValue(1);	m_btnAlign1.SetValue(0);

	SelAlignVal= 0;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//===================================================================================
void CdlgRecipeSet::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

}
//===================================================================================
void CdlgRecipeSet::Data_Update() 
{
	m_dStage1_Len= ParamFile()->Model_data.dRun_Length[STAGE_1_ALIGN_PROC];
	m_dStage2_Len= ParamFile()->Model_data.dRun_Length[STAGE_2_ALIGN_PROC];
	m_dStage3_Len= ParamFile()->Model_data.dRun_Length[STAGE_3_ALIGN_PROC];
	m_dStage1_Speed= ParamFile()->Model_data.dRun_Speed[STAGE_1_ALIGN_PROC];
	m_dStage2_Speed= ParamFile()->Model_data.dRun_Speed[STAGE_2_ALIGN_PROC];
	m_dStage3_Speed= ParamFile()->Model_data.dRun_Speed[STAGE_3_ALIGN_PROC];
	m_dStage1_Y_Offset= 0.0;
	m_dStage2_Y_Offset= 0.0;
	m_dStage3_Y_Offset= 0.0;

	m_dStage1_Y_Offset = ParamFile()->Model_data.dRun_Offset_Y[STAGE_1_ALIGN_PROC];
	m_dStage2_Y_Offset = ParamFile()->Model_data.dRun_Offset_Y[STAGE_2_ALIGN_PROC];
	m_dStage3_Y_Offset = ParamFile()->Model_data.dRun_Offset_Y[STAGE_3_ALIGN_PROC];
	m_dStage1_X_Offset = ParamFile()->Model_data.dRun_Offset_X[STAGE_1_ALIGN_PROC];
	m_dStage2_X_Offset = ParamFile()->Model_data.dRun_Offset_X[STAGE_2_ALIGN_PROC];
	m_dStage3_X_Offset = ParamFile()->Model_data.dRun_Offset_X[STAGE_3_ALIGN_PROC];

	UpdateData(FALSE);
}
//===================================================================================
void CdlgRecipeSet::Data_Save() 
{
}
//===================================================================================
void CdlgRecipeSet::DispDisable() 
{
	GetDlgItem(IDC_EDIT_X_LEN_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RUN_SPEED_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_X_LEN_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RUN_SPEED_4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE); 

	GetDlgItem(IDC_EDIT_X_LEN_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RUN_SPEED_5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);

	GetDlgItem(IDC_RADIO11)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO12)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO13)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO14)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO15)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO16)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_X1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_X2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_X3)->EnableWindow(FALSE);

	GetDlgItem(IDB_ENABLE_3)->EnableWindow(FALSE);
	GetDlgItem(IDB_ENABLE_7)->EnableWindow(FALSE);

}
//===================================================================================
BEGIN_EVENTSINK_MAP(CdlgRecipeSet, CDialog)
	//{{AFX_EVENTSINK_MAP(CdlgRecipeSet)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_0, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel0, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_1, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel1, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_2, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel2, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_3, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel3, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_4, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel4, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_5, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel5, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_6, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel6, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_7, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel7, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_8, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel8, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_9, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel9, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RECIPE_SEL_10, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickRecipeSel10, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RESULT_ADD2, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickResultAdd2, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RESULT_ADD3, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickResultAdd3, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RESULT_ADD, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickResultAdd, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RESULT_DEL, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickResultDel, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RESULT_DELALL, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickResultDelall, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_RESULT_SAVE, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickResultSave, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_MODEL_DATA, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickModelData, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDOK4, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickOk4, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_ENABLE_3, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickEnable3, VTS_NONE)
	ON_EVENT(CdlgRecipeSet, IDB_ENABLE_7, DISPID_CLICK /* Click */, CdlgRecipeSet::OnClickEnable7, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel0() 
{
	DispDisable() ;	
	m_iOP_Command= _OP_MOVE_WAIT_ALL;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel1() 
{
	DispDisable() ;	
	m_iOP_Command= _OP_MOVE_WAIT_X;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel2() 
{
	DispDisable() ;	
	m_iOP_Command= _OP_MOVE_WAIT_Y;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel3() 
{
	DispDisable() ;	
	m_iOP_Command= _OP_MOVE_WAIT_RZ;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel4() 
{
	DispDisable() ;	
	GetDlgItem(IDC_EDIT_X_LEN_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RUN_SPEED_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_X1)->EnableWindow(TRUE);
	UpdateData(TRUE);
	m_iOP_Command= _OP_PROC_STAGE1;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel5() 
{
	DispDisable() ;	
	GetDlgItem(IDC_EDIT_X_LEN_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RUN_SPEED_4)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_X2)->EnableWindow(TRUE);
	UpdateData(TRUE);
	m_iOP_Command= _OP_PROC_STAGE2;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel6() 
{
	DispDisable() ;	
	GetDlgItem(IDC_EDIT_X_LEN_3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RUN_SPEED_5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_X3)->EnableWindow(TRUE);
	UpdateData(TRUE);
	m_iOP_Command= _OP_PROC_STAGE3;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel7() 
{
	DispDisable() ;	
	m_iOP_Command= _OP_PROC_PLATE_ALIGN;
	GetDlgItem(IDB_ENABLE_3)->EnableWindow(TRUE);
	GetDlgItem(IDB_ENABLE_7)->EnableWindow(TRUE);
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel8() 
{
	DispDisable() ;
	GetDlgItem(IDC_RADIO11)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO12)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO13)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO14)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO15)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO16)->EnableWindow(TRUE);
	UpdateData(TRUE);
	m_iOP_Command= _OP_PROC_VACUUM;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel9() 
{
	DispDisable() ;
	GetDlgItem(IDC_EDIT4)->EnableWindow(TRUE);
	UpdateData(TRUE);
	m_iOP_Command= _OP_PROC_DELAY;
}
//===================================================================================
void CdlgRecipeSet::OnClickRecipeSel10() 
{
	DispDisable() ;
	GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
	UpdateData(TRUE);
	m_iOP_Command= _OP_PROC_MESSAGE;
}
//===================================================================================
void CdlgRecipeSet::OnClickResultAdd2() 
{
	UpdateData(TRUE);
	int SelNo= -1;
	int i= 0, j= 0;

	int		cmd;
	double	op1;
	double	op2;
	double	op3;
	double	op4;
	double	op5;
	TCHAR	char_op[255];

	CString str;

	int CntNo= m_ListCtrl.GetItemCount();

	for(i= 0; i< CntNo; i++) {
		if(m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) { SelNo= i; break; }
	}

	if(SelNo> 0)
	{
		cmd= ParamFile()->Recipe_data[SelNo-1].iCommand;
		op1= ParamFile()->Recipe_data[SelNo-1].dOP_Val1;
		op2= ParamFile()->Recipe_data[SelNo-1].dOP_Val2;
		op3= ParamFile()->Recipe_data[SelNo-1].dOP_Val3;
		op4= ParamFile()->Recipe_data[SelNo-1].dOP_Val4;
		op5= ParamFile()->Recipe_data[SelNo-1].dOP_Val5;
		_tcscpy(char_op, ParamFile()->Recipe_data[SelNo-1].chrOP_Val);

		ParamFile()->Recipe_data[SelNo-1].iCommand	=   ParamFile()->Recipe_data[SelNo].iCommand;
		ParamFile()->Recipe_data[SelNo-1].dOP_Val1= ParamFile()->Recipe_data[SelNo].dOP_Val1;
		ParamFile()->Recipe_data[SelNo-1].dOP_Val2 = ParamFile()->Recipe_data[SelNo].dOP_Val2;
		ParamFile()->Recipe_data[SelNo-1].dOP_Val3 = ParamFile()->Recipe_data[SelNo].dOP_Val3;
		ParamFile()->Recipe_data[SelNo-1].dOP_Val4 = ParamFile()->Recipe_data[SelNo].dOP_Val4;
		ParamFile()->Recipe_data[SelNo-1].dOP_Val5 = ParamFile()->Recipe_data[SelNo].dOP_Val5;
		_tcscpy(ParamFile()->Recipe_data[SelNo-1].chrOP_Val, ParamFile()->Recipe_data[SelNo].chrOP_Val);

		ParamFile()->Recipe_data[SelNo].iCommand= cmd;
		ParamFile()->Recipe_data[SelNo].dOP_Val1= op1;
		ParamFile()->Recipe_data[SelNo].dOP_Val2= op2;
		ParamFile()->Recipe_data[SelNo].dOP_Val3= op3;
		ParamFile()->Recipe_data[SelNo].dOP_Val4= op4;
		ParamFile()->Recipe_data[SelNo].dOP_Val5= op5;
		_tcscpy(ParamFile()->Recipe_data[SelNo].chrOP_Val, char_op);

		ListUpdate();

		CntNo= m_ListCtrl.GetItemCount();
		for(i= 0; i< CntNo; i++) {
			m_ListCtrl.SetItemState(i, 0, LVIS_SELECTED);
		}
		SelNo = SelNo-1;
		if(SelNo >= CntNo) SelNo= CntNo-1;
	}
	m_ListCtrl.SetItemState(SelNo, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
	m_ListCtrl.SetItemState(SelNo , 0, LVIS_FOCUSED);
	m_ListCtrl.SetFocus();
	UpdateData(FALSE);
}
//===================================================================================
void CdlgRecipeSet::OnClickResultAdd3() 
{
	UpdateData(TRUE);
	int SelNo= -1;
	int i= 0, j= 0;

	int		cmd;
	double	op1;
	double	op2;
	double	op3;
	double	op4;
	double	op5;
	TCHAR	char_op[255];

	CString str;

	int CntNo= m_ListCtrl.GetItemCount();

	for(i= 0; i< CntNo; i++) {
		if(m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) { SelNo= i; break; }
	}

	if(SelNo< CntNo-1)
	{
		cmd= ParamFile()->Recipe_data[SelNo+1].iCommand;
		op1= ParamFile()->Recipe_data[SelNo+1].dOP_Val1;
		op2= ParamFile()->Recipe_data[SelNo+1].dOP_Val2;
		op3= ParamFile()->Recipe_data[SelNo+1].dOP_Val3;
		op4= ParamFile()->Recipe_data[SelNo+1].dOP_Val4;
		op5= ParamFile()->Recipe_data[SelNo+1].dOP_Val5;
		_tcscpy(char_op, ParamFile()->Recipe_data[SelNo+1].chrOP_Val);

		ParamFile()->Recipe_data[SelNo+1].iCommand	=   ParamFile()->Recipe_data[SelNo].iCommand;
		ParamFile()->Recipe_data[SelNo+1].dOP_Val1= ParamFile()->Recipe_data[SelNo].dOP_Val1;
		ParamFile()->Recipe_data[SelNo+1].dOP_Val2 = ParamFile()->Recipe_data[SelNo].dOP_Val2;
		ParamFile()->Recipe_data[SelNo+1].dOP_Val3 = ParamFile()->Recipe_data[SelNo].dOP_Val3;
		ParamFile()->Recipe_data[SelNo+1].dOP_Val4 = ParamFile()->Recipe_data[SelNo].dOP_Val4;
		ParamFile()->Recipe_data[SelNo+1].dOP_Val5 = ParamFile()->Recipe_data[SelNo].dOP_Val5;
		_tcscpy(ParamFile()->Recipe_data[SelNo+1].chrOP_Val, ParamFile()->Recipe_data[SelNo].chrOP_Val);

		ParamFile()->Recipe_data[SelNo].iCommand= cmd;
		ParamFile()->Recipe_data[SelNo].dOP_Val1= op1;
		ParamFile()->Recipe_data[SelNo].dOP_Val2= op2;
		ParamFile()->Recipe_data[SelNo].dOP_Val3= op3;
		ParamFile()->Recipe_data[SelNo].dOP_Val4= op4;
		ParamFile()->Recipe_data[SelNo].dOP_Val5= op5;
		_tcscpy(ParamFile()->Recipe_data[SelNo].chrOP_Val, char_op);

		ListUpdate();

		CntNo= m_ListCtrl.GetItemCount();
		for(i= 0; i< CntNo; i++) {
			m_ListCtrl.SetItemState(i, 0, LVIS_SELECTED);
		}
		SelNo = SelNo+1;
		if(SelNo >= CntNo) SelNo= CntNo-1;
	}
	m_ListCtrl.SetItemState(SelNo, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
	m_ListCtrl.SetItemState(SelNo , 0, LVIS_FOCUSED);
	m_ListCtrl.SetFocus();
	UpdateData(FALSE);
}
//===================================================================================
void CdlgRecipeSet::OnClickResultAdd() 
{
	//	int i, j;
	//	ListClear();
	UpdateData(TRUE);

	int CntNo= m_ListCtrl.GetItemCount();

	int i, SelNo;
	SelNo= CntNo;
	for(i= 0; i< CntNo; i++) {
		if(m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) { SelNo= i; break; }
	}
	if(m_iOP_Command< 1) 
	{
		AfxMessageBox(_T("항목 미선택!!!")); return;
	}
	if(SelNo< 0) SelNo= CntNo;


	switch(m_iOP_Command)
	{
	case _OP_NONE:				
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//0
	case _OP_MOVE_WAIT_ALL:		
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//1
	case _OP_MOVE_WAIT_X:		
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//2
	case _OP_MOVE_WAIT_Y:		
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//3
	case _OP_MOVE_WAIT_RZ:		
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//4
	case _OP_PROC_STAGE1:		
//		m_dOP_Val1= m_dStage1_Len;			m_dOP_Val2= m_dStage1_Speed;
		m_dOP_Val1= ParamFile()->Model_data.dRun_Length[STAGE_1_ALIGN_PROC]= m_dStage1_Len;
		m_dOP_Val2= ParamFile()->Model_data.dRun_Speed[STAGE_1_ALIGN_PROC]= m_dStage1_Speed;
		m_dOP_Val3= ParamFile()->Model_data.dRun_Offset_X[STAGE_1_ALIGN_PROC]= m_dStage1_X_Offset;
		m_dOP_Val4= ParamFile()->Model_data.dRun_Offset_Y[STAGE_1_ALIGN_PROC]= m_dStage1_Y_Offset;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//5
	case _OP_PROC_STAGE2:		
//		m_dOP_Val1= m_dStage2_Len;		m_dOP_Val2= m_dStage2_Speed;
		m_dOP_Val1= ParamFile()->Model_data.dRun_Length[STAGE_2_ALIGN_PROC]= m_dStage2_Len;
		m_dOP_Val2= ParamFile()->Model_data.dRun_Speed[STAGE_2_ALIGN_PROC]= m_dStage2_Speed;
		m_dOP_Val3= ParamFile()->Model_data.dRun_Offset_X[STAGE_2_ALIGN_PROC]= m_dStage2_X_Offset;
		m_dOP_Val4= ParamFile()->Model_data.dRun_Offset_Y[STAGE_2_ALIGN_PROC]= m_dStage2_Y_Offset;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//5
	case _OP_PROC_STAGE3:		
//		m_dOP_Val1= m_dStage3_Len;		m_dOP_Val2= m_dStage3_Speed;
		m_dOP_Val1= ParamFile()->Model_data.dRun_Length[STAGE_3_ALIGN_PROC]= m_dStage3_Len;
		m_dOP_Val2= ParamFile()->Model_data.dRun_Speed[STAGE_3_ALIGN_PROC]= m_dStage3_Speed;
		m_dOP_Val3= ParamFile()->Model_data.dRun_Offset_X[STAGE_3_ALIGN_PROC]= m_dStage3_X_Offset;	
		m_dOP_Val4= ParamFile()->Model_data.dRun_Offset_Y[STAGE_3_ALIGN_PROC]= m_dStage3_Y_Offset;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//7
	case _OP_PROC_PLATE_ALIGN:		
		m_dOP_Val1= (int)SelAlignVal;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//8
	case _OP_PROC_VACUUM:		
		m_dOP_Val1= (double)(m_iVacOnOffSel1);
		m_dOP_Val2= (double)(m_iVacOnOffSel2);		m_dOP_Val3= (double)(m_iVacOnOffSel3);  	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//9
	case _OP_PROC_DELAY:		
		m_dOP_Val1= (double)(m_iDelayTime);
		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0;  	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//10
	case _OP_PROC_MESSAGE:		
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0;  	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, m_strMessage.GetBuffer(0));
		break;//11
	case _OP_PROC_END:			
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;//12
	default:					
		m_dOP_Val1= 0.0;		m_dOP_Val2= 0.0;		m_dOP_Val3= 0.0; 	m_dOP_Val4= 0.0;	m_dOP_Val5= 0.0;
		_tcscpy(m_chrOP_Val, _T(" "));
		break;
	}

	if(CntNo> 0)
	{
		for(i= CntNo; i>= SelNo; i--)
		{
			ParamFile()->Recipe_data[i+1].iCommand= ParamFile()->Recipe_data[i].iCommand;
			ParamFile()->Recipe_data[i+1].dOP_Val1= ParamFile()->Recipe_data[i].dOP_Val1;
			ParamFile()->Recipe_data[i+1].dOP_Val2= ParamFile()->Recipe_data[i].dOP_Val2;
			ParamFile()->Recipe_data[i+1].dOP_Val3= ParamFile()->Recipe_data[i].dOP_Val3;
			ParamFile()->Recipe_data[i+1].dOP_Val4= ParamFile()->Recipe_data[i].dOP_Val4;
			ParamFile()->Recipe_data[i+1].dOP_Val5= ParamFile()->Recipe_data[i].dOP_Val5;
			//int len= sizeof(ParamFile()->Recipe_data[i].chrOP_Val);
			//int len1= sizeof(ParamFile()->Recipe_data[i+1].chrOP_Val);
			//_tcscpy_s(ParamFile()->Recipe_data[i+1].chrOP_Val, 255, ParamFile()->Recipe_data[i].chrOP_Val);
//			_stprintf(ParamFile()->Recipe_data[i+1].chrOP_Val, L"%s", ParamFile()->Recipe_data[i].chrOP_Val);
			_tcscpy(ParamFile()->Recipe_data[i+1].chrOP_Val, ParamFile()->Recipe_data[i].chrOP_Val);
		}
	}
	ParamFile()->Recipe_data[SelNo].iCommand= m_iOP_Command;
	ParamFile()->Recipe_data[SelNo].dOP_Val1= m_dOP_Val1;
	ParamFile()->Recipe_data[SelNo].dOP_Val2= m_dOP_Val2;
	ParamFile()->Recipe_data[SelNo].dOP_Val3= m_dOP_Val3;
	ParamFile()->Recipe_data[SelNo].dOP_Val4= m_dOP_Val4;
	ParamFile()->Recipe_data[SelNo].dOP_Val5= m_dOP_Val5;
	_tcscpy(ParamFile()->Recipe_data[SelNo].chrOP_Val, m_chrOP_Val);
//	_stprintf(ParamFile()->Recipe_data[SelNo].chrOP_Val, L"%s", m_chrOP_Val);

	ListUpdate();
}
//===================================================================================
void CdlgRecipeSet::OnClickResultDel() 
{
	UpdateData(TRUE);
	int SelNo= -1;
	int i= 0, j= 0;

	CString str;

	int CntNo= m_ListCtrl.GetItemCount();

	for(i= 0; i< CntNo; i++) {
		if(m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) { SelNo= i; break; }
	}

	if(SelNo>= 0)
	{
		for(i= SelNo, j= SelNo; i< CntNo-1; i++)
		{
			ParamFile()->Recipe_data[i].iCommand= ParamFile()->Recipe_data[i+1].iCommand;
			ParamFile()->Recipe_data[i].dOP_Val1= ParamFile()->Recipe_data[i+1].dOP_Val1;
			ParamFile()->Recipe_data[i].dOP_Val2= ParamFile()->Recipe_data[i+1].dOP_Val2;
			ParamFile()->Recipe_data[i].dOP_Val3= ParamFile()->Recipe_data[i+1].dOP_Val3;
			ParamFile()->Recipe_data[i].dOP_Val4= ParamFile()->Recipe_data[i+1].dOP_Val4;
			ParamFile()->Recipe_data[i].dOP_Val5= ParamFile()->Recipe_data[i+1].dOP_Val5;
			_tcscpy(ParamFile()->Recipe_data[i].chrOP_Val, ParamFile()->Recipe_data[i+1].chrOP_Val);
		}
		ParamFile()->Recipe_data[i].iCommand= -1;
		ParamFile()->Recipe_data[i].dOP_Val1= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val2= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val3= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val4= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val5= 0.0;
		_tcscpy(ParamFile()->Recipe_data[i].chrOP_Val, _T(" "));

		ListUpdate();

		CntNo= m_ListCtrl.GetItemCount();
		for(i= 0; i< CntNo; i++) {
			m_ListCtrl.SetItemState(i, 0, LVIS_SELECTED);
		}
		if(SelNo >= CntNo) SelNo= CntNo-1;
		m_ListCtrl.SetItemState(SelNo, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
		m_ListCtrl.SetItemState(SelNo , 0, LVIS_FOCUSED);
		m_ListCtrl.SetFocus();
		UpdateData(FALSE);
	}
}
//===================================================================================
void CdlgRecipeSet::OnClickResultDelall() 
{
	int i;
	for(i= 0; i< _MAX_RECIPE; i++)
	{
		ParamFile()->Recipe_data[i].iCommand= -1;
		ParamFile()->Recipe_data[i].dOP_Val1= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val2= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val3= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val4= 0.0;
		ParamFile()->Recipe_data[i].dOP_Val5= 0.0;
		_tcscpy(ParamFile()->Recipe_data[i].chrOP_Val, _T(" "));
	}
	ListUpdate();	
}
//===================================================================================
void CdlgRecipeSet::OnClickResultSave() 
{

	//	ParamFile()->SlotDie_data.iSyringeMoveMode= _SYRINGE_MOVE_MODE_TIME;
	if (MyMessageBox(_T("입력된 값을 저장 하시겠습니까? ."), _T("확인"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO) {
		Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => OnClickDataSave.! => CANCEL"));
		return;
	}
	Log()->SaveLogFile(_SYSTEM_LOG, _T("[SAVE] => OnClickDataSave.!"));

// 	if(!wcscmp(ParamFile()->System_data.strCurrentRecipeName, _T("Default")))
// 	{
// 		ParamFile()->OpenRecipeConfigFile(ParamFile()->System_data.strCurrentRecipeName);
// 	}
// 	else if(!wcscmp(ParamFile()->System_data.strCurrentRecipeName, _T("default")))
// 	{
// 		ParamFile()->OpenRecipeConfigFile(ParamFile()->System_data.strCurrentRecipeName);
// 	}
// 	else
// 	{
		ParamFile()->SaveModelConfigFile(ParamFile()->System_data.strCurrentModelName);
		ParamFile()->SaveRecipeConfigFile(ParamFile()->System_data.strCurrentRecipeName);
		ParamFile()->OpenRecipeConfigFile(ParamFile()->System_data.strCurrentRecipeName);
// 	}
	ListUpdate();
//	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
// 	pFrm->DataUpdateSend();
// */
}
//===================================================================================
void CdlgRecipeSet::OnClickModelData() 
{
}
//===================================================================================
void CdlgRecipeSet::ListUpdate()
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
// 				strOP1.Format(_T("L:%.3f mm"), (ParamFile()->Recipe_data[cnt].dOP_Val1));	
// 				strOP2.Format(_T("S:%.3f mm"), (ParamFile()->Recipe_data[cnt].dOP_Val2));	
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
				str.Format(_T("Align 동작"));			
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				if	   (ParamFile()->Recipe_data[cnt].dOP_Val1== 0)		strOP1.Format(_T("Roll-Align"));
				else if(ParamFile()->Recipe_data[cnt].dOP_Val1== 1)		strOP1.Format(_T("Stage Align"));
				else if(ParamFile()->Recipe_data[cnt].dOP_Val1== 2)		strOP1.Format(_T("Stage #2-Align"));
				else if(ParamFile()->Recipe_data[cnt].dOP_Val1== 3)		strOP1.Format(_T("Stage #3-Align"));
				else	{												strOP1.Format(_T("Roll-Align")); ParamFile()->Recipe_data[cnt].dOP_Val1= 0; }
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
				break;//9
			case _OP_PROC_VACUUM:		
				str.Format(_T("진공 On/Off 동작"));			
				strOP1.Format(_T(" "));	strOP2.Format(_T(" "));	strOP3.Format(_T(" "));	
				strOP4.Format(_T(" "));	strOP5.Format(_T(" "));	strcharOP.Format(_T(" "));
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
}
//===================================================================================
void CdlgRecipeSet::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	int SelNo= -1;
	int i= 0, j= 0;

	CString str;

	int CntNo= m_ListCtrl.GetItemCount();

	for(i= 0; i< CntNo; i++) {
		if(m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) { SelNo= i; break; }
	}
	if(ParamFile()->Recipe_data[SelNo].iCommand== _OP_PROC_STAGE1 || ParamFile()->Recipe_data[SelNo].iCommand== _OP_PROC_STAGE2 || 
		ParamFile()->Recipe_data[SelNo].iCommand== _OP_PROC_STAGE3)
	{
		int stageNo= 0;
		if(ParamFile()->Recipe_data[SelNo].iCommand== _OP_PROC_STAGE1)			stageNo= _STAGE_1;
		else if(ParamFile()->Recipe_data[SelNo].iCommand== _OP_PROC_STAGE2)		stageNo= _STAGE_2;
		else if(ParamFile()->Recipe_data[SelNo].iCommand== _OP_PROC_STAGE3)		stageNo= _STAGE_3;
		double speed=0.0, len= 0.0;
		if(SelNo>= 0)
		{
			CdlgRecipePrintEdit dlg;
			dlg.iCMDNo= ParamFile()->Recipe_data[SelNo].iCommand;

			dlg.dOp1= ParamFile()->Recipe_data[SelNo].dOP_Val1;	
			dlg.dOp2= ParamFile()->Recipe_data[SelNo].dOP_Val2;	
			dlg.dOp3= ParamFile()->Recipe_data[SelNo].dOP_Val3;	
			dlg.dX= ParamFile()->Recipe_data[SelNo].dOP_Val3;	
			dlg.dY= ParamFile()->Recipe_data[SelNo].dOP_Val4;	
			dlg.dT= ParamFile()->Recipe_data[SelNo].dOP_Val5;	
			int ret= dlg.DoModal();
			if(ret== IDOK)
			{
				ParamFile()->Recipe_data[SelNo].dOP_Val1= dlg.dOp1;	
				ParamFile()->Recipe_data[SelNo].dOP_Val2= dlg.dOp2;	
				//ParamFile()->Recipe_data[SelNo].dOP_Val3= dlg.dOp3;	
				ParamFile()->Recipe_data[SelNo].dOP_Val3= dlg.dX;
				ParamFile()->Recipe_data[SelNo].dOP_Val4= dlg.dY;
				ParamFile()->Recipe_data[SelNo].dOP_Val5= dlg.dT;
				ListUpdate();
			}

		}
	}


	*pResult = 0;
}
//===================================================================================
void CdlgRecipeSet::OnClickOk4() 
{
	OnOK();
}
//===================================================================================
void CdlgRecipeSet::OnClickEnable3() 
{
	m_btnAlignR.SetValue(1);	m_btnAlign1.SetValue(0);
	SelAlignVal=0;
}
//===================================================================================
void CdlgRecipeSet::OnClickEnable7() 
{
	m_btnAlignR.SetValue(0);	m_btnAlign1.SetValue(1);
	SelAlignVal=1;
}
//===================================================================================
void CdlgRecipeSet::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
//===================================================================================
