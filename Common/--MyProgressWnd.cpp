#include "stdafx.h"
#include "MyProgressWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
#define IDC_TEXT     11
#define IDC_PROGRESS 12
#define IDC_CANCEL 13

/////////////////////////////////////////////////////////////////////////////
// CMyProgressWnd

CMyProgressWnd::CMyProgressWnd(BOOL bPos /*=TRUE*/)
{
	m_hbrush = NULL;
	m_MostTopFlag = FALSE;
	m_bCenter = bPos;
    CommonConstruct();
}

CMyProgressWnd::CMyProgressWnd(CWnd* pParent, LPCTSTR pszTitle, BOOL bPos/*=TRUE*/, BOOL bSmooth /* = TRUE */)
{
	m_hbrush = NULL;
	m_MostTopFlag = FALSE;
	m_bCenter = bPos;
	CommonConstruct();
	m_strTitle = pszTitle;

	Create(pParent, pszTitle, bSmooth);
}

void CMyProgressWnd::CommonConstruct()
{
	m_nNumTextLines = 4;
	m_nPrevPos      = 0;
	m_nPrevPercent  = 0;
	m_nStep         = 1;
	m_nMinValue     = 0;
	m_nMaxValue     = 100;

	m_strTitle      = _T("Progress");
}

CMyProgressWnd::~CMyProgressWnd()
{
	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	
	if( m_hbrush ) ::DeleteObject(m_hbrush);
	m_font.DeleteObject();

    DestroyWindow();
}

BOOL CMyProgressWnd::Create(CWnd* pParent, LPCTSTR pszTitle, BOOL bSmooth /* = TRUE */)
{
	BOOL bSuccess;

	// Register window class
	CString csClassName = AfxRegisterWndClass(CS_OWNDC|CS_HREDRAW|CS_VREDRAW,
	//                                        ::LoadCursor(NULL, IDC_APPSTARTING),
														   ::LoadCursor(NULL, IDC_WAIT),
														   CBrush(::GetSysColor(COLOR_BTNFACE)));

	// Get the system window message font for use in the cancel button and text area
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
	m_font.CreateFontIndirect(&(ncm.lfMessageFont)); 

	// If no parent supplied then try and get a pointer to it anyway
	if(!pParent) pParent = AfxGetMainWnd();

	// Create popup window
	bSuccess = CreateEx(WS_EX_DLGMODALFRAME,            // Extended style
							  csClassName,                    // Classname
							  pszTitle,                       // Title
							  WS_POPUP|WS_BORDER|WS_CAPTION,  // style
							  0,0,                            // position - updated soon.
							  300,130,                        // Size - updated soon
							  pParent->GetSafeHwnd(),         // handle to parent
							  0,                              // No menu
							  NULL);    
	if (!bSuccess) return FALSE;

	// Now create the controls
	CRect TempRect(0,0,10,10);

	bSuccess = m_Text.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOPREFIX|SS_LEFTNOWORDWRAP, TempRect, this, IDC_TEXT);
	if(!bSuccess) return FALSE;

	DWORD dwProgressStyle = WS_CHILD|WS_VISIBLE;
#ifdef PBS_SMOOTH    
	if(bSmooth) dwProgressStyle |= PBS_SMOOTH;
#endif
	bSuccess = m_wndProgress.Create(dwProgressStyle, TempRect, this, IDC_PROGRESS);
	if(!bSuccess) return FALSE;

/* 
	bSuccess = m_CancelButton.Create(_T("Cancel"), WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON, 
												TempRect, this, IDC_CANCEL);
	if (!bSuccess) return FALSE;

	m_CancelButton.SetFont(&m_font, TRUE);
*/

	m_Text.SetFont(&m_font, TRUE);

	// Resize the whole thing according to the number of text lines, desired window
	// width and current font.
	SetWindowSize(m_nNumTextLines, 300);

	if( m_MostTopFlag )
		SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	// Center and show window
	int sys_width = ::GetSystemMetrics(SM_CXFULLSCREEN);
	int sys_height = ::GetSystemMetrics(SM_CYFULLSCREEN);
	
	if( m_bCenter ) CenterWindow();
	else 
	{
		GetWindowRect(&TempRect);
		int x = (sys_width - (TempRect.right - TempRect.left)) / 2;
		int width = TempRect.right - TempRect.left;
		int y = TempRect.bottom - TempRect.top;

		TempRect.left = x;
		TempRect.right = x + width;
		TempRect.bottom = sys_height - 20;
		TempRect.top = TempRect.bottom  - y;

		MoveWindow(TempRect);
	}

	CRect rect;
	GetWindowRect(&rect);
	int disp_OffsetY= (1080 / 2) - (rect.Height() / 2) - 20;
	int disp_OffsetX= (1920 / 2) - (rect.Width() / 2) - 30;
	
	//	ProgWnd.SetWindowPos(ProgWnd.GetSafeHwnd(), 600, 350, rect.Width(), rect.Height(),SWP_NOMOVE|SWP_NOSIZE);
	MoveWindow(CRect(disp_OffsetX, disp_OffsetY, disp_OffsetX+rect.Width(), disp_OffsetY+rect.Height())); // H=239, W=384

	Show();

	if( m_MostTopFlag )
		SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;
}

