// CommProc.cpp: implementation of the CCommProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommProc.h"
#include "MainFrm.h"
#include "string.h"
#include <direct.h>	// Dos Command. //
#include "stdio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CSystemClass pSystem;
CParameterFile* CCommProc::Log()		{ return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
CParameterFile* CCommProc::ParamFile()	{ return (CParameterFile*)(pSystem.GetParameterFileComponent()); }
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommProc::CCommProc()
{
	InitFlag = false;
	m_iPortNo = 0;

	ComLenCnt = 0;
	ComRecBuf[0] = 0;
	ComFlag = false;
	rec_count = 0;

	m_pThread = NULL;
	m_bThreadLife = FALSE;
	m_bThreadTerminate = FALSE;

	WH_Count = 0.0;
}

CCommProc::~CCommProc()
{
// 	if (InitFlag)
// 	{
// 		DWORD exitcode;
// 	}
}

//===================================================================================
UINT CCommProc::threadCommProcFunction(LPVOID pParam)
//-----------------------------------------------------------------------------------
{
	return 0;
}

bool CCommProc::ComInit(int port)
{
	m_iPortNo = port;

	CString strText;

	strText.Format(L"[COM_Init] => ComPort=%d", port);
	Log()->SaveLogFile(_SYSTEM_LOG, strText);

	//Ahn_Test End
	//	InitFlag= true;
	if (OpenPort(port, B9600))	InitFlag = true;
	else { return false; }
	//Ahn_Test End

	return true;
}

BOOL CCommProc::OpenPort(int Port, int Baud)
{
	int     ret;
	CString strText;

	GCommData.Port = Port;
	GCommData.ibaudrate = Baud;
	GCommData.iparity = 0;
	GCommData.ibytesize = 3;
	GCommData.istopbits = 1;
	GCommData.BaudRate = Baud;
	GCommData.Parity = P_NONE;
	GCommData.ByteSize = COM_BIT_8;
	GCommData.StopBits = STOP_1;
	GCommData.Hw = FALSE;
	GCommData.Sw = FALSE;
	GCommData.Dtr = TRUE;
	GCommData.Rts = TRUE;

	if ((ret = sio_open(GCommData.Port)) != SIO_OK){
		strText.Format(L"[COM_Init] => sio open error! [ComPort=%d]", Port);
		Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
		return FALSE;
	}
	strText.Format(L"[COM_Init] => sio open! [ComPort=%d]", Port);
	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));
	if (!PortSet()){
		sio_close(GCommData.Port);
		return FALSE;
	}
	InitFlag= true;
	return TRUE;
}

BOOL CCommProc::PortSet(void)
{
	int	port = GCommData.Port;
	int	mode = GCommData.Parity | GCommData.ByteSize | GCommData.StopBits;
	int	hw = GCommData.Hw ? 3 : 0;	/* bit0 and bit1 */
	int	sw = GCommData.Sw ? 12 : 0;     /* bit2 and bit3 */
	int	ret;
	DWORD   tout;

	if ((ret = sio_ioctl(port, GCommData.BaudRate, mode)) != SIO_OK){
		Log()->SaveLogFile(_SYSTEM_LOG, L"[COM-PORT] sio_ioctl error");
		return FALSE;
	}

	if ((ret = sio_flowctrl(port, hw | sw)) != SIO_OK){
		Log()->SaveLogFile(_SYSTEM_LOG, L"[COM-PORT] sio_flowctrl error");
		return FALSE;
	}

	if ((ret = sio_DTR(port, (GCommData.Dtr ? 1 : 0))) != SIO_OK){
		Log()->SaveLogFile(_SYSTEM_LOG, L"[COM-PORT] sio_DTR error");
		return FALSE;
	}

	if (!GCommData.Hw){
		if ((ret = sio_RTS(port, (GCommData.Rts ? 1 : 0))) != SIO_OK){
			Log()->SaveLogFile(_SYSTEM_LOG, L"[COM-PORT] sio_RTS error");
			return FALSE;
		}
	}

	tout = 1000 / sio_getbaud(port);  /* ms/byte */
	if (tout < 1)
		tout = 1;
	tout = tout * 1 * 3;             /* 1 byte; '*3' is for delay */
	//     sio_SetWriteTimeouts(port, tout);

	DWORD		baud;

	baud = sio_getbaud(port);
	tout = ((10 * 1024) / (baud / 10) * 3000) * 3;
	//		sio_SetReadTimeouts(port, 0, 0);

	return TRUE;
}

BOOL CCommProc::ClosePort(void)
{
	CString strText;

	strText.Format(L"[COM_CLOSE] => sio Close! [ComPort=%d]", GCommData.Port);
	Log()->SaveLogFile(_SYSTEM_LOG, strText.GetBuffer(0));

	sio_close(GCommData.Port);

	InitFlag = false;

	return TRUE;
}

