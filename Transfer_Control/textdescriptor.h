#if !defined(AFX_TEXTDESCRIPTOR_H__76F7119B_35DB_41F5_815E_08D447228E72__INCLUDED_)
#define AFX_TEXTDESCRIPTOR_H__76F7119B_35DB_41F5_815E_08D447228E72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CTextDescriptor wrapper class

class CTextDescriptor : public COleDispatchDriver
{
public:
	CTextDescriptor() {}		// Calls COleDispatchDriver default constructor
	CTextDescriptor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTextDescriptor(const CTextDescriptor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	unsigned long GetColorNormal();
	void SetColorNormal(unsigned long);
	unsigned long GetColorMouseOver();
	void SetColorMouseOver(unsigned long);
	unsigned long GetColorPressed();
	void SetColorPressed(unsigned long);
	unsigned long GetColorSolidBack();
	void SetColorSolidBack(unsigned long);
	unsigned long GetColorBorder();
	void SetColorBorder(unsigned long);
	short GetOffsetX();
	void SetOffsetX(short);
	short GetOffsetY();
	void SetOffsetY(short);
	long GetSpecialEffect();
	void SetSpecialEffect(long);
	short GetSpecialEffectFactor();
	void SetSpecialEffectFactor(short);
	short GetAngle();
	void SetAngle(short);
	short GetTransparentFactor();
	void SetTransparentFactor(short);
	BOOL GetAntialias();
	void SetAntialias(BOOL);
	BOOL GetSolidBack();
	void SetSolidBack(BOOL);
	CString GetText();
	void SetText(LPCTSTR);
	BOOL GetVisible();
	void SetVisible(BOOL);
	BOOL GetBorder();
	void SetBorder(BOOL);
	BOOL GetDoubleSpace();
	void SetDoubleSpace(BOOL);
	BOOL GetSolidBackShadow();
	void SetSolidBackShadow(BOOL);
	unsigned long GetColorHighlight();
	void SetColorHighlight(unsigned long);
	unsigned long GetColorShadow();
	void SetColorShadow(unsigned long);
	unsigned long GetColorHalo();
	void SetColorHalo(unsigned long);
	unsigned long GetColorSolidBackShadow();
	void SetColorSolidBackShadow(unsigned long);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTDESCRIPTOR_H__76F7119B_35DB_41F5_815E_08D447228E72__INCLUDED_)
