// PeToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>
#include "PeFile.h"
#include "PeToolM.h"
#include "PeToolDlg.h"
#include "TabPeFile.h"
#include "TabDosHdr.h"
#include "TabFilHdr.h"
#include "TabOptHdr.h"
#include "TabDatDir.h"
#include "TabBlkTbl.h"
#include "TabImpTbl.h"
#include "TabExpTbl.h"
#include "TabRlcTbl.h"
#include "TabResTbl.h"
#include "TabUsrOpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeToolDlg dialog

CPeToolDlg::CPeToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPeToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN_WND);
  this->m_lpCmdLine = NULL;
  this->m_lpCmdLine = AfxGetApp()->m_lpCmdLine;
  ZeroMemory((PVOID )(this->m_lpPages), sizeof(this->m_lpPages));
}

void CPeToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeToolDlg)
	DDX_Control(pDX, IDC_TAB_PETOOL, m_oPeTabCtrl);
	//}}AFX_DATA_MAP
}

void CPeToolDlg::InitTabCtrl(void)
{
  WORD i = 0;
  struct SPePage* lpPePage = NULL;
  CDialog* lpTabDlg = NULL;
  BOOL bRet = FALSE;
  
  for(i = 0; i < NUMBER_OF_TABS; i++)
  {
    this->m_lpPages[i] = new struct SPePage;
  }
  
  this->m_lpPages[TAB_PE_FILE]->strTitle = TEXT("文  件");
  this->m_lpPages[TAB_PE_FILE]->uiIDD = IDD_DLG_PE_FILE;
  //this->m_lpPages[TAB_PE_FILE]->lpTabDlg = new CTabPeFile(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_PE_FILE]->lpTabDlg = new CTabPeFile(this);

  this->m_lpPages[TAB_DOS_HDR]->strTitle = TEXT("DOS 头");
  this->m_lpPages[TAB_DOS_HDR]->uiIDD = IDD_DLG_DOS_HDR;
  //this->m_lpPages[TAB_DOS_HDR]->lpTabDlg = new CTabDosHdr(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_DOS_HDR]->lpTabDlg = new CTabDosHdr(this);

  this->m_lpPages[TAB_FIL_HDR]->strTitle = TEXT("文件头");
  this->m_lpPages[TAB_FIL_HDR]->uiIDD = IDD_DLG_FIL_HDR;
  //this->m_lpPages[TAB_FIL_HDR]->lpTabDlg = new CTabFilHdr(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_FIL_HDR]->lpTabDlg = new CTabFilHdr(this);

  this->m_lpPages[TAB_OPT_HDR]->strTitle = TEXT("可选头");
  this->m_lpPages[TAB_OPT_HDR]->uiIDD = IDD_DLG_OPT_HDR;
  //this->m_lpPages[TAB_OPT_HDR]->lpTabDlg = new CTabOptHdr(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_OPT_HDR]->lpTabDlg = new CTabOptHdr(this);

  this->m_lpPages[TAB_DAT_DIR]->strTitle = TEXT("数据目录");
  this->m_lpPages[TAB_DAT_DIR]->uiIDD = IDD_DLG_DAT_DIR;
  //this->m_lpPages[TAB_DAT_DIR]->lpTabDlg = new CTabDatDir(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_DAT_DIR]->lpTabDlg = new CTabDatDir(this);

  this->m_lpPages[TAB_BLK_TBL]->strTitle = TEXT("节  表");
  this->m_lpPages[TAB_BLK_TBL]->uiIDD = IDD_DLG_BLK_TBL;
  //this->m_lpPages[TAB_BLK_TBL]->lpTabDlg = new CTabBlkTbl(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_BLK_TBL]->lpTabDlg = new CTabBlkTbl(this);

  this->m_lpPages[TAB_EXP_TBL]->strTitle = TEXT("导出表");
  this->m_lpPages[TAB_EXP_TBL]->uiIDD = IDD_DLG_EXP_TBL;
  //this->m_lpPages[TAB_EXP_TBL]->lpTabDlg = new CTabExpTbl(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_EXP_TBL]->lpTabDlg = new CTabExpTbl(this);

  this->m_lpPages[TAB_IMP_TBL]->strTitle = TEXT("导入表");
  this->m_lpPages[TAB_IMP_TBL]->uiIDD = IDD_DLG_IMP_TBL;
  //this->m_lpPages[TAB_IMP_TBL]->lpTabDlg = new CTabImpTbl(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_IMP_TBL]->lpTabDlg = new CTabImpTbl(this);

  this->m_lpPages[TAB_RLC_TBL]->strTitle = TEXT("重定位表");
  this->m_lpPages[TAB_RLC_TBL]->uiIDD = IDD_DLG_RLC_TBL;
  //this->m_lpPages[TAB_RLC_TBL]->lpTabDlg = new CTabRlcTbl(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_RLC_TBL]->lpTabDlg = new CTabRlcTbl(this);

  this->m_lpPages[TAB_RES_TBL]->strTitle = TEXT("资源表");
  this->m_lpPages[TAB_RES_TBL]->uiIDD = IDD_DLG_RES_TBL;
  //this->m_lpPages[TAB_RES_TBL]->lpTabDlg = new CTabResTbl(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_RES_TBL]->lpTabDlg = new CTabResTbl(this);

  this->m_lpPages[TAB_USR_OPR]->strTitle = TEXT("操  作");
  this->m_lpPages[TAB_USR_OPR]->uiIDD = IDD_DLG_USR_OPR;
  //this->m_lpPages[TAB_USR_OPR]->lpTabDlg = new CTabUsrOpr(&(this->m_oPeTabCtrl));
  this->m_lpPages[TAB_USR_OPR]->lpTabDlg = new CTabUsrOpr(this);

  g_lpPages = (struct SPePage*)(this->m_lpPages);

  //Initialize all pages
  CRect rs;
  this->m_oPeTabCtrl.GetClientRect(&rs);
  rs.top += 20;
  rs.bottom -= 2;
  rs.left += 2;
  rs.right -= 2;
  
  for(i = 0; i < NUMBER_OF_TABS; i++)
  {
    lpPePage = this->m_lpPages[i];
    if(lpPePage)
    {
      lpTabDlg = lpPePage->lpTabDlg;
      if(lpTabDlg)
      {
        //bRet = lpTabDlg->Create(lpPePage->uiIDD, &(this->m_oPeTabCtrl));
        bRet = lpTabDlg->Create(lpPePage->uiIDD, GetDlgItem(IDC_TAB_PETOOL));
        lpTabDlg->MoveWindow(&rs);
        lpTabDlg->ShowWindow(TAB_PE_FILE == i);
      }
      this->m_oPeTabCtrl.InsertItem(i, lpPePage->strTitle);
    }
  }

  this->m_oPeTabCtrl.SetCurSel(TAB_PE_FILE);
}

