#if !defined(AFX_TABBLKTBL_H__9047767B_50C2_4913_9FA7_9F7337B52C1B__INCLUDED_)
#define AFX_TABBLKTBL_H__9047767B_50C2_4913_9FA7_9F7337B52C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabBlkTbl.h : header file
//
#include "TabDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTabBlkTbl dialog

class CTabBlkTbl : public TabDialog
{
// Construction
public:
	CTabBlkTbl(CWnd* pParent = NULL);   // standard constructor

private:
  BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabBlkTbl)
	enum { IDD = IDD_DLG_BLK_TBL };
	CListCtrl	m_oLvBlkTbl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabBlkTbl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabBlkTbl)
	virtual BOOL OnInitDialog();
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBLKTBL_H__9047767B_50C2_4913_9FA7_9F7337B52C1B__INCLUDED_)
