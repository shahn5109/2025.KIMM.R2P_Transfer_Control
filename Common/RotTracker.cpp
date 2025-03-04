#include "StdAfx.h"
#include ".\rottracker.h"

CRotTracker::CRotTracker(void)
{
//	initVariables() ;
}
CRotTracker::CRotTracker(CWnd *pWnd1, CWnd *pWnd2)
{	
	initVariables() ;
	parent = pWnd1 ;
	disp = pWnd2 ;
}

CRotTracker::~CRotTracker(void)
{
}
void CRotTracker::initVariables()
{
	m_nMode = TransformNone ;
	m_bCreate = true;
	InitTrackerInfo(&m_poly) ;
	InitTrackerInfo(&m_tmppoly) ;
	InitTrackerInfo(&m_oldpoly) ;	
	parent = 0, disp =0 ;
	oldcenter.x =0 ; oldcenter.y =0;
}
void CRotTracker::ConnectTracker(CWnd *pWnd1 , CWnd *pWnd2)
{
	parent = pWnd1 ;
	disp = pWnd2 ;
	m_bCreate = true;
}
void CRotTracker::TrackRubberBandSet()
{
	CRect rect ;
	m_poly.m_bDraw = true ;
	rect.top = min(m_rect.top,m_rect.bottom) ;
	rect.bottom = max(m_rect.top,m_rect.bottom) ;
	rect.left = min(m_rect.left,m_rect.right) ;
	rect.right = max(m_rect.left,m_rect.right) ;
	
	m_rect = rect ;	
	m_poly.angle = 0. ;
	m_poly.rect = m_rect ;
	SetPolyGon() ;
	SetTrackPoint() ;	
}
void CRotTracker::EraseTracker()
{
	InitTrackerInfo(&m_poly) ;
	InitTrackerInfo(&m_tmppoly) ;
	InitTrackerInfo(&m_oldpoly) ;	
	/*CRect rect ;
	m_poly.m_bDraw = false ;
	rect.top = 0 ;
	rect.bottom = 0 ;
	rect.left = 0 ;
	rect.right = 0 ;	
	m_rect = rect ;	
	m_poly.angle = 0. ;
	m_poly.rect = rect ;
	SetPolyGon() ;
	SetTrackPoint() ;
	*/
}
void CRotTracker::InitTrackerInfo(Tracker_info *info)
{
	memset(info , 0, sizeof(Tracker_info)) ;
}
void CRotTracker::SetPolyGon()	
{
//	m_poly.r = sqrt(pow((double)(m_rect.left - m_rect.right), 2.) + pow((double)(m_rect.top - m_rect.bottom), 2.)) / 2. ;

	m_poly.rect = m_rect ;
	m_poly.poly[0].x = m_rect.left ;
	m_poly.poly[0].y = m_rect.top ;	
	m_poly.poly[1].x = m_poly.poly[0].x + m_rect.Width() ;
	m_poly.poly[1].y = m_poly.poly[0].y ;
	m_poly.poly[2].x = m_poly.poly[0].x + m_rect.Width() ;
	m_poly.poly[2].y = m_poly.poly[0].y + m_rect.Height() ;
	m_poly.poly[3].x = m_poly.poly[0].x ;
	m_poly.poly[3].y = m_poly.poly[0].y + m_rect.Height() ;

	m_poly.poly[4].x = m_poly.poly[0].x ;
	m_poly.poly[4].y = m_poly.poly[0].y;

	m_poly.center.x = m_poly.poly[0].x + (int)(m_rect.Width()/2.) ;
	m_poly.center.y = m_poly.poly[0].y + (int)(m_rect.Height()/2.) ;

	m_poly.endpt.x = m_poly.center.x + (int)(m_rect.Width() * 0.7) ;
	m_poly.endpt.y = m_poly.center.y ;	

	if(m_poly.angle != 0)
	{
		long endx = m_poly.center.x + (int)(m_rect.Width() * 0.7) ;
		long endy = m_poly.center.y ;		
		RotatePoint(m_poly.center.x , m_poly.center.y, &endx , &endy, m_poly.angle-180) ;
		
		m_poly.endpt.x = endx ;
		m_poly.endpt.y = endy ;
//		TRACE("endx1 = %d , endy1 = %d \n", endx , endy) ;
	}
}

