#if !defined(AFX_TABOPTHDR_H__CA23231C_4B20_4E9A_BE08_7D7BD89BA93C__INCLUDED_)
#define AFX_TABOPTHDR_H__CA23231C_4B20_4E9A_BE08_7D7BD89BA93C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabOptHdr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabOptHdr dialog

class CTabOptHdr : public CDialog
{
// Construction
public:
	CTabOptHdr(CWnd* pParent = NULL);   // standard constructor

private:
  BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabOptHdr)
	enum { IDD = IDD_DLG_OPT_HDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabOptHdr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabOptHdr)
		// NOTE: the ClassWizard will add member functions here
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABOPTHDR_H__CA23231C_4B20_4E9A_BE08_7D7BD89BA93C__INCLUDED_)
