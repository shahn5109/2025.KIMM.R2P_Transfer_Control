//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


// CdlgStampInst_Jog 대화 상자입니다.

class CdlgStampInst_Jog : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgStampInst_Jog)

public:
	CdlgStampInst_Jog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgStampInst_Jog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANU_STAMP_AT };
	CBtnEnh	m_btnJogModeSel1;
	int Jog_Mode;

	BOOL MoveEndCheck(int Axis) ;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	BOOL m_bRollZ_Enable;
	DECLARE_EVENTSINK_MAP()
	void ClickJogSelect0();
	void ClickJogSelect1();
	void ClickJogSelect2();
	void ClickExit();
	void ClickExit2();
	void MouseDownJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCw(short Button, short Shift, long x, long y);
	void MouseDownJogZCcw(short Button, short Shift, long x, long y);
	void MouseUpJogZCcw(short Button, short Shift, long x, long y);
	void MouseDownJogZCw(short Button, short Shift, long x, long y);
	void MouseUpJogZCw(short Button, short Shift, long x, long y);
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedButton1();
	void ClickZWaitPos3();
};
