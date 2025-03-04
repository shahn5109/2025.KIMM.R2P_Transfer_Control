// SystemClass.h: interface for the CSystemClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMCLASS_H__E8252D8A_2128_4ECF_8DF3_E53D30B98D77__INCLUDED_)
#define AFX_SYSTEMCLASS_H__E8252D8A_2128_4ECF_8DF3_E53D30B98D77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#include "Comizoa_Class.h"
*/
#include "ParameterFile.h"
#include "ProcessThread_Main.h"
#include "Motion.h"
#include "IO.h"
#include "Matrox.h"

class CSystemClass  
{
public:
	CSystemClass();
	virtual ~CSystemClass();

public:
	int		Initialize();
	void	SystemClose(); // ��� �ý��� ���� Close
	BOOL	IO_SystemInitial();

	void	RunModeChange();
	void	AutoRunErrorProc(CString strRet, int step);
	void	AutoRunEndProc();

//=================================================================================
public:
	CParameterFile*		GetParameterFileComponent();
//=================================================================================
private:
	CParameterFile*		m_pParameterFile;
	int					createParameterFileComponent();
//=================================================================================
public:
	CIO*				GetIO();		// -
	BOOL				IsIOSystemInitial();	// TRUE / FALSE
	BOOL				IOSystemInitial();		// -
private:
	CIO*				m_pIO;
	BOOL				m_bIOIsInitial;			// 
	int					createIOComponent();	// -
	void				IOSystemClose();		// -
//=================================================================================
public:
	CMotion*			GetMotionComponent();		// -
	BOOL				IsMotionSystemInitial();	// TRUE / FALSE
	BOOL				MotionSystemInitial();		// -
private:
	CMotion*			m_pMotion;
	BOOL				m_bMotionIsInitial;			// 
	int					createMotionComponent();	// -
	void				MotionSystemClose();		// -
//=================================================================================
public:
	CMatrox*			GetVisionComponent();
	BOOL				IsVisionSystemInitial();	// ������ ���� �Ϸ� ���� Ȯ��
	BOOL				VisionSystemInitial();		// ������ ���� : �ý��۸� Alloc
private:
	CMatrox*			m_pVision;
	BOOL				m_bVisionIsInitial;			// ������ ���� �Ϸ� ���� Ȯ��
	int					createVisionComponent();
	void				VisionSystemClose();		// ������ ����

//=================================================================================
public:
 	CProcessThread_Main*		m_pProcessThread_Main;
 	int							createProcessThread_MainComponent();
 	CProcessThread_Main*		GetProcessThread_MainComponent();
};

#endif // !defined(AFX_SYSTEMCLASS_H__E8252D8A_2128_4ECF_8DF3_E53D30B98D77__INCLUDED_)
