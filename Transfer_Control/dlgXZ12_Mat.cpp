// dlgXZ12_Mat.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Transfer_Control.h"
#include "dlgXZ12_Mat.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CdlgXZ12_Mat 대화 상자입니다.
extern CSystemClass pSystem;
CParameterFile* CdlgXZ12_Mat::ParamFile() {	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CdlgXZ12_Mat::Log()		{	return (CParameterFile*)(pSystem.GetParameterFileComponent()); }

IMPLEMENT_DYNAMIC(CdlgXZ12_Mat, CDialogEx)

CdlgXZ12_Mat::CdlgXZ12_Mat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdlgXZ12_Mat::IDD, pParent)
{
	m_selListData = 0.0;
	m_selListData2 = 0.0;
	m_dKgPerum = 0.0;
	m_dExcept_XPos = 0.0;
	m_dExcept_XPos2 = 0.0;
	m_iCalSelNo = -1;
	m_i1stVal = 0;
	m_i2ndVal = 0;
	m_dCalSumVal = 0;
}

CdlgXZ12_Mat::~CdlgXZ12_Mat()
{
}

void CdlgXZ12_Mat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_D1, m_ctrlDownload_1);
	DDX_Control(pDX, IDC_LIST_M1, m_ctrlCorrect_1);
	DDX_Control(pDX, IDC_LIST_B1, m_ctrlOriginal_1);
	DDX_Text(pDX, IDC_EDIT1, m_selListData);
	DDX_Text(pDX, IDC_EDIT3, m_selListData2);
	DDX_Text(pDX, IDC_EDIT2, m_dKgPerum);
	DDX_Text(pDX, IDC_EDIT7, m_dExcept_XPos);
	DDX_Text(pDX, IDC_EDIT8, m_dExcept_XPos2);
	DDX_Radio(pDX, IDC_RADIO17, m_iCalSelNo);
	DDX_Text(pDX, IDC_EDIT5, m_i1stVal);
	DDX_Text(pDX, IDC_EDIT9, m_i2ndVal);
	DDX_Text(pDX, IDC_EDIT10, m_dCalSumVal);
}


BEGIN_MESSAGE_MAP(CdlgXZ12_Mat, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON8, &CdlgXZ12_Mat::OnButtonLoadDataBuild)
	ON_BN_CLICKED(IDC_BUTTON13, &CdlgXZ12_Mat::OnButtonGraph_Load)
	ON_BN_CLICKED(IDC_BUTTON2, &CdlgXZ12_Mat::OnButtonMat_DownLoad)
	ON_BN_CLICKED(IDC_BUTTON12, &CdlgXZ12_Mat::OnButtonOrg_DownLoad)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_M1, &CdlgXZ12_Mat::OnDblclkListM1)
	ON_BN_CLICKED(IDC_BUTTON1, &CdlgXZ12_Mat::OnButton1)
	ON_BN_CLICKED(IDC_BUTTON14, &CdlgXZ12_Mat::OnButtondataInit)
	ON_BN_CLICKED(IDC_BUTTON3, &CdlgXZ12_Mat::OnButton3)
END_MESSAGE_MAP()


// CdlgXZ12_Mat 메시지 처리기입니다.


void CdlgXZ12_Mat::OnBnClickedButton14()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CdlgXZ12_Mat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrlDownload_1.InsertColumn(0, _T("No."), LVCFMT_LEFT, 42, 0); 
	m_ctrlDownload_1.InsertColumn(1, _T("X DATA"), LVCFMT_LEFT, 65, 0); 
	m_ctrlDownload_1.InsertColumn(2, _T("Z1 DATA"), LVCFMT_LEFT, 65, 0); 
	m_ctrlDownload_1.InsertColumn(3, _T("Z2 DATA"), LVCFMT_LEFT, 65, 0); 
	m_lListCount_Dn_1= 0;
	m_lListCount_Dn_2= 0;

	m_ctrlCorrect_1.InsertColumn(0, _T("No."), LVCFMT_LEFT, 42, 0); 
	m_ctrlCorrect_1.InsertColumn(1, _T("X DATA"), LVCFMT_LEFT, 65, 0); 
	m_ctrlCorrect_1.InsertColumn(2, _T("Z1 DATA"), LVCFMT_LEFT, 65, 0); 
	m_ctrlCorrect_1.InsertColumn(3, _T("Z2 DATA"), LVCFMT_LEFT, 65, 0); 
	m_lListCount_Cor_1= 0;
	m_lListCount_Cor_2= 0;

	m_ctrlOriginal_1.InsertColumn(0, _T("No."), LVCFMT_LEFT, 42, 0); 
	m_ctrlOriginal_1.InsertColumn(1, _T("X DATA"), LVCFMT_LEFT, 65, 0); 
	m_ctrlOriginal_1.InsertColumn(2, _T("Z1 DATA"), LVCFMT_LEFT, 65, 0); 
	m_ctrlOriginal_1.InsertColumn(3, _T("Z2 DATA"), LVCFMT_LEFT, 65, 0); 
	m_lListCount_Org_1= 0;
	m_lListCount_Org_2= 0;

	sel_listselno= -1;

	ListClear(0);
	ListClear(1);
	ListClear(2);

	Open_PMC_ORG_File();
	//	Open_PMC_CORRECT_File();

	ResultUpdate_Org();

	OpenMatDataFile();	
	ResultUpdate_Mat();
	CalMatdata();

	Build_PMC_DOWNLOAD_File();
	ResultUpdate_Down();

	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	m_dKgPerum=			ParamFile()->System_data.dKgPerum;
	m_dExcept_XPos= 	ParamFile()->System_data.dExcept_XPos;
	m_dExcept_XPos2= 	ParamFile()->System_data.dExcept_XPos2;

	CString strText;
	CString strText1;
	double L1_start= 0.0;
	double L2_start= 0.0;

	if(ParamFile()->Model_data.iRunEnable[STAGE_1_ALIGN_PROC]== RUN_ENABLE)
	{
			L1_start= PlotData_L1_1[0];		L2_start= PlotData_L2_1[0];
	}
	else if(ParamFile()->Model_data.iRunEnable[STAGE_2_ALIGN_PROC]== RUN_ENABLE)
	{
			L1_start= PlotData_L1_2[0];		L2_start= PlotData_L1_2[0];
	}
	else if(ParamFile()->Model_data.iRunEnable[STAGE_3_ALIGN_PROC]== RUN_ENABLE)
	{
			L1_start= PlotData_L1_3[0];		L2_start= PlotData_L1_2[0];
	}
	else
	{
		L1_start= PlotData_L1_1[0];		L2_start= PlotData_L2_1[0];
	}
	strText.Format(_T("보정 DATA [ %.2f kg ] 기준"), L1_start);
	strText1.Format(_T("Download 데이터 [ %.2f kg ] 기준"), L2_start);

	SetDlgItemText(IDS_STATIC_179, strText.GetBuffer(0));
	SetDlgItemText(IDS_STATIC_180, strText1.GetBuffer(0));

	m_iCalSelNo= 2;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CdlgXZ12_Mat::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CdlgXZ12_Mat::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN){  if((pMsg->wParam == VK_ESCAPE)){          return TRUE;        }    }
	return CDialog::PreTranslateMessage(pMsg);
}
void CdlgXZ12_Mat::ListClear(int no)
{
//	m_lListCount_Dn_1= -1;	m_lListCount_Dn_2= -1;	m_lListCount_Cor_1= -1;	m_lListCount_Cor_2= -1;	m_lListCount_Org_1= -1;	m_lListCount_Org_2= -1;
	
	if(no== 2)
	{
		m_ctrlDownload_1.DeleteAllItems ();	m_ctrlDownload_1.SetBkColor(RGB(255,235,235));	m_ctrlDownload_1.SetTextBkColor(RGB(255,230,230));
		m_ctrlDownload_1.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}
	
	if(no== 1)
	{
		m_ctrlCorrect_1.DeleteAllItems ();	m_ctrlCorrect_1.SetBkColor(RGB(235,255,235));	m_ctrlCorrect_1.SetTextBkColor(RGB(230,255,230));
		m_ctrlCorrect_1.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}
	
	if(no== 0)
	{
		m_ctrlOriginal_1.DeleteAllItems ();	m_ctrlOriginal_1.SetBkColor(RGB(235,235,255));	m_ctrlOriginal_1.SetTextBkColor(RGB(230,230,255));
		m_ctrlOriginal_1.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	}

}

