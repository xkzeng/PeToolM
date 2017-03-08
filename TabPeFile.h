#if !defined(AFX_TABPEFILE_H__3A1C3C93_0001_457B_85B3_0FF8F1074E99__INCLUDED_)
#define AFX_TABPEFILE_H__3A1C3C93_0001_457B_85B3_0FF8F1074E99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPeFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabPeFile dialog

class CTabPeFile : public CDialog
{
// Construction
public:
	CTabPeFile(CWnd* pParent = NULL);   // standard constructor

private:
  PSYSTEMTIME FileTime2SystemTime(PFILETIME lpFileTime);
  WORD FormatFileTime(LPSTR strBuf, WORD wBufSize, PFILETIME lpFileTime);
  WORD FormatFileAttr(LPSTR strBuf, WORD wBufSize, DWORD dwFileAttributes);
  BOOL ParseFeFile(LPTSTR strFileName);
  VOID ShowDefaultCopyRight(VOID);
  BOOL ShowCopyRight(VOID);

// Dialog Data
	//{{AFX_DATA(CTabPeFile)
	enum { IDD = IDD_DLG_PE_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPeFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabPeFile)
	afx_msg void OnBtnOpenFile();
  afx_msg void OnParseCommandLine(WPARAM wParam, LPARAM lParam);
  afx_msg void OnUpdateCopyRight(WPARAM wParam, LPARAM lParam);
  virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropFile);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPEFILE_H__3A1C3C93_0001_457B_85B3_0FF8F1074E99__INCLUDED_)
