// dlgModelNameInput.cpp : implementation file
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgModelNameInput.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdlgModelNameInput dialog

extern CSystemClass pSystem;
CParameterFile* CdlgModelNameInput::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

CdlgModelNameInput::CdlgModelNameInput(CWnd* pParent /*=NULL*/)
	: CDialog(CdlgModelNameInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CdlgModelNameInput)
	m_strAfterModelName = _T("");
	//}}AFX_DATA_INIT
}


void CdlgModelNameInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdlgModelNameInput)
	DDX_Text(pDX, IDC_EDIT_MODELNAME, m_strAfterModelName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CdlgModelNameInput, CDialog)
	//{{AFX_MSG_MAP(CdlgModelNameInput)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CdlgModelNameInput message handlers
BOOL CdlgModelNameInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_brush.CreateSolidBrush(BackColor); 	brush1.CreateSolidBrush(RGB(255, 0, 0)); 
	m_GreenBrush.CreateSolidBrush(RGB(212, 255, 212));	m_BlueBrush.CreateSolidBrush(RGB(212, 212, 255));
	m_YellowBrush.CreateSolidBrush(RGB(255, 255, 212));	m_RedBrush.CreateSolidBrush(RGB(255, 212, 212));
	m_GrayBrush.CreateSolidBrush(RGB(196, 196, 196));
	CFont font;
	font.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET,	3, 2, 1,VARIABLE_PITCH | FF_ROMAN, _T("바탕"));
	GetDlgItem(IDC_EDIT_MODELNAME)->SetFont(&font, TRUE);	font.Detach();	font.DeleteObject();
	
	SetDlgItemText(IDS_LABEL_TOTAL_MODELNAME, strBeforeModelNAme.GetBuffer(0));
	SetDlgItemText(IDS_LABEL_CUR_MODELNAME, strBeforeModelNAme.GetBuffer(0));
	SetDlgItemText(IDC_EDIT_MODELNAME, strBeforeModelNAme.GetBuffer(0));

	if(inputmodelType== 0)
	{		
		SetDlgItemText(IDS_COMMAND4, _T("모델 파일 로딩"));
		SetDlgItemText(IDS_COMMAND3, _T("현재 Model 명"));
		SetDlgItemText(IDS_COMMAND5, _T("변경 Model 명"));
	}
	else
	{
		SetDlgItemText(IDS_COMMAND4, _T("Recipe 파일 로딩"));
		SetDlgItemText(IDS_COMMAND3, _T("현재 Recipe 명"));
		SetDlgItemText(IDS_COMMAND5, _T("변경 Recipe 명"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CdlgModelNameInput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
//	pDC->SetBkMode(TRANSPARENT); 
//	if(pWnd->GetDlgCtrlID()==IDS_LABEL_CUR_MODELNAME) {	pDC->SetTextColor(RGB(0, 100, 0));		return (HBRUSH)m_GreenBrush;	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CdlgModelNameInput::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}

void CdlgModelNameInput::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_brush.DeleteObject (); 	brush1.DeleteObject (); 
	m_RedBrush.DeleteObject();	m_GreenBrush.DeleteObject();
	m_BlueBrush.DeleteObject();	m_YellowBrush.DeleteObject();
	m_GrayBrush.DeleteObject();
	
}

BEGIN_EVENTSINK_MAP(CdlgModelNameInput, CDialog)
    //{{AFX_EVENTSINK_MAP(CdlgModelNameInput)
	ON_EVENT(CdlgModelNameInput, IDOK, -600 /* Click */, CdlgModelNameInput::OnClickOk, VTS_NONE)
	ON_EVENT(CdlgModelNameInput, IDOK2, -600 /* Click */, CdlgModelNameInput::OnClickCancel, VTS_NONE)
	ON_EVENT(CdlgModelNameInput, IDS_LABEL_CUR_MODELNAME, -600 /* Click */, OnClickLabelCurModelname, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CdlgModelNameInput::OnClickOk() 
{
	UpdateData(TRUE);
	
	CString strPath;
	CString strToPath;
	
	if(inputmodelType== 0)
	{
		strPath.Format(_T("%s\\%s.*"), ParamFile()->strDirConfigPath, ParamFile()->System_data.strCurrentModelName);
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
				trFile.Format(_T("%s"), ParamFile()->System_data.strCurrentModelName);
				
				CString strFilePath = (LPCTSTR)finder.GetFilePath();
				CString strFileName = (LPCTSTR)finder.GetFileName();
				
				int retssLen= trFile.GetLength(); 
				int retssLen2= strFileName.GetLength();
				
				strFileName= strFileName.Right(retssLen2- retssLen);
				strPath.Format(_T("%s\\%s%s"), 
					ParamFile()->strDirConfigPath, ParamFile()->System_data.strCurrentModelName, strFileName);
				strToPath.Format(_T("%s\\%s%s"), 
					ParamFile()->strDirConfigPath, m_strAfterModelName, strFileName);
				CopyFile((LPCTSTR)strPath, (LPCTSTR)strToPath, FALSE);

				CString strModelNamePathOld;
				CString strModelNamePathNew;
				int i;
				for(i= 0; i< _MAX_FIND_MODEL; i++)	
				{//m_strAfterModelName
					strModelNamePathOld.Format(_T("%s\\P_Model_%d_%s"), ParamFile()->strDirConfigPath, i, ParamFile()->System_data.strCurrentModelName);
					strModelNamePathNew.Format(_T("%s\\P_Model_%d_%s"), ParamFile()->strDirConfigPath, i, m_strAfterModelName);
					CopyFile((LPCTSTR)strModelNamePathOld, (LPCTSTR)strModelNamePathNew, FALSE);
				}

			}
		}
		finder.Close();
	}
	else
	{
		strPath.Format(_T("%s\\%s.*"), ParamFile()->strDirConfigPath, ParamFile()->System_data.strCurrentRecipeName);
		//		CFileFind finder;
		//		BOOL bWorking = finder.FindFile( strPath );
		/*
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
			trFile.Format("%s", ParamFile()->System_data.strCurrentRecipeName);
			
			  CString strFilePath = (LPCTSTR)finder.GetFilePath();
			  CString strFileName = (LPCTSTR)finder.GetFileName();
			  
				int retssLen= trFile.GetLength(); 
				int retssLen2= strFileName.GetLength();
				
				  strFileName= strFileName.Right(retssLen2- retssLen);
				  strPath.Format("%s\\%s%s", 
				  ParamFile()->strDirConfigPath, ParamFile()->System_data.strCurrentRecipeName, strFileName);
				  strToPath.Format("%s\\%s%s", 
				  ParamFile()->strDirConfigPath, m_strAfterModelName, strFileName);
				  CopyFile((LPCTSTR)strPath, (LPCTSTR)strToPath, FALSE);
				  }
				  }
				  finder.Close();
		*/
		CString trFile;
		trFile.Format(_T("%s"), ParamFile()->System_data.strCurrentRecipeName);
		strPath.Format(_T("%s\\%s.Recipe_"), 
			ParamFile()->strDirConfigPath, ParamFile()->System_data.strCurrentRecipeName);
		strToPath.Format(_T("%s\\%s.Recipe_"), 
			ParamFile()->strDirConfigPath, m_strAfterModelName);
		CopyFile((LPCTSTR)strPath, (LPCTSTR)strToPath, FALSE);
		
	}
	//
	strAfterModelName= m_strAfterModelName;
	OnOK();
}

void CdlgModelNameInput::OnClickCancel() 
{
	OnCancel();
}

void CdlgModelNameInput::OnClickLabelCurModelname() 
{
	int axisNo= 0;
	TCHAR cAfterVal[35];
	TCHAR cBeforeVal[35];
	TCHAR strTitle[50] = _T(" ");
	CString strTmp;

	GetDlgItemText(IDS_LABEL_CUR_MODELNAME, strTmp);
	_stprintf(strTitle, _T("%s -> 기존 모델 명"), strBeforeModelNAme);
	
	/** 새로운 목표 위치 입력 받기 **/
	_stprintf(cBeforeVal, _T("%s"), strBeforeModelNAme);
	if(!GetValueBox(cAfterVal, 30, cBeforeVal, strTitle)) { 	return; }
	
	if (cAfterVal == _T("") || cAfterVal[0]== 0x00)		{ MyMessageBox(_T("입력치가 부적절 합니다!"), _T("입력값 오류"), M_ICONERROR); return; }
	
	m_strAfterModelName.Format(_T("%s"), cAfterVal);
	SetDlgItemText(IDS_LABEL_CUR_MODELNAME, m_strAfterModelName);
	SetDlgItemText(IDS_LABEL_CUR_MODELNAME, m_strAfterModelName);
	
}