void CPeToolDlg::FreeTabCtrl(void)
{
  WORD i = 0;
  struct SPePage* lpPePage = NULL;
  CDialog* lpTabDlg = NULL;

  for(i = 0; i < NUMBER_OF_TABS; i++)
  {
    lpPePage = this->m_lpPages[i];
    if(lpPePage)
    {
      lpTabDlg = lpPePage->lpTabDlg;
      if(lpTabDlg)
      {
        lpTabDlg->DestroyWindow();
        delete lpTabDlg;
        lpTabDlg = NULL;
        lpPePage->lpTabDlg = NULL;
      }
      
      delete lpPePage;
      lpPePage = NULL;
      this->m_lpPages[i] = NULL;
    }
  }
}

BEGIN_MESSAGE_MAP(CPeToolDlg, CDialog)
	//{{AFX_MSG_MAP(CPeToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PETOOL, OnSelchangeTabPetool)
  ON_MESSAGE(UDM_FILLFORM, OnFileForm)
  ON_MESSAGE(UDM_SET_TITLE_TEXT, OnSetTitleText)
  ON_MESSAGE(UDM_CR_UPDATE, OnUpdateCopyRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeToolDlg message handlers

BOOL CPeToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
  g_lpPeFile = NULL;
  g_lpPeFile = CPeFile::Make();
  if(NULL == g_lpPeFile)
  {
    MessageBox(TEXT("Failed to get memory for pe file!!!"), TEXT("ERROT"), MB_OK | MB_ICONERROR);
    return FALSE;
  }

  this->InitTabCtrl();

  if(this->m_lpCmdLine && this->m_lpCmdLine[0])
  {
    if(this->m_lpPages[TAB_PE_FILE] && this->m_lpPages[TAB_PE_FILE]->lpTabDlg)
    {
      LRESULT lResult = -1;
      lResult = this->m_lpPages[TAB_PE_FILE]->lpTabDlg->SendMessage(WM_PARSECOMMANDLINE, 0, (LPARAM)(this->m_lpCmdLine));
      if(lResult <= 0)
      {
        MessageBox("命令行参数解析失败", "错误", MB_OK | MB_ICONERROR);
        return FALSE;
      }
    }
  }
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPeToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPeToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPeToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPeToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->FreeTabCtrl();

  CPeFile::Free();
  g_lpPeFile = NULL;
}

