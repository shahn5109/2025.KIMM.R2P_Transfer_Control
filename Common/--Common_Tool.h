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
BOOL		FileDeleteAll(char * pn);
BOOL		CheckDirExist(char * pn);
BOOL		CheckFileExist(char * pn);
int			FileCountInDir(char* pn);
int			DirCountInDir(char* pn);
BOOL		CopyFileDir(char* dest, char* src, int deletemode=FALSE);
void		SetStatusText(LPCTSTR sText);
void		DisplayMessage(char* msg);
UINT		GetIOKey(BOOL sts);

BOOL		ErrorDisplay(UINT err);
BOOL		ErrorDisplay(UINT err, BOOL AutoMode);
void		ClearError();

int			get_fpoint(FILE* fp, char* s, char* sbuf);

UINT		MyMessageBox(char* message, char* title, UINT type=M_OK,
		  						 char* YesBut="Yes", char* NoBut="No");

UINT		MyMessageBox(UINT type, char* title, 
		  						 char* YesBut, char* NoBut, char* message ...);

UINT		MyMessageBox(UINT i, char* title, UINT type=M_OK,
		  						 char* YesBut="Yes", char* NoBut="No");

UINT		MyMessageBox(char* tmp, UINT i, char* title, UINT type, char* YesBut="Yes", char* NoBut="No");

BOOL		GetNumberBox(char* szNum, int maxchar = 10, 
								 char* getnum = "", char* title = "Insert Number Only.",
																	BOOL pwd = FALSE);
BOOL		GetValueBox(char* szVal, int maxchar = 10, 
								char* getval = "", char* title = "Insert Character",
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
double ConvertPointNum(char* val, int cnt);
int ConvertPointNum(char* val);
void ChartoDec(char* tmp, int &rval);
void DectoBin(int tmp,char* rval);
void GetDigitalPressure(int sel, int pressure, int &rval);
void GetRealPressure(int sel, int dpressure, int &rval);

double rad2deg(double s);
double deg2rad(double s);
void Set_OP_Status(UINT op_status);
UINT Get_OP_Status();

void TempCheckSum(char* str);


BOOL DispTactTime(double dt);

BOOL DisplayPLULength(double dl, double dt);
BOOL DisplayPCBLength(double dl, double dt);
BOOL DisplayLCDLength(double dl, double dt);
BOOL DisplayBAULength(double dl, double dt);
BOOL DisplayPIULengthLeft(double dl, double dt);
BOOL DisplayPIULengthRight(double dl, double dt);
BOOL DisplayPIULength(double dl, double dt);

BOOL DisplayLCDIs(char* lcdid);
BOOL DisplayPCBIs(CString pcbid);
BOOL DisplaySTUPCBLeftBlob(int dt);
BOOL DisplaySTUPCBRightBlob(int dt);
BOOL DisplayACFCheckBlob(int dt);

BOOL WritePressure(int chno, int val);
BOOL ReadPressure(int chno, int &val/* 12Bit Digital*/);
BOOL SetTempControl(int UtNo, double Temp);
double ReadTempControl(int UtNo);

CString ChoiceKorEngString(CString strText);


//_________________________________________________________________

#endif
