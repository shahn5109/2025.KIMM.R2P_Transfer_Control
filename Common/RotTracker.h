#pragma once
class CRotTracker:public CRectTracker
{
public:
	CRotTracker(void);
	CRotTracker(CWnd *pWnd1 , CWnd *pWnd2);
	~CRotTracker(void);
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


public:
	double angle ;
	Tracker_info m_poly, m_oldpoly , m_tmppoly ;	
	bool m_bCreate ;
	Mode m_nMode ;	

public :
	void ConnectTracker(CWnd *parent , CWnd *disp) ;
//	void DrawTracker(bool repaint = true) ;
	void initVariables() ;	
	void TrackRubberBandSet() ;
	void EraseTracker() ;
	int BeginTrack(int HitFlag) ;
	int EndTrack() ;
	int Track(CPoint pt , int HitFlag) ;
	int Hit_Test(CPoint pt) ;
	int OnMouseMessage(UINT msg, int HitFlag, CPoint pt);
	void OnUpdate(CDC * pDC, UINT nMode, CRect rect) ;
	void SetPolyGon() ;
	void SetTrackPoint() ;	
	



	void ShiftPolygon(CPoint pt) ;
	void ScalePolygon(int HitFlag , CPoint pt) ;
	void ScalePolygon1(int HitFlag , CPoint pt) ;

	void RotatePolygon(CPoint pt) ;
	void RotateRect(Tracker_info *info);
	void RotateRect1(Tracker_info *info, POINT center);
	CRect GetRect(POINT *poly) ;
	

//	void DrawRect(CDC *pDC,bool repaint = true) ;
//	void DrawRectPoint(CDC *pDC ,bool repaint = true) ;
//	void DrawArrow(CDC *pDC, bool repaint = true) ;
	void RotatePoint(long centx , long centy, long *ptx, long *pty,double angle) ;
	void RotatePoint1(long centx , long centy, long *ptx, long *pty,double angle) ;
	double NodeLength(POINT pt1, POINT pt2, CPoint pt3);
	double CalculateAngle(CPoint pt) ;
	void InitTrackerInfo(Tracker_info *info) ;
	BOOL PolyInside(POINT *point, CPoint pt, int num=4) ;
	CWnd *parent , *disp ;
	CPoint m_offset ;
	bool m_bTrackStart ;
	POINT center, end, oldcenter ;
private:
	

};
