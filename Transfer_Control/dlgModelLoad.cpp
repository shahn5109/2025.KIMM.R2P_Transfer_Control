// dlgModelLoad.cpp : implementation file
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgModelLoad.h"
#include "dlgModelNameInput.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdlgModelLoad dialog
extern CSystemClass pSystem;
CParameterFile* CdlgModelLoad::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }


CdlgModelLoad::CdlgModelLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CdlgModelLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CdlgModelLoad)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	strChangeModelName= _T("");
}


void CdlgModelLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdlgModelLoad)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CdlgModelLoad, CDialog)
	//{{AFX_MSG_MAP(CdlgModelLoad)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CdlgModelLoad message handlers

BOOL CdlgModelLoad::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CdlgModelLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ListCtrl.InsertColumn(0, _T("Model-List"), LVCFMT_LEFT, 300, 0);
	ListClear();

	CString strText;

	FileList();
	strText.Format(_T("%s"), ParamFile()->System_data.strCurrentModelName);
	SetDlgItemText(IDS_LABEL_TOTAL_MODELNAME, strText.GetBuffer(0));
	SetDlgItemText(IDS_LABEL_CUR_MODELNAME, strText.GetBuffer(0));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CdlgModelLoad::FileList()
{ 
	//	char szParamFile[255];
	CString strPath;

	//	sprintf(szParamFile, "%s\\%s.Model_", ParamFile()->strDirConfigPath, strName );

	strPath.Format(_T("%s\\*.Model_"), ParamFile()->strDirConfigPath);
	CFileFind finder;
	BOOL bWorking = finder.FindFile( strPath );
	while ( bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() )		continue;

		if ( finder.IsDirectory() )
		{
			continue;
			//FileFind( finder.GetFilePath() );
		}
		else
		{
			CString trFile;

			CString strFilePath = (LPCTSTR)finder.GetFilePath();
			CString strFileName = (LPCTSTR)finder.GetFileName();
			int retssLen= strFileName.GetLength(); 
			strFileName= strFileName.Left(retssLen- 7);
			ListPrint(strFileName);
		}
	}
	finder.Close();
}




void CdlgModelLoad::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

}

BEGIN_EVENTSINK_MAP(CdlgModelLoad, CDialog)
	//{{AFX_EVENTSINK_MAP(CdlgModelLoad)
	ON_EVENT(CdlgModelLoad, IDB_MODEL_SAVE, -600 /* Click */, CdlgModelLoad::OnClickModelSave, VTS_NONE)
	ON_EVENT(CdlgModelLoad, IDOK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CdlgModelLoad, IDB_MODEL_LOAD, -600 /* Click */, CdlgModelLoad::OnClickModelLoad, VTS_NONE)
	ON_EVENT(CdlgModelLoad, IDB_MODEL_SAVE2, -600 /* Click */, CdlgModelLoad::OnClickModelSave2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CdlgModelLoad::OnClickModelSave() 
{
	CdlgModelNameInput dlg;
	dlg.inputmodelType= 0;
	dlg.strBeforeModelNAme= ParamFile()->System_data.strCurrentModelName;
	int ret= dlg.DoModal();
	if(ret== IDOK) {
		ListClear();
		FileList();
		CString strText;
		ParamFile()->SaveFile(dlg.strAfterModelName);	
		strText.Format(_T("%s"), ParamFile()->System_data.strCurrentModelName);
		SetDlgItemText(IDS_LABEL_TOTAL_MODELNAME, strText.GetBuffer(0));
		SetDlgItemText(IDS_LABEL_CUR_MODELNAME, strText.GetBuffer(0));
	}
}
void CdlgModelLoad::ListPrint(CString strText)
{
	CString str; 
	str.Format(_T("%s"), strText); 			m_ListCtrl.InsertItem(0, str); 
}

void CdlgModelLoad::ListClear()
{

	m_ListCtrl.DeleteAllItems ();

	m_ListCtrl.SetBkColor(RGB(255,230,230));
	m_ListCtrl.SetTextBkColor(RGB(255,230,230));
	// 리스트 컨트롤의 행 선택,Flat,라인.. 스타일
	m_ListCtrl.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES);
}

void CdlgModelLoad::OnClickOk() 
{
	OnOK();
}

void CdlgModelLoad::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// 	TRACE("%d, %d\n", pNMListView->iItem, pNMListView->iSubItem);
	int nRow= pNMListView->iItem;

	strChangeModelName = m_ListCtrl.GetItemText(nRow, 0);

	SetDlgItemText(IDS_LABEL_CUR_MODELNAME, strChangeModelName.GetBuffer(0));

	*pResult = 0;
}

void CdlgModelLoad::OnClickModelLoad() 
{
	CString strText;
	ParamFile()->ModelNameChangeFile(strChangeModelName);	
	strText.Format(_T("%s"), ParamFile()->System_data.strCurrentModelName);
	SetDlgItemText(IDS_LABEL_TOTAL_MODELNAME, strText.GetBuffer(0));
}

void CdlgModelLoad::OnClickModelSave2() 
{
	CTime time = CTime::GetCurrentTime();

	CString strPath;
	CString strPath2;
	strPath.Format(_T("%s\\%s.Model_"), ParamFile()->strDirConfigPath, strChangeModelName);
	strPath2.Format(_T("%s\\%s._%04d%02d%02d_Model_"), ParamFile()->strDirConfigPath, strChangeModelName, 
		time.GetYear(), time.GetMonth(), time.GetDay());
	::MoveFile(strPath.GetBuffer(0), strPath2.GetBuffer(0));

	ListClear();
	FileList();
}
