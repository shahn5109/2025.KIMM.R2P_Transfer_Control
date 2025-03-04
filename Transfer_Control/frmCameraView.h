#pragma once

#include "../Common/ImageView/Include/xImageViewCtrl.h"
#include "../Common/ImageView/Include/ImageObj.h"
#include "../Common/ImageView/Include/xGraphicObject.h"

//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES

// CfrmCameraView 폼 뷰입니다.

class CfrmCameraView : public CFormView
{
	DECLARE_DYNCREATE(CfrmCameraView)

protected:
	CfrmCameraView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CfrmCameraView();

protected:
	CxImageViewCtrl*	m_pImageCtrl;
	CImageObj*		m_pImgObj;

public:
	enum { IDD = IDD_FORMVIEW_CAM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CBrush m_brush,	brush1;
	COLORREF BackColor;
	CBrush m_GreenBrush;
	CBrush m_YellowBrush;
	CBrush m_BlueBrush;
	CBrush m_RedBrush;
	CBrush m_GrayBrush;
	CBrush m_WhiteBrush;

	CBtnEnh	m_btnTopCam_2X;
	CBtnEnh	m_btnTopCam_20X;
	CBtnEnh	m_btnBotCam_2X;
	CBtnEnh	m_btnBotCam_20X;

	CBtnEnh m_btnTopCam_Live;
	CBtnEnh m_btnBotCam_Live;

	int		mTopCameraNo;
	int		mBotCameraNo;

	bool	bBotCam_Live,	bTopCam_Live;

public :
	static void APIENTRY OnDrawExt( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData );
	static void APIENTRY OnConfirmTracker( CRect& rcTrackRegion, UINT nIndexData, LPVOID lpUsrData );
	static BOOL APIENTRY OnFireMouseEvent(  DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData );

	void RegistModel( int nViewIdx );
	void CameraSeletUpdate();
	void DispCross(int camno, double posx, double posy, bool flag);
	BOOL MoveEndCheck(int Axis) ;
	BOOL AF_MoveEndCheckNonDisp(int Axis) ;
	BOOL AF_Proc_Run(int CamNo, double *Z_Pos);

private:
	CMatrox*				m_pVision;

public:
	CParameterFile*			ParamFile();
	CParameterFile*			Log();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_EVENTSINK_MAP()
	void ClickJogSelect0();
	void ClickJogSelect1();
	void ClickJogSelect2();
	void ClickJogSelect3();
	void ClickJogSelect4();
	void ClickJogSelect5();
	void DblClickJogSelect0();
	void DblClickJogSelect1();
	void DblClickJogSelect2();
	void DblClickJogSelect3();
	void DblClickCmMarkLoad();
	void DblClickCmMarkLoad2();
	afx_msg void OnBnClickedButton1();
};


