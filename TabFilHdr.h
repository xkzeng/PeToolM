#if !defined(AFX_TABFILHDR_H__6128D56C_327B_498C_AB85_E5503640FE8E__INCLUDED_)
#define AFX_TABFILHDR_H__6128D56C_327B_498C_AB85_E5503640FE8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabFilHdr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabFilHdr dialog

class CTabFilHdr : public CDialog
{
// Construction
public:
	CTabFilHdr(CWnd* pParent = NULL);   // standard constructor

private:
  BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabFilHdr)
	enum { IDD = IDD_DLG_FIL_HDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabFilHdr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabFilHdr)
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABFILHDR_H__6128D56C_327B_498C_AB85_E5503640FE8E__INCLUDED_)
