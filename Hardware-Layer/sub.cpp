#include "stdafx.h"
#include "sub.h"
#include "mxtool.h"
#include "comm.h"
#include "PComm.h"
#include "string.h"

//extern char *Send_Buf;/
//extern char *Rec_Buf;
COMMDATA	GCommData1;
COMMDATA	GCommData2;

static	HANDLE	hExit1;
static	HANDLE	hCommWatchThread1;

UINT	CommWatchProc1(LPVOID pParam);

#define _COM_MAX_RECV_BUF_	2000

int		ComLenCnt1, rec_count1, rec_count1_Bak;
BYTE	ComRecBuf1[_COM_MAX_RECV_BUF_];
bool	ComFlag1;

//--- 클래스 생성자
CSub::CSub()
{
	ComLenCnt1 = 0;
	ComRecBuf1[0] = 0;
	ComFlag1 = false;
	rec_count1 = 0;

}

CSub::~CSub()
{

}

BOOL CSub::OpenPort(int Baud)
{
	DWORD   dwThreadID1;
	int     ret;

	GCommData1.Port = 3;

	GCommData1.ibaudrate = Baud;
	GCommData1.iparity = 0;
	GCommData1.ibytesize = 3;
	GCommData1.istopbits = 1;
	GCommData1.BaudRate = Baud;
	GCommData1.Parity = P_NONE;
	GCommData1.ByteSize = COM_BIT_8;
	GCommData1.StopBits = STOP_1;
	GCommData1.Hw = FALSE;
	GCommData1.Sw = FALSE;
	GCommData1.Dtr = TRUE;
	GCommData1.Rts = TRUE;

	GbOpen1 = FALSE;

	if ((ret = sio_open(GCommData1.Port)) != SIO_OK){
		TRACE("sio1 open error");
		return FALSE;
	}
	if (!PortSet1()){
		sio_close(GCommData1.Port);
		return FALSE;
	}
	if ((hExit1 = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
		return FALSE;
	hCommWatchThread1 = CreateThread((LPSECURITY_ATTRIBUTES)NULL,
		0,
		(LPTHREAD_START_ROUTINE)CommWatchProc1,
		(LPVOID)NULL,
		0, &dwThreadID1);
	if (hCommWatchThread1 == NULL){
		CloseHandle(hExit1);
		sio_close(GCommData1.Port);
		return FALSE;
	}
	GbOpen1 = TRUE;
	/////////////////////////////////
	/////////////////////////////////
	ShowStatus();
	return TRUE;
}


BOOL CSub::PortSet1(void)
{
	int	port = GCommData1.Port;
	int	mode = GCommData1.Parity | GCommData1.ByteSize | GCommData1.StopBits;
	int	hw = GCommData1.Hw ? 3 : 0;	/* bit0 and bit1 */
	int	sw = GCommData1.Sw ? 12 : 0;     /* bit2 and bit3 */
	int	ret;
	DWORD   tout;

	if ((ret = sio_ioctl(port, GCommData1.BaudRate, mode)) != SIO_OK){
		//          MxShowError("sio_ioctl",ret);
		TRACE("sio_ioctl error");
		return FALSE;
	}

	if ((ret = sio_flowctrl(port, hw | sw)) != SIO_OK){
		//            MxShowError("sio_flowctrl",ret);
		TRACE("sio_flowctrl error");
		return FALSE;
	}

	if ((ret = sio_DTR(port, (GCommData1.Dtr ? 1 : 0))) != SIO_OK){
		//            MxShowError("sio_DTR",ret);
		TRACE("sio_DTR error");
		return FALSE;
	}

	if (!GCommData1.Hw){
		if ((ret = sio_RTS(port, (GCommData1.Rts ? 1 : 0))) != SIO_OK){
			//                MxShowError("sio_RTS",ret);
			TRACE("sio_RTS error");
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

	ShowStatus();
	return TRUE;
}
BOOL CSub::ClosePort(void)
{

	SetEvent(hExit1);

	if (WaitForSingleObject(hCommWatchThread1, 1000) == WAIT_TIMEOUT)
		TerminateThread(hCommWatchThread1, 0);
	CloseHandle(hExit1);
	CloseHandle(hCommWatchThread1);
	sio_close(GCommData1.Port);
	GbOpen1 = FALSE;
	///////////////////////////////////
	ShowStatus();

	return TRUE;
}

void CSub::ShowStatus(void)
{
}

UINT CommWatchProc1(LPVOID pParam)
{
	int	len;
	char	buf[512];
	DWORD	dRes;

	ComLenCnt1 = 0; rec_count1 = 0;
	ComFlag1 = false;

	while (1)
	{
		dRes = WaitForSingleObject(hExit1, 100);
		switch (dRes){
		case WAIT_OBJECT_0:/* got hExit event,exit thread. */
			return 0;
		default:
			break;
		}

		int i;
		bool okflag = false;
		int endcnt = 0;

		len = sio_read(GCommData1.Port, buf, 128);
		if (len > 0)
		{
			for (i = 0; i < len; i++)
			{
				ComRecBuf1[rec_count1++] = buf[i];
				if (_COM_MAX_RECV_BUF_ <= rec_count1) rec_count1 = 0;
			}
		}
	}
}
void CSub::Com_Write1(char *buf)
{
	sio_write(GCommData1.Port, buf, strlen(buf));
}

void CSub::ComBufInit1()
{
	ComLenCnt1 = 0;
	for (int i = 0; i < 100; i++) 	ComRecBuf1[i] = 0;
	ComFlag1 = false;
	rec_count1 = 0;
}

bool CSub::isGetData1()
{
	if (rec_count1_Bak != rec_count1) return  true;	else return false;
}
bool CSub::Get_DancerData1(double *retDouble)
{
	//	if(rec_count2== rec_count2_bak) return DataBuffer1;
	//	*retDouble= -999.0;
	int cnt = rec_count1;
	int i, end_cnt = 0, count = 0;
	char	ReadBufferTmp[512];
//	char	ComBuf[50];

	ComFlag1 = false;
	int ch_cnt = 0;


	if (cnt <= 15)	{ cnt = _COM_MAX_RECV_BUF_ - 1; /*TRACE("buf=0\n");*/ }
	for (i = cnt - 1; i >= 0; i--, ch_cnt++)
	{
		if (ComRecBuf1[i - 1] == 0xEF)
		{
			end_cnt = i;
			if (ch_cnt< 5) continue;
			break;
		}
	}
	if (end_cnt > 0 && ComRecBuf1[end_cnt - 2] == 0x00 && ComRecBuf1[end_cnt - 3] == 0x00 && ComRecBuf1[end_cnt - 8] == 0xea)
	{
		ReadBufferTmp[0] = ComRecBuf1[end_cnt - 4];
		ReadBufferTmp[1] = ComRecBuf1[end_cnt - 5];
		ReadBufferTmp[2] = ComRecBuf1[end_cnt - 6];
		ReadBufferTmp[3] = ComRecBuf1[end_cnt - 7];
		ReadBufferTmp[4] = 0;
		//		DataBuffer1.Format("%s", ReadBufferTmp);
		double CnvData = 0.0;
		unsigned long lCnvData = 0L;
		lCnvData = lCnvData | (ReadBufferTmp[0] & 0x0f);
		lCnvData = lCnvData | (ReadBufferTmp[0] & 0xf0);

		lCnvData = lCnvData | ((ReadBufferTmp[1] & 0x0f) << 8);
		lCnvData = lCnvData | ((ReadBufferTmp[1] & 0xf0) << 8);

		lCnvData = lCnvData | ((ReadBufferTmp[2] & 0x0f) << 16);
		lCnvData = lCnvData | ((ReadBufferTmp[2] & 0xf0) << 16);

		lCnvData = lCnvData | ((ReadBufferTmp[3] & 0x0f) << 24);
		lCnvData = lCnvData | ((ReadBufferTmp[3] & 0xf0) << 24);
		if (lCnvData == 0)
		{
			TRACE("Dan=0\n");
		}
		*retDouble = (double)(lCnvData);
		ComFlag1 = true;
	}
	else
	{
		//		DataBuffer1.Format("???");
		TRACE("1...1\n");
		return false;
	}
	//		ComBufInit1();
	//	TRACE("1.[%d]\n", rec_count1);
	return true;
}
double	CSub::GetDoubleData1()
{
	double ret = 0.0;

	return ret;
}

char CSub::DecToHexAsc(int dat)
{
	if (dat >= 10) {
		return (dat - 10) + 'A';
	}
	else {
		return dat + '0';
	}
}
int CSub::HexAscToDec(char dat)
{
	if (dat >= '0' && dat <= '9')		return dat - '0';
	else if (dat >= 'A' && dat <= 'F') return dat - 'A' + 10;
	else return 0;
}
