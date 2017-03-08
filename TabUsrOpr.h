#if !defined(AFX_TABUSROPR_H__6EA49AEF_65E2_41EF_9318_F0895FECB56F__INCLUDED_)
#define AFX_TABUSROPR_H__6EA49AEF_65E2_41EF_9318_F0895FECB56F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabUsrOpr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabUsrOpr dialog

class CTabUsrOpr : public CDialog
{
// Construction
public:
	CTabUsrOpr(CWnd* pParent = NULL);   // standard constructor

private:
  VOID UpdateCheckBox(VOID);
  BOOL UseDefaultSign(VOID);
  VOID EnableSignControl(BOOL bEnable = TRUE);
  BOOL Fill(LPVOID lpParam = NULL);

// Dialog Data
	//{{AFX_DATA(CTabUsrOpr)
	enum { IDD = IDD_DLG_USR_OPR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabUsrOpr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabUsrOpr)
	virtual BOOL OnInitDialog();
  afx_msg void OnFillForm(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheckAddressConvert();
	afx_msg void OnBtnAddressConvert();
	afx_msg void OnBtnSearchImport();
	afx_msg void OnBtnSearchExport();
	afx_msg void OnBtnSoftSign();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABUSROPR_H__6EA49AEF_65E2_41EF_9318_F0895FECB56F__INCLUDED_)