/*
void CdlgLogData::OnButtonLens() 
{
	if(MenuOpenFlag) return;
	MenuOpenFlag= true;
	InitSel(); m_btnLens.SetColorTab(MDKBLUE);		m_btnLens.SetColor(MDKBLUE, WHITE);
	CString strName;
	strName.Format("%s.INI", Model_Data.strModelName);
	
	ListClear();
	char szFilter[]= "LOG Files(*.LOG)";
	CFileDialog fileDlg(TRUE, "INI", "C:\\LensAssy\\LogData\\LENS_LOG\\*.LOG", OFN_EXPLORER, szFilter) ; //EXPLORER, szFilter);
	if(fileDlg.DoModal ()== IDOK) {
		strName= fileDlg.GetPathName();
		FILE *stream;
		char buffer[255];
		int  i, ch;
		
		// Open file to read line from:
		if( (stream = fopen( strName, "r" )) != NULL )	{
			/ Read in first 80 characters and place them in "buffer":
			ch = fgetc( stream );
			while(feof( stream ) == 0) {
				for( i=0; i < 255 ; i++ )	{
					buffer[i] = (char)ch;
					if(ch== '\n' || ch== '\r') break;
					if(buffer[i]== '\t') buffer[i]= ':' ;
					//					else if(feof( stream ) == 0) break;
					ch = fgetc( stream );
				}
				// Add null to end string 
				buffer[i] = '\0';
				CString strText;
				strText.Format("%s", buffer);
				ListPrint(strText);
				ch = fgetc( stream );
			}
			fclose( stream );
		}
	}
	MenuOpenFlag= false;
}
*/
void CdlgXZ12_Mat::Open_PMC_ORG_File()
{
	CString strPath;
	strPath.Format(_T("%s\\X_Z12_Mat_Org.pmc"), ParamFile()->strDirConfigPath);
	
	FILE *stream;
	TCHAR buffer[255];
	TCHAR tmp[10];
	int  i, ch_no= 0, ch;
	bool Data_flag= false;
	int get_data_no= 0;
	int get_data_read_no= 0;
	int read_data= 0;
	
//	MAT_Org_Data_Z1[i]=	MAT_Org_Data_Z2[i]=	MAT_Down_Data_Z1[i]= MAT_Down_Data_Z2[i]= MAT_COR_Data_Z1[i]= MAT_COR_Data_Z2[i]= 0;
	// Open file to read line from:
	if( (stream = _wfopen( strPath, _T("r") )) != NULL )	
	{
		// Read in first 80 characters and place them in "buffer":
		ch = fgetc( stream );
		while(feof( stream ) == 0) 
		{
			for( i=0; i < 255 ; i++ )	
			{
				buffer[i] = (char)ch;
				if(ch== '\n' || ch== '\r') break;
				if(buffer[i]== '\t') buffer[i]= ':' ;
				//					else if(feof( stream ) == 0) break;
				ch = fgetc( stream );
			}
			// Add null to end string 
			buffer[i] = '\0';
			TRACE("%s\n", buffer);
			CString strText;
			if((buffer[0]>= '0' && buffer[0]<= '9') || buffer[0]== '-')
			{
				if(Data_flag)
				{
					if(ch_no== 5)		{
						MAT_Org_Data_Z1[get_data_read_no] = _wtoi(buffer);
						TRACE("[ DATA Z1 = %d [%d / %d] ]\n", MAT_Org_Data_Z1[i], get_data_no, get_data_read_no);
						get_data_read_no++;
						m_lListCount_Org_1= get_data_read_no;
					}
					else if(ch_no== 6) {
						MAT_Org_Data_Z2[get_data_read_no] = _wtoi(buffer);
						TRACE("[ DATA Z2 = %d [%d / %d] ]\n", MAT_Org_Data_Z2[i], get_data_no, get_data_read_no);
						get_data_read_no++;
						m_lListCount_Org_2= get_data_read_no;
					}
					if(get_data_read_no>= get_data_no) 
					{
						Data_flag= false;
						TRACE("[ DATA END [%d / %d]!!! ]\n", get_data_no, get_data_read_no);
					}
				}
			}
			else
			{
				if(buffer[0]== '#' && (buffer[1]== '5'||buffer[1]== '6') && buffer[2]== ' ' && buffer[3]== 'D' && buffer[4]== 'E' && buffer[5]== 'F' && buffer[6]== 'I' && buffer[7]== 'N' && buffer[8]== 'E')
				{
					ch_no= buffer[1] - '0';
					if(buffer[15]>= '0' && buffer[15]<= '9') 
					{
						tmp[0]= buffer[15];
						if(buffer[16]>= '0' && buffer[16]<= '9') 
						{
							tmp[1]= buffer[16];
							if(buffer[17]>= '0' && buffer[17]<= '9') 
							{
								tmp[2]= buffer[17];
								tmp[3]= 0;
							}
							else tmp[2]= 0;
						}
						else tmp[1]= 0;
						get_data_no= _wtoi(tmp);
						TRACE("[ DATA = %d ]\n", get_data_no);
						get_data_read_no= 0;
					}
					else 
					{
						get_data_no= 0;
						TRACE("[ DATA ZERO!!! ]\n");
						return;
					}
					Data_flag= true;
					TRACE("[ %s ]\n", buffer);
//					strText.Format("%s", buffer);
//					ListPrint(strText);
				}
				else
				{
					TRACE("[ %s ]\n", buffer);
				}
			}
			ch = fgetc( stream );
		}
		fclose( stream );
	}
}
//void CdlgXZ12_Mat::Open_PMC_CORRECT_File()
//{
	