void CMyProgressWnd::SetWindowSize(int nNumTextLines, int nWindowWidth /*=390*/)
{
	int nMargin = 10;
	CSize EdgeSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

	CRect TextRect, ProgressRect;
	CSize CancelSize, TextSize;
	CRect CancelRect;

	// Set up a default size for the text area in case things go wrong
	TextRect.SetRect(nMargin,nMargin, nWindowWidth-2*nMargin, 100+2*nMargin);

	// Get DrawText to tell us how tall the text area will be (while we're at
	// it, we'll see how big the word "Cancel" is)
	CDC* pDC = GetDC();
	if(pDC) 
	{
        CFont* pOldFont = pDC->SelectObject(&m_font);
        CString str = _T("M");
        for (int i = 0; i < nNumTextLines-1; i++) str += _T("\nM");
        pDC->DrawText(str, TextRect, DT_CALCRECT|DT_NOCLIP|DT_NOPREFIX);
        TextRect.right = TextRect.left + nWindowWidth;
        CancelSize = pDC->GetTextExtent(_T(" Cancel "))+CSize(EdgeSize.cx*4, EdgeSize.cy*3);
        TextSize = pDC->GetTextExtent(_T("M "))+CSize(EdgeSize.cx*4, EdgeSize.cy*3);
        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);
    }
    
    // Work out how big (and where) the cancel button should be
    CancelRect.SetRect(TextRect.right-CancelSize.cx, TextRect.bottom+nMargin, 
                       TextRect.right, TextRect.bottom+nMargin + CancelSize.cy);


    // Work out how big (and where) the progress control should be
//    ProgressRect.SetRect(TextRect.left, CancelRect.top + EdgeSize.cy - 7, 
//                         CancelRect.left-nMargin, CancelRect.bottom - EdgeSize.cy);
    ProgressRect.SetRect(TextRect.left, CancelRect.top + EdgeSize.cy - 7, 
                         TextRect.right, CancelRect.bottom - EdgeSize.cy);


    // Resize the main window to fit the controls
    CSize ClientSize(nMargin + TextRect.Width() + nMargin,
                     nMargin + TextRect.Height() + nMargin + CancelRect.Height() + nMargin);

    CRect WndRect, ClientRect;
    GetWindowRect(WndRect); GetClientRect(ClientRect);
    WndRect.right = WndRect.left + WndRect.Width()-ClientRect.Width()+ClientSize.cx;
    WndRect.bottom = WndRect.top + WndRect.Height()-ClientRect.Height()+ClientSize.cy;
    MoveWindow(WndRect);

    // Now reposition the controls...
    m_wndProgress.MoveWindow(ProgressRect);
//    m_CancelButton.MoveWindow(CancelRect);
    m_Text.MoveWindow(TextRect);
}

void CMyProgressWnd::Clear() 
{ 
    SetText(_T(""));
    SetPos(0);
    m_nPrevPos = 0;

    if (::IsWindow(GetSafeHwnd()))
        UpdateWindow();
}

void CMyProgressWnd::Hide()  
{ 
    if (!::IsWindow(GetSafeHwnd())) 
        return;

    Clear();
    if (::GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_VISIBLE)
    {
        ShowWindow(SW_HIDE);
        ModifyStyle(WS_VISIBLE, 0);
    }
}

void CMyProgressWnd::Show()  
{ 
    if (!::IsWindow(GetSafeHwnd()))
        return;

    if ((::GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_VISIBLE) == 0)
    {
        ModifyStyle(0, WS_VISIBLE);
        ShowWindow(SW_SHOWNA);
        RedrawWindow(NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE);
    }
}

