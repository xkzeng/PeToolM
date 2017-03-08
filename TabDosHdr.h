#if !defined(AFX_TABDOSHDR_H__61C705BE_4CE4_4357_A9DA_7C99957DD726__INCLUDED_)
#define AFX_TABDOSHDR_H__61C705BE_4CE4_4357_A9DA_7C99957DD726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDosHdr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabDosHdr dialog

class CTabDosHdr : public CDialog
{
// Construction
public:
	CTabDosHdr(CWnd* pParent = NULL);   // standard constructor

private:
	BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabDosHdr)
	enum { IDD = IDD_DLG_DOS_HDR };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabDosHdr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabDosHdr)
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDOSHDR_H__61C705BE_4CE4_4357_A9DA_7C99957DD726__INCLUDED_)
