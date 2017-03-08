// TabFilHdr.cpp : implementation file
//

#include "stdafx.h"
#include <time.h>
#include "PeFile.h"
#include "PeToolM.h"
#include "TabFilHdr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabFilHdr dialog

CTabFilHdr::CTabFilHdr(CWnd* pParent /*=NULL*/)
	: CDialog(CTabFilHdr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabFilHdr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabFilHdr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabFilHdr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabFilHdr, CDialog)
	//{{AFX_MSG_MAP(CTabFilHdr)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabFilHdr::Fill(LPVOID lpParam/* = NULL*/)
{
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_NT_HEADERS lpNtHdr = NULL;
  PIMAGE_FILE_HEADER lpHdr = NULL;
  LPBYTE tmp = NULL;
  LPSTR strFileType = NULL;
  CHAR strTime[32];
  CHAR strMsg[TEMP_BUFF_SIZE+2];
  
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取FileHeader时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == lpFmtPe->lpNtHdr) || (NULL == lpFmtPe->lpFilHdr))
  {
    MessageBox("读取的FileHeader无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  lpNtHdr = lpFmtPe->lpNtHdr;
  lpHdr   = lpFmtPe->lpFilHdr;

  //Signature
  tmp = (LPBYTE)&(lpNtHdr->Signature);
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X -> %c%c%c%c", lpNtHdr->Signature, tmp[0], tmp[1], tmp[2], tmp[3]);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_SIGNATURE), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Machine
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X -> %s", lpHdr->Machine, g_lpPeFile->GetMachineString(lpHdr->Machine));
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_MACHINE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //NumberOfSections
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->NumberOfSections);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_NUMBEROFSECTION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //TimeDateStamp
  ZeroMemory(strTime, sizeof(strTime));
  strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", localtime((const time_t*)&(lpHdr->TimeDateStamp)));
  
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X -> %s", lpHdr->TimeDateStamp, strTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_TIMEDATESTAMP), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //PointerToSymbolTable
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->PointerToSymbolTable);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_POINTERTOSYMBOLTABLE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //NumberOfSymbols
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->NumberOfSymbols);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_NUMBEROFSYMBOLS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfOptionalHeader
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X -> %d", lpHdr->SizeOfOptionalHeader, lpHdr->SizeOfOptionalHeader);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_SIZEOFOPTIONALHEADER), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //Characteristics
  ZeroMemory(strMsg, sizeof(strMsg));
  //if(lpHdr->Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE)
  if(lpHdr->Characteristics & IMAGE_FILE_DLL)
  {
    strFileType = "DLL";
  }
  else if(lpHdr->Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
  {
    strFileType = "EXE";
  }
  else if(lpHdr->Characteristics & IMAGE_FILE_SYSTEM)
  {
    strFileType = "SYS";
  }
  else
  {
    strFileType = "UNK";
  }

  _snprintf(strMsg, sizeof(strMsg), " %04X -> %s", lpHdr->Characteristics, strFileType);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_FIL_CHARACTERISTICS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabFilHdr message handlers

void CTabFilHdr::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
