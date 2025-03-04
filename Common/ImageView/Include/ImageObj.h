#ifndef _IMAGE_OBJECT_H_
#define _IMAGE_OBJECT_H_

#ifdef IMAGEOBJ_EXPORTS
#define IMAGE_OBJECT_EXPORTS	__declspec(dllexport)
#else
#define IMAGE_OBJECT_EXPORTS __declspec(dllimport)
#endif

#include <wtypes.h>
#include <tchar.h>

typedef void (APIENTRY* FnOnImageProgress)( int nProgress );

#if _MSC_VER > 1000
#pragma once
#endif

struct _IplImage;
class IMAGE_OBJECT_EXPORTS CImageObj 
{
public:
	CImageObj();
	virtual ~CImageObj();

protected:
	struct _IplImage*	m_plplImage;
	BOOL					m_bDelete;
	CRITICAL_SECTION	m_CsLock;
	BOOL					m_bNotifyChangeImg;
	FnOnImageProgress	m_fnOnImageProgress;
	HBITMAP				m_hBitmap;

public:
	static	void CDECL	_OnProgress( int nProgress, LPVOID lpUserData );
	virtual void OnProgress( int nProgress ) {}
	FnOnImageProgress SetOnImageProgress( FnOnImageProgress _fnOnImageProgress );

	BOOL Create( UINT nWidth, UINT nHeight, UINT nBpp, UINT nOrigin = 0 );
	void Destroy();
	
	BOOL isValid() { return m_plplImage == NULL ? FALSE : TRUE; }
	struct _IplImage* GetImage() const;
	UINT GetHeight() const;
	UINT GetWidth() const;
	UINT GetBpp() const;
	static int GetWidthBytes( int nCx, int nBitCount );
	int GetWidthBytes() const;
	LPVOID GetImageBuffer() const;

	void AttachHBitmap( HBITMAP hBitmap );
	void DetachHBitmap();

	void ClearNotify();
	BOOL isNotify();

	BOOL LoadFromFile( LPCTSTR lpszFileName, BOOL bForceGray = FALSE );
	BOOL SaveImage( LPCTSTR lpszFileName );
	BOOL CopyImageToThis( const CImageObj* pDstImg );
	BOOL CreateFromBuffer( LPVOID lpImgBuffer, UINT nWidth, UINT nHeight, UINT nBpp );

	BYTE GetPixelValue( UINT x, UINT y ) const;
	COLORREF GetPixelColor( UINT x, UINT y ) const;
};

#endif