#if !defined(AFX_TABRLCTBL_H__EEC92EBD_AD2C_443B_92CE_87733BB85C6C__INCLUDED_)
#define AFX_TABRLCTBL_H__EEC92EBD_AD2C_443B_92CE_87733BB85C6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRlcTbl.h : header file
//
#include "TabDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTabRlcTbl dialog

class CTabRlcTbl : public TabDialog
{
// Construction
public:
	CTabRlcTbl(CWnd* pParent = NULL);   // standard constructor

private:
  BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabRlcTbl)
	enum { IDD = IDD_DLG_RLC_TBL };
	CListCtrl	m_oLvRlcTbl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRlcTbl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabRlcTbl)
	virtual BOOL OnInitDialog();
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRLCTBL_H__EEC92EBD_AD2C_443B_92CE_87733BB85C6C__INCLUDED_)
