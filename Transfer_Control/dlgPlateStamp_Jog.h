//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#pragma once


// CdlgPlateStamp_Jog 대화 상자입니다.

class CdlgPlateStamp_Jog : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgPlateStamp_Jog)

public:
	CdlgPlateStamp_Jog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgPlateStamp_Jog();

public:
	CBtnEnh m_btnTitle;
	CBtnEnh	m_btnJogModeSel1;
	int Jog_Mode;

	double CurRoll_Pos_1, CurZ1_Pos_1, CurZ2_Pos_1;
	double CurRoll_Pos_2, CurZ1_Pos_2, CurZ2_Pos_2;

	double CurX_Pos_1, CurY_Pos_1;
	double CurX_Pos_2, CurY_Pos_2;

	int m_calib_Mode; // 0= X보정(Roll회전 사용), 1= Y보정 (Z축 사용)
	double retVal;


	BOOL MoveEndCheck(int Axis) ;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANU_STAMP_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();

	DECLARE_EVENTSINK_MAP()
	void ClickExit();
	void ClickExit2();
	void MouseDownJogPbCw(short Button, short Shift, long x, long y);
	void MouseUpJogPbCw(short Button, short Shift, long x, long y);
	void MouseDownJogPbCcw(short Button, short Shift, long x, long y);
	void MouseUpJogPbCcw(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCw(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCw(short Button, short Shift, long x, long y);
	void MouseDownJogStageTCcw(short Button, short Shift, long x, long y);
	void MouseUpJogStageTCcw(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCcw(short Button, short Shift, long x, long y);
	void MouseDownJogStageXCw(short Button, short Shift, long x, long y);
	void MouseUpJogStageXCw(short Button, short Shift, long x, long y);
	void MouseDownJogCameraYCcw(short Button, short Shift, long x, long y);
	void MouseUpJogCameraYCcw(short Button, short Shift, long x, long y);
	void MouseDownJogCameraYCw(short Button, short Shift, long x, long y);
	void MouseUpJogCameraYCw(short Button, short Shift, long x, long y);
	void MouseDownJogZ2Ccw(short Button, short Shift, long x, long y);
	void MouseUpJogZ2Ccw(short Button, short Shift, long x, long y);
	void MouseDownJogZ2Cw(short Button, short Shift, long x, long y);
	void MouseUpJogZ2Cw(short Button, short Shift, long x, long y);
	void MouseDownJogZCcw(short Button, short Shift, long x, long y);
	void MouseUpJogZCcw(short Button, short Shift, long x, long y);
	void MouseDownJogZCw(short Button, short Shift, long x, long y);
	void MouseUpJogZCw(short Button, short Shift, long x, long y);
	void ClickJogSelect0();
	void ClickJogSelect1();
	void ClickJogSelect2();
	void ClickJogSelect6();
	void ClickJogSelect7();
	void ClickCmMove6();
	afx_msg void OnBnClickedButton1();
	double m_dStamp_Mark;
	double m_dPosT1;
	double m_dPosT2;
	double m_dPosZ2_1;
	double m_dPosZ2_2;
};
