// PeToolDlg.h : header file
//

#if !defined(AFX_PETOOLDLG_H__EFDE87E2_DB75_4027_A6DC_313F6AE9778F__INCLUDED_)
#define AFX_PETOOLDLG_H__EFDE87E2_DB75_4027_A6DC_313F6AE9778F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PeFile.h"

/////////////////////////////////////////////////////////////////////////////
// CPeToolDlg dialog

class CPeToolDlg : public CDialog
{
private:
  enum TabId
  {
    TAB_PE_FILE = 0,  //Home Page
    TAB_DOS_HDR = 1,  //Dos Header
    TAB_FIL_HDR = 2,  //File Header
    TAB_OPT_HDR = 3,  //Optional Header
    TAB_DAT_DIR = 4,  //Data Directory
    TAB_BLK_TBL = 5,  //Section Table
    TAB_EXP_TBL = 6,  //Export Table
    TAB_IMP_TBL = 7,  //Import Table
    TAB_RLC_TBL = 8,  //Base Relocation Table
    TAB_RES_TBL = 9,  //Resource Table
    TAB_USR_OPR = 10, //User Operation
    NUMBER_OF_TABS
  };

// Construction
public:
	CPeToolDlg(CWnd* pParent = NULL);	// standard constructor

private:
  LPTSTR m_lpCmdLine;
  struct SPePage* m_lpPages[NUMBER_OF_TABS];

private:
  void InitTabCtrl(void);
  void FreeTabCtrl(void);

// Dialog Data
	//{{AFX_DATA(CPeToolDlg)
	enum { IDD = IDD_PE_MAIN_WND };
	CTabCtrl	m_oPeTabCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPeToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabPetool(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnFileForm(WPARAM wParam, LPARAM lParam);
  afx_msg void OnSetTitleText(WPARAM wParam, LPARAM lParam);
  afx_msg void OnUpdateCopyRight(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PETOOLDLG_H__EFDE87E2_DB75_4027_A6DC_313F6AE9778F__INCLUDED_)
