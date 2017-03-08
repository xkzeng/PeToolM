// PeToolM.h : main header file for the PETOOL application
//

#if !defined(AFX_PETOOL_H__4CEA0F8E_F957_48BF_8E54_116564817C92__INCLUDED_)
#define AFX_PETOOL_H__4CEA0F8E_F957_48BF_8E54_116564817C92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPeToolApp:
// See PeToolM.cpp for the implementation of this class
//

class CPeToolApp : public CWinApp
{
public:
	CPeToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPeToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PETOOL_H__4CEA0F8E_F957_48BF_8E54_116564817C92__INCLUDED_)
