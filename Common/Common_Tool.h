#ifndef _COMMON_TOOL_HEADER
#define _COMMON_TOOL_HEADER

#include "MsgDlg.h"
#include "MyProgressWnd.h"
#include "MyMsgWnd.h"
#include "MyWnd.h"


// Button Color
const long DEF_OFF_COLOR = 0x008A8A8A;
const long DEF_ON_COLOR = 0x0095CAFF;

/////////////////////////////////////
//*----- For MyMessageBox --------*//

#define  M_ICONINFORMATION    0x01 
#define  M_ICONQUESTION       0x02 
#define  M_ICONSTOP           0x04 
#define  M_ICONERROR          0x08 	

#define  M_OK                 0x10
#define  M_OKCANCEL           0x20
#define  M_YESNO              0x40
#define  M_ABORTRETRYIGNORE   0x80
#define  M_RETRYCANCEL        0x100

//*----- For MyMessageBox --------*//
/////////////////////////////////////

////////////////////////////////////////////////
//////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*------------------------------- For Common Function --------------------------------------------*//

CString ReturnSelect(UINT ret);

BOOL ChangeTouch(BOOL act);

int			MyAbs( int val );
double		MyAbs( double val );
BOOL		FileDeleteAll(TCHAR * pn);
BOOL		CheckDirExist(TCHAR * pn);
BOOL		CheckFileExist(TCHAR * pn);
int			FileCountInDir(TCHAR* pn);
int			DirCountInDir(TCHAR* pn);
BOOL		CopyFileDir(TCHAR* dest, TCHAR* src, int deletemode=FALSE);
void		SetStatusText(LPCTSTR sText);
void		DisplayMessage(TCHAR* msg);
UINT		GetIOKey(BOOL sts);

BOOL		ErrorDisplay(UINT err);
BOOL		ErrorDisplay(UINT err, BOOL AutoMode);
void		ClearError();

int			get_fpoint(FILE* fp, TCHAR* s, TCHAR* sbuf, int nLen);

UINT		MyMessageBox(TCHAR* message, TCHAR* title, UINT type=M_OK,
		  						 TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"));

UINT		MyMessageBox(UINT type, TCHAR* title, 
		  						 TCHAR* YesBut, TCHAR* NoBut, TCHAR* message ...);

UINT		MyMessageBox(UINT i, TCHAR* title, UINT type=M_OK,
		  						 TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"));

UINT		MyMessageBox(TCHAR* tmp, UINT i, TCHAR* title, UINT type, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"));

BOOL		GetNumberBox(TCHAR* szNum, int maxchar = 10, 
								 TCHAR* getnum = _T(""), TCHAR* title = _T("Insert Number Only."),
																	BOOL pwd = FALSE);
BOOL		GetValueBox(TCHAR* szVal, int maxchar = 10, 
								TCHAR* getval = _T(""), TCHAR* title = _T("Insert Character"),
								BOOL pwd = FALSE, BOOL btndisable = FALSE);

//BOOL TactTimeSave(char* pn);

//*------------------------------- For Common Function --------------------------------------------*//
//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DispMainSheetCount_1();
BOOL DispMainSheetCount_2();
BOOL DispMainSheetCount_3();

BOOL DispPressingTime(double dt);
BOOL DispCoolingTime(double dt);
BOOL DispAcfPressingTime(double dt);
BOOL DispAcfCoolingTime(double dt);

void DispAcfPressCnt();
void DispMainPressCnt();
BOOL ErrorListSave(char* pn);

BOOL STUCamDataInitial();
DWORD HexToInt(char* s);

double ConvertPointNum(double val, int cnt);
double ConvertPointNum(TCHAR* val, int cnt);
int ConvertPointNum(TCHAR* val);
void ChartoDec(TCHAR* tmp, int &rval);
void DectoBin(int tmp,TCHAR* rval);
void GetDigitalPressure(int sel, int pressure, int &rval);
void GetRealPressure(int sel, int dpressure, int &rval);

double rad2deg(double s);
double deg2rad(double s);
void Set_OP_Status(UINT op_status);
UINT Get_OP_Status();

void TempCheckSum(TCHAR* str);


BOOL DispTactTime(double dt);

BOOL DisplayPLULength(double dl, double dt);
BOOL DisplayPCBLength(double dl, double dt);
BOOL DisplayLCDLength(double dl, double dt);
BOOL DisplayBAULength(double dl, double dt);
BOOL DisplayPIULengthLeft(double dl, double dt);
BOOL DisplayPIULengthRight(double dl, double dt);
BOOL DisplayPIULength(double dl, double dt);

BOOL DisplayLCDIs(TCHAR* lcdid);
BOOL DisplayPCBIs(CString pcbid);
BOOL DisplaySTUPCBLeftBlob(int dt);
BOOL DisplaySTUPCBRightBlob(int dt);
BOOL DisplayACFCheckBlob(int dt);

BOOL WritePressure(int chno, int val);
BOOL ReadPressure(int chno, int &val/* 12Bit Digital*/);
BOOL SetTempControl(int UtNo, double Temp);
double ReadTempControl(int UtNo);

CString ChoiceKorEngString(CString strText);

inline void CreateFolder( LPCTSTR lpszPath )
{
	CString strPath = lpszPath;

	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;

	while( (nEnd = strPath.Find('\\', nStart)) >= 0)
	{
		{
			CString csToken = strPath.Mid(nStart, nEnd-nStart);
			CreateDirectory(csPrefix + csToken, NULL);

			csPrefix += csToken;
			csPrefix += _T("/");
			nStart = nEnd+1;
		}
	}
	csToken = strPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);
}

//_________________________________________________________________

#endif