void CRotTracker::SetTrackPoint()
{
	m_poly.end.left =  m_poly.endpt.x - 7 ;
	m_poly.end.right =  m_poly.endpt.x+7;
	m_poly.end.top =  m_poly.endpt.y -7;
	m_poly.end.bottom =  m_poly.endpt.y + 7;

	m_poly.lt.left  = (LONG)(m_poly.poly[0].x) -3 ;
	m_poly.lt.top   = m_poly.poly[0].y-3 ;
	m_poly.lt.right = m_poly.lt.left +6 ;
	m_poly.lt.bottom = m_poly.lt.top +6 ;

	m_poly.rt.left   = m_poly.poly[1].x - 3;
	m_poly.rt.top   = m_poly.poly[1].y - 3;
	m_poly.rt.right = m_poly.rt.left +6 ;
	m_poly.rt.bottom = m_poly.rt.top +6 ;

	m_poly.rb.left   = m_poly.poly[2].x- 3 ;
	m_poly.rb.top   = m_poly.poly[2].y - 3;
	m_poly.rb.right = m_poly.rb.left +6 ;
	m_poly.rb.bottom = m_poly.rb.top +6 ;


	m_poly.lb.left  = m_poly.poly[3].x- 3 ;
	m_poly.lb.top  = m_poly.poly[3].y- 3 ;
	m_poly.lb.right = m_poly.lb.left + 6 ;
	m_poly.lb.bottom = m_poly.lb.top +6 ;

	m_poly.top.left = (LONG)(m_poly.poly[0].x + (m_poly.poly[1].x - m_poly.poly[0].x)/2.) -3;
	m_poly.top.right = m_poly.top.left + 6;
	m_poly.top.top = (LONG)(m_poly.poly[0].y + (m_poly.poly[1].y - m_poly.poly[0].y)/2.) -3;
	m_poly.top.bottom = m_poly.top.top + 6 ;


	m_poly.right.left = (LONG)(m_poly.poly[1].x + (m_poly.poly[2].x - m_poly.poly[1].x)/2.) -3;
	m_poly.right.right = m_poly.right.left + 6;

	m_poly.right.top = (LONG)(m_poly.poly[1].y + (m_poly.poly[2].y - m_poly.poly[1].y)/2.) -3;
	m_poly.right.bottom = m_poly.right.top + 6 ;


	m_poly.bottom.left = (LONG)(m_poly.poly[3].x + (m_poly.poly[2].x - m_poly.poly[3].x)/2.) -3;
	m_poly.bottom.right = m_poly.bottom.left + 6;
	m_poly.bottom.top = (LONG)(m_poly.poly[2].y + (m_poly.poly[3].y - m_poly.poly[2].y)/2.) -3;
	m_poly.bottom.bottom = m_poly.bottom.top + 6 ;


	m_poly.left.left = (LONG)(m_poly.poly[3].x + (m_poly.poly[0].x - m_poly.poly[3].x)/2.) -3;
	m_poly.left.right = m_poly.left.left + 6;
	m_poly.left.top = (LONG)(m_poly.poly[0].y + (m_poly.poly[3].y - m_poly.poly[0].y)/2.) -3;
	m_poly.left.bottom = m_poly.left.top + 6 ;
}

