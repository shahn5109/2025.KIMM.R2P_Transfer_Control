#include "stdafx.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "Common_Tool.h"

//#include "GetNumDlg.h"
//#include "GetValueDlg.h"
//#include "GetNumDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSystemClass pSystem;

char ErrorDataFileName[100];
char MinPassWord[50];
//___________________________________________________________________
double rad2deg(double s)
{
	return (s*180.0/PI);
}
double deg2rad(double s)
{
	return (s*PI/180.0);
}

CString ReturnSelect(UINT ret)
{
	switch(ret) {
		case IDOK: return "IDOK";
		case IDYES: return "IDYES";
		case IDRETRY: return "IDRETRY";
		case IDCANCEL: return "IDCANCEL";
		case IDNO: return "IDNO";
		case IDIGNORE: return "IDIGNORE";
		default: return "NOT-DEFINE";
	}
	return "NOT-DEFINE";
}

UINT MyMessageBox(char* message, char* title, UINT type, char* YesBut/*="예"*/, char* NoBut/*="아니오"*/)
{
	//  CMyMessageBoxDlg dlg;
	CMsgDlg dlg;
	
	if( type & M_ICONINFORMATION ) dlg.m_icontype = M_ICONINFORMATION;
	else if( type & M_ICONQUESTION ) dlg.m_icontype = M_ICONQUESTION;
	else if( type & M_ICONSTOP ) dlg.m_icontype = M_ICONSTOP;
	else if( type & M_ICONERROR ) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;
	
	if( type & M_OK ) dlg.m_type = M_OK;
	else if( type & M_OKCANCEL ) dlg.m_type = M_OKCANCEL;
	else if( type & M_YESNO ) dlg.m_type = M_YESNO;
	else if( type & M_ABORTRETRYIGNORE ) dlg.m_type = M_ABORTRETRYIGNORE;
	else if( type & M_RETRYCANCEL ) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;
	
	strcpy(dlg.m_message, message);
	strcpy(dlg.m_title, title);
	
	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);

	UINT ret= dlg.DoModal();

	CString strText;
	strText.Format("[MessageBox] [Message=>%s] [Title=>%s] [Select=>%s]",
							message, title, ReturnSelect(ret));
	pSystem.GetParameterFileComponent()->SaveLogFile(_SYSTEM_LOG, strText);

	return (ret);
}

UINT MyMessageBox(UINT type, char* title,  char* YesBut, char* NoBut, char* message ...)
{
	return 0;
//	CMyMessageBoxDlg dlg;
	CMsgDlg dlg;
//	int iRtn = 0;

	if( type & M_ICONINFORMATION ) dlg.m_icontype = M_ICONINFORMATION;
	else if( type & M_ICONQUESTION ) dlg.m_icontype = M_ICONQUESTION;
	else if( type & M_ICONSTOP ) dlg.m_icontype = M_ICONSTOP;
	else if( type & M_ICONERROR ) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;

	if( type & M_OK ) dlg.m_type = M_OK;
	else if( type & M_OKCANCEL ) dlg.m_type = M_OKCANCEL;
	else if( type & M_YESNO ) dlg.m_type = M_YESNO;
	else if( type & M_ABORTRETRYIGNORE ) dlg.m_type = M_ABORTRETRYIGNORE;
	else if( type & M_RETRYCANCEL ) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;

	
	va_list args;
	char	cmessage[_MAX_PATH];

	memset(cmessage, NULL, sizeof(cmessage));

	va_start(args, message);
	_vstprintf(cmessage, message, args);
	va_end(args);

	strcpy(dlg.m_message, cmessage);
	strcpy(dlg.m_title, title);

	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);
	/*
	if (AfxGetMainWnd() != NULL)
	{
		HWND m_hWnd = AfxGetMainWnd()->m_hWnd;

		::SendMessage(m_hWnd, WM_MSGBOX_MSG, (WPARAM)&iRtn, (LPARAM)&dlg);

		while (TRUE)
		{
			if (iRtn != 0)
				break;
		}

		return iRtn;
	}
	else
	{
		return (dlg.DoModal());
	}
	*/

	return (dlg.DoModal());
}

UINT MyMessageBox(UINT i, char* title, UINT type, char* YesBut/*="예"*/, char* NoBut/*="아니오"*/)
{
	CString strTmp,strTmp2;
	char language_select[50];
	char cTmp[200], cTmp2[200], MessageBuf[200];
	
	strTmp.Format("%ld",i); //한글출력
	UINT uiTmp = GetPrivateProfileString("MESSAGE", strTmp, "error", cTmp, 200, ".\\Data\\DisMessage.dat");
//	strTmp.Format("%s", cTmp);

	strTmp2.Format("%ld",i+40000); //영문출력
	uiTmp = GetPrivateProfileString("MESSAGE", strTmp2, "error", cTmp2, 200, ".\\Data\\DisMessage.dat");
//	strTmp2.Format("%s", cTmp);

	uiTmp = GetPrivateProfileString("LANGUAGE", "100", "language", language_select, 200, ".\\Data\\DisMessage.dat");
	if( atoi(language_select) == 1) sprintf(MessageBuf,"%s",cTmp);
	else if( atoi(language_select) == 2) sprintf(MessageBuf,"%s",cTmp2);
	else if( atoi(language_select) == 3) sprintf(MessageBuf,"%s\n\n%s",cTmp2, cTmp);
	else sprintf(MessageBuf,"언어선택 오류!!!");

	return(MyMessageBox(MessageBuf,title,type,YesBut,NoBut));
}

