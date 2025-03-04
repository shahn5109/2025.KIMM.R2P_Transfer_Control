#ifndef __TRACK_INFO_H__
#define __TRACK_INFO_H__
typedef struct _TRACK_INFO
{
	POINT poly[5] ;
	POINT center ;	 
	POINT endpt ;
	CRect	lt ,
			rt, 
			lb ,
			rb ,
			top ,
			left,
			right ,
			bottom,
			end;
	double angle ;
	double r ;
	CRect rect ;
	bool m_bDraw ;
}Tracker_info ; 
#endif