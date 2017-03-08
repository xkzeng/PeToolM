#if !defined(AFX_TABDATDIR_H__A59FEC03_F0B8_4CED_882D_89F8A278D057__INCLUDED_)
#define AFX_TABDATDIR_H__A59FEC03_F0B8_4CED_882D_89F8A278D057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDatDir.h : header file
//
#include "TabDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTabDatDir dialog

class CTabDatDir : public TabDialog
{
// Construction
public:
	CTabDatDir(CWnd* pParent = NULL);   // standard constructor

private:
  BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabDatDir)
	enum { IDD = IDD_DLG_DAT_DIR };
	CListCtrl	m_oLvDatDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabDatDir)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabDatDir)
	virtual BOOL OnInitDialog();
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDATDIR_H__A59FEC03_F0B8_4CED_882D_89F8A278D057__INCLUDED_)
