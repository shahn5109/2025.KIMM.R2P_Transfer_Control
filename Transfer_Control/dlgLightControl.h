#pragma once


// CdlgLightControl ��ȭ �����Դϴ�.

class CdlgLightControl : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgLightControl)

public:
	CdlgLightControl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CdlgLightControl();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MANU_LIGHT };

	CSliderCtrl m_ctrlLedSlider1, m_ctrlLedSlider2, m_ctrlLedSlider3, m_ctrlLedSlider4;

private:
	CParameterFile* ParamFile();
	CParameterFile* Log();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
