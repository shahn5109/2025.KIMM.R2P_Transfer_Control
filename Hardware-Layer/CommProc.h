// InverterComm.h: interface for the CCommProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMPROC_H__08D168E8_670A_49A5_8CA3_29D2D1B8AEB2__INCLUDED_)
#define AFX_COMMPROC_H__08D168E8_670A_49A5_8CA3_29D2D1B8AEB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mxtool.h"
#include "comm.h"
#include "PComm.h"

class CCommProc
{
public:
	CCommProc();
	virtual ~CCommProc();

	COMMDATA	GCommData;

	bool	ComInit(int port);
	BOOL	OpenPort(int Port, int Baud);
	BOOL	ClosePort(void);
	BOOL	PortSet(void);

	void	ComBufInit();
	BOOL	GetData(int msTimeOut);

	BOOL	LightVal(int ch, bool onoff, int val);

	double WH_Count;

	CWnd *pWnd;

	int m_iPortNo;
	bool InitFlag;

	int		ComLenCnt, rec_count;
	BYTE	ComRecBuf[1000];
	BYTE	RECV_BUF[1000];

	bool	ComFlag;
	char	DecToHexAsc(int dat);
	int		HexAscToDec(char dat);

	/** Thread 구동 Flag*/
	BOOL m_bThreadLife;
	/** Thread의 Terminate Flag */
	BOOL m_bThreadTerminate;
	/** Thread Handle */
	CWinThread* m_pThread;

	/**
	* Thread 함수
	*/
	static UINT threadCommProcFunction(LPVOID pParam);

private:
	CParameterFile*		Log();
	CParameterFile*		ParamFile();
	void	SaveLogFile(int type, CString strText);
};

#endif // !defined(AFX_COMMPROC_H__08D168E8_670A_49A5_8CA3_29D2D1B8AEB2__INCLUDED_)
