#if !defined(AFX_TABRESTBL_H__56761FB7_8EC8_4EA2_A261_E716C255DEE5__INCLUDED_)
#define AFX_TABRESTBL_H__56761FB7_8EC8_4EA2_A261_E716C255DEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabResTbl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabResTbl dialog

class CTabResTbl : public CDialog
{
// Construction
public:
	CTabResTbl(CWnd* pParent = NULL);   // standard constructor

private:
  LPBYTE m_strResTbl;

private:
  VOID CreateTreeView(HTREEITEM hParent, WORD wDepth, LPSTR strLvlStr, WORD wCount);
  VOID InitTreeView(VOID);
  LPCTSTR Type2Name(WORD wTypeId);
  BOOL ParseResourceDirectory(PIMAGE_RESOURCE_DIRECTORY lpResDir, HTREEITEM hParent, WORD wDepth);
  BOOL Fill(LPVOID lpParam = NULL);
  BOOL ReadResourceData(PIMAGE_RESOURCE_DATA_ENTRY lpResDataEntry);

// Dialog Data
	//{{AFX_DATA(CTabResTbl)
	enum { IDD = IDD_DLG_RES_TBL };
	CTreeCtrl	m_oTvResTbl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabResTbl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabResTbl)
	virtual BOOL OnInitDialog();
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangedTreeResourceTable(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRESTBL_H__56761FB7_8EC8_4EA2_A261_E716C255DEE5__INCLUDED_)
