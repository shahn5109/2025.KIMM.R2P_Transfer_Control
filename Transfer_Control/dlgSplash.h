#pragma once


// CdlgSplash ��ȭ �����Դϴ�.

class CdlgSplash : public CDialogEx
{
	DECLARE_DYNAMIC(CdlgSplash)

public:
	CdlgSplash(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CdlgSplash();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SPLASH };

public:
	CParameterFile* ParamFile();
	CParameterFile* Log();

	int SplashInitStep[10];
	bool m_bIsPaint;
	BOOL bIsSuccess;
	bool bIsInitEnd;

private:


#define _SPLASH_X_POS	0
#define _SPLASH_Y_POS	0

public:
	CProgressCtrl	m_ctrlProgress;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};
