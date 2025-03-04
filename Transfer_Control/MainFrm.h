// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://msdn.microsoft.com/officeui�� �����Ͻʽÿ�.
//
// Copyright (C) Microsoft Corporation
// ��� �Ǹ� ����.

// MainFrm.h : CMainFrame Ŭ������ �������̽�
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

protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// Ư���Դϴ�.
public:

	// �۾��Դϴ�.
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

	// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

	// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};


