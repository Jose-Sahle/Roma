// Roma.h : main header file for the ROMA application
//

#if !defined(AFX_ROMA_H__B8A67623_65ED_4DD4_972D_1717F54C28AC__INCLUDED_)
#define AFX_ROMA_H__B8A67623_65ED_4DD4_972D_1717F54C28AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRomaApp:
// See Roma.cpp for the implementation of this class
//

class CRomaApp : public CWinApp
{
public:
	CRomaApp();
	~CRomaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRomaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRomaApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROMA_H__B8A67623_65ED_4DD4_972D_1717F54C28AC__INCLUDED_)