UINT MyMessageBox(char* tmp, UINT i, char* title, UINT type, char* YesBut/*="예"*/, char* NoBut/*="아니오"*/)
{
	CString strTmp,aaa;
	char eMsg[200];
	char cTmp[200];

	strTmp.Format("%ld",i); //한글출력
	
//	UINT uiTmp = GetPrivateProfileString("MESSAGE", strTmp, "error", cTmp, 200, ".\\Data\\DisMessage.dat");
	
	aaa.Format("%s", tmp);
	strTmp.Format("%s", cTmp);
	sprintf(eMsg,strTmp,tmp);
	strTmp = aaa+strTmp;
	//AfxMessageBox(strTmp);
	return(MyMessageBox(eMsg,title,type,YesBut,NoBut));
}

BOOL GetNumberBox(char* szNum, int maxchar, char* getnum, char* title, BOOL pwd/*=FALSE*/)
{
 /*  //CGetNum dlg(maxchar, getnum, title, NULL, pwd);
   CGetNumDlg dlg(maxchar, getnum, title, NULL, pwd);
   if( dlg.DoModal() == IDOK )
   {
	  lstrcpy(szNum, dlg.m_strGetNum);
	  return TRUE;
   }
   else */
/* 터치 사용시 해제
	CGetNumDlg dlg(maxchar, getnum, title, NULL, pwd);
	if( dlg.DoModal() == IDOK )
	{
		lstrcpy(szNum, dlg.m_strGetNum);
		return TRUE;
	}
	else
		return FALSE;
*/
	return FALSE;
}



BOOL GetValueBox(char* szVal, int maxchar, char* getval, char* title, BOOL pwd/*=FALSE*/, BOOL btndisable/*= FALSE*/ )
{
 /*  CGetValueDlg dlg(maxchar, getval, title, NULL, pwd, btndisable);

   if( dlg.DoModal() == IDOK )
   {
      lstrcpy(szVal, dlg.m_strGetVal);
      return TRUE;
   }
   else */
   return FALSE;
}

DWORD HexToInt(char* s)
{
	char hexch[] = "0123456789ABCDEF";
	int i,j;
   DWORD r,n,k;
   char ch;

  	k=1; r=0;
  	for(i=strlen(s);  i>0; i--) 
	{
   	ch = s[i-1]; if (ch > 0x3f) ch &= 0xDF;
   	n = 0;
    	for(j = 0; j<16; j++)
		{
      	if (ch == hexch[j])
         	n = j;
		}
    	r += (n*k);
     	k *= 16;
	}
  	return r;
}

void SetStatusText(LPCTSTR sText)
{
	CStatusBar* pStatusBar = NULL;

   CWnd *pMainWnd = AfxGetMainWnd();

   if(!pMainWnd) return;

   pStatusBar = (CStatusBar*)pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
    
   // - OR -
   // if (!pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
   // pStatusBar = (CStatusBar*) ((CFrameWnd*) pFrame)->GetMessageBar();

   if(!pStatusBar || !pStatusBar->IsKindOf(RUNTIME_CLASS(CStatusBar))) return;

   pStatusBar->SetWindowText(sText);
}

BOOL DisplayErrorDialog = FALSE;

BOOL ErrorDisplay(UINT err)
{
	CString strErrNo,strTmp;
	char cTmp[500], cTmp2[200], MessageBuf[200];
	
	FILE* fp;
	
	CTime t = CTime::GetCurrentTime();
	CString strTime = t.Format( "%m/%d-%H:%M:%S" );

	if(DisplayErrorDialog)
	{
		MyMessageBox(17203, "Error Display Error", M_ICONERROR);
					//17203=이미 Error Dialog Box가 실행중입니다.
		return FALSE;
	}

	DisplayErrorDialog = TRUE;

	if( (err < 1) && (err > 49999) )
	{
		MyMessageBox(17204,"Error",M_ICONERROR);
					//17204=Error Number는 10000 ~ 49999사이에 있어야 합니다.
		DisplayErrorDialog = FALSE;
		return FALSE;
	}
	strErrNo.Format("%d",err);

	//////////////////////////////////////////////
	if(( fp= fopen( ErrorDataFileName, "rt")) == NULL)
	{
		MyMessageBox(17205,"Error",M_ICONERROR);
					//17205=Error File을 Open할 수가 없습니다.
		DisplayErrorDialog = FALSE;
		return FALSE;
	}
	else fclose(fp);

	strTmp.Format("%ld", err); //한글출력
	char language_select[50];
	UINT uiTmp = GetPrivateProfileString("PCBERROR", strTmp, "Cannot find the Error Number in the Error File.", cTmp, 200, ".\\Data\\Error.dat");
	//strTmp.Format("%s", cTmp);
	strTmp.Format("%ld",err+40000); //영문출력
	uiTmp = GetPrivateProfileString("PCBERROR", strTmp, "Cannot find the Error Number in the Error File.", cTmp2, 200, ".\\Data\\Error.dat");

	//ckh1024
	uiTmp = GetPrivateProfileString("LANGUAGE", "100", "language", language_select, 200, ".\\Data\\Error.dat");
	if( atoi(language_select) == 1) sprintf(MessageBuf,"%s",cTmp);
	else if( atoi(language_select) == 2) sprintf(MessageBuf,"%s",cTmp2);
	else if( atoi(language_select) == 3) sprintf(MessageBuf,"%s\n%s",cTmp2, cTmp);
	else sprintf(MessageBuf,"language Select Error!!!");
	//ckh1024

//	if(Pcb_OpManager.GetOpMode() == AUTORUN_MODE)
//	{
//		P_M.Error(err);
//		Sleep(300);
//	}

	MyMessageBox(MessageBuf, "Error", M_ICONERROR);
	Sleep(200);
	
	//ckh1024
//	char * ErrSav = strtok(MessageBuf,"\n");
	
	/////////////////////////
//	if(Pcb_OpManager.GetOpMode() == AUTORUN_MODE)
//	{
//		P_M.ErrorReset();
//	}

	DisplayErrorDialog = FALSE;

	sprintf(cTmp,"%s,%s,%s",strErrNo,MessageBuf,strTime);
	//CString ErrorSave;
	//ErrorSave.Format("%s",cTmp);
	//ErrorSave = ErrorSave+","+strTime;

//	if(Pcb_OpManager.GetOpMode() == AUTORUN_MODE)
//	{
//		if(!ErrorListSave(cTmp))
//			return FALSE;
//	}
	return TRUE;
}                                                                                                                                                                                                                                                                                                                      