char CCommProc::DecToHexAsc(int dat)
{
	if (dat >= 10) {
		return (dat - 10) + 'A';
	}
	else {
		return dat + '0';
	}
}
int CCommProc::HexAscToDec(char dat)
{
	if (dat >= '0' && dat <= '9')		return dat - '0';
	else if (dat >= 'A' && dat <= 'F') return dat - 'A' + 10;
	else return 0;
}
//===================================================================================
void CCommProc::SaveLogFile(int type, CString strText)
//-----------------------------------------------------------------------------------
{
/*
	char value[512];
	CString strFile__Name;

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString m_strDayLogDirName;

	m_strDayLogDirName.Format("%s\\%4d%02d%02d", ParamFile()->strDirSystemLogPath, st.wYear, st.wMonth, st.wDay); // File Name 생성
	_mkdir(m_strDayLogDirName);

	strFile__Name.Format("%s\\INV_COMM.Log", m_strDayLogDirName);

	FILE* fout = fopen(strFile__Name.GetBuffer(0), "a+");   // 파일에 계속 추가시킨다. //
	if (fout == NULL) 	{ return; }

	sprintf(value, "%s", strText);

	CString strMsg;
	if (type == _COM_WRITE) {
		strMsg.Format("[WRITE][%02ld:%02ld:%02ld]\t[%s]", st.wHour, st.wMinute, st.wSecond, value);
	}
	if (type == _COM_REC_ERROR)
		strMsg.Format("[ERROR][%02ld:%02ld:%02ld]\t[%s]", st.wHour, st.wMinute, st.wSecond, value);
	else
		strMsg.Format("[READ][%02ld:%02ld:%02ld]\t[%s]", st.wHour, st.wMinute, st.wSecond, value);
	fprintf(fout, "%s\n", strMsg);

	fclose(fout);
*/
}

/*
void CCommProc::Com_StrWrite(char *buf)
{
	sio_write(GCommData.Port, buf, strlen(buf));
}

bool CCommProc::Com_Write(int Unit_Id, int mode)
{
	CString strText;
	char	ComSendBuf[100];
	int i = 0;
	int Inv_Id = Unit_Id;

	ZeroMemory(ComSendBuf, sizeof(ComSendBuf));
	ComSendBuf[i++] = '^';
	ComSendBuf[i++] = 'P';
	ComSendBuf[i++] = (Inv_Id / 100) + '0';
	ComSendBuf[i++] = (Inv_Id / 10) + '0';
	ComSendBuf[i++] = Inv_Id + '0';

	ComSendBuf[i] = 0;
	ComBufInit();
	sio_write(GCommData.Port, ComSendBuf, strlen(ComSendBuf));
	return true;
}
*/
void CCommProc::ComBufInit()
{
	char	buf[512];
	sio_read(GCommData.Port, buf, 512);
	ComLenCnt = 0;
	for (int i = 0; i< 255; i++) 	ComRecBuf[i] = 0;
	ComFlag = false;
	rec_count = 0;
}

//===================================================================================
BOOL CCommProc::GetData(int msTimeOut)
//-----------------------------------------------------------------------------------
{
	//Ahn_Test Start
	//	Data_Cnv(Unit_Id, mode);
	//	return TRUE;
	//Ahn_Test End

	if(ParamFile()->System_data.Led_Remote== 0) return TRUE;

	int	len, i;
	char	buf[512];
	CString aaa, bb;
	DWORD st = GetTickCount();
	CString strText;
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString straaa;

	//Com_Write(Unit_Id, mode);	//	sio_SetReadTimeouts(GCommData.Port, 1000, 0);
	if(!InitFlag)
	{
		straaa.Format(_T("[LED-Controller : ERROR] Port-Open\n"));
		if(pFrm->isView_Init()) pFrm->GetWindowHandler_DebugView()->ListPrint(straaa.GetBuffer(0));
		return FALSE;
	}
	while (1) 
	{
		len = sio_read(GCommData.Port, buf, 512);

		if (len> 0){
			buf[len] = '\x0';

			if (len> 200) len = 200;
			ComFlag = true;
			if(buf[0]== 0x75) return TRUE;

			for (i = 0; i < len + 1; i++) 
			{
				ComRecBuf[rec_count++] = buf[i];
			}
			
			rec_count = ComLenCnt = rec_count - 1;
//			TRACE("INV_TIME=> %d\n", GetTickCount() - st);
			ZeroMemory(RECV_BUF, sizeof(RECV_BUF));
			if (ComLenCnt> 900) ComLenCnt = 900;
			ComRecBuf[ComLenCnt] = 0;

			for (i = 0; i< ComLenCnt; i++) {
				if (i >= 125) { RECV_BUF[i] = 0; break; }
				RECV_BUF[i] = ComRecBuf[i];
			}
			RECV_BUF[i] = 0;
			strText.Format(L"%s", RECV_BUF);
			return TRUE;
		}
		if (GetTickCount() - st> (DWORD)(msTimeOut)) break;
		Sleep(1);
	}
	straaa.Format(_T("[LED-Controller : ERROR][%d] Time-Out=> %d\n"), len, GetTickCount() - st);
	if(pFrm->isView_Init()) pFrm->GetWindowHandler_DebugView()->ListPrint(straaa.GetBuffer(0));
	return FALSE;
}

BOOL CCommProc::LightVal(int ch, bool onoff, int val)
{
	CString strText;
	char	ComSendBuf[20];
	int i = 0;

	if(ParamFile()->System_data.Led_Remote== 0) return TRUE;

	ZeroMemory(ComSendBuf, sizeof(ComSendBuf));
//	val= 90;
	ComSendBuf[0] = 0x95;
	ComSendBuf[1] = 0x02;
	ComSendBuf[2] = ch;
	if (onoff) ComSendBuf[3] = 0x01; else ComSendBuf[3] = 0x00;
	ComSendBuf[4] = val;
	ComSendBuf[5] = (ComSendBuf[0] + ComSendBuf[1] + ComSendBuf[2] + ComSendBuf[3] + ComSendBuf[4]) & 0xff; //checksum

	ComSendBuf[6] = 0;
	ComBufInit();
	sio_write(GCommData.Port, ComSendBuf, 7); //return : 0x75 [OK]

	return GetData(500);
}

