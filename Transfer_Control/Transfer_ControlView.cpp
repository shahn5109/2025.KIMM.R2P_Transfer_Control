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

// Transfer_ControlView.cpp : CTransfer_ControlView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Transfer_Control.h"
#endif

#include "Transfer_ControlDoc.h"
#include "Transfer_ControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransfer_ControlView

IMPLEMENT_DYNCREATE(CTransfer_ControlView, CFormView)

BEGIN_MESSAGE_MAP(CTransfer_ControlView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTransfer_ControlView ����/�Ҹ�

CTransfer_ControlView::CTransfer_ControlView()
	: CFormView(CTransfer_ControlView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTransfer_ControlView::~CTransfer_ControlView()
{
}

void CTransfer_ControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CTransfer_ControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CTransfer_ControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CTransfer_ControlView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTransfer_ControlView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTransfer_ControlView ����

#ifdef _DEBUG
void CTransfer_ControlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTransfer_ControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTransfer_ControlDoc* CTransfer_ControlView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransfer_ControlDoc)));
	return (CTransfer_ControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransfer_ControlView �޽��� ó����
