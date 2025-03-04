#ifndef __XDATA_TYPES_H__
#define __XDATA_TYPES_H__

#ifndef M_PI
#define	M_PI		(3.1415926535897932384626433832795)
#endif

#include <math.h>
#include <wtypes.h>
#include <tchar.h>

struct _DPOINT
{
	double	x;
	double	y;
};

class DPOINT : public _DPOINT
{
public:
	DPOINT( double _x, double _y ) { x = _x; y = _y; }
	DPOINT() {}

	DPOINT( const DPOINT& Other )
	{
		x = Other.x;
		y = Other.y;
	}

	DPOINT( const POINT Other )
	{
		x = (double)Other.x;
		y = (double)Other.y;
	}
	
	// did not support -> use ToPOINT
	//operator POINT() const;

	POINT ToPOINT() const
	{
		POINT point;
		point.x	= (long)(x+.5);
		point.y	= (long)(y+.5);
		return point;
	}

	void FromPOINT( const POINT other )
	{
		x = (double)other.x;
		y = (double)other.y;
	}

	BOOL operator == ( const DPOINT& point ) const
	{
		return ((point.x == x) && (point.y == y)) ? TRUE : FALSE;
	}
	BOOL operator != ( const DPOINT& point ) const
	{
		return ((point.x == x) && (point.y == y)) ? FALSE : TRUE;
	}
	void operator += (const DPOINT& point)
	{
		x += point.x; y += point.y;
	}
	void operator -= (const DPOINT& point)
	{
		x -= point.x; y -= point.y;
	}

	DPOINT operator-() const
	{
		return DPOINT( -x, -y );
	}
	
	DPOINT operator + ( DPOINT& point) const
	{
		return DPOINT( x+point.x, y+point.y );
	}

	DPOINT operator - ( DPOINT& point) const
	{
		return DPOINT( x-point.x, y-point.y );
	}

	const DPOINT& operator = ( const DPOINT& point )
	{
		(*this).x = point.x;
		(*this).y = point.y;
		return (*this);
	}
};

typedef DPOINT* LPDPOINT;

inline DPOINT operator + ( const DPOINT& lhs, const DPOINT& rhs )
{
	return DPOINT( lhs.x+rhs.x, lhs.y+rhs.y );
}

inline DPOINT operator - ( const DPOINT& lhs, const DPOINT& rhs )
{
	return DPOINT( lhs.x-rhs.x, lhs.y-rhs.y );
}

struct _DRECT
{
	double left;
	double top;
	double right;
	double bottom;
};

class DRECT : public _DRECT
{
public:
	DRECT() {}
	DRECT( const DRECT& OtherRect )
	{
		left	= OtherRect.left;
		top		= OtherRect.top;
		right	= OtherRect.right;
		bottom	= OtherRect.bottom;
	}
	DRECT( const RECT OtherRect )
	{
		left	= (double)OtherRect.left;
		top		= (double)OtherRect.top;
		right	= (double)OtherRect.right;
		bottom	= (double)OtherRect.bottom;
	}

	RECT ToRECT() const
	{
		RECT rect;
		rect.left	= (long)(left+.5);
		rect.top	= (long)(top+.5);
		rect.right	= (long)(right+.5);
		rect.bottom = (long)(bottom+.5);
		return rect;	
	}

	DRECT( double l, double t, double r, double b ) { left = l; top = t; right = r; bottom = b; }

	double Width() const { return fabs(right-left); }
	double Height() const { return fabs(bottom-top); }
	DPOINT CenterPoint() const { return DPOINT((left+right)/2., (top+bottom)/2.); }

	BOOL IsEmpty() const { if (Width() <= 0. || Height() <= 0.) return TRUE; return FALSE; }

	void SetRectEmpty() { left = right = top = bottom = 0.; }

	BOOL PtInRect( const DPOINT& point ) const
	{
		if ( left > point.x || right <= point.x ) return FALSE;
		if ( top > point.y || bottom <= point.y ) return FALSE;
	
		return TRUE;
	}

	void DeflateRect(double l, double t, double r, double b) { left+=l; top+=t; right-=r; bottom-=b; }
	void InflateRect(double l, double t, double r, double b) { left-=l; top-=t; right+=r; bottom+=b; }