void CMyProgressWnd::SetRange(int nLower, int nUpper, int nStep /* = 1 */)    
{
    if (!::IsWindow(GetSafeHwnd())) 
        return;

    // To take advantage of the Extended Range Values we use the PBM_SETRANGE32
    // message intead of calling CProgressCtrl::SetRange directly. If this is
    // being compiled under something less than VC 5.0, the necessary defines
    // may not be available.
#ifdef PBM_SETRANGE32
    ASSERT(-0x7FFFFFFF <= nLower && nLower <= 0x7FFFFFFF);
    ASSERT(-0x7FFFFFFF <= nUpper && nUpper <= 0x7FFFFFFF);
    m_wndProgress.SendMessage(PBM_SETRANGE32, (WPARAM) nLower, (LPARAM) nUpper);
#else
    ASSERT(0 <= nLower && nLower <= 65535);
    ASSERT(0 <= nUpper && nUpper <= 65535);
    m_wndProgress.SetRange(nLower, nUpper);
#endif

    m_nMaxValue = nUpper;
    m_nMinValue = nLower;
    m_nStep     = nStep;

    m_wndProgress.SetStep(nStep);
}

int CMyProgressWnd::OffsetPos(int nPos)
{ 
    if (!::IsWindow(GetSafeHwnd())) 
        return m_nPrevPos;

    return SetPos(m_nPrevPos + nPos);  
}

int CMyProgressWnd::StepIt()                
{
    if (!::IsWindow(GetSafeHwnd())) 
        return m_nPrevPos;

    return SetPos(m_nPrevPos + m_nStep); 
}

int CMyProgressWnd::SetStep(int nStep)
{
    int nOldStep = m_nStep;
    m_nStep = nStep;
    if (!::IsWindow(GetSafeHwnd())) 
        return nOldStep;

    return m_wndProgress.SetStep(nStep); 
}

int CMyProgressWnd::SetPos(int nPos)                    
{
#ifdef PBM_SETRANGE32
    ASSERT(-0x7FFFFFFF <= nPos && nPos <= 0x7FFFFFFF);
#else
    ASSERT(0 <= nPos && nPos =< 65535);
#endif

    if (!::IsWindow(GetSafeHwnd())) 
        return m_nPrevPos;

    CString strTitle;
    int nPercentage;
    
    m_nPrevPos = nPos;

    if (m_nMaxValue > m_nMinValue)
        nPercentage = (nPos*100)/(m_nMaxValue - m_nMinValue);
    else
        nPercentage = 0;

    if (nPercentage != m_nPrevPercent) 
    {
        m_nPrevPercent = nPercentage;
        strTitle.Format(_T("%s [%d%%]"),m_strTitle,nPercentage);
        SetWindowText(strTitle);
    }
    return m_wndProgress.SetPos(nPos);        
}

void CMyProgressWnd::SetText(LPCTSTR fmt, ...)
	{

	if (!::IsWindow(GetSafeHwnd())) 
      return;

    va_list args;
    TCHAR buffer[512];

    va_start(args, fmt);
    _vstprintf(buffer, fmt, args);
    va_end(args);

    m_Text.SetWindowText(buffer);
}

BEGIN_MESSAGE_MAP(CMyProgressWnd, CWnd)
    //{{AFX_MSG_MAP(CMyProgressWnd)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyProgressWnd message handlers
/*BOOL CMyProgressWnd::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message == WM_KEYDOWN)
	{ 
		if((pMsg->wParam == VK_ESCAPE)){ 
			return TRUE; 
		}  
	}
//			return TRUE; 
	return CWnd::PreTranslateMessage(pMsg);
}
*/
BOOL CMyProgressWnd::OnEraseBkgnd(CDC* pDC) 
{
    // Fill background with Catchment background colour
    CBrush backBrush(GetSysColor(COLOR_BTNFACE));
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);
	backBrush.DeleteObject();

    return TRUE;
}

// Message pumping function that can either be used to pump messages during
// long operations. This version will only pass messages to this window (and
// all child windows). (Thanks to Michael <mbh-ep@post5.tele.dk> for this)
void CMyProgressWnd::PeekAndPump()
{
//	SetFocus();

    MSG msg;
    while(::PeekMessage(&msg, NULL,0,0,PM_NOREMOVE)) 
    {
		Sleep(10);
		if(!AfxGetApp()->PumpMessage()) 
        {
			::PostQuitMessage(0);
			return;
		} 
    }
}
