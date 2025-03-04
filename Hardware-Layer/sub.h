class	CSub
{
public:

	CSub();
	~CSub();

public:

	BOOL	GbOpen1;

	BOOL	OpenPort(int Baud);
	BOOL	ClosePort(void);

	BOOL	PortSet1(void);

	void 	ShowStatus(void);
	void	Com_Write1(char *buf);
	void	ComBufInit1();
	bool	Get_DancerData1(double *retDouble);

	// 	CString DataBuffer1;
	// 	CString DataBuffer2;
	double	GetDoubleData1();

	bool isGetData1();
	char DecToHexAsc(int dat);
	int HexAscToDec(char dat);
private:

};

//extern CSub Sub;

/*
extern	int		ComLenCnt;
extern	BYTE	ComRecBuf[1000];
extern	bool	ComFlag;
extern  int		rec_count;
*/