	void IntersectRect( const DRECT& rc1, const DRECT& rc2 ) 
	{
		BOOL bIntersect =  (		rc2.left < rc1.right
								&&	rc2.right > rc1.left
								&&	rc2.top < rc1.bottom
								&&	rc2.bottom > rc1.top
							); 

		if ( bIntersect )
		{
			left	= max(rc1.left, rc2.left);
			top		= max(rc1.top, rc2.top);
			right	= min(rc1.right, rc2.right);
			bottom	= min(rc1.bottom, rc2.bottom);
		}
		else
		{
			left = top = right = bottom = 0.0;
		}
	}

	void UnionRect( const DRECT& rc1, const DRECT& rc2 ) 
	{
		left	= min(rc1.left, rc2.left);
		right	= max(rc1.right, rc2.right);
		top		= min(rc1.top, rc2.top);
		bottom	= max(rc1.bottom, rc2.bottom);
	}

	void NormalizeRect()
	{
		double t;
		if ( left > right ) { t = left; left = right ; right = t; }
		if ( top > bottom ) { t = top; top = bottom; bottom = t; }
	}

	const DRECT& operator = ( const DRECT& OtherRect )
	{
		left	= OtherRect.left;
		top		= OtherRect.top;
		right	= OtherRect.right;
		bottom	= OtherRect.bottom;
		return (*this);
	}
	const DRECT& operator = ( const RECT& OtherRect )
	{
		left	= (double)OtherRect.left;
		top		= (double)OtherRect.top;
		right	= (double)OtherRect.right;
		bottom	= (double)OtherRect.bottom;
		return (*this);
	}

	BOOL operator == ( const DRECT& OtherRect ) const
	{
		if ( (left == OtherRect.left) && (top == OtherRect.top) && (right == OtherRect.right) && (bottom == OtherRect.bottom) )
			return TRUE;
		return FALSE;
	}

	BOOL operator != ( const DRECT& OtherRect ) const
	{
		return !(operator == (OtherRect));
	}

	BOOL IsOverlap( const DRECT& OtherRect ) const
	{
		if ( left > OtherRect.left ) {	if ( left > OtherRect.right ) return FALSE; }
		else { if ( right < OtherRect.left ) return FALSE;	}
		
		if ( top > OtherRect.top )	{ if ( top > OtherRect.bottom ) return FALSE; }
		else { if ( bottom < OtherRect.top ) return FALSE; }
		
		return TRUE;
	}
	
	BOOL IsContain(const DRECT& OtherRect) const
	{
		return left <= OtherRect.left && right >= OtherRect.right && top <= OtherRect.top && bottom >= OtherRect.bottom;
	}
	
	BOOL IsOverlapWithLine(const DPOINT& pt1, const DPOINT& pt2) const
	{
		DRECT TempRect;

		if ( pt1.x < pt2.x ) { TempRect.left = pt1.x; TempRect.right = pt2.x; }
		else { TempRect.left = pt2.x; TempRect.right = pt1.x; }

		if ( pt1.y < pt2.y ) { TempRect.top = pt1.y; TempRect.bottom = pt2.y; }
		else { TempRect.top = pt2.y; TempRect.bottom = pt1.y; }
		
		if ( left>TempRect.right || right<TempRect.left || top>TempRect.bottom || bottom<TempRect.top )
		{
			return FALSE;
		}
		
		double dDx = pt1.x - pt2.x;
		double dDy = pt1.y - pt2.y;
		
		double dP1 = dDy * ( left - pt2.x )	- dDx * (top - pt2.y );
		double dP2 = dDy * ( left - pt2.x )	- dDx * (bottom - pt2.y );
		double dP3 = dDy * ( right - pt2.x ) - dDx * (top - pt2.y );
		double dP4 = dDy * ( right - pt2.x ) - dDx * (bottom - pt2.y );
		
		if ( dP1 > 0.0f ) { if ( dP2>0.0f && dP3>0.0f && dP4>0.0f ) return FALSE; }
		else if ( dP1 < 0.0f ) { if ( dP2<0.0f && dP3<0.0f && dP4<0.0f ) return FALSE; }
		
		return TRUE;
	}
};

#endif //__XDATA_TYPES_H__