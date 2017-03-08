// TabDosHdr.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabDosHdr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabDosHdr dialog

CTabDosHdr::CTabDosHdr(CWnd* pParent /*=NULL*/)
	: CDialog(CTabDosHdr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabDosHdr)
	//}}AFX_DATA_INIT
}


void CTabDosHdr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabDosHdr)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabDosHdr, CDialog)
	//{{AFX_MSG_MAP(CTabDosHdr)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabDosHdr::Fill(LPVOID lpParam/* = NULL*/)
{
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_DOS_HEADER lpHdr = NULL;
  LPBYTE tmp = NULL;
  LPWORD w = NULL;
  CHAR strMsg[TEMP_BUFF_SIZE+20];
  
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取DosHeader时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if(NULL == lpFmtPe->lpDosHdr)
  {
    MessageBox("读取的DosHeader无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  lpHdr = lpFmtPe->lpDosHdr;

  //e_magic
  tmp = (LPBYTE)&(lpHdr->e_magic);
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X -> %c%c", lpHdr->e_magic, tmp[0], tmp[1]);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_MAGIC), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //e_cblp
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_cblp);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_CBLP), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_cp
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_cp);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_CP), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_crlc
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_crlc);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_CRLC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_cparhdr
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_cparhdr);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_CPARHDR), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_minalloc
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_minalloc);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_MINALLOC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_maxalloc
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_maxalloc);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_MAXALLOC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_ss
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_ss);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_SS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_sp
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_sp);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_SP), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_csum
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_csum);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_CSUM), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_ip
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_ip);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_IP), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_cs
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_cs);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_CS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //e_lfarlc
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_lfarlc);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_LFARLC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_ovno
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_ovno);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_OVNO), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_res
  w = lpHdr->e_res;
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X %04X %04X %04X", w[0], w[1], w[2], w[3]);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_RES), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //e_oemid
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_oemid);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_OEMID), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_oeminfo
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->e_oeminfo);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_OEMINFO), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_res2
  w = lpHdr->e_res2;
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X", w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9]);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_RES2), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //e_lfanew
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->e_lfanew);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_DOS_LFANEW), WM_SETTEXT, NULL, (LPARAM)strMsg);
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabDosHdr message handlers

void CTabDosHdr::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