//}
void CdlgXZ12_Mat::Build_PMC_DOWNLOAD_File()
{
	CString strPath;
	strPath.Format(_T("%s\\X_Z12_Mat_Down.pmc"), ParamFile()->strDirConfigPath);
	
	FILE *stream;
	int i= 0;

//	char buffer[255];
	if( (stream = _wfopen( strPath, _T("w") )) != NULL )	
	{
		for(i= 0; i< 32; i++)
			fwprintf(stream, _T("#%d DELETE COMP\n"), i+1);
		fwprintf(stream, _T("\n"));
		fwprintf(stream, _T("#6 DEFINE COMP %d, #1, #6, %d\n"), m_lListCount_Org_1, (m_lListCount_Org_1-1) * 5000);
		for(i= 0; i< m_lListCount_Org_1; i++)
			fwprintf(stream, _T("%.3f\n"), MAT_Down_Data_Z2[i]);

		fwprintf(stream, _T("\n"));
		fwprintf(stream, _T("#5 DEFINE COMP %d, #1, #5, %d\n"), m_lListCount_Org_1, (m_lListCount_Org_1-1) * 5000);
		for(i= 0; i< m_lListCount_Org_1; i++)
			fwprintf(stream,_T( "%.3f\n"), MAT_Down_Data_Z1[i]);
		fclose( stream );
	}
}
//===================================================================================
void CdlgXZ12_Mat::ResultUpdate_Org() 
//-----------------------------------------------------------------------------------	
{
	int i;
	CString str; 
//	m_ctrlOriginal_1.DeleteAllItems ();
	
	for (i = 0; i <  m_lListCount_Org_1; i++) 
	{ //㎧㎛㎲㎣
		// 일련 번호를 넣고 
		str.Format(_T("%d"), i+1); 							m_ctrlOriginal_1.InsertItem(i, str); 
		str.Format(_T("%d"), (i+1) * 1);					m_ctrlOriginal_1.SetItemText(i, 1, str); 
		str.Format(_T("%.3f"), MAT_Org_Data_Z1[i]);			m_ctrlOriginal_1.SetItemText(i, 2, str); 
		str.Format(_T("%.3f"), MAT_Org_Data_Z2[i]);			m_ctrlOriginal_1.SetItemText(i, 3, str); 
	}
}
//===================================================================================
void CdlgXZ12_Mat::ResultUpdate_Mat() 
//-----------------------------------------------------------------------------------	
{
	int i;
	CString str; 
	//	m_ctrlOriginal_1.DeleteAllItems ();
	
	for (i = 0; i <  m_lListCount_Org_1; i++) 
	{ //㎧㎛㎲㎣
		// 일련 번호를 넣고 
		str.Format(_T("%d"), i+1); 								m_ctrlCorrect_1.InsertItem(i, str); 
		str.Format(_T("%d"), (i+1) * 1);						m_ctrlCorrect_1.SetItemText(i, 1, str); 
		str.Format(_T("%.2f"), dMAT_COR_Data_Z1[i]);			m_ctrlCorrect_1.SetItemText(i, 2, str); 
		str.Format(_T("%.2f"), dMAT_COR_Data_Z2[i]);			m_ctrlCorrect_1.SetItemText(i, 3, str); 
	}
}
//===================================================================================
void CdlgXZ12_Mat::ResultUpdate_Down() 
//-----------------------------------------------------------------------------------	
{
	int i;
	CString str; 
	//	m_ctrlOriginal_1.DeleteAllItems ();
	
	for (i = 0; i <  m_lListCount_Org_1; i++) 
	{ //㎧㎛㎲㎣
		// 일련 번호를 넣고 
		str.Format(_T("%d"), i+1); 								m_ctrlDownload_1.InsertItem(i, str); 
		str.Format(_T("%d"), (i+1) * 1);						m_ctrlDownload_1.SetItemText(i, 1, str); 
		str.Format(_T("%.3f"), MAT_Down_Data_Z1[i]);			m_ctrlDownload_1.SetItemText(i, 2, str); 
		str.Format(_T("%.3f"), MAT_Down_Data_Z2[i]);			m_ctrlDownload_1.SetItemText(i, 3, str); 
	}
}









