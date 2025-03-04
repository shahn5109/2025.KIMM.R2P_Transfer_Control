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

#include "xCriticalSection.h"

struct _IplImage;
class IMAGE_OBJECT_EXPORTS CImageObj 
{
protected:
	struct _IplImage*	m_plplImage;
	BOOL					m_bDelete;
	CRITICAL_SECTION	m_CsLock;
	BOOL					m_bNotifyChangeImg;
	FnOnImageProgress	m_fnOnImageProgress;

	CxCriticalSection*	m_pCsLockImage;

	HBITMAP			m_hBitmap;

public:
	CImageObj();
	virtual ~CImageObj();

	static	void CDECL	_OnProgress( int nProgress, LPVOID lpUserData );
	virtual void OnProgress( int nProgress ) {}
	FnOnImageProgress SetOnImageProgress( FnOnImageProgress _fnOnImageProgress );

	// HBITMAP handling
	void AttachHBitmap( HBITMAP hBitmap );
	void DetachHBitmap();
	BOOL IsHBitmapAttached();
	// Validation
	BOOL IsValid() const;

	// Construction
	BOOL CreateFromBuffer( LPVOID lpImgBuffer, UINT nWidth, UINT nHeight, UINT nBpp );
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

	void ClearNotify();
	BOOL isNotify();

	BOOL LoadFromFile( LPCTSTR lpszFileName, BOOL bForceGray = FALSE );
	BOOL SaveImage( LPCTSTR lpszFileName );
	BOOL CopyImageToThis( const CImageObj* pDstImg );


	BYTE GetPixelValue( UINT x, UINT y ) const;
	COLORREF GetPixelColor( UINT x, UINT y ) const;
	
	CImageObj(const CImageObj &rhs);
	CImageObj &operator=(const CImageObj &rhs);

	CxCriticalSection*	GetImageLockObject();
};

#endif