BOOL ErrorDisplay(UINT err, BOOL AutoMode)
{
//	UINT key = 0;
	FILE* fp;
	char tmp[20];
//	BOOL buzzerflag = FALSE;
	
	CTime t = CTime::GetCurrentTime();
	CString s = t.Format( ", %y/%m/%d-%H:%M" );

	if( DisplayErrorDialog )
	{
		MyMessageBox(17203, "Error Display Error", M_ICONERROR);
					//17203=이미 Error Dialog Box가 실행중입니다.
		return FALSE;
	}

	if( (err < 1) && (err > 9999999) )
	{
		MyMessageBox(17204,"Error",M_ICONERROR);
					//17204=Error Number는 10000 ~ 49999사이에 있어야 합니다.
		return FALSE;
	}

	if((fp = fopen( ErrorDataFileName, "rt")) == NULL)
	{
		MyMessageBox(17205,"Error",M_ICONERROR);
					//17205=Error File을 Open할 수가 없습니다.
		return FALSE;
	}

	_itoa(err,tmp,10);

	char Error[100];
	s = tmp + s;
	strcpy(Error, s);

	strcat(tmp,":");
	char buf[500];

	if( !get_fpoint(fp,tmp,buf))
	{
		_itoa(99999,tmp,10);
		strcat(tmp,":");
		if( !get_fpoint(fp,tmp,buf))
		{
			fclose(fp);
			sprintf(buf, "Cannot find the Error Number in the Error File. : (%d) I am sorry, but can you call Mechatronics Center with Memo of this Number and Step!!!", err);
			sprintf(buf,"%d",err);
			MyMessageBox(buf, 17207, "Error",M_ICONERROR|M_OK);
						//17207=Error File에서 Error 번호를 찾을수가 없습니다. : (%d) 죄송하지만 이번호와 Step을 Memo해서 생기센타로 연락해 주세요 !!!
			return FALSE;
		}
	}
	fclose(fp);
	 
	DisplayErrorDialog = TRUE;

	_itoa(err,tmp,10);

	CString emsg = buf;
	CString stmp = tmp;
	CString orgmsg;

	stmp += ":";

	emsg.TrimLeft(stmp);
	emsg.TrimLeft();

	stmp.Empty();
	orgmsg.Empty();

	int rel,i;
	while(1)
	{
		rel = emsg.Find("^");
		if( rel == -1 )
		{
			orgmsg += emsg;
			break;
		}

		stmp = emsg.Left(rel);
		stmp += "\r\n";
		orgmsg += stmp;
		stmp.Empty();

		for(i=0;i<rel+1;i++)
		{
			emsg.SetAt(i,' ');
		}
		emsg.TrimLeft();
	}

	char ebuf[100];
	memset(&ebuf, 0, sizeof(ebuf));

//	if( err == 18071 ) // temp winpcb
//	{
//		orgmsg += "\r\n";
//		sprintf_s(ebuf, "( 기준값 : %d, 측정값 : %d )", PCBRightBlob, PCBRightBlobArea);
//		orgmsg += ebuf;
//	}
	
//	if( err == 18072 )
//	{
//		orgmsg += "\r\n";
//		sprintf_s(ebuf, "( 기준값 : %d, 측정값 : %d )", PCBLeftBlob, PCBLeftBlobArea);
//		orgmsg += ebuf;
//	}

	if(AutoMode == TRUE)
	{
//		ErrorListSave(Error);
	}

//	CErrorDlg* errdlg = new CErrorDlg;

//	errdlg->SetTextMsg(tmp, orgmsg);
//	errdlg->CenterWindow();
//	errdlg->ShowWindow(SW_SHOW);
//	errdlg->PeekAndPump();

/*	ResetLamp(IO_ON); // temp Winpcb

#ifdef _LINE_CONTROLLER
	switch( err )
	{
		case 16050 :
		case 16051 :
			break;

		default : 
			P_M.Error(err);
			break;
	}
#endif

	buzzerflag = FALSE;
	while(1)
	{
		errdlg->PeekAndPump();
		if(errdlg->m_Cancel) 
		{	
			delete errdlg;
			break;
		}

		if( !buzzerflag )
		{
			if( errdlg->m_BuzzerOffFlag )
			{
				BuzzerOnOff(IO_OFF);
				buzzerflag = TRUE;
			}
		}

		key = GetOldIOKey();
		if( (key == IO_RESET_KEY) ||
			 (key == IO_STOP_KEY) ||
			 (key == IO_ESTOP_KEY) )
		{
			delete errdlg;
			break;
		}
	}

	while(1)
	{
		if( !GetOldIOKey() ) break;
	}

	ResetLamp(IO_OFF);
	TowerLamp(T_YELLOW);
	BuzzerOnOff(IO_OFF);
*/
	DisplayErrorDialog = FALSE;

//#ifdef _LINE_CONTROLLER
//	P_M.SetFlag(PM_ERROR_CLEAR);
//#endif

	return TRUE;
}