//===================================================================================
void CdlgXZ12_Mat::OpenMatDataFile() // System_data.strCurrentModelName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[50], value[50], ReadModel[50];
	int i= 0, j= 0;

	CString strText;
	TCHAR szParamFile[255];
	int load_data= 0;

	_stprintf(szParamFile, _T("%s\\X_Z12_Mat_Cal.ini"), ParamFile()->strDirConfigPath);

	//===================================================================================================================
	_stprintf(ReadModel, _T("Z1_DATA"));
	_stprintf(buf, _T("Data_Count_Z1"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);
	load_data= _wtoi(value);

	if(load_data != m_lListCount_Org_1)
	{
		strText.Format(_T("Z1 Data갯수가 다릅니다.[%d/%d] 데이터를 재생성 하십시요!"), load_data, m_lListCount_Org_1);
		Log()->SaveLogFile(_SYSTEM_LOG, strText);
		AfxMessageBox(strText);
		 m_lListCount_Org_1= load_data;
	}
	for(j= 0; j< m_lListCount_Org_1; j++) 
	{
		_stprintf(buf, _T("Data_Z1_%d"), j);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	dMAT_COR_Data_Z1[j] = _wtof(value);
	}
	//===================================================================================================================
	_stprintf(ReadModel, _T("Z2_DATA"));
	_stprintf(buf, _T("Data_Count_Z2"));	GetPrivateProfileString(ReadModel, buf, _T("0"), value, sizeof(value), szParamFile);
	load_data= _wtoi(value);
	
	if(load_data != m_lListCount_Org_2)
	{
		strText.Format(_T("Z2 Data갯수가 다릅니다.[%d/%d] 데이터를 재생성 하십시요!"), load_data, m_lListCount_Org_2);
		Log()->SaveLogFile(_SYSTEM_LOG, strText);
		AfxMessageBox(strText);
		m_lListCount_Org_2= load_data;
	}
	for(j= 0; j< m_lListCount_Org_1; j++) 
	{
		_stprintf(buf, _T("Data_Z2_%d"), j);	GetPrivateProfileString(ReadModel, buf, _T("0.0"), value, sizeof(value), szParamFile);	dMAT_COR_Data_Z2[j] = _wtof(value);
	}
}
//===================================================================================
void CdlgXZ12_Mat::SaveMatDataFile() // System_data.strCurrentModelName
//-----------------------------------------------------------------------------------
{
	TCHAR buf[50], value[50], ReadModel[50];
	CString strText;
	TCHAR szParamFile[255];
	int i= 0, j= 0;

	_stprintf(szParamFile, _T("%s\\X_Z12_Mat_Cal.ini"), ParamFile()->strDirConfigPath);
	
	//---------------------------------------
	_stprintf(ReadModel, _T("Z1_DATA"));
	_stprintf(value, _T("%d"), m_lListCount_Org_1);	_stprintf(buf, _T("Data_Count_Z1"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	for(j= 0; j< m_lListCount_Org_1; j++) {
		_stprintf(value, _T("%f"), dMAT_COR_Data_Z1[j]	);		_stprintf(buf, _T("Data_Z1_%d"), j);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}
	//---------------------------------------
	_stprintf(ReadModel, _T("Z2_DATA"));
	_stprintf(value, _T("%d"), m_lListCount_Org_2);	_stprintf(buf, _T("Data_Count_Z2"));	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	for(j= 0; j< m_lListCount_Org_1; j++) {
		_stprintf(value, _T("%f"), dMAT_COR_Data_Z2[j]	);		_stprintf(buf, _T("Data_Z2_%d"), j);	WritePrivateProfileString(ReadModel, buf, value, szParamFile);
	}
}

void CdlgXZ12_Mat::CalMatdata()
{
	int i= 0, j= 0;
	double Z_fact= -1000.0;//(1.0 / ParamFile()->Motion_data.dPulsePerMM[_U_AXIS_ROLL_Z]) * 1000.0;
	int icaldata= 0;
	double dcaldata= 0.0;
	MAT_Down_Data_Z1[0]= 0.0;
	MAT_Down_Data_Z2[0]= 0.0;
	for(j= 0; j< m_lListCount_Org_1; j++) 
	{
		icaldata= 0; dcaldata= 0.0;
		//MAT_Down_Data_Z1, dMAT_COR_Data_Z1, MAT_Org_Data_Z1
		dcaldata= dMAT_COR_Data_Z1[j] / Z_fact;
		//icaldata= (int)(dcaldata * 16.0);
		if(dcaldata > 0.03) dcaldata= 0.03;
		if(dcaldata < -0.03) dcaldata= -0.03;
		MAT_Down_Data_Z1[j+1]= dcaldata;//icaldata;// + MAT_Org_Data_Z1[j];
		//---------------------------------------------------------
		icaldata= 0; dcaldata= 0.0;
		dcaldata= dMAT_COR_Data_Z2[j] / Z_fact;
		//icaldata= (int)(dcaldata * 16.0);
		if(dcaldata > 0.03) dcaldata= 0.03;
		if(dcaldata < -0.03) dcaldata= -0.03;
		MAT_Down_Data_Z2[j+1]= dcaldata;//icaldata;// + MAT_Org_Data_Z2[j];
	}
	int StartNo1= 0, StartNo2= 0;
	int EndNo1= 0, EndNo2= 0;
	int z1Flag= 0, z2Flag= 0;
	for(j= 0; j< m_lListCount_Org_1; j++) 
	{
		if(z1Flag== 0 && dMAT_COR_Data_Z1[j]!= 0.0) { StartNo1= j; z1Flag= 1; }
		if(z1Flag== 1 && dMAT_COR_Data_Z1[j]== 0.0) { EndNo1= j; z1Flag= 2; }
		
		if(z2Flag== 0 && dMAT_COR_Data_Z2[j]!= 0.0) { StartNo2= j; z2Flag= 1; }
		if(z2Flag== 1 && dMAT_COR_Data_Z2[j]== 0.0) { EndNo2= j; z2Flag= 2; }
	}
	int offsetno= 5;
	double factZ1= dMAT_COR_Data_Z1[StartNo1] / (StartNo1-offsetno);
	double factZ2= dMAT_COR_Data_Z2[StartNo2] / (StartNo2-offsetno);
	
/*
	for(j= offsetno; j< StartNo1; j++) 
	{
		icaldata= 0; dcaldata= 0.0;		dcaldata= (factZ1 * (j-offsetno)) / Z_fact;		MAT_Down_Data_Z1[j+1]= dcaldata;
		icaldata= 0; dcaldata= 0.0;		dcaldata= (factZ2 * (j-offsetno)) / Z_fact;		MAT_Down_Data_Z2[j+1]= dcaldata;
	}
	
	factZ1= dMAT_COR_Data_Z1[EndNo1-1] / (10.0);
	factZ2= dMAT_COR_Data_Z2[EndNo1-1] / (10.0); 
	
	for(j= EndNo1; j< EndNo1+10; j++) 
	{
		icaldata= 0; dcaldata= 0.0;		dcaldata= (factZ1 * (9-(j-EndNo1))) / Z_fact;		MAT_Down_Data_Z1[j]= dcaldata;
		icaldata= 0; dcaldata= 0.0;		dcaldata= (factZ2 * (9-(j-EndNo1))) / Z_fact;		MAT_Down_Data_Z2[j]= dcaldata;
	}
*/
}

void CdlgXZ12_Mat::OnButtonLoadDataBuild() 
{
	UpdateData(TRUE);
	ParamFile()->System_data.dKgPerum= m_dKgPerum;
	ParamFile()->System_data.dExcept_XPos= m_dExcept_XPos;
	ParamFile()->System_data.dExcept_XPos2= m_dExcept_XPos2;
	ParamFile()->SaveSytemConfigFile();

	ListClear(0);
	ListClear(1);
	ListClear(2);

	ResultUpdate_Org();

	SaveMatDataFile();	
	OpenMatDataFile();	
	ResultUpdate_Mat();
	CalMatdata();
	Build_PMC_DOWNLOAD_File();

	ResultUpdate_Down();
}

void CdlgXZ12_Mat::OnButtonGraph_Load() 
{
	UpdateData(TRUE);
	if(m_iCalSelNo< 0) 
	{
		AfxMessageBox(_T("번호 선택?????"));
		return;
	}

	int sum_cnt1= 0;
	int sum_cnt2= 0;
	double sum_data1= 0.0;
	double sum_data2= 0.0;
/*
	if(ParamFile()->System_data.dZForceControlFlag)
	{
		AfxMessageBox("Z-Force Control Enable [하부] => Checked\n 위의 옵션이 선택된 상태에서는 보정 데이터 생성이 안됩니다.!!!");
		GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
		return;
	}
*/
	UpdateData(TRUE);
	ParamFile()->System_data.dKgPerum= m_dKgPerum;
	ParamFile()->System_data.dExcept_XPos= m_dExcept_XPos;
	ParamFile()->System_data.dExcept_XPos2= m_dExcept_XPos2;
	ParamFile()->SaveSytemConfigFile();

	double L1_set_data= 0.0;
	double L2_set_data= 0.0;
	double X_IncGap= 1.0;
	int data_cnt= 0;
	double X_Pos= 0.0;
	double L1_start= 0.0;
	double L2_start= 0.0;
	int mat_no= 0;
	int i= 0;
	CString strText;
	CString strText1;
	double X_start= 0.0;
	double X_end= 0.0;
	int cccccc= 0;
	
	double sumArrL1[1000];
	double sumArrL2[1000];
	int sumCnt= 0;
	int DataNo= 0;
	double sumValL1= 0.0;
	double sumValL2= 0.0;

	double Z2OffsetVal= 0.0;
	double Z2OffsetFact= 0.1;
	int Start_OffsetNo= 0;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if(m_iCalSelNo== _STAGE_1)
	{
/*
		int graphNo= pFrm->GetWindowHandler_MainView()->Max_X1_Cnt;
		if(graphNo<= 0) 
		{
			AfxMessageBox(_T("데이터 생성에 실패 했습니다.!!!"));
			return;
			//GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
		}
		X_start= PlotData_XP_1[Start_OffsetNo];// + m_dExcept_XPos;
		X_Pos= PlotData_XP_1[Start_OffsetNo];// + m_dExcept_XPos;
		
		
		L1_start= PlotData_L1_1[Start_OffsetNo];
		L2_start= PlotData_L2_1[Start_OffsetNo];
		strText.Format(_T("보정 DATA [ %.2f kg ] 기준"), L1_start);
		strText1.Format(_T("Download 데이터 [ %.2f kg ] 기준"), L1_start);
		
		SetDlgItemText(IDS_STATIC_179, strText.GetBuffer(0));
		SetDlgItemText(IDS_STATIC_180, strText1.GetBuffer(0));
		
		
		for(i= 0; i< 200; i++, data_cnt++)
		{
			X_Pos= data_cnt*X_IncGap;
			if(data_cnt<= 0)	{	DataNo= 0;			X_end= 0.0; }
			else				{	DataNo= data_cnt-1;	X_end= (data_cnt-1)*X_IncGap; }
			if(X_start <= X_Pos) 			break;
		}
		
		for(i= 0; i< 1000; i++)		{	sumArrL1[i]= 0.0;	sumArrL2[i]= 0.0;		}
		sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;
		
		for(i= 0; i< graphNo; i++)
		{
			if(/ *PlotData_C1_1[i+Start_OffsetNo] <= X_start &&* / PlotData_XP_1[i+Start_OffsetNo] >= X_end)
			{
				sumArrL1[sumCnt]= PlotData_L1_1[i+Start_OffsetNo];
				sumArrL2[sumCnt]= PlotData_L2_1[i+Start_OffsetNo];
				sumCnt++;
			}
			else
			{
				if(sumCnt> 0)
				{
					sumValL1= 0.0;		sumValL2= 0.0;
					for(int j= 0; j< sumCnt; j++)		
					{
						sumValL1= sumValL1 + sumArrL1[j];	sumValL2= sumValL2 + sumArrL2[j];		
					}
					sumValL1= sumValL1 / sumCnt;
					sumValL2= sumValL2 / sumCnt;
					L1_set_data= ((sumValL1-L1_start)*m_dKgPerum);
					L2_set_data= ((sumValL2-L2_start)*m_dKgPerum);
					dMAT_COR_Data_Z1[DataNo-1]= dMAT_COR_Data_Z1[DataNo-1] + L1_set_data;
					Z2OffsetVal= L1_set_data * Z2OffsetFact;
					dMAT_COR_Data_Z2[DataNo-1]= dMAT_COR_Data_Z2[DataNo-1] + (L2_set_data-Z2OffsetVal);
				}
				sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;
				for(int k= 0; k< 1000; k++)		{	sumArrL1[k]= 0.0;	sumArrL2[k]= 0.0;		}
				X_start= X_end; //X_start - 5.0;
				X_end= X_end - 5.0;
				DataNo--;
			}
		}
		ListClear(1);
		ResultUpdate_Mat();
*/
	}
	else if(m_iCalSelNo== _STAGE_2)
	{ 
/*
		int graphNo= pFrm->GetWindowHandler_MainView()->Max_X2_Cnt;
		if(graphNo<= 0) 
		{
			AfxMessageBox(_T("데이터 생성에 실패 했습니다.!!!"));
			return;
			//GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
		}
		X_start= PlotData_XP_2[Start_OffsetNo];// + m_dExcept_XPos;
		X_Pos= PlotData_XP_2[Start_OffsetNo];// + m_dExcept_XPos;
		
		
		L1_start= PlotData_L1_2[Start_OffsetNo];
		L2_start= PlotData_L2_2[Start_OffsetNo];
		strText.Format(_T("보정 DATA [ %.2f kg ] 기준"), L1_start);
		strText1.Format(_T("Download 데이터 [ %.2f kg ] 기준"), L1_start);
		
		SetDlgItemText(IDS_STATIC_179, strText.GetBuffer(0));
		SetDlgItemText(IDS_STATIC_180, strText1.GetBuffer(0));
		
		
		for(i= 0; i< 200; i++, data_cnt++)
		{
			X_Pos= data_cnt*X_IncGap;
			if(data_cnt<= 0)	{	DataNo= 0;			X_end= 0.0; }
			else				{	DataNo= data_cnt-1;	X_end= (data_cnt-1)*X_IncGap; }
			if(X_start <= X_Pos) 			break;
		}
		
		for(i= 0; i< 1000; i++)		{	sumArrL1[i]= 0.0;	sumArrL2[i]= 0.0;		}
		sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;
		
		for(i= 0; i< graphNo; i++)
		{
			if(/ *PlotData_C2_1[i+Start_OffsetNo] <= X_start &&* / PlotData_XP_2[i+Start_OffsetNo] >= X_end)
			{
				sumArrL1[sumCnt]= PlotData_L1_2[i+Start_OffsetNo];
				sumArrL2[sumCnt]= PlotData_L2_2[i+Start_OffsetNo];
				sumCnt++;
			}
			else
			{
				if(sumCnt> 0)
				{
					sumValL1= 0.0;		sumValL2= 0.0;
					for(int j= 0; j< sumCnt; j++)		
					{
						sumValL1= sumValL1 + sumArrL1[j];	sumValL2= sumValL2 + sumArrL2[j];		
					}
					sumValL1= sumValL1 / sumCnt;
					sumValL2= sumValL2 / sumCnt;
					L1_set_data= ((sumValL1-L1_start)*m_dKgPerum);
					L2_set_data= ((sumValL2-L2_start)*m_dKgPerum);
					dMAT_COR_Data_Z1[DataNo-1]= dMAT_COR_Data_Z1[DataNo-1] + L1_set_data;
					Z2OffsetVal= L1_set_data * Z2OffsetFact;
					dMAT_COR_Data_Z2[DataNo-1]= dMAT_COR_Data_Z2[DataNo-1] + (L2_set_data-Z2OffsetVal);
				}
				sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;
				for(int k= 0; k< 1000; k++)		{	sumArrL1[k]= 0.0;	sumArrL2[k]= 0.0;		}
				X_start= X_end; //X_start - 5.0;
				X_end= X_end - 5.0;
				DataNo--;
			}
		}
		ListClear(1);
		ResultUpdate_Mat();
*/
	}
	else if(m_iCalSelNo== _STAGE_3)
	{
#define fnc_MAX_DATA_NO 10000
		double X_POS_DATA[fnc_MAX_DATA_NO];
		double Z1_DATA[fnc_MAX_DATA_NO];
		double Z2_DATA[fnc_MAX_DATA_NO];

		int graphNo= pFrm->GetWindowHandler_MainView()->Max_X3_Cnt;
		if(graphNo<= 0) 
		{
			AfxMessageBox(_T("데이터 생성에 실패 했습니다.!!!"));
			return;
			//GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
		}

		for(i= 0; i< fnc_MAX_DATA_NO; i++)		
		{
			X_POS_DATA[i]= 0.0;	Z1_DATA[i]= 0.0;	Z2_DATA[i]= 0.0;	
		}
		int Real_DataCnt= 0;
		double X_RealStartPos= PlotData_XP_3[0] - 1.0;
		double X_RealEndPos= PlotData_XP_3[graphNo] + 1.0 ;

		if(fnc_MAX_DATA_NO <= graphNo) graphNo= fnc_MAX_DATA_NO-1;

		for(i= 0; i< graphNo; i++)		
		{

			if(PlotData_XP_3[i] < X_RealStartPos && PlotData_XP_3[i] > 0.0)
			{
				if(fabs(PlotData_XS_3[i]) > 0.01)
				{
					X_POS_DATA[Real_DataCnt]= (PlotData_XP_3[i]);
					Z1_DATA[Real_DataCnt]= (PlotData_L1_3[i]);
					Z2_DATA[Real_DataCnt]= (PlotData_L2_3[i]);
					Real_DataCnt++;
				}
			}
			if(PlotData_XP_3[i] < X_RealEndPos )	break;
		}

		X_start= PlotData_XP_3[Start_OffsetNo];// + m_dExcept_XPos;
		X_Pos= PlotData_XP_3[Start_OffsetNo];// + m_dExcept_XPos;
		
		
		L1_start= PlotData_L1_3[Start_OffsetNo];
		L2_start= PlotData_L2_3[Start_OffsetNo];
		strText.Format(_T("보정 DATA [ %.2f kg ] 기준"), L1_start);
		strText1.Format(_T("Download 데이터 [ %.2f kg ] 기준"), L1_start);
	
		int xxx= (int)X_RealStartPos;
		double dxx= (double)xxx;


		xxx= (int)(X_RealStartPos/1.0);
		dxx= (double)xxx * 1.0;

		dxx= (double)xxx * 1.0;
		dxx= (double)xxx * 1.0;
		dxx= (double)xxx * 1.0;
		dxx= (double)xxx * 1.0;
		dxx= (double)xxx * 1.0;

		SetDlgItemText(IDS_STATIC_179, strText.GetBuffer(0));
		SetDlgItemText(IDS_STATIC_180, strText1.GetBuffer(0));
		
/*
		for(i= 0; i< 600; i++, data_cnt++)
		{
			X_Pos= data_cnt*X_IncGap;
			if(data_cnt<= 0)	{	DataNo= 0;			X_end= 0.0; }
			else				{	DataNo= data_cnt-1;	X_end= (data_cnt-1)*X_IncGap; }
			if(X_RealStartPos <= X_Pos) 
			{
				break;
			}
		}
*/
		X_end= dxx;
		DataNo= xxx;

		for(i= 0; i< 1000; i++)		{	sumArrL1[i]= 0.0;	sumArrL2[i]= 0.0;		}
		sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;

		for(i= 0; i< Real_DataCnt; i++)
		{
			if(X_POS_DATA[i+Start_OffsetNo] <= X_end)
			{
				sumValL1= Z1_DATA[i+Start_OffsetNo];
				sumValL2= Z2_DATA[i+Start_OffsetNo];


				L1_set_data= ((sumValL1-L1_start)*m_dKgPerum);
				L2_set_data= ((sumValL2-L2_start)*m_dKgPerum);

					dMAT_COR_Data_Z1[DataNo-1]= dMAT_COR_Data_Z1[DataNo-1] + L1_set_data;
					Z2OffsetVal= L1_set_data * Z2OffsetFact;
					dMAT_COR_Data_Z2[DataNo-1]= dMAT_COR_Data_Z2[DataNo-1] + (L2_set_data-Z2OffsetVal);

				sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;
				for(int k= 0; k< 1000; k++)		{	sumArrL1[k]= 0.0;	sumArrL2[k]= 0.0;		}
				X_start= X_end; //X_start - 5.0;
				X_end= X_end - 1.0;
				DataNo--;
			}
		}
/*
		for(i= 0; i< Real_DataCnt; i++)
		{
			if(X_POS_DATA[i+Start_OffsetNo] >= X_end)/ *PlotData_C3_1[i+Start_OffsetNo] <= X_start &&* / 
			{
				sumArrL1[sumCnt]= Z1_DATA[i+Start_OffsetNo];
				sumArrL2[sumCnt]= Z2_DATA[i+Start_OffsetNo];
				sumCnt++;
			}
			else
			{
				if(sumCnt> 0)
				{
					sumValL1= 0.0;		sumValL2= 0.0;
					for(int j= 0; j< sumCnt; j++)		
					{
						sumValL1= sumValL1 + sumArrL1[j];	sumValL2= sumValL2 + sumArrL2[j];		
					}
					sumValL1= sumValL1 / sumCnt;
					sumValL2= sumValL2 / sumCnt;
					L1_set_data= ((sumValL1-L1_start)*m_dKgPerum);
					L2_set_data= ((sumValL2-L2_start)*m_dKgPerum);
					dMAT_COR_Data_Z1[DataNo-1]= dMAT_COR_Data_Z1[DataNo-1] + L1_set_data;
					Z2OffsetVal= L1_set_data * Z2OffsetFact;
					dMAT_COR_Data_Z2[DataNo-1]= dMAT_COR_Data_Z2[DataNo-1] + (L2_set_data-Z2OffsetVal);
				}
				sumCnt= 0;		sumValL1= 0.0;		sumValL2= 0.0;
				for(int k= 0; k< 1000; k++)		{	sumArrL1[k]= 0.0;	sumArrL2[k]= 0.0;		}
				X_start= X_end; //X_start - 5.0;
				X_end= X_end - 5.0;
				DataNo--;
			}
		}
*/
		int End_no= 0;

		if(DataNo > 0.0)
		{
			End_no= DataNo;
			dMAT_COR_Data_Z1[DataNo-1]= dMAT_COR_Data_Z1[End_no]-(dMAT_COR_Data_Z1[End_no]/3.0);
			dMAT_COR_Data_Z2[DataNo-1]= dMAT_COR_Data_Z2[End_no]-(dMAT_COR_Data_Z2[End_no]/3.0);
			dMAT_COR_Data_Z1[DataNo-2]= dMAT_COR_Data_Z1[DataNo-1]-(dMAT_COR_Data_Z1[End_no]/3.0);
			dMAT_COR_Data_Z2[DataNo-2]= dMAT_COR_Data_Z2[DataNo-1]-(dMAT_COR_Data_Z2[End_no]/3.0);
			dMAT_COR_Data_Z1[DataNo-3]= dMAT_COR_Data_Z1[DataNo-2]-(dMAT_COR_Data_Z1[End_no]/3.0);
			dMAT_COR_Data_Z2[DataNo-3]= dMAT_COR_Data_Z2[DataNo-2]-(dMAT_COR_Data_Z2[End_no]/3.0);
		}

		ListClear(1);
		ResultUpdate_Mat();
	}
	//GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
	
}

void CdlgXZ12_Mat::OnButtonMat_DownLoad() 
{
	CString strText;
	double ToPosition= 0.0;
	
/*
	strText.Format(_T("선택된 축들을 보정 위치로 이동 하시겠습니까? \n _AXIS_STAGE_X = [이동 위치: %.5fmm]"), ToPosition);
	if (MyMessageBox(strText.GetBuffer(0), _T("축 이동"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)		return;
	//	}
	
	double z_pos= pSystem.GetMotionComponent()->Get_Position(_ACS_AXIS_IMP_ROLL_Z1);
/ *
	if(z_pos> ParamFile()->Motion_data.RZ_DownPosInterLock)
	{
		MyMessageBox(_T("Z축 위치 확인\n모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
	}
* /
	if(!pSystem.GetMotionComponent()->isMotion_ReadyCheck(_ACS_AXIS_STAGE_X)) {
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR); return;
	}
	pSystem.GetMotionComponent()->ABS_Move(_ACS_AXIS_STAGE_X, ToPosition,	FALSE);			
	
	if(MoveEndCheck(_ACS_AXIS_STAGE_X)== FALSE)
		MyMessageBox(_T("모터 이동이 실패했습니다."), _T("Position-Move Failed"), M_ICONERROR);
	
	Sleep(1000);
*/

	pSystem.GetMotionComponent()->DownLoadMAT_Data(MAT_Down_Data_Z1, MAT_Down_Data_Z2);
//	AfxMessageBox(_T("X축과 Z축의 원점을 다시 실행하십시요!"));
}
//===================================================================================
BOOL CdlgXZ12_Mat::MoveEndCheck(int Axis) 
//-----------------------------------------------------------------------------------
{
	DWORD dwCheckTimeStart_Axis;
	CString strText;
	
	dwCheckTimeStart_Axis= GetTickCount();
	
	strText.Format(_T("선택된 축들을 설정 위치로 이동 중 입니다. [%d]"), Axis);
	
	MyProgressWnd ProgWnd(NULL, strText.GetBuffer(0));
	
	ProgWnd.SetRange(0, ParamFile()->System_data._AXIS_MOVE_TIME_OUT);
  
	ProgWnd.SetText( _T("%s\n  잠시만 기다려 주세요..."), strText);
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
/*	pFrm->m_ProgWait->TotalTime= ParamFile()->System_data._AXIS_MOVE_TIME_OUT;
	pFrm->m_ProgWait->strTitle.Format("%s\n 잠시만 기다려 주세요...", strText);
	pFrm->m_ProgWait->strMsg= strText;
	pFrm->ProgWaitView(true);
	////////////////////////////////////////////////////////////////////////////////
	*/
	
	
	DWORD st= GetTickCount();
	DWORD progst= GetTickCount();
	((CTransfer_ControlApp *)AfxGetApp())->DoEvents();
	
	while(1) 
	{
		ProgWnd.SetPos(GetTickCount() - st);
		if(pSystem.GetMotionComponent()->isDone(Axis)) 
		{
				return TRUE;
		}
		Sleep(9);
		
		if(/*!pFrm->m_ProgWait->ProgEnable || */GetTickCount() - dwCheckTimeStart_Axis > (DWORD)ParamFile()->System_data._AXIS_MOVE_TIME_OUT)  
		{
				pSystem.GetMotionComponent()->MoveStop(Axis); 
				return FALSE;
		}
		/// 사용자가 캔슬 버튼을 눌렀다.
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
		if ( ProgWnd.Cancelled() )
		{
			if     (Axis== _ACS_AXIS_IMP_ROLL_Z1 || Axis== _ACS_AXIS_IMP_ROLL_Z2)	pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_IMP_ROLL_Z1, _ACS_AXIS_IMP_ROLL_Z2);
			else if(Axis== _ACS_AXIS_ALIGN_Y1 || Axis== _ACS_AXIS_ALIGN_Y2)			pSystem.GetMotionComponent()->JogStop_Dual(_ACS_AXIS_ALIGN_Y1, _ACS_AXIS_ALIGN_Y2);
			else 			pSystem.GetMotionComponent()->MoveStop(Axis); 

			Log()->SaveLogFile(_MOTION_LOG, _T("모션 동작이 취소 되었습니다......"));
			return FALSE;
		}
	}
	return FALSE;
}

void CdlgXZ12_Mat::OnButtonOrg_DownLoad() 
{
/*
	pSystem.GetACS_MotionComponent()->DownLoadOrg_Data();
	AfxMessageBox("X축과 Z축의 원점을 다시 실행하십시요!");
*/
}

void CdlgXZ12_Mat::OnDblclkListM1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
//	char chText[50];
//	char chText1[50];
	
	int no= pNMListView->iItem;
	sel_listselno= no;
	
	if(sel_listselno>= 0)
	{
		m_selListData= dMAT_COR_Data_Z1[sel_listselno];
		m_selListData2= dMAT_COR_Data_Z2[sel_listselno];
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		UpdateData(FALSE);
/*		m_ListCtrl.GetItemText(pNMListView->iItem, 2, chText, 50);
		m_ListCtrl.GetItemText(pNMListView->iItem, 3, chText1, 50);
		if(m_btnImageReview.GetValue() && selX>= 0 && selY>= 0)
		{
			CString strDirName, strName;
			strDirName.Format("%s\\Auto\\Image_x%02d_y%02d_s%02d", ParamFile()->strDirReportSavePath, selX, selY, _wtoi(chText));
			if(no<= ListCnt)
			{
				strName.Format("%s\\Image_%03d.tif", strDirName, _wtoi(chText1)); //ParamFile()->strDirMeasImageSavePath);
				pSystem.GetVisionComponent()->InspImageLoad(_MEASURE_CAM, strName);
			}
		}
*/

	}
	*pResult = 0;
}

