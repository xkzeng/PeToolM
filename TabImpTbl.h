#if !defined(AFX_TABIMPTBL_H__711AF7C0_A357_436D_84A2_2EAE4A6354C7__INCLUDED_)
#define AFX_TABIMPTBL_H__711AF7C0_A357_436D_84A2_2EAE4A6354C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabImpTbl.h : header file
//
#include "TabDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTabImpTbl dialog

class CTabImpTbl : public TabDialog
{
// Construction
public:
	CTabImpTbl(CWnd* pParent = NULL);   // standard constructor

private:
  BOOL Fill(LPVOID lpParam = NULL);
  BOOL FillDetail(LPNM_LISTVIEW lpLv);

private:
  int m_iRowSelectedLast;
  BOOL m_bImpTblIsEmpty;

// Dialog Data
	//{{AFX_DATA(CTabImpTbl)
	enum { IDD = IDD_DLG_IMP_TBL };
	CListCtrl	m_oLvImpDtl;
	CListCtrl	m_oLvImpTbl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabImpTbl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabImpTbl)
	virtual BOOL OnInitDialog();
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	afx_msg void OnItemchangedListImportTable(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABIMPTBL_H__711AF7C0_A357_436D_84A2_2EAE4A6354C7__INCLUDED_)
