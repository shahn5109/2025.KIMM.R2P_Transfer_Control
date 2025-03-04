#pragma once


// CdlgLightControl 대화 상자입니다.

class CdlgLightControl : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgLightControl)

public:
	CdlgLightControl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CdlgLightControl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MANU_LIGHT };

	CSliderCtrl m_ctrlLedSlider1, m_ctrlLedSlider2, m_ctrlLedSlider3, m_ctrlLedSlider4;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ClickExit();
	int m_iLightVal1;
	int m_iLightVal2;
	int m_iLightVal3;
	int m_iLightVal4;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
