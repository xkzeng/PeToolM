// TabOptHdr.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabOptHdr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabOptHdr dialog

CTabOptHdr::CTabOptHdr(CWnd* pParent /*=NULL*/)
	: CDialog(CTabOptHdr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabOptHdr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabOptHdr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabOptHdr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabOptHdr, CDialog)
	//{{AFX_MSG_MAP(CTabOptHdr)
		// NOTE: the ClassWizard will add message map macros here
    ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabOptHdr::Fill(LPVOID lpParam/* = NULL*/)
{
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_OPTIONAL_HEADER lpHdr = NULL;
  CHAR strMsg[TEMP_BUFF_SIZE];
  
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取OptionalHeader时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  if(NULL == lpFmtPe->lpOptHdr)
  {
    MessageBox("读取的OptionalHeader无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  lpHdr = lpFmtPe->lpOptHdr;
  
  //Magic
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->Magic);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MAGIC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MajorLinkerVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %02X", lpHdr->MajorLinkerVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MAJORLINKERVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MinorLinkerVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %02X", lpHdr->MinorLinkerVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MINORLINKERVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfCode
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfCode);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFCODE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfInitializedData
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfInitializedData);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFINITIALIZEDDATA), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfUninitializedData
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfUninitializedData);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFUNINITIALIZEDDATA), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //AddressOfEntryPoint
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->AddressOfEntryPoint);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_ADDRESSOFENTRYPOINT), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //BaseOfCode
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->BaseOfCode);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_BASEOFCODE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //BaseOfData
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->BaseOfData);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_BASEOFDATA), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //ImageBase
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->ImageBase);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_IMAGEBASE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SectionAlignment
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SectionAlignment);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SECTIONALIGNMENT), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //FileAlignment
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->FileAlignment);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_FILEALIGNMENT), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MajorOperatingSystemVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->MajorOperatingSystemVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MAJOROPERATINGSYSTEMVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MinorOperatingSystemVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->MinorOperatingSystemVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MINOROPERATINGSYSTEMVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MajorImageVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->MajorImageVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MAJORIMAGEVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MinorImageVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->MinorImageVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MINORIMAGEVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MajorSubsystemVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->MajorSubsystemVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MAJORSUBSYSTEMVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MinorSubsystemVersion
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->MinorSubsystemVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_MINORSUBSYSTEMVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //Win32VersionValue
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->Win32VersionValue);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_WIN32VERSIONVALUE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfImage
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfImage);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFIMAGE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfHeaders
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfHeaders);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFHEADERS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //CheckSum
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->CheckSum);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_CHECKSUM), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //Subsystem
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X -> %s", lpHdr->Subsystem, g_lpPeFile->GetSubsystemString(lpHdr->Subsystem, lpHdr->Magic));
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SUBSYSTEM), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //DllCharacteristics
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpHdr->DllCharacteristics);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_DLLCHARACTERISTICS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfStackReserve
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfStackReserve);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFSTACKRESERVE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfStackCommit
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfStackCommit);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFSTACKCOMMIT), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfHeapReserve
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfHeapReserve);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFHEAPRESERVE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfHeapCommit
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->SizeOfHeapCommit);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_SIZEOFHEAPCOMMIT), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //LoaderFlags
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->LoaderFlags);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_LOADERFLAGS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //NumberOfRvaAndSizes
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpHdr->NumberOfRvaAndSizes);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_NUMBEROFRVAANDSIZES), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //DataDirectory
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %s", "NEXT");
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_OPT_DATA_DIR), WM_SETTEXT, NULL, (LPARAM)strMsg);
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabOptHdr message handlers

void CTabOptHdr::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