int CRotTracker::Hit_Test(CPoint pt)
{
	CPoint npt, ppt;
	int ret_val= 0;

	if(PolyInside(m_poly.poly , pt) == TRUE){ret_val= 8 ;}
	else if(m_poly.lt.PtInRect(pt) || m_poly.lt.PtInRect(pt)){ret_val= 0 ;}
	else if(m_poly.rb.PtInRect(pt)){ret_val= 2 ;}
	else if(m_poly.rt.PtInRect(pt)){ret_val= 1 ;}
	else if(m_poly.lb.PtInRect(pt)){ret_val= 3 ;}

	else if(m_poly.top.PtInRect(pt)){
		ret_val= 4 ;
	}
	else if(m_poly.bottom.PtInRect(pt)){
		ret_val= 6 ;
	}
	///////////////
	else if(m_poly.left.PtInRect(pt)){
		ret_val= 5;
	}
	else if(m_poly.right.PtInRect(pt)){
		ret_val= 7;
	}

	else if(m_poly.end.PtInRect(pt)){
		ret_val= 9;
	}
	else ret_val= -1 ;

	return ret_val;
}
/*
void CRotTracker::DrawTracker(bool repaint)
{	
	TRACE("S-DrawTracker");
	CRect rect ;
   	disp->GetWindowRect(rect) ;
   	disp->ScreenToClient(rect) ;
   	CDC *pDC = disp->GetDC() ;	

//	int nOldRop = pDC->SetROP2(R2_XORPEN);		
   	int nOldRop = pDC->SetROP2(R2_COPYPEN);		

   	HRGN hRgn = ::CreateRectRgnIndirect(&rect);
   	::SelectClipRgn(pDC->m_hDC , hRgn) ;

   	DrawRect(pDC) ;
   	DrawArrow(pDC) ;
   	DrawRectPoint(pDC) ;
	
   	pDC->SetROP2(nOldRop);	

   	::SelectClipRgn(pDC->m_hDC, NULL) ;
   	disp->ReleaseDC(pDC) ;	
	
	TRACE("DrawTracker-E\n");
}
*/
/*void CRotTracker::DrawRect(CDC *pDC, bool repaint)
{		
	TRACE("S-DrawRect");
   	CPen *oldPen ;
   	CPen pen(PS_COSMETIC | PS_ALTERNATE, 1, RGB(0,255,0)) ;	
   	oldPen=pDC->SelectObject(&pen);			
   	
   	pDC->MoveTo(m_poly.poly[0].x, m_poly.poly[0].y);    
   	for(int i = 1; i < 4; i++)
   	{        
   		pDC->LineTo(m_poly.poly[i].x, m_poly.poly[i].y) ;
   	}
   	pDC->LineTo(m_poly.poly[0].x, m_poly.poly[0].y) ;	
   	
   	
   	pDC->SelectObject(oldPen);
   	pen.DeleteObject();	
	TRACE("DrawRect-E\n");
}
*/
/*void CRotTracker::DrawRectPoint(CDC *pDC, bool repaint)
{
	TRACE("S-DrawRectPoint");
	CPen *oldPen ;
	CPen pen(PS_COSMETIC | PS_ALTERNATE, 1, RGB(255,0,0)) ;	
	oldPen=pDC->SelectObject(&pen);	
	
	CBrush brush, *oldBrush ;
	brush.CreateSolidBrush(RGB(255,0,0)) ;
	oldBrush=pDC->SelectObject(&brush);	
		

	pDC->FillRect(m_poly.left, &brush) ;
	pDC->FillRect(m_poly.right, &brush) ;
	pDC->FillRect(m_poly.top, &brush) ;
	pDC->FillRect(m_poly.bottom, &brush) ;

	pDC->FillRect(m_poly.end, &brush) ;

	pDC->FillRect(m_poly.lb, &brush) ;
	pDC->FillRect(m_poly.lt, &brush) ;
	pDC->FillRect(m_poly.rb, &brush) ;
	pDC->FillRect(m_poly.rt, &brush) ;	

	
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pen.DeleteObject() ;
	brush.DeleteObject() ;

	
	TRACE("DrawRectPoint-E\n");
}*/
/*
void CRotTracker::DrawArrow(CDC *pDC, bool repaint)
{
	TRACE("S-DrawArrow");
	CPen *oldPen ;
	CPen pen(PS_COSMETIC | PS_ALTERNATE, 1, RGB(0,255,0)) ;
	
	
	oldPen=pDC->SelectObject(&pen);	
	pDC->MoveTo(m_poly.center.x, m_poly.center.y);    	
	pDC->LineTo(m_poly.endpt.x, m_poly.endpt.y) ;		
	
	pDC->SelectObject(oldPen);
	pen.DeleteObject();	
	TRACE("DrawArrow-E\n");
}
*/
/*
enum TrackState
	{
		TrackFailed = -2,
		TrackCancelled = -1,
		TrackContinue = 0,
		TrackSucceeded = 1,
		TrackCopy = 2,
		TrackNoMove = 3
	};
enum Mode
	{
		TransformNone,
		TransformMove,
		TransformScale,
		TransformRotate,
		TransformShear,
		TransformCenter,
	};
*/
int CRotTracker::BeginTrack(int HitFlag)
{
	switch(HitFlag)
	{
	case 8:
		m_nMode = TransformMove ;
		break ;
	case 9:
		m_nMode = TransformRotate ;
		break ;
	case -1 :
		m_nMode = TransformNone ;
		break ;
	default:
		m_nMode = TransformScale ;
	}		

	m_bTrackStart = true ;
	return TrackContinue ;
}
int CRotTracker::EndTrack()
{
	m_nMode = TransformNone ;
	return TrackSucceeded ;
}
int CRotTracker::Track(CPoint pt, int HitFlag)
{
	
	int status = -2 ;
//	if (::GetCapture()) return TrackFailed ;	// 마우스 Capture 실패.

	status = BeginTrack(HitFlag) ;

  // 	::AfxLockTempMaps();
	parent->SetCapture();	
//	CClientDC cdc(disp) ;
   	CDC *cdc = disp->GetDC() ;

	CRect rect ;
	disp->GetWindowRect(rect) ;
	disp->ScreenToClient(rect) ;
	m_oldpoly = m_poly ;	
	InitTrackerInfo(&m_tmppoly) ;

	while (status == TrackContinue)
	{
		MSG msg;		
		while (! ::PeekMessage(& msg, NULL, 0, 0, PM_REMOVE))
		{
			Sleep(1) ;
		}
		if (CWnd::GetCapture() != parent || msg.message == WM_CANCELMODE) 
			status = TrackFailed;
		else if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
		{
			int x = (int)(short) LOWORD(msg.lParam);
			int y = (int)(short) HIWORD(msg.lParam);
			
			
			CPoint pnt(x, y);
   			if (cdc) cdc->DPtoLP(& pnt);			
			pnt = pnt - pt;

			if(m_nMode == TransformMove && m_bTrackStart == true)
			{
				m_offset.x = pnt.x - m_poly.center.x ;
				m_offset.y = pnt.y - m_poly.center.y ;
				m_bTrackStart = false ;
			}
			if(m_nMode == TransformScale && m_bTrackStart == true)
			{
				m_tmppoly = m_oldpoly ;
				center = m_poly.center ;
				end = m_poly.endpt ;
				m_bTrackStart = false ;
			}
   			status = OnMouseMessage(msg.message, HitFlag, pnt);
   			OnUpdate(cdc , 0, rect) ;
			
		}
		else 
		{
   	//		::DispatchMessage(& msg);	
		}
	}
	if(m_nMode != TransformScale)
	{		

		CRect Trackrect ;

		Trackrect.top = min(m_rect.top,m_rect.bottom) ;
		Trackrect.bottom = max(m_rect.top,m_rect.bottom) ;
		Trackrect.left = min(m_rect.left,m_rect.right) ;
		Trackrect.right = max(m_rect.left,m_rect.right) ;				
		m_rect = Trackrect ;
   		SetPolyGon() ;
		if(m_poly.angle){		
   			RotateRect(&m_poly) ; 		
		}	
		SetTrackPoint() ;	
   		OnUpdate(cdc , 1, rect) ;
	}
	else
	{		
   		OnUpdate(cdc , 1, rect) ;
		m_rect = GetRect(m_poly.poly) ;		
   		SetPolyGon() ;
		if(m_poly.angle){		
   			RotateRect(&m_poly) ; 		
		}	
		SetTrackPoint() ;
	}	
	
   	disp->ReleaseDC(cdc) ;
	::ReleaseCapture() ;
// 	::AfxUnlockTempMaps();
	status = EndTrack() ;
	return 0 ;
}
int CRotTracker::OnMouseMessage(UINT msg, int HitFlag, CPoint pt)
{
	if(msg == WM_LBUTTONUP) {
		return TrackSucceeded ;	
	}
	switch (m_nMode)
	{
	case TransformMove:
		{
			ShiftPolygon(pt) ;
		}
		break;
	case TransformScale:
		{
			ScalePolygon(HitFlag , pt) ;				
		}
		break ;
	case TransformRotate:
		RotatePolygon(pt) ;
		break ;
	}
//	if (msg == WM_LBUTTONUP) return (nFlags & MK_CONTROL) ? TrackCopy : TrackSucceeded;
//	if (msg == WM_RBUTTONDOWN) return TrackCancelled;
	return TrackContinue;
}
void CRotTracker::RotatePolygon(CPoint pt)
{
	m_tmppoly = m_poly ;
	m_poly.angle = CalculateAngle(pt) ;
	RotateRect(&m_poly) ;
	SetTrackPoint() ;
}