//////////////////////////////////////////////////////////////
// Error List File 저장
BOOL ErrorListSave(char* pn)
{
	FILE* fsource, *ftarget;
	CFileFind fn;
//	char fname[100] = ".\\data\\ErrorList.dat";
	char fname1[100] = ".\\data\\ErrorList.bak";


	/*
	if(fn.FindFile(fname,0))
	{
		if(!CopyFile(fname, fname1, FALSE))
		{
			MyMessageBox(17208, "Data\\ErrorList.bak File Write Error!", M_ICONERROR);
						//17208=Error List Backup File을 만들수가 없습니다. Data\\ErrorList.bak을 점검하시오.
			return FALSE;
		}
		if(!DeleteFile(fname))
		{
			MyMessageBox(17209, "Data\\ErrorList.dat을 File Delete Error!", M_ICONERROR);
						//17209=Error List  File을 지울수가 없습니다. Data\\ErrorList.dat을 점검하시오.
			return FALSE;
		}
	}
	*/
	if(fsource= fopen(".\\data\\ErrorList.dat", "w+"))
	{
		MyMessageBox(17210, "Data\\ErrorList.Dat File Write Error!", M_ICONERROR);
					//17210=Error List File을 만들수가 없습니다. Data\\ErrorList.dat를 확인하시오.
		return FALSE;
	}
	fprintf(fsource, "%s\n", pn);

//	errno_t ferr;
	if(!fn.FindFile(fname1,0))
	{
		fclose(fsource);
		return TRUE;
	}
	else if( ftarget= fopen(".\\data\\ErrorList.bak", "r") )
	{
		MyMessageBox(17211, "Data\\ErrorList.bak File Read Error!", M_ICONERROR);
					//17211=Error List File을 읽을수가 없습니다. Data\\ErrorList.bak를 확인하시오.
		return FALSE;
	}

	char buf[200];
	int count=0;

	fgets( buf,200,ftarget );
	while(!feof(ftarget))
	{
		if(count > 200) break; //cyj 2003.11.26 100->200
		fputs(buf,fsource);
		fgets( buf,100,ftarget );
		count += 1;
	}

	fclose(fsource);
	fclose(ftarget);

	return TRUE;
}



int get_fpoint(FILE* fp, char* s, char* sbuf)
{
	char buf[500];
	int len,rel;
	CString strBuf;

	while(1)
	{
		if( feof( fp ) ) return FALSE;
		fgets( buf,500,fp );
		strBuf = buf;
		rel = strBuf.Find(s);
		if( strBuf.Find(s) >= 0 ) break;
	}

	len = strlen(buf);
	strncpy(sbuf,buf,len);
	sbuf[len-1] = '\0';

	return TRUE;
}

int MyAbs( int val )
{
	int tmpval;

	tmpval = val;

	if( tmpval < 0 ) tmpval *= -1;

	return tmpval;
}

double MyAbs( double val )
{
	double tmpval;

	tmpval = val;

	if( tmpval < 0.0 ) tmpval *= -1.0;

	return tmpval;
}

BOOL FileDeleteAll(char * pn)
{
	char buf[100];
	CFileFind fn;
	int nextfile, i;
	int filecnt=0;

	sprintf(buf,"%s\\*.*", pn);
	
	filecnt = FileCountInDir(buf);

	if( !filecnt ) return TRUE;

	CString* filename = new CString[filecnt+1];

	filecnt = 0;
	if( !fn.FindFile(buf,0) ) return FALSE;
	nextfile = fn.FindNextFile();
	while(1)
	{
		if( !fn.IsDirectory() )
		{
			filename[filecnt] = fn.GetFileName();
			filecnt++;
		}

		if( !nextfile) break;
		nextfile = fn.FindNextFile();
	}


	CMyProgressWnd ProgWnd(NULL, "Call the Model.....");
	ProgWnd.SetRange(0,filecnt);
	ProgWnd.SetText("...is delete a File...\n"
						  "Wait a moment...");

	for(i=0;i<filecnt;i++)
	{
		sprintf(buf,"%s\\%s", pn, (LPCTSTR)filename[i]);
		CFile::Remove(buf);

		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
	}

	delete[] filename;
	return TRUE;
}

BOOL CheckDirExist(char * pn)
{
	CFileFind fn;
	char buf[100];
	int nextfile;

	sprintf(buf,"%s\\*.*", pn);

	if( !fn.FindFile(buf,0) )
	{
		MyMessageBox(17212, "Error", M_ICONERROR);
					//17212=File Searching Error
		return FALSE;
	}

	nextfile = fn.FindNextFile();

	while(1)
	{
		if( fn.IsDirectory() ) 
			if( !fn.IsDots() ) return FALSE;
		if(!nextfile) break;
		nextfile = fn.FindNextFile();
	}
	return TRUE;
}

BOOL CheckFileExist(char * pn)
{
	CFileFind fn;

	if( !fn.FindFile(pn,0) ) return FALSE;
	else return TRUE;
}

int FileCountInDir(char* pn)
{
	CFileFind ff;
	int cnt=0, nt;

	if( !ff.FindFile(pn,0) ) return FALSE;

	nt = ff.FindNextFile();
	while(1)
	{
		if( !ff.IsDirectory() ) cnt++;
		if( !nt ) break;
		nt = ff.FindNextFile();
	}

	return cnt;
}