void CdlgXZ12_Mat::OnButton1() 
{
	if(sel_listselno>= 0)
	{
		UpdateData(TRUE);
		dMAT_COR_Data_Z1[sel_listselno]= m_selListData;
		dMAT_COR_Data_Z2[sel_listselno]= m_selListData2;
		ListClear(1);
//		ListClear(2);

		SaveMatDataFile();	
		OpenMatDataFile();	
		ResultUpdate_Mat();
//		CalMatdata();
//		ResultUpdate_Down();
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
}

void CdlgXZ12_Mat::OnButtondataInit() 
{
	ListClear(0);
	ListClear(1);
	ListClear(2);
	for(int i= 0; i< _MAX_MAT_DATA; i++)
	{
		MAT_Org_Data_Z1[i]=	MAT_Org_Data_Z2[i]=	MAT_Down_Data_Z1[i]= MAT_Down_Data_Z2[i]= 0;
		dMAT_COR_Data_Z1[i]= dMAT_COR_Data_Z2[i]= 0.0;
	}

	Open_PMC_ORG_File();
	ResultUpdate_Org();

	SaveMatDataFile();	
	OpenMatDataFile();	

	ResultUpdate_Mat();
	CalMatdata();
	
	Build_PMC_DOWNLOAD_File();
	ResultUpdate_Down();
}

void CdlgXZ12_Mat::OnButton3() 
{
//	m_i1stVal;m_i2ndVal; m_dCalSumVal

	UpdateData(TRUE);
	int i= 0, j= 0;
	double Old_vaa1= 0.0;
	double Old_vaa2= 0.0;
	bool b1st_flag1= false;
	bool b1st_flag2= false;
	int stepcnt= m_i2ndVal - m_i1stVal;
	double stepVal1= MAT_Down_Data_Z1[stepcnt+1] / stepcnt;
	double stepVal2= MAT_Down_Data_Z2[stepcnt+1] / stepcnt;

	for(j= 0; j< stepcnt; j++) 
	{
		MAT_Down_Data_Z1[j]= stepVal1 * j;
		MAT_Down_Data_Z2[j]= stepVal2 * j;
	}
	ListClear(2);
	ResultUpdate_Down();
}
