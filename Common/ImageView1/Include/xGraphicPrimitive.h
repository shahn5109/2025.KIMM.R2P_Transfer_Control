#ifndef __GRAPHIC_PRIMITIVE_H__
#define __GRAPHIC_PRIMITIVE_H__

#include <XImageView/export.h>
#include <XImageView/xDatatypes.h>

XIMAGE_VIEW_API BOOL WINAPI DPtInPolygon( const DPOINT & dpt, const UINT unptNum, const DPOINT * pdPolyPt );
XIMAGE_VIEW_API BOOL WINAPI PtInPolygon( const POINT & pt, const UINT uptNum, const POINT * pPolyPt );
XIMAGE_VIEW_API BOOL WINAPI PtInPolygonW( const POINT & pt, const UINT uptNum, const POINT * pPolyPt );
XIMAGE_VIEW_API BOOL WINAPI IsOverlapDBoundDLine( const DRECT & dbd, const DPOINT pdpt[2] );
XIMAGE_VIEW_API BOOL WINAPI IsOverlapDBoundDBound( const DRECT & dbd1, const DRECT & dbd2 );
XIMAGE_VIEW_API BOOL WINAPI IsContainDBoundDBound( const DRECT & dbd1, const DRECT & dbd2 );
XIMAGE_VIEW_API BOOL WINAPI IsOverlapDBoundDPoly( const DRECT & dbd, const UINT uNPtNum, const DPOINT * pdpt, BOOL bClosed );
XIMAGE_VIEW_API BOOL WINAPI GetPointOnPolyline( const double fRatio, const UINT uPtNum, const DPOINT * pdPts, DPOINT * pdPtOut );
XIMAGE_VIEW_API BOOL WINAPI GetPointOnPolylineI( const double fRatio, const UINT uPtNum, const POINT * pPts, POINT * pPtOut );
XIMAGE_VIEW_API BOOL WINAPI TrimPolylineI( UINT * puPtNum, POINT * pPts );

XIMAGE_VIEW_API void WINAPI InflatePolygon( const POINT& ptCenter, const UINT unptNum, 
									  POINT* pPolyPt, const int x, const int y );
XIMAGE_VIEW_API void WINAPI InflatePolygon( const POINT& ptCenter, const UINT unptNum, 
									  POINT* pPolyPt, const double dMagnitude );
XIMAGE_VIEW_API void WINAPI InflatePolygon( const DPOINT& ptCenter, const UINT unptNum, 
									  DPOINT* pPolyPt, const double x, const double y );
XIMAGE_VIEW_API void WINAPI InflatePolygon( const DPOINT& ptCenter, const UINT unptNum, 
									  DPOINT* pPolyPt, const double dMagnitude );

XIMAGE_VIEW_API void WINAPI DeflatePolygon( const POINT& ptCenter, const UINT unptNum, 
									  POINT* pPolyPt, const int x, const int y );
XIMAGE_VIEW_API void WINAPI DeflatePolygon( const POINT& ptCenter, const UINT unptNum, 
									  POINT* pPolyPt, const double dMagnitude );
XIMAGE_VIEW_API void WINAPI DeflatePolygon( const DPOINT& ptCenter, const UINT unptNum, 
									  DPOINT* pPolyPt, const double x, const double y );
XIMAGE_VIEW_API void WINAPI DeflatePolygon( const DPOINT& ptCenter, const UINT unptNum, 
									  DPOINT* pPolyPt, const double dMagnitude );


#endif //_GRAPHIC_PRIMITIVE_H__