BOOL CopyFileDir(char* dest, char* src, int deletemode/*=FALSE*/)
{
	int cnt=0, nt, i;
	char buf[100];

	CFileFind ff;

	sprintf(buf,"%s\\*.*",src);
	cnt = FileCountInDir(buf);

	if( !cnt )
	{
		MyMessageBox(17213,"Confirm",M_ICONSTOP);
					//17213=Directory에 File이 없습니다.
		return FALSE;
	}

	if( deletemode ) FileDeleteAll(dest);

	CString* filename = new CString[cnt+1];

	cnt=0;
	if( !ff.FindFile(buf,0) ) return FALSE;
	nt = ff.FindNextFile();
	while(1)
	{
		if( !ff.IsDirectory() )
		{
			filename[cnt] = ff.GetFileName();
			cnt++;
		}
		if( !nt ) break;
		nt = ff.FindNextFile();
	}

	CMyProgressWnd ProgWnd(NULL, "Model Copy.....");
	ProgWnd.SetRange(0,cnt);
	ProgWnd.SetText("...is copying a File...\n"
					 "Wait a moment...");

	char destfile[100];
	char srcfile[100];

	for(i=0; i<cnt; i++)
	{
		sprintf(srcfile,"%s\\%s", src, filename[i]);
		sprintf(destfile, "%s\\%s", dest, filename[i]);
		if( !CopyFile(srcfile, destfile, FALSE) )
		{
			delete[] filename;
			MyMessageBox(17214,"Error",M_ICONERROR);
						//17214=File Copy중 Error가 발생하였습니다.
			return FALSE;
		}
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();
	}

	delete[] filename;
	return TRUE;
}

int DirCountInDir(char* pn)
{
	CFileFind ff;
	int cnt=0, nt;

	if( !ff.FindFile(pn,0) ) return FALSE;

	nt = ff.FindNextFile();
	while(1)
	{
		if( ff.IsDirectory() ) 
		{
			if( !ff.IsDots() ) cnt++;
		}
		if( !nt ) break;
		nt = ff.FindNextFile();
	}

	return cnt;
}

void DisplayMessage(char* msg)
{
   CMyMsgWnd* mwnd = new CMyMsgWnd();
   mwnd->SetMessage(msg);
   while(1)
   {
//      mwnd->SetFocus();
      mwnd->PeekAndPump();
      if(mwnd->m_CancelFlag) break;
// 		if( GetOldIOKey() == IO_RESET_KEY ) break; //temp winpcb
   }

//	mwnd->DestroyWindow();
	delete mwnd;
}


/*
BOOL DispBAUPanelInfo(int work)
{
	if(work == 1) gMainView->m_sBAUStep.SetColor(BLACK, YELLOW);
	else if(work ==2) gMainView->m_sBAUStep.SetColor(WHITE, RED);
	else  gMainView->m_sBAUStep.SetColor(BLACK, LTGRAY);

	return TRUE;
}

BOOL DispIPUPanelInfo(int work)
{
	if(work == 1) gMainView->m_sIPUStep.SetColor(BLACK, YELLOW);
	else if(work ==2) gMainView->m_sIPUStep.SetColor(WHITE, RED);
	else  gMainView->m_sIPUStep.SetColor(BLACK, LTGRAY);

	return TRUE;
}

BOOL DispSTUPanelInfo(int work)
{
	if(work == 1) gMainView->m_sStageStep.SetColor(BLACK, YELLOW);
	else if(work ==2) gMainView->m_sStageStep.SetColor(WHITE, RED);
	else  gMainView->m_sStageStep.SetColor(BLACK, LTGRAY);

	return TRUE;
}

BOOL DispOPUPanelInfo(int work)
{
	if(work == 1) gMainView->m_sOPUStep.SetColor(BLACK, YELLOW);
	else if(work ==2) gMainView->m_sOPUStep.SetColor(WHITE, RED);
	else  gMainView->m_sOPUStep.SetColor(BLACK, LTGRAY);

	return TRUE;
}

BOOL DispPIUPanelInfo(int work)
{
	if(work == 1) gMainView->m_sPIUStep.SetColor(BLACK, YELLOW);
	else if(work ==2) gMainView->m_sPIUStep.SetColor(WHITE, RED);
	else  gMainView->m_sPIUStep.SetColor(BLACK, LTGRAY);

	return TRUE;
}

BOOL DispNTUPanelInfo(int work)
{
	if(work == 1) gMainView->m_sNTUStep.SetColor(BLACK, YELLOW);
	else if(work ==2) gMainView->m_sNTUStep.SetColor(WHITE, RED);
	else  gMainView->m_sNTUStep.SetColor(BLACK, LTGRAY);

	return TRUE;
}
*/
// temp winpcb
BOOL DispMainSheetCount_1()
{

//	sprintf_s(temp,"%3d/%3d",SheetFeedingIncrement_1,SheetMaxCount_1);

//	gMainView->m_sSheetCounting1.SetText(temp, WHITE, BLACK);

	return TRUE;
}

BOOL DispMainSheetCount_2()
{

//	sprintf_s(temp,"%3d/%3d",SheetFeedingIncrement_2,SheetMaxCount_2);

//	gMainView->m_sSheetCounting2.SetText(temp, WHITE, BLACK);

	return TRUE;
}

BOOL DispMainSheetCount_3()
{

//	sprintf_s(temp,"%3d/%3d",SheetFeedingIncrement_3,SheetMaxCount_3);

//	gMainView->m_sSheetCounting3.SetText(temp, WHITE, BLACK);

	return TRUE;
}

BOOL DispPressingTime(double dt)
{
//	sprintf_s(temp,"%.1f/%.1f",dt,PressSetTime);
//	gMainView->m_sPressingTime.SetText(temp, WHITE, BLACK);

	return TRUE;
}

BOOL DispCoolingTime(double dt)
{
//	sprintf_s(temp,"%.1f/%.1f",dt,CoolingSetTime);
//	gMainView->m_sCoolingTime.SetText(temp, WHITE, BLACK);

	return TRUE;
}