/*
void CPeToolDlg::OnSelchangeTabPetool(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	WORD i = 0, wSelCur = 0;
  struct SPePage* lpPePage = NULL;
  CDialog* lpTabDlg = NULL;

  wSelCur = this->m_oPeTabCtrl.GetCurSel();
  for(i = 0; i < NUMBER_OF_TAB; i++)
  {
    lpPePage = this->m_lpPages[i];
    if(i == wSelCur)
    {
      if(lpPePage)
      {
        lpTabDlg = lpPePage->lpTabDlg;
        if(lpTabDlg)
        {
          lpTabDlg->ShowWindow(TRUE);
        }
      }
    }
    else
    {
      if(lpPePage)
      {
        lpTabDlg = lpPePage->lpTabDlg;
        if(lpTabDlg)
        {
          lpTabDlg->ShowWindow(FALSE);
        }
      }
    }
  }

	*pResult = 0;
}
*/

void CPeToolDlg::OnSelchangeTabPetool(NMHDR* pNMHDR, LRESULT* pResult) 
{
  // TODO: Add your control notification handler code here
  WORD i = 0, wSelCur = 0;
  struct SPePage* lpPePage = NULL;
  CDialog* lpTabDlg = NULL;
  
  wSelCur = this->m_oPeTabCtrl.GetCurSel();
  for(i = 0; i < NUMBER_OF_TABS; i++)
  {
    lpPePage = this->m_lpPages[i];
    if(lpPePage)
    {
      lpTabDlg = lpPePage->lpTabDlg;
      if(lpTabDlg)
      {
        lpTabDlg->ShowWindow(i == wSelCur);
      }
    }
  }
  
  *pResult = 0;
}

//收到通知消息之后,再通知其它子窗口,进行填充界面;
void CPeToolDlg::OnFileForm(WPARAM wParam, LPARAM lParam)
{
  WORD i = 0;
  struct SPePage* lpPePage = NULL;
  CDialog* lpTabDlg = NULL;
  
  for(i = 0; i < NUMBER_OF_TABS; i++)
  {
    lpPePage = this->m_lpPages[i];
    if(lpPePage)
    {
      lpTabDlg = lpPePage->lpTabDlg;
      if(lpTabDlg)
      {
        lpTabDlg->PostMessage(UDM_FILLFORM, 0, 0);
      }
    }
  }
}

void CPeToolDlg::OnSetTitleText(WPARAM wParam, LPARAM lParam)
{
  DWORD i = 0, dwLen = 0;
  LRESULT lLen = 0;
  LPSTR pos = NULL;
  CHAR strTitle[TEMP_BUFF_SIZE+88];
  
  ZeroMemory(strTitle, sizeof(strTitle));
  
  dwLen = GetModuleFileName(NULL, strTitle, sizeof(strTitle));
  if(0 == dwLen)
  {
    ZeroMemory(strTitle, sizeof(strTitle));
    lLen = ::SendMessage(this->m_hWnd, WM_GETTEXT, (WPARAM)sizeof(strTitle), (LPARAM)strTitle);
    dwLen = (DWORD)lLen;
  }
  
  pos = strTitle + dwLen;
  while(('\\' != (*pos)) && (pos != strTitle))
  {
    pos--;
  }
  
  if('\\' == (*pos)) //找到'\'
  {
    //skip '\'
    pos++;
    
    //copy file name
    i = 0;
    for(i = 0; 0 != (*pos); pos++, i++)
    {
      strTitle[i] = *pos;
    }
  }
  else //没有找到'\'
  {
    i = dwLen;
  }
  
  ZeroMemory(strTitle + i, sizeof(strTitle) - i);
  _snprintf(strTitle + i, sizeof(strTitle) - i, " -> %s", (PSTR)lParam);
  ::SendMessage(this->m_hWnd, WM_SETTEXT, 0, (LPARAM)strTitle);
}

void CPeToolDlg::OnUpdateCopyRight(WPARAM wParam, LPARAM lParam)
{
  if(this->m_lpPages[TAB_PE_FILE] && this->m_lpPages[TAB_PE_FILE]->lpTabDlg)
  {
    LRESULT lResult = -1;
    lResult = this->m_lpPages[TAB_PE_FILE]->lpTabDlg->SendMessage(UDM_CR_UPDATE, 0, 0);
  }
}
