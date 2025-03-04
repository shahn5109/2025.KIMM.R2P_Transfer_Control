// OScopeCtrl.cpp : implementation file//

#include "stdafx.h"
#include "math.h"

#include "OScopeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl
COScopeCtrl::COScopeCtrl()
{
  // since plotting is based on a LineTo for each new point
  // we need a starting point (i.e. a "previous" point)
  // use 0.0 as the default first point.
  // these are public member variables, and can be changed outside
  // (after construction).  Therefore m_perviousPosition could be set to
  // a more appropriate value prior to the first call to SetPosition.
  m_dPreviousPosition =   0.0 ;
  m_dCurrentPosition = 0.0;

  // public variable for the number of decimal places on the y axis
  m_nYDecimals = 3 ;

  // set some initial values for the scaling until "SetRange" is called.
  // these are protected varaibles and must be set with SetRange
  // in order to ensure that m_dRange is updated accordingly
  m_dLowerLimit = -10.0 ;
  m_dUpperLimit =  10.0 ;
  m_dLowerLimitRange     = -5;
  m_dUpperLimitRange     = 5 ;
  m_dRange      =  m_dUpperLimit - m_dLowerLimit ;   // protected member variable

  // m_nShiftPixels determines how much the plot shifts (in terms of pixels) 
  // with the addition of a new data point
  m_nShiftPixels     = 7 ;//4
  m_nHalfShiftPixels = m_nShiftPixels/2 ;                     // protected
  m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels ;  // protected

  // background, grid and data colors
  // these are public variables and can be set directly
  m_crBackColor  = RGB(  0,   0,   0) ;  // see also SetBackgroundColor
  m_crGridColor  = RGB(  0, 255, 255) ;  // see also SetGridColor
  m_crPlotColor  = RGB(255, 255, 255) ;  // see also SetPlotColor
  m_crLimiteColor = RGB(255, 0, 0);

  // protected variables
  m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

  // public member variables, can be set directly 
  m_strXUnitsString.Format("Samples") ;  // can also be set with SetXUnits
  m_strYUnitsString.Format("Y units") ;  // can also be set with SetYUnits

  // protected bitmaps to restore the memory DC's
  m_pbitmapOldGrid = NULL ;
  m_pbitmapOldPlot = NULL ;


	axisDrawFont.CreateFont (14, 0, 0, 0, 300,
						FALSE, FALSE, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, 
						DEFAULT_PITCH|FF_SWISS, "Arial") ;
}  // COScopeCtrl

/////////////////////////////////////////////////////////////////////////////
COScopeCtrl::~COScopeCtrl()
{
  // just to be picky restore the bitmaps for the two memory dc's
  // (these dc's are being destroyed so there shouldn't be any leaks)
  if (m_pbitmapOldGrid != NULL)
    m_dcGrid.SelectObject(m_pbitmapOldGrid) ;  
  if (m_pbitmapOldPlot != NULL)
    m_dcPlot.SelectObject(m_pbitmapOldPlot) ;  

} // ~COScopeCtrl


BEGIN_MESSAGE_MAP(COScopeCtrl, CWnd)
  //{{AFX_MSG_MAP(COScopeCtrl)
  ON_WM_PAINT()
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
BOOL COScopeCtrl::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, UINT nID) 
{
  BOOL result ;
  static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;

  result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
                          className, NULL, dwStyle, 
                          rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
                          pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
  if (result != 0)
    InvalidateCtrl() ;

  return result ;

} // Create

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetRange(double dLower, double dUpper, double dDnLim, double dUpLim, int nDecimalPlaces)
{
  ASSERT(dUpper > dLower) ;

  m_dLowerLimit     = dLower ;
  m_dUpperLimit     = dUpper ;
  m_dLowerLimitRange     = dDnLim ;
  m_dUpperLimitRange     = dUpLim ;
  m_nYDecimals      = nDecimalPlaces ;
  m_dRange          = m_dUpperLimit - m_dLowerLimit ;
  m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 
  
  // clear out the existing garbage, re-start with a clean plot
  InvalidateCtrl() ;

}  // SetRange


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetXUnits(CString string)
{
  m_strXUnitsString = string ;

  // clear out the existing garbage, re-start with a clean plot
  InvalidateCtrl() ;

}  // SetXUnits

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetYUnits(CString string)
{
  m_strYUnitsString = string ;

  // clear out the existing garbage, re-start with a clean plot
  InvalidateCtrl() ;

}  // SetYUnits

 void COScopeCtrl::InputXUnits(double dLower, double dUpper, double dDnLim, double dUpLim, int nDecimalPlaces, CString string)
 {
	m_strXUnitsString = string ;

	// clear out the existing garbage, re-start with a clean plot
	InvalidateCtrl3(dLower, dUpper, dDnLim, dUpLim, nDecimalPlaces) ;
 }

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetGridColor(COLORREF color1, COLORREF color2)
{
  m_crGridColor = color1 ;
  m_crLimiteColor = color2 ;

  // clear out the existing garbage, re-start with a clean plot
  InvalidateCtrl() ;

}  // SetGridColor


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetPlotColor(COLORREF color)
{
  m_crPlotColor = color ;

  m_penPlot.DeleteObject() ;
  m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;

  // clear out the existing garbage, re-start with a clean plot
  InvalidateCtrl() ;

}  // SetPlotColor


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::SetBackgroundColor(COLORREF color)
{
  m_crBackColor = color ;

  m_brushBack.DeleteObject() ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

  // clear out the existing garbage, re-start with a clean plot
  InvalidateCtrl() ;

}  // SetBackgroundColor

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::InvalidateCtrl()
{
  // There is a lot of drawing going on here - particularly in terms of 
  // drawing the grid.  Don't panic, this is all being drawn (only once)
  // to a bitmap.  The result is then BitBlt'd to the control whenever needed.
  int i ;
  int nCharacters ;
  int nTopGridPix, nMidGridPix, nBottomGridPix ;

  CPen *oldPen ;
  CPen solidPen(PS_SOLID, 0, m_crGridColor) ;
  CFont axisFont, yUnitFont, *oldFont ;
  CString strTemp ;

  // in case we haven't established the memory dc's
  CClientDC dc(this) ;  

  // if we don't have one yet, set up a memory dc for the grid
  if (m_dcGrid.GetSafeHdc() == NULL)
  {
    m_dcGrid.CreateCompatibleDC(&dc) ;
    m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
    m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid) ;
  }
  
  m_dcGrid.SetBkColor (m_crBackColor) ;

  // fill the grid background
  m_dcGrid.FillRect(m_rectClient, &m_brushBack) ;

  // draw the plot rectangle:
  // determine how wide the y axis scaling values are
  nCharacters = abs((int)log10(fabs(m_dUpperLimit))) ;
  nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit)))) ;

  // add the units digit, decimal point and a minus sign, and an extra space
  // as well as the number of decimal places to display
  nCharacters = nCharacters + 4 + m_nYDecimals ;  

  // adjust the plot rectangle dimensions
  // assume 6 pixels per character (this may need to be adjusted)
  m_rectPlot.left = m_rectClient.left + 5*(nCharacters) ;
  m_nPlotWidth    = m_rectPlot.Width() ;

  // draw the plot rectangle
  oldPen = m_dcGrid.SelectObject (&solidPen) ; 
  m_dcGrid.MoveTo (m_rectPlot.left, m_rectPlot.top) ;
  m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top) ;
  m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1) ;
  m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.bottom+1) ;
  m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.top) ;
  m_dcGrid.SelectObject (oldPen) ; 

  // draw the dotted lines, 
  // use SetPixel instead of a dotted pen - this allows for a 
  // finer dotted line and a more "technical" look
  double up = fabs((m_dUpperLimit - m_dUpperLimitRange) * 1000.0) / 1000.0;
  double down = fabs((m_dLowerLimit - m_dLowerLimitRange) * 1000.0) / 1000.0;
  nMidGridPix    = 0;
  nTopGridPix    = m_rectPlot.bottom - (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / down));
  nBottomGridPix = m_rectPlot.top + (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / up)); 

  for (i=m_rectPlot.left; i<m_rectPlot.right; i+=4)
  {
    m_dcGrid.SetPixel (i, nTopGridPix,    m_crLimiteColor) ;
    m_dcGrid.SetPixel (i, nBottomGridPix, m_crLimiteColor) ;
  }

  // create some fonts (horizontal and vertical)
  // use a height of 14 pixels and 300 weight 
  // (these may need to be adjusted depending on the display)
  axisFont.CreateFont (14, 0, 0, 0, 300,
                       FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  yUnitFont.CreateFont (14, 0, 0, 0, 300,
                       FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  // grab the horizontal font
  oldFont = m_dcGrid.SelectObject(&axisFont) ;
  
  // y max
  m_dcGrid.SetTextColor (m_crGridColor) ;
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimit) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.top - 5, strTemp) ;

  // y min
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimit) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.bottom + 5, strTemp) ;

  // y max limit
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimitRange) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, nBottomGridPix - 5, strTemp) ;

  // y min limit
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimitRange) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, nTopGridPix - 6, strTemp) ;

  // x min
  m_dcGrid.SetTextAlign (TA_LEFT|TA_TOP) ;
  m_dcGrid.TextOut (m_rectPlot.right -2 , m_rectPlot.bottom+5, "0") ;

  // x max
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%d", m_nPlotWidth/m_nShiftPixels) ; 
  m_dcGrid.TextOut (m_rectPlot.left + 10, m_rectPlot.bottom+5, strTemp) ;

  // x units
  m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP) ;
  m_dcGrid.TextOut ((m_rectPlot.left+m_rectPlot.right)/2 - 10, 
                    m_rectPlot.bottom+20, m_strXUnitsString) ;

  // restore the font
  m_dcGrid.SelectObject(oldFont) ;

  // y units
  oldFont = m_dcGrid.SelectObject(&yUnitFont) ;
  m_dcGrid.SetTextAlign (TA_CENTER | TA_BASELINE) ;
  m_dcGrid.TextOut ((m_rectClient.left+m_rectClient.right)/2, 
                    m_rectPlot.top-8, m_strYUnitsString) ;
  m_dcGrid.SelectObject(oldFont) ;

  // at this point we are done filling the the grid bitmap, 
  // no more drawing to this bitmap is needed until the setting are changed
  
  // if we don't have one yet, set up a memory dc for the plot
  if (m_dcPlot.GetSafeHdc() == NULL)
  {
    m_dcPlot.CreateCompatibleDC(&dc) ;
    m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
    m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot) ;
  }

  // make sure the plot bitmap is cleared
  m_dcPlot.SetBkColor (m_crBackColor) ;
  m_dcPlot.FillRect(m_rectClient, &m_brushBack) ;


  oldPen->DeleteObject();
  oldFont->DeleteObject();

  // finally, force the plot area to redraw
  InvalidateRect(m_rectClient) ;

} // InvalidateCtrl

void COScopeCtrl::InvalidateCtrl2(double dLower, double dUpper, double dDnLim, double dUpLim, int nDecimalPlaces)
{
  // There is a lot of drawing going on here - particularly in terms of 
  // drawing the grid.  Don't panic, this is all being drawn (only once)
  // to a bitmap.  The result is then BitBlt'd to the control whenever needed.
  int i ;
  int nCharacters ;
  int nTopGridPix, nMidGridPix, nBottomGridPix ;

  CPen *oldPen ;
  CPen solidPen(PS_SOLID, 0, m_crGridColor) ;
  CFont axisFont, *oldFont ;
  CString strTemp ;

  // in case we haven't established the memory dc's
  CClientDC dc(this) ;  

  // if we don't have one yet, set up a memory dc for the grid
  if (m_dcGrid.GetSafeHdc() == NULL)
  {
    m_dcGrid.CreateCompatibleDC(&dc) ;
    m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
    m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid) ;
  }
  
  m_dcGrid.SetBkColor (m_crBackColor) ;

  // fill the grid background
//  m_dcGrid.FillRect(m_rectClient, &m_brushBack) ;

  // draw the plot rectangle:
  // determine how wide the y axis scaling values are
  nCharacters = abs((int)log10(fabs(m_dUpperLimit))) ;
  nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit)))) ;

  // add the units digit, decimal point and a minus sign, and an extra space
  // as well as the number of decimal places to display
  nCharacters = nCharacters + 4 + m_nYDecimals ;  

  // adjust the plot rectangle dimensions
  // assume 6 pixels per character (this may need to be adjusted)
  m_rectPlot.left = m_rectClient.left + 5*(nCharacters) ;
  m_nPlotWidth    = m_rectPlot.Width() ;
 
  // draw the dotted lines, 
  // use SetPixel instead of a dotted pen - this allows for a 
  // finer dotted line and a more "technical" look
  double up = fabs((m_dUpperLimit - m_dUpperLimitRange) * 1000.0) / 1000.0;
  double down = fabs((m_dLowerLimit - m_dLowerLimitRange) * 1000.0) / 1000.0;
  nMidGridPix    = 0;
  nTopGridPix    = m_rectPlot.bottom - (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / down));
  nBottomGridPix = m_rectPlot.top + (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / up)); 

  for (i=m_rectPlot.left; i<m_rectPlot.right; i+=4)
  {
    m_dcGrid.SetPixel (i, nTopGridPix,    RGB(0,0,0)) ;
    m_dcGrid.SetPixel (i, nBottomGridPix, RGB(0,0,0)) ;
  }

  // create some fonts (horizontal and vertical)
  // use a height of 14 pixels and 300 weight 
  // (these may need to be adjusted depending on the display)
  axisFont.CreateFont (14, 0, 0, 0, 300,
                       FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  // grab the horizontal font
  oldFont = m_dcGrid.SelectObject(&axisFont) ;

  m_dcGrid.SetTextColor (RGB(0, 0, 0)) ;


  // y max limit
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimitRange) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, nBottomGridPix - 5, strTemp) ;

  // y min limit
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimitRange) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, nTopGridPix - 6, strTemp) ;

  m_dcGrid.SetTextColor (m_crGridColor) ;














  ASSERT(dUpper > dLower) ;

  m_dLowerLimit     = dLower ;
  m_dUpperLimit     = dUpper ;
  m_dLowerLimitRange     = dDnLim ;
  m_dUpperLimitRange     = dUpLim ;
  m_nYDecimals      = nDecimalPlaces ;
  m_dRange          = m_dUpperLimit - m_dLowerLimit ;
  m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 




  // draw the plot rectangle:
  // determine how wide the y axis scaling values are
  nCharacters = abs((int)log10(fabs(m_dUpperLimit))) ;
  nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit)))) ;

  // add the units digit, decimal point and a minus sign, and an extra space
  // as well as the number of decimal places to display
  nCharacters = nCharacters + 4 + m_nYDecimals ;  

  // adjust the plot rectangle dimensions
  // assume 6 pixels per character (this may need to be adjusted)
  m_rectPlot.left = m_rectClient.left + 5*(nCharacters) ;
  m_nPlotWidth    = m_rectPlot.Width() ;
 
  // draw the dotted lines, 
  // use SetPixel instead of a dotted pen - this allows for a 
  // finer dotted line and a more "technical" look
  up = fabs((m_dUpperLimit - m_dUpperLimitRange) * 1000.0) / 1000.0;
  down = fabs((m_dLowerLimit - m_dLowerLimitRange) * 1000.0) / 1000.0;
  nMidGridPix    = 0;
  nTopGridPix    = m_rectPlot.bottom - (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / down));
  nBottomGridPix = m_rectPlot.top + (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / up)); 

  for (i=m_rectPlot.left; i<m_rectPlot.right; i+=4)
  {
    m_dcGrid.SetPixel (i, nTopGridPix,    m_crLimiteColor) ;
    m_dcGrid.SetPixel (i, nBottomGridPix, m_crLimiteColor) ;
  }

  // draw the plot rectangle
  oldPen = m_dcGrid.SelectObject (&solidPen) ; 
  m_dcGrid.MoveTo (m_rectPlot.left, m_rectPlot.top) ;
  m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top) ;
  m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1) ;
  m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.bottom+1) ;
  m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.top) ;
  m_dcGrid.SelectObject (oldPen) ; 

  // y max
//  m_dcGrid.SetTextColor (m_crGridColor) ;
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimit) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.top - 5, strTemp) ;

  // y min
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimit) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, m_rectPlot.bottom + 5, strTemp) ;


  // y max limit
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dUpperLimitRange) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, nBottomGridPix - 5, strTemp) ;

  // y min limit
  m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
  strTemp.Format ("%.*lf", m_nYDecimals, m_dLowerLimitRange) ;
  m_dcGrid.TextOut (m_rectPlot.left-4, nTopGridPix - 6, strTemp) ;



  // restore the font
  m_dcGrid.SelectObject(oldFont) ;

  oldPen->DeleteObject();
  oldFont->DeleteObject();

  // finally, force the plot area to redraw
  InvalidateRect(m_rectClient) ;

} // InvalidateCtrl

void COScopeCtrl::InvalidateCtrl3(double dLower, double dUpper, double dDnLim, double dUpLim, int nDecimalPlaces)
{
  // There is a lot of drawing going on here - particularly in terms of 
  // drawing the grid.  Don't panic, this is all being drawn (only once)
  // to a bitmap.  The result is then BitBlt'd to the control whenever needed.
  int i ;
  int nCharacters ;
  int nTopGridPix, nMidGridPix, nBottomGridPix ;

//  CPen *oldPen ;
  CPen solidPen(PS_SOLID, 0, m_crGridColor) ;
  CFont axisFont, yUnitFont, *oldFont ;
  CString strTemp ;

  // in case we haven't established the memory dc's
  CClientDC dc(this) ;  


  ASSERT(dUpper > dLower) ;

  m_dLowerLimit     = dLower ;
  m_dUpperLimit     = dUpper ;
  m_dLowerLimitRange     = dDnLim ;
  m_dUpperLimitRange     = dUpLim ;
  m_nYDecimals      = nDecimalPlaces ;
  m_dRange          = m_dUpperLimit - m_dLowerLimit ;
  m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 

  // draw the plot rectangle:
  // determine how wide the y axis scaling values are
  nCharacters = abs((int)log10(fabs(m_dUpperLimit))) ;
  nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit)))) ;

  // add the units digit, decimal point and a minus sign, and an extra space
  // as well as the number of decimal places to display
  nCharacters = nCharacters + 4 + m_nYDecimals ;  

  // adjust the plot rectangle dimensions
  // assume 6 pixels per character (this may need to be adjusted)
  m_rectPlot.left = m_rectClient.left + 5*(nCharacters) ;
  m_nPlotWidth    = m_rectPlot.Width() ;

  // draw the dotted lines, 
  // use SetPixel instead of a dotted pen - this allows for a 
  // finer dotted line and a more "technical" look
  double up = fabs((m_dUpperLimit - m_dUpperLimitRange) * 1000.0) / 1000.0;
  double down = fabs((m_dLowerLimit - m_dLowerLimitRange) * 1000.0) / 1000.0;
  nMidGridPix    = 0;
  nTopGridPix    = m_rectPlot.bottom - (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / down));
  nBottomGridPix = m_rectPlot.top + (long)(m_rectPlot.Height() / ((m_dUpperLimit - m_dLowerLimit) / up)); 

  for (i=m_rectPlot.left; i<m_rectPlot.right; i+=4)
  {
    m_dcGrid.SetPixel (i, nTopGridPix,    m_crLimiteColor) ;
    m_dcGrid.SetPixel (i, nBottomGridPix, m_crLimiteColor) ;
  }

  // create some fonts (horizontal and vertical)
  // use a height of 14 pixels and 300 weight 
  // (these may need to be adjusted depending on the display)
  axisFont.CreateFont (14, 0, 0, 0, 300,
                       FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  yUnitFont.CreateFont (14, 0, 0, 0, 300,
                       FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  // grab the horizontal font
  oldFont = m_dcGrid.SelectObject(&axisFont) ;

  // x units
  m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP) ;
  m_dcGrid.TextOut ((m_rectPlot.left+m_rectPlot.right)/2 - 10, 
                    m_rectPlot.bottom+20, m_strXUnitsString) ;

  // restore the font
  m_dcGrid.SelectObject(oldFont) ;

  oldFont->DeleteObject();

  // finally, force the plot area to redraw
  InvalidateRect(m_rectClient) ;
} // InvalidateCtrl

/////////////////////////////////////////////////////////////////////////////
double COScopeCtrl::AppendPoint(double dNewPoint, CString data)
{
  double dPrevious ;
  
  dPrevious = m_dCurrentPosition ;
  m_dCurrentPosition = dNewPoint ;
  DrawPoint(data) ;

  Invalidate() ;

  return dPrevious ;

} // AppendPoint
 
////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::OnPaint() 
{
  CPaintDC dc(this) ;  // device context for painting
  CDC memDC ;
  CBitmap memBitmap ;
  CBitmap* oldBitmap ; // bitmap originally found in CMemDC

  // no real plotting work is performed here, 
  // just putting the existing bitmaps on the client

  // to avoid flicker, establish a memory dc, draw to it 
  // and then BitBlt it to the client
  memDC.CreateCompatibleDC(&dc) ;
  memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
  oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

  if (memDC.GetSafeHdc() != NULL)
  {
    // first drop the grid on the memory dc
    memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
                 &m_dcGrid, 0, 0, SRCCOPY) ;
    // now add the plot on top as a "pattern" via SRCPAINT.
    // works well with dark background and a light plot
    memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
                 &m_dcPlot, 0, 0, SRCPAINT) ;  //SRCPAINT
    // finally send the result to the display
    dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
              &memDC, 0, 0, SRCCOPY) ;
  }

  memDC.SelectObject(oldBitmap) ;

} // OnPaint

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::DrawPoint(CString data)
{
  // this does the work of "scrolling" the plot to the left
  // and appending a new data point all of the plotting is 
  // directed to the memory based bitmap associated with m_dcPlot
  // the will subsequently be BitBlt'd to the client in OnPaint
  
  int currX, prevX, currY, prevY ;

  CPen *oldPen ;
  CRect rectCleanUp ;
  CFont *oldDrawFont ;

  if (m_dcPlot.GetSafeHdc() != NULL)
  {
    // shift the plot by BitBlt'ing it to itself 
    // note: the m_dcPlot covers the entire client
    //       but we only shift bitmap that is the size 
    //       of the plot rectangle
    // grab the right side of the plot (exluding m_nShiftPixels on the left)
    // move this grabbed bitmap to the left by m_nShiftPixels

    m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top+1, 
                    m_nPlotWidth, m_nPlotHeight, &m_dcPlot, 
                    m_rectPlot.left+m_nShiftPixels, m_rectPlot.top+1, 
                    SRCCOPY) ;

    // establish a rectangle over the right side of plot
    // which now needs to be cleaned up proir to adding the new point
    rectCleanUp = m_rectPlot ;
    rectCleanUp.left  = rectCleanUp.right - m_nShiftPixels ;

    // fill the cleanup area with the background
    m_dcPlot.FillRect(rectCleanUp, &m_brushBack) ;

    // draw the next line segement

    // grab the plotting pen
    oldPen = m_dcPlot.SelectObject(&m_penPlot) ;

    // move to the previous point
    prevX = m_rectPlot.right-m_nPlotShiftPixels ;
    prevY = m_rectPlot.bottom - 
            (long)((m_dPreviousPosition - m_dLowerLimit) * m_dVerticalFactor) ;
    m_dcPlot.MoveTo (prevX, prevY) ;

		currX = m_rectPlot.right-m_nHalfShiftPixels ;
		currY = m_rectPlot.bottom -
				(long)((m_dCurrentPosition - m_dLowerLimit) * m_dVerticalFactor) ;
		m_dcPlot.LineTo (currX, currY) ;
    // draw to the current point

    // restore the pen 
    m_dcPlot.SelectObject(oldPen) ;

    // if the data leaks over the upper or lower plot boundaries
    // fill the upper and lower leakage with the background
    // this will facilitate clipping on an as needed basis
    // as opposed to always calling IntersectClipRect
 /*
 if ((prevY <= m_rectPlot.top) || (currY <= m_rectPlot.top))
      m_dcPlot.FillRect(CRect(prevX, m_rectClient.top, currX+1, m_rectPlot.top+1), &m_brushBack) ;
    if ((prevY >= m_rectPlot.bottom) || (currY >= m_rectPlot.bottom))
      m_dcPlot.FillRect(CRect(prevX, m_rectPlot.bottom+1, currX+1, m_rectClient.bottom+1), &m_brushBack) ;
*/
    // store the current point for connection to the next point
    m_dPreviousPosition = m_dCurrentPosition ;

	m_strXUnitsString = data;
	// append a data point to the plot
	// return the previous point
	// grab the horizontal font
	oldDrawFont = m_dcGrid.SelectObject(&axisDrawFont) ;
	if(oldDrawFont== NULL) {
		TRACE("oldDrawFont== NULL\n");
	}
	m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP) ;