BOOL DispAcfPressingTime(double dt)
{
//	sprintf_s(temp,"%.1f/%.1f",dt,AcfPressSetTime);
//	gMainView->m_sAcfPressingTime.SetText(temp, WHITE, BLACK);

	return TRUE;
}

BOOL DispAcfCoolingTime(double dt)
{
//	sprintf_s(temp,"%.1f/%.1f",dt,AcfCoolingSetTime);
//	gMainView->m_sAcfCoolingTime.SetText(temp, WHITE, BLACK);

	return TRUE;
}

void DispAcfPressCnt()
{
//	if( itmpAcfSheetUseCount == tmpAcfSheetUseCount ) return;

//	itmpAcfSheetUseCount = tmpAcfSheetUseCount;
//	char temp[30];
//	sprintf_s(temp,"%3d/%3d",tmpAcfSheetUseCount,AcfSheetUseCount);
//	gMainView->m_sAcfPressCnt.SetText(temp, WHITE, BLACK);
}

void DispMainPressCnt()
{
//	if( itmpSheetUseCount == tmpSheetUseCount ) return;
//	itmpSheetUseCount = tmpSheetUseCount;
	
//	char temp[30];
//	sprintf_s(temp,"%3d/%3d",tmpSheetUseCount,SheetUseCount);
//	gMainView->m_sMainPressCnt.SetText(temp, WHITE, BLACK);
}