void CRotTracker::RotateRect(Tracker_info *info)
{
	POINT poly[5] ;

	poly[0].x = info->rect.left ;
	poly[0].y = info->rect.top ;	
	poly[1].x = poly[0].x + info->rect.Width() ;
	poly[1].y = poly[0].y ;
	poly[2].x = poly[0].x + info->rect.Width() ;
	poly[2].y = poly[0].y + info->rect.Height() ;
	poly[3].x = poly[0].x ;
	poly[3].y = poly[0].y + info->rect.Height() ;

	poly[4].x = poly[0].x ;
	poly[4].y = poly[0].y;

	
	for(int i=0 ; i<4 ; i++)
		RotatePoint(info->center.x , info->center.y, &poly[i].x , &poly[i].y, info->angle-180) ;

	poly[4] = poly[0] ;
	
	memcpy(info->poly , poly, sizeof(POINT) * 5) ;
	
	
}
void CRotTracker::RotateRect1(Tracker_info *info, POINT cent)
{
	POINT poly[5] ;

	poly[0].x = info->rect.left ;
	poly[0].y = info->rect.top ;	
	poly[1].x = poly[0].x + info->rect.Width() ;
	poly[1].y = poly[0].y ;
	poly[2].x = poly[0].x + info->rect.Width() ;
	poly[2].y = poly[0].y + info->rect.Height() ;
	poly[3].x = poly[0].x ;
	poly[3].y = poly[0].y + info->rect.Height() ;

	poly[4].x = poly[0].x ;
	poly[4].y = poly[0].y;

	
	for(int i=0 ; i<4 ; i++)
		RotatePoint1(cent.x , cent.y, &poly[i].x , &poly[i].y, info->angle-180) ;

	poly[4] = poly[0] ;
	
	memcpy(info->poly , poly, sizeof(POINT) * 5) ;
	
}
void CRotTracker::RotatePoint1(long centx, long centy, long *ptx, long *pty, double angle)
{	
	double x=0, y=0 ;
	double cosine=0 , sine =0 ;
	cosine = cos(3.14 - angle* 3.141592/180) ;
	sine = sin(3.14 - angle * 3.141592/180) ;

	x = (*ptx -  centx) *cosine - (*pty -  centy) * sine ;
	y = (*pty -  centy) *cosine + (*ptx -  centx) * sine ;

	*ptx = (long)( x + centx );
	*pty = (long)( y + centy );	

}
void CRotTracker::RotatePoint(long centx, long centy, long *ptx, long *pty, double angle)
{	
	double x=0, y=0 ;
	double cosine=0 , sine =0 ;
	cosine = cos(3.14 - angle* 3.141592/180) ;
	sine = sin(3.14 - angle * 3.141592/180) ;

	x = (*ptx -  centx) *cosine - (*pty -  centy) * sine ;
	y = (*pty -  centy) *cosine + (*ptx -  centx) * sine ;

	*ptx = (long)( x + centx );
	*pty = (long)( y + centy );	

}
double CRotTracker::CalculateAngle(CPoint pt)
{
	double angle =0 ;
	angle  = atan2((double)-(pt.y-m_poly.center.y), (double)(pt.x - m_poly.center.x)) ;
	angle = angle * (180 / 3.141592) ;	
	if(angle <0) angle += 360 ;		
	return angle ;
}
void CRotTracker::ShiftPolygon(CPoint pt)
{	
	m_tmppoly = m_poly ;
	CRect rect ;

	long centx = pt.x - m_offset.x  ;
	long centy = pt.y - m_offset.y ;
	rect.left = (long)(centx - m_rect.Width()/2.) ;
	rect.top = (long)(centy - m_rect.Height()/2.) ;
	rect.right = rect.left + m_rect.Width() ;
	rect.bottom = rect.top + m_rect.Height() ;

	m_rect = rect ;
	SetPolyGon() ;
	if(m_poly.angle !=0)
	{
		RotateRect(&m_poly) ;		
	}	
}
void CRotTracker::ScalePolygon(int HitFlag, CPoint pt)
{
	m_tmppoly = m_poly ;
	CRect rect=0, oldrect ;
	oldrect = m_oldpoly.rect ;
	POINT p1 , p2;			
	double r1 =0 ;
	double angle =0 ;			
	switch(HitFlag)
	{
	case 0:		
		p1.x = pt.x - m_oldpoly.poly[2].x ;
		p1.y =  m_oldpoly.poly[2].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;		

		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	

		if(p2.x < 0 && p2.y >=0)
		{
			rect.right = oldrect.right ;
			rect.bottom = oldrect.bottom ;
			rect.left = rect.right + p2.x ;
			rect.top = rect.bottom - p2.y ;					
		}
		if(p2.x > 0 && p2.y > 0)
		{
			rect.left = oldrect.right ;
			rect.bottom = oldrect.bottom ;
			rect.right = rect.left + p2.x ;
			rect.top = rect.bottom - p2.y ;
		}
		if(p2.x > 0 && p2.y < 0)
		{
			rect.left = oldrect.right ;
			rect.top = oldrect.bottom ;
			rect.right = rect.left + p2.x ;
			rect.bottom = rect.top - p2.y ;
		}		
		if(p2.x < 0 && p2.y < 0)
		{
			rect.right = oldrect.right ;
			rect.top = oldrect.bottom ;
			rect.left = rect.right + p2.x ;
			rect.bottom = rect.top - p2.y ;
		}
		break ;
	case 1:
		p1.x = pt.x - m_oldpoly.poly[3].x ;
		p1.y =  m_oldpoly.poly[3].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;	
		//if(angle <0) angle = angle + 180 ;

		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	
		
		if(p2.x >= 0 && p2.y >=0)
		{
			rect.left = oldrect.left ;
			rect.bottom = oldrect.bottom ;
			rect.right = rect.left + p2.x ;
			rect.top = rect.bottom - p2.y ;		
		}

		if(p2.x < 0 && p2.y > 0)
		{
			//rect.left
			rect.right = oldrect.left ;
			rect.bottom = oldrect.bottom ;
			rect.left = rect.right + p2.x ;
			rect.top = rect.bottom - p2.y ;
		}

		if(p2.x > 0 && p2.y < 0)
		{
			rect.left= oldrect.left ;
			rect.top = oldrect.bottom ;

			rect.right = rect.left + p2.x ;
			rect.bottom = rect.top - p2.y ;		
		}
		
		if(p2.x < 0 && p2.y < 0)
		{
			rect.right = oldrect.left ;
			rect.top = oldrect.bottom ;

			rect.left = rect.right + p2.x ; 
			rect.bottom = rect.top - p2.y ;
		}
		break ;
	case 2:
		p1.x = pt.x - m_oldpoly.poly[0].x ;
		p1.y =  m_oldpoly.poly[0].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;	
		//if(angle <0) angle = angle + 180 ;

		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	
		if(p2.x >= 0 && p2.y >=0)
		{
			rect.left = oldrect.left ;
			rect.bottom = oldrect.top ;
			rect.right = rect.left + p2.x ;
			rect.top = rect.bottom - p2.y ;
		}
		if(p2.x < 0 && p2.y > 0)
		{
			rect.right = oldrect.left ;
			rect.bottom = oldrect.top ;
			rect.left = rect.right + p2.x ;
			rect.top = rect.bottom - p2.y ;
		}
		if(p2.x > 0 && p2.y < 0)
		{
			rect.left = oldrect.left ;
			rect.top = oldrect.top ;
			rect.right = rect.left + p2.x ;
			rect.bottom = rect.top - p2.y ;
		}		
		if(p2.x < 0 && p2.y < 0)
		{
			rect.right = oldrect.left ;
			rect.top = oldrect.top ;
			rect.left = rect.right + p2.x ;
			rect.bottom = rect.top - p2.y ;
		}
		break ;
	case 3:
		p1.x = pt.x - m_oldpoly.poly[1].x ;
		p1.y =  m_oldpoly.poly[1].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;	
		//if(angle <0) angle = angle + 180 ;

		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;			
		if(p2.x >= 0 && p2.y >=0)
		{
			rect.left = oldrect.right ;
			rect.bottom = oldrect.top ;
			rect.right = rect.left + p2.x ;
			rect.top = rect.bottom - p2.y ;
		}
		if(p2.x < 0 && p2.y > 0)
		{
			rect.right = oldrect.right ;
			rect.bottom = oldrect.top ;
			rect.left = rect.right + p2.x ;
			rect.top = rect.bottom - p2.y ;
		}
		if(p2.x > 0 && p2.y < 0)
		{
			rect.left = oldrect.right ;
			rect.top = oldrect.top ;
			rect.right = rect.left + p2.x ;
			rect.bottom = rect.top - p2.y ;
		}		
		if(p2.x < 0 && p2.y < 0)
		{
			rect.right = oldrect.right ;
			rect.top = oldrect.top ;
			rect.left = rect.right + p2.x ;
			rect.bottom = rect.top - p2.y ;
		}
		break ;
	case 4:
		p1.x = pt.x - m_oldpoly.poly[2].x ;
		p1.y =  m_oldpoly.poly[2].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;		
		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	

		rect.right = oldrect.right ;
		rect.left = oldrect.left ;
		if(p2.y <= 0)
		{
			rect.top = oldrect.bottom ;
			rect.bottom = rect.top - p2.y ;
		}
		else
		{
			rect.bottom = oldrect.bottom ;
			rect.top = rect.bottom - p2.y ;
		}
		break ;
	case 5:
		p1.x = pt.x - m_oldpoly.poly[1].x ;
		p1.y =  m_oldpoly.poly[1].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;		
		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	
		rect.top = oldrect.top ;
		rect.bottom = oldrect.bottom ;
		if(p2.x <=0)
		{
			rect.right = oldrect.right ;
			rect.left = rect.right + p2.x ;
		}
		else			
		{
			rect.left = oldrect.right ;
			rect.right = rect.left + p2.x ;
		}		

		break ;
	case 6:
		p1.x = pt.x - m_oldpoly.poly[1].x ;
		p1.y =  m_oldpoly.poly[1].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;		
		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	

		rect.right = oldrect.right ;
		rect.left = oldrect.left ;
		if(p2.y <= 0)
		{
			rect.top = oldrect.top ;
			rect.bottom = rect.top - p2.y ;
		}
		else
		{
			rect.bottom = oldrect.top ;
			rect.top = rect.bottom - p2.y ;
		}
		break ;
	case 7:
		p1.x = pt.x - m_oldpoly.poly[0].x ;
		p1.y =  m_oldpoly.poly[0].y - pt.y ;		
		r1 = sqrt(pow((double)(p1.x), 2.) + pow((double)(p1.y), 2.)) ;
		angle = atan2((double)p1.y , (double)p1.x)* (180/3.14) ;		
		p2.x = (long)(r1 * cos((angle-m_oldpoly.angle) * (3.14/180)) ) ;
		p2.y = (long)(r1 * sin( (angle-m_oldpoly.angle) * (3.14/180)) ) ;	
		rect.top = oldrect.top ;
		rect.bottom = oldrect.bottom ;
		if(p2.x <=0)
		{
			rect.right = oldrect.left ;
			rect.left = rect.right + p2.x ;
		}
		else			
		{
			rect.left = oldrect.left ;
			rect.right = rect.left + p2.x ;
		}		
		break ;
	}
	m_rect.top = min(rect.top,rect.bottom) ;
	m_rect.bottom = max(rect.top,rect.bottom) ;
	m_rect.left = min(rect.left,rect.right) ;
	m_rect.right = max(rect.left,rect.right) ;		
	m_poly.angle = m_oldpoly.angle ;
	SetPolyGon() ;
	if(m_poly.angle)
	{
		RotateRect1(&m_poly,center) ;			
	}
}
CRect CRotTracker::GetRect(POINT *poly)
{
	CRect rect =0;
	POINT cent ;
	POINT tmp[5] ;
	memcpy(tmp , poly, sizeof(POINT) * 5) ;
	int x1, y1 , x2, y2;
	x1 = min(poly[0].x , poly[2].x) ;
	x2 = max(poly[0].x , poly[2].x) ;
	y1 = min(poly[0].y , poly[2].y) ;
	y2 = max(poly[0].y , poly[2].y) ;

	cent.x = (long)( x1 + ( x2-x1)* 0.5) ;
	cent.y = (long)(y1 + ( y2- y1)* 0.5) ;	

	RotatePoint(cent.x , cent.y, &tmp[0].x , &tmp[0].y, -m_poly.angle-180) ;
	
	rect.left = tmp[0].x ;
	rect.top = tmp[0].y ;
	rect.right = rect.left + m_poly.rect.Width() ;
	rect.bottom = rect.top + m_poly.rect.Height() ;
	return rect ;
}
double CRotTracker::NodeLength(POINT pt1, POINT pt2, CPoint pt3)
{
	double slope=0, inter=0 , slope2 =0 , inter2 =0 ;	
	
	double distance = 0 ;


	if(pt1.x == pt2.x)
	{
		slope = 1 ;		
		inter = 0 ;
	}
	else
	{
		slope = (double)(-pt1.y + pt2.y) / (double)(pt1.x - pt2.x) ;
		inter = -pt2.y - slope * pt2.x ;	
	}
	
	distance = fabs(slope * pt3.x + pt3.y + inter) / sqrt(pow(slope, 2) + pow(inter, 0));

//	TRACE(" distance = %.3lf \n", distance) ;
//	distance = fabs(distance) ;

//	TRACE(" x1 [ %d ], y1[%d] ,x2[%d] , y2[%d] , x3 [%d] , y3 [ %d] \n", pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y) ;
//	TRACE(" Slope[%.3lf], Inter[%.3lf] ,distance[%.3lf] \n", slope, inter, distance) ;	

	return distance ;
}
void CRotTracker::OnUpdate(CDC *pDC , UINT nMode, CRect rect)
{
	int i;

	int oldROP2(R2_COPYPEN);
	if (pDC)
	{
		oldROP2 = pDC->SetROP2(R2_NOTXORPEN);		
	}
	
	HRGN hRgn = ::CreateRectRgnIndirect(&rect);
	::SelectClipRgn(pDC->m_hDC , hRgn) ;
	
	CPen pen , *oldPen ;	

	pDC->SetROP2(R2_XORPEN);
	pen.CreatePen(PS_DOT,1,RGB(125,125,125));
//	pen.CreatePen(PS_DOT,1,RGB(255,0,0));
	oldPen=pDC->SelectObject(&pen);
	
	switch(nMode)
	{
		case 0:
		{		
			pDC->MoveTo(m_tmppoly.poly[0].x, m_tmppoly.poly[0].y);    
			for( i = 1; i < 4; i++)
			{        
				pDC->LineTo(m_tmppoly.poly[i].x, m_tmppoly.poly[i].y) ;
			}
			// 다각형을 다 그리고 나서 마무리
			pDC->LineTo(m_tmppoly.poly[0].x, m_tmppoly.poly[0].y) ;
		
			pDC->MoveTo(m_poly.poly[0].x, m_poly.poly[0].y);    
			for( i = 1; i < 4; i++)
			{        
				pDC->LineTo(m_poly.poly[i].x, m_poly.poly[i].y) ;
			}
			pDC->LineTo(m_poly.poly[0].x, m_poly.poly[0].y) ;
			
		}
		break ;

		case 1:
			{
				pDC->MoveTo(m_oldpoly.poly[0].x, m_oldpoly.poly[0].y);    
				for( i = 1; i < 4; i++)
				{        
					pDC->LineTo(m_oldpoly.poly[i].x, m_oldpoly.poly[i].y) ;
				}
				// 다각형을 다 그리고 나서 마무리
				pDC->LineTo(m_oldpoly.poly[0].x, m_oldpoly.poly[0].y) ;
		
				pDC->MoveTo(m_tmppoly.poly[0].x, m_tmppoly.poly[0].y);    
				for( i = 1; i < 4; i++)
				{        
					pDC->LineTo(m_tmppoly.poly[i].x, m_tmppoly.poly[i].y) ;
				}
				// 다각형을 다 그리고 나서 마무리
				pDC->LineTo(m_tmppoly.poly[0].x, m_tmppoly.poly[0].y) ;							

			}
			break;

	}
	
    // 다각형을 다 그리고 나서 마무리 
	pDC->SelectObject(oldPen);
	pen.DeleteObject();	

	if (pDC)
	{
		// Clean up pDC
		pDC->SetROP2(oldROP2);	
	}
	::SelectClipRgn(pDC->m_hDC, NULL) ;

}

BOOL CRotTracker::PolyInside(POINT *point, CPoint pt, int num)
{
	int i, j;
	BOOL result = FALSE;
    for (i = 0, j = num-1; i < num; j = i++) {
        if ((((point[i].y<=pt.y) && (pt.y<point[j].y)) ||
            ((point[j].y<=pt.y) && (pt.y<point[i].y))) &&
            (pt.x < (point[j].x - point[i].x) * (pt.y - point[i].y) / (point[j].y - point[i].y) + point[i].x))
          result = !result;
    }
    return result;
}