//	if(oldDrawFont== NULL) {
//		m_dcGrid.TextOut ((m_rectPlot.left+m_rectPlot.right)/2 - 10, 
//							m_rectPlot.bottom+20, "???????????????????") ;
//	}
//	else {
		m_dcGrid.TextOut ((m_rectPlot.left+m_rectPlot.right)/2 - 10, 
							m_rectPlot.bottom+20, m_strXUnitsString) ;
//	}
	m_dcGrid.SelectObject(oldDrawFont) ;
	if(oldDrawFont== NULL) {
		TRACE("oldDrawFont== NULL\n");
	}
	else oldDrawFont->DeleteObject();
  }
} // end DrawPoint

/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::OnSize(UINT nType, int cx, int cy) 
{
  CWnd::OnSize(nType, cx, cy) ;

  // NOTE: OnSize automatically gets called during the setup of the control
  
  GetClientRect(m_rectClient) ;

  // set some member variables to avoid multiple function calls
  m_nClientHeight = m_rectClient.Height() ;
  m_nClientWidth  = m_rectClient.Width() ;

  // the "left" coordinate and "width" will be modified in 
  // InvalidateCtrl to be based on the width of the y axis scaling
  m_rectPlot.left   = 20 ;  
  m_rectPlot.top    = 20 ;
  m_rectPlot.right  = m_rectClient.right-10 ;
  m_rectPlot.bottom = m_rectClient.bottom-40 ;

  // set some member variables to avoid multiple function calls
  m_nPlotHeight = m_rectPlot.Height() ;
  m_nPlotWidth  = m_rectPlot.Width() ;

  // set the scaling factor for now, this can be adjusted 
  // in the SetRange functions
  m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 

} // OnSize


/////////////////////////////////////////////////////////////////////////////
void COScopeCtrl::Reset()
{
  // to clear the existing data (in the form of a bitmap)
  // simply invalidate the entire control
  InvalidateCtrl() ;
}
