// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// 모든 권리 보유.

// MainFrm.h : CMainFrame 클래스의 인터페이스
//


//===================================================================================
#include "MultiViewSplitter.h"

//#include "dlgCamView.h"
#include "frmTitleView.h"
#include "frmDebugView.h"
#include "frmMainView.h"
#include "frmSubMenuView.h"
#include "frmManualView.h"
#include "frmSetupView.h"
#include "frmCalibView.h"
#include "frmCameraView.h"
#include "frmAlignView.h"
#include "frmTeachView.h"
#include "frmDataView.h"
#include "frmStampView.h"
#include "frmCompView.h"
//===================================================================================


#pragma once

class CMainFrame : public CFrameWndEx
{

protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 특성입니다.
public:

	// 작업입니다.
public:
	CMultiViewSplitter				m_wndSplitter1;
	CMultiViewSplitter				m_wndSplitter2;
	CMultiViewSplitter				m_wndSplitter3;
	CMultiViewSplitter				m_wndSplitter4;
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmTitleView;
	CfrmTitleView*					m_hfrmTitleView;
	CfrmTitleView*					GetWindowHandler_frmTitleView()						{	return m_hfrmTitleView;	}
	void							SetWindowHandler_frmTitleView(CfrmTitleView* hWnd)	{	m_hfrmTitleView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmDebugView;
	CfrmDebugView*					m_hfrmDebugView;
	CfrmDebugView*					GetWindowHandler_DebugView()						{	return m_hfrmDebugView;	}
	void							SetWindowHandler_DebugView(CfrmDebugView* hWnd)		{	m_hfrmDebugView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmMainView;
	CfrmMainView*					m_hfrmMainView;
	CfrmMainView*					GetWindowHandler_MainView()							{	return m_hfrmMainView;	}
	void							SetWindowHandler_MainView(CfrmMainView* hWnd)		{	m_hfrmMainView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmSubMenuView;
	CfrmSubMenuView*				m_hfrmSubMenuView;
	CfrmSubMenuView*				GetWindowHandler_SubMenuView()						{	return m_hfrmSubMenuView;	}
	void							SetWindowHandler_SubMenuView(CfrmSubMenuView* hWnd)	{	m_hfrmSubMenuView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmManualView;
	CfrmManualView*					m_hfrmManualView;
	CfrmManualView*					GetWindowHandler_ManualView()						{	return m_hfrmManualView;	}
	void							SetWindowHandler_ManualView(CfrmManualView* hWnd)	{	m_hfrmManualView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmSetupView;
	CfrmSetupView*					m_hfrmSetupView;
	CfrmSetupView*					GetWindowHandler_SetupView()						{	return m_hfrmSetupView;	}
	void							SetWindowHandler_SetupView(CfrmSetupView* hWnd)		{	m_hfrmSetupView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmTeachView;
	CfrmTeachView*					m_hfrmTeachView;
	CfrmTeachView*					GetWindowHandler_TeachView()						{	return m_hfrmTeachView;	}
	void							SetWindowHandler_TeachView(CfrmTeachView* hWnd)		{	m_hfrmTeachView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmAlignView;
	CfrmAlignView*					m_hfrmAlignView;
	CfrmAlignView*					GetWindowHandler_AlignView()						{	return m_hfrmAlignView;	}
	void							SetWindowHandler_AlignView(CfrmAlignView* hWnd)		{	m_hfrmAlignView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmCameraView;
	CfrmCameraView*					m_hfrmCameraView;
	CfrmCameraView*					GetWindowHandler_CameraView()						{	return m_hfrmCameraView;	}
	void							SetWindowHandler_CameraView(CfrmCameraView* hWnd)	{	m_hfrmCameraView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmCalibView;
	CfrmCalibView*					m_hfrmCalibView;
	CfrmCalibView*					GetWindowHandler_CalibView()						{	return m_hfrmCalibView;	}
	void							SetWindowHandler_CalibView(CfrmCalibView* hWnd)		{	m_hfrmCalibView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmDataView;
	CfrmDataView*					m_hfrmDataView;
	CfrmDataView*					GetWindowHandler_DataView()							{	return m_hfrmDataView;	}
	void							SetWindowHandler_DataView(CfrmDataView* hWnd)		{	m_hfrmDataView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmStampView;
	CfrmStampView*					m_hfrmStampView;
	CfrmStampView*					GetWindowHandler_StampView()						{	return m_hfrmStampView;	}
	void							SetWindowHandler_StampView(CfrmStampView* hWnd)		{	m_hfrmStampView = hWnd;	}
	//--------------------------------------------------------
	CRuntimeClass*					m_pfrmCompView;
	CfrmCompView*					m_hfrmCompView;
	CfrmCompView*					GetWindowHandler_CompView()							{	return m_hfrmCompView;	}
	void							SetWindowHandler_CompView(CfrmCompView* hWnd)		{	m_hfrmCompView = hWnd;	}
	//--------------------------------------------------------
	void GetReleaseDate(int *Year, int *Mon, int *Day);
	void SwitchMainViews(int viewNumber);
	void ModelUpdate();
	int GetCurrViewNo();
	int CurrViewNo;

	void AppExitProc();
	bool isView_Init();

	void Data_Reload();

	WCHAR chTime[50];
	WCHAR chDate[50];

private:
	CParameterFile*		Log();
	CParameterFile*		ParamFile();

public:
	int				m_nMainPage[8];

	// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

	// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};