BOOL DisplayPLULength(double dl, double dt)
{
//	str.Format("%.0lf, %.3lf", dl, dt);
//	gMainView->m_sPLULength.SetText(str, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayPCBLength(double dl, double dt)
{

//	str.Format("%.0lf, %.3lf", dl, dt);
//	gMainView->m_sPCBLength2.SetText(str, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayLCDLength(double dl, double dt)
{

//	str.Format("%.0lf, %.3lf", dl, dt);
//	gMainView->m_sLCDLength2.SetText(str, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayBAULength(double dl, double dt)
{

//	str.Format("%.0lf, %.3lf", dl, dt);
//	gMainView->m_sBAURel.SetText(str, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayPIULength(double dl, double dt)
{

//	str.Format("%.0lf, %.3lf", dl, dt);
//	gMainView->m_sPIUAlignRel.SetText(str, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayPIULengthLeft(double dl, double dt)
{

//	str.Format("%.3lf, %.3lf", dl, dt);
//	gMainView->m_sPIURel.SetText(str, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayPIULengthRight(double dl, double dt)
{

//	str.Format("%.3lf, %.3lf", dl, dt);
//	gMainView->m_sPIURelRight.SetText(str, WHITE, BLACK);

	return TRUE;
}
/*
BOOL DispTactTime(double dt)
{
	gMainView->m_sTactTime.SetText(dt, 1);

	return TRUE;
}

BOOL DisplayLCDIs(char* lcdid)
{
	gMainView->m_sLCDId.SetText(lcdid);

	return TRUE;
}
*/
BOOL DisplayPCBIs(CString pcbid)
{
//	gMainView->m_sPCBId.SetText(pcbid, WHITE, BLACK);

	return TRUE;
}

BOOL DisplaySTUPCBLeftBlob(int dt)
{
//	gMainView->m_sSTUPcbLeftBlob.SetText(dt, WHITE, BLACK);

	return TRUE;
}

BOOL DisplaySTUPCBRightBlob(int dt)
{
//	gMainView->m_sSTUPcbRightBlob.SetText(dt, WHITE, BLACK);

	return TRUE;
}

BOOL DisplayACFCheckBlob(int dt)
{
//	gMainView->m_sAcfCheck.SetText(dt, WHITE, BLACK);

	return TRUE;
}


//BOOL DispTLUStatus(char* msg, BOOL flash/*=FALSE*/)
/*{
	if( flash )
	{
		gMainView->m_sTLUStatus.SetText(msg, WHITE, BLACK);
	}
	else
	{
		gMainView->m_sTLUStatus.SetText(msg, WHITE, BLACK);
	}

	return TRUE;
}
*/
BOOL DispTULUStatus(char* msg, BOOL flash/*=FALSE*/)
{
	return TRUE;
}


void StartBarCodeReading()
{
}

void EndBarCodeReading()
{
}

void StartPCBBarCodeReading()
{
}

void EndPCBBarCodeReading()
{
}

void TempCheckSum(char* str)
{
	int sum = 0;
	int tmplen = strlen(str);

	for(int i=0;i<tmplen;i++) 
		sum^=str[i];	//EX-OR

	sprintf(str+tmplen, "%02X%c%c",sum&0xFF, 0x2A, 0x0D);
}


BOOL SetTempControl(int UtNo, double Temp)
{
	char buf[50];

	memset(buf, 0, sizeof(buf));

	char ctmp[4];
	int itmp;

	ctmp[3] = char(Temp/1000);
	itmp = int(Temp)%1000;
	ctmp[2] = char(itmp/100);
	itmp = int(Temp)%100;
	ctmp[1] = char(itmp/10);
	itmp = int(Temp)%10;
	ctmp[0] = char(itmp);

	sprintf(buf, "@%02dWS01%X%X%X%1d", UtNo, ctmp[3], ctmp[2], ctmp[1], ctmp[0]);	
	TempCheckSum(buf);

	// Write
//	gMainView->ClearCom2Buffer();
//	gMainView->m_com2.SetOutput(COleVariant(buf));


//	CTimerCheck E5CNTimer;
//	E5CNTimer.StartTimer();

//	gMainView->ReadComm2Port();

//	while(1)
//	{
//		if(ReceiveCom2Flag ) break;
//		else
//		{
//			if(E5CNTimer.MoreThan(0.5))
//			{
//				sprintf_s(errmsg, "%d번 온도를 Write하는데 Error가 발생하였습니다.", tempno);
//				MyMessageBox(tempno, 17215, "온도 Write Error", M_ICONERROR);
							//17215=%d번 온도를 Write하는데 Error가 발생하였습니다.
//				return FALSE;			
//			}
//		}

//		gMainView->ReadComm2Port();
//	}

//	strcpy_s((buf, gMainView->m_sCom2Buff);

	if(buf[5] == '0' && buf[6] == '0')
	{
		// 확인 및 재 시도
//		if(GetSettingTemp(tempno) != tempval)		
			return TRUE;
	}

	return FALSE;
}

double ReadTempControl(int UtNo)
{
	char buf[100];
	double dTmp;

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "@%02dRS01", UtNo);
	TempCheckSum(buf);

	// Write
//	gMainView->ClearCom2Buffer();
//	gMainView->m_com2.SetOutput(COleVariant(buf));

	Sleep(50);

//	CTimerCheck E5CNTimer;
//	E5CNTimer.StartTimer();

//	gMainView->ReadComm2Port();

//	while(1)
//	{
//		if(ReceiveCom2Flag ) break;
//		else
//		{
//			if(E5CNTimer.MoreThan(1.0))
//			{
//				sprintf_s(errmsg, "%d번 온도를 Write하는데 Error가 발생하였습니다.", tempno);
//				MyMessageBox(tempno, 17215, "온도 Write Error", M_ICONERROR);
							//17215=%d번 온도를 Write하는데 Error가 발생하였습니다.
//				return 9999.0;			
//			}
//		}

//		gMainView->ReadComm2Port();
//	}

//	strcpy_s((buf, gMainView->m_sCom2Buff);

	if(buf[5] == 0x30 && buf[6] == 0x30)
	{
		dTmp = ((double(buf[7])-48.0)*1000) + ((double(buf[8])-48.0)*100) 
			 + ((double(buf[9])-48.0)*10) + (double(buf[10])-48.0);

		return dTmp;
	}
	else
		return 9999.0;
}
/*
void DisplayStatusMtrError(int mtrno, int mtrerrno)
{
	CString mtrstr;

	mtrstr.Format("%d번째축 : %d, 0x%x", mtrno, mtrerrno, mtrerrno);
	mtrstr.MakeUpper();

	CMainFrame* mwnd = (CMainFrame*)AfxGetMainWnd();
	mwnd->SetStatusText_3(mtrstr);
}
*/
void ChartoDec(char* tmp, int &rval)
{
	// 2진수 char[13]를 10진수로...
	int aa, i, itmp;

	aa = 0;

	for(i=0;i<12;i++)
	{
		if(tmp[i] == '0') itmp = 0;
		else itmp = 1;
		
		aa += (int)(itmp)*(int)pow(2.0,(double)(i));
	}

	rval = aa;
}

void DectoBin(int tmp,char* rval)
{
	int i;
	// 10진수를 2진수 char로....

	for(i=0;i<12;i++)
	{
		if(tmp%2 == 0) rval[i] = '0';
		else rval[i] = '1';

		tmp = (int)((double)tmp/2.0);
	}
}

void GetDigitalPressure(int sel, int pressure, int &rval)
{
	//int aa;

/*	if(sel == 15) // MTU
	{
		aa = (int)((3500.0-500.0)/((double)PressureCalib_Y2-(double)PressureCalib_Y1)
			*((double)pressure-(double)PressureCalib_Y1)+500.0);
	}
	else if(sel == 16) // ATU
	{
		aa = (int)((3500.0-500.0)/((double)PressureCalib_Y4-(double)PressureCalib_Y3)
			*((double)pressure-(double)PressureCalib_Y3)+500.0);
	}

	rval = aa; 	
	*/
}

void GetRealPressure(int sel, int dpressure, int &rval)
{
//	int aa;
/*
	if(sel == 15) // MTU
	{
		aa = (int)(((double)PressureCalib_Y2-(double)PressureCalib_Y1)/((double)ReadPressureCalib_X2-(double)ReadPressureCalib_X1)
			*((double)dpressure-(double)ReadPressureCalib_X1)+(double)PressureCalib_Y1);
	}
	else if(sel == 16) // ATU
	{
		aa = (int)(((double)PressureCalib_Y4-(double)PressureCalib_Y3)/((double)ReadPressureCalib_X4-(double)ReadPressureCalib_X3)
			*((double)dpressure-(double)ReadPressureCalib_X3)+(double)PressureCalib_Y3);
	}

	rval = aa; 	
	*/
}

BOOL ChangeTouch(BOOL act)
{
//	if( act )
//		return ( OutputOff(oChangeTouch) );
//	else 
//		return ( OutputOn(oChangeTouch) );
	return TRUE;
}


BOOL ReadPressure(int chno, int &val/* 12Bit Digital*/)
{
/*
	char temp[13] = "000000000000";

	if(chno == 15) // MTU
	{
		if(InputIsOn(iMTUPressure_00))	temp[0] = '1';
		else temp[0] = '0';

		if(InputIsOn(iMTUPressure_01))	temp[1] = '1';
		else temp[1] = '0';

		if(InputIsOn(iMTUPressure_02))	temp[2] = '1';
		else temp[2] = '0';

		if(InputIsOn(iMTUPressure_03))	temp[3] = '1';
		else temp[3] = '0';

		if(InputIsOn(iMTUPressure_04))	temp[4] = '1';
		else temp[4] = '0';

		if(InputIsOn(iMTUPressure_05))	temp[5] = '1';
		else temp[5] = '0';

		if(InputIsOn(iMTUPressure_06))	temp[6] = '1';
		else temp[6] = '0';

		if(InputIsOn(iMTUPressure_07))	temp[7] = '1';
		else temp[7] = '0';

		if(InputIsOn(iMTUPressure_08))	temp[8] = '1';
		else temp[8] = '0';

		if(InputIsOn(iMTUPressure_09))	temp[9] = '1';
		else temp[9] = '0';

		if(InputIsOn(iMTUPressure_10))	temp[10] = '1';
		else temp[10] = '0';

		if(InputIsOn(iMTUPressure_11))	temp[11] = '1';
		else temp[11] = '0';
	}
	else if(chno == 16)// ATU
	{
		if(InputIsOn(iATUPressure_00))	temp[0] = '1';
		else temp[0] = '0';

		if(InputIsOn(iATUPressure_01))	temp[1] = '1';
		else temp[1] = '0';

		if(InputIsOn(iATUPressure_02))	temp[2] = '1';
		else temp[2] = '0';

		if(InputIsOn(iATUPressure_03))	temp[3] = '1';
		else temp[3] = '0';

		if(InputIsOn(iATUPressure_04))	temp[4] = '1';
		else temp[4] = '0';

		if(InputIsOn(iATUPressure_05))	temp[5] = '1';
		else temp[5] = '0';

		if(InputIsOn(iATUPressure_06))	temp[6] = '1';
		else temp[6] = '0';

		if(InputIsOn(iATUPressure_07))	temp[7] = '1';
		else temp[7] = '0';

		if(InputIsOn(iATUPressure_08))	temp[8] = '1';
		else temp[8] = '0';

		if(InputIsOn(iATUPressure_09))	temp[9] = '1';
		else temp[9] = '0';

		if(InputIsOn(iATUPressure_10))	temp[10] = '1';
		else temp[10] = '0';

		if(InputIsOn(iATUPressure_11))	temp[11] = '1';
		else temp[11] = '0';
	}
	else
	{
		return FALSE;
	}
	
	ChartoDec(temp, val);
*/
	return TRUE;
}

BOOL WritePressure(int chno, int val)
{
//	char temp[13]="000000000000";
/*
	DectoBin(val, temp);

	if(chno == 15) // MTU
	{
		if(temp[0] == '1')	OutputOn(oMTUPressure_00);
		else OutputOff(oMTUPressure_00);

		if(temp[1] == '1')	OutputOn(oMTUPressure_01);
		else OutputOff(oMTUPressure_01);

		if(temp[2] == '1')	OutputOn(oMTUPressure_02);
		else OutputOff(oMTUPressure_02);

		if(temp[3] == '1')	OutputOn(oMTUPressure_03);
		else OutputOff(oMTUPressure_03);

		if(temp[4] == '1')	OutputOn(oMTUPressure_04);
		else OutputOff(oMTUPressure_04);

		if(temp[5] == '1')	OutputOn(oMTUPressure_05);
		else OutputOff(oMTUPressure_05);

		if(temp[6] == '1')	OutputOn(oMTUPressure_06);
		else OutputOff(oMTUPressure_06);

		if(temp[7] == '1')	OutputOn(oMTUPressure_07);
		else OutputOff(oMTUPressure_07);

		if(temp[8] == '1')	OutputOn(oMTUPressure_08);
		else OutputOff(oMTUPressure_08);

		if(temp[9] == '1')	OutputOn(oMTUPressure_09);
		else OutputOff(oMTUPressure_09);

		if(temp[10] == '1')	OutputOn(oMTUPressure_10);
		else OutputOff(oMTUPressure_10);

		if(temp[11] == '1')	OutputOn(oMTUPressure_11);
		else OutputOff(oMTUPressure_11);
	}
	else if(chno == 16)// ATU
	{
		if(temp[0] == '1')	OutputOn(oATUPressure_00);
		else OutputOff(oATUPressure_00);

		if(temp[1] == '1')	OutputOn(oATUPressure_01);
		else OutputOff(oATUPressure_01);

		if(temp[2] == '1')	OutputOn(oATUPressure_02);
		else OutputOff(oATUPressure_02);

		if(temp[3] == '1')	OutputOn(oATUPressure_03);
		else OutputOff(oATUPressure_03);

		if(temp[4] == '1')	OutputOn(oATUPressure_04);
		else OutputOff(oATUPressure_04);

		if(temp[5] == '1')	OutputOn(oATUPressure_05);
		else OutputOff(oATUPressure_05);

		if(temp[6] == '1')	OutputOn(oATUPressure_06);
		else OutputOff(oATUPressure_06);

		if(temp[7] == '1')	OutputOn(oATUPressure_07);
		else OutputOff(oATUPressure_07);

		if(temp[8] == '1')	OutputOn(oATUPressure_08);
		else OutputOff(oATUPressure_08);

		if(temp[9] == '1')	OutputOn(oATUPressure_09);
		else OutputOff(oATUPressure_09);

		if(temp[10] == '1')	OutputOn(oATUPressure_10);
		else OutputOff(oATUPressure_10);

		if(temp[11] == '1')	OutputOn(oATUPressure_11);
		else OutputOff(oATUPressure_11);
	}
	else
	{
		return FALSE;
	}
*/	return TRUE;
}

CString ChoiceKorEngString(CString strText)
{
//	strText.Format("%s",Text);
	//////////////////////....... 한/영 전환 ........///////////////
	int no = strText.Find('$',0);
//	int length = strText.GetLength();
	if(no != -1)
	{
//		if(g_bEnglishDisplay) 	strText = strText.Left(no);
//		else 	strText = strText.Mid(no+1,length-no);
	}
	return strText;
}