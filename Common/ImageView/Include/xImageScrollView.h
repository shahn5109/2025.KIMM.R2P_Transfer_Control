#if !defined(AFX_IMAGESCROLLVIEW_H__4D1E9E26_C0AF_41BA_BD51_02ABC757CC10__INCLUDED_)
#define AFX_IMAGESCROLLVIEW_H__4D1E9E26_C0AF_41BA_BD51_02ABC757CC10__INCLUDED_

#include"export.h"
#include"xGraphicObject.h"
#include"IxDeviceContext.h"

//#define BTN_EVENT_PAN_CLICK			0x100
#define BTN_EVENT_ZOOM_IN_CLICK			0x101
#define BTN_EVENT_ZOOM_OUT_CLICK		0x102
#define BTN_EVENT_ZOOM_INOUT_CLICK		0x103
#define BTN_EVENT_ZOOM_NOT_CLICK		0x104
#define BTN_EVENT_ZOOM_FIT_CLICK		0x105
//#define BTN_EVENT_SMART_CLICK			0x105
#define BTN_EVENT_LOAD_CLICK			0x106
#define BTN_EVENT_SAVE_CLICK			0x107
#define BTN_EVENT_MAXIMIZE_CLICK		0x110
#define BTN_EVENT_MEASURE_CLICK			0x111

#define ACTION_EVENT_PAN_START			0x200
#define ACTION_EVENT_PAN_END			0x201
#define ACTION_EVENT_ZOOM_IN			0x202
#define ACTION_EVENT_ZOOM_OUT			0x203
//#define ACTION_EVENT_MEASURE_START	0x204
//#define ACTION_EVENT_MEASURE_END		0x205
#define ACTION_EVENT_LOAD_FILE			0x206
#define ACTION_EVENT_SAVE_FILE			0x207
#define ACTION_EVENT_MAXIMIZE_SCRN		0x208
#define ACTION_EVENT_NORMAL_SCRN		0x209

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageScrollView.h : header file
//

#define TEXT_LOCALE_KOREA				0x600
#define TEXT_LOCALE_ENGLIGH				0x601

/////////////////////////////////////////////////////////////////////////////
// CxImageScrollView view
class CxImageScrollView;
typedef void (APIENTRY *FnOnDrawExt)( IxDeviceContext* pIDC, CDC* pDC, UINT nIndexData, LPVOID lpUsrData );
typedef void (APIENTRY *FnOnMeasure)( float fValueX, float fValueY, UINT nIndexData, LPVOID lpUsrData );
typedef void (APIENTRY *FnOnConfirmTracker)( CRect& rcTrackRegion, UINT nIndexData, LPVOID lpUsrData );

typedef BOOL (APIENTRY* FnOnFireMouseEvent) ( DWORD dwMsg, CPoint& point, UINT nIndexData, LPVOID lpUsrData );
typedef BOOL (APIENTRY* FnOnEvent) ( DWORD dwEvent, UINT nIndexData, LPVOID lpUsrData );

struct REGISTER_CALLBACK
{
	FnOnDrawExt				fnOnDrawExt;
	FnOnMeasure				fnOnMeasure;
	FnOnConfirmTracker		fnOnConfirmTracker;

	FnOnFireMouseEvent		fnOnFireMouseEvent;
	FnOnEvent				fnOnEvent;

	LPVOID					lpUsrData[5];
};

class CxDirectDIB;
class CxDrawDIB;
class CImageObj;
class CxRender2D;
class CInnerUI;
class XIMAGE_VIEW_API CxImageScrollView : public CScrollView, public IxDeviceContext
{
public:
	enum ScreenMode { ScreenModeNone, ScreenModePanning, ScreenModeZoomIn, ScreenModeZoomOut, ScreenModeZoomInOut, ScreenModeSmart, ScreenModeMeasure, ScreenModeTracker };
	enum MouseWheelMode { MouseWheelModeZoom, MouseWheelModeVerticalScroll };

public:
	CxImageScrollView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CxImageScrollView)

protected:

	CxDirectDIB*	m_pDirectDIB;
	CxDrawDIB*		m_pDrawDIB;
	CImageObj*	m_pImageObject;

	BOOL			m_bMouseOverCheck;

	CDC*			m_pDC;

	BYTE			m_cClearColor;
	BYTE*			m_pClearBuffer;

	double			m_dDrawElapsedTime;
	BOOL			m_bShowDrawElapsedTime;

	BOOL			m_bUseThreadDrawing;

	float			m_fZoomRatio;
	float			m_fZoomMin;
	float			m_fZoomMax;
	float			m_fZoomFit;

	int				m_nBodyOffsetX;
	int				m_nBodyOffsetY;

	BOOL			m_bLockUpdate;
	BOOL			m_bEnableSmartMove;
	BOOL			m_bEnableLockMouse;
	BOOL			m_bShowDigitize;
	BOOL			m_bShowScaleBar;

	MouseWheelMode  m_eMouseWheelMode;

	int m_nWidth;
	int m_nHeight;
	int m_nBitCnt;

	CxRender2D* m_pRenderer;

	HCURSOR		m_hCursorZoomIn;
	HCURSOR		m_hCursorZoomInOut;
	HCURSOR		m_hCursorZoomOut;
	HCURSOR		m_hCursorZoomNot;
	HCURSOR		m_hCursorPan;
	HCURSOR		m_hCursorNormal;
	HCURSOR		m_hCursorTracker;

	ScreenMode	m_eScreenMode;
	ScreenMode  m_eOldScreenMode;

	CPoint		m_ptViewLastMouse;

	BOOL		m_bMicro;

	int m_nWidthBytes;
	BOOL InitGraphics( CxRender2D* pRenderer, int nWidth, int nHeight, int nBitCnt );
	BOOL ExitGraphics();
	BOOL EndDraw();
	virtual void Clear();

	void DrawDigitizedValues( CDC* pDC, int nBlockSize, int nOffsetX, int nOffsetY, int nSrcX, int nSrcY, int nSrcW, int nSrcH );
	void DrawMeasure( CDC* pDC, CPoint& ptScroll, BOOL bMicro );
	void DrawScaleMark( HDC hDC, RECT rc, float fZoomRatio, LPCTSTR lpctszMeasures );
	BOOL DrawScaleMark_Simple0( HDC hDC, RECT rc, float fScale, LPCTSTR lpctszUnitForm, int nSplit );
	void DrawElapsedTime( HDC hDC );

	void WheelMenuUpdateUI();

	BOOL MyOnScrollBy( CSize sizeScroll, BOOL bDoScroll );

	float CalcZoomValid( float fZoom );

	void MousePan( CPoint& point );
	void MouseSmartPan( CPoint& point );

	FnOnDrawExt			m_fnOnDrawExt;
	LPVOID				m_lpUsrDataOnDrawExt;
	FnOnMeasure			m_fnOnMeasure;
	LPVOID				m_lpUsrDataOnMeasure;
	FnOnConfirmTracker	m_fnOnConfirmTracker;
	LPVOID				m_lpUsrDataOnConfirmTracker;
	FnOnFireMouseEvent	m_fnOnFireMouseEvent;
	LPVOID				m_lpUsrDataOnFireMouseEvent;
	FnOnEvent			m_fnOnEvent;
	LPVOID				m_lpUsrDataOnEvent;
	UINT				m_nIndexData;

	int m_nSrcX, m_nSrcY, m_nSrcW, m_nSrcH;

	float		m_fRealPixelSizeW, m_fRealPixelSizeH;

	CInnerUI*					m_pInnerUI;

	CxGraphicObject				m_GraphicObject;

	int							m_nCurrentTextLocale;

// Attributes
public:
	void SetPalette( const BYTE* pPal );
	//////////////////////////////////////////////////////////////////////////
	// Callback functions
	void SetOnDrawExt( FnOnDrawExt _fnOnDrawExt, LPVOID _lpUsrData );
	void SetOnMeasure( FnOnMeasure _fnOnMeasure, LPVOID _lpUsrData );
	void SetOnConfirmTracker( FnOnConfirmTracker _fnOnConfirmTracker, LPVOID _lpUsrData );
	void SetOnFireMouseEvent( FnOnFireMouseEvent _fnOnFireMouseEvent, LPVOID _lpUsrData );
	void SetOnEvent( FnOnEvent _fnOnEvent, LPVOID _lpUsrData );

	// Helper
	void SetRegisterCallBack( UINT nIndexData, REGISTER_CALLBACK* pRegisterCB );

	//////////////////////////////////////////////////////////////////////////
	// Pixel resolution
	float GetRealSizePerPixel() const;
	void SetRealSizePerPixel( float fRealPixelSize );

	float GetRealSizePerPixelW() const;
	void SetRealSizePerPixelW( float fRealPixelSizeW );
	float GetRealSizePerPixelH() const;
	void SetRealSizePerPixelH( float fRealPixelSizeH );

	void SetMeasureVisibleMicro( BOOL bMicro );

	//////////////////////////////////////////////////////////////////////////
	// Associate CImageObj
	BOOL SetImageObject( CImageObj* pImgObj );
	CImageObj* GetImageObject() { return m_pImageObject; }
	
	//////////////////////////////////////////////////////////////////////////
	// Screen mode
	ScreenMode SetScreenMode( ScreenMode _ScreenMode );
	ScreenMode GetScreenMode() const;
	void SetTracker( CRect& rcTrack );
	void SetTrackMode( BOOL bIsFixed );

	void SetMouseWheelMode( MouseWheelMode eMouseWheelMode );
	MouseWheelMode GetMouseWheelMode() const;

	//////////////////////////////////////////////////////////////////////////
	// Adjust View
	void ZoomFit( BOOL bCalcScrollBar = TRUE );
	void OnZoomFit( BOOL bCalcScrollBar = TRUE );
	void ZoomNot();
	void ZoomTo( CPoint ptScreen, float fZoom, BOOL bSyncControl=FALSE );
	void ImageZoomTo( CPoint ptImage, float fZoom, BOOL bSyncControl=FALSE );
	void MoveTo( CPoint ptImage, BOOL bSyncControl=FALSE );
	void SetZoomRatio( float fZoom, BOOL bInvalidate=TRUE );
	virtual float GetZoomRatio() { return m_fZoomRatio; }
	void SetImageUpdateLock() { m_bLockUpdate = TRUE; }
	void SetImageUpdateUnlock() { m_bLockUpdate = FALSE; }
	void RecalcZoomRatio( BOOL bCalcScrollBar = TRUE );

	//////////////////////////////////////////////////////////////////////////
	// coordinate conversion
	virtual POINT	MousePosToImagePos( int nMouseX, int nMouseY, BOOL* pbIsValidPos=NULL );
	virtual RECT	MousePosToImagePos( int nMouseLeft, int nMouseTop, int nMouseRight, int nMouseBottom );

	virtual DPOINT	ImagePosToScreenPos( double dX, double dY );
	virtual POINT	ImagePosToScreenPos( int x, int y);
	virtual RECT	ImagePosToScreenPos( int nLeft, int nTop, int nRight, int nBottom );
	virtual DRECT	ImagePosToScreenPos( double dLeft, double dTop, double dRight, double dBottom );

	virtual POINT	ScreenPosToOverlay( int x, int y );


	//////////////////////////////////////////////////////////////////////////
	// Retreieve information
	CxGraphicObject& GetGraphicObject() { return m_GraphicObject; }
	virtual int GetViewWidth() { return m_nWidth; }
	virtual int GetViewHeight() { return m_nHeight; }
	virtual void GetCurrentViewingCoordinate( int& nX1, int& nY1, int& nX2, int& nY2 );
	virtual void GetClientRect( LPRECT lpRect ) { CScrollView::GetClientRect(lpRect); }
	virtual POINT GetDeviceScrollPosition() const { return CScrollView::GetDeviceScrollPosition(); }

	void GetScrollBarDimension( int& nSWW, int &nSWH );

	//////////////////////////////////////////////////////////////////////////
	// Overridable
	virtual void OnDrawExt( IxDeviceContext* pIDC, CDC* pDC ) {}
	virtual void OnMeasure( float fValueX, float fValueY, BOOL bum = TRUE ) {}
	virtual void OnConfirmTracker( CRect& rcTrackRegion ) {}
	virtual BOOL OnFireMouseEvent( DWORD dwMsg, CPoint& point ) { return FALSE; }
	virtual BOOL OnEvent( DWORD dwEvent ) { return FALSE; }


	void DrawScreen();

	//void SetTextLocale( int nLocale );
	//void ShowScaleBar( BOOL bShow );
	void ShowScrollBar( BOOL bShow );
	BOOL IsShowScrollBar();
	void ShowDigitize( BOOL bShow );
	void ShowDrawElapsedTime( BOOL bShow );
	//void EnableThreadDrawing( BOOL bEnable );

	void SetClearColor( BYTE c );

	void UpdateRenderer( CImageObj* pImageObject );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxImageScrollView)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();     // first time after construct
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CxImageScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CxImageScrollView)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT _OnZoomFit( WPARAM wParam, LPARAM lParam );
	afx_msg void OnContextMenuHandler( UINT uID );
	DECLARE_MESSAGE_MAP()
};

inline void CxImageScrollView::GetCurrentViewingCoordinate( int& nX1, int& nY1, int& nX2, int& nY2 )
{
	nX1 = m_nSrcX;
	nY1 = m_nSrcY;
	nX2 = m_nSrcX+m_nSrcW;
	nY2 = m_nSrcY+m_nSrcH;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGESCROLLVIEW_H__4D1E9E26_C0AF_41BA_BD51_02ABC757CC10__INCLUDED_